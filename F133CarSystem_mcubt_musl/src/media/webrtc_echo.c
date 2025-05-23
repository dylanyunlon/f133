/*
 * webrtc_echo.c
 *
 *  Created on: 2023年8月31日
 *      Author: ZKSWE Develop Team
 */

#include <stdlib.h>
#include "config.h"
#include "webrtc_aec.h"
#include "webrtc_ns.h"
#include "webrtc_echo.h"

#define SAMPLE_NR       160

#define ENABLE_WEBRTC 1

typedef struct {
	void *aec_obj;
	NsHandle *ns_obj;
} echo_handle_t;

static void inline _int16_to_float(const int16_t *src, float *dst, size_t size) {
	for (size_t i = 0; i < size; ++i) {
		dst[i] = (float) src[i];
	}
}

static void inline _float_to_int16(const float *src, int16_t *dst, size_t size) {
	for (size_t i = 0; i < size; ++i) {
		dst[i] = (int16_t) src[i];
	}
}

void* webrtc_echo_create(int rate) {
#if ENABLE_WEBRTC
	void *aec_obj = WebRtcAec_Create();
	if (!aec_obj) {
		return NULL;
	}

	if (WebRtcAec_Init(aec_obj, rate, rate) < 0) {
		WebRtcAec_Free(aec_obj);
		return NULL;
	}

	AecConfig config = { 0 };
	config.nlpMode = kAecNlpAggressive;

	if (WebRtcAec_set_config(aec_obj, config) < 0) {
		WebRtcAec_Free(aec_obj);
		return NULL;
	}

	NsHandle *ns_obj = NULL;
#if MEDIA_ECHO_NS_ENABLE
	ns_obj = WebRtcNs_Create();
	if (ns_obj) {
		if (WebRtcNs_Init(ns_obj, rate) != 0) {
			WebRtcNs_Free(ns_obj);
			ns_obj = NULL;
		} else {
			WebRtcNs_set_policy(ns_obj, 2);
		}
	}
#endif

	echo_handle_t *handle = (echo_handle_t *) malloc(sizeof(echo_handle_t));
	handle->aec_obj = aec_obj;
	handle->ns_obj = ns_obj;

	return handle;
#else
	return NULL;
#endif
}

void webrtc_echo_destroy(void *handle) {
#if ENABLE_WEBRTC
	if (!handle) {
		return;
	}

	echo_handle_t *echo = (echo_handle_t *) handle;
	if (echo->aec_obj) {
		WebRtcAec_Free(echo->aec_obj);
	}
	if (echo->ns_obj) {
		WebRtcNs_Free(echo->ns_obj);
	}

	free(echo);
#endif
}

int webrtc_echo_process(void *handle, const int16_t *rec, const int16_t *play, size_t samples, int16_t *out) {
#if ENABLE_WEBRTC
	if (!handle || !rec || !play || !out) {
		return -1;
	}

	echo_handle_t *echo = (echo_handle_t *) handle;
	void *aec_obj = echo->aec_obj;
	NsHandle *ns_obj = echo->ns_obj;

	float rec_frm[SAMPLE_NR];
	float play_frm[SAMPLE_NR];
	float out_frm[SAMPLE_NR];
	float *rec_frm_p = rec_frm;
	float *ns_frm_p = play_frm;
	float *out_frm_p = out_frm;

	for (size_t i = 0; i <= (samples - SAMPLE_NR); i += SAMPLE_NR) {
		_int16_to_float(play + i, play_frm, SAMPLE_NR);
		if (WebRtcAec_BufferFarend(aec_obj, play_frm, SAMPLE_NR) < 0)  {
			return -1;
		}

		_int16_to_float(rec + i, rec_frm, SAMPLE_NR);

		if (ns_obj) {
			WebRtcNs_Analyze(ns_obj, rec_frm_p);
			out_frm_p = ns_frm_p;
		}

		if (WebRtcAec_Process(aec_obj, (const float* const*) &rec_frm_p, 1,
				(float* const*) &out_frm_p, SAMPLE_NR, MEDIA_ECHO_FAR_DELAY, 0) < 0)  {
			return -1;
		}

		if (ns_obj) {
			out_frm_p = out_frm;
			WebRtcNs_Process(ns_obj, (const float* const*) &ns_frm_p,
					1, (float* const*) &out_frm_p);
		}

		_float_to_int16(out_frm, out + i, SAMPLE_NR);
	}
#endif
	return 0;
}
