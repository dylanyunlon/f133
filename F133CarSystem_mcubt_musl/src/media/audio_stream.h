/*
 * audio_stream.h
 *
 *  Created on: 2023年6月28日
 *      Author: ZKSWE Develop Team
 */

#ifndef _MEDIA_AUDIO_STREAM_H_
#define _MEDIA_AUDIO_STREAM_H_

#include <stdint.h>
#include <queue>
#include "system/Thread.h"

typedef void (*stream_data_cb)(uint8_t *data, uint32_t size, void *user_data);

namespace audio {

class StreamTransfer : public Thread {
public:
	StreamTransfer();
	~StreamTransfer();

	bool start(stream_data_cb cb, void *user_data = NULL, bool dup = true, int delay_ms = 0);
	void stop();
	bool put(uint8_t *data, uint32_t size);

protected:
	virtual bool threadLoop();

private:
	struct data_unit_t {
		uint8_t *data;
		uint32_t size;
	};

	std::queue<data_unit_t> data_queue_;
	stream_data_cb cb_;
	void *user_data_;
	bool dup_;
	int delay_ms_;

	Mutex lock_;
};

}

#endif /* _MEDIA_AUDIO_STREAM_H_ */
