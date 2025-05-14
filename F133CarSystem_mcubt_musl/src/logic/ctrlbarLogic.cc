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
#include "config.h"
#include "app/SysAppFactory.h"
#include "sysapp_context.h"
#include "mode_observer.h"
#include "system/setting.h"
#include "link/context.h"
#include "bt/context.h"
#include "media/audio_context.h"

REGISTER_SYSAPP(APP_TYPE_SYS_CTRLBAR, ctrlbarActivity);

#define BAR_HIDE_TIMER       1
#define BAR_HIDE_TIMEOUT     5000

extern void setSettingFtu_BrillianceSeekBar(int progress);
extern void setSettingFtu_MediaSeekBar(int progress);
extern void set_navibar_brightnessBar(int progress);
extern void set_navibar_PlayVolSeekBar(int progress);
static bool ctrlbar_is_load = false;

void set_ctrlbar_lightSeekBar(int progress) {
	if (ctrlbar_is_load) {
		mlightSeekBarPtr->setProgress(progress);
	}
}

void set_ctrlbar_volumSeekBar(int progress) {
	if (ctrlbar_is_load) {
		mvolumSeekBarPtr->setProgress(progress);
	}
}

namespace {
class SeekListener : public ZKSeekBar::ISeekBarChangeListener {
public:
	virtual void onProgressChanged(ZKSeekBar *pSeekBar, int progress) {
		if (pSeekBar->getID() == mvolumSeekBarPtr->getID()) {
			bool effect = bt::is_calling() || (lk::is_connected() && lk::get_is_call_state() != CallState_Hang);
			audio::set_system_vol(progress / 10.f, !effect);
			mvolumTextViewPtr->setText(progress*10);
			setSettingFtu_MediaSeekBar(progress);
			set_navibar_PlayVolSeekBar(progress);
		} else if (pSeekBar->getID() == mlightSeekBarPtr->getID()) {
			sys::setting::set_brightness(progress*10);
			mlightTextViewPtr->setText(progress*10);
			setSettingFtu_BrillianceSeekBar(progress);
			set_navibar_brightnessBar(progress);
		}
	}
	virtual void onStartTrackingTouch(ZKSeekBar *pSeekBar) {

	}
	virtual void onStopTrackingTouch(ZKSeekBar *pSeekBar) {
		int progress = pSeekBar->getProgress();
		if (pSeekBar->getID() == mvolumSeekBarPtr->getID()) {
//			bool effect = bt::is_calling() || (lk::is_connected() && lk::get_is_call_state() != CallState_Hang);
//			audio::set_system_vol(progress / 10.f, !effect);
			setSettingFtu_MediaSeekBar(progress);
			set_navibar_PlayVolSeekBar(progress);
		} else if (pSeekBar->getID() == mlightSeekBarPtr->getID()) {
//			sys::setting::set_brightness(progress*10);
			setSettingFtu_BrillianceSeekBar(progress);
			set_navibar_brightnessBar(progress);
		}
	}
};

}
static SeekListener seekListener;

static void _event_mode_cb(event_mode_e mode) {
	if (mode == E_EVENT_MODE_BRIGHTNESS) {
		mlightSeekBarPtr->setProgress(sys::setting::get_brightness() / 10);
		mlightTextViewPtr->setText(sys::setting::get_brightness());
		mlightWindowPtr->showWnd();
		mvolumWindowPtr->hideWnd();
		mActivityPtr->unregisterUserTimer(BAR_HIDE_TIMER);
		mActivityPtr->registerUserTimer(BAR_HIDE_TIMER, BAR_HIDE_TIMEOUT);
	} else if (mode == E_EVENT_MODE_VOICE) {
		mvolumSeekBarPtr->setProgress(audio::get_system_vol() * 10);
		mvolumTextViewPtr->setText((int)(audio::get_system_vol()*100));
		mvolumWindowPtr->showWnd();
		mlightWindowPtr->hideWnd();
		mActivityPtr->unregisterUserTimer(BAR_HIDE_TIMER);
		mActivityPtr->registerUserTimer(BAR_HIDE_TIMER, BAR_HIDE_TIMEOUT);
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
	_event_mode_cb(mode::get_event_mode());
	mode::add_event_mode_cb(_event_mode_cb);

	ctrlbar_is_load = true;
	mvolumSeekBarPtr->setSeekBarChangeListener(&seekListener);
	mlightSeekBarPtr->setSeekBarChangeListener(&seekListener);
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
	mode::remove_event_mode_cb(_event_mode_cb);
	ctrlbar_is_load = false;
	mvolumSeekBarPtr->setSeekBarChangeListener(NULL);
	mlightSeekBarPtr->setSeekBarChangeListener(NULL);
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
	case BAR_HIDE_TIMER:
		app::hide_ctrlbar();
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
static bool onctrlbarActivityTouchEvent(const MotionEvent &ev) {
	static bool isHit;

    switch (ev.mActionStatus) {
	case MotionEvent::E_ACTION_DOWN://触摸按下
		//LOGD("时刻 = %ld 坐标  x = %d, y = %d", ev.mEventTime, ev.mX, ev.mY);
		if (mActivityPtr->isShow()) {
			if (mActivityPtr->getPosition().isHit(ev.mX, ev.mY)) {
				isHit = true;
				mActivityPtr->resetUserTimer(BAR_HIDE_TIMER, BAR_HIDE_TIMEOUT);
				break;
			}
		}

		isHit = false;
		break;
	case MotionEvent::E_ACTION_MOVE://触摸滑动
		if (isHit) {
			mActivityPtr->resetUserTimer(BAR_HIDE_TIMER, BAR_HIDE_TIMEOUT);
		}
		break;
	case MotionEvent::E_ACTION_UP:  //触摸抬起
		if (!isHit) {
			mActivityPtr->resetUserTimer(BAR_HIDE_TIMER, 10);
		}
		break;
	default:
		break;
	}
	return false;
}
static void onProgressChanged_volumSeekBar(ZKSeekBar *pSeekBar, int progress) {
    //LOGD(" ProgressChanged volumSeekBar %d !!!\n", progress);
}

static bool onButtonClick_volumButton(ZKButton *pButton) {
    LOGD(" ButtonClick volumButton !!!\n");
    return false;
}

static void onProgressChanged_lightSeekBar(ZKSeekBar *pSeekBar, int progress) {
    //LOGD(" ProgressChanged lightSeekBar %d !!!\n", progress);
}

static bool onButtonClick_lightButton(ZKButton *pButton) {
    LOGD(" ButtonClick lightButton !!!\n");
    return false;
}
