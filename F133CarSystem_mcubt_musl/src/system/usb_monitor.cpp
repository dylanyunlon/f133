/*
 * usb_monitor.cpp
 *
 *  Created on: 2023年4月19日
 *      Author: ZKSWE Develop Team
 */

#include <sys/mount.h>
#include "common.h"
#include "usb_monitor.h"
#include "fy/os.hpp"

#define USB_DEV_ROOT        "/sys/devices/platform/soc@3000000/soc@3000000:usbc0@0/"
#define USB_OTG_ROLE        USB_DEV_ROOT "otg_role"
#define USB_DEVICE          USB_DEV_ROOT "usb_device"
#define USB_HOST            USB_DEV_ROOT "usb_host"
#define USB_NULL            USB_DEV_ROOT "usb_null"

#define KERNEL_CONFIG       "/sys/kernel/config/"
#define USB_GADGET          KERNEL_CONFIG "usb_gadget/"
#define USB_GADGET_G1       USB_GADGET "g1/"
#define USB_GADGET_G1_SUB   USB_GADGET_G1 "strings/0x409/"
#define USB_GADGET_G1_C1    USB_GADGET_G1 "configs/c.1/"
#define USB_GADGET_FUN_FFS  USB_GADGET_G1 "functions/ffs.adb"
#define USB_GADGET_C1_FFS   USB_GADGET_G1_C1 "ffs.adb"
#define USB_FFS_ADB         "/dev/usb-ffs/adb"

namespace sys {

static void _read_content(const char *file, char *buf, int len) {
	FILE *f = fopen(file, "r");
	if (!f) {
		LOGE("read %s error, %s\n", file, strerror(errno));
		return;
	}

	memset(buf, 0, len);
	fgets(buf, len, f);
	fclose(f);

	len = strlen(buf);
	if ((len > 0) && (buf[len - 1] == '\n')) {
		buf[len - 1] = '\0';
	}
}

static int _write_content(const char *file, const char *content) {
	FILE *f = fopen(file, "w");
	if (!f) {
		LOGE("write %s error, %s\n", file, strerror(errno));
		return -1;
	}

	int ret = fwrite(content, 1, strlen(content), f);
	fclose(f);

	return ret;
}

void change_usb_mode(usb_mode_e mode) {
	char content[32];
	_read_content(USB_NULL, content, sizeof(content));

	if (mode == E_USB_MODE_DEVICE) {
		LOGD("change usb to device mode\n");
		_read_content(USB_DEVICE, content, sizeof(content));
		set_usb_config(mode);
		RESTART_SERVICE("adbd");
	} else if (mode == E_USB_MODE_HOST) {
		LOGD("change usb to host mode\n");
		_read_content(USB_HOST, content, sizeof(content));
		set_usb_config(mode);
	}
}

usb_mode_e get_usb_mode() {
	char content[32];
	_read_content(USB_OTG_ROLE, content, sizeof(content));

	if (strcmp(content, "usb_device") == 0) {
		return E_USB_MODE_DEVICE;
	}
	if (strcmp(content, "usb_host") == 0) {
		return E_USB_MODE_HOST;
	}

	return E_USB_MODE_NULL;
}

void set_usb_config(usb_mode_e mode) {
	if (!FILE_EXIST(USB_GADGET)) {
		mount("none", KERNEL_CONFIG, "configfs", MS_SILENT, NULL);
		fy::path::mkdir(USB_GADGET_G1_SUB);
		_write_content(USB_GADGET_G1_SUB "manufacturer", "zkswe");
		_write_content(USB_GADGET_G1_SUB "product", "flythings");
		_write_content(USB_GADGET_G1_SUB "serialnumber", "20080411");
	}

	if (!FILE_EXIST(USB_GADGET_G1_C1)) {
		fy::path::mkdir(USB_GADGET_G1_C1 "strings/0x409");
		_write_content(USB_GADGET_G1_C1 "bmAttributes", "0xc0");
		_write_content(USB_GADGET_G1_C1 "MaxPower", "500");
	}

	if (!FILE_EXIST(USB_GADGET_FUN_FFS)) {
		fy::path::mkdir(USB_GADGET_FUN_FFS);
	}

	unlink(USB_GADGET_C1_FFS);
	unlink(USB_GADGET_G1_C1 "f1");

	bool is_adb = (mode == E_USB_MODE_DEVICE);

	_write_content(USB_GADGET_G1 "idVendor", is_adb ? "0x18D1" : "0x1F3A");
	_write_content(USB_GADGET_G1 "idProduct", is_adb ? "0xD002" : "0x1001");

	if (!is_adb) {
		return;
	}

	symlink(USB_GADGET_FUN_FFS, USB_GADGET_C1_FFS);

	if (!FILE_EXIST(USB_FFS_ADB)) {
		fy::path::mkdir(USB_FFS_ADB);
		mount("adb", USB_FFS_ADB, "functionfs", MS_SILENT, "uid=2000,gid=2000");
	}

	std::vector<std::string> udc = fy::path::ls("/sys/class/udc");
	if (!udc.empty()) {
		_write_content(USB_GADGET_G1 "UDC", udc[0].c_str());
	}
}

}
