/*
 * ap.cpp
 *
 *  Created on: 2023年3月8日
 *      Author: ZKSWE Develop Team
 */

#include "fy/files.hpp"
#include "fy/os.hpp"
#include "fy/format.hpp"

#include "system/setting.h"
#include "private.h"
#include "ap.h"
#include "config.h"

namespace net {
namespace ap {

static void _make_hostapd_config() {
	remove(NET_HOSTAPD_CONF_FILE);

	std::string dev_name = sys::setting::get_dev_name();
	std::string data;
	if (sys::setting::get_link_mode() == E_LINK_MODE_AIRPLAY) {
		LOGD("[net] airplay ap make %s\n", NET_HOSTAPD_CONF_FILE);
		data = fy::format(NET_HOSTAPD_AP_CONF_DATA, NET_AP_IFACE, dev_name.c_str());
	} else {
		LOGD("[net] ap make %s\n", NET_HOSTAPD_CONF_FILE);
		data = fy::format(NET_HOSTAPD_CONF_DATA, NET_AP_IFACE, dev_name.c_str());
	}
	fy::files::save_data(NET_HOSTAPD_CONF_FILE, data.c_str(), data.size());
}

static void _make_udhcpd_config() {
	remove(NET_UDHCPD_CONF_FILE);
	std::string data = fy::format(NET_UDHCPD_CONF_DATA, NET_AP_IFACE);
	fy::files::save_data(NET_UDHCPD_CONF_FILE, data.c_str(), data.size());
}

static bool _start_hostapd() {
	fy::net::iface ifc(NET_AP_IFACE);
	RETURN_VALUE_IF_FAIL(ifc.up(), false);
	RETURN_VALUE_IF_FAIL(ifc.set_addr(NET_STATIC_IP), false);
	RETURN_VALUE_IF_FAIL(ifc.set_mask("255.255.255.0"), false);

	_make_hostapd_config();
	_make_udhcpd_config();

	return _start_service(NET_AP_SUPP);
}

static void _stop_hostapd() {
#if 0
	if (sys::setting::get_link_mode() == E_LINK_MODE_AIRPLAY) {
		fy::kill_process(NET_AP_SUPP);
		DELAY(2000);
		return;
	}
#endif
	fy::net::iface::down(NET_AP_IFACE);
	_stop_service(NET_AP_SUPP);
	remove(NET_AP_CTRL_PATH);
}

bool enable() {
	LOGD("[net] ap %s\n", __FUNCTION__);
	RETURN_VALUE_IF_FAIL(wifi_load_driver() == 0, false);

	RETURN_VALUE_IF_FAIL(_start_hostapd(), false);

	_update_udhcpd_conf(NET_AP_IFACE);
	RETURN_VALUE_IF_FAIL(_start_service("udhcpd"), false);

	return true;
}

bool disable() {
	LOGD("[net] ap %s\n", __FUNCTION__);
	_stop_hostapd();
	_stop_service("udhcpd");
	wifi_unload_driver();
	return true;
}

bool restart_hostapd() {
	LOGD("[net] ap %s\n", __FUNCTION__);
	_stop_hostapd();
	return _start_hostapd();
}

}
}
