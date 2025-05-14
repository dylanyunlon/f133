/*
 * usb_monitor.h
 *
 *  Created on: 2023年4月19日
 *      Author: guoxs
 */

#ifndef _SYSTEM_USB_MONITOR_H_
#define _SYSTEM_USB_MONITOR_H_

typedef enum {
	E_USB_MODE_NULL,        // 无usb状态
	E_USB_MODE_DEVICE,      // usb从设备，即adb模式
	E_USB_MODE_HOST,        // usb主设备，即u盘模式
} usb_mode_e;

namespace sys {

void change_usb_mode(usb_mode_e mode);
usb_mode_e get_usb_mode();
void set_usb_config(usb_mode_e mode);

}

#endif /* _SYSTEM_USB_MONITOR_H_ */
