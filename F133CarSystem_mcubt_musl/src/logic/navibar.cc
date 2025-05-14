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
#include "media/media_context.h"
#include "media/audio_context.h"
#include "utils/SlideManager.h"
#include <sys/sysinfo.h>
#include "system/usb_monitor.h"
#include "bt/context.h"
#include "link/context.h"
#include "fy/os.hpp"
#include "system/setting.h"
#include "net/context.h"
#include "utils/TimeHelper.h"
#include "manager/LanguageManager.h"
#include "system/reverse.h"
#include "config.h"
#include "uart/context.h"
#include "mode_observer.h"
#include "sysapp_context.h"


#define DELAY_INIT_TIMER     2
static bt_cb_t _s_bt_cb;
extern void setSettingFtu_BrillianceSeekBar(int progress);
extern void setSettingFtu_MediaSeekBar(int progress);
extern void set_ctrlbar_lightSeekBar(int progress);
extern void set_ctrlbar_volumSeekBar(int progress);
static bool Activity_is_load = false;

void set_navibar_brightnessBar(int progress) {
	if (Activity_is_load)
		mbriSeekBarPtr->setProgress(progress);
}
void set_navibar_PlayVolSeekBar(int progress) {
	if (Activity_is_load)
		mPlayVolSeekBarPtr->setProgress(progress);
}

static void sys_time_(tm t ) {
	const char* week[] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
	char buf[128] = {0};
	int hour = t.tm_hour;
	if (!sys::setting::is_time_format_24h()) {
		mMeridiemTextViewPtr->setTextTr((hour >= 12) ? "PM" : "AM");
		hour = (hour > 12) ? (hour % 12) : hour;
	} else {
		mMeridiemTextViewPtr->setText("");
	}


	sprintf(buf, "%02d-%02d %02d:%02d", t.tm_mon+1, t.tm_mday, hour, t.tm_min);
	mdataTextViewPtr->setText(buf);

	mweekTextViewPtr->setText(LANGUAGEMANAGER->getValue(week[t.tm_wday%7]));
}

void showSysInfo(ZKTextView* textViewPtr) {
	struct sysinfo info;
	sysinfo(&info);

	long freeram = info.freeram;

	if(freeram <= 5*1000*1000) {	//剩余内存小于5M时,释放缓存
		fy::drop_caches();
	}
}

static void _bt_power_cb(bt_power_state_e state) {
	switch (state) {
	case E_BT_POWER_STATE_OFF:
		mbtButtonPtr->setTextTr("Bluetooth");
		mbtTextViewPtr->setTextTr("ununited");

		mbtButtonPtr->setInvalid(false);
		mbtButtonPtr->setSelected(false);
		break;
	case E_BT_POWER_STATE_ON:
		mbtButtonPtr->setInvalid(false);
		mbtButtonPtr->setSelected(true);
		break;
	case E_BT_POWER_STATE_CHANGING:
		mbtButtonPtr->setInvalid(true);
		break;
	}
}

static void _bt_connect_cb(bt_connect_state_e state) {
	switch (state) {
	case E_BT_CONNECT_STATE_IDLE:               // 未连接
		mbtButtonPtr->setTextTr("Bluetooth");
		mbtTextViewPtr->setTextTr("ununited");
		break;
	case E_BT_CONNECT_STATE_CONNECTING:         // 连接中
		mbtTextViewPtr->setTextTr("connecting");
		break;
	case E_BT_CONNECT_STATE_CONNECTED:          // 已连接
		mbtButtonPtr->setText(bt::get_connect_dev().name);
		mbtTextViewPtr->setTextTr("connected");
		break;
	default:
		break;
	}
}

static bool can_long_click = true;
static void _bt_call_cb(bt_call_state_e state) {
	if (lk::is_connected()) {		// 手机互联时不会触发蓝牙通话
		return;
	}

	switch(state) {
	case E_BT_CALL_STATE_OUTGOING:					// 去电
	case E_BT_CALL_STATE_TALKING:                	// 通话中
	case E_BT_CALL_STATE_INCOMING:
		can_long_click = false;
		break;
	case E_BT_CALL_STATE_IDLE:
		can_long_click = true;
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

//收起状态栏
void fold_statusbar() {
	LayoutPosition pos = mnavibarPtr->getPosition();
	if(pos.mTop == 0){
		pos.mTop = -pos.mHeight;
		mnavibarPtr->setPosition(pos);
	    SLIDEMANAGER->setCanSlide(false);
	}
}

namespace {
class LongClickListener : public ZKBase::ILongClickListener
{
public:
	virtual void onLongClick(ZKBase *pBase)
	{
		if(!can_long_click)
			return ;
		if(pBase->getID() == ID_NAVIBAR_fmButton){
			EASYUICONTEXT->openActivity("FMemitActivity");
		}
		else if (pBase->getID() == ID_NAVIBAR_btButton) {
			EASYUICONTEXT->openActivity("btsettingActivity");
		}
		fold_statusbar();
	}
};

class SeekListener : public ZKSeekBar::ISeekBarChangeListener {
public:
	virtual void onProgressChanged(ZKSeekBar *pSeekBar, int progress) {
		if (pSeekBar->getID() == mPlayVolSeekBarPtr->getID()) {
			bool effect = bt::is_calling() || (lk::is_connected() && lk::get_is_call_state() != CallState_Hang);
			audio::set_system_vol(progress / 10.f, !effect);
			mvoiceNumTextViewPtr->setText(progress*10);
			setSettingFtu_MediaSeekBar(progress);
			set_ctrlbar_volumSeekBar(progress);
		} else if (pSeekBar->getID() == mbriSeekBarPtr->getID()) {
			sys::setting::set_brightness(progress*10);
			mlightNumTextViewPtr->setText(progress*10);
			setSettingFtu_BrillianceSeekBar(progress);
			set_ctrlbar_lightSeekBar(progress);;
		}
	}
	virtual void onStartTrackingTouch(ZKSeekBar *pSeekBar) {

	}
	virtual void onStopTrackingTouch(ZKSeekBar *pSeekBar) {
		int progress = pSeekBar->getProgress();
		if (pSeekBar->getID() == mPlayVolSeekBarPtr->getID()) {
//			bool effect = bt::is_calling() || (lk::is_connected() && lk::get_is_call_state() != CallState_Hang);
//			audio::set_system_vol(progress / 10.f, !effect);
			setSettingFtu_MediaSeekBar(progress);
			set_ctrlbar_volumSeekBar(progress);
		} else if (pSeekBar->getID() == mbriSeekBarPtr->getID()) {
//			sys::setting::set_brightness(progress*10);
			setSettingFtu_BrillianceSeekBar(progress);
			set_ctrlbar_lightSeekBar(progress);
		}
	}
};

}
static SeekListener seekListener;
static LongClickListener longButtonClickListenerWin;

static void _UI_init() {
	mfmTextViewPtr->setTouchPass(true);
//	mTextView1Ptr->setTouchPass(true);
	mbtTextViewPtr->setTouchPass(true);
//	mTextView2Ptr->setTouchPass(true);
	msoundButtonPtr->setTouchPass(true);
	mbriButtonPtr->setTouchPass(true);
	mfmTextViewPtr->setLongMode(ZKTextView::E_LONG_MODE_DOTS);
	mbtButtonPtr->setLongMode(ZKTextView::E_LONG_MODE_DOTS);
}

static void _audio_output_mode_cb(audio_player_mode_e new_mode, audio_player_mode_e old_mode) {
	LOGD("[navibar] audio output mode %d ---> %d\n", old_mode, new_mode);
	switch (old_mode) {
	case E_AUDIO_PLAYER_MODE_FM:
		uart::fm_switch(false);
		break;
	default:
		break;
	}

	switch (new_mode) {
	case E_AUDIO_PLAYER_MODE_FM:
		uart::fm_switch(true);
		break;
	default:
		break;
	}
}

static void _sound_mode_cb(sound_mode_e mode) {
	switch (mode) {
	case E_SOUND_MODE_SPK:
		mfmButtonPtr->setSelected(false);
		mfmTextViewPtr->setTextTr("Turn off");
		audio::change_output_mode(E_AUDIO_PLAYER_MODE_SPK);
		break;
	case E_SOUND_MODE_LINK:
		mfmButtonPtr->setSelected(false);
		mfmTextViewPtr->setTextTr("Turn off");
//		audio::change_output_mode(E_AUDIO_PLAYER_MODE_BT);
		break;
	case E_SOUND_MODE_FM:
		mfmButtonPtr->setSelected(true);
		mfmTextViewPtr->setTextTr("Turn on");
		audio::change_output_mode(E_AUDIO_PLAYER_MODE_FM);
		break;
	case E_SOUND_MODE_AUX:
		mfmButtonPtr->setSelected(false);
		mfmTextViewPtr->setTextTr("Turn off");
		audio::change_output_mode(E_AUDIO_PLAYER_MODE_AUX);
		break;
	default:
		break;
	}
}

static void _change_sound_mode(sound_mode_e mode) {
	mode::set_sound_mode(mode);
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
	LOGD("topbar show");
	_UI_init();

	mfmButtonPtr->setLongClickListener(&longButtonClickListenerWin);
	mbtButtonPtr->setLongClickListener(&longButtonClickListenerWin);

	Activity_is_load = true;
	mnavibarPtr->show();
	mPlayVolSeekBarPtr->setSeekBarChangeListener(&seekListener);
//	mCpSeekBarPtr->setSeekBarChangeListener(&seekListener);
	mbriSeekBarPtr->setSeekBarChangeListener(&seekListener);


	LayoutPosition pos = mnavibarPtr->getPosition();
	pos.mTop = -pos.mHeight;
	mnavibarPtr->setPosition(pos);

	_bt_add_cb();

	mfmTextViewPtr->setTextTr(mfmButtonPtr->isSelected() ? "Turn on" : "Turn off");

	_sound_mode_cb(mode::get_sound_mode());
	audio::add_output_mode_cb(_audio_output_mode_cb);
	mode::add_sound_mode_cb(_sound_mode_cb);

	//注册定时器
	mnavibarPtr->registerUserTimer(DELAY_INIT_TIMER, 0);
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
	Activity_is_load = false;
	mPlayVolSeekBarPtr->setSeekBarChangeListener(NULL);
	mbriSeekBarPtr->setSeekBarChangeListener(NULL);
	mfmButtonPtr->setLongClickListener(NULL);
	mbtButtonPtr->setLongClickListener(NULL);
	audio::remove_output_mode_cb(_audio_output_mode_cb);
	mode::remove_sound_mode_cb(_sound_mode_cb);
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
		showSysInfo(mTextviewDDRPtr);
		sys_time_(*TimeHelper::getDateTime());
	}
		break;
	case DELAY_INIT_TIMER: 		//等待初始化完成
		mbriSeekBarPtr->setProgress(sys::setting::get_brightness() / 10);
		mPlayVolSeekBarPtr->setProgress(audio::get_system_vol() * 10);
		EASYUICONTEXT->showNaviBar();
		return false;			//只执行一次
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
static bool onnavibarActivityTouchEvent(const MotionEvent &ev) {
	if (sys::reverse_does_enter_status()){
		return false;
	}
	if (mPlayVolSeekBarPtr->isPressed()) {
		return false;
	}
	if (mbriSeekBarPtr->isPressed()) {
		return false;
	 	 }
	SLIDEMANAGER->onTouchEvent(ev, mnavibarPtr);
	static MotionEvent last;
	switch (ev.mActionStatus) {
	case MotionEvent::E_ACTION_DOWN://触摸按下
//		 LOGD("时刻 = %ld 坐标  x = %d, y = %d", ev.mEventTime, ev.mX, ev.mY);
		break;
	case MotionEvent::E_ACTION_MOVE://触摸滑动
		break;
	case MotionEvent::E_ACTION_UP:  //触摸抬起
		SLIDEMANAGER->setCanSlide(true);
		break;
	default:
		SLIDEMANAGER->setCanSlide(true);
		break;
	}
	return false;
}

static void onProgressChanged_PlayVolSeekBar(ZKSeekBar *pSeekBar, int progress) {
    //LOGD(" ProgressChanged PlayVolSeekBar %d !!!\n", progress);
//	msoundButtonPtr->setSelected(progress < 1);
//	fy::message _s_message(PLAY_VOL, NULL, progress);
//	_s_handler.send_message(_s_message);
}

//static void onProgressChanged_CpSeekBar(ZKSeekBar *pSeekBar, int progress) {
    //LOGD(" ProgressChanged CpSeekBar %d !!!\n", progress);
//	mcallButtonPtr->setSelected(progress < 1);
//	fy::message _s_message(CALL_VOL, NULL, progress);
//	_s_handler.send_message(_s_message);
//}

static void onProgressChanged_briSeekBar(ZKSeekBar *pSeekBar, int progress) {
    //LOGD(" ProgressChanged RecSeekBar %d !!!\n", progress);
//	mbriButtonPtr->setSelected(progress < 1);
//	sys::setting::set_brightness(progress*10);
//	setSettingFtu_BrillianceSeekBar(progress);
}

static bool onButtonClick_soundButton(ZKButton *pButton) {
    LOGD(" ButtonClick soundButton !!!\n");
    return false;
}

static bool onButtonClick_briButton(ZKButton *pButton) {
    LOGD(" ButtonClick briButton !!!\n");
    return false;
}

static bool onButtonClick_btButton(ZKButton *pButton) {
    LOGD(" ButtonClick btButton !!!\n");
    bt::is_on() ? bt::power_off() : bt::power_on();
    return false;
}

static bool onButtonClick_fmButton(ZKButton *pButton) {
    LOGD(" ButtonClick fmButton !!!\n");
    if (mode::get_sound_mode() == E_SOUND_MODE_FM) {
    	_change_sound_mode(E_SOUND_MODE_SPK);
    } else {
    	_change_sound_mode(E_SOUND_MODE_FM);
    }
    return false;
}

static bool onButtonClick_screenoffBtn(ZKButton *pButton) {
    LOGD(" ButtonClick screenoffBtn !!!\n");
    if (!sys::reverse_does_enter_status()){
    	LayoutPosition pos = mnavibarPtr->getPosition();
    	pos.mTop = -pos.mHeight;
    	mnavibarPtr->setPosition(pos);
    	EASYUICONTEXT->openActivity("screenOffActivity");
    }

    return false;
}
static bool onButtonClick_SoundButton(ZKButton *pButton) {
    LOGD(" ButtonClick SoundButton !!!\n");
    EASYUICONTEXT->openActivity("soundEffectActivity");
	fold_statusbar();
    return false;
}

static bool onButtonClick_settingButton(ZKButton *pButton) {
    LOGD(" ButtonClick settingButton !!!\n");
    EASYUICONTEXT->openActivity("settingsActivity");
	fold_statusbar();
    return false;
}

static bool onButtonClick_Button1(ZKButton *pButton) {
    LOGD(" ButtonClick Button1 !!!\n");
    return false;
}
