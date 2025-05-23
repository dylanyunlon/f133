/*
 * context.cpp
 *
 *  Created on: 2023年11月14日
 *      Author: ZKSWE Develop Team
 */

#include <fcntl.h>
#include <termio.h>
#include <poll.h>
#include <errno.h>
#include "fy/thread.hpp"
#include "fy/os.hpp"
#include "fy/format.hpp"
#include "context.h"
#include "UartContext.h"
#include "common.h"
#include "system/Mutex.h"
#include <string>
#include <set>
#include <unistd.h>
#include <stdint.h>
#include "utils/log.h"
#include <string.h>
#include "utils.hpp"
#include "link/lylinkapi.h"
#include "system/setting.h"
#include "tire/tire_parse.h"


#define DEBUG	LOGD("--%d-- --%s-- DEBUG! \n", __LINE__, __FILE__);

#define LOCK_OP(lock, op)		{ Mutex::Autolock _l(lock); op;}

namespace uart {

#define DEBUG_UART        0
#define BUFF_DATA_SIZE    2048
#define BT_RF_SERIAL      "/dev/rf_serial"

// FF 55 cmd len(1~2Bytes) data(nBytes) sum(2Bytes)
#define FRAME_HEAD1       0xFF
#define FRAME_HEAD2       0x55
#define FRAME_MIN_LEN     6

const char upd_filename[] = "/mnt/extsd/fw5000.upd";     //"/res/ui/fw5000_vc.upd";
const std::string str_start_sign = "START_UPD^_^";
const std::string str_recive_success = "RECEIVESTART";

typedef enum {
	E_SERIAL_TYPE_UART,   // 蓝牙物理串口
	E_SERIAL_TYPE_GOC,    // 蓝牙虚拟串口，应用使用
	E_SERIAL_TYPE_RF,     // 蓝牙虚拟串口，辽原互联使用
	E_SERIAL_TYPE_MAX,
} serial_type_e;

typedef struct {
	const char *dev;
	uint32_t baudrate;
	int fd;
	uint8_t data[BUFF_DATA_SIZE];
	int data_len;
	int (*proc)(serial_type_e type, const uint8_t *data, int len);
} serial_t;

typedef struct {
	sound_channel_e channel = SOUND_CHANNEL_BT;
	uint8_t volume = 25;	//所有通道使用同一个音量
	uint8_t mute = false;
	Mutex _s_vol_mutex;
} sys_voice_s;

mcu_time_t time_info;
// (0~24) 显示 -10~10 dB
static uint8_t _arr_eq[] = {0xc, 0xc, 0xc, 0xc, 0xc, 0xc, 0xc, 0xc, 0xc, 0xc};
static uint8_t _arr_field[] = {0xa, 0xa, 0xa, 0xa};

static int _proc_uart_data(serial_type_e type, const uint8_t *data, int len);
static int _proc_bt_data(serial_type_e type, const uint8_t *data, int len);

static serial_t _s_serial_tab[E_SERIAL_TYPE_MAX] = {
	{ BT_UART_DEV, B115200, -1, { 0 }, 0, _proc_uart_data },
	{ BT_SERIAL, 0, -1, { 0 }, 0, _proc_bt_data },
	{ BT_RF_SERIAL, 0, -1, { 0 }, 0, _proc_bt_data },
};


static upd_uart_m_t upd_uart_m;
static int _s_mcu_update_fd = -1;

static sys_voice_s sys_voice;


static std::set<_s_mcu_cb> _s_mcu_cb_set;
static std::set<fm_state_cb> _s_fm_state_set;
static std::set<fm_fre_cb> _s_fm_fre_set;

static Mutex _s_mcu_cb_set_lock;
static Mutex _s_uart_lock;

static void _notify_fm_state(bool state) {
	Mutex::Autolock _l(_s_mcu_cb_set_lock);
	for(auto it : _s_fm_state_set)
	{
		it(state);
	}
}

static void _notify_fm_fre(int fre) {
	Mutex::Autolock _l(_s_mcu_cb_set_lock);
	for(auto it : _s_fm_fre_set)
	{
		it(fre);
	}
}

static void _notify_scan_cb(MCU_EVENT event, void* data, uint32_t len) {
	Mutex::Autolock _l(_s_mcu_cb_set_lock);
	for (auto it : _s_mcu_cb_set) {
		(*it)(event, data, len);
	}
}

static std::string channel_to_str(sound_channel_e& channel) {
	switch (channel) {
	case SOUND_CHANNEL_ARM: return "channel_arm";
	case SOUND_CHANNEL_FM:  return "channel_fm";
	case SOUND_CHANNEL_AUX: return "channel_aux";
	case SOUND_CHANNEL_BT:  return "channel_bt";
	default: "channel_unknown";
	}
}

static inline const char* _get_tag_by_type(serial_type_e type) {
	switch (type) {
	case E_SERIAL_TYPE_UART: return "serial_uart";
	case E_SERIAL_TYPE_GOC:  return "serial_goc";
	case E_SERIAL_TYPE_RF:   return "serial_rf";
	default: return "serial_unknown";
	}
}

static void _print_serial_data(serial_type_e type, int is_recv, int is_str, const uint8_t *data, int len) {
#if DEBUG_UART
	int str_len = is_str ? len : len * 2;
	char str[str_len + 1];

	if (is_str) {
		memcpy(str, data, str_len);
		str[str_len] = '\0';
	} else {
		for (int i = 0; i < len; ++i) {
			sprintf(str + i * 2, "%02X", data[i]);
		}
	}

	LOGD("[uart]: %s %s %s\n", _get_tag_by_type(type), is_recv ? "->" : "<-", str);
#endif
}

static int _init_serial() {
	for (int i = 0; i < E_SERIAL_TYPE_MAX; ++i) {
		serial_t *s = _s_serial_tab + i;
		if (s->baudrate > 0) {
			s->fd = fy::os::open_uart(s->dev, s->baudrate);
		} else {
			s->fd = fy::os::create_pty(s->dev);
		}
		if (s->fd < 0) {
			return -1;
		}
		s->data_len = 0;
	}
	return 0;
}

static void _deinit_serial() {
	for (int i = 0; i < E_SERIAL_TYPE_MAX; ++i) {
		serial_t *s = _s_serial_tab + i;
		if (s->fd >= 0) {
			close(s->fd);
			s->fd = -1;
		}
	}
}

static bool _send_to_serial(serial_type_e from, serial_type_e to, const uint8_t *data, int len) {
	_print_serial_data(from, 0, 0, data, len);

	if (write(_s_serial_tab[to].fd, data, len) != len) {
		LOGE("[uart]: %s write to %s error %s\n", _get_tag_by_type(from), _get_tag_by_type(to), strerror(errno));
		return false;
	}
	return true;
}

static int _send_frame_data(uint8_t cmd, const uint8_t *data, int len) {
	uint8_t frame_data[BUFF_DATA_SIZE];

	int frame_len = 0;
	frame_data[frame_len++] = FRAME_HEAD1;
	frame_data[frame_len++] = FRAME_HEAD2;
	frame_data[frame_len++] = cmd;

	if (cmd == CMD_BT) {
		frame_data[frame_len++] = (uint8_t) (len >> 8);
		frame_data[frame_len++] = (uint8_t) len;
	} else {
		frame_data[frame_len++] = (uint8_t) len;
	}

	if (len > 0) {
		memcpy(frame_data + frame_len, data, len);
		frame_len += len;
	}

	uint16_t sum = _get_check_sum(frame_data, frame_len);
	frame_data[frame_len++] = (uint8_t) (sum >> 8);
	frame_data[frame_len++] = (uint8_t) sum;

//	char tmp[4] = {0};
//	std::string str;
//	for(int level1 = 0; level1 < frame_len; ++level1)
//	{
//		snprintf(tmp, sizeof(tmp), "%.2X ", frame_data[level1]);
//		str.append(tmp);
//	}
//	LOGD("--%d-- --%s-- 发送数据：%s\n", __LINE__, __FILE__,str.c_str());
	{
		Mutex::Autolock _l(_s_uart_lock);
		if (write(_s_serial_tab[E_SERIAL_TYPE_UART].fd, frame_data, frame_len) != frame_len) {
			LOGE("[uart]: write to uart error %s\n", strerror(errno));
			return false;
		}
	}
	return true;
}

// 解析非蓝牙协议数据
static void _proc_parse_data(uint8_t cmd, const uint8_t *data, int len) {
//	char buf[len * 2];
//	for (int i=0; i<len; i++) {
//		sprintf(buf + i * 2, "%02X", data[i]);
//	}
	char tmp[4] = {0};
	std::string str;
	for(int level1 = 0; level1 < len; ++level1) {
		snprintf(tmp, sizeof(tmp), "%.2X ", data[level1]);
		str.append(tmp);
	}
	LOGD("str = %s",str.c_str());

	std::string str1(reinterpret_cast<const char*>(data));
	mcu_time_t &info = time_info;
	struct tm t;

	switch (cmd) {
	case CMD_UPDATE_MODE:			// 进入升级模式
		if (data[0] == 0x01) {		// 进入升级模式
			LOGD("--%d-- --%s-- 进入升级模式\n ", __LINE__, __FILE__);
			upd_uart_m.step = UPD_M_SEND_START_SIGN;
			_notify_scan_cb(MCU_UPD_MODE_ENTRY, nullptr, 0);
		}
		break;
	case CMD_QUERY_VER:				// mcu版本号
		_notify_scan_cb(MCU_VERSION, (void*)data, len);
		break;
	case CMD_FM_SW:
		_notify_fm_state(data[0]);
		break;
	case CMD_FM_FRE:
		_notify_fm_fre(((data[0]<<8) | data[1]));
		break;
	case CMD_BT_TIME:
		info.year = atoi(std::string(str1).substr(0, 2).c_str());
		info.month = atoi(std::string(str1).substr(2, 2).c_str());
		info.day = atoi(std::string(str1).substr(4, 2).c_str());
		info.hour = atoi(std::string(str1).substr(6, 2).c_str());
		info.min = atoi(std::string(str1).substr(8, 2).c_str());
		info.sec = atoi(std::string(str1).substr(10, 2).c_str());

		t.tm_year = info.year + 100;
		t.tm_mon = info.month - 1;
		t.tm_mday = info.day;
		t.tm_hour = info.hour;
		t.tm_min = info.min;
		t.tm_sec = info.sec;

		sys::setting::set_DateTime(&t);

		_notify_scan_cb(MCU_PHONE_TIME, (void*)data, len);
#if 1
	LOGD("time: %d:%d:%d:%d:%d:%d", info.year, info.month, info.day, info.hour, info.min, info.sec);
#endif
		break;
	case CMD_BLE_TIRE:
		tire::parseTirePressureProtocol(data, len);
		break;
	default:
		break;
	}
	LOGD("[uart_recv] cmd = %x", cmd);
}


static std::string btcmd[] = {"SV", "SS", "IA", "SI", "SR", "MZ12", "JH", "IS"};


static int _proc_uart_data(serial_type_e type, const uint8_t *data, int len) {
	int remain_len = len;
	int data_len;
	int frame_len;
	int index = 0;
	int crc_err_flag = 0;
//	LOGD("--%d-- --%s-- 解析串口数据\n", __LINE__, __FILE__);
	while (remain_len >= FRAME_MIN_LEN) {
		if ((data[index] != FRAME_HEAD1) || (data[index + 1] != FRAME_HEAD2)) {
			index++;
			remain_len--;
			continue;
		}

		uint8_t cmd = data[index + 2];

		//LOGD("--%d-- --%s-- cmd = %d\n", __LINE__, __FILE__, cmd);
		if (cmd == CMD_BT) {
			data_len = (data[index + 3] << 8) | data[index + 4];
			frame_len = data_len + FRAME_MIN_LEN + 1;
		} else {
			data_len = data[index + 3];
			frame_len = data_len + FRAME_MIN_LEN;
		}

		if (frame_len > remain_len) {
			break;
		}

		const uint8_t *frame_data = data + index;
		uint16_t sum = (frame_data[frame_len - 2] << 8) | frame_data[frame_len - 1];

		crc_err_flag = 0;
		if (_get_check_sum(frame_data, frame_len - 2) == sum) {

			if (cmd == CMD_BT) {
				const uint8_t *bt_frame_data = frame_data + 5;


				//LOGD("[uart to bt]: %s \n", std::string((char*)bt_frame_data, data_len).c_str());
				_send_to_serial(type, E_SERIAL_TYPE_GOC, bt_frame_data, data_len);

				if ((bt_frame_data[0] == 'M' && (bt_frame_data[1] == 'X' || bt_frame_data[1] == 'P' || bt_frame_data[1] == 'I'))
						|| (bt_frame_data[0] == 'P' && (bt_frame_data[1] == 'D' || bt_frame_data[1] == 'B'))) {
				// 过滤电话本，通话记录，配对记录
				} else {
					_send_to_serial(type, E_SERIAL_TYPE_RF, bt_frame_data, data_len);
				}

			} else {
				_proc_parse_data(cmd, frame_data + 4, data_len);
			}
		} else {
			LOGE("[uart] proc uart data check sum error\n");
			crc_err_flag = 1;		// 处理嵌套帧
			if (cmd == CMD_BT) {
				index += 2;
				remain_len -= 2;
			} else {
				index += 2;
				remain_len -= 2;
			}
		}

		if(!crc_err_flag) {
			index += frame_len;
			remain_len -= frame_len;
		}
	}

	return len - remain_len;
}

static int _proc_bt_data(serial_type_e type, const uint8_t *data, int len) {
	int start_index = 0;
	for (int i = 0; i <= len - 2; ++i) {
		// end flag \r\n
		if ((data[i] != '\r') || (data[i + 1] != '\n')) {
			continue;
		}

		int bt_frame_len = i - start_index + 2;

		if (bt_frame_len > 2) {
			const uint8_t *bt_frame_data = data + start_index;
			_print_serial_data(type, 1, 1, bt_frame_data, bt_frame_len);
			if(bt_frame_data[0] == 'A' && bt_frame_data[1] == 'T') {
				_send_frame_data(CMD_BT, bt_frame_data, bt_frame_len);
			} else {
//				LOGE("[uart] Not starting with AT\n");
			}
		} else {
			// LOGE("[uart] no bt data\n");
		}
		start_index = i + 2;
	}
	return start_index;
}

static bool sed_to_mcu(const uint8_t *data, int len) {
	LOGD("--%d-- --%s-- sed_to_mcu\n", __LINE__, __FILE__);
	if (write(_s_serial_tab[E_SERIAL_TYPE_UART].fd, data, len) != len) {
		LOGE("[uart]: write to uart error %s\n", strerror(errno));
		return false;
	}
	return true;
}

static void recive_cmd(uint8_t *data, uint8_t *buf, uint16_t len) {
    uint8_t ch;
    uint8_t cnt = 0;
	for (uint16_t i=0; i<len; i++) {
		ch = data[i];
		if (ch == 0xAA && cnt == 0) {
			buf[cnt++] = ch;
		} else if(ch == 0x55 && cnt == 1) {
			buf[cnt++] = ch;
		} else if(cnt > 1) {
			buf[cnt++] = ch;
		} else {
			cnt = 0;
		}
	}
}

// 进入升级
static void func_uart_update_enter(void) {
	if (upd_uart_m.step == UPD_M_RECIVE_CMD) {
		LOGD("already update enter!");
		return;
	}

	int cmd_len = str_start_sign.length();
    uint8_t data[cmd_len];
    long long timeTamp = get_timestamp();
    memset(&upd_uart_m, 0, sizeof(upd_uart_m));
    memcpy(data, str_start_sign.c_str(), cmd_len);

	while(1) {
		if (get_timestamp() - timeTamp > 100) {
			sed_to_mcu(data, cmd_len);		//100ms发送一次start命令
			timeTamp = get_timestamp();
		}
		if(upd_uart_m.step == UPD_M_RECIVE_CMD) {
			_s_mcu_update_fd = open(upd_filename, O_RDONLY);
			LOGD("open upgrade file %s %s!\n", upd_filename, (_s_mcu_update_fd > 0) ? "success" : "fail");
			break;
		}
		usleep(1000*10);
	}
}

// 如果没有主动进入升级模式，就进入restart，那么表示升级失败，，
static bool is_recive_cmd(std::string recv_str) {
	LOGD("--%d-- --%s-- [uart_recv]  data = %s\n", __LINE__, __FILE__, recv_str.c_str());
	if (recv_str == "RECEIVESTART") {
		LOGD("--%d-- --%s-- 接收到  RECEIVESTART \n", __LINE__, __FILE__);
		upd_uart_m.step = UPD_M_RECIVE_CMD;
		return true;
	}
	return false;
}

static void rsp_check_uart_mode(uart_upd_m_txcmd_t *txcmd) {
//    delay_ms(5);//每次接收到cmd等待5ms的时间，等待从机准备完毕
    usleep(500*1000);	 	//每次接收到cmd等待5ms的时间，等待从机准备完毕
    memset(txcmd, 0, sizeof(uart_upd_m_txcmd_t));
    txcmd->sign = 0x55AA;
    txcmd->cmd = CMD_CHECK_UART_UPD;
    txcmd->crc = (uint16_t)uart_cal_calc_crc(txcmd, sizeof(uart_upd_m_txcmd_t)-2);
    sed_to_mcu((uint8_t*)txcmd, sizeof(uart_upd_m_txcmd_t));

    int len = sizeof(uart_upd_m_txcmd_t);
	char buf[len* 2];
	for (int i=0; i<len; i++) {
		sprintf(buf + i*2, "%02X", ((uint8_t*)txcmd)[i]);
	}
	LOGD("--%d-- --%s-- send:  %s\n", __LINE__, __FILE__, buf);
}

static bool upd_send_data(uart_upd_m_rxcmd_t *rxcmd, uart_upd_m_txcmd_t *txcmd) {
    usleep(10*1000);	//每次接收到cmd等待5ms的时间，等待从机准备完毕
    int rlen = 0;
    uint8_t upd_buf[512];

    // 1.获取文件长度
    uint32_t total_length = lseek(_s_mcu_update_fd, 0, SEEK_END);	//回到文件的末尾,故返回的是文件的大小
    lseek(_s_mcu_update_fd, 0, SEEK_SET);
    // LOGD("--%d-- --%s-- rxcmd->len = %u \n", __LINE__, __FILE__, rxcmd->len);

	for(uint32_t len=0; len<rxcmd->len; len+=512) {
//		LOGD("--%d-- --%s-- len = %u, rxcmd->len = %u \n", __LINE__, __FILE__, len, rxcmd->len);
//		LOGD("--%d-- --%s-- uart_addr = %u   rxcmd->addr = %u \n", __LINE__, __FILE__, upd_uart_m.addr, rxcmd->addr);
		if(upd_uart_m.addr != rxcmd->addr) {
			off_t res = lseek(_s_mcu_update_fd, rxcmd->addr, SEEK_SET);
			UpdProgress upd_progress(total_length, res);
			if (upd_progress.cur_pos != 0) {
				_notify_scan_cb(MCU_UPD_PRGGRESS, &upd_progress, sizeof(upd_progress));
			}

//			LOGD("--%d-- --%s-- total_length = %u,  res = %lu \n", __LINE__, __FILE__, total_length, res);

			if (res < 0) {
				LOGD("--%d-- --%s-- lseek error %s!\n", __LINE__, __FILE__, strerror(errno));
				return false;
			}
		}
		upd_uart_m.addr = rxcmd->addr + len;
		rlen = read(_s_mcu_update_fd, upd_buf, 512);

		if(rlen > 0) {
//			LOGD("--%d-- --%s-- len = %u \n", __LINE__, __FILE__, rlen);
			txcmd->sign = 0x55AA;
			txcmd->cmd = rxcmd->cmd;
			txcmd->addr = rxcmd->addr + len;
			txcmd->data_crc = uart_cal_calc_crc(upd_buf, rlen);
			txcmd->crc = (uint16_t)uart_cal_calc_crc(txcmd, sizeof(uart_upd_m_txcmd_t)-2);
			sed_to_mcu((uint8_t *)txcmd, sizeof(uart_upd_m_txcmd_t));
			usleep(10*1000);
			sed_to_mcu(upd_buf, rlen);
		} else {
//			LOGD("--%d-- --%s-- 读取完毕 \n", __LINE__, __FILE__);
//			close(_s_mcu_update_fd);
		}
    }
    return true;
}

static void upd_read_status(uart_upd_m_rxcmd_t *rxcmd) {
    LOGD(" status %x\n",rxcmd->status);
    if(rxcmd->status == 0xff) {
		LOGD("mcu upgrade successful\n");
		upd_uart_m.step = UPD_M_DONE;
		_notify_scan_cb(MCU_UPD_FINISHED, nullptr, 0);
		close(_s_mcu_update_fd);
    }
}

static void func_uart_update_event(uint8_t* data, uint16_t len) {
	char buf[len * 2];
	for (int i=0; i<len; i++) {
		sprintf(buf + i*2, "%02X", data[i]);
	}
	LOGD("--%d-- --%s-- recv:  %s\n", __LINE__, __FILE__, buf);

	uart_upd_m_rxcmd_t *rxcmd = &upd_uart_m.rxcmd;
    uart_upd_m_txcmd_t *txcmd = &upd_uart_m.txcmd;
    uint16_t upd_len = sizeof(uart_upd_m_rxcmd_t);

    if (len >= upd_len) {
    	recive_cmd(data, (uint8_t*)rxcmd, len);
    	LOGD("--%d-- --%s-- rxcmd->cmd = %d \n", __LINE__, __FILE__, rxcmd->cmd);
    }

	switch (rxcmd->cmd) {
	case CMD_CHECK_UART_UPD: 	// 检查uart_upd模式
		rsp_check_uart_mode(txcmd);
		break;
	case CMD_SEND_DATA: 		// 读文件命令
		upd_send_data(rxcmd, txcmd);
		break;
	case CMD_READ_STATUS: 	 	// 是否升级完成     0xff
		upd_read_status(rxcmd);
		break;
	default: break;
	}
	memset(&upd_uart_m.rxcmd, 0, sizeof(uart_upd_m_rxcmd_t));
}

static void _serial_normal_mode(serial_t *s, int index, uint16_t len) {
//	LOGD("--%d-- --%s-- type: %d, 串口正常模式\n ", __LINE__, __FILE__, index);
	int data_len = s->data_len + len;
	len = s->proc((serial_type_e) index, s->data, data_len);
	s->data_len = data_len - len;

//	LOGD("--%d-- --%s-- len = %d, data_len = %d \n", __LINE__, __FILE__, len, data_len);

	if ((len > 0) && (len < data_len)) {
//		LOGD("--%d-- --%s-- 正常拷贝数据\n ", __LINE__, __FILE__);
		memcpy(s->data, s->data + len, s->data_len);
	}
}

static void* _serial_thread(void *args) {
	while (true) {
		struct pollfd pfd[E_SERIAL_TYPE_MAX] = { 0 };
		for (int i = 0; i < E_SERIAL_TYPE_MAX; ++i) {
			pfd[i].fd = _s_serial_tab[i].fd;
			pfd[i].events = POLLIN;
		}

		int ret = poll(pfd, E_SERIAL_TYPE_MAX, -1);
		if (ret == -1) {
			LOGE("[uart] poll error %s!\n", strerror(errno));
			continue;
		}

		for (int i = 0; i < E_SERIAL_TYPE_MAX; ++i) {
			if (!(pfd[i].revents & POLLIN)) {
				if ((pfd[i].revents & POLLHUP) && (ret == 1)) {
					DELAY(100);
				}
				continue;
			}
			serial_t *s = _s_serial_tab + i;

			int len = read(s->fd, s->data + s->data_len, BUFF_DATA_SIZE - s->data_len);
			if (len < 0) {
				usleep(50*1000);		// 防止log瞬间写满
				LOGE("[uart] read err\n");
				continue;
			}

//			LOGD("[uart revc from mcu]: len = %d  step:%d  \n", len, upd_uart_m.step);
//			if (len > 0) { LOGD("uart 接收内容   len = %d \n", len); }


			switch (upd_uart_m.step) {
			case UPD_M_SEND_START_SIGN:	// 进入升级
				is_recive_cmd(std::string((char*)(s->data + s->data_len), len));
				break;
			case UPD_M_RECIVE_CMD: 		// 升级模式
				func_uart_update_event(s->data + s->data_len, len);
				break;
			case UPD_M_DONE:			// 正常使用模式
				_serial_normal_mode(s, i, len);
				break;
			default: break;
			}
		}
	}
	return NULL;
}

bool init() {
	_init_serial();
	// 原串口框架初始化
	//UartContext::init();
	return fy::run_thread(_serial_thread, NULL);
}

void add_mcubt_cb(_s_mcu_cb cb) {
	Mutex::Autolock _l(_s_mcu_cb_set_lock);
	_s_mcu_cb_set.insert(cb);
}

void remove_mcubt_cb(_s_mcu_cb cb) {
	Mutex::Autolock _l(_s_mcu_cb_set_lock);
	_s_mcu_cb_set.erase(cb);
}

void add_fm_state_cb(fm_state_cb cb){
	Mutex::Autolock _l(_s_mcu_cb_set_lock);
	_s_fm_state_set.insert(cb);
}
void remove_fm_state_cb(fm_state_cb cb){
	Mutex::Autolock _l(_s_mcu_cb_set_lock);
	_s_fm_state_set.erase(cb);
}

void add_fm_fre_cb(fm_fre_cb cb){
	Mutex::Autolock _l(_s_mcu_cb_set_lock);
	_s_fm_fre_set.insert(cb);
}

void remove_fm_fre_cb(fm_fre_cb cb){
	Mutex::Autolock _l(_s_mcu_cb_set_lock);
	_s_fm_fre_set.erase(cb);
}

bool set_volume(float vol) {
	LOGD("vol = %f",vol);
	//vol = range_ctrl(vol, 0.f, 1.6f);
	LOCK_OP(sys_voice._s_vol_mutex, sys_voice.volume = (uint8_t)(10.0 * vol))		//((vol < 0.1) ? 0 : (20.0 * vol + 19))
	uint8_t data[] = { sys_voice.channel, sys_voice.volume };
	return _send_frame_data(CMD_CHANNEL, data, 2);
}

bool set_amplifier_mute(uint8_t mute) {
	LOGD("set_amplifier_mute mute = %d",mute);
	LOCK_OP(sys_voice._s_vol_mutex, sys_voice.mute = mute)
	uint8_t data = sys_voice.mute;
	return _send_frame_data(CMD_SOUND_MUTE, &data, 1);
}

bool set_sound_channel(sound_channel_e channel) {
	LOGD("VOICE_CHANNEL: %s\n", channel_to_str(channel).c_str());
	LOCK_OP(sys_voice._s_vol_mutex, sys_voice.channel = channel)
	uint8_t data[] = { sys_voice.channel, sys_voice.volume };
	return _send_frame_data(CMD_CHANNEL, data, TABLESIZE(data));
}

bool set_sound_effect(int index, int eq) {
	uint8_t ep_data[2] = {0};
//	index = range_ctrl(index, 0, 9);
//	eq = range_ctrl(index, 0, 24);
	ep_data[0] = index;
	ep_data[1] = eq;
//	_arr_eq[index] = eq;
//
//	char buf[32] = {0};
//	std::string str;
//	for (size_t i=0; i<TABLESIZE(_arr_eq); i++) {
//		sprintf(buf, "%d ", _arr_eq[i]);
//		str += buf;
//	}
//	LOGD("--%d-- --%s-- _arr_eq_str = %s\n", __LINE__, __FILE__, str.c_str());
	return _send_frame_data(CMD_SOUND_EFFECT, ep_data, TABLESIZE(ep_data));
}

bool set_sound_field(sound_field_e field, int gain) {
	gain = range_ctrl(gain, 0, 20);
	_arr_field[field] = gain;
	return _send_frame_data(CMD_FIELD_GAIN, _arr_field, TABLESIZE(_arr_field));
}

bool query_update_file() {
	return access(upd_filename, F_OK) == 0;
}

bool entry_update_mode() {
	static uint8_t _update_mode[] = {0x00, 0x01, 0x02, 0x03};
	return _send_frame_data(CMD_UPDATE_MODE, _update_mode, TABLESIZE(_update_mode));
}

bool start_mcu_update() {
	if(access(upd_filename, F_OK) != 0) {
        LOGD("mcu upgrade file %s does not exist\n", upd_filename);
        return false;
    }
	LOGD("start mcu update \n");
	func_uart_update_enter();
	return true;
}

bool query_mcubt_version() {
	return _send_frame_data(CMD_QUERY_VER, nullptr, 0);
}

bool fm_switch(bool status){
	uint8_t status_data[1] = {0};
	status_data[0] = status;
	return _send_frame_data(CMD_FM_SW, status_data, TABLESIZE(status_data));
}

bool fm_frequency(int value){
	uint8_t fre_data[2] = {0};
	fre_data[0] = (value & 0xff00) >> 8;
	fre_data[1] = value & 0xff;
	return _send_frame_data(CMD_FM_FRE, fre_data, TABLESIZE(fre_data));
}

bool query_fmswitch(){
	uint8_t data[1] = {0};
	data[0] = 0xff;
	return _send_frame_data(CMD_FM_SW, data, TABLESIZE(data));
}

bool query_frequency(){
	uint8_t data[2] = {0};
	data[0] = 0xff;
	data[1] = 0xff;
	return _send_frame_data(CMD_FM_FRE, data, TABLESIZE(data));
}

}





