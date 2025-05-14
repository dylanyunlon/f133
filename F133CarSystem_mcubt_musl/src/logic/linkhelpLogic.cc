#pragma once
#include "uart/ProtocolSender.h"
/*
*此文件由GUI工具生成
*文件功能：用于处理用户的逻辑相应代码
*功能说明：
*========================onButtonClick_XXXX
当页面中的按键按下后系统会调用对应的函数，XXX代表GUI工具里面的[ID值]名称，
如Button1,当返回值为false的时候系统将不再处理这个按键，返回true的时候系统将会继续处理此按键。比如SYS_BACK.
*========================onSlideWindowItemClick_XXXX(int index) 
当页面中存在滑动窗口并且用户点击了滑动窗口的图标后系统会调用此函数,XXX代表GUI工具里面的[ID值]名称，
如slideWindow1;index 代表按下图标的偏移值
*========================onSeekBarChange_XXXX(int progress) 
当页面中存在滑动条并且用户改变了进度后系统会调用此函数,XXX代表GUI工具里面的[ID值]名称，
如SeekBar1;progress 代表当前的进度值
*========================ogetListItemCount_XXXX() 
当页面中存在滑动列表的时候，更新的时候系统会调用此接口获取列表的总数目,XXX代表GUI工具里面的[ID值]名称，
如List1;返回值为当前列表的总条数
*========================oobtainListItemData_XXXX(ZKListView::ZKListItem *pListItem, int index)
 当页面中存在滑动列表的时候，更新的时候系统会调用此接口获取列表当前条目下的内容信息,XXX代表GUI工具里面的[ID值]名称，
如List1;pListItem 是贴图中的单条目对象，index是列表总目的偏移量。具体见函数说明
*========================常用接口===============
*LOGD(...)  打印调试信息的接口
*mTextXXXPtr->setText("****") 在控件TextXXX上显示文字****
*mButton1Ptr->setSelected(true); 将控件mButton1设置为选中模式，图片会切换成选中图片，按钮文字会切换为选中后的颜色
*mSeekBarPtr->setProgress(12) 在控件mSeekBar上将进度调整到12
*mListView1Ptr->refreshListView() 让mListView1 重新刷新，当列表数据变化后调用
*mDashbroadView1Ptr->setTargetAngle(120) 在控件mDashbroadView1上指针显示角度调整到120度
*
* 在Eclipse编辑器中  使用 “alt + /”  快捷键可以打开智能提示
*/

#include <string>
#include "bt/context.h"
#include "net/ap.h"
#include "net/context.h"
#include "link/context.h"
#include "system/setting.h"
#include "fy/format.hpp"
#include "fy/os.hpp"
#include "common.h"

#define LINK_RESTART_TIMER    1

static void _confirm_link_mode(link_mode_e mode) {
	link_mode_e last_mode = sys::setting::get_link_mode();
	sys::setting::set_link_mode(mode);
	switch (mode) {
	case E_LINK_MODE_CARPLAY:
	case E_LINK_MODE_ANDROIDAUTO:
	case E_LINK_MODE_AIRPLAY:
		if (net::get_mode() == E_NET_MODE_AP) {
			// airplay与 cp aa切换需要重启hostapd
			if ((last_mode == E_LINK_MODE_AIRPLAY) || (mode == E_LINK_MODE_AIRPLAY)) {
				LOGD("[LinkLogic] need to restart hostapd\n");
				net::ap::restart_hostapd();
			}
		} else {
			net::change_mode(E_NET_MODE_AP);
		}
		break;
	case E_LINK_MODE_CARLIFE:
		net::change_mode(E_NET_MODE_WIFI);
		break;
	case E_LINK_MODE_MIRACAST:
	case E_LINK_MODE_LYLINK:
		net::change_mode(E_NET_MODE_P2P);
		break;
	default:
		break;
	}
	LOGD("[linkhelp] will restart link");
	mActivityPtr->registerUserTimer(LINK_RESTART_TIMER, 10);
//	lk::restart_lylink();
}

static void _select_link_mode(link_mode_e mode) {
	LOGD("get choose id = %s", sys::setting::get_link_mode_str(mode));
	net_mode_e set_mode = E_NET_MODE_NULL;
	switch (mode) {
	case E_LINK_MODE_ANDROIDAUTO:
		maaWindowPtr->showWnd();
		set_mode = E_NET_MODE_AP;
		break;
	case E_LINK_MODE_AIRPLAY:
		mapWindowPtr->showWnd();
		set_mode = E_NET_MODE_AP;
		break;
	case E_LINK_MODE_CARLIFE:
		mcfWindowPtr->showWnd();
		set_mode = E_NET_MODE_WIFI;
		break;
	case E_LINK_MODE_MIRACAST:
		mmcWindowPtr->showWnd();
		set_mode = E_NET_MODE_P2P;
		break;
	case E_LINK_MODE_LYLINK:
		macWindowPtr->showWnd();
		set_mode = E_NET_MODE_P2P;
		break;
	case E_LINK_MODE_CARPLAY:
	default:
		mcpWindowPtr->showWnd();
		set_mode = E_NET_MODE_AP;
		break;
	}
	if((sys::setting::get_link_mode() == mode) && (net::get_mode() == set_mode)){
		return;
	}
	_confirm_link_mode(mode);
}

static void _update_layout() {
	std::string bt_name = sys::setting::get_bt_name();
	std::string dev_name = sys::setting::get_dev_name();

	std::string cpStep = LTOV("cp_step1") + "\n"
					   + bt_name + "\n\n"
					   + LTOV("cp_step2");

	std::string aaStep = LTOV("aa_step1") +  "\n\n"
					   + LTOV("aa_step2") + "\n"
					   + bt_name;

	std::string apStep = LTOV("ap_step1") + "\n" + dev_name + "\n\n"
					   + LTOV("ap_step2") + std::string("12345678") + "\n\n"
					   + LTOV("ap_step3") + "\n" + dev_name;

	std::string mcStep = LTOV("mc_step1") +  "\n\n"
					   + LTOV("mc_step2") +  "\n\n"
					   + LTOV("mc_step3") + dev_name;

	std::string acStep = LTOV("ac_step1") +  "\n\n"
					   + LTOV("ac_step2") +  "\n\n"
					   + LTOV("ac_step3") + dev_name;
	mcpTipTextViewPtr->setText(LTOV("cp_help"));
	mcpStepTextViewPtr->setText(cpStep);
	maaTipTextViewPtr->setText(LTOV("aa_help"));
	maaStepTextViewPtr->setText(aaStep);
	mapTipTextViewPtr->setText(LTOV("ap_help"));
	mapStepTextViewPtr->setText(apStep);
	mmcTipTextViewPtr->setText(LTOV("mc_help"));
	mmcStepTextViewPtr->setText(mcStep);
	macTipTextViewPtr->setText(LTOV("ac_help"));
	macStepTextViewPtr->setText(acStep);
}

/**
 * 注册定时器
 * 填充数组用于注册定时器
 * 注意：id不能重复
 */
static S_ACTIVITY_TIMEER REGISTER_ACTIVITY_TIMER_TAB[] = {
	//{0,  6000}, //定时器id=0, 时间间隔6秒
	//{1,  1000},
};

/**
 * 当界面构造时触发
 */
static void onUI_init(){
    //Tips :添加 UI初始化的显示代码到这里,如:mText1Ptr->setText("123");
//	std::string bt_name = sys::setting::get_bt_name();//bt::get_name();
//	std::string tips = std::string("3.找到") + bt_name + std::string(",点击进行配对连接");

//	mcp3TextViewPtr->setText(tips);
//	maa3TextViewPtr->setText(tips);

	_update_layout();
}

/**
 * 当切换到该界面时触发
 */
static void onUI_intent(const Intent *intentPtr) {
    if (intentPtr != NULL) {
        //TODO
    	_select_link_mode((link_mode_e)atoi(intentPtr->getExtra("link_mode").c_str()));
    }
}

/*
 * 当界面显示时触发
 */
static void onUI_show() {
	//mmc3TextViewPtr->setText(fy::format("3.找到lylink_%s,点击进行连接", fy::gen_uuid_str().c_str()));
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
//	lk::remove_lylink_callback(_lylink_callback);
}

/**
 * 串口数据回调接口
 */
static void onProtocolDataUpdate(const SProtocolData &data) {

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
	case LINK_RESTART_TIMER:
		lk::restart_lylink();
		return false;
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
static bool onlinkhelpActivityTouchEvent(const MotionEvent &ev) {
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

static bool onButtonClick_sys_back(ZKButton *pButton) {
    LOGD(" ButtonClick sys_back !!!\n");
    return false;
}
