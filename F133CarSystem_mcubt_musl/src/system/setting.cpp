/*
 * setting.cpp
 *
 *  Created on: 2023年4月17日
 *      Author: ZKSWE Develop Team
 */

#include "misc/storage.h"
#include "setting.h"
#include "usb_monitor.h"
#include "entry/EasyUIContext.h"
#include "utils/BrightnessHelper.h"
#include "utils/TimeHelper.h"
#include "fy/os.hpp"
#include "common.h"

#define D_ZKCAMERA_PROP_DEV		  		"/dev/video0"
#define D_ZKCAMERA_PROP_SIZE	  		"1920x1080"
#define D_ZKCAMERA_PROP_RATE	 		25
#define D_ZKCAMERA_PROP_CHN		 		0
#define D_ZKCAMERA_PROP_SKIP			14
#define D_ZKCAMERA_PROP_ROT		 		0

#define D_ZKCAMERA_PROP_BUFCNT		 	3
#define D_ZKCAMERA_PROP_MEMTYPE		 	2

#define ZKCAMERA_PROP_DEV         		"persist.zkcamera.dev"
#define ZKCAMERA_PROP_SIZE        		"persist.zkcamera.size"
#define ZKCAMERA_PROP_RATE        		"persist.zkcamera.rate"
#define ZKCAMERA_PROP_ROT         		"persist.zkcamera.rot"
#define ZKCAMERA_PROP_CHN         		"persist.zkcamera.chn"
#define ZKCAMERA_PROP_SKIP        		"persist.zkcamera.skip"
#define ZKCAMERA_PROP_BUFCNT 			"persist.zkcamera.bufcount"
#define ZKCAMERA_PROP_MEMTYPE			"persist.zkcamera.memtype"

#define GET_ZKCAMERA_PROP_DEV(c) 	 	 SystemProperties::getString(ZKCAMERA_PROP_DEV,c,D_ZKCAMERA_PROP_DEV);
#define GET_ZKCAMERA_PROP_SIZE(c) 	 	 SystemProperties::getString(ZKCAMERA_PROP_SIZE, c,D_ZKCAMERA_PROP_SIZE);
#define GET_ZKCAMERA_PROP_RATE(i)     	 SystemProperties::getInt(ZKCAMERA_PROP_RATE, i, D_ZKCAMERA_PROP_RATE);
#define GET_ZKCAMERA_PROP_CHN(i)         SystemProperties::getInt(ZKCAMERA_PROP_CHN,i,D_ZKCAMERA_PROP_CHN);
#define GET_ZKCAMERA_PROP_SKIP(i)        SystemProperties::getInt(ZKCAMERA_PROP_SKIP,i,D_ZKCAMERA_PROP_SKIP);
#define GET_ZKCAMERA_PROP_ROT(i)		 SystemProperties::getInt(ZKCAMERA_PROP_ROT,i,D_ZKCAMERA_PROP_ROT);
#define GET_ZKCAMERA_PROP_BUFCNT(i)		 SystemProperties::getInt(ZKCAMERA_PROP_BUFCNT,i,D_ZKCAMERA_PROP_BUFCNT);
#define GET_ZKCAMERA_PROP_MEMTYPE(i)	 SystemProperties::getInt(ZKCAMERA_PROP_MEMTYPE,i,D_ZKCAMERA_PROP_MEMTYPE);

#define AUDIO_PLAY_MODE					 "audio_play_mode"
#define SYS_BRIGHTNESS					 "brightness"
#define SYS_CONTRAST					 "contrast"
#define SYS_SATRATION					 "saturation"
#define SYS_HUE							 "hue"
#define SYS_LUMINANCE					 "luminance"
#define LINK_VOICE						 "link_voice"
#define LINK_TYPE					 	 "link_type"
#define LINK_MODE						 "link_mode"
#define SOUND_MODE                 		 "sound_mode"
#define LINK_CARLIFE					 "carlife"
#define USB_MODE						 "usb_mode"

#define BT_NAME                    		 "bt_name"
#define DEV_NAME				   		 "dev_name"

#define DEVICE_TYPE					 	 "zkswe"

#define TIRE_MODULE_FRONT_DATA	 		 "tire_module_front_data"
#define TIRE_MODULE_REAR_DATA			 "tire_module_rear_data"
#define TIRE_OPTION_INFO				 "tire_option_info"
#define FM_FREQUENCY               "fm_frequency"
#define FM_SWITCH				   "fm_switch"

#define VIDEO_PLAY_MODE_KEY			"video_play_mode_key"
#define MUSIC_PLAY_DEV				"music_play_dev"

#define TIME_FORMAT_24H            "time_format_24h"
#define REVERSE_LINE_POINT         "reverse_line_point"
#define REVERSE_LINE_VIEW		   "reverse_line_view"

#define DEFAULT_FREQUENCY		   900

typedef struct{
	char dev[16];
	char size[16];
	int rate;
	int chn;
	int skip;
	int wide;
	int high;
	int rot;
	int buf_count;
	int mem_type;
} camera_info_tab;
static bool _s_time_format_24h = true;
static camera_info_tab s_camera_info_tab;
static std::set<play_mode_state_cb> _s_play_state_set;
static Mutex _s_play_mutex;

static void _notify_play_state(audio_player_mode_e state) {
	Mutex::Autolock _l(_s_play_mutex);
	for(auto it : _s_play_state_set)
	{
		it(state);
	}
}

namespace sys {
namespace setting {

void init() {
	GET_ZKCAMERA_PROP_DEV(s_camera_info_tab.dev)
	GET_ZKCAMERA_PROP_SIZE(s_camera_info_tab.size)
	GET_ZKCAMERA_PROP_RATE(&s_camera_info_tab.rate)
	GET_ZKCAMERA_PROP_CHN(&s_camera_info_tab.chn)
	GET_ZKCAMERA_PROP_SKIP(&s_camera_info_tab.skip)
	GET_ZKCAMERA_PROP_ROT(&s_camera_info_tab.rot)
	GET_ZKCAMERA_PROP_BUFCNT(&s_camera_info_tab.buf_count)
	GET_ZKCAMERA_PROP_MEMTYPE(&s_camera_info_tab.mem_type)

	sscanf(s_camera_info_tab.size, "%dx%d", &s_camera_info_tab.wide, &s_camera_info_tab.high);
	set_camera_dev(s_camera_info_tab.dev);
	set_camera_size(s_camera_info_tab.wide, s_camera_info_tab.high);
	set_camera_rate(s_camera_info_tab.rate);
	set_camera_rot(s_camera_info_tab.rot);
	set_camera_chn(s_camera_info_tab.chn);
	set_camera_skip(s_camera_info_tab.skip);

	// 解决快速倒车画面卡顿的问题
	set_camera_buf_count(s_camera_info_tab.buf_count);
	set_camera_mem_type(s_camera_info_tab.mem_type);

	int brival = storage::get_int(SYS_BRIGHTNESS, -1);
	int contrastval = storage::get_int(SYS_CONTRAST, -1);
	int satrationval = storage::get_int(SYS_SATRATION, -1);
	int hueval = storage::get_int(SYS_HUE, -1);
	int luminanceval = storage::get_int(SYS_LUMINANCE, -1);
	if (brival != -1) {
		set_brightness(brival);
	}
	if (contrastval != -1) {
		set_contrast(contrastval);
	}
	if (satrationval != -1) {
		set_saturation(satrationval);
	}
	if (hueval != -1) {
		set_hue(hueval);
	}
	if (luminanceval != -1) {
		set_luminance(luminanceval);
	}

//	if (is_usb_adb_enabled()) {
//		if (get_usb_mode() != E_USB_MODE_DEVICE) {
//			change_usb_mode(E_USB_MODE_DEVICE);
//		}
//	} else {
//		set_usb_config(E_USB_MODE_HOST);
//	}
	_s_time_format_24h = storage::get_bool(TIME_FORMAT_24H, true);
	uint8_t otp_tag[2];
	if (storage::otp_read_data(BT_LIC_OFFSET + BT_LIC_SIZE, otp_tag, 2)) {
		LOGD("### otp auth tag: %02X %02X\n", otp_tag[0], otp_tag[1]);
	}

	// 设置互联miracast名称
//	setenv("PRODUCT_NAME", "zkcast",  1);
}

void set_current_sys_player_mode(audio_player_mode_e mode) {
	// 控制FM逻辑
//	sys::set_fmt_switch(mode != E_AUDIO_PLAYER_MODE_FM);
//
//	if (mode != E_AUDIO_PLAYER_MODE_BT) {
//		btemit::power_be_off();
//	}
	zk_audio_player_set_mode(mode);
	storage::put_int(AUDIO_PLAY_MODE, mode);
	_notify_play_state(mode);
}

audio_player_mode_e get_current_sys_player_mode() {
	return (audio_player_mode_e)storage::get_int(AUDIO_PLAY_MODE, E_AUDIO_PLAYER_MODE_SPK);
}

void add_play_state_cb(play_mode_state_cb cb) {
	Mutex::Autolock _l(_s_play_mutex);
	_s_play_state_set.insert(cb);
}

void remove_play_state_cb(play_mode_state_cb cb) {
	Mutex::Autolock _l(_s_play_mutex);
	_s_play_state_set.erase(cb);
}

void set_link_type(LINK_TYPE_E type){
	storage::put_int(LINK_TYPE, (int)type);
}

LINK_TYPE_E get_link_type() {
	return (LINK_TYPE_E)storage::get_int(LINK_TYPE, (int)E_TYPE_AB);
}

void set_is_use_link_carlife(bool is_use) {
	storage::put_bool(LINK_CARLIFE, is_use);
}

bool get_is_use_link_carlife() {
	return storage::get_bool(LINK_CARLIFE, false);
}

void set_linksound_type(linkvioce_mode_e type){
	storage::put_int(LINK_VOICE, type);
}

linkvioce_mode_e get_linksound_type() {
	return (linkvioce_mode_e)storage::get_int(LINK_VOICE, (linkvioce_mode_e)E_TYPE_ENGINE);
}

const char* get_link_mode_str(link_mode_e mode) {
	switch (mode) {
	case E_LINK_MODE_HICAR:
		return "hicar";
	case E_LINK_MODE_ANDROIDAUTO:
		return "android auto";
	case E_LINK_MODE_CARPLAY:
		return "carplay";
	case E_LINK_MODE_AIRPLAY:
		return "airplay";
	case E_LINK_MODE_CARLIFE:
		return "carlife";
	case E_LINK_MODE_MIRACAST:
		return "miracast";
	case E_LINK_MODE_LYLINK:
		return "aicast";
	default:
		return "unknow";
	}
}

link_mode_e get_link_mode() {
	return (link_mode_e) storage::get_int(LINK_MODE, E_LINK_MODE_CARPLAY);
}

void set_link_mode(link_mode_e mode) {
	storage::put_int(LINK_MODE, mode);
}

void set_camera_chn(int chn){
	SystemProperties::setInt(ZKCAMERA_PROP_CHN, chn);
}

int get_camera_chn(){
	GET_ZKCAMERA_PROP_CHN(&s_camera_info_tab.chn)
	return s_camera_info_tab.chn;
}

void set_camera_skip(int skip){
	char str[8] = { 0 };
	sprintf(str, "%d", skip);
	setenv("ZKCAMERA_SKIP_FRAMES", str, 1);
	SystemProperties::setInt(ZKCAMERA_PROP_SKIP, skip);
}

int get_camera_skip(){
	GET_ZKCAMERA_PROP_SKIP(&s_camera_info_tab.skip)
	return s_camera_info_tab.skip;
}

void set_camera_dev(const char *dev){
	SystemProperties::setString(ZKCAMERA_PROP_DEV, dev);
}

char *get_camera_dev(){
	GET_ZKCAMERA_PROP_DEV(s_camera_info_tab.dev)
	return s_camera_info_tab.dev;
}

int get_camera_wide(){
	return s_camera_info_tab.wide;
}

int get_camera_high(){
	return s_camera_info_tab.high;
}

void set_camera_size(int wide,int high){
	s_camera_info_tab.wide = wide;
	s_camera_info_tab.high = high;
	char size[16] = {0};
	sprintf(size,"%dx%d",wide,high);
	SystemProperties::setString(ZKCAMERA_PROP_SIZE,size);
}

int get_camera_rate(){
	return s_camera_info_tab.rate;
}

void set_camera_rate(int rate){
	s_camera_info_tab.rate = rate;
	SystemProperties::setInt(ZKCAMERA_PROP_RATE, rate);
}

int get_camera_rot(){
	GET_ZKCAMERA_PROP_ROT(&s_camera_info_tab.rot)
	return s_camera_info_tab.rot;
}

void set_camera_rot(int rot){
	SystemProperties::setInt(ZKCAMERA_PROP_ROT, rot);
}

int get_camera_buf_count(){
	GET_ZKCAMERA_PROP_ROT(&s_camera_info_tab.buf_count)
	return s_camera_info_tab.buf_count;
}

void set_camera_buf_count(int cnt){
	SystemProperties::setInt(ZKCAMERA_PROP_BUFCNT, cnt);
}

int get_camera_mem_type(){
	GET_ZKCAMERA_PROP_ROT(&s_camera_info_tab.mem_type)
	return s_camera_info_tab.mem_type;
}

void set_camera_mem_type(int type){
	SystemProperties::setInt(ZKCAMERA_PROP_MEMTYPE, type);
}

void set_brightness(int brightness) {
	storage::put_int(SYS_BRIGHTNESS,brightness);
	BRIGHTNESSHELPER->setBrightness(brightness);
}

int get_brightness() {
	return BRIGHTNESSHELPER->getBrightness();
}

void set_contrast(int contrast) {
	storage::put_int(SYS_CONTRAST,contrast);
	BRIGHTNESSHELPER->setContrast(contrast);
}

int get_contrast(){
	return BRIGHTNESSHELPER->getContrast();
}

void set_saturation(int saturation) {
	storage::put_int(SYS_SATRATION,saturation);
	BRIGHTNESSHELPER->setSaturation(saturation);
}

int get_saturation(){
	return BRIGHTNESSHELPER->getSaturation();
}

void set_hue(int hue) {
	storage::put_int(SYS_HUE,hue);
	BRIGHTNESSHELPER->setHue(hue);
}

int get_hue(){
	return BRIGHTNESSHELPER->getHue();
}

void set_luminance(int luminance){
	storage::put_int(SYS_LUMINANCE,luminance);
	BRIGHTNESSHELPER->setLuminance(luminance);
}

int get_luminance(){
	return BRIGHTNESSHELPER->getLuminance();
}

void backlight_on() {
	BRIGHTNESSHELPER->backlightOn();
}

void backlight_off() {
	BRIGHTNESSHELPER->backlightOff();
}

void screen_off(){
	BRIGHTNESSHELPER->screenOff();
}

void screen_on(){
	BRIGHTNESSHELPER->screenOn();
}

bool is_screenon(){
	return BRIGHTNESSHELPER->isScreenOn();
}

void update_localescode(const char *code){
	EASYUICONTEXT->updateLocalesCode(code);
}

bool set_DateTime(struct tm *ptm){
	return TimeHelper::setDateTime(ptm);
}

bool is_usb_adb_enabled() {
	return storage::get_int(USB_MODE, E_USB_MODE_HOST) == E_USB_MODE_DEVICE;
}

void enable_usb_adb(bool enabled) {
	usb_mode_e mode = enabled ? E_USB_MODE_DEVICE : E_USB_MODE_HOST;
	change_usb_mode(mode);
	storage::put_int(USB_MODE, mode);
}

std::string get_dev_name() {
	return storage::get_string(DEV_NAME, DEVICE_TYPE + fy::gen_uuid_str());
}

void set_bt_name(const std::string &name) {
	storage::put_string(BT_NAME, name);
}

std::string get_bt_name() {
	return storage::get_string(BT_NAME, get_dev_name());
}

void set_sound_mode(sound_mode_e mode) {
	storage::put_int(SOUND_MODE, mode);
}

sound_mode_e get_sound_mode() {
	return (sound_mode_e) storage::get_int(SOUND_MODE, E_SOUND_MODE_SPK);
}

void set_front_tire_data(std::string& data) {
	storage::put_string(TIRE_MODULE_FRONT_DATA, data);
}

std::string get_front_tire_data() {
	return storage::get_string(TIRE_MODULE_FRONT_DATA, "");
}

void set_rear_tire_data(std::string& data) {
	storage::put_string(TIRE_MODULE_REAR_DATA, data);
}

std::string get_rear_tire_data() {
	return storage::get_string(TIRE_MODULE_REAR_DATA, "");
}

void set_tire_option_info(std::string& data) {
	storage::put_string(TIRE_OPTION_INFO, data);
}

std::string get_tire_option_info() {
	return storage::get_string(TIRE_OPTION_INFO, "");
}

int get_fm_frequency() {
	return storage::get_int(FM_FREQUENCY, DEFAULT_FREQUENCY);
}

void set_fm_frequency(int freq) {
	storage::put_int(FM_FREQUENCY, freq);
}

bool get_fm_switch() {
	return storage::get_bool(FM_SWITCH, false);
}

void set_fm_switch(bool status) {
	storage::put_bool(FM_SWITCH, status);
}

void set_time_format_24h(bool format_24h) {
	_s_time_format_24h = format_24h;
	storage::put_bool(TIME_FORMAT_24H, format_24h);
}

bool is_time_format_24h() {
	return _s_time_format_24h;
}

void set_reverse_line_point(const SZKPoint &lt, const SZKPoint &rt, const SZKPoint &lb, const SZKPoint &rb) {
	char buf[128];
	sprintf(buf, "%d,%d,%d,%d,%d,%d,%d,%d",
			(int) lt.x, (int) lt.y, (int) rt.x, (int) rt.y,
			(int) lb.x, (int) lb.y, (int) rb.x, (int) rb.y);
	storage::put_string(REVERSE_LINE_POINT, buf);
}

void get_reverse_line_point(SZKPoint &lt, SZKPoint &rt, SZKPoint &lb, SZKPoint &rb) {
	std::string str = storage::get_string(REVERSE_LINE_POINT, "");
	if (!str.empty()) {
		int ltx = 0, lty = 0, rtx = 0, rty = 0, lbx = 0, lby = 0, rbx = 0, rby = 0;
		sscanf(str.c_str(), "%d,%d,%d,%d,%d,%d,%d,%d", &ltx, &lty, &rtx, &rty, &lbx, &lby, &rbx, &rby);
		lt.x = ltx;
		lt.y = lty;
		rt.x = rtx;
		rt.y = rty;
		lb.x = lbx;
		lb.y = lby;
		rb.x = rbx;
		rb.y = rby;
	} else {
		lt.x = REVERSE_LINE_DEF_LTX;
		lt.y = REVERSE_LINE_DEF_LTY;
		rt.x = REVERSE_LINE_DEF_RTX;
		rt.y = REVERSE_LINE_DEF_RTY;
		lb.x = REVERSE_LINE_DEF_LBX;
		lb.y = REVERSE_LINE_DEF_LBY;
		rb.x = REVERSE_LINE_DEF_RBX;
		rb.y = REVERSE_LINE_DEF_RBY;
	}
}

void set_reverse_line_view (bool show) {
	storage::put_bool(REVERSE_LINE_VIEW, show);
}
bool is_reverse_line_view () {
	return storage::get_bool(REVERSE_LINE_VIEW, false);
}

void video_set_play_mode(media_play_mode_e mode) {
	storage::put_int(VIDEO_PLAY_MODE_KEY, mode);
}

media_play_mode_e video_get_play_mode() {
	return (media_play_mode_e) storage::get_int(VIDEO_PLAY_MODE_KEY, E_MEDIA_PLAY_MODE_CYCLE);
}

void set_music_play_dev(audio_type_e dev) {
	storage::put_int(MUSIC_PLAY_DEV, dev);
}
audio_type_e get_music_play_dev() {
	return (audio_type_e) storage::get_int(MUSIC_PLAY_DEV, E_AUDIO_TYPE_MUSIC);
}

}
}
