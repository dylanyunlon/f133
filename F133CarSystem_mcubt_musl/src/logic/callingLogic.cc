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

#include "bt/context.h"
#include "utils/TimeHelper.h"
#include "utils/log.h"
#include "link/context.h"
#include "system/reverse.h"

#define CALL_TIMER		9

static long long call_time = 0;
static std::string redial_munber_str;

static bt_cb_t _s_bt_cb;

namespace {
class LongClickListener : public ZKBase::ILongClickListener {
public:
	virtual void onLongClick(ZKBase *pBase) {
		redial_munber_str = "";
		mnumTextViewPtr->setText(redial_munber_str);
	}
};
}
static LongClickListener delLongClickListener;


static void put_number_str(const std::string input) {
	if (input == "del") {
		if (redial_munber_str.length() == 0) {
			return;
		}
		redial_munber_str.erase(redial_munber_str.end() - 1);
	}
	else {
		redial_munber_str += input;
	}
	mnumTextViewPtr->setText(redial_munber_str);
}

static void _bt_misc_info_cb(bt_misc_info_e info) {
	switch(info) {
	case E_BT_MISC_INFO_CAR_SOUND:        // 车机端出声音
		msoundButtonPtr->setSelected(true);
		break;
	case E_BT_MISC_INFO_PHONE_SOUND:      // 手机端出声音
		msoundButtonPtr->setSelected(false);
		break;
	case E_BT_MISC_INFO_MIC_MUTE:         // mic静音
		LOGD("mic静音");
		break;
	case E_BT_MISC_INFO_MIC_UNMUTE:       // mic解除静音
		LOGD("mic解除静音");
		break;
	default:
		break;
	}
}

static void _switch_app_window(bool show) {
	if (show) {
		mInfoWndPtr->setPosition(LayoutPosition(621, 112, 381, 424));
	} else {
		mInfoWndPtr->setPosition(LayoutPosition(414, 112, 381, 424));
	}
}

static void _bt_call_cb(bt_call_state_e state) {
//	if (state == E_BT_CALL_STATE_OUTGOING) {        // 去电
//		std::string name = bt::get_call_contact();
//		std::string num = bt::get_call_num();
//	    mnameTextViewPtr->setText(name == "" ? num : name);
////	    LOGD("--%d-- --%s-- call_name:%s  call_num:%s \n", __LINE__, __FILE__, name.c_str(), num.c_str());
//	}
	std::string name = bt::get_call_contact();
	std::string num = bt::get_call_num();
	switch(state) {
	case E_BT_CALL_STATE_IDLE:					// 挂断
		EASYUICONTEXT->hideStatusBar();
		EASYUICONTEXT->closeActivity("callingActivity");
		break;
	case E_BT_CALL_STATE_OUTGOING:               // 去电
	    mnameTextViewPtr->setText(name == "" ? num : name);
//	    mnumberTextViewPtr->setVisible(false);
		moutWindowPtr->showWnd();
		minWindowPtr->hideWnd();
		break;
	case E_BT_CALL_STATE_INCOMING:               // 来电
		mnameTextViewPtr->setText(name == "" ? num : name);
//		mnumberTextViewPtr->setVisible(false);
		moutWindowPtr->hideWnd();
		minWindowPtr->showWnd();
		break;
	case E_BT_CALL_STATE_TALKING:                // 通话中
		moutWindowPtr->hideWnd();
		minWindowPtr->hideWnd();
		mcommunicateWindowPtr->setVisible(state == E_BT_CALL_STATE_TALKING);
		mcallTimeTextViewPtr->setVisible(state == E_BT_CALL_STATE_TALKING);
		break;
	}
}

static void _bt_add_cb() {
	_s_bt_cb.misc_info_cb = _bt_misc_info_cb;
	_s_bt_cb.call_cb = _bt_call_cb;
	bt::add_cb(&_s_bt_cb);
}

static void _bt_remove_cb() {
	bt::remove_cb(&_s_bt_cb);
}

static void set_call_time() {
	call_time = bt::get_talk_time() / 1000;
	LOGD("bt::get_talk_time():%lld,   call_time:%lld\n", bt::get_talk_time(), call_time);
	if (call_time < 0 || call_time >= 86400) {
		call_time = 0;
		return;
	}
	char timeBuf[256] = {0};
	if (call_time > 3600 - 1) {
		sprintf(timeBuf, "%lld:%02lld:%02lld", call_time/3600, call_time/60, call_time%60);
		mcallTimeTextViewPtr->setText(timeBuf);
	}
	else {
		sprintf(timeBuf, "%02lld:%02lld", (call_time/60), (call_time%60));
		mcallTimeTextViewPtr->setText(timeBuf);
	}
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
	mActivityPtr->registerUserTimer(CALL_TIMER, 1000);

    msoundButtonPtr->setSelected(bt::is_car_sound());
    _bt_add_cb();

	std::string name = bt::get_call_contact();
	std::string num = bt::get_call_num();
    mnameTextViewPtr->setText(name == "" ? num : name);
//    LOGD("--%d-- --%s-- call_name:%s  call_num:%s \n", __LINE__, __FILE__, name.c_str(), num.c_str());

    bt_call_state_e call_state = bt::get_call_state();
    mcommunicateWindowPtr->setVisible(call_state == E_BT_CALL_STATE_TALKING);
	mcallTimeTextViewPtr->setVisible(call_state == E_BT_CALL_STATE_TALKING);

	redial_munber_str = "";
	mdelButtonPtr->setLongClickListener(&delLongClickListener);
	set_call_time();
	_switch_app_window(false);
}

/**
 * 当切换到该界面时触发
 */
static void onUI_intent(const Intent *intentPtr) {
    if (intentPtr != NULL) {

    }
}

/*
 * 当界面显示时触发
 */
static void onUI_show() {
	EASYUICONTEXT->hideStatusBar();
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
	mdelButtonPtr->setLongClickListener(NULL);
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
	case CALL_TIMER:
		set_call_time();
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
static bool oncallingActivityTouchEvent(const MotionEvent &ev) {
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

static bool onButtonClick_stopMicButton(ZKButton *pButton) {
    LOGD(" ButtonClick stopMicButton !!!\n");
    pButton->setSelected(!pButton->isSelected());
    bt::set_mic_mute(pButton->isSelected());
//    bt::set_mic_switch();
    return false;
}

static bool onButtonClick_soundButton(ZKButton *pButton) {
    LOGD(" ButtonClick soundButton !!!\n");
    bt::is_car_sound() ? bt::sound_to_phone() : bt::sound_to_car();
    return false;
}

static bool onButtonClick_hangupButton(ZKButton *pButton) {
    LOGD(" ButtonClick hangupButton !!!\n");
    bt::hangup();
    return false;
}

static bool onButtonClick_keyBoardButton(ZKButton *pButton) {
    LOGD(" ButtonClick keyBoardButton !!!\n");
    pButton->setSelected(!pButton->isSelected());
    mkeyBoardWindowPtr->setVisible(pButton->isSelected());
    _switch_app_window(pButton->isSelected());
    return false;
}

static bool onButtonClick_key1Button(ZKButton *pButton) {
    LOGD(" ButtonClick key1Button !!!\n");
    put_number_str("1");
    bt::redial("1");
    return false;
}

static bool onButtonClick_key2Button(ZKButton *pButton) {
    LOGD(" ButtonClick key2Button !!!\n");
    put_number_str("2");
    bt::redial("2");
    return false;
}

static bool onButtonClick_key3Button(ZKButton *pButton) {
    LOGD(" ButtonClick key3Button !!!\n");
    put_number_str("3");
    bt::redial("3");
    return false;
}

static bool onButtonClick_key4Button(ZKButton *pButton) {
    LOGD(" ButtonClick key4Button !!!\n");
    put_number_str("4");
    bt::redial("4");
    return false;
}

static bool onButtonClick_key5Button(ZKButton *pButton) {
    LOGD(" ButtonClick key5Button !!!\n");
    put_number_str("5");
    bt::redial("5");
    return false;
}

static bool onButtonClick_key6Button(ZKButton *pButton) {
    LOGD(" ButtonClick key6Button !!!\n");
    put_number_str("6");
    bt::redial("6");
    return false;
}

static bool onButtonClick_key7Button(ZKButton *pButton) {
    LOGD(" ButtonClick key7Button !!!\n");
    put_number_str("7");
    bt::redial("7");
    return false;
}

static bool onButtonClick_key8Button(ZKButton *pButton) {
    LOGD(" ButtonClick key8Button !!!\n");
    put_number_str("8");
    bt::redial("8");
    return false;
}

static bool onButtonClick_key9Button(ZKButton *pButton) {
    LOGD(" ButtonClick key9Button !!!\n");
    put_number_str("9");
    bt::redial("9");
    return false;
}

static bool onButtonClick_key0Button(ZKButton *pButton) {
    LOGD(" ButtonClick key0Button !!!\n");
    put_number_str("0");
    bt::redial("0");
    return false;
}

static bool onButtonClick_keyJINGButton(ZKButton *pButton) {
    LOGD(" ButtonClick keyJINGButton !!!\n");
    put_number_str("*");
    bt::redial("*");
    return false;
}

static bool onButtonClick_keyXINGButton(ZKButton *pButton) {
    LOGD(" ButtonClick keyXINGButton !!!\n");
    put_number_str("#");
    bt::redial("#");
    return false;
}

static bool onButtonClick_delButton(ZKButton *pButton) {
    LOGD(" ButtonClick delButton !!!\n");
    put_number_str("del");
    return false;
}


static bool onButtonClick_inhangupButton(ZKButton *pButton) {
    LOGD(" ButtonClick inhangupButton !!!\n");
    bt::hangup();
    return false;
}

static bool onButtonClick_inanswerButton(ZKButton *pButton) {
    LOGD(" ButtonClick inanswerButton !!!\n");
    bt::answer();
    return false;
}

static bool onButtonClick_outhangupButton(ZKButton *pButton) {
    LOGD(" ButtonClick outhangupButton !!!\n");
    bt::hangup();
    return false;
}

static bool onButtonClick_queryMusicButton(ZKButton *pButton) {
    LOGD(" ButtonClick queryMusicButton !!!\n");
    EASYUICONTEXT->openActivity("btMusicActivity");
    EASYUICONTEXT->closeActivity("btRecordsActivity");
    EASYUICONTEXT->closeActivity("btContactsActivity");
    EASYUICONTEXT->closeActivity("btDialActivity");
    return false;
}

static bool onButtonClick_phoneButton(ZKButton *pButton) {
    LOGD(" ButtonClick phoneButton !!!\n");
    EASYUICONTEXT->closeActivity("btContactsActivity");
    EASYUICONTEXT->closeActivity("btRecordslActivity");
    EASYUICONTEXT->closeActivity("btMusicActivity");
    return false;
}

static bool onButtonClick_btrecordButton(ZKButton *pButton) {
    LOGD(" ButtonClick btrecordButton !!!\n");
    EASYUICONTEXT->closeActivity("btDialActivity");
    EASYUICONTEXT->openActivity("btRecordsActivity");
    EASYUICONTEXT->closeActivity("btContactsActivity");
    EASYUICONTEXT->closeActivity("btMusicActivity");
    return false;
}

static bool onButtonClick_btcontactsButton(ZKButton *pButton) {
    LOGD(" ButtonClick btcontactsButton !!!\n");
    EASYUICONTEXT->closeActivity("btDialActivity");
    EASYUICONTEXT->closeActivity("btRecordsActivity");
    EASYUICONTEXT->openActivity("btContactsActivity");
    EASYUICONTEXT->closeActivity("btMusicActivity");
    return false;
}

static bool onButtonClick_btsettingButton(ZKButton *pButton) {
    LOGD(" ButtonClick btsettingButton !!!\n");
    EASYUICONTEXT->openActivity("btsettingActivity");
    EASYUICONTEXT->closeActivity("btRecordsActivity");
    EASYUICONTEXT->closeActivity("btContactsActivity");
    EASYUICONTEXT->closeActivity("btDialActivity");
    EASYUICONTEXT->closeActivity("btMusicActivity");
    return false;
}
