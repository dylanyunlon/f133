/*
 * mode_observer.cpp
 *
 *  Created on: 2024年3月2日
 *      Author: guoxs
 */

#include "misc/storage.h"
#include "mode_observer.h"
#include "common.h"

namespace mode {

static std::set<event_mode_cb> _s_event_mode_set;
static std::set<sound_mode_cb> _s_sound_mode_set;
static event_mode_e _s_event_mode = E_EVENT_MODE_NULL;

void set_event_mode(event_mode_e mode) {
	_s_event_mode = mode;
	SET_FOREACH(_s_event_mode_set, event_mode_cb, it) {
		(*it)(mode);
	}
}

event_mode_e get_event_mode() {
	return _s_event_mode;
}

void add_event_mode_cb(event_mode_cb cb) {
	_s_event_mode_set.insert(cb);
}

void remove_event_mode_cb(event_mode_cb cb) {
	_s_event_mode_set.erase(cb);
}

void set_sound_mode(sound_mode_e mode) {
	sys::setting::set_sound_mode(mode);
	SET_FOREACH(_s_sound_mode_set, sound_mode_cb, it) {
		(*it)(mode);
	}
}

sound_mode_e get_sound_mode() {
	return sys::setting::get_sound_mode();
}

void add_sound_mode_cb(sound_mode_cb cb) {
	_s_sound_mode_set.insert(cb);
}

void remove_sound_mode_cb(sound_mode_cb cb) {
	_s_sound_mode_set.erase(cb);
}

}
