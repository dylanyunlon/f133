/*
 * reverse.cpp
 *
 *  Created on: Dec 7, 2022
 *      Author: ZKSWE Develop Team
 */

#include <fcntl.h>
#include <poll.h>
#include <errno.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <linux/netlink.h>
#include "system/Thread.h"
#include "os/SystemProperties.h"
#include "reverse.h"
#include "common.h"

#define REVERSE_STATUS_NODE   "/sys/devices/platform/soc@3000000/5c01000.tvd0/tvd0_attr/tvd_back_det"
#define UEVENT_MSG_LEN        1024

namespace sys {

static int _s_reverse_status = REVERSE_STATUS_EXIT;
static std::set<reverse_status_cb> _s_cb_set;
static Mutex _s_mutex;

static int _uevent_open_socket(int buf_sz, bool passcred) {
	struct sockaddr_nl addr;
	int on = passcred;
	int s;

	memset(&addr, 0, sizeof(addr));
	addr.nl_family = AF_NETLINK;
	addr.nl_pid = 1234;
	addr.nl_groups = 1;

	s = socket(PF_NETLINK, SOCK_DGRAM, NETLINK_KOBJECT_UEVENT);
	if (s < 0) {
		return -1;
	}

	setsockopt(s, SOL_SOCKET, SO_RCVBUFFORCE, &buf_sz, sizeof(buf_sz));
	setsockopt(s, SOL_SOCKET, SO_PASSCRED, &on, sizeof(on));

	if (bind(s, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
		close(s);
		return -1;
	}

	return s;
}

static ssize_t _uevent_kernel_multicast_recv(int socket, void *buffer, size_t length) {
	struct iovec iov = { buffer, length };
	struct sockaddr_nl addr;
	char control[CMSG_SPACE(sizeof(struct ucred))];
	ssize_t n;
	struct cmsghdr *cmsg;
	struct ucred *cred;
	struct msghdr hdr = {
		&addr,
		sizeof(addr),
		&iov,
		1,
		.msg_control = (void *)control,
		.msg_controllen = sizeof(control),
		0,
	};

	n = recvmsg(socket, &hdr, 0);
	if (n <= 0) {
		return n;
	}

	cmsg = CMSG_FIRSTHDR(&hdr);
	if (cmsg == NULL || cmsg->cmsg_type != SCM_CREDENTIALS) {
		/* ignoring netlink message with no sender credentials */
		goto OUT;
	}

	cred = (struct ucred *) CMSG_DATA(cmsg);
	if (cred->uid != 0) {
		/* ignoring netlink message from non-root user */
		goto OUT;
	}

	if (addr.nl_groups == 0 || addr.nl_pid != 0) {
		/* ignoring non-kernel or unicast netlink message */
		goto OUT;
	}

	return n;

OUT:
	/* clear residual potentially malicious data */
	bzero(buffer, length);
	errno = EIO;
	return -1;
}

static void _notify_status_cb(int status) {
	RETURN_IF_FAIL(_s_reverse_status != status)
	_s_reverse_status = status;

	Mutex::Autolock _l(_s_mutex);
	SET_FOREACH(_s_cb_set, reverse_status_cb, it) {
		(*it)(status);
	}
}

#define DEBUG_REVERSE    0

#if DEBUG_REVERSE
class DebugThread : public Thread {
public:
	bool start() {
		return run("debug_reverse");
	}

	int getStatus() {
		int status = 0;
		SystemProperties::getInt("sys.zkreverse.status", &status, 0);
		return status;
	}

protected:
	virtual bool threadLoop() {
		_notify_status_cb(getStatus());
		DELAY(30);
		return true;
	}
};

static DebugThread _s_debug_thread;
#endif

class DetectThread : public Thread {
public:
	DetectThread() : mWakeFd(-1) {

	}

	bool start() {
		return run("reverse_detect");
	}

	void stop() {
		if (mWakeFd != -1) {
			ssize_t nWrite;
			do {
				nWrite = ::write(mWakeFd, "W", 1);
			} while ((nWrite == -1) && (errno == EINTR));
		}

		requestExit();
	}

protected:
	virtual bool threadLoop() {
		LOGD("reverse detect thread enter !!!\n");

		int event_fd = -1;
		int wake_fds[2] = { -1, -1 };
		struct pollfd ufd[2];

		event_fd = _uevent_open_socket(256 * 1024, true);
		if (event_fd == -1) {
			LOGE("Open uevent socket error!\n");
			goto _EXIT;
		}

		if (pipe(wake_fds) < 0) {
			LOGE("Create pipe error!\n");
			goto _EXIT;
		}

		fcntl(event_fd, F_SETFL, O_NONBLOCK);
		fcntl(wake_fds[0], F_SETFL, O_NONBLOCK);
		fcntl(wake_fds[1], F_SETFL, O_NONBLOCK);

		mWakeFd = wake_fds[1];

		ufd[0].events = POLLIN;
		ufd[0].fd = event_fd;
		ufd[0].revents = 0;

		ufd[1].events = POLLIN;
		ufd[1].fd = wake_fds[0];
		ufd[1].revents = 0;

		while (true) {
			if (poll(ufd, 2, -1) == -1) {
				LOGE("poll error !\n");
				goto _EXIT;
			}

			if (ufd[0].revents & POLLIN) {
				char msg[UEVENT_MSG_LEN + 2];
				int n;
				while ((n = _uevent_kernel_multicast_recv(event_fd, msg, UEVENT_MSG_LEN)) > 0) {
					if (n >= UEVENT_MSG_LEN) {		/* overflow -- discard */
						continue;
					}

					msg[n] = '\0';
					msg[n + 1] = '\0';

					const char *pMsg = msg;
					while (*pMsg) {
						// LOGD("uevent msg: %s\n", pMsg);
						const char *pTag;
						if ((pTag = strstr(pMsg, "ZKVIDEO_BACK=")) != NULL) {
							_notify_status_cb((pTag[13] == '1') ? REVERSE_STATUS_ENTER : REVERSE_STATUS_EXIT);
						}

						/* advance to after the next \0 */
						while (*pMsg++) ;
					}
				}
			}

			if (ufd[1].revents & POLLIN) {
				LOGD("R++++ wake up\n");
				char buffer[16] = { 0 };
				read(wake_fds[0], buffer, sizeof(buffer));
				goto _EXIT;
			}
		}

	_EXIT:
		mWakeFd = -1;

		if (event_fd >= 0) {
			close(event_fd);
		}
		if (wake_fds[0] >= 0) {
			close(wake_fds[0]);
		}
		if (wake_fds[1] >= 0) {
			close(wake_fds[1]);
		}

		LOGD("reverse detect thread exit !!!\n");

		return false;
	}

private:
	int mWakeFd;
};

static DetectThread _s_detect_thread;

static bool _does_reverse_status_enter() {
#if DEBUG_REVERSE
	return _s_debug_thread.getStatus() == REVERSE_STATUS_ENTER;
#endif

	int fd = open(REVERSE_STATUS_NODE, O_RDONLY);
	RETURN_VALUE_IF_FAIL(fd >= 0, false)

	char status = '\0';
	read(fd, &status, 1);
	close(fd);

	return status == '1';
}

bool reverse_detect_start() {
	RETURN_VALUE_IF_FAIL(FILE_EXIST(REVERSE_STATUS_NODE), false)

	// 设置app为running状态，表示应用正常启动，zkfast退出当前倒车后，直接退出进程，不再做检测
	SystemProperties::setString("sys.zkapp.state", "running");

	// 检测快速倒车是否已完全退出，才可执行应用倒车
	while (true) {
		char status[32] = { 0 };
		SystemProperties::getString("init.svc.zklogo", status, "");
		if (strcmp(status, "running") != 0) {
			break;
		}

		DELAY(30);
	}

	if (_does_reverse_status_enter()) {
		_notify_status_cb(REVERSE_STATUS_ENTER);
	}

#if DEBUG_REVERSE
	_s_debug_thread.start();
#endif

	return _s_detect_thread.start();
}

void reverse_detect_stop() {
	_s_detect_thread.stop();
}

void reverse_add_status_cb(reverse_status_cb cb) {
	Mutex::Autolock _l(_s_mutex);
	_s_cb_set.insert(cb);
}

void reverse_remove_status_cb(reverse_status_cb cb) {
	Mutex::Autolock _l(_s_mutex);
	_s_cb_set.erase(cb);
}

bool reverse_does_enter_status() {
	return _s_reverse_status == REVERSE_STATUS_ENTER;
}

}
