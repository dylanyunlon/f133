/*
 * context.h
 *
 *  Created on: 2023年11月14日
 *      Author: ZKSWE Develop Team
 */

#ifndef _UART_CONTEXT_H_
#define _UART_CONTEXT_H_

#include "ProtocolData.h"


typedef struct UpdP_s{
	UpdP_s(uint32_t _total, off_t _cur_pos) {
		total = _total;
		cur_pos = _cur_pos;
	}
	uint32_t total;
	off_t cur_pos;
} UpdProgress;

typedef enum {
	MCU_UPD_MODE_ENTRY,		// 进入升级模式
	MCU_UPD_PRGGRESS,		// UpdProgress
	MCU_UPD_FINISHED,
	MCU_VERSION,			// string
	MCU_PHONE_TIME,
} MCU_EVENT;

// 手机时间
struct mcu_time_t{
	int year;
	int month;
	int day;
	int hour;
	int min;
	int sec;
};

typedef void (*_s_mcu_cb)(MCU_EVENT event, void* data, uint32_t len);

typedef void (*fm_state_cb) (bool status);
typedef void (*fm_fre_cb) (int fre);


namespace uart {

bool init();

void add_mcubt_cb(_s_mcu_cb cb);
void remove_mcubt_cb(_s_mcu_cb cb);

void add_fm_state_cb(fm_state_cb cb);
void remove_fm_state_cb(fm_state_cb cb);

void add_fm_fre_cb(fm_fre_cb cb);
void remove_fm_fre_cb(fm_fre_cb cb);


bool set_volume(float vol);

bool set_amplifier_mute(uint8_t mute);

//  音频通道      0：ARM, 1：FM, 2：AUX(mcubt无效), 3:BT
bool set_sound_channel(sound_channel_e channel);

bool set_sound_effect(int index, int eq);

bool set_sound_field(sound_field_e field, int gain);

// 检查更新
bool query_update_file();
// 进入升级模式
bool entry_update_mode();
// 开始mcu升级
bool start_mcu_update();
// 查询版本号
bool query_mcubt_version();

//fm开关 0：关  1：开
bool fm_switch(bool status);

//fm频率 范围875-1080
bool fm_frequency(int value);

//查询fm开关状态
bool query_fmswitch();

//查询fm频率
bool query_frequency();

}

#endif /* _UART_CONTEXT_H_ */
