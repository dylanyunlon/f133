#pragma once
#include "uart/ProtocolSender.h"
/*
*此文件由GUI工具生成
*文件功能：用于处理用户的逻辑相应代码
*功能说明：
*========================onButtonClick_XXXX
当页面中的按键按下后系统会调用对应的函数，XXX代表GUI工具里面的[标识]名称，
如Button1,当返回值为false的时候系统将不再处理这个按键，返回true的时候系统将会继续处理此按键。比如SYS_BACK.
*========================onSlideWindowItemClick_XXXX(int index) 
当页面中存在滑动窗口并且用户点击了滑动窗口的图标后系统会调用此函数,XXX代表GUI工具里面的[标识]名称，
如slideWindow1;index 代表按下图标的偏移值
*========================onSeekBarChange_XXXX(int progress) 
当页面中存在滑动条并且用户改变了进度后系统会调用此函数,XXX代表GUI工具里面的[标识]名称，
如SeekBar1;progress 代表当前的进度值
*========================ogetListItemCount_XXXX() 
当页面中存在滑动列表的时候，更新的时候系统会调用此接口获取列表的总数目,XXX代表GUI工具里面的[标识]名称，
如List1;返回值为当前列表的总条数
*========================oobtainListItemData_XXXX(ZKListView::ZKListItem *pListItem, int index)
 当页面中存在滑动列表的时候，更新的时候系统会调用此接口获取列表当前条目下的内容信息,XXX代表GUI工具里面的[标识]名称，
如List1;pListItem 是贴图中的单条目对象，index是列表总目的偏移量。具体见函数说明
*========================常用接口===============
*LOGD(...)  打印调试信息的接口
*mTextXXX->setText("****") 在控件TextXXX上显示文字****
*mButton1->setSelected(true); 将控件mButton1设置为选中模式，图片会切换成选中图片，按钮文字会切换为选中后的颜色
*mSeekBar->setProgress(12) 在控件mSeekBar上将进度调整到12
*mListView1->refreshListView() 让mListView1 重新刷新，当列表数据变化后调用
*mDashbroadView1->setTargetAngle(120) 在控件mDashbroadView1上指针显示角度调整到120度

*/

#include "app/Activity.h"

#include "system/reverse.h"
#include "utils/TimeHelper.h"
#include "common.h"
#include "link/context.h"
#include "net/NetManager.h"
#include "utils/TimeHelper.h"
#include "manager/LanguageManager.h"
#include "os/MountMonitor.h"
#include "bt/context.h"
#include "media/audio_context.h"
#include "system/fm_emit.h"
#include "system/setting.h"

#define WIFIMANAGER			NETMANAGER->getWifiManager()
extern void screenOn_event();

#define DEBUG		//LOGD("--%d-- --%s-- DEBUG! \n", __LINE__, __FILE__);

#define CALL_TIMER		9

static long long call_time = 0;
static bt_cb_t _s_bt_cb;

static void set_bt_call_info() {
	mnumberTextViewPtr->setText(bt::get_call_num());
	mnameTextViewButtonPtr->setText(bt::get_call_contact());
}

static void set_status_layout(bt_call_state_e state) {
	LayoutPosition position = EASYUICONTEXT->getStatusBar()->getPosition();
	if (state == E_BT_CALL_STATE_INCOMING) {
		set_bt_call_info();
		position = LayoutPosition(314, 3, 396, 60);
		EASYUICONTEXT->getStatusBar()->setPosition(position);
		mincomingWindowPtr->showWnd();
	}
	else if (state == E_BT_CALL_STATE_OUTGOING) {
		set_bt_call_info();
		position = LayoutPosition(314, 3, 396-60, 60);
		EASYUICONTEXT->getStatusBar()->setPosition(position);
		mincomingWindowPtr->showWnd();
	}
	else if (state == E_BT_CALL_STATE_TALKING) {
		position = LayoutPosition(314+158, 0, 80, 40);
		EASYUICONTEXT->getStatusBar()->setPosition(position);
		mincomingWindowPtr->hideWnd();
	}
}

static void _bt_call_cb(bt_call_state_e state) {
	LOGD("--%d-- --%s-- state:%d \n", __LINE__, __FILE__, state);
	switch(state) {
	case E_BT_CALL_STATE_IDLE:					// 挂断
		EASYUICONTEXT->hideStatusBar();
		EASYUICONTEXT->closeActivity("callingActivity");
		EASYUICONTEXT->closeActivity("btIncomingActivity");
		break;
	case E_BT_CALL_STATE_OUTGOING:               // 去电
	  LOGD("--%d-- --%s-- 蓝牙去电! \n", __LINE__, __FILE__);
	  screenOn_event();
		set_status_layout(E_BT_CALL_STATE_OUTGOING);
		if (sys::reverse_does_enter_status()) {
			EASYUICONTEXT->showStatusBar();
		} else {
			EASYUICONTEXT->openActivity("callingActivity");
		}
		break;
	case E_BT_CALL_STATE_INCOMING:               // 来电
//		audio::handle_phone(E_AUDIO_TYPE_BT_PHONE, true);
		LOGD("--%d-- --%s-- 蓝牙来电! \n", __LINE__, __FILE__);
		screenOn_event();
		EASYUICONTEXT->closeActivity("btSettingActivity");
		set_status_layout(E_BT_CALL_STATE_INCOMING);
		if (sys::reverse_does_enter_status()) {

			EASYUICONTEXT->showStatusBar();
		} else {
			EASYUICONTEXT->openActivity("btIncomingActivity");
		}
		break;
	case E_BT_CALL_STATE_TALKING:                // 通话中
		EASYUICONTEXT->closeActivity("btIncomingActivity");
		set_status_layout(E_BT_CALL_STATE_TALKING);
		if (sys::reverse_does_enter_status()) {
			EASYUICONTEXT->showStatusBar();
		} else {
			EASYUICONTEXT->openActivity("callingActivity");
		}
		break;
	}
}

static void _bt_power_cb(bt_power_state_e state) {
	switch (state) {
	case E_BT_POWER_STATE_OFF:
		EASYUICONTEXT->hideStatusBar();  /**********/
		EASYUICONTEXT->closeActivity("callingActivity");
		EASYUICONTEXT->closeActivity("btIncomingActivity");
		EASYUICONTEXT->closeActivity("btDialActivity");
		EASYUICONTEXT->closeActivity("btRecordsActivity");
		EASYUICONTEXT->closeActivity("btContactsActivity");
//		set_status_btn_visble_pos(mbtStatusButtonPtr, false);
		break;
	case E_BT_POWER_STATE_CHANGING:
		break;
	case E_BT_POWER_STATE_ON:
//		set_status_btn_visble_pos(mbtStatusButtonPtr, true);
		break;
	default:
		break;
	}
}

static void _bt_connect_cb(bt_connect_state_e state) {
	switch (state) {
	case E_BT_CONNECT_STATE_IDLE:                // 未连接
		EASYUICONTEXT->hideStatusBar();		/**********/
		EASYUICONTEXT->closeActivity("callingActivity");
		EASYUICONTEXT->closeActivity("btIncomingActivity");
		EASYUICONTEXT->closeActivity("btDialActivity");
		EASYUICONTEXT->closeActivity("btRecordsActivity");
		EASYUICONTEXT->closeActivity("btContactsActivity");
//		mbtStatusButtonPtr->setSelected(false);
		break;
	case E_BT_CONNECT_STATE_CONNECTING:          // 连接中
		break;
	case E_BT_CONNECT_STATE_CONNECTED:           // 已连接
//		mbtStatusButtonPtr->setSelected(true);
		break;
	default:
		break;
	}
}

static void _bt_add_cb() {
	_s_bt_cb.call_cb = _bt_call_cb;
	_s_bt_cb.power_cb = _bt_power_cb;
	_s_bt_cb.connect_cb = _bt_connect_cb;
	bt::add_cb(&_s_bt_cb);
}

static void _bt_remove_cb() {
	bt::remove_cb(&_s_bt_cb);
}

/**
 * 注册定时器
 * 填充数组用于注册定时器
 * 注意：id不能重复
 */
static S_ACTIVITY_TIMEER REGISTER_ACTIVITY_TIMER_TAB[] = {
	{CALL_TIMER,  1000},
};

/**
 * 当界面构造时触发
 */
static void onUI_init(){
    //Tips :添加 UI初始化的显示代码到这里,如:mText1->setText("123");
	_bt_add_cb();
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
static bool onUI_Timer(int id){
	switch (id) {
	case CALL_TIMER: {
		call_time = bt::get_talk_time() / 1000;
		if (call_time < 0 || call_time >= 86400) {
			call_time = 0;
		}
		char timeBuf[256] = {0};
		if (call_time > 3600 - 1) {
			sprintf(timeBuf, "%lld:%02lld:%02lld", call_time/3600, call_time/60, call_time%60);
			//LOGD("timeBuf=%s\n",timeBuf);
			mtimingTextViewButtonPtr->setText(timeBuf);
		}
		else {
			sprintf(timeBuf, "%02lld:%02lld", (call_time/60), (call_time%60));
			mtimingTextViewButtonPtr->setText(timeBuf);
		}
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
static bool onstatusbarActivityTouchEvent(const MotionEvent &ev) {
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


static bool onButtonClick_rejectButton(ZKButton *pButton) {
    LOGD(" ButtonClick rejectButton !!!\n");
    bt::reject();
    return false;
}

static bool onButtonClick_answerButton(ZKButton *pButton) {
    LOGD(" ButtonClick answerButton !!!\n");
    bt::answer();
    return false;
}
static bool onButtonClick_timingTextViewButton(ZKButton *pButton) {
    LOGD(" ButtonClick timingTextViewButton !!!\n");
    bt::query_state();
    return false;
}

static bool onButtonClick_nameTextViewButton(ZKButton *pButton) {
    LOGD(" ButtonClick nameTextViewButton !!!\n");
    bt::query_state();
    return false;
}
