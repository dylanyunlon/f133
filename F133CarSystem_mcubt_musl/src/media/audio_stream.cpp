/*
 * audio_stream.cpp
 *
 *  Created on: 2023年6月28日
 *      Author: ZKSWE Develop Team
 */

#include "common.h"
#include "audio_stream.h"

namespace audio {

StreamTransfer::StreamTransfer() :
		cb_(NULL),
		user_data_(NULL),
		dup_(true),
		delay_ms_(0) {

}

StreamTransfer::~StreamTransfer() {
	stop();
}

bool StreamTransfer::start(stream_data_cb cb, void *user_data, bool dup, int delay_ms) {
	if (isRunning()) {
		return true;
	}

	cb_ = cb;
	user_data_ = user_data;
	dup_ = dup;
	delay_ms_ = delay_ms;

	return run("stream");
}

void StreamTransfer::stop() {
	requestExitAndWait();
}

bool StreamTransfer::put(uint8_t *data, uint32_t size) {
	if (!isRunning()) {
		return false;
	}

	Mutex::Autolock _l(lock_);

	data_unit_t unit = { data, size };
	if (dup_) {
		unit.data = (uint8_t *) malloc(size);
		memcpy(unit.data, data, size);
	}

	data_queue_.push(unit);

	return true;
}

bool StreamTransfer::threadLoop() {
	LOGD("stream transfer thread start\n");

	data_unit_t unit;

	if (delay_ms_ > 0) {
		DELAY(delay_ms_);
	}

	while (true) {
		{
			Mutex::Autolock _l(lock_);
			if (data_queue_.empty()) {
				if (exitPending()) {
					break;
				}
				continue;
			}

			unit = data_queue_.front();
			data_queue_.pop();
		}

		if (cb_) {
			cb_(unit.data, unit.size, user_data_);
		}

		if (dup_) {
			free(unit.data);
		}
	}

	LOGD("stream transfer thread end\n");

	return false;
}

}
