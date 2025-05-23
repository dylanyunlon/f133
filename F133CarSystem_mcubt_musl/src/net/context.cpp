/*
 * context.cpp
 *
 *  Created on: 2023年3月6日
 *      Author: ZKSWE Develop Team
 */

#include <sys/utsname.h>
#include "fy/os.hpp"
#include "fy/handler.hpp"
#include "private.h"
#include "context.h"
#include "fy/files.hpp"
#include "wifi.h"
#include "ap.h"
#include "p2p.h"
#include "link/context.h"
#include "system/setting.h"

namespace net {

static net_mode_e _s_net_mode;
static std::set<net_mode_update_cb> _s_cb_set;
static Mutex _s_mutex;

static void _message_handler(const fy::message *msg, const void *user_data);
static fy::handler _s_handler(_message_handler, NULL);

static void _notify_mode_update_cb(net_mode_e mode) {
	if (_s_net_mode == mode) {
		return;
	}

	_s_net_mode = mode;

	Mutex::Autolock _l(_s_mutex);
	SET_FOREACH(_s_cb_set, net_mode_update_cb, it) {
		(*it)(mode);
	}
}

static void _disable_current_net() {
	switch (_s_net_mode) {
	case E_NET_MODE_AP:
		ap::disable();
		break;

	case E_NET_MODE_WIFI:
		wifi::disable();
		break;

	case E_NET_MODE_P2P:
		p2p::disable();
		break;

	default:
		break;
	}
}

class MyWifiListener : public WifiManager::IWifiListener {
public:
	void handleWifiEnable(E_WIFI_ENABLE event, int args) {
		switch (event) {
		case E_WIFI_ENABLE_ENABLEING:
			LOGD("[net] wifi enabling ...\n");
			_disable_current_net();
			break;

		case E_WIFI_ENABLE_ENABLE:
			LOGD("[net] wifi enable ...\n");
			_notify_mode_update_cb(E_NET_MODE_WIFI);
			break;

		case E_WIFI_ENABLE_DISABLE:
			LOGD("[net] wifi disable ...\n");
			// wifi关闭后，切换到ap模式
			if (_s_net_mode == E_NET_MODE_WIFI) {
				change_mode(E_NET_MODE_AP);
			}
			break;

		default:
			break;
		}
	}

	virtual void handleWifiConnect(E_WIFI_CONNECT event, int args) {
#if 1  // carlife need setup
		switch (event) {
		case E_WIFI_CONNECT_CONNECTED: {
			if (sys::setting::get_link_mode() == E_LINK_MODE_CARLIFE) {
				LYLINKAPI_WIFI_PARAM param;
				memset(&param, 0, sizeof(param));
				param.stadisc.id = LYLINK_WIFI_STA_CONNECTED;
				WIFIMANAGER->getConfigureInfo(NULL, NULL, param.stadisc.gateway, NULL, NULL);
				LOGD("[net] wifi gw: %s\n", param.stadisc.gateway);
				lylinkapi_wifi(&param);
			}
		}
			break;
		case E_WIFI_CONNECT_DISCONNECT: {
			LYLINKAPI_WIFI_PARAM param;
			param.apclose.id = LYLINK_WIFI_STA_DISCONNECTED;
			lylinkapi_wifi(&param);
		}
			break;
		default:
			break;
		}
#endif
	}
};

static void _message_handler(const fy::message *msg, const void *user_data) {
	LOGD("[net] handle what: %d\n", msg->what);

	if (!msg->arg1 && (_s_net_mode == msg->what)) {
		LOGD("[net] same mode, do nothing\n");
		return;
	}

	_disable_current_net();

	switch (msg->what) {
	case E_NET_MODE_AP:
		if (ap::enable()) {
			_notify_mode_update_cb(E_NET_MODE_AP);
		} else {
			ap::disable();
			_notify_mode_update_cb(E_NET_MODE_NULL);
		}
		break;

	case E_NET_MODE_WIFI:
		if (wifi::enable()) {
			_notify_mode_update_cb(E_NET_MODE_WIFI);
		} else {
			wifi::disable();
			_notify_mode_update_cb(E_NET_MODE_NULL);
		}
		break;

	case E_NET_MODE_P2P:
		if (p2p::enable()) {
			_notify_mode_update_cb(E_NET_MODE_P2P);
		} else {
			p2p::disable();
			_notify_mode_update_cb(E_NET_MODE_NULL);
		}

		// wifi关闭会切换到ap模式，移除该事件
		_s_handler.remove_messages(E_NET_MODE_AP);
		break;

	default:
		_notify_mode_update_cb(E_NET_MODE_NULL);
		break;
	}
}

static bool _is_module_exist(const char *name) {
	const char *modules_path[] = {
		"/lib/modules",
		"/late/lib/modules",
	};

	struct utsname u;
	char path[256];

	uname(&u);

	for (size_t i = 0; i < TAB_SIZE(modules_path); ++i) {
		sprintf(path, "%s/%s/%s.ko", modules_path[i], u.release, name);
		if (FILE_EXIST(path)) {
			return true;
		}
	}

	return false;
}

static MyWifiListener _s_wifi_listener;

void init() {
	const char *modules_name[] = {
		"8821cs",
		"8821cu",
		"8733bs",
	};

	for (size_t i = 0; i < TAB_SIZE(modules_name); ++i) {
		if (_is_module_exist(modules_name[i])) {
			// rtw_vht_enable=2
			const char *args_list = "rtw_country_code=" NET_COUNTRY_CODE;
			const char *args_key = "persist.wifi.args";
			char args_val[32] = { 0 };

			SystemProperties::getString(args_key, args_val, "");
			if (strcmp(args_val, args_list) != 0) {
				SystemProperties::setString(args_key, args_list);
			}

			break;
		}
	}

	// 设置套接字接收缓冲区的最大字节数5M
	system("echo 5242880 > /proc/sys/net/core/rmem_max");

	mkdir("/data/run", 0666);
	mkdir(NET_AP_CTRL_DIR, 0666);

	std::string device_name = sys::setting::get_dev_name();

	if (fy::files::size_of(NET_P2P_SUP_CONF_FILE) <= 0) {
		std::string data = fy::format(NET_P2P_SUP_CONF_DATA, device_name.c_str());
		fy::files::save_data(NET_P2P_SUP_CONF_FILE, data.c_str(), data.size());
	}

	WIFIMANAGER->addWifiListener(&_s_wifi_listener);

	int wifi_on;
	SystemProperties::getInt("persist.wifi.on", &wifi_on, 0);
	if (!wifi_on) {
//		if (sys::setting::get_link_mode() == E_LINK_MODE_MIRACAST) {
		if (sys::setting::get_link_mode() == E_LINK_MODE_LYLINK || sys::setting::get_link_mode() == E_LINK_MODE_MIRACAST) {
			change_mode(E_NET_MODE_P2P);
		} else {
			change_mode(E_NET_MODE_AP);
		}
	} else {
		if (WIFIMANAGER->isWifiEnable()) {
			_notify_mode_update_cb(E_NET_MODE_WIFI);
		}
	}
}

net_mode_e get_mode() {
	return _s_net_mode;
}

void change_mode(net_mode_e mode, bool force) {
	if (!force && (_s_net_mode == mode)) {
		return;
	}

	_s_handler.send_message(fy::message(mode, NULL, force));
}

void add_mode_update_cb(net_mode_update_cb cb) {
	Mutex::Autolock _l(_s_mutex);
	_s_cb_set.insert(cb);
}

void remove_mode_update_cb(net_mode_update_cb cb) {
	Mutex::Autolock _l(_s_mutex);
	_s_cb_set.erase(cb);
}

}
