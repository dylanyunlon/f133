/*
 * media_base.h
 *
 *  Created on: 2023年2月1日
 *      Author: ZKSWE Develop Team
 */

#ifndef _MEDIA_MEDIA_BASE_H_
#define _MEDIA_MEDIA_BASE_H_

#include <string>
#include <vector>
#include "common.h"

#define MEDIA_FILE_MAX_PATH        256

typedef enum {
	E_MEDIA_TYPE_AUDIO,
	E_MEDIA_TYPE_VIDEO,
	E_MEDIA_TYPE_IMAGE,
	E_MEDIA_TYPE_INVALID,
} media_type_e;

typedef enum {
	E_STORAGE_TYPE_SD,
	E_STORAGE_TYPE_USB1,
	E_STORAGE_TYPE_USB2,
	E_STORAGE_TYPE_INVALID,
} storage_type_e;

typedef enum {
	E_MEDIA_PLAY_MODE_CYCLE,     // 全部循环
	E_MEDIA_PLAY_MODE_SINGLE,    // 单曲循环
	E_MEDIA_PLAY_MODE_RANDOM,    // 随机
} media_play_mode_e;

typedef enum {
	E_AUDIO_MODULE_BT = 0x10,
	E_AUDIO_MODULE_LYLINK = 0x20,
	E_AUDIO_MODULE_MASK = 0xF0,
} audio_module_e;

typedef enum {
	E_AUDIO_TYPE_NULL,
	E_AUDIO_TYPE_MUSIC,          // 本地音乐
	E_AUDIO_TYPE_RADIO,          // 收音机
	E_AUDIO_TYPE_LINEIN,         // linein
	E_AUDIO_TYPE_VIDEO,          // 本地视频

	E_AUDIO_TYPE_BT_MUSIC = E_AUDIO_MODULE_BT,       // 蓝牙音乐
	E_AUDIO_TYPE_BT_PHONE,       // 蓝牙电话

	E_AUDIO_TYPE_LYLINK_MUSIC = E_AUDIO_MODULE_LYLINK,   // carplay、auto music
	E_AUDIO_TYPE_LYLINK_PHONE,   // carplay、auto phone
	E_AUDIO_TYPE_LYLINK_VR,		 // carplay、auto 语音助手
	E_AUDIO_TYPE_LYLINK_TTS,     // carplay、auto tts
} audio_type_e;
typedef enum {
	E_AUDIO_SOURCE_NULL,
	E_AUDIO_SOURCE_MEDIA,
	E_AUDIO_SOURCE_BT,
	E_AUDIO_SOURCE_RADIO,
} audio_source_e;

typedef enum {
	E_MUSIC_PLAY_STATUS_STARTED,    // 播放开始
	E_MUSIC_PLAY_STATUS_PAUSE,      // 暂停播放
	E_MUSIC_PLAY_STATUS_RESUME,     // 恢复播放
	E_MUSIC_PLAY_STATUS_STOP,       // 停止播放
	E_MUSIC_PLAY_STATUS_COMPLETED,  // 播放结束
	E_MUSIC_PLAY_STATUS_ERROR       // 播放错误
} music_play_status_e;

typedef struct {
	storage_type_e type;
	const char *root_path;
} storage_info_t;

typedef struct {
	char title[66];     // 标题
	char artist[66];    // 歌手
	char album[66];     // 专辑
} id3_info_t;


typedef struct {
	int width;
	int height;
	int bit_rate;
} video_info_t;

#endif /* _MEDIA_MEDIA_BASE_H_ */
