/*
 * setting.h
 *
 *  Created on: 2023年4月17日
 *      Author: ZKSWE Develop Team
 */

#ifndef _SYSTEM_SETTING_H_
#define _SYSTEM_SETTING_H_
#include "media/audio_player.h"
#include "media/media_base.h"
#include "common.h"


typedef enum {
	E_TYPE_ENGINE,//互联声音从车机发出
	E_TYPE_PHONE,//互联声音从手机发出
}linkvioce_mode_e;


typedef enum {
	E_TYPE_AB,		//互联国外版本（android auto）
	E_TYPE_CN,		//互联国内版本（hicar、carlife）
}LINK_TYPE_E;

typedef enum {
	E_LINK_MODE_CARPLAY,       // carplay
	E_LINK_MODE_ANDROIDAUTO,   // android auto
	E_LINK_MODE_CARLIFE,      	// carlife
	E_LINK_MODE_HICAR,       	// hicar
	E_LINK_MODE_AIRPLAY,       // airplay
	E_LINK_MODE_MIRACAST,      // miracast
	E_LINK_MODE_LYLINK,		   //AICast
	E_LINK_MODE_BLUEMUSIC,     // 蓝牙音乐
} link_mode_e;

typedef enum {
	E_SOUND_MODE_SPK,     // 喇叭出声
	E_SOUND_MODE_LINK,    // 互联手机出声
	E_SOUND_MODE_FM,      // FM发射
	E_SOUND_MODE_AUX,     // aux
} sound_mode_e;

typedef void (*play_mode_state_cb) (audio_player_mode_e mode);



namespace sys {
namespace setting {

void init();

/*
 * 声音模式
 */
void set_current_sys_player_mode(audio_player_mode_e mode);
audio_player_mode_e get_current_sys_player_mode();
void add_play_state_cb(play_mode_state_cb cb);
void remove_play_state_cb(play_mode_state_cb cb);

/*
 * 互联版本类型   国内/国外版
 */
void set_link_type(LINK_TYPE_E type);
LINK_TYPE_E get_link_type();

// 是否使用carlife
void set_is_use_link_carlife(bool is_use);
bool get_is_use_link_carlife();


/**
 * @brief 设置互联声音类型（从手机或者车机发出,目前只支持carplay）
 * @param type : E_ENGINE车机发出     E_PHONE手机发出
 */
void set_linksound_type(linkvioce_mode_e type);
linkvioce_mode_e get_linksound_type();

const char* get_link_mode_str(link_mode_e mode);
link_mode_e get_link_mode();
void set_link_mode(link_mode_e mode);

/**
 * @brief 设置摄像头通道
 * @param chn 范围：0 ~ 3
 */
void set_camera_chn(int chn);

/**
 * @brief 获取摄像头通道
 */
int get_camera_chn();

/**
 * @brief 设置跳过前面几帧图像，摄像头刚打开那会数据不稳定会有抖动的现象，可通过设置该参数来跳过
 * @param skip
 */
void set_camera_skip(int skip);

/**
 * @brief 获取跳过了前面多少帧图像
 */
int get_camera_skip();

/**
 * @brief  设置摄像头节点  ahd 或 cvbs
 * @param dev  "/dev/video0"AHD  "/dev/video4"CVBS
 */
void set_camera_dev(const char *dev);

/**
 * @brief 获取摄像头节点
 */
char *get_camera_dev();

/**
 * @brief 获取摄像头图像宽
 */
int get_camera_wide();

/**
 * @brief 获取摄像头图像高
 */
int get_camera_high();

/**
 * @brief 设置摄像头分辨率
 * @param wide宽     high高
 */
void set_camera_size(int wide,int high);

/**
 * @brief 获取摄像头帧率
 */
int get_camera_rate();

/**
 * @brief 设置摄像头帧率
 * @param rate帧率
 */
void set_camera_rate(int rate);

/**
 * @brief 获取摄像头旋转角度
 */
int get_camera_rot();

/**
 * @brief 设置旋转角度，默认0不旋转，如果屏幕有旋转才需要设置
 */
void set_camera_rot(int rot);

int get_camera_buf_count();
void set_camera_buf_count(int cnt);
int get_camera_mem_type();
void set_camera_mem_type(int type);

/**
 * @brief 设置亮度值
 * @param brightness 范围：0 ~ 100
 */
void set_brightness(int brightness);

/**
 * @brief 获取亮度值
 */
int get_brightness();

/**
 * @brief 设置对比度
 * @param contrast 范围：0 ~ 100
 */
void set_contrast(int contrast);

/**
 * @brief 获取对比度
 */
int get_contrast();

/**
 * @brief 设置饱和度
 * @param saturation 范围：0 ~ 100
 */
void set_saturation(int saturation);

/**
 * @brief 获取饱和度
 */
int get_saturation();

/**
 * @brief 设置色调
 * @param hue 范围：0 ~ 100
 */
void set_hue(int hue);

/**
 * @brief 获取色调
 */
int get_hue();

/**
 * @brief 设置明度
 * @param luminance 范围：0 ~ 100
 */
void set_luminance(int luminance);

/**
 * @brief 获取明度
 */
int get_luminance();

/**
 * @brief 开背光
 */
void backlight_on();

/**
 * @brief 关背光
 */
void backlight_off();

/**
 * @brief 关屏
 */
void screen_off();

/**
 * @brief 开屏
 */
void screen_on();

/**
 * @brief 是否是开屏状态
 */
bool is_screenon();

/**
 * @brief 切换多国语言
 * @param code 语言编码
 */
void update_localescode(const char *code);

/*
 * @brief 设置日期
 */
bool set_DateTime(struct tm *ptm);

/*
 * @brief usb adb是否开启
 */
bool is_usb_adb_enabled();

/*
 * @brief 开关usb adb
 */
void enable_usb_adb(bool enabled);

/*
 * @brief 获取设备的名称
 */
std::string get_dev_name();

/*
 * @brief 设置蓝牙名称
 */
void set_bt_name(const std::string &name);

/*
 * @brief 获取蓝牙名称
 */
std::string get_bt_name();

void set_link_mode(link_mode_e mode);
link_mode_e get_link_mode();

void set_sound_mode(sound_mode_e mode);
sound_mode_e get_sound_mode();

/*
 * @brief 获取胎压模块保存的数据
 */
void set_front_tire_data(std::string& data);
std::string get_front_tire_data();

void set_rear_tire_data(std::string& data);
std::string get_rear_tire_data();

/*
 * @brief 获取保存的胎压配置信息
 */
void set_tire_option_info(std::string& data);
std::string get_tire_option_info();

// 获取fm频率
int get_fm_frequency();
// 设置fm频率
void set_fm_frequency(int freq);

bool get_fm_switch();
void set_fm_switch(bool status);

void set_time_format_24h(bool format_24h);
bool is_time_format_24h();

// left top, right top, left bottom, right bottom
void set_reverse_line_point(const SZKPoint &lt, const SZKPoint &rt, const SZKPoint &lb, const SZKPoint &rb);
void get_reverse_line_point(SZKPoint &lt, SZKPoint &rt, SZKPoint &lb, SZKPoint &rb);

void set_reverse_line_view(bool show);
bool is_reverse_line_view();

void video_set_play_mode(media_play_mode_e mode);
media_play_mode_e video_get_play_mode();

void set_music_play_dev(audio_type_e dev);
audio_type_e get_music_play_dev();

}

}

#endif /* _SYSTEM_SETTING_H_ */
