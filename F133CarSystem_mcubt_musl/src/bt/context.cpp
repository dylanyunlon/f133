/*
 * context.cpp
 *
 *  Created on: 2023年1月4日
 *      Author: ZKSWE Develop Team
 */

#include <termio.h>
#include <map>
#include "context.h"
#include "common.h"
#include "system/Mutex.h"
#include "fy/files.hpp"
#include "fy/asio.hpp"
#include "fy/thread.hpp"
#include "fy/os.hpp"
#include "fy/time.hpp"
#include "fy/strings.hpp"
#include "misc/storage.h"
#include "media/audio_context.h"
#include "link/context.h"
#include "utils/TimeHelper.h"
#include "system/reverse.h"
#include "entry/EasyUIContext.h"
#include "system/setting.h"
#include "uart/context.h"
#include <algorithm>

namespace bt {
#define LLONG_MAX 9223372036854775807

#define DATA_BUF_LEN     2048

// 诚谦授权文件
#define BLINK_DIR        "/data/blink/"
#define BLINK_LIC        BLINK_DIR ".btlicense"

#define BT_POWER_KEY                "bt_power_key"
#define BT_AUTO_CONNECT_KEY         "bt_auto_connect_key"

#define BT_CMD_OPEN_POWER 			"P1"		// 打开蓝牙
#define BT_CMD_CLOSE_POWER 			"P0"		// 关闭蓝牙
#define BT_CMD_OPEN_AUTOCONNECT 	"MG"		// 开启自动重连
#define BT_CMD_CLOSE_AUTOCONNECT 	"MH"		// 关闭自动重连
#define BT_CMD_OPEN_AUTOANSWER 		"MP"		// 开启来电自动应答
#define BT_CMD_CLOSE_AUTOANSWER 	"MQ"		// 关闭来电自动应答

#define BT_CMD_QUERY_INFO 			"MY"		// 查询模块版本号
#define BT_CMD_QUERY_ADDR			"DF"		// 查询本地蓝牙地址
#define BT_CMD_QUERY_CONNECT_NAME 	"QA"		// 查询连接蓝牙名称
#define BT_CMD_QUERY_CONNECT_ADDR 	"QB"		// 查询连接蓝牙地址
#define BT_CMD_QUERY_STATE 			"CY"		// 查询状态
#define BT_CMD_QUERY_AUTOANSWERCON 	"MF"		// 查询自动应答和自动重连设置
#define BT_CMD_QUERY_MATCH_ADDR		"MX"		// 查询与模块配对过的蓝牙列表
#define BT_CMD_QUERY_AUTHORIZATION	"QL"		// 查询模块授权状态

#define BT_CMD_ENABLE_MATCH 		"CA"		// 进入配对模式
#define BT_CMD_DISABLE_MATCH 		"CB"		// 取消配对模式

#define BT_CMD_CONNECT 				"CC"		// 建立连接
#define BT_CMD_DISCONNECT 			"CD"		// 断开连接
#define BT_CMD_CLE_MATCH			"CV"		// 清除配对记录

#define BT_CMD_PHONE_BOOK			"PA"		// 下载电话本		(sim)
#define BT_CMD_PHONE_BOOK2			"PB"		// 下载电话本		(手机)
#define BT_CMD_RECORD_OUTGOING 		"PH"		// 下载去电记录
#define BT_CMD_RECORD_INCOMING 		"PI"		// 下载来电记录
#define BT_CMD_RECORD_MISSED 		"PJ"		// 下载未接来电
#define BT_CMD_RECORD_RECENT 		"PK"		// 同步最近通话记录
#define BT_CMD_SYNC_PB_OR_CR		"PX"		// 同步数据(发了以上4条指令，只是告诉了蓝牙模块接下来要同步什么数据，还需要发PX指令，才能下载到相应的数据)

#define BT_CMD_DIAL 				"CW"		// 播出电话号码
#define BT_CMD_ANSWER 				"CE"		// 接听来电
#define BT_CMD_REJECT 				"CF"		// 拒绝来电
#define BT_CMD_TERMINATE 			"CG"		// 结束通话或结束拨号
#define BT_CMD_DIAL_KEY_NUM			"CX"	 	// 拨分机号码
#define BT_CMD_CALL_VOL				"CK"	 	// 蓝牙通话声音

#define BT_CMD_START_SCAN			"SD"		// 查找蓝牙设备
#define BT_CMD_STOP_SCAN 			"ST"		// 停止搜索蓝牙设备

#define BT_CMD_SOUND_TO_PHONE		"CN"		// 声音切到手机
#define BT_CMD_SOUND_TO_CAR			"CP"		// 声音切到车机
#define BT_CMD_MIC_SWITCH			"CM"		// 打开/关闭麦克风

#define BT_CMD_QUERY_MODIFY_NAME 	"MM"		// 查询/修改名称
#define BT_CMD_QUERY_MODIFY_PIN 	"MN"		// 查询/修改密码

#define BT_CMD_MUSIC_INFO			"MK"		// 查询 a2dp 信息
#define BT_CMD_MUSIC_PREV 			"ME"		// 上一曲
#define BT_CMD_MUSIC_NEXT 			"MD"		// 下一曲
#define BT_CMD_MUSIC_PLAY 			"MA"		// 播放/暂停
#define BT_CMD_MUSIC_STOP 			"MC"		// 停止
//#define BT_CMD_MUSIC_VA 			"VA"		// 静音
//#define BT_CMD_MUSIC_VB 			"VB"		// 出声音
#define BT_CMD_MUSIC_PM0 			"PM0"		// 静音
#define BT_CMD_MUSIC_PM1 			"PM1"		// 出声音

#define BT_CMD_START_SCAN_BS 		"BS"		// 扫描方控设备
#define BT_CMD_START_SCAN_VB 		"VB"		// 停止扫描方控设备

#define BT_CMD_BT_INITIA 			"QS"		// 查询蓝牙初始化状态

#define CB_FOREACH(s, t, cb, arg)   SET_FOREACH(s, t, it) { if ((*it)->cb) { (*it)->cb(arg); }}
#define LOCK_OP(lock, op)           { Mutex::Autolock _l(lock); op; }

typedef struct {
	bool is_authorized = false; 	// 蓝牙授权状态
	bool is_on;         			// 蓝牙开关状态
	bool scanning = false;      	// 扫描状态
	bool is_downloading_phone = false;   	// 联系
	bool is_downloading_record = false;		// 通话记录下载状态

	bool calling;       			// 通话中
	bool bt_is_mute;				// 蓝牙是否静音

	bool auto_connect;  			// 自动连接
	bool auto_answer;   			// 自动接听

	bool car_sound = true;     		// true: 车机端出声音, false: 手机端出声音
	bool mic_mute;

	uint32_t talk_start_time;               // 记录开始通话时间

	bt_dev_t dev_info;  			// 本机设备信息
	std::string pin_code;

	bt_connect_state_e connect_state = E_BT_CONNECT_STATE_IDLE;  // 连接状态
	bt_dev_t connect_dev;           // 连接设备

	bt_call_state_e call_state;
	std::string call_num;

	bt_music_t music_info;
	bool music_playing;

	std::map<std::string, std::string> scan_dev_map;    // map[addr] = name
	std::map<std::string, std::string> matched_dev_map;

	std::vector<bt_contact_t> contact_list;
	std::vector<bt_record_t> record_list;

	long long call_start_time = LLONG_MAX;			//通话时长
} bt_info_t;

static int _s_bt_fd = -1;

static bt_info_t _s_bt_info;
static Mutex _s_info_mutex;
static Mutex _s_send_lock;

static std::set<bt_cb_t *> _s_cb_set;
static Mutex _s_cb_mutex;
static void* _bt_thread(void *args);
static bool _send_cmd(const char *cmd);

typedef struct {
	const char *cmd;
	void (*proc)(const char *data, int len);
} proc_fun;

static void _notify_power_cb(bt_power_state_e state) {
	Mutex::Autolock _l(_s_cb_mutex);
	CB_FOREACH(_s_cb_set, bt_cb_t *, power_cb, state);
}

static void _notify_connect_cb(bt_connect_state_e state) {
	Mutex::Autolock _l(_s_cb_mutex);
	_s_bt_info.connect_state = state;
	CB_FOREACH(_s_cb_set, bt_cb_t *, connect_cb, state);
}

static void _notify_scan_cb(bt_scan_state_e state) {
	Mutex::Autolock _l(_s_cb_mutex);
	CB_FOREACH(_s_cb_set, bt_cb_t *, scan_cb, state);
}

static void _notify_call_cb(bt_call_state_e state) {
	Mutex::Autolock _l(_s_cb_mutex);
	_s_bt_info.call_state = state;
	CB_FOREACH(_s_cb_set, bt_cb_t *, call_cb, state);
}

static void _notify_download_cb(bt_download_state_e state) {
	Mutex::Autolock _l(_s_cb_mutex);
	CB_FOREACH(_s_cb_set, bt_cb_t *, download_cb, state);
}

static void _notify_music_cb(bt_music_state_e state) {
	Mutex::Autolock _l(_s_cb_mutex);
	CB_FOREACH(_s_cb_set, bt_cb_t *, music_cb, state);
}

static void _notify_misc_info_cb(bt_misc_info_e info) {
	Mutex::Autolock _l(_s_cb_mutex);
	CB_FOREACH(_s_cb_set, bt_cb_t *, misc_info_cb, info);
}

static void _notify_raw_data_cb(const char *data, int len) {
	Mutex::Autolock _l(_s_cb_mutex);
	SET_FOREACH(_s_cb_set, bt_cb_t *, it) {
		if ((*it)->raw_data_cb) {
			(*it)->raw_data_cb(data, len);
		}
	}
}

static void _notify_square_data_cb(const char *data) {
	Mutex::Autolock _l(_s_cb_mutex);
	SET_FOREACH(_s_cb_set, bt_cb_t *, it) {
		if ((*it)->square_data_cb) {
			(*it)->square_data_cb(data);
		}
	}
}

std::vector<bt_contact_t> m_contact_list;
std::vector<bt_record_t> m_record_list;

struct Record_Sort {
  bool operator() (bt_record_t record1, bt_record_t record2) {
	  return (record1.time > record2.time);
  }
} record_sort;

static void* _records_sort_thread(void *args) {
	LOGD("[bt] thread start...\n");
	LOCK_OP(_s_info_mutex, m_record_list = _s_bt_info.record_list);

	// 通话记录按时间排序
	std::sort(m_record_list.begin(), m_record_list.end(), record_sort);
	LOCK_OP(_s_info_mutex, _s_bt_info.record_list = m_record_list);
	_notify_download_cb(E_BT_DOWNLOAD_RECORD_COMPLETED);
	LOCK_OP(_s_info_mutex, _s_bt_info.is_downloading_record = false)
	return NULL;
}

static std::string to_time_format_str(std::string str) {
	str.insert(4, "/");
	str.insert(7, "/");
	str.replace(10, 1, " ");
	str.insert(10, " ");
	str.insert(14, ":");
	str.insert(17, ":");
//	return str.substr(5, 12);
	return str;
}

static void _proc_power_on(const char *data, int len) {
	LOGD("[bt] power on\n");
	bool bten = storage::get_bool(BT_POWER_KEY, true);
	LOGD("--%d-- --%s-- power on!   bten = %d \n", __LINE__, __FILE__, bten);
	if (!bten) {				//蓝牙默认打开   所以主动恢复上次关闭状态
		_send_cmd(BT_CMD_CLOSE_POWER);
		return;
	}
	bt::query_authorization();		// 查询蓝牙授权
	modify_name(sys::setting::get_bt_name().c_str());
	query_matched();
	_s_bt_info.is_on = true;
	_notify_power_cb(E_BT_POWER_STATE_ON);
	//set_bt_mute(true);
}

static void _proc_power_off(const char *data, int len) {
	LOGD("[bt] power off\n");
	_s_bt_info.is_on = false;
	// 结束通话状态——>音量恢复
	if (_s_bt_info.calling) { audio::handle_phone(E_AUDIO_TYPE_BT_PHONE, false); }
	{
		Mutex::Autolock _l(_s_info_mutex);
		_s_bt_info.is_downloading_phone = false;
		_s_bt_info.is_downloading_record = false;
		_s_bt_info.scanning = false;
		_s_bt_info.connect_state = E_BT_CONNECT_STATE_IDLE;
		_s_bt_info.call_state = E_BT_CALL_STATE_IDLE;
		_s_bt_info.calling = false;
		_s_bt_info.music_playing = false;
		_s_bt_info.music_info = bt_music_t();

		_s_bt_info.contact_list.clear();
		_s_bt_info.record_list.clear();
		_s_bt_info.scan_dev_map.clear();
		_s_bt_info.matched_dev_map.clear();
	}

	_notify_power_cb(E_BT_POWER_STATE_OFF);
}

static void _proc_dev_name(const char *data, int len) {
	LOCK_OP(_s_info_mutex, _s_bt_info.dev_info.name = std::string(data, len));
	LOGD("[bt] dev name: %s\n", _s_bt_info.dev_info.name.c_str());

	_notify_misc_info_cb(E_BT_MISC_INFO_DEV_NAME);
}

static void _proc_dev_addr(const char *data, int len) {
	LOCK_OP(_s_info_mutex, _s_bt_info.dev_info.addr = std::string(data, len));
	LOGD("[bt] dev addr: %s\n", _s_bt_info.dev_info.addr.c_str());

	_notify_misc_info_cb(E_BT_MISC_INFO_DEV_ADDR);
}

static void _proc_pin_code(const char *data, int len) {
	LOCK_OP(_s_info_mutex, _s_bt_info.pin_code = std::string(data, len));
	LOGD("[bt] pin code: %s\n", _s_bt_info.pin_code.c_str());

	_notify_misc_info_cb(E_BT_MISC_INFO_PIN_CODE);
}

static void _proc_auto_answer_conn(const char *data, int len) {
	if (len < 2) {
		return;
	}
	if (data[1] == '1') {
		_s_bt_info.auto_connect = true;
		_notify_misc_info_cb(E_BT_MISC_INFO_AUTO_CONNECT_ON);
	} else {
		_s_bt_info.auto_connect = false;
		_notify_misc_info_cb(E_BT_MISC_INFO_AUTO_CONNECT_OFF);
	}

	if (data[0] == '1') {
		_s_bt_info.auto_answer = true;
		_notify_misc_info_cb(E_BT_MISC_INFO_AUTO_ANSWER_ON);
	} else {
		_s_bt_info.auto_answer = false;
		_notify_misc_info_cb(E_BT_MISC_INFO_AUTO_ANSWER_OFF);
	}
}

static void _proc_scan(const char *data, int len) {
	if (len <= 12) {  // 地址12个字节
		return;
	}

	std::string addr = std::string(data, 12);
	std::string name = std::string(data + 12, len - 12);
	LOGD("[bt] scan addr: %s, name: %s\n", addr.c_str(), name.c_str());

	if (!_s_bt_info.scanning) {
		_s_bt_info.scanning = true;
		LOCK_OP(_s_info_mutex, _s_bt_info.scan_dev_map.clear());
		_notify_scan_cb(E_BT_SCAN_STATE_STARTED);
	}

	LOCK_OP(_s_info_mutex, _s_bt_info.scan_dev_map[addr] = name);
	_notify_scan_cb(E_BT_SCAN_STATE_SCANNING_NEW_DEV);
}

static void _proc_scan_end(const char *data, int len) {
	LOGD("[bt] scan end\n");
	_s_bt_info.scanning = false;
	_notify_scan_cb(E_BT_SCAN_STATE_COMPLETED);
}

static void _proc_match_device(const char *data, int len) {
	if (len <= 13) {  // index 1字节, 地址12个字节
		return;
	}

	char index = data[0];
	std::string addr = std::string(data + 1, 12);
	std::string name = std::string(data + 13, len - 13);
	LOGD("[bt] match index: %c, addr: %s, name: %s\n", index, addr.c_str(), name.c_str());

	LOCK_OP(_s_info_mutex, _s_bt_info.matched_dev_map[addr] = name);
	_notify_scan_cb(E_BT_SCAN_STATE_SCANNING_MATCHED);
}

static void entry_lylink_ftu() {
	if (!sys::reverse_does_enter_status()) {
		EASYUICONTEXT->openActivity("lylinkviewActivity");
	} else {
		LOGD("Is reverse status !!!\n");
		lylinkapi_cmd(LYLINK_VIDEO_NATIVE);
	}
}
static void _proc_hfp_state(const char *data, int len) {
	bool calling = false;

	switch (data[0]) {
	case '0':
		LOGD("[bt] HFP state: 初始化\n");
		_notify_connect_cb(E_BT_CONNECT_STATE_IDLE);
		break;
	case '1':
		LOGD("[bt] HFP state: 待机状态\n");
		_notify_connect_cb(E_BT_CONNECT_STATE_IDLE);
		break;
	case '2':
		LOGD("[bt] HFP state: 连接中\n");
		_notify_connect_cb(E_BT_CONNECT_STATE_CONNECTING);
		break;
	case '3':
		LOGD("[bt] HFP state: 连接成功\n");
		if (strcmp(_s_bt_info.connect_dev.name.c_str(), "") == 0) {
			LOGD("--%d-- --%s-- name = %s!!!\n", __LINE__, __FILE__, bt::get_connect_dev().name.c_str());
			query_state();
		}
		_notify_connect_cb(E_BT_CONNECT_STATE_CONNECTED);
		break;
	case '4':
		LOGD("[bt] HFP state: 电话拨出\n");
//		_notify_call_cb(E_BT_CALL_STATE_OUTGOING);
		if (lk::get_lylink_type() == LINK_TYPE_NULL) {
			_notify_call_cb(E_BT_CALL_STATE_OUTGOING);
		}
		calling = true;
		break;
	case '5':
		LOGD("[bt] HFP state: 电话打入\n");
//		_notify_call_cb(E_BT_CALL_STATE_INCOMING);
		//if (lk::get_lylink_type() == LINK_TYPE_NULL) {_notify_call_cb(E_BT_CALL_STATE_INCOMING); }
		calling = true;
		break;
	case '6':
		LOGD("[bt] HFP state: 通话中 \n");
//		_notify_call_cb(E_BT_CALL_STATE_TALKING);
		LOCK_OP(_s_info_mutex, _s_bt_info.talk_start_time = (uint32_t) fy::time::uptime());
		if (lk::get_lylink_type() == LINK_TYPE_NULL) {_notify_call_cb(E_BT_CALL_STATE_TALKING);}
		calling = true;
		break;
	}
	if (_s_bt_info.calling != calling) {
		_s_bt_info.calling = calling;
		set_bt_mute(false);
		audio::handle_phone(E_AUDIO_TYPE_BT_PHONE, calling);
		if (!calling) {
			// 通话结束
			Mutex::Autolock _l(_s_info_mutex);
			_s_bt_info.call_num.clear();
		}
	}
}

static void _proc_disconnect(const char *data, int len) {
	// 结束通话状态——>音量恢复
	if (_s_bt_info.calling) { audio::handle_phone(E_AUDIO_TYPE_BT_PHONE, false); }
	Mutex::Autolock _l(_s_info_mutex);
	_s_bt_info.is_downloading_phone = false;
	_s_bt_info.is_downloading_record = false;
	_s_bt_info.music_playing = false;
	_s_bt_info.connect_dev = bt_dev_t();
	_s_bt_info.contact_list.clear();
	_s_bt_info.record_list.clear();
	_s_bt_info.music_info = bt_music_t();
	_notify_connect_cb(E_BT_CONNECT_STATE_IDLE);
}

static void _proc_connected(const char *data, int len) {
	LOGD("--%d-- --%s-- HFP已连接\n", __LINE__, __FILE__);
	_notify_connect_cb(E_BT_CONNECT_STATE_CONNECTED);
	#if BT_MODULE == BT_MODULE_LYGOC
		query_matched();
	#endif
}

static void _proc_connect_name(const char *data, int len) {
	LOCK_OP(_s_info_mutex, _s_bt_info.connect_dev.name = std::string(data, len));
	LOGD("[bt] connect name: %s\n", _s_bt_info.connect_dev.name.c_str());
}

static void _proc_connect_addr(const char *data, int len) {
	LOCK_OP(_s_info_mutex, _s_bt_info.connect_dev.addr = std::string(data, len));
	LOGD("[bt] connect addr: %s\n", _s_bt_info.connect_dev.addr.c_str());
}

static void _proc_call_outgoing(const char *data, int len) {
	LOCK_OP(_s_info_mutex, _s_bt_info.call_num = std::string(data, len));
	LOGD("[bt] outgoing num: %s\n", _s_bt_info.call_num.c_str());

	if (lk::get_lylink_type() == LINK_TYPE_WIFIAUTO || lk::get_lylink_type() == LINK_TYPE_USBAUTO) { entry_lylink_ftu(); }
	else if (lk::get_lylink_type() == LINK_TYPE_NULL) {
		//set_bt_mute(false);
		if(get_call_state() != E_BT_CALL_STATE_TALKING)
			_notify_call_cb(E_BT_CALL_STATE_OUTGOING);
	}
}

static void _proc_call_incoming(const char *data, int len) {
	LOCK_OP(_s_info_mutex, _s_bt_info.call_num = std::string(data, len));
	LOGD("[bt] incoming num: %s\n", _s_bt_info.call_num.c_str());

	if (lk::get_lylink_type() == LINK_TYPE_WIFIAUTO || lk::get_lylink_type() == LINK_TYPE_USBAUTO) { entry_lylink_ftu(); }
	else if (lk::get_lylink_type() == LINK_TYPE_NULL) {
		//set_bt_mute(false);
		_notify_call_cb(E_BT_CALL_STATE_INCOMING);
	}
}

static void _proc_answer(const char *data, int len) {
	LOGD("[bt] answer\n");
	if (lk::get_lylink_type() == LINK_TYPE_NULL) { _notify_call_cb(E_BT_CALL_STATE_TALKING); }
	LOCK_OP(_s_info_mutex, _s_bt_info.call_start_time = TimeHelper::getCurrentTime());
}

static void _proc_hangup(const char *data, int len) {
	LOGD("[bt] hangup\n");
	if (lk::get_lylink_type() == LINK_TYPE_NULL) { _notify_call_cb(E_BT_CALL_STATE_IDLE); }
	LOCK_OP(_s_info_mutex, _s_bt_info.call_start_time = LLONG_MAX);
	_notify_call_cb(E_BT_CALL_STATE_IDLE);

	// 结束通话状态——>音量恢复
	if (_s_bt_info.calling) { audio::handle_phone(E_AUDIO_TYPE_BT_PHONE, false); }
	_s_bt_info.calling = false;
	DELAY(500);
	bt::download_call_record(BT_RECORD_RECENT);
}

static void _proc_a2dp_state(const char *data, int len) {
	switch (data[0]) {
	case '1': {     // A2DP没有连接
		LOGD("[bt] a2dp no connect\n");
		Mutex::Autolock _l(_s_info_mutex);
		bt_music_t &info = _s_bt_info.music_info;
		info.title.clear();
		info.artist.clear();
		info.album.clear();
		info.lyric.clear();
		info.curpos = 0;
		info.duration = 0;
		break;
	}
	case '2':       // A2DP 连接成功
		LOGD("[bt] a2dp connected\n");
		break;
	case '3':       // A2DP 播放中
		LOGD("[bt] a2dp playing\n");
		break;
	}
}

static void _proc_music_pause(const char *data, int len) {
	LOGD("[bt] music pause\n");
	LOCK_OP(_s_info_mutex, _s_bt_info.music_playing = false);
	_notify_music_cb(E_BT_MUSIC_STATE_PAUSE);
}

static void _proc_music_play(const char *data, int len) {
	LOGD("[bt] music play\n");
	LOCK_OP(_s_info_mutex, _s_bt_info.music_playing = true);
	if (lk::get_lylink_type() != LINK_TYPE_WIFIAUTO && lk::get_lylink_type() != LINK_TYPE_USBAUTO) {		//其他多种
		audio::change_audio_type(E_AUDIO_TYPE_BT_MUSIC);
		_notify_music_cb(E_BT_MUSIC_STATE_PLAY);
	}
//	_notify_music_cb(E_BT_MUSIC_STATE_PLAY);
}

static void _proc_music_info(const char *data, int len) {
	// [lyric]  [name][artist][time][index][count][album]

	std::vector<std::string> res;
	fy::strings::split(res, std::string(data, len), 0xFF);

//	for (size_t i = 0; i < res.size(); ++i) {
//		LOGD("[bt] %d: %s\n", i, res[i].c_str());
//	}

	{
		Mutex::Autolock _l(_s_info_mutex);
		bt_music_t &info = _s_bt_info.music_info;

		if (res.size() == 6) {
			info.lyric.clear();
			info.title = res.at(0);
			info.artist = res.at(1);
			info.album = res.at(5);
			info.duration = atoi(res.at(2).c_str()) / 1000;
		} else if (res.size() == 7) {
			info.lyric = res.at(0);
			info.title = res.at(1);
			info.artist = res.at(2);
			info.album = res.at(6);
			info.duration = atoi(res.at(3).c_str()) / 1000;
		}
		else {
			info = bt_music_t();
		}

//		LOGD("[bt] music info title: %s, artist: %s, album: %s\n",
//				info.title.c_str(), info.artist.c_str(), info.album.c_str());
	}

//	_notify_music_cb(E_BT_MUSIC_STATE_PROGRESS);
//	if (lk::music_is_connected()) {	//其他多种
//		return;
//	}
	if (!_s_bt_info.bt_is_mute) {
		_notify_music_cb(E_BT_MUSIC_STATE_PROGRESS);
	}
}

static void _proc_music_play_progress(const char *data, int len) {
	std::vector<std::string> res;
	fy::strings::split(res, std::string(data, len), 0xFF);

//	for (size_t i = 0; i < res.size(); ++i) {
//		LOGD("[bt] %d: %s\n", i, res[i].c_str());
//	}

	bt_music_t &info = _s_bt_info.music_info;
	int play_state = 0;
	if (res.size() == 3) {
		info.duration = atoi(res.at(0).c_str()) / 1000;
		info.curpos = atoi(res.at(1).c_str()) / 1000;
		play_state = atoi(res.at(2).c_str());
	} else {
		info = bt_music_t();
	}


//	if (lk::music_is_connected()) {//lk::get_lylink_type() == LINK_TYPE_WIFIAUTO
//		bt::set_bt_mute(false);
//		return;
//	}
//	else if (lk::is_connected()) {
//		return;
//	}

//	if (lk::music_is_connected()) {	//其他多种
//		return;
//	}

	if (!_s_bt_info.bt_is_mute) {
		_notify_music_cb(E_BT_MUSIC_STATE_PROGRESS);
	}

	LOGD("[bt] music play progress duration: %d, curpos: %d\n", info.duration, info.curpos);
#if BT_MODULE == BT_MODULE_LYGOC
// mcubt 改动指令     播放状态 0:stopped,  1:playing,  2:paused, 3fwd_seek, 4:rev_seek
	if ((play_state == 2 || play_state == 0) && music_is_playing()) {
		LOCK_OP(_s_info_mutex, _s_bt_info.music_playing = false);
		LOGD("蓝牙音乐暂停!");
		_notify_music_cb(E_BT_MUSIC_STATE_PAUSE);
	} else if (play_state == 1  && !music_is_playing()) {
		LOCK_OP(_s_info_mutex, _s_bt_info.music_playing = true);
		LOGD("蓝牙音乐播放!");
		_notify_music_cb(E_BT_MUSIC_STATE_PAUSE);
		_notify_music_cb(E_BT_MUSIC_STATE_PROGRESS);
	} else {
		_notify_music_cb(E_BT_MUSIC_STATE_PROGRESS);
	}
#else
	_notify_music_cb(E_BT_MUSIC_STATE_PROGRESS);
#endif
}

static std::string trim_blank(std::string str){
	str.erase(remove(str.begin(),str.end(),' '),str.end());
	return str;
}
static void _proc_download_contact(const char *data, int len) {
//	LOGD("[bt] contact: %s \n", std::string(data, len).c_str());
	if (!_s_bt_info.is_downloading_phone) {
		LOGD("--%d-- --%s-- --联系人--下载已结束--!\n", __LINE__, __FILE__);
		return;
	}
	int name_len = atoi(std::string(data, 2).c_str());
	int num_len = atoi(std::string(data+2, 2).c_str());

	bt_contact_t info;
	info.name = std::string(data + 4, name_len);
	info.num = std::string(data + 4 + name_len, num_len);
	info.num = trim_blank(info.num);
	//LOGD("contact name: %s, num: %s\n", info.name.c_str(), info.num.c_str());

//	LOGD("contact name: %s, num: %s, pinyin: %s\n", info.name.c_str(), info.num.c_str(), info.pinyin.c_str());

	LOCK_OP(_s_info_mutex, _s_bt_info.contact_list.push_back(info));
	_notify_download_cb(E_BT_DOWNLOAD_CONTACT_DATA);
}

static void _proc_sync_phone(const char *data, int len) {
	start_download();
}

static void _proc_download_contact_end(const char *data, int len) {
	LOGD("[bt] download contact end\n");
//	LOCK_OP(_s_info_mutex, phone_book_sort.start(_s_bt_info.contact_list));
	_notify_download_cb(E_BT_DOWNLOAD_CONTACT_COMPLETED);
	LOCK_OP(_s_info_mutex, _s_bt_info.is_downloading_phone = false)
}

static void _proc_download_record(const char *data, int len) {
	if (!_s_bt_info.is_downloading_record) {
		LOGD("--%d-- --%s-- --通话记录--下载已结束--!\n", __LINE__, __FILE__);
		return;
	}

	std::vector<std::string> resV;
	fy::strings::split(resV, std::string(data+1, len-1), 0xFF);

//	for (size_t i = 0; i < resV.size(); ++i) {
//		LOGD("[bt] %d: %s\n", i, resV[i].c_str());
//	}

	bt_record_t record;
	switch(data[0]) {
	case '4':
		record.type = E_BT_RECORD_TYPE_DIAL;
		break;
	case '5':
		record.type = E_BT_RECORD_TYPE_ANSWER;
		break;
	case '6':
		record.type = E_BT_RECORD_TYPE_MISSED;
		break;
	}
	if (resV.size() == 3) {
		record.name = resV.at(0);
		record.num = resV.at(1);
		record.time = to_time_format_str(resV.at(2));
	} else {
		record = bt_record_t();
	}


	LOCK_OP(_s_info_mutex, _s_bt_info.record_list.push_back(record));

	_notify_download_cb(E_BT_DOWNLOAD_RECORD_DATA);
}

static void _proc_download_record_end(const char *data, int len) {
	LOGD("[bt] download record end\n");
	fy::run_thread(_records_sort_thread, NULL);
//	_notify_download_cb(E_BT_DOWNLOAD_RECORD_COMPLETED);
//	LOCK_OP(_s_info_mutex, _s_bt_info.is_downloading_record = false);
}

static void _proc_car_sound(const char *data, int len) {
	LOGD("[bt] car sound\n");
	_s_bt_info.car_sound = true;
	call_vol(audio::get_lylink_call_vol());
	_notify_misc_info_cb(E_BT_MISC_INFO_CAR_SOUND);
}

static void _proc_phone_sound(const char *data, int len) {
	LOGD("[bt] phone sound\n");
	_s_bt_info.car_sound = false;
	_notify_misc_info_cb(E_BT_MISC_INFO_PHONE_SOUND);
}

static void _proc_auth(const char *data, int len) {
	if (data[0] == '0') {	// VS0
		LOGD("\n\n  --%d-- --%s-- 蓝牙授权成功! \n\n", __LINE__, __FILE__);
		LOCK_OP(_s_info_mutex, _s_bt_info.is_authorized = true);

		uint8_t otp_data[BT_LIC_SIZE];
		if (!storage::otp_read_data(BT_LIC_OFFSET, otp_data, BT_LIC_SIZE)) {
			LOGE("[bt] otp read data err !!!\n");
			return;
		}

		uint32_t lic_size = 0;
		uint8_t *lic_data = fy::files::load_data(BLINK_LIC, lic_size);
		if (!lic_data) {
			LOGE("[bt] load lic file err !!!\n");
			return;
		}

		if (lic_size == BT_LIC_SIZE) {
			if (memcmp(otp_data, lic_data, BT_LIC_SIZE) != 0) {
				if (storage::otp_write_data(BT_LIC_OFFSET, lic_data, BT_LIC_SIZE)) {
					_notify_misc_info_cb(E_BT_MISC_INFO_AUTH);
				}
			} else {
				_notify_misc_info_cb(E_BT_MISC_INFO_AUTH);
			}
		} else {
			LOGE("[bt] lic size %d != %d\n", lic_size, BT_LIC_SIZE);
		}

		free(lic_data);
	}
}

static void _proc_error_tips(const char *data, int len) {
	LOGE("[bt] error !!!\n");
}

static void _proc_control_data(const char *data, int len) {
	_notify_square_data_cb(std::string(data, len).c_str());
}

// 指令解析表
static proc_fun _s_proc_fun_tab[] = {
	"IS", _proc_power_on,             // 蓝牙初始化完成
	"ST", _proc_power_off,            // 关闭蓝牙

	"MM", _proc_dev_name,             // 模块设备名
	"DB", _proc_dev_addr,             // 模块设备地址
	"MN", _proc_pin_code,             // 模块 PIN 码
	"MF", _proc_auto_answer_conn,     // 自动应答、模块上电自动重连

	"SF", _proc_scan,                 // 搜索附近蓝牙设备
	"SH", _proc_scan_end,             // 扫描结束
	"MX", _proc_match_device,         // 匹配过的蓝牙设备

	"MG", _proc_hfp_state,            // HFP的状态  0:初始化, 1:待机状态, 2:连接中, 3:连接成功, 4:电话拨出, 5:电话打入, 6:通话中
	"IA", _proc_disconnect,           // HFP断开
	"IB", _proc_connected,            // HFP已连接
	"SA", _proc_connect_name,         // 当前已连接蓝牙设备名字
	"JH", _proc_connect_addr,         // 当前已连接蓝牙设备地址

	"IR", _proc_call_outgoing,        // 去电
	"ID", _proc_call_incoming,        // 来电
	"IG", _proc_answer,               // 接听
	"IF", _proc_hangup,               // 挂机

	"MU", _proc_a2dp_state,           // A2DP状态      1: A2DP没有连接     2: A2DP 连接成功    3: A2DP 播放中
	"MA", _proc_music_pause,          // 音乐暂停
	"MB", _proc_music_play,           // 音乐播放
	"MI", _proc_music_info,           // 蓝牙音乐信息
	"MP", _proc_music_play_progress,  // 蓝牙音乐进度信息

	"PA", _proc_sync_phone, 		  // 同步电话本或通话记录
	"PB", _proc_download_contact,	  // 电话本下载
	"PC", _proc_download_contact_end, // 电话本下载结束
	"PD", _proc_download_record,      // 通话记录下载
	"PE", _proc_download_record_end,  // 通话记录下载完成

	"T0", _proc_car_sound,            // 车机出声音
	"T1", _proc_phone_sound,          // 手机出声音

	"VS", _proc_auth,                 // 保存授权码
	"ER", _proc_error_tips,           // 出错

	"HD", _proc_control_data,		  //方控数据
	"BR", _proc_control_data,		  //方控数据
};

static const char *_s_raw_data_tab[] = {
	"IB",    // 已连接
	"IA",    // 断开连接
	"SA",    // 已连接设备名称
	"JH",    // 已连接设备地址
	"MZ",    // 模块设备地址
	"SR",
	"PR",
	"SV",
	"SI",
	"SS",
};


static bool _send_cmd(const char *cmd) {
	char data[256];
	sprintf(data, "AT#%s\r\n", cmd);

	bool ret = send_data(data, strlen(data));
	LOGD("[bt] send cmd %s %s\n", data, ret ? "ok" : "fail");

	return ret;
}

// 初始化设置
static void _init_setup() {
	// 蓝牙开关
	bool bten = storage::get_bool(BT_POWER_KEY, true);
	bool btcn = storage::get_bool(BT_AUTO_CONNECT_KEY, true);
	LOGD("--%d-- --%s-- 蓝牙初始化!   bten = %d \n", __LINE__, __FILE__, bten);

	if (bten) {
		power_on();
	}
	if (btcn) {
		_s_bt_info.auto_connect = btcn;
		set_auto_connect(btcn);
	}
	// 查询状态信息
	query_state();
	query_bt_initia();
}

bool bt_is_authored() {
	Mutex::Autolock _l(_s_info_mutex);
#if BT_MODULE == BT_MODULE_LYGOC
	return true;
#elif
	return _s_bt_info.is_authorized;
#endif
}

void init() {
	fy::run_thread(_bt_thread, NULL);
}

void add_cb(bt_cb_t *cb) {
	Mutex::Autolock _l(_s_cb_mutex);
	_s_cb_set.insert(cb);
}

void remove_cb(bt_cb_t *cb) {
	Mutex::Autolock _l(_s_cb_mutex);
	_s_cb_set.erase(cb);
}

bool power_on() {
	_notify_power_cb(E_BT_POWER_STATE_CHANGING);
	storage::put_bool(BT_POWER_KEY, true);
	return _send_cmd(BT_CMD_OPEN_POWER);
}

bool power_off() {
	_notify_power_cb(E_BT_POWER_STATE_CHANGING);
	storage::put_bool(BT_POWER_KEY, false);
	return _send_cmd(BT_CMD_CLOSE_POWER);
}

bool is_on() {
	return _s_bt_info.is_on;
}

bool start_scan() {
	return _send_cmd(BT_CMD_START_SCAN);
}

bool stop_scan() {
	return _send_cmd(BT_CMD_STOP_SCAN);
}

bool is_scan() {
	return _s_bt_info.scanning;
}

bool is_scan_bs(){
	return _send_cmd(BT_CMD_START_SCAN_BS);
}


uint32_t get_scan_dev_size() {
	Mutex::Autolock _l(_s_info_mutex);
	return _s_bt_info.scan_dev_map.size();
}

bool get_scan_dev_by_index(int index, bt_dev_t &dev) {
	Mutex::Autolock _l(_s_info_mutex);

	std::map<std::string, std::string>::const_iterator it = _s_bt_info.scan_dev_map.begin();
	for (int i = 0; it != _s_bt_info.scan_dev_map.end(); ++i, ++it) {
		if (i == index) {
			dev.addr = it->first;
			dev.name = it->second;
			return true;
		}
	}
	return false;
}

uint32_t get_matched_dev_size() {
	Mutex::Autolock _l(_s_info_mutex);
	return _s_bt_info.matched_dev_map.size();
}

bool get_matched_dev_by_index(int index, bt_dev_t &dev) {
	Mutex::Autolock _l(_s_info_mutex);

	std::map<std::string, std::string>::const_iterator it = _s_bt_info.matched_dev_map.begin();
	for (int i = 0; it != _s_bt_info.matched_dev_map.end(); ++i, ++it) {
		if (i == index) {
			dev.addr = it->first;
			dev.name = it->second;
			return true;
		}
	}
	return false;
}

bool connect(const char *addr) {
	char cmd[64];
	snprintf(cmd, sizeof(cmd), "%s%s", BT_CMD_CONNECT, addr);
	return _send_cmd(cmd);
}

bool disconnect() {
	return _send_cmd(BT_CMD_DISCONNECT);
}

bool start_match() {
	return _send_cmd(BT_CMD_ENABLE_MATCH);
}

bool clear_match_device() {
	LOCK_OP(_s_info_mutex, _s_bt_info.matched_dev_map.clear());
	return _send_cmd(BT_CMD_CLE_MATCH);
}

bool set_auto_connect(bool auto_connect) {
	storage::put_bool(BT_AUTO_CONNECT_KEY, auto_connect);
	return _send_cmd(auto_connect ? BT_CMD_OPEN_AUTOCONNECT : BT_CMD_CLOSE_AUTOCONNECT);
}

bool is_auto_connect() {
	return _s_bt_info.auto_connect;
}

bt_connect_state_e get_connect_state() {
	return _s_bt_info.connect_state;
}

bt_dev_t get_connect_dev() {
	Mutex::Autolock _l(_s_info_mutex);
	return _s_bt_info.connect_dev;
}

bool dial(const char *num) {
	char cmd[64];
	snprintf(cmd, sizeof(cmd), "%s%s", BT_CMD_DIAL, num);
	return _send_cmd(cmd);
}

bool redial(const char *num) {
	char cmd[512];
	sprintf(cmd, "%s%s", BT_CMD_DIAL_KEY_NUM, num);
	return _send_cmd(cmd);
}

bool answer() {
	return _send_cmd(BT_CMD_ANSWER);
}

bool reject() {
	return _send_cmd(BT_CMD_REJECT);
}

bool hangup() {
	return _send_cmd(BT_CMD_TERMINATE);
}

bool dial_key_num(const char *num) {
	char cmd[64];
	snprintf(cmd, sizeof(cmd), "%s%s", BT_CMD_DIAL_KEY_NUM, num);
	return _send_cmd(cmd);
}

bool sound_to_phone() {
	return _send_cmd(BT_CMD_SOUND_TO_PHONE);
}

bool sound_to_car() {
	return _send_cmd(BT_CMD_SOUND_TO_CAR);
}

bool call_vol(float vol) {
	//vol = (vol < 0) ? 0 : (vol > 10 ? 10 : vol);
	char cmd[64];
	snprintf(cmd, sizeof(cmd), "%s%d", BT_CMD_CALL_VOL, (int)(vol*10.0));
	return _send_cmd(cmd);
}

bool set_mic_mute(bool mute) {
	char cmd[16];
	sprintf(cmd, "%s%d", BT_CMD_MIC_SWITCH, mute ? 0 : 1);
	return _send_cmd(cmd);
}

bool set_auto_answer(bool auto_answer, int time_s) {
	if (auto_answer) {
		char cmd[16];
		sprintf(cmd, "%s%d", BT_CMD_OPEN_AUTOANSWER, time_s);
		return _send_cmd(cmd);
	} else {
		return _send_cmd(BT_CMD_CLOSE_AUTOANSWER);
	}
}

bool is_calling() {
	return _s_bt_info.calling;
}

bool is_car_sound() {
	return _s_bt_info.car_sound;
}

bool is_mic_mute() {
	return _s_bt_info.mic_mute;
}

bool is_auto_answer() {
	return _s_bt_info.auto_answer;
}

std::string get_call_num() {
	Mutex::Autolock _l(_s_info_mutex);
	return _s_bt_info.call_num;
}

std::string get_call_contact() {
	Mutex::Autolock _l(_s_info_mutex);
	for (size_t i=0; i<_s_bt_info.contact_list.size(); i++) {
		if (_s_bt_info.contact_list[i].num == _s_bt_info.call_num) {
			return _s_bt_info.contact_list[i].name;
		}
	}
	return "";
}

bt_call_state_e get_call_state() {
	Mutex::Autolock _l(_s_info_mutex);
	return _s_bt_info.call_state;
}

uint32_t get_talk_time() {
	Mutex::Autolock _l(_s_info_mutex);
	if (_s_bt_info.call_state != E_BT_CALL_STATE_TALKING) {
		return 0;
	}

	return (uint32_t) fy::time::uptime() - _s_bt_info.talk_start_time;
}


long long get_call_start_time() {
	Mutex::Autolock _l(_s_info_mutex);
	return _s_bt_info.call_start_time;
}

void set_call_start_time(long long new_time) {
	Mutex::Autolock _l(_s_info_mutex);
	_s_bt_info.call_start_time = new_time;
}


bt_music_t get_music_info() {
	Mutex::Autolock _l(_s_info_mutex);
	return _s_bt_info.music_info;
}

//bool music_set_mute(bool mute, bool calling) {
//	LOCK_OP(_s_info_mutex, _s_bt_info.bt_is_mute = mute);
//	if (!mute && !calling) {
//		audio::change_audio_type(E_AUDIO_TYPE_BT_MUSIC);
//	}
//	return _send_cmd(mute ? BT_CMD_MUSIC_PM0 : BT_CMD_MUSIC_PM1);
//}

bool set_bt_mute(bool mute, bool is_change) {
	LOCK_OP(_s_info_mutex, _s_bt_info.bt_is_mute = mute);
	if (is_change) {
		audio::change_audio_type(E_AUDIO_TYPE_BT_MUSIC);
	}
	return _send_cmd(mute ? BT_CMD_MUSIC_PM0 : BT_CMD_MUSIC_PM1);
}

bool get_bt_is_mute() {
	return _s_bt_info.bt_is_mute;
}

bool query_state() {
	return _send_cmd(BT_CMD_QUERY_STATE);
}

bool query_bt_initia(){
	return _send_cmd(BT_CMD_BT_INITIA);
}

bool query_matched() {
	return _send_cmd(BT_CMD_QUERY_MATCH_ADDR);
}

bool query_authorization() {
	return _send_cmd(BT_CMD_QUERY_AUTHORIZATION);
}

bool query_mf(){
	return _send_cmd(BT_CMD_QUERY_AUTOANSWERCON);
}

bool modify_name(const char *name) {
	char cmd[64];
	snprintf(cmd, sizeof(cmd), "%s%s", BT_CMD_QUERY_MODIFY_NAME, name);
	return _send_cmd(cmd);
}

bool modify_pin(const char *pin) {
	char cmd[64];
	snprintf(cmd, sizeof(cmd), "%s%s", BT_CMD_QUERY_MODIFY_PIN, pin);
	return _send_cmd(cmd);
}

bool get_is_authorized() {
	Mutex::Autolock _l(_s_info_mutex);
	return _s_bt_info.is_authorized;
}

std::string get_name() {
	Mutex::Autolock _l(_s_info_mutex);
	return _s_bt_info.dev_info.name;
}

std::string get_pin() {
	Mutex::Autolock _l(_s_info_mutex);
	return _s_bt_info.pin_code;
}

bool download_phone_book() {
//	LOGD("--%d-- --%s-- is_downloading = %d \n", __LINE__, __FILE__, _s_bt_info.is_downloading);
	if (!_s_bt_info.is_downloading_phone && !_s_bt_info.is_downloading_record) {
		{
			Mutex::Autolock _l(_s_info_mutex);
			_s_bt_info.is_downloading_phone = true;
			_s_bt_info.contact_list.clear();
		}
		return _send_cmd(BT_CMD_PHONE_BOOK2);
	}
	return false;
}

bool delete_phone_book() {
	Mutex::Autolock _l(_s_info_mutex);
	_s_bt_info.is_downloading_phone = false;
	_s_bt_info.contact_list.clear();
	return false;
}

bool download_call_record(int record) {
//	LOGD("--%d-- --%s-- is_downloading = %d \n", __LINE__, __FILE__, _s_bt_info.is_downloading);
	const char *cmd;
	switch (record) {
	case BT_RECORD_OUTGOING:
		cmd = BT_CMD_RECORD_OUTGOING;
		break;
	case BT_RECORD_INCOMING:
		cmd = BT_CMD_RECORD_INCOMING;
		break;
	case BT_RECORD_MISSED:
		cmd = BT_CMD_RECORD_MISSED;
		break;
	case BT_RECORD_RECENT:
		cmd = BT_CMD_RECORD_RECENT;
		break;
	}
	if (!_s_bt_info.is_downloading_record && !_s_bt_info.is_downloading_phone) {
		{
			Mutex::Autolock _l(_s_info_mutex);
			_s_bt_info.is_downloading_record = true;
			_s_bt_info.record_list.clear();
		}

		return _send_cmd(cmd);
	}
	return false;
}

bool delete_call_record() {
	Mutex::Autolock _l(_s_info_mutex);
	_s_bt_info.is_downloading_record = false;
	_s_bt_info.record_list.clear();
	return false;
}

bool start_download() {
	return _send_cmd(BT_CMD_SYNC_PB_OR_CR);
}

uint32_t get_contact_size() {
	Mutex::Autolock _l(_s_info_mutex);
	return _s_bt_info.contact_list.size();
}

bool get_contact_by_index(int index, bt_contact_t &contact) {
	Mutex::Autolock _l(_s_info_mutex);
	if (index < (int)_s_bt_info.contact_list.size()) {
		contact = _s_bt_info.contact_list[index];
		return true;
	}
	return false;
}

uint32_t get_record_size() {
	Mutex::Autolock _l(_s_info_mutex);
	return _s_bt_info.record_list.size();
}

bool get_record_by_index(int index, bt_record_t &record) {
	Mutex::Autolock _l(_s_info_mutex);
	if (index < (int)_s_bt_info.record_list.size()) {
		record = _s_bt_info.record_list[index];
		return true;
	}
	return false;
}

int find_first_contact_by_initial(char initial) {
	Mutex::Autolock _l(_s_info_mutex);
	for (size_t i=0; i<_s_bt_info.contact_list.size(); i++) {
		if (tolower(initial) == tolower(_s_bt_info.contact_list[i].pinyin[0])) {
			return i;
		}
	}
	return -1;
}

bool music_play() {
	if(_s_bt_info.music_playing == true){
		_s_bt_info.music_playing = false;
	}else{
		_s_bt_info.music_playing = true;
	}
	return _send_cmd(BT_CMD_MUSIC_PLAY);
}

bool music_pause() {
	if(_s_bt_info.music_playing == true){
		_s_bt_info.music_playing = false;
	}else{
		_s_bt_info.music_playing = true;
	}
	return _send_cmd(BT_CMD_MUSIC_PLAY);
}

bool music_next() {
	return _send_cmd(BT_CMD_MUSIC_NEXT);
}

bool music_prev() {
	return _send_cmd(BT_CMD_MUSIC_PREV);
}

bool music_is_playing() {
	return _s_bt_info.music_playing;
}

bool start_scan_control() {
	return _send_cmd(BT_CMD_START_SCAN_BS);
}

bool stop_scan_control() {
	return _send_cmd(BT_CMD_START_SCAN_VB);
}

bool query_name() {
	return _send_cmd(BT_CMD_QUERY_CONNECT_NAME);
}


/************************ 南硅mcu指令 *************************/
bool sound_to_arm() {
	return _send_cmd("AU");
}

bool sound_to_mcu() {
	return _send_cmd("BT");
}

bool set_volume(float vol) {
	vol = (vol < 0.0) ? 0.0 : ((vol > 1.0) ? 1.0 : vol);

	char cmd[16];
	sprintf(cmd, "VO%02d", (int) (vol * 16));
	return _send_cmd(cmd);
}
/************************************************************/

bool send_data(const char *data, int len) {
	Mutex::Autolock _l(_s_send_lock);
	RETURN_VALUE_IF_FAIL(_s_bt_fd >= 0, false)
	return (write(_s_bt_fd, data, len) == len);
}


// 检测授权信息
static void _check_lic() {
#if BT_MODULE == BT_MODULE_BLINK
	if (fy::files::exists(BLINK_LIC) && (fy::files::size_of(BLINK_LIC) == BT_LIC_SIZE)) {
		return;
	}

	if (!fy::files::exists(BLINK_DIR)) {
		mkdir(BLINK_DIR, 0666);
	}

	storage::otp_load_data_to_file(BT_LIC_OFFSET, BT_LIC_SIZE, BLINK_LIC);
#endif
}

static bool _start_bt_service() {
#if BT_MODULE == BT_MODULE_BLINK
	// 生成运行脚本
	fy::files::create_script(BT_BIN_SH, BT_SCRIPT);
	// 启动蓝牙服务
	START_SERVICE(BT_SERVICE);
	// 等待虚拟串口创建完成
	WAIT(FILE_EXIST(BT_SERIAL), 500, 5);
#endif
	return FILE_EXIST(BT_SERIAL);
}

// 解析协议数据, 返回已解析的数据长度
static int _parse_data(const char *data, int len) {
	int start_index = 0;
	for (int i = 0; i <= len - 2; ++i) {
		// end flag \r\n
		if ((data[i] != '\r') || (data[i + 1] != '\n')) {
			continue;
		}

		int frame_len = i - start_index;
		if (frame_len >= 2) {
			const char *frame_data = data + start_index;

#if 1
			std::string frame_str(data + start_index, frame_len);
			LOGD("[bt] frame data: %s\n", frame_str.c_str());
#endif

			for (size_t j = 0; j < TAB_SIZE(_s_proc_fun_tab); ++j) {
				if (strncmp(_s_proc_fun_tab[j].cmd, frame_data, 2) == 0) {
					_s_proc_fun_tab[j].proc(frame_data + 2, frame_len - 2);
					break;
				}
			}

			for (size_t j = 0; j < TAB_SIZE(_s_raw_data_tab); ++j) {
				if (strncmp(_s_raw_data_tab[j], frame_data, 2) == 0) {
					_notify_raw_data_cb(frame_data, frame_len);
					break;
				}
			}
		}

		start_index = i + 2;
	}

	return start_index;
}

static void* _bt_thread(void *args) {
	LOGD("[bt] thread start...\n");

	_check_lic();

	if (!_start_bt_service()) {
		LOGE("[bt] start service fail\n");
		return NULL;
	}

	_s_bt_fd = fy::os::open_uart(BT_SERIAL, BT_BAUDRATE);
	if (_s_bt_fd < 0) {
		return NULL;
	}

	_init_setup();

	char data_buf[DATA_BUF_LEN];
	int data_len = 0;

	while (true) {
		if (fy::asio::readable(_s_bt_fd, -1) <= 0) {
			LOGE("[bt] asio readable err\n");
			continue;
		}

		int len = read(_s_bt_fd, data_buf + data_len, DATA_BUF_LEN - data_len);
		if (len <= 0) {
			LOGE("[bt] read err\n");
			continue;
		}

		data_len += len;

		len = _parse_data(data_buf, data_len);
		if ((len > 0) && (len < data_len)) {
			memcpy(data_buf, data_buf + len, data_len - len);
		}

		data_len -= len;
	}

	close(_s_bt_fd);
	_s_bt_fd = -1;

	return NULL;
}

}
