/*
 *  Copyright (c) 2012 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#ifndef _WEBRTC_NS_H_
#define _WEBRTC_NS_H_

#include <stddef.h>
#include <stdint.h>

typedef struct NsHandleT NsHandle;

#ifdef __cplusplus
extern "C" {
#endif

/*
 * This function creates an instance of the floating point Noise Suppression.
 */
NsHandle* WebRtcNs_Create();

/*
 * This function frees the dynamic memory of a specified noise suppression
 * instance.
 *
 * Input:
 *      - NS_inst       : Pointer to NS instance that should be freed
 */
void WebRtcNs_Free(NsHandle* NS_inst);

/*
 * This function initializes a NS instance and has to be called before any other
 * processing is made.
 *
 * Input:
 *      - NS_inst       : Instance that should be initialized
 *      - fs            : sampling frequency
 *
 * Output:
 *      - NS_inst       : Initialized instance
 *
 * Return value         :  0 - Ok
 *                        -1 - Error
 */
int WebRtcNs_Init(NsHandle* NS_inst, uint32_t fs);

/*
 * This changes the aggressiveness of the noise suppression method.
 *
 * Input:
 *      - NS_inst       : Noise suppression instance.
 *      - mode          : 0: Mild, 1: Medium , 2: Aggressive
 *
 * Output:
 *      - NS_inst       : Updated instance.
 *
 * Return value         :  0 - Ok
 *                        -1 - Error
 */
int WebRtcNs_set_policy(NsHandle* NS_inst, int mode);

/*
 * This functions estimates the background noise for the inserted speech frame.
 * The input and output signals should always be 10ms (80 or 160 samples).
 *
 * Input
 *      - NS_inst       : Noise suppression instance.
 *      - spframe       : Pointer to speech frame buffer for L band
 *
 * Output:
 *      - NS_inst       : Updated NS instance
 */
void WebRtcNs_Analyze(NsHandle* NS_inst, const float* spframe);

/*
 * This functions does Noise Suppression for the inserted speech frame. The
 * input and output signals should always be 10ms (80 or 160 samples).
 *
 * Input
 *      - NS_inst       : Noise suppression instance.
 *      - spframe       : Pointer to speech frame buffer for each band
 *      - num_bands     : Number of bands
 *
 * Output:
 *      - NS_inst       : Updated NS instance
 *      - outframe      : Pointer to output frame for each band
 */
void WebRtcNs_Process(NsHandle* NS_inst,
                     const float* const* spframe,
                     size_t num_bands,
                     float* const* outframe);

/* Returns the internally used prior speech probability of the current frame.
 * There is a frequency bin based one as well, with which this should not be
 * confused.
 *
 * Input
 *      - handle        : Noise suppression instance.
 *
 * Return value         : Prior speech probability in interval [0.0, 1.0].
 *                        -1 - NULL pointer or uninitialized instance.
 */
float WebRtcNs_prior_speech_probability(NsHandle* handle);

#ifdef __cplusplus
}
#endif

#endif  // _WEBRTC_NS_H_
