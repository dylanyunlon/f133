#ifndef __LYLINK_RESAMPLER_H__
#define __LYLINK_RESAMPLER_H__


#ifdef __cplusplus
extern "C" {
#endif


typedef struct resampler_t *RESAMPLER;
struct resampler_t {
	int (*src) (RESAMPLER inst, int rate, int bits, int chns);
	int (*dst) (RESAMPLER inst, int rate, int bits, int chns);
	int (*process) (RESAMPLER inst, const void * src, unsigned int srclen, void * dst, unsigned int dstlen);
};

RESAMPLER lylink_resampler_create();
void lylink_resampler_delete(RESAMPLER inst);


#ifdef __cplusplus
}
#endif


#endif
