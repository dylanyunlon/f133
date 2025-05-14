/*
 * ap.h
 *
 *  Created on: 2023年3月8日
 *      Author: ZKSWE Develop Team
 */

#ifndef _NET_AP_H_
#define _NET_AP_H_

#include <string>

namespace net {
namespace ap {

bool enable();
bool disable();
bool restart_hostapd();

}
}

#endif /* _NET_AP_H_ */
