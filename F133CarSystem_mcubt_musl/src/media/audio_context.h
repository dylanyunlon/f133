/*
 * audio_context.h
 *
 *  Created on: 2022年7月13日
 *      Author: ZKSWE Develop Team
 */

#ifndef _MEDIA_AUDIO_CONTEXT_H_
#define _MEDIA_AUDIO_CONTEXT_H_

#include "media_base.h"
#include "audio_record.h"
#include "audio_player.h"

typedef void (*media_vol_change_cb)(float vol);
typedef void (*audio_output_mode_cb)(audio_player_mode_e new_mode, audio_player_mode_e old_mode);

namespace audio {

void init();

// 切换不同音频类型时调用
void change_audio_type(audio_type_e type);
audio_type_e get_audio_type();

// 处理打电话
void handle_phone(audio_type_e type, bool phoning);

// 处理导航播报
void handle_tts(audio_type_e type, bool playing);

// vol: 0.0 ~ 1.0    effect: 是否立即生效
void set_record_vol(float vol, bool effect = true);
void set_system_vol(float vol, bool effect = true);

void set_lylink_call_vol(float vol, bool effect = false);

float get_record_vol();

float get_system_vol();
float get_lylink_call_vol();

void change_output_mode(audio_player_mode_e mode);
audio_player_mode_e get_output_mode();
void add_output_mode_cb(audio_output_mode_cb cb);
void remove_output_mode_cb(audio_output_mode_cb cb);

}

#endif /* _MEDIA_AUDIO_CONTEXT_H_ */
