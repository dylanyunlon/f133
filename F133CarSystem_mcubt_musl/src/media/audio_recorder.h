/*
 * audio_recorder.h
 *
 *  Created on: 2023年2月6日
 *      Author: ZKSWE Develop Team
 */

#ifndef _MEDIA_AUDIO_RECORDER_H_
#define _MEDIA_AUDIO_RECORDER_H_

#include <stdint.h>

typedef void (*audio_record_cb)(const void *data, uint32_t size);

namespace audio {

bool record_start(uint32_t rate, audio_record_cb cb);
void record_stop();

}

#endif /* _MEDIA_AUDIO_RECORDER_H_ */
