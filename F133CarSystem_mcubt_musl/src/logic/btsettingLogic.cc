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
#include "uart/context.h"
#include "bt/context.h"
#include "link/context.h"
#include "system/setting.h"
#include "utils/Loading_icon.hpp"
#include "media/audio_context.h"

static zkf::IconRotate iconRotate;
//#include "bt/database.h"

#define DEBUG	LOGD("--%d-- --%s-- DEBUG!!!\n", __LINE__, __FILE__);

static bt_cb_t _s_bt_cb;

static bt_dev_t next_connect_bt_info;				// 要切换的连接设备信息

static void _update_dev_name() {
	mbtNameEditTextPtr->setText(bt::get_name());
}

static void _update_pin() {
	mPinEditTextPtr->setText(bt::get_pin());
}

static void _bt_info_init() {
	switch(bt::get_connect_state()){
	case E_BT_CONNECT_STATE_IDLE:
		mconnDevButtonPtr->setTextTr("ununited");
		break;
	case E_BT_CONNECT_STATE_CONNECTING:
		mconnDevButtonPtr->setTextTr("connecting");
		break;
	case E_BT_CONNECT_STATE_CONNECTED:
		mconnDevButtonPtr->setText(bt::get_connect_dev().name);
		break;
	default:
		break;
	}
	mbtNameEditTextPtr->setText(bt::get_name());
	LOGD("--%d-- --%s-- bt::is_on():%d \n", __LINE__, __FILE__, bt::is_on());
	menableButtonPtr->setSelected(bt::is_on());
	mbtEnableWindowPtr->setVisible(bt::is_on());

	LOGD("--%d-- --%s-- connect_state:%d\n", __LINE__, __FILE__, bt::get_connect_state());
	mbtAppWindowPtr->setVisible(bt::get_connect_state() == E_BT_CONNECT_STATE_CONNECTED);

	mautoAnswerButtonPtr->setSelected(bt::is_auto_answer());
	mautoConnButtonPtr->setSelected(bt::is_auto_connect());
}

//关闭蓝牙应用及界面
static void close_bt_appActivity() {
	mdisConnectWindowPtr->hideWnd();
	mbtAppWindowPtr->hideWnd();

	EASYUICONTEXT->closeActivity("callingActivity");
	EASYUICONTEXT->closeActivity("btMusicActivity");
	EASYUICONTEXT->closeActivity("btDialActivity");
	EASYUICONTEXT->closeActivity("btRecordsActivity");
	EASYUICONTEXT->closeActivity("btContactsActivity");

	if (EASYUICONTEXT->isStatusBarShow()) {
		EASYUICONTEXT->hideStatusBar();
	}
}

static void _bt_power_cb(bt_power_state_e state) {
	switch (state) {
	case E_BT_POWER_STATE_OFF:
		close_bt_appActivity();
		mbtEnableWindowPtr->hideWnd();
		mconnDevButtonPtr->setTextTr("ununited");
		mopeningWndPtr->hideWnd();
		iconRotate.requestExit();
		menableButtonPtr->setVisible(true);
		//menableButtonPtr->setInvalid(false);
		menableButtonPtr->setSelected(false);
		break;
	case E_BT_POWER_STATE_ON:
		//menableButtonPtr->setInvalid(false);
		mopeningWndPtr->hideWnd();
		iconRotate.requestExit();
		menableButtonPtr->setVisible(true);
		menableButtonPtr->setSelected(true);
		mbtEnableWindowPtr->showWnd();
		break;
	case E_BT_POWER_STATE_CHANGING:
		mopeningWndPtr->showWnd();
		iconRotate.run();
		menableButtonPtr->setVisible(false);
		//menableButtonPtr->setInvalid(true);
		break;
	}
}

static void _bt_connect_cb(bt_connect_state_e state) {
	switch (state) {
	case E_BT_CONNECT_STATE_IDLE:
		close_bt_appActivity();
		mbtAppWindowPtr->hideWnd();
		mconnDevButtonPtr->setTextTr("ununited");
		if (next_connect_bt_info.addr != "") {
			bt::connect(next_connect_bt_info.addr.c_str());
			next_connect_bt_info.addr = "";
		}
		mscanButtonPtr->setTouchable(true);
		break;
	case E_BT_CONNECT_STATE_CONNECTING:
		mconnDevButtonPtr->setTextTr("connecting");
		mscanButtonPtr->setTouchable(false);
		break;
	case E_BT_CONNECT_STATE_CONNECTED:
		DEBUG
		mbtAppWindowPtr->showWnd();
		mconnDevButtonPtr->setText(bt::get_connect_dev().name);
		mscanButtonPtr->setTouchable(true);
		break;
	}
}

static void _bt_scan_cb(bt_scan_state_e state) {
	switch(state){
	case E_BT_SCAN_STATE_STARTED:
		break;
	case E_BT_SCAN_STATE_COMPLETED:
		break;
	default:
		break;
	}
	mbtListViewPtr->refreshListView();
	mmatchedListViewPtr->refreshListView();
}


static void _bt_misc_info_cb(bt_misc_info_e info) {
	switch (info) {
	case E_BT_MISC_INFO_DEV_NAME:
		_update_dev_name();
		break;
	case E_BT_MISC_INFO_PIN_CODE:
		_update_pin();
		break;
	case E_BT_MISC_INFO_AUTO_ANSWER_ON:   // 自动接听打开
//		mautoAnswerButtonPtr->setInvalid(false);
		mautoAnswerButtonPtr->setSelected(true);
		break;
	case E_BT_MISC_INFO_AUTO_ANSWER_OFF:  // 自动接听关闭
//		mautoAnswerButtonPtr->setInvalid(false);
		mautoAnswerButtonPtr->setSelected(false);
		break;
	case E_BT_MISC_INFO_AUTO_CONNECT_ON:  // 自动连接打开
//		mautoConnButtonPtr->setInvalid(false);
		mautoConnButtonPtr->setSelected(true);
		break;
	case E_BT_MISC_INFO_AUTO_CONNECT_OFF: // 自动连接关闭
//		mautoConnButtonPtr->setInvalid(false);
		mautoConnButtonPtr->setSelected(false);
		break;
	case E_BT_MISC_INFO_AUTH:             // 授权
		break;
	default:
		break;
	}
}

static void _bt_add_cb() {
	_s_bt_cb.power_cb = _bt_power_cb;
	_s_bt_cb.connect_cb = _bt_connect_cb;
	_s_bt_cb.scan_cb = _bt_scan_cb;
	_s_bt_cb.misc_info_cb = _bt_misc_info_cb;
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
	//{0,  6000}, //定时器id=0, 时间间隔6秒
	//{1,  1000},
};

static void _init_bt_info() {
	menableButtonPtr->setSelected(bt::is_on());
	_update_dev_name();
	_update_pin();
}

/**
 * 当界面构造时触发
 *
 */
static void onUI_init() {
    //Tips :添加 UI初始化的显示代码到这里,如:mText1Ptr->setText("123");
//	mdevNameButtonPtr->setLongMode(ZKTextView::E_LONG_MODE_DOTS);
//	bt::create_base_table_phone_book();
	bt::query_mf();
	_bt_add_cb();
	_init_bt_info();
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
	iconRotate.SetCtrl(mopeningPointerPtr, mopeningWndPtr);
	_bt_info_init();
}

/*
 * 当界面隐藏时触发
 */
static void onUI_hide() {
	if (iconRotate.isRunning())
		iconRotate.requestExitAndWait();

}

/*
 * 当界面完全退出时触发
 */
static void onUI_quit() {
//	phone_book_sort.requestExitAndWait();
	if (iconRotate.isRunning())
		iconRotate.requestExitAndWait();
	_bt_remove_cb();
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
static bool onbtsettingActivityTouchEvent(const MotionEvent &ev) {
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

static bool onButtonClick_queryMusicButton(ZKButton *pButton) {
    LOGD(" ButtonClick queryMusicButton !!!\n");
    if (lk::is_connected()) {
    	mbtTipsWindowPtr->showWnd();
    	return false;
    }
//    uart::set_sound_channel(SOUND_CHANNEL_BT);
    audio::change_audio_type(E_AUDIO_TYPE_BT_MUSIC);
    bt::set_bt_mute(false, true);
    EASYUICONTEXT->openActivity("btMusicActivity");
    return false;
}

static bool onButtonClick_phoneButton(ZKButton *pButton) {
    LOGD(" ButtonClick Button5 !!!\n");
    if (lk::is_connected()) {
    	mbtTipsWindowPtr->showWnd();
    	return false;
    }
    EASYUICONTEXT->openActivity("btDialActivity");
    return false;
}

static bool onButtonClick_sys_back(ZKButton *pButton) {
    LOGD(" ButtonClick sys_back !!!\n");
    return false;
}

static int getListItemCount_matchedListView(const ZKListView *pListView) {
    //LOGD("getListItemCount_matchedListView !\n");
//    return bt_match_list.size();
    return bt::get_matched_dev_size();
}
static void obtainListItemData_matchedListView(ZKListView *pListView,ZKListView::ZKListItem *pListItem, int index) {
    //LOGD(" obtainListItemData_ matchedListView  !!!\n");
	bt_dev_t dev;
	bt::get_matched_dev_by_index(index, dev);
	pListItem->setText(dev.name);
//	pListItem->setText(bt_match_list[index].name);
}
static void onListItemClick_matchedListView(ZKListView *pListView, int index, int id) {
    //LOGD(" onListItemClick_ matchedListView  !!!\n");
	bt_dev_t click_dev;
	bt::get_matched_dev_by_index(index, click_dev);

	bt_dev_t conn_dev = bt::get_connect_dev();

	if (click_dev.addr == conn_dev.addr) {
		return;
	}
	if (bt::get_connect_state() == E_BT_CONNECT_STATE_CONNECTED) {
		mconnectedNameTextViewPtr->setText(conn_dev.name);
		mdisConnectWindowPtr->showWnd();
		next_connect_bt_info = click_dev;
	} else {
		bt::connect(click_dev.addr.c_str());
	}
}

static int getListItemCount_btListView(const ZKListView *pListView) {
    //LOGD("getListItemCount_btListView !\n");
	return bt::get_scan_dev_size();
}
static void obtainListItemData_btListView(ZKListView *pListView,ZKListView::ZKListItem *pListItem, int index) {
    //LOGD(" obtainListItemData_ btListView  !!!\n");
	bt_dev_t dev;
	bt::get_scan_dev_by_index(index, dev);
	pListItem->setText(dev.name);
}
static void onListItemClick_btListView(ZKListView *pListView, int index, int id) {
    //LOGD(" onListItemClick_ btListView  !!!\n");
	bt_dev_t click_dev;
	bt::get_scan_dev_by_index(index, click_dev);
	bt_dev_t conn_dev = bt::get_connect_dev();

	if (click_dev.addr == conn_dev.addr) {
		return;
	}
	if (bt::get_connect_state() == E_BT_CONNECT_STATE_CONNECTED) {
		mconnectedNameTextViewPtr->setText(conn_dev.name);
		mdisConnectWindowPtr->showWnd();
		next_connect_bt_info = click_dev;
	} else {
		bt::connect(click_dev.addr.c_str());
	}
}

static bool onButtonClick_autoConnButton(ZKButton *pButton) {
    LOGD(" ButtonClick autoConnButton !!!\n");
    bt::set_auto_connect(!pButton->isSelected());
    bt::query_mf();
    return false;
}

static bool onButtonClick_autoAnswerButton(ZKButton *pButton) {
    LOGD(" ButtonClick autoAnswerButton !!!\n");
    bt::set_auto_answer(!pButton->isSelected());
    bt::query_mf();
    return false;
}

static bool onButtonClick_scanButton(ZKButton *pButton) {
    LOGD(" ButtonClick scanButton !!!\n");

    if (bt::is_scan()) {DEBUG
    	bt::stop_scan();
    }
    else { // 先断开再扫描
//    	bt::disconnect();
    	bt::start_scan();
    }
    return false;
}

static bool onButtonClick_enableButton(ZKButton *pButton) {
    LOGD(" ButtonClick enableButton !!!\n");
    if(bt::is_calling()){
    	return false;
    }
    if (bt::is_on()) {
    	bt::power_off();
    	mbtEnableWindowPtr->hideWnd();		// 处理mcubt关闭比较慢
    } else {
    	bt::power_on();
    }
    return false;
}

static bool onButtonClick_disConnectButton(ZKButton *pButton) {
    LOGD(" ButtonClick disConnectButton !!!\n");
    bt::disconnect();
    return false;
}

static bool onButtonClick_disconCancelButton(ZKButton *pButton) {
    LOGD(" ButtonClick disconCancelButton !!!\n");
    mdisConnectWindowPtr->hideWnd();
    return false;
}

static bool onButtonClick_connDevButton(ZKButton *pButton) {
    LOGD(" ButtonClick connDevButton !!!\n");
    if(bt::get_connect_state() == E_BT_CONNECT_STATE_IDLE || bt::get_connect_state() == E_BT_CONNECT_STATE_CONNECTING){
    	return false;
    }
    mconnectedNameTextViewPtr->setText(mconnDevButtonPtr->getText());
    mdisConnectWindowPtr->showWnd();
    return false;
}

static void onEditTextChanged_btNameEditText(const std::string &text) {
    //LOGD(" onEditTextChanged_ btNameEditText %s !!!\n", text.c_str());
	if (text != bt::get_name()) {
		if (text.length() > 26) {
			sys::setting::set_bt_name(text.substr(0, 26).c_str());
			mbtNameEditTextPtr->setText(text.substr(0, 26));
			mnameLongTipsWindowPtr->showWnd();
		}
		else {
			sys::setting::set_bt_name(text);
		}
		bt::modify_name(sys::setting::get_bt_name().c_str());
	}
}

static void onEditTextChanged_PinEditText(const std::string &text) {
    //LOGD(" onEditTextChanged_ PinEditText %s !!!\n", text.c_str());
	if (text != bt::get_pin()) {
		bt::modify_pin(text.c_str());
	}
}

