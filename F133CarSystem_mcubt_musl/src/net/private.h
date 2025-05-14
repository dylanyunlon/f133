/*
 * private.h
 *
 *  Created on: 2023年3月18日
 *      Author: ZKSWE Develop Team
 */

#ifndef _NET_PRIVATE_H_
#define _NET_PRIVATE_H_

#include <errno.h>
#include "common.h"
#include "fy/net.hpp"
#include "os/SystemProperties.h"

#ifndef TEMP_FAILURE_RETRY
#define TEMP_FAILURE_RETRY(exp) ({         \
    long int _rc;                          \
    do {                                   \
        _rc = (exp);                       \
    } while (_rc == -1 && errno == EINTR); \
    _rc; })
#endif

extern "C" {
int wifi_load_driver();
int wifi_unload_driver();
int is_wifi_driver_loaded();
};

static inline bool _does_service_running(const char *name) {
	char key[32], status[32] = { 0 };
	sprintf(key, "init.svc.%s", name);
	SystemProperties::getString(key, status, "");
	return strcmp(status, "running") == 0;
}

static inline bool _start_service(const char *name) {
	if (_does_service_running(name)) {
		return true;
	}

	LOGD("[net] to start %s service\n", name);

	SystemProperties::setString("ctl.start", name);
	DELAY(100);

	for (int i = 0; i < 100; ++i) {
		if (_does_service_running(name)) {
			LOGD("[net] start %s service ok\n", name);
			return true;
		}

		DELAY(100);
	}

	LOGE("[net] start %s service fail !!!\n", name);

	return false;
}

static inline bool _stop_service(const char *name) {
	if (!_does_service_running(name)) {
		return true;
	}

	LOGD("[net] to stop %s service\n", name);

	SystemProperties::setString("ctl.stop", name);
	DELAY(100);

	for (int i = 0; i < 100; ++i) {
		if (!_does_service_running(name)) {
			LOGD("[net] stop %s service ok\n", name);
			return true;
		}

		DELAY(100);
	}

	LOGE("[net] stop %s service fail !!!\n", name);

	return false;
}

inline void _update_udhcpd_conf(const char *name) {
	const char *conf_file = NET_UDHCPD_CONF_FILE;
	FILE *pf = fopen(conf_file, "r");
	if (!pf) {
		LOGE("failed to open %s read\n", conf_file);
		return;
	}

	int len = 0;
	char content[1024];

	if (fgets(content, sizeof(content), pf) && !strstr(content, name)) {
		len = fread(content, 1, sizeof(content), pf);
	}
	fclose(pf);

	if (len > 0) {
		pf = fopen(conf_file, "w");
		if (!pf) {
			LOGE("failed to open %s write\n", conf_file);
			return;
		}

		char buf[64];
		sprintf(buf, "interface %s\n", name);

		fwrite(buf, 1, strlen(buf), pf);
		fwrite(content, 1, len, pf);
		fflush(pf);
		fclose(pf);
	}
}

#endif /* _NET_PRIVATE_H_ */
