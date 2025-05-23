/*
 * music_player.cpp
 *
 *  Created on: 2023年2月5日
 *      Author: ZKSWE Develop Team
 */

#include <set>
#include "music_player.h"
#include "audio_context.h"
#include "media_context.h"
#include "misc/storage.h"
#include "uart/context.h"

#define MUSIC_PLAY_MODE_KEY        "music_play_mode_key"
#define MUSIC_PLAY_VOLUME_KEY	   "music_play_volume_key"

namespace media {

static std::set<music_play_status_cb> _s_play_status_cb_set;
static Mutex _s_play_status_mutex;
static bool _s_play_started;
static media_play_mode_e _s_play_mode;
static storage_type_e _s_play_storage_type;
static int _s_play_index = -1;

static void _notify_play_status_cb(music_play_status_e status) {
	Mutex::Autolock _l(_s_play_status_mutex);
	std::set<music_play_status_cb>::iterator it = _s_play_status_cb_set.begin();
	for (; it != _s_play_status_cb_set.end(); ++it) {
		(*it)(status);
	}
}

class MusicPlayerMessageListener : public ZKMediaPlayer::IPlayerMessageListener {
protected:
	virtual void onPlayerMessage(ZKMediaPlayer *pMediaPlayer, int msg, void *pMsgData) {
		// LOGD("music onPlayerMessage %d\n", msg);
		switch (msg) {
		case ZKMediaPlayer::E_MSGTYPE_PLAY_STARTED:
			_s_play_started = true;
			_notify_play_status_cb(E_MUSIC_PLAY_STATUS_STARTED);
			break;

		case ZKMediaPlayer::E_MSGTYPE_PLAY_COMPLETED:
			_s_play_started = false;
			_notify_play_status_cb(E_MUSIC_PLAY_STATUS_COMPLETED);
			break;

		case ZKMediaPlayer::E_MSGTYPE_ERROR_INVALID_FILEPATH:
		case ZKMediaPlayer::E_MSGTYPE_ERROR_MEDIA_ERROR:
			_s_play_started = false;
			_notify_play_status_cb(E_MUSIC_PLAY_STATUS_ERROR);
			break;
		}
	}
};

static MusicPlayerMessageListener _s_player_listener;
static ZKMediaPlayer _s_player(ZKMediaPlayer::E_MEDIA_TYPE_AUDIO);

void music_init() {
	srand(time(NULL));

	_s_play_mode = (media_play_mode_e) storage::get_int(MUSIC_PLAY_MODE_KEY, E_MEDIA_PLAY_MODE_CYCLE);
	_s_player.setPlayerMessageListener(&_s_player_listener);
}

void music_play(storage_type_e type, int file_index, int msec) {
	_s_play_storage_type = type;
	_s_play_index = file_index;
	music_play(get_audio_file(type, file_index).c_str(), msec);
}

int music_get_play_index() {
	return _s_play_index;
}

void music_play(const char *path, int msec) {
//	uart::set_sound_channel(SOUND_CHANNEL_ARM);
	audio::change_audio_type(E_AUDIO_TYPE_MUSIC);
	_s_player.play(path, msec);
}

void music_pause() {
	if (!_s_play_started) {
		return;
	}

	_s_player.pause();
	_notify_play_status_cb(E_MUSIC_PLAY_STATUS_PAUSE);
}

void music_resume() {
	if (!_s_play_started) {
		return;
	}
//	uart::set_sound_channel(SOUND_CHANNEL_ARM);
	audio::change_audio_type(E_AUDIO_TYPE_MUSIC);
	_s_player.resume();
	_notify_play_status_cb(E_MUSIC_PLAY_STATUS_RESUME);
}

void music_seek(int msec) {
	_s_player.seekTo(msec);
}

void music_stop() {
	_s_play_started = false;
	_s_player.stop();
	_s_play_index = -1;		//完全停止,不再恢复
	_notify_play_status_cb(E_MUSIC_PLAY_STATUS_STOP);
}

bool music_is_playing() {
	return _s_player.isPlaying();
}

void music_next(bool isConvert) {
	if (_s_play_mode == E_MEDIA_PLAY_MODE_CYCLE
			|| (isConvert && _s_play_mode == E_MEDIA_PLAY_MODE_SINGLE)) {
		int list_size = get_audio_list_size(_s_play_storage_type);
		if (list_size > 0) {
			_s_play_index = (_s_play_index + 1) % list_size;
			music_play(get_audio_file(_s_play_storage_type, _s_play_index).c_str());
		}
	} else if (_s_play_mode == E_MEDIA_PLAY_MODE_RANDOM) {
		int list_size = get_audio_list_size(_s_play_storage_type);
		if (list_size > 0) {
			int rand_index = rand() % list_size;
			_s_play_index = (rand_index != _s_play_index) ? rand_index : ((_s_play_index + 1) % list_size);
			music_play(get_audio_file(_s_play_storage_type, _s_play_index).c_str());
		}
	} else if (_s_play_mode == E_MEDIA_PLAY_MODE_SINGLE) {
		music_play(get_audio_file(_s_play_storage_type, _s_play_index).c_str());
	}
}

void music_prev(bool isConvert) {
	if (_s_play_mode == E_MEDIA_PLAY_MODE_CYCLE
			|| (isConvert && _s_play_mode == E_MEDIA_PLAY_MODE_SINGLE)) {
		int list_size = get_audio_list_size(_s_play_storage_type);
		if (list_size > 0) {
			_s_play_index = (_s_play_index - 1 + list_size) % list_size;
			music_play(get_audio_file(_s_play_storage_type, _s_play_index).c_str());
		}
	} else if (_s_play_mode == E_MEDIA_PLAY_MODE_RANDOM || _s_play_mode == E_MEDIA_PLAY_MODE_SINGLE) {
		music_next(isConvert);
	}
}


void music_set_play_mode(media_play_mode_e mode) {
	_s_play_mode = mode;
	storage::put_int(MUSIC_PLAY_MODE_KEY, mode);
}

media_play_mode_e music_get_play_mode() {
	return _s_play_mode;
}

void music_set_volume(float volume) {
	_s_player.setVolume(volume);
}

void music_set_gain(float gain) {
	_s_player.setGain(gain);
}

int music_get_duration() {
	return _s_player.getDuration();
}

int music_get_current_position() {
	return _s_player.getCurrentPosition();
}

std::string music_get_current_play_file(bool check_state) {
	if (!check_state || _s_play_started) {
		return get_audio_file(_s_play_storage_type, _s_play_index);
	} else {
		return "";
	}
}

void music_add_play_status_cb(music_play_status_cb cb) {
	Mutex::Autolock _l(_s_play_status_mutex);
	_s_play_status_cb_set.insert(cb);
}

void music_remove_play_status_cb(music_play_status_cb cb) {
	Mutex::Autolock _l(_s_play_status_mutex);
	_s_play_status_cb_set.erase(cb);
}

}
