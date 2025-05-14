/*
 * net.hpp
 *
 *  Created on: 2023年3月6日
 *      Author: ZKSWE Develop Team
 */

#ifndef _FY_NET_H_
#define _FY_NET_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "utils/Log.h"

namespace fy {
namespace net {

class iface {
public:
	iface(const char *name) {
		name_ = name;
		fd_ = socket(AF_INET, SOCK_DGRAM, 0);
		if (fd_ < 0) {
			LOGE("socket() failed: %s\n", strerror(errno));
		}
	}

	~iface() {
		if (is_valid()) {
			close(fd_);
		}
	}

	bool is_valid() const { return fd_ >= 0; }

	bool up() {
		bool ret = set_flags(IFF_UP, 0);
		LOGD("iface up(%s) = %d\n", name_, ret);
		return ret;
	}

	bool down() {
		bool ret = set_flags(0, IFF_UP);
		LOGD("iface down(%s) = %d\n", name_, ret);
		return ret;
	}

	bool is_up() {
		return has_flags(IFF_UP);
	}

	bool set_addr(const char *addr) {
		if (!is_valid()) {
			return false;
		}

		struct ifreq ifr;
		init_ifr(&ifr);
		init_sockaddr_in(&ifr.ifr_addr, inet_addr(addr));

		int ret = ioctl(fd_, SIOCSIFADDR, &ifr);
		LOGD("set_addr(%s, %s) = %d\n", name_, addr, ret);

		return ret == 0;
	}

	bool set_mask(const char *mask) {
		if (!is_valid()) {
			return false;
		}

		struct ifreq ifr;
		init_ifr(&ifr);
		init_sockaddr_in(&ifr.ifr_addr, inet_addr(mask));

		int ret = ioctl(fd_, SIOCSIFNETMASK, &ifr);
		LOGD("set_mask(%s, %s) = %d\n", name_, mask, ret);

		return ret == 0;
	}

	static bool up(const char *name) {
		iface ifc(name);
		return ifc.up();
	}

	static bool down(const char *name) {
		iface ifc(name);
		return ifc.set_addr("0.0.0.0") && ifc.down();
	}

private:
	void init_ifr(struct ifreq *ifr) {
		memset(ifr, 0, sizeof(struct ifreq));
		strncpy(ifr->ifr_name, name_, IFNAMSIZ);
		ifr->ifr_name[IFNAMSIZ - 1] = 0;
	}

	void init_sockaddr_in(struct sockaddr *sa, in_addr_t addr) {
		struct sockaddr_in *sin = (struct sockaddr_in *) sa;
		sin->sin_family = AF_INET;
		sin->sin_port = 0;
		sin->sin_addr.s_addr = addr;
	}

	bool set_flags(unsigned set, unsigned clr) {
		if (!is_valid()) {
			return false;
		}

		struct ifreq ifr;
		init_ifr(&ifr);

		if (ioctl(fd_, SIOCGIFFLAGS, &ifr) < 0) {
			LOGE("ioctl SIOCGIFFLAGS err\n");
			return false;
		}

		ifr.ifr_flags = (ifr.ifr_flags & (~clr)) | set;
		return ioctl(fd_, SIOCSIFFLAGS, &ifr) == 0;
	}

	bool has_flags(unsigned bits) {
		if (!is_valid()) {
			return false;
		}

		struct ifreq ifr;
		init_ifr(&ifr);

		if (ioctl(fd_, SIOCGIFFLAGS, &ifr) < 0) {
			LOGE("ioctl SIOCGIFFLAGS err\n");
			return false;
		}

		return ifr.ifr_flags & bits;
	}

private:
	int fd_;
	const char *name_;
};

}
}

#endif
