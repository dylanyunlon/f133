/*
 * audio_recorder.cpp
 *
 *  Created on: 2023年2月6日
 *      Author: ZKSWE Develop Team
 */

#include "audio_context.h"
#include "audio_recorder.h"
#include "webrtc_echo.h"
#include "wav_utils.h"
#include "system/Thread.h"


#define DEBUG_SAVE_WAV  0		// 生成调试音频

namespace audio {

class RecThread : public Thread {
public:
	RecThread() : mRate(16000), mCB(NULL) {

	}

	bool start(uint32_t rate, audio_record_cb cb) {
		mRate = rate;
		mCB = cb;
		return run("rec");
	}

	void stop() {
#if 1
		while(isRunning()){
			requestExit();
			zk_audio_record_wake();
			WAIT(!isRunning(),100,10);
		}
#else
		requestExitAndWait();
#endif
	}

protected:
	static void splitSample(const int16_t *src_data, uint32_t size, int16_t *near_data, int16_t *far_data) {
		uint32_t sample_count = size / 2;
		for (uint32_t i = 0; i < sample_count; ++i) {
			near_data[i] = *src_data++;
			far_data[i] = *src_data++;
		}
	}

	virtual bool threadLoop() {
		LOGD("rec thread start...\n");

		const uint32_t kSampleSize = 1280;
		const uint32_t kRecvSize = kSampleSize * 2;
		uint8_t *recv_data, *near_data, *far_data, *echo_data;

		recv_data = (uint8_t *) malloc(kRecvSize);
		near_data = (uint8_t *) malloc(kSampleSize);
		far_data = (uint8_t *) malloc(kSampleSize);
		echo_data = (uint8_t *) malloc(kSampleSize);

#if DEBUG_SAVE_WAV
		void *recv_wav = zk_wav_init("/mnt/extsd/recv.wav", 2, mRate);
		void *near_wav = zk_wav_init("/mnt/extsd/near.wav", 1, mRate);   // near (mic)
		void *far_wav = zk_wav_init("/mnt/extsd/far.wav", 1, mRate);     // far (loopback)
		void *echo_wav = zk_wav_init("/mnt/extsd/echo.wav", 1, mRate);
#else
		void *recv_wav = NULL;
		void *near_wav = NULL;
		void *far_wav = NULL;
		void *echo_wav = NULL;
#endif

		void *echo_handle = webrtc_echo_create(mRate);
		if (!echo_handle) {
			LOGE("webrtc echo create fail!\n");
		}

		zk_audio_record_init(E_AUDIO_RECORD_TYPE_MIC, 2, mRate, 1024, 4);

		while (!exitPending()) {
			if (zk_audio_record_get_frame(recv_data, kRecvSize) != 0) {
				LOGE("get frame err !!!\n");
				break;
			}

			// 分离左右声道数据
			splitSample((int16_t *) recv_data, kSampleSize, (int16_t *) near_data, (int16_t *) far_data);

			zk_wav_add_data(recv_wav, recv_data, kRecvSize);
			zk_wav_add_data(near_wav, near_data, kSampleSize);
			zk_wav_add_data(far_wav, far_data, kSampleSize);

			if (!echo_handle) {
				if (mCB) {
					mCB(near_data, kSampleSize);
				}
				continue;
			}
			webrtc_echo_process(echo_handle, (const int16_t *) near_data, (const int16_t *) far_data, kSampleSize / 2, (int16_t *) echo_data);
			if (mCB) {
				mCB(echo_data, kSampleSize);
			}

			zk_wav_add_data(echo_wav, echo_data, kSampleSize);
		}

		zk_audio_record_deinit();
		webrtc_echo_destroy(echo_handle);

		free(recv_data);
		free(near_data);
		free(far_data);
		free(echo_data);

		zk_wav_deinit(recv_wav);
		zk_wav_deinit(near_wav);
		zk_wav_deinit(far_wav);
		zk_wav_deinit(echo_wav);

		LOGD("rec thread end...\n");

		return false;
	}

private:
	uint32_t mRate;
	audio_record_cb mCB;
};

static RecThread _s_rec_thread;

bool record_start(uint32_t rate, audio_record_cb cb) {
	return _s_rec_thread.start(rate, cb);
}

void record_stop() {
	_s_rec_thread.stop();
}

}
