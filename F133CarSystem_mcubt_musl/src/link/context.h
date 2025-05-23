/*
 * context.h
 *
 *  Created on: 2022年6月13日
 *      Author: ZKSWE Develop Team
 */

#ifndef _LINK_CONTEXT_H_
#define _LINK_CONTEXT_H_

#include "common.h"
#include "lylinkapi.h"

typedef void (*lylink_callback)(LYLINKAPI_EVENT evt, int para0, void *para1);

namespace lk {

typedef enum {
	E_START_LYLINK,
	E_STOP_LYLINK,
	E_RESTART_LYLINK,
} lylink_switch_e;

//bool start_lylink();
void start_lylink();
void stop_lylink();
void restart_lylink();

void add_lylink_callback(lylink_callback cb);
void remove_lylink_callback(lylink_callback cb);

// 查询互联授权状态
void query_is_authorized();
bool get_is_authorized();

bool get_mfi_is_OK();

void music_pause();
void music_resume();
bool music_is_playing();

void video_start();
void video_stop();
void video_show(int x, int y, int w, int h);
void video_hide();

// 获取视频信息
bool get_video_param(LYLINKAPI_VIDEO_PARAM &param);

bool get_is_ringing();
LYLINK_CALLSTATEFOCUS_E get_is_call_state();

// 获取连接类型
LYLINK_TYPE_E get_lylink_type();
bool is_connected();
bool is_lylink_started();

/*
 * 设置互联画面夜间/白天模式
 */
void set_link_night_mode(bool is_night);

static inline const char* _audio_type_to_str(LYLINK_AUDIOTYPE_E type) {
	switch (type) {
	case AudioType_Default:
		return "default";
	case AudioType_Media:
		return "media";
	case AudioType_VR:
		return "vr";
	case AudioType_TTS:
		return "tts";
	case AudioType_Phone:
		return "phone";
	default:
		return "unknown";
	}
}

static inline const char* _link_type_to_str(LYLINK_TYPE_E type) {
	switch (type) {
	case LINK_TYPE_NULL:
		return "none";
	case LINK_TYPE_WIFICP:
		return "wifi carplay";
	case LINK_TYPE_USBCP:
		return "usb carplay";
	case LINK_TYPE_USBAUTO:
		return "usb android auto";
	case LINK_TYPE_WIFIAUTO:
		return "wifi android auto";
	case LINK_TYPE_USBLIFE:
		return "usb carlife";
	case LINK_TYPE_WIFILIFE:
		return "wifi carlife";
	case LINK_TYPE_USBHICAR:
		return "usb hicar";
	case LINK_TYPE_WIFIHICAR:
		return "wifi hicar";
	case LINK_TYPE_AIRPLAY:
		return "airplay";
	case LINK_TYPE_MIRACAST:
		return "miracast";
	case LINK_TYPE_USBLY:
		return "usb lylink";
	case LINK_TYPE_WIFILY:
		return "wifi lylink";
	default:
		return "unknown";
	}
}

}

#endif /* _LINK_CONTEXT_H_ */
