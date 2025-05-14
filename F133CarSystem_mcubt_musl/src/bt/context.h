/*
 * context.h
 *
 *  Created on: 2023年1月4日
 *      Author: ZKSWE Develop Team
 */

#ifndef _BT_CONTEXT_H_
#define _BT_CONTEXT_H_

#include <stdio.h>
#include <stdint.h>
#include <string>
#include <vector>

// 开关状态
typedef enum {
	E_BT_POWER_STATE_OFF,                  // 关闭
	E_BT_POWER_STATE_ON,                   // 开启
	E_BT_POWER_STATE_CHANGING,             // 开启/关闭中
} bt_power_state_e;

// 连接状态
typedef enum {
	E_BT_CONNECT_STATE_IDLE,                // 未连接
	E_BT_CONNECT_STATE_CONNECTING,          // 连接中
	E_BT_CONNECT_STATE_CONNECTED,           // 已连接
} bt_connect_state_e;

// 扫描状态
typedef enum {
	E_BT_SCAN_STATE_IDLE,
	E_BT_SCAN_STATE_STARTED,                // 扫描开始
	E_BT_SCAN_STATE_SCANNING_NEW_DEV,       // 扫描到新设备
	E_BT_SCAN_STATE_SCANNING_MATCHED,       // 已配对设备
	E_BT_SCAN_STATE_COMPLETED,              // 扫描完成
} bt_scan_state_e;

// 通话状态
typedef enum {
	E_BT_CALL_STATE_IDLE,
	E_BT_CALL_STATE_OUTGOING,               // 去电
	E_BT_CALL_STATE_INCOMING,               // 来电
	E_BT_CALL_STATE_TALKING,                // 通话中
} bt_call_state_e;

// 联系人、通话记录下载状态
typedef enum {
	E_BT_DOWNLOAD_IDLE,
	E_BT_DOWNLOAD_CONTACT_STARTED,          // 开始下载联系人
	E_BT_DOWNLOAD_RECORD_STARTED,           // 开始下载通话记录
	E_BT_DOWNLOAD_CONTACT_DATA,             // 联系人数据
	E_BT_DOWNLOAD_RECORD_DATA,              // 通话记录数据
	E_BT_DOWNLOAD_CONTACT_COMPLETED,        // 联系人下载完成
	E_BT_DOWNLOAD_RECORD_COMPLETED,         // 通话记录下载完成
} bt_download_state_e;

// 音乐状态
typedef enum {
	E_BT_MUSIC_STATE_PAUSE,                 // 暂停
	E_BT_MUSIC_STATE_PLAY,                  // 播放
	E_BT_MUSIC_STATE_PROGRESS,              // 进度
} bt_music_state_e;

// 通话记录类型
typedef enum {
	E_BT_RECORD_TYPE_DIAL,     // 去电
	E_BT_RECORD_TYPE_ANSWER,   // 接听
	E_BT_RECORD_TYPE_MISSED,   // 未接听
} bt_record_type_e;

// 其他信息项
typedef enum {
	E_BT_MISC_INFO_DEV_NAME,         // 设备名称
	E_BT_MISC_INFO_DEV_ADDR,         // 设备地址
	E_BT_MISC_INFO_PIN_CODE,         // PIN码
	E_BT_MISC_INFO_CAR_SOUND,        // 车机端出声音
	E_BT_MISC_INFO_PHONE_SOUND,      // 手机端出声音
	E_BT_MISC_INFO_MIC_MUTE,         // mic静音
	E_BT_MISC_INFO_MIC_UNMUTE,       // mic解除静音
	E_BT_MISC_INFO_AUTO_ANSWER_ON,   // 自动接听打开
	E_BT_MISC_INFO_AUTO_ANSWER_OFF,  // 自动接听关闭
	E_BT_MISC_INFO_AUTO_CONNECT_ON,  // 自动连接打开
	E_BT_MISC_INFO_AUTO_CONNECT_OFF, // 自动连接关闭
	E_BT_MISC_INFO_AUTH,             // 授权
} bt_misc_info_e;

// 设备信息
typedef struct {
	std::string addr;
	std::string name;
} bt_dev_t;

// 联系人信息
typedef struct {
	std::string name;
	std::string num;
	std::string pinyin;
} bt_contact_t;

// 通话记录信息
typedef struct {
	bt_record_type_e type;
	std::string name;
	std::string num;
	std::string time;
} bt_record_t;

// 音乐信息
typedef struct {
	std::string title;
	std::string artist;
	std::string album;
	std::string lyric;
	int curpos = 0;
	int duration = 0;
} bt_music_t;

typedef struct {
	void (*power_cb)(bt_power_state_e state);        // 开关状态回调
	void (*connect_cb)(bt_connect_state_e state);    // 连接状态回调
	void (*scan_cb)(bt_scan_state_e state);          // 扫描状态回调
	void (*call_cb)(bt_call_state_e state);          // 通话状态回调
	void (*download_cb)(bt_download_state_e state);  // 下载状态回调
	void (*music_cb)(bt_music_state_e state);        // 音乐状态回调
	void (*misc_info_cb)(bt_misc_info_e info);       // 其他信息回调
	void (*raw_data_cb)(const char *data, int len);  // 原始协议数据，主要用于手机互联转发数据
	void (*square_data_cb)(const char *data);  		 // 方控数据
} bt_cb_t;

namespace bt {

bool bt_is_authored();

void init();

void add_cb(bt_cb_t *cb);
void remove_cb(bt_cb_t *cb);

/**
 * 开关
 */
bool power_on();
bool power_off();
bool is_on();

/**
 * 扫描
 */
bool start_scan();
bool stop_scan();
bool is_scan();
bool is_scan_bs();//扫描方控设备

// 扫描到的新设备
uint32_t get_scan_dev_size();
bool get_scan_dev_by_index(int index, bt_dev_t &dev);

// 已配对设备
uint32_t get_matched_dev_size();
bool get_matched_dev_by_index(int index, bt_dev_t &dev);

/**
 * 连接
 */
bool connect(const char *addr);
bool disconnect();
bool start_match();
bool clear_match_device();
bool set_auto_connect(bool auto_connect);
bool is_auto_connect();
bt_connect_state_e get_connect_state();
bt_dev_t get_connect_dev();

/**
 * 通话
 */
bool dial(const char *num);
bool redial(const char *num);
bool answer();
bool reject();
bool hangup();
bool dial_key_num(const char *num);
bool sound_to_phone();
bool sound_to_car();

// 0 ~ 1.0
bool call_vol(float vol);

bool set_mic_mute(bool mute);
bool set_auto_answer(bool auto_answer, int time_s = 5);
bool is_calling();
bool is_car_sound();
bool is_mic_mute();
bool is_auto_answer();
std::string get_call_num();
std::string get_call_contact();
bt_call_state_e get_call_state();
uint32_t get_talk_time();

/**
 * 基础信息
 */
bool query_state();
bool query_bt_initia();
bool query_matched();
bool query_authorization();
bool query_mf();

bool modify_name(const char *name);
bool modify_pin(const char *pin);

bool get_is_authorized();
std::string get_name();
std::string get_pin();

/**
 * 电话本、通话记录
 */
bool download_phone_book();
bool download_call_record();
bool start_download();
uint32_t get_contact_size();
bool get_contact_by_index(int index, bt_contact_t &contact);
uint32_t get_record_size();
bool get_record_by_index(int index, bt_record_t &record);
int find_first_contact_by_initial(char initial);


/**
 * 蓝牙音乐
 */
bool music_play();
bool music_pause();
bool music_next();
bool music_prev();
bool music_is_playing();
bt_music_t get_music_info();

/*
 * 蓝牙声音
 */
//bool music_set_mute(bool mute, bool calling = false);
bool set_bt_mute(bool mute, bool is_change = false);
bool get_bt_is_mute();

/*
 * 方控设备
 */
bool start_scan_control();
bool stop_scan_control();

/**
 * 声音 (南硅mcu指令)
 */
// 声音切换到arm端
bool sound_to_arm();
// 声音切换到mcu端
bool sound_to_mcu();
// 设置音量 0 ~ 1.0
bool set_volume(float vol);

/**
 * 发送指令数据
 */
bool send_data(const char *data, int len);

}

#endif /* _BT_CONTEXT_H_ */
