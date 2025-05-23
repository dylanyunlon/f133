/*
 * webrtc_echo.h
 *
 *  Created on: 2023年8月31日
 *      Author: ZKSWE Develop Team
 */

#ifndef _WEBRTC_ECHO_H_
#define _WEBRTC_ECHO_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void* webrtc_echo_create(int rate);
void webrtc_echo_destroy(void *handle);
int webrtc_echo_process(void *handle, const int16_t *rec_frm, const int16_t *play_frm, size_t samples, int16_t *out);

#ifdef __cplusplus
}
#endif

#endif /* _WEBRTC_ECHO_H_ */
