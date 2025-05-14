/*
 * fm_emit.cpp
 *
 *  Created on: 2023年5月19日
 *      Author: ZKSWE Develop Team
 */

#include <unistd.h>
#include "fm_emit.h"
#include "media/audio_player.h"
#include "utils/I2CHelper.h"
#include "utils/Log.h"
#include "misc/storage.h"
#include "setting.h"

#define I2C_DEV         1
#define I2C_SLAVE_ADDR  0x2c

#define CH_STEP       0x00
#define CH            0x01
#define CH_CH         0x03
#define FREQ2CHREG(freq)   ((freq-7600)/5)
#define CHREG2FREQ(ch)     (ch*5+7600)

#define DELAY(ms)     usleep((ms) * 1000)

#define RETURN_IF_FAIL(c)             if (!(c)) { return; }
#define RETURN_VALUE_IF_FAIL(c, v)    if (!(c)) { return (v); }


static bool _fm_is_enable = false;

namespace sys {

static bool _i2c_read_byte(I2CHelper &i2c, uint8_t read_addr, uint8_t *read_data) {
    if (!i2c.write(&read_addr, 1)) {
		LOGE("[fm] i2c write addr %x error!\n", read_addr);
		return false;
	}

    if (!i2c.read(read_data, 1)) {
    	LOGE("[fm] i2c read addr %x error!\n", read_addr);
		return false;
	}

    return true;
}

static bool _i2c_write_byte(I2CHelper &i2c, uint8_t write_addr, uint8_t write_data) {
	uint8_t buf[2];

	buf[0] = write_addr;
	buf[1] = write_data;
	if (!i2c.write(buf, 2)) {
		LOGE("[fm] i2c write addr %x, data %x error!\n", write_addr, write_data);
		return false;
	}
    return true;
}

bool fm_start() {
    I2CHelper i2c(I2C_DEV, I2C_SLAVE_ADDR, 1000, 5);
    uint8_t chipid0 = 0, chipid1 = 0;
    bool ret = false;

    do {
    	if (!_i2c_read_byte(i2c, 0x05, &chipid0)) {
    		break;
    	}
    	if (!_i2c_read_byte(i2c, 0x06, &chipid1)) {
    		break;
    	}

		LOGD("[fm] chipid: 0x%x%x\n", chipid0, chipid1);

		// Set the All the register to default values
		if (!_i2c_write_byte(i2c, 0x00, 0x81)) {
			break;
		}

		DELAY(20);

		// Using the default setting
		if (!_i2c_write_byte(i2c, 0x03, 0x10)) {
			break;
		}
		// Set the OSC frequency : 0x33 12MHz ;0xb3:24M
		if (!_i2c_write_byte(i2c, 0x04, 0x53)) {
			break;
		}
		if (!_i2c_write_byte(i2c, 0x00, 0x41)) {
			break;
		}
		if (!_i2c_write_byte(i2c, 0x00, 0x1)) {
			break;
		}

		DELAY(20);

		//SNR
		if (!_i2c_write_byte(i2c, 0x18, 0xe4)) {
			break;
		}

		if (!_i2c_write_byte(i2c, 0x1b, 0xf0)) {
			break;
		}

		//freq
		if (!_i2c_write_byte(i2c, 0x01, 0x7e)) {
			break;
		}
		//cancel pa func
		if (!_i2c_write_byte(i2c, 0x02, 0xb9)) {
			break;
		}
		//start
		if (!_i2c_write_byte(i2c, 0x00, 0x02)) {
			break;
		}

		ret = true;

		LOGD("[fm] start ok\n");
    } while (0);

	return ret;
}


bool fm_set_freq(int freq) {
	storage::put_int("fmt_freq", freq);

	LOGD("--%d-- --%s-- --%s+++ freq:%d \n", __LINE__, __FILE__, __FUNCTION__, freq);

	// calculate ch parameter used for register setting
	uint8_t tStep;
	uint8_t tCh;
	uint16_t f;
	f = FREQ2CHREG(freq);

	// set to reg: CH
	tCh = (uint8_t) f;

	I2CHelper i2c(I2C_DEV, I2C_SLAVE_ADDR, 1000, 5);

	RETURN_VALUE_IF_FAIL(_i2c_write_byte(i2c, CH, tCh), false);
	RETURN_VALUE_IF_FAIL(_i2c_read_byte(i2c, CH_STEP, &tStep), false);

	// set to reg: CH_STEP
	tStep &= ~CH_CH;
	tStep |= ((uint8_t) (f >> 8) & CH_CH);
	RETURN_VALUE_IF_FAIL(_i2c_write_byte(i2c, CH_STEP, tStep), false);
	LOGD("--%d-- --%s-- --%s--- freq:%d \n", __LINE__, __FILE__, __FUNCTION__, freq);
	return true;
}

uint16_t fm_get_freq() {
	int freq = storage::get_int("fmt_freq", 9000);
//	uint8_t tCh;
//	uint8_t  tStep;
//	uint16_t ch = 0;
//
//	I2CHelper i2c(I2C_DEV, I2C_SLAVE_ADDR, 1000, 5);
//
//	// set to reg: CH_STEP
//	RETURN_VALUE_IF_FAIL(_i2c_read_byte(i2c, CH_STEP, &tStep), 0);
//	RETURN_VALUE_IF_FAIL(_i2c_read_byte(i2c, CH, &tCh), 0);
//
//	tStep &= CH_CH;
//	ch = tStep;
//	ch = (ch << 8) + tCh;
//	return CHREG2FREQ(ch);
	return (uint16_t)freq;
}

bool fm_txreq(bool transmit) {
//	I2CHelper i2c(I2C_DEV, I2C_SLAVE_ADDR, 1000, 5);
//
//	uint8_t tValue;
//	RETURN_VALUE_IF_FAIL(_i2c_read_byte(i2c, 0x00, &tValue), false);
//
//	/*modify start*/
//	tValue &= 0x80;
//	tValue |= 0x1;
//	/*modify end*/
//
//	if (transmit) {
//		tValue |= 0x20;
//	} else {
//		tValue &= ~0x20;
//	}
//
//	RETURN_VALUE_IF_FAIL(_i2c_write_byte(i2c, 0x00, tValue), false);

	return true;
}

bool set_fmt_switch(bool mute) {
	I2CHelper i2c(I2C_DEV, I2C_SLAVE_ADDR, 1000, 5);

	uint8_t tValue;
	RETURN_VALUE_IF_FAIL(_i2c_read_byte(i2c, 0x00, &tValue), false);

	/*modify start*/
	tValue &= 0x80;
	tValue |= 0x1;
	/*modify end*/


	if (mute) {
		tValue |= 0x08;
		tValue &= ~0x20;		// 关闭transmit
	} else {
		tValue &= ~0x08;
		tValue |= 0x20;			// 打开transmit
	}

	RETURN_VALUE_IF_FAIL(_i2c_write_byte(i2c, 0x00, tValue), false);
	return true;
}

bool fm_set_mute(bool mute) {

//	I2CHelper i2c(I2C_DEV, I2C_SLAVE_ADDR, 1000, 5);
//
//	uint8_t tValue;
//	RETURN_VALUE_IF_FAIL(_i2c_read_byte(i2c, 0x00, &tValue), false);
//
//	if (mute) {
//		tValue |= 0x08;
//	} else {
//		tValue &= ~0x08;
//	}
//
//	RETURN_VALUE_IF_FAIL(_i2c_write_byte(i2c, 0x00, tValue), false);
	set_fmt_switch(mute);
	_fm_is_enable = (!mute);
	sys::setting::set_current_sys_player_mode(mute ? E_AUDIO_PLAYER_MODE_SPK : E_AUDIO_PLAYER_MODE_FM);
	zk_audio_player_set_mode(mute ? E_AUDIO_PLAYER_MODE_SPK : E_AUDIO_PLAYER_MODE_FM);
	if (!mute) {
		sys::fm_set_freq(sys::fm_get_freq());
	}
	return true;
}

bool fm_is_enable() {
	return E_AUDIO_PLAYER_MODE_FM == sys::setting::get_current_sys_player_mode();
}

void fm_dump() {
	uint8_t mem = 0;
	I2CHelper i2c(I2C_DEV, I2C_SLAVE_ADDR, 1000, 5);

	for (int i = 0; i < 0x13; i++) {
		RETURN_IF_FAIL(_i2c_read_byte(i2c, i, &mem));
		LOGD("[fm]: [0x%x]=0x%x\n", i, mem);
	}
}



}
