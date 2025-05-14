#pragma once
#include "uart/ProtocolSender.h"

#include "app/SysAppFactory.h"
#include "sysapp_context.h"
#include "config.h"
#include "system/setting.h"
#include "utils/TimeHelper.h"
#include "sysapp_context.h"
#include "mode_observer.h"
#include "media/audio_context.h"
#include "bt/context.h"

REGISTER_SYSAPP(APP_TYPE_SYS_TOPBAR, topbarActivity);
static bt_cb_t _s_bt_cb;


static void sys_time_(tm t ) {
	char buf[128] = {0};
	int hour = t.tm_hour;
	if (!sys::setting::is_time_format_24h()) {
		mtimeSystemTextViewPtr->setTextTr((hour >= 12) ? "PM" : "AM");
		hour = (hour > 12) ? (hour % 12) : hour;
	} else {
		mtimeSystemTextViewPtr->setText("");
	}

	sprintf(buf, "%02d:%02d", hour, t.tm_min);
	mtimeTextViewPtr->setText(buf);
}

static void _bt_connect_cb(bt_connect_state_e state) {
	switch (state) {
	case E_BT_CONNECT_STATE_IDLE:                // 未连接
		mbtButtonPtr->setSelected(true);
		break;
	case E_BT_CONNECT_STATE_CONNECTING:          // 连接中
		break;
	case E_BT_CONNECT_STATE_CONNECTED:           // 已连接
		mbtButtonPtr->setSelected(false);
		break;
	default:
		break;
	}
}

static void _bt_add_cb() {
	_s_bt_cb.connect_cb = _bt_connect_cb;
	bt::add_cb(&_s_bt_cb);
}

static void _bt_remove_cb() {
	bt::remove_cb(&_s_bt_cb);
}

static void Icon_Init() {
	mvolumButtonPtr->setSelected(audio::get_system_vol() > 0.1);

	_bt_add_cb();
}
/**
 * 注册定时器
 * 填充数组用于注册定时器
 * 注意：id不能重复
 */
static S_ACTIVITY_TIMEER REGISTER_ACTIVITY_TIMER_TAB[] = {
	{0,  1000}, //定时器id=0, 时间间隔1秒
	//{1,  100},
};

/**
 * 当界面构造时触发
 */
static void onUI_init() {
    //Tips :添加 UI初始化的显示代码到这里,如:mText1->setText("123");
	Icon_Init();
}

/**
 * 当切换到该界面时触发
 */
static void onUI_intent(const Intent *intentPtr) {
    if (intentPtr != NULL) {
        //TODO
    }
}

/*
 * 当界面显示时触发
 */
static void onUI_show() {

}

/*
 * 当界面隐藏时触发
 */
static void onUI_hide() {

}

/*
 * 当界面完全退出时触发
 */
static void onUI_quit() {
	_bt_remove_cb();
}

/**
 * 串口数据回调接口
 */
static void onProtocolDataUpdate(const SProtocolData &data) {
    //串口数据回调接口
}

/**
 * 定时器触发函数
 * 不建议在此函数中写耗时操作，否则将影响UI刷新
 * 参数： id
 *         当前所触发定时器的id，与注册时的id相同
 * 返回值: true
 *             继续运行当前定时器
 *         false
 *             停止运行当前定时器
 */
static bool onUI_Timer(int id) {
	switch (id) {
	case 0: {	//内存监视
		sys_time_(*TimeHelper::getDateTime());
		mvolumTextViewPtr->setText((int)(audio::get_system_vol()*100));
		mvolumButtonPtr->setSelected(audio::get_system_vol() > 0.1);
	}
		break;
		default:
			break;
	}
    return true;
}

/**
 * 有新的触摸事件时触发
 * 参数：ev
 *         新的触摸事件
 * 返回值：true
 *            表示该触摸事件在此被拦截，系统不再将此触摸事件传递到控件上
 *         false
 *            触摸事件将继续传递到控件上
 */
static bool ontopbarActivityTouchEvent(const MotionEvent &ev) {
	LayoutPosition pos = EASYUICONTEXT->getNaviBar()->getPosition();
	if (pos.mTop != -pos.mHeight) {	return false; } 	// 导航栏下滑了
	switch (ev.mActionStatus) {
		case MotionEvent::E_ACTION_DOWN://触摸按下
			//LOGD("时刻 = %ld 坐标  x = %d, y = %d", ev.mEventTime, ev.mX, ev.mY);
			break;
		case MotionEvent::E_ACTION_MOVE://触摸滑动
			break;
		case MotionEvent::E_ACTION_UP:  //触摸抬起
			break;
		default:
			break;
	}
	return false;
}

static bool onButtonClick_btButton(ZKButton *pButton) {
    LOGD(" ButtonClick btButton !!!\n");
    return false;
}
static bool onButtonClick_volumButton(ZKButton *pButton) {
    LOGD(" ButtonClick volumButton !!!\n");
	if (app::is_show_ctrlbar() && (mode::get_event_mode() == E_EVENT_MODE_VOICE)) {
		app::hide_ctrlbar();
	} else {
		mode::set_event_mode(E_EVENT_MODE_VOICE);
		app::show_ctrlbar();
	}
	LOGD("[ctrlbar] volum mode::get_event_mode() = %d", mode::get_event_mode());
    return false;
}

static bool onButtonClick_lightButton(ZKButton *pButton) {
    LOGD(" ButtonClick lightButton !!!\n");
	if (app::is_show_ctrlbar() && (mode::get_event_mode() == E_EVENT_MODE_BRIGHTNESS)) {
		app::hide_ctrlbar();
	} else {
		mode::set_event_mode(E_EVENT_MODE_BRIGHTNESS);
		app::show_ctrlbar();
	}
	LOGD("[ctrlbar] light mode::get_event_mode() = %d", mode::get_event_mode());
    return false;
}

static bool onButtonClick_sys_home(ZKButton *pButton) {
    LOGD(" ButtonClick sys_home !!!\n");
    return false;
}

static bool onButtonClick_sys_back(ZKButton *pButton) {
    LOGD(" ButtonClick sys_back !!!\n");
    return false;
}
