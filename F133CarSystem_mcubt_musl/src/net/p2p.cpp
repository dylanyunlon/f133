/*
 * p2p.cpp
 *
 *  Created on: 2023年3月8日
 *      Author: ZKSWE Develop Team
 */

#include <fcntl.h>
#include <poll.h>
#include "fy/files.hpp"
#include "fy/os.hpp"
#include "fy/format.hpp"
#include "system/Thread.h"
#include "private.h"
#include "wpa_ctrl.h"
#include "p2p.h"

namespace net {
namespace p2p {

#define MATCH(str, tag) (strncmp(str, tag, strlen(tag)) == 0)

class P2PManager : public Thread {
public:
	P2PManager() {
		ctrl_ = NULL;
		exit_pipe_[0] = exit_pipe_[1] = -1;
	}

	bool enable() {
		if (!connectSupplicant()) {
			LOGE("[net] p2p connect supplicant fail\n");
			return false;
		}

		if (!group_add()) {
			LOGE("[net] p2p group add fail\n");
			return false;
		}

		return run("p2p");
	}

	void disable() {
		wakeup();
		requestExit();
		disconnectSupplicant();
	}

	bool start_find() {
		RETURN_VALUE_IF_FAIL(ctrl_, false);

		char reply[1024];
		RETURN_VALUE_IF_FAIL(doCommand("P2P_FIND", reply, sizeof(reply)), false);

		return true;
	}

	bool stop_find() {
		RETURN_VALUE_IF_FAIL(ctrl_, false);

		char reply[1024];
		RETURN_VALUE_IF_FAIL(doCommand("P2P_STOP_FIND", reply, sizeof(reply)), false);

		return true;
	}

	bool group_add() {
		RETURN_VALUE_IF_FAIL(ctrl_, false);

		char reply[1024];
		RETURN_VALUE_IF_FAIL(doCommand("P2P_GROUP_ADD freq=5", reply, sizeof(reply)), false);

		return true;
	}

	bool group_remove() {
		RETURN_VALUE_IF_FAIL(ctrl_, false);

		char reply[1024];
		RETURN_VALUE_IF_FAIL(doCommand("P2P_GROUP_REMOVE", reply, sizeof(reply)), false);
		RETURN_VALUE_IF_FAIL(doCommand("P2P_FLUSH", reply, sizeof(reply)), false);

		return true;
	}

	bool connect(const char *addr) {
		RETURN_VALUE_IF_FAIL(ctrl_, false);

		char cmd[64];
		sprintf(cmd, "P2P_CONNECT %s pbc go_intent=14", addr);

		char reply[1024];
		RETURN_VALUE_IF_FAIL(doCommand(cmd, reply, sizeof(reply)), false);

		return true;
	}

private:
	virtual bool threadLoop() {
		struct pollfd rfds[2] = { 0 };
		rfds[0].fd = exit_pipe_[0];
		rfds[0].events = POLLIN;
		rfds[1].fd = wpa_ctrl_get_fd(ctrl_);
		rfds[1].events = POLLIN;

		int res = TEMP_FAILURE_RETRY(poll(rfds, 2, -1));
		if (res < 0) {
			LOGE("[net] p2p error poll = %d\n", res);
			return false;
		}

		if (rfds[0].revents & POLLIN) {
			LOGE("[net] p2p exit thread !!!\n");
			return false;
		}

		if (rfds[1].revents & POLLIN) {
			char reply[1024];
			size_t reply_len = sizeof(reply) - 1;
			if (wpa_ctrl_recv(ctrl_, reply, &reply_len) < 0) {
				LOGE("[net] p2p wpa_ctrl_recv error: %s\n", strerror(errno));
				return true;
			}

			if (reply_len == 0) {
				LOGE("[net] p2p received EOF on supplicant socket\n");
				return true;
			}

			reply[reply_len] = '\0';
			if (reply[0] == '<') {
				char *match = strchr(reply, '>');
				if (match != NULL) {
					reply_len -= (match + 1 - reply);
					memmove(reply, match + 1, reply_len + 1);
				}
			}

			LOGD("[net] p2p recv reply: %s\n", reply);

			if (MATCH(reply, P2P_EVENT_PROV_DISC_PBC_REQ)) {
				// 按钮配置，无需密码 (Push Button Configuration，PBC)
				doCommand("WPS_PBC", reply, sizeof(reply));
			} else if (MATCH(reply, AP_STA_CONNECTED)) {

			} else if (MATCH(reply, AP_STA_DISCONNECTED)) {

			}
		}

		return true;
	}

	bool connectSupplicant() {
		struct wpa_ctrl *ctrl = NULL;
		for (int i = 0; i < 30; ++i) {
			ctrl = wpa_ctrl_open(NET_P2P_CTRL_PATH);
			if (ctrl) {
				break;
			}

			DELAY(100);
		}

		RETURN_VALUE_IF_FAIL(ctrl, false);

		if (wpa_ctrl_attach(ctrl) != 0) {
			LOGE("[net] p2p wpa_ctrl_attach fail\n");
			wpa_ctrl_close(ctrl);
			return false;
		}

		if (pipe(exit_pipe_) < 0) {
			LOGE("[net] p2p create pipe fail\n");
			wpa_ctrl_close(ctrl);
			return false;
		}

		fcntl(exit_pipe_[0], F_SETFL, O_NONBLOCK);
		ctrl_ = ctrl;

		return true;
	}

	void disconnectSupplicant() {
		if (ctrl_) {
			wpa_ctrl_detach(ctrl_);
			wpa_ctrl_close(ctrl_);
			ctrl_ = NULL;
		}

		if (exit_pipe_[0] != -1) {
			close(exit_pipe_[0]);
			exit_pipe_[0] = -1;
		}
		if (exit_pipe_[1] != -1) {
			close(exit_pipe_[1]);
			exit_pipe_[1] = -1;
		}
	}

	void wakeup() {
		RETURN_IF_FAIL(exit_pipe_[1] != -1);
		TEMP_FAILURE_RETRY(write(exit_pipe_[1], "W", 1));
	}

	bool doCommand(const char *cmd, char *reply, size_t reply_len) {
		RETURN_VALUE_IF_FAIL(ctrl_, false);

		reply_len--;    // Ensure we have room to add NUL termination.
		int ret = wpa_ctrl_request(ctrl_, cmd, strlen(cmd), reply, &reply_len, NULL);

		if (ret == -2) {
			LOGE("[net] p2p '%s' command timed out.\n", cmd);
			return false;
		} else if ((ret < 0) || (strncmp(reply, "FAIL", 4) == 0)) {
			LOGE("[net] p2p command reply FAIL \n");
			return false;
		}

		if (reply[reply_len - 1] == '\n') {
			reply[reply_len - 1] = '\0';
		} else {
			reply[reply_len] = '\0';
		}

		LOGD("[net] p2p cmd: %s\n", cmd);
		LOGD("[net] p2p reply: %s\n", reply);

		return true;
	}

private:
	struct wpa_ctrl *ctrl_;
	int exit_pipe_[2];
};

static P2PManager _s_manager;

static void _make_udhcpd_config() {
	remove(NET_UDHCPD_CONF_FILE);
	std::string data = fy::format(NET_UDHCPD_CONF_DATA, NET_AP_IFACE);;
	fy::files::save_data(NET_UDHCPD_CONF_FILE, data.c_str(), data.size());
}

bool enable() {
	LOGD("[net] p2p %s\n", __FUNCTION__);

	RETURN_VALUE_IF_FAIL(wifi_load_driver() == 0, false);

	fy::net::iface ifc(NET_P2P_IFACE);
	RETURN_VALUE_IF_FAIL(ifc.up(), false);
	RETURN_VALUE_IF_FAIL(ifc.set_addr(NET_STATIC_IP), false);
	RETURN_VALUE_IF_FAIL(ifc.set_mask("255.255.255.0"), false);

	_make_udhcpd_config();

	_update_udhcpd_conf(NET_P2P_IFACE);
	RETURN_VALUE_IF_FAIL(_start_service(NET_P2P_SUPP), false);
	RETURN_VALUE_IF_FAIL(_start_service("udhcpd"), false);

	return _s_manager.enable();
}

bool disable() {
	LOGD("[net] p2p %s\n", __FUNCTION__);

	_s_manager.disable();

	_stop_service(NET_P2P_SUPP);
	_stop_service("udhcpd");

	fy::net::iface::down(NET_P2P_IFACE);
	remove(NET_P2P_CTRL_PATH);

	wifi_unload_driver();

	return true;
}

}
}
