/*
 * audio_linein.cpp
 *
 *  Created on: 2023年2月6日
 *      Author: ZKSWE Develop Team
 */

#include "audio_linein.h"
#include "audio_context.h"
#include "wav_utils.h"
#include "system/Thread.h"

#define DEBUG_SAVE_WAV  0		// 生成调试音频

namespace audio {

class LineinThread : public Thread {
public:
	bool start(uint32_t channels, uint32_t rate) {
		mChannels = channels;
		mRate = rate;
		return run("linein");
	}

	void stop() {
		requestExitAndWait();
	}

	bool isPlaying() const {
		return isRunning();
	}

protected:
	virtual bool threadLoop() {
		LOGD("linein thread start...\n");

		void *player = zk_audio_multi_player_init(mChannels, mRate, 1024, 4);
		zk_audio_record_init(E_AUDIO_RECORD_TYPE_LINEIN, mChannels, mRate, 1024, 4);

#if DEBUG_SAVE_WAV
		void *wav_handle = zk_wav_init("/mnt/extsd/linein.wav", mChannels, mRate);
#else
		void *wav_handle = NULL;
#endif

		uint32_t size = 4096;
		uint8_t data[size];
		while (!exitPending()) {
			if (zk_audio_record_get_frame(data, size) != 0) {
				break;
			}

			zk_audio_multi_player_put_frame(player, data, size);
			zk_wav_add_data(wav_handle, data, size);
		}

		zk_audio_record_deinit();
		zk_audio_multi_player_deinit(player);

		zk_wav_deinit(wav_handle);

		LOGD("linein thread end...\n");

		return false;
	}

private:
	uint32_t mChannels;
	uint32_t mRate;
};

static LineinThread _s_linein_thread;

bool linein_start() {
	change_audio_type(E_AUDIO_TYPE_LINEIN);
	return _s_linein_thread.start(2, 48000);
}

void linein_stop() {
	_s_linein_thread.stop();
}

bool linein_is_playing() {
	return _s_linein_thread.isPlaying();
}

}
