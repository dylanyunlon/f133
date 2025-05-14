/*
 * audio_linein.h
 *
 *  Created on: 2023年2月6日
 *      Author: ZKSWE Develop Team
 */

#ifndef _MEDIA_AUDIO_LINEIN_H_
#define _MEDIA_AUDIO_LINEIN_H_

namespace audio {

bool linein_start();
void linein_stop();
bool linein_is_playing();

}

#endif /* _MEDIA_AUDIO_LINEIN_H_ */
