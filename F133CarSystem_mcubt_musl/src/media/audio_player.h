/*
 * audio_player.h
 *
 *  Created on: 2022年6月24日
 *      Author: ZKSWE Develop Team
 */

#ifndef _MEDIA_AUDIO_PLAYER_H_
#define _MEDIA_AUDIO_PLAYER_H_

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#include <stdint.h>

typedef enum {
	E_AUDIO_PLAYER_MODE_SPK,    // 扬声器
	E_AUDIO_PLAYER_MODE_HP,     // 耳机
	E_AUDIO_PLAYER_MODE_FM,     // fm发射
	E_AUDIO_PLAYER_MODE_BT,     // 蓝牙发射
	E_AUDIO_PLAYER_MODE_AUX,    // aux out
} audio_player_mode_e;

// 独占模式，不支持多实例播放，同时只能有一个播放器输出声音
/**
 * channels:        通道数
 * rate:            采样率
 * period_size:     周期大小
 * period_count:    周期数
 */
int zk_audio_player_init(uint32_t channels, uint32_t rate, uint32_t period_size, uint32_t period_count);
void zk_audio_player_deinit();
int zk_audio_player_put_frame(uint8_t *data, uint32_t size);

// 支持多播放实例
/**
 * channels:        通道数
 * rate:            采样率
 * period_size:     周期大小
 * period_count:    周期数
 * return player
 */
void* zk_audio_multi_player_init(uint32_t channels, uint32_t rate, uint32_t period_size, uint32_t period_count);
void zk_audio_multi_player_deinit(void *player);
int zk_audio_multi_player_enable_loopback(void *player);
void zk_audio_multi_player_set_gain(void *player, float gain);
int zk_audio_multi_player_put_frame(void *player, uint8_t *data, uint32_t size);

// vol 0.0 ~ 1.0
void zk_audio_player_set_volume(float vol);

void zk_audio_player_set_mode(audio_player_mode_e mode);

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif /* _MEDIA_AUDIO_PLAYER_H_ */
