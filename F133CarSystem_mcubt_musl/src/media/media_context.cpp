/*
 * media_context.cpp
 *
 *  Created on: 2023年2月2日
 *      Author: ZKSWE Develop Team
 */

#include <dirent.h>
#include <sys/stat.h>
#include <set>
#include "manager/ConfigManager.h"
#include "media_context.h"
#include "audio_context.h"
#include "music_player.h"
#include "os/MountMonitor.h"
#include "fy/handler.hpp"
#include "h264_player.h"

namespace media {

// 媒体文件存放列表
static std::vector<std::string> _s_audio_file_list[E_STORAGE_TYPE_INVALID];
static std::vector<std::string> _s_video_file_list[E_STORAGE_TYPE_INVALID];
static std::vector<std::string> _s_image_file_list[E_STORAGE_TYPE_INVALID];

static std::set<media_scan_cb> _s_media_scan_cb_set;
static Mutex _s_media_scan_mutex;
static bool _s_scan_started;

static const storage_info_t _s_storage_info_tab[] = {
	{ E_STORAGE_TYPE_SD, "/mnt/extsd" },
	{ E_STORAGE_TYPE_USB1, "/mnt/usb1" },
	{ E_STORAGE_TYPE_USB2, "/mnt/usbotg" },
};

static inline storage_type_e _get_storage_type(const char *root_path) {
	for (int i = 0; i < (int) TAB_SIZE(_s_storage_info_tab); ++i) {
		if (strcmp(_s_storage_info_tab[i].root_path, root_path) == 0) {
			return _s_storage_info_tab[i].type;
		}
	}
	return E_STORAGE_TYPE_INVALID;
}

static inline const char* _get_root_path(storage_type_e type) {
	for (int i = 0; i < (int) TAB_SIZE(_s_storage_info_tab); ++i) {
		if (_s_storage_info_tab[i].type == type) {
			return _s_storage_info_tab[i].root_path;
		}
	}
	return NULL;
}

// 忽略扫描的文件夹
static inline bool _is_ignored_folder(const char *folder) {
	const char *ignored_tab[] = {
		"LOST.DIR",
		"$RECYCLE.BIN",
		"System Volume Information",
		NULL
	};

	for (int i = 0; ignored_tab[i] != NULL; ++i) {
		if (strcasecmp(folder, ignored_tab[i]) == 0) {
			return true;
		}
	}

	return false;
}

static inline bool _is_match_suffix(const char *suffix, const char **suffix_tab) {
	for (int i = 0; suffix_tab[i] != NULL; ++i) {
		if (strcasecmp(suffix, suffix_tab[i]) == 0) {
			return true;
		}
	}
	return false;
}

// 扫描过程中回调
static void _scan_cb(const char *file, void *user_data) {
	const char *suffix = strrchr(file, '.');
	if (!suffix) {
		return;
	}

	suffix++;

	storage_type_e type = *(storage_type_e *) user_data;

	// audio file
	const char *audio_suffix_tab[] = {
		"wav", "mp3", "ogg", "flac", "aac", "ape", NULL
	};
	if (_is_match_suffix(suffix, audio_suffix_tab)) {
		LOGD("audio file: %s\n", file);
		Mutex::Autolock _l(_s_media_scan_mutex);
		_s_audio_file_list[type].push_back(file);
		return;
	}

	// video file
	const char *video_suffix_tab[] = {
		"mp4", "mkv", "mov", "flv", "ts", "3gp", "vob", NULL
	};

	if (_is_match_suffix(suffix, video_suffix_tab)) {
		//LOGD("video file: %s\n", file);
		Mutex::Autolock _l(_s_media_scan_mutex);
		_s_video_file_list[type].push_back(file);
		return;
	}

	// image file
	const char *image_suffix_tab[] = {
		"jpg", "png", "jpeg", "bmp", NULL
	};

	if (_is_match_suffix(suffix, image_suffix_tab)) {
		//LOGD("image file: %s\n", file);
		Mutex::Autolock _l(_s_media_scan_mutex);
		_s_image_file_list[type].push_back(file);
		return;
	}
}

// 执行扫描
static void _proc_scan(const char *dir, void (*cb)(const char *file, void *user_data), void *user_data) {
	if (!cb) {
		return;
	}

	DIR *dp = opendir(dir);
	if (!dp) {
		return;
	}

	struct dirent *entry;
	struct stat buff;

	char path[MEDIA_FILE_MAX_PATH];
	strcpy(path, dir);

	int dir_len = strlen(dir);
	if (path[dir_len - 1] != '/') {
		path[dir_len++] = '/';
	}

	char *file_name = path + dir_len;

	while ((entry = readdir(dp)) != NULL) {
		if (entry->d_name[0] == '.') {
			continue;
		}
		if ((dir_len + strlen(entry->d_name)) >= MEDIA_FILE_MAX_PATH) {
			continue;
		}

		strcpy(file_name, entry->d_name);
		if (stat(path, &buff)) {
			LOGE("path: %s, stat error!", path);
			continue;
		}

		if (S_ISREG(buff.st_mode)) {
			cb(path, user_data);
		} else if (S_ISDIR(buff.st_mode)) {
			if (!_is_ignored_folder(entry->d_name)) {
				_proc_scan(path, cb, user_data);
			}
		}
	}

	closedir(dp);
}

static void _notify_scan_cb(const char *dir, storage_type_e type, bool started) {
	Mutex::Autolock _l(_s_media_scan_mutex);
	std::set<media_scan_cb>::iterator it = _s_media_scan_cb_set.begin();
	for (; it != _s_media_scan_cb_set.end(); ++it) {
		(*it)(dir, type, started);
	}
}


static void _message_handler(const fy::message *msg, const void *user_data);
static fy::handler _s_handler(_message_handler, NULL);


#define MSG_SCAN       1
#define MSG_REMOVE     2

static void _message_handler(const fy::message *msg, const void *user_data) {
	switch (msg->what) {
	case MSG_SCAN: {
		storage_type_e type = (storage_type_e) msg->arg1;
		const char *root_path = _get_root_path(type);
		if (!root_path) {
			break;
		}

		_s_scan_started = true;
		_notify_scan_cb(root_path, type, true);

		LOGD("start scan %s\n", root_path);
		_proc_scan(root_path, _scan_cb, (void *) &type);
		LOGD("scan end...\n");

		if (!_s_handler.has_message(MSG_SCAN)) {
			_s_scan_started = false;
		}

		_notify_scan_cb(root_path, type, false);
		break;
	}

	case MSG_REMOVE: {
		storage_type_e type = (storage_type_e) msg->arg1;
		const char *root_path = _get_root_path(type);
		if (!root_path) {
			break;
		}

		_notify_scan_cb(root_path, type, false);

		{
			Mutex::Autolock _l(_s_media_scan_mutex);
			_s_audio_file_list[type].clear();
			_s_video_file_list[type].clear();
			_s_image_file_list[type].clear();
		}

		_notify_scan_cb(root_path, type, false);
		break;
	}
	}
}

class MediaMountListener : public MountMonitor::IMountListener {
protected:
	virtual void notify(int what, int status, const char *msg) {
		switch (status) {
		case MountMonitor::E_MOUNT_STATUS_MOUNTED: {
			LOGD("media mount path: %s\n", msg);
			storage_type_e type = _get_storage_type(msg);
			if (type != E_STORAGE_TYPE_INVALID) {
				_s_handler.send_message(fy::message(MSG_SCAN, NULL, type));
			}
			break;
		}

		case MountMonitor::E_MOUNT_STATUS_UNMOUNTING: {
			LOGD("media remove path: %s\n", msg);
			storage_type_e type = _get_storage_type(msg);
			if (type != E_STORAGE_TYPE_INVALID) {
				// 检测当前播放歌曲是否在移除的设备中，需停止播放
				std::string play_file = music_get_current_play_file(false);
				std::string root_path = std::string(msg) + "/";
				if (root_path.compare(0, root_path.size(), play_file, 0, root_path.size()) == 0) {
					LOGE("need to stop music\n");
					music_stop();
				}
				fy::message msg = fy::message(MSG_REMOVE, NULL, type);
				_s_handler.send_message_delayed(msg, 50);
				//_s_handler.send_message(fy::message(MSG_REMOVE, NULL, type));
			}
			break;
		}
		}
	}
};

static MediaMountListener _s_mount_listener;

void init() {
	// 2 * 1024 * 1024 = 2097152   1024对齐
	setenv("ZKMEDIA_H264_VBVSIZE", "1048576", 1);
//	ZKMEDIA_VIDEO_MAX_SIZE与VD_LBC_MODE不能同时使用；启用LBC提高视频画质，并不会占用太多的内存
#if 1
	setenv("ZKMEDIA_VIDEO_MAX_SIZE", "640x480", 1);
#else
	enum disp_rot_e rot = (enum disp_rot_e) (CONFIGMANAGER->getScreenRotate() / 90);
	setenv("VD_LBC_MODE", "2", 1);
	if(rot == E_DISP_ROT_0) {
		setenv("VD_LBC_RC_EN", "1", 1);
	} else {
		setenv("VD_LBC_RC_EN", "0", 1);
	}
#endif

	audio::init();
	music_init();
	zk_h264_player_preload();
	MOUNTMONITOR->addMountListener(&_s_mount_listener);

	for (int i = 0; i < (int) TAB_SIZE(_s_storage_info_tab); ++i) {
		_s_handler.send_message(fy::message(MSG_SCAN, NULL, _s_storage_info_tab[i].type));
	}
}

void add_scan_cb(media_scan_cb cb) {
	Mutex::Autolock _l(_s_media_scan_mutex);
	_s_media_scan_cb_set.insert(cb);
}

void remove_scan_cb(media_scan_cb cb) {
	Mutex::Autolock _l(_s_media_scan_mutex);
	_s_media_scan_cb_set.erase(cb);
}

bool is_scanning() {
	return _s_scan_started;
}

int get_audio_list_size(storage_type_e type) {
	Mutex::Autolock _l(_s_media_scan_mutex);
	return (type < E_STORAGE_TYPE_INVALID) ? _s_audio_file_list[type].size() : 0;
}

std::string get_audio_file(storage_type_e type, int index) {
	Mutex::Autolock _l(_s_media_scan_mutex);
	if (type >= E_STORAGE_TYPE_INVALID) {
		return "";
	}
	if ((index < 0) || (index >= (int) _s_audio_file_list[type].size())) {
		return "";
	}
	return _s_audio_file_list[type].at(index);
}

int get_video_list_size(storage_type_e type) {
	Mutex::Autolock _l(_s_media_scan_mutex);
	return (type < E_STORAGE_TYPE_INVALID) ? _s_video_file_list[type].size() : 0;
}

std::string get_video_file(storage_type_e type, int index) {
	Mutex::Autolock _l(_s_media_scan_mutex);
	if (type >= E_STORAGE_TYPE_INVALID) {
		return "";
	}
	if ((index < 0) || (index >= (int) _s_video_file_list[type].size())) {
		return "";
	}
	return _s_video_file_list[type].at(index);
}

int get_image_list_size(storage_type_e type) {
	Mutex::Autolock _l(_s_media_scan_mutex);
	return (type < E_STORAGE_TYPE_INVALID) ? _s_image_file_list[type].size() : 0;
}

std::string get_image_file(storage_type_e type, int index) {
	Mutex::Autolock _l(_s_media_scan_mutex);
	if (type >= E_STORAGE_TYPE_INVALID) {
		return "";
	}
	if ((index < 0) || (index >= (int) _s_image_file_list[type].size())) {
		return "";
	}
	return _s_image_file_list[type].at(index);
}

}
