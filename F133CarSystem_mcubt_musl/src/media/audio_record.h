/*
 * audio_record.h
 *
 *  Created on: 2022年7月7日
 *      Author: ZKSWE Develop Team
 */

#ifndef _MEDIA_AUDIO_RECORD_H_
#define _MEDIA_AUDIO_RECORD_H_

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#include <stdint.h>

typedef enum {
	E_AUDIO_RECORD_TYPE_MIC,
	E_AUDIO_RECORD_TYPE_LINEIN,
	E_AUDIO_RECORD_TYPE_FMIN,
	E_AUDIO_RECORD_TYPE_HDMI,
} audio_record_type_e;

int zk_audio_record_init(audio_record_type_e type, uint32_t channels, uint32_t rate, uint32_t period_size, uint32_t period_count);
void zk_audio_record_deinit();
int zk_audio_record_get_frame(uint8_t *data, uint32_t size);
void zk_audio_record_wake();

// vol 0.0 ~ 1.0
void zk_audio_record_set_volume(float vol);

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif /* _MEDIA_AUDIO_RECORD_H_ */
