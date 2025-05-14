/*
 * reverse.h
 *
 *  Created on: Dec 7, 2022
 *      Author: ZKSWE Develop Team
 */

#ifndef _SYSTEM_REVERSE_H_
#define _SYSTEM_REVERSE_H_

#define REVERSE_STATUS_ENTER    1    // 进入倒车
#define REVERSE_STATUS_EXIT     0    // 退出倒车

typedef void (*reverse_status_cb)(int status);

namespace sys {

// 开启倒车检测
bool reverse_detect_start();
void reverse_detect_stop();

void reverse_add_status_cb(reverse_status_cb cb);
void reverse_remove_status_cb(reverse_status_cb cb);

// 是否在倒车状态
bool reverse_does_enter_status();

}

#endif /* _SYSTEM_REVERSE_H_ */
