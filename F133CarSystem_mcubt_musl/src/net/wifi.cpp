/*
 * wifi.cpp
 *
 *  Created on: 2023年4月18日
 *      Author: ZKSWE Develop Team
 */

#include "common.h"
#include "wifi.h"

namespace net {
namespace wifi {

bool enable() {
	if (WIFIMANAGER->getEnableStatus() != E_WIFI_ENABLE_ENABLE) {
		LOGD("[net] wifi %s\n", __FUNCTION__);
		for (int cnt = 0; cnt < 15; ++cnt) {
			if ((cnt % 10) == 0) {
				WIFIMANAGER->enableWifi(true);
			}

			if (WIFIMANAGER->getEnableStatus() == E_WIFI_ENABLE_ENABLE) {
				LOGD("[net] wifi %s ok\n", __FUNCTION__);
				break;
			}

			DELAY(1000);
		}
	}

	return WIFIMANAGER->getEnableStatus() == E_WIFI_ENABLE_ENABLE;
}

bool disable() {
	if (WIFIMANAGER->getEnableStatus() != E_WIFI_ENABLE_DISABLE) {
		LOGD("[net] wifi %s\n", __FUNCTION__);
		for (int cnt = 0; cnt < 15; ++cnt) {
			if ((cnt % 10) == 0) {
				WIFIMANAGER->enableWifi(false);
			}

			if (WIFIMANAGER->getEnableStatus() == E_WIFI_ENABLE_DISABLE) {
				break;
			}

			DELAY(1000);
		}
	}

	return WIFIMANAGER->getEnableStatus() == E_WIFI_ENABLE_DISABLE;
}

}
}
