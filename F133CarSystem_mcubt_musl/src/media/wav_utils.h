#ifndef _MEDIA_WAV_UTILS_H_
#define _MEDIA_WAV_UTILS_H_

#include <stdio.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void* zk_wav_init(const char *path, uint16_t channels, uint32_t sample_rate);
int zk_wav_add_data(void *handle, const uint8_t *data, uint32_t len);
int zk_wav_deinit(void *handle);

#ifdef __cplusplus
}
#endif

#endif
