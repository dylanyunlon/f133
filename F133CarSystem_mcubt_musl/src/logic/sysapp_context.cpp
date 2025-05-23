/*
 * sysapp_context.cpp
 *
 *  Created on: 2023年5月26日
 *      Author: ZKSWE Develop Team
 */

#include "app/SysAppFactory.h"
#include "sysapp_context.h"
#include "common.h"

namespace app {

#define SHOW_CTRLBAR_TIMER     10000
#define HIDE_CTRLBAR_TIMER     10001
#define SHOW_TOPBAR_TIMER    10002
#define HIDE_TOPBAR_TIMER    10003

static app_register_timer_fun _s_reg_timer_fun;
static app_unregister_timer_fun _s_unreg_timer_fun;

static BaseApp *_s_ctrlbar;
static BaseApp *_s_topbar;

static void _reg_timer(int id, int time) {
	if (_s_reg_timer_fun) {
		_s_reg_timer_fun(id, time);
	}
}

void attach_timer(app_register_timer_fun reg, app_unregister_timer_fun unreg) {
	_s_reg_timer_fun = reg;
	_s_unreg_timer_fun = unreg;
}

bool on_timer(int id) {
	switch (id) {
	case SHOW_CTRLBAR_TIMER:
		if (!_s_ctrlbar) {
			_s_ctrlbar = SYSAPPFACTORY->create(APP_TYPE_SYS_CTRLBAR);
		}
		return true;

	case HIDE_CTRLBAR_TIMER:
		if (_s_ctrlbar) {
			delete _s_ctrlbar;
			_s_ctrlbar = NULL;
		}
		return true;

	case SHOW_TOPBAR_TIMER:
		if (!_s_topbar) {
			_s_topbar = SYSAPPFACTORY->create(APP_TYPE_SYS_TOPBAR);
		}
		return true;

	case HIDE_TOPBAR_TIMER:
		if (_s_topbar) {
			delete _s_topbar;
			_s_topbar = NULL;
		}
		return true;
	}

	return false;
}

void show_ctrlbar() {
	_reg_timer(SHOW_CTRLBAR_TIMER, 0);
}

void hide_ctrlbar() {
	_reg_timer(HIDE_CTRLBAR_TIMER, 0);
}

bool is_show_ctrlbar() {
	return _s_ctrlbar ? _s_ctrlbar->isShow() : false;
}

bool is_hit_ctrlbar(int x, int y) {
	return _s_ctrlbar ? _s_ctrlbar->getPosition().isHit(x, y) : false;
}

void show_topbar() {
	_reg_timer(SHOW_TOPBAR_TIMER, 0);
}

void hide_topbar() {
	_reg_timer(HIDE_TOPBAR_TIMER, 0);
}

bool is_show_topbar() {
	return _s_topbar ? _s_topbar->isShow() : false;
}

bool is_hit_topbar(int x, int y) {
	return _s_topbar ? _s_topbar->getPosition().isHit(x, y) : false;
}

LayoutPosition get_topbar_position() {
	return _s_topbar ? _s_topbar->getPosition() : LayoutPosition(0, 0, 0 ,0);
}
}
