/*
 * context.h
 *
 *  Created on: 2023年3月6日
 *      Author: ZKSWE Develop Team
 */

#ifndef _NET_CONTEXT_H_
#define _NET_CONTEXT_H_

#include "common.h"

typedef enum {
	E_NET_MODE_NULL,
	E_NET_MODE_AP,     // 热点
	E_NET_MODE_WIFI,   // wifi sta
	E_NET_MODE_P2P,    // wifi直连
} net_mode_e;

typedef void (*net_mode_update_cb)(net_mode_e mode);

namespace net {

void init();

// 获取当前网络模式
net_mode_e get_mode();

// 切换网络模式
void change_mode(net_mode_e mode, bool force = false);

void add_mode_update_cb(net_mode_update_cb cb);
void remove_mode_update_cb(net_mode_update_cb cb);

}

#endif /* _NET_CONTEXT_H_ */
