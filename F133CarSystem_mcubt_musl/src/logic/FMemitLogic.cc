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

#include "system/fm_emit.h"
#include "media/audio_player.h"
#include "system/setting.h"
#include "uart/context.h"
#include "media/audio_context.h"
#include "mode_observer.h"
#include "link/context.h"

#define LINK_RESTART_TIMER 1

static void set_EditTextFmFreq_fmVal(int progress) {
	char buf[8] = {0};
	sprintf(buf, "%.1f", (875 + progress) / 10.f);
	mEditTextFmFreqPtr->setText(buf);
}

static bool _probe_link_connected() {
	LYLINK_TYPE_E link_type = lk::get_lylink_type();
	if ((link_type == LINK_TYPE_WIFICP) || (link_type == LINK_TYPE_WIFIAUTO)) {
		char tip[256];
		sprintf(tip, "%s%s,%s",
				LANGUAGEMANAGER->getValue("connected").c_str(),
				(link_type == LINK_TYPE_WIFICP) ? "CarPlay" : "Android Auto",
				LANGUAGEMANAGER->getValue("confirm_switch_link_screen_mode").c_str());
		mLinkSwitchTipTextViewPtr->setText(tip);
		mLinkTipWindowPtr->showWnd();
		return true;
	}

	return false;
}

namespace {

static void _sound_mode_cb(sound_mode_e mode) {
	switch (mode) {
	case E_SOUND_MODE_SPK:
		mARMButtonPtr->setSelected(true);
		mARMWindowPtr->showWnd();
		mBTButtonPtr->setSelected(false);
		mAUXButtonPtr->setSelected(false);
		mButtonFMPtr->setSelected(false);
		mLinkWindowPtr->hideWnd();
		mFMWindowPtr->hideWnd();
		mAUXWindowPtr->hideWnd();
		break;

	case E_SOUND_MODE_LINK:
		mBTButtonPtr->setSelected(true);
		mLinkWindowPtr->showWnd();
		mARMButtonPtr->setSelected(false);
		mAUXButtonPtr->setSelected(false);
		mButtonFMPtr->setSelected(false);
		mARMWindowPtr->hideWnd();
		mFMWindowPtr->hideWnd();
		mAUXWindowPtr->hideWnd();
		break;

	case E_SOUND_MODE_FM:
		mButtonFMPtr->setSelected(true);
		mFMWindowPtr->showWnd();
		mBTButtonPtr->setSelected(false);
		mARMButtonPtr->setSelected(false);
		mAUXButtonPtr->setSelected(false);
		mARMWindowPtr->hideWnd();
		mLinkWindowPtr->hideWnd();
		mAUXWindowPtr->hideWnd();
		break;

	case E_SOUND_MODE_AUX:
		mAUXButtonPtr->setSelected(true);
		mAUXWindowPtr->showWnd();
		mBTButtonPtr->setSelected(false);
		mARMButtonPtr->setSelected(false);
		mButtonFMPtr->setSelected(false);
		mARMWindowPtr->hideWnd();
		mLinkWindowPtr->hideWnd();
		mFMWindowPtr->hideWnd();
		break;

	default:
		break;
	}
}

static void _change_sound_mode(sound_mode_e mode) {
	if ((mode::get_sound_mode() == E_SOUND_MODE_LINK) || (mode == E_SOUND_MODE_LINK)) {
		mActivityPtr->registerUserTimer(LINK_RESTART_TIMER, 0);
	}

	mode::set_sound_mode(mode);
}


class FMSeekBarChangeListener : public ZKSeekBar::ISeekBarChangeListener {
public:
	virtual void onProgressChanged(ZKSeekBar *pSeekBar, int progress) {
		set_EditTextFmFreq_fmVal(progress);
	}
	virtual void onStartTrackingTouch(ZKSeekBar *pSeekBar) {

	}
	virtual void onStopTrackingTouch(ZKSeekBar *pSeekBar) {
		//pSeekBar->getProgress();
		uart::fm_frequency(pSeekBar->getProgress() + 875);
		sys::setting::set_fm_frequency(pSeekBar->getProgress() + 875);
	    //sys::fm_set_freq((int)(atof(mEditTextFmFreqPtr->getText().c_str()) * 100));
	    usleep(1000 * 100);
	    LOGD("--%d-- fm_get_freq %d\n", __LINE__, pSeekBar->getProgress() + 875);
	}
};

}
static FMSeekBarChangeListener sSeekBarChangeListener;

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
	mTextViewFMPtr->setTouchPass(true);
	mTextViewARMPtr->setTouchPass(true);
	mTextViewAUXPtr->setTouchPass(true);
	mTextViewCPPtr->setTouchPass(true);
	mfmSeekBarPtr->setSeekBarChangeListener(&sSeekBarChangeListener);
	mfmSeekBarPtr->setProgress(sys::setting::get_fm_frequency() - 880);
	char buf[16];
	sprintf(buf, "%.1f", (sys::setting::get_fm_frequency()) / 10.f);
	mEditTextFmFreqPtr->setText(buf);

	_sound_mode_cb(mode::get_sound_mode());
	mode::add_sound_mode_cb(_sound_mode_cb);
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
//	app::show_headwnd_widget();
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
	mode::remove_sound_mode_cb(_sound_mode_cb);
	mfmSeekBarPtr->setSeekBarChangeListener(NULL);
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
static bool onFMemitActivityTouchEvent(const MotionEvent &ev) {
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

static void onEditTextChanged_EditTextFmFreq(const std::string &text) {
    //LOGD(" onEditTextChanged_ EditTextFmFreq %s !!!\n", text.c_str());
}

static bool onButtonClick_ButtonFM(ZKButton *pButton) {
    LOGD(" ButtonClick ButtonFM !!!\n");
    _change_sound_mode(E_SOUND_MODE_FM);
    return false;
}

static bool onButtonClick_decButton(ZKButton *pButton) {
    LOGD(" ButtonClick decButton !!!\n");
	int progress = mfmSeekBarPtr->getProgress();
	if (progress > 0) {
		mfmSeekBarPtr->setProgress(progress - 1);
		uart::fm_frequency(progress - 1 + 875);
		sys::setting::set_fm_frequency(progress - 1 + 875);
	    //sys::fm_set_freq((int)(atof(mEditTextFmFreqPtr->getText().c_str()) * 100));
	    usleep(1000 * 100);
	}
    return false;
}

static bool onButtonClick_addButton(ZKButton *pButton) {
    LOGD(" ButtonClick addButton !!!\n");
	int progress = mfmSeekBarPtr->getProgress();
	if (progress < mfmSeekBarPtr->getMax()) {
		mfmSeekBarPtr->setProgress(progress + 1);
		uart::fm_frequency(progress + 1 + 875);
		sys::setting::set_fm_frequency(progress + 1 +875);
	    //sys::fm_set_freq((int)(atof(mEditTextFmFreqPtr->getText().c_str()) * 100));
	    usleep(1000 * 100);
	}
    return false;
}

static void onProgressChanged_fmSeekBar(ZKSeekBar *pSeekBar, int progress) {
    //LOGD(" ProgressChanged fmSeekBar %d !!!\n", progress);
}

static void onProgressChanged_invalidSeekBar(ZKSeekBar *pSeekBar, int progress) {
    //LOGD(" ProgressChanged invalidSeekBar %d !!!\n", progress);
}

static bool onButtonClick_AUXButton(ZKButton *pButton) {
    LOGD(" ButtonClick AUXButton !!!\n");
    _change_sound_mode(E_SOUND_MODE_AUX);
    return false;
}

static bool onButtonClick_ARMButton(ZKButton *pButton) {
    LOGD(" ButtonClick ARMButton !!!\n");
    _change_sound_mode(E_SOUND_MODE_SPK);
    return false;
}

static bool onButtonClick_BTButton(ZKButton *pButton) {
    LOGD(" ButtonClick BTButton !!!\n");
    if (!_probe_link_connected()) {
		_change_sound_mode(E_SOUND_MODE_LINK);
    }
    return false;
}
static bool onButtonClick_tipSure(ZKButton *pButton) {
    LOGD(" ButtonClick tipSure !!!\n");
	mLinkTipWindowPtr->hideWnd();
	_change_sound_mode(E_SOUND_MODE_LINK);
    return false;
}

static bool onButtonClick_tipCancel(ZKButton *pButton) {
    LOGD(" ButtonClick tipCancel !!!\n");
    mLinkTipWindowPtr->hideWnd();
    return false;
}
