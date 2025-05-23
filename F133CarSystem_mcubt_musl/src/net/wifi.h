/*
 * wifi.h
 *
 *  Created on: 2023年4月18日
 *      Author: ZKSWE Develop Team
 */

#ifndef _NET_WIFI_H_
#define _NET_WIFI_H_

#include "net/NetManager.h"

#define WIFIMANAGER        NETMANAGER->getWifiManager()

namespace net {
namespace wifi {

bool enable();
bool disable();

}
}

#endif /* _NET_WIFI_H_ */
