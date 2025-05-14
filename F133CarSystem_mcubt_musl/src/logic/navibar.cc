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
#include "net/NetManager.h"
#include "net/context.h"
#include "utils/TimeHelper.h"
#include "manager/LanguageManager.h"
//#include "system/fm_emit.h"
#include "system/reverse.h"

#include "uart/context.h"


#define WIFIMANAGER			NETMANAGER->getWifiManager()
#define DELAY_INIT_TIMER     2
#define DELAY_CLEAR_TIMER     3
static bool release_flag = false;
static int useag_value;
static bt_cb_t _s_bt_cb;
extern void setSettingFtu_BrillianceSeekBar(int progress);
extern void setettingFtu_CallSeekBar(int progress);
extern void setettingFtu_MediaSeekBar(int progress);
extern void MainFtu_MediaSeekBar(int progress);
static bool navibar_is_load = false;

void set_navibar_brightnessBar(int progress) {
	if (navibar_is_load)
		mbriSeekBarPtr->setProgress(progress);
}
void set_navibar_PlayVolSeekBar(int progress) {
	if (navibar_is_load)
		mPlayVolSeekBarPtr->setProgress(progress);
}
void set_navibar_CpSeekBar(int progress) {
	if (navibar_is_load)
		mCpSeekBarPtr->setProgress(progress);
}

// 显示导航栏home键
void to_home_btn_show() {
	msys_homePtr->setVisible(true);
}

static void sys_time_(tm t ) {
	const char* week[] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
	char buf[128] = {0};
	sprintf(buf, "%d-%02d-%02d", t.tm_year+1900, t.tm_mon+1, t.tm_mday);
	mdataTextViewPtr->setText(buf);
	sprintf(buf, "%02d:%02d", t.tm_hour, t.tm_min);
	mweekTextViewPtr->setText(LANGUAGEMANAGER->getValue(week[t.tm_wday%7]));
}

void showSysInfo(ZKTextView* textViewPtr) {
	struct sysinfo info;
	sysinfo(&info);

	long totoalMem = info.totalram;
	long freeram = info.freeram;

	if(freeram <= 5*1000*1000) {	//剩余内存小于5M时,释放缓存
		fy::drop_caches();
	}

	char text[128];
	sprintf(text,"freeram:%d totoal:%d (MB)  useag:%.2f%%"
			, (int)(freeram>>20), (int)(totoalMem>>20), ((totoalMem-freeram)*1.0)/(totoalMem*1.0)*100) ;
	textViewPtr->setText(text);
	useag_value = (int)((totoalMem-freeram)*1.0)/(totoalMem*1.0)*100;
	if(!release_flag){
		mcacheCircleBarPtr->setProgress((int)((totoalMem-freeram)*1.0)/(totoalMem*1.0)*100);
	}
}

static void _bt_power_cb(bt_power_state_e state) {
	switch (state) {
	case E_BT_POWER_STATE_OFF:
		mbtTextViewPtr->setTextTr("Bluetooth");
		mbtButtonPtr->setTextTr("ununited");

		msys_homePtr->setVisible(true);
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
		msys_homePtr->setVisible(true);
		mbtTextViewPtr->setTextTr("Bluetooth");
		mbtButtonPtr->setTextTr("ununited");
		break;
	case E_BT_CONNECT_STATE_CONNECTING:         // 连接中
		mbtButtonPtr->setTextTr("connecting");
		break;
	case E_BT_CONNECT_STATE_CONNECTED:          // 已连接
		mbtTextViewPtr->setText(bt::get_connect_dev().name);
		mbtButtonPtr->setTextTr("connected");
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
		msys_homePtr->setVisible(false);
		break;
	case E_BT_CALL_STATE_IDLE:
		msys_homePtr->setVisible(true);
		can_long_click = true;
		break;
	default:
		msys_homePtr->setVisible(true);
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
			msoundButtonPtr->setSelected(progress < 1);
			bool effect = bt::is_calling() || (lk::is_connected() && lk::get_is_call_state() != CallState_Hang);
			audio::set_system_vol(progress / 10.f, !effect);
			setettingFtu_MediaSeekBar(progress);
			MainFtu_MediaSeekBar(progress);
		} else if (pSeekBar->getID() == mCpSeekBarPtr->getID()) {
			audio::set_lylink_call_vol(progress / 10.f, false);
			if(bt::is_calling()){
				bt::call_vol(progress / 10.f);
			}else if((lk::is_connected() && lk::get_is_call_state() != CallState_Hang)){
				audio::set_lylink_call_vol(progress / 10.f, true);
			}
			mcallButtonPtr->setSelected(progress < 1);
			setettingFtu_CallSeekBar(progress);
		} else if (pSeekBar->getID() == mbriSeekBarPtr->getID()) {
			sys::setting::set_brightness(progress*10);
			setSettingFtu_BrillianceSeekBar(progress);
			mbriButtonPtr->setSelected(progress < 1);
		}
	}
	virtual void onStartTrackingTouch(ZKSeekBar *pSeekBar) {

	}
	virtual void onStopTrackingTouch(ZKSeekBar *pSeekBar) {
		int progress = pSeekBar->getProgress();
		if (pSeekBar->getID() == mPlayVolSeekBarPtr->getID()) {
//			bool effect = bt::is_calling() || (lk::is_connected() && lk::get_is_call_state() != CallState_Hang);
//			audio::set_system_vol(progress / 10.f, !effect);
//			setettingFtu_MediaSeekBar(progress);
			//MainFtu_MediaSeekBar(progress);
		} else if (pSeekBar->getID() == mCpSeekBarPtr->getID()) {
//			audio::set_lylink_call_vol(progress / 10.f, false);
//			if(bt::is_calling()){
//				bt::call_vol(progress / 10.f);
//			}else if((lk::is_connected() && lk::get_is_call_state() != CallState_Hang)){
//				audio::set_lylink_call_vol(progress / 10.f, true);
//			}
//			bool effect = bt::is_calling() || (lk::is_connected() && lk::get_is_call_state() != CallState_Hang);
//			audio::set_lylink_call_vol(progress / 10.f, effect);
			setettingFtu_CallSeekBar(progress);
		} else if (pSeekBar->getID() == mbriSeekBarPtr->getID()) {
//			sys::setting::set_brightness(progress*10);
//			setSettingFtu_BrillianceSeekBar(progress);
		}
	}
};

}
static SeekListener seekListener;
static LongClickListener longButtonClickListenerWin;



// 此种方式效果不太好，，实际失效时间有延迟，调节响应不跟手
//static void _message_handler(const fy::message *msg, const void *user_data);
//static fy::handler _s_handler(_message_handler, NULL);
//
//
//#define PLAY_VOL       1
//#define CALL_VOL       2
//
//static void _message_handler(const fy::message *msg, const void *user_data) {
//	switch (msg->what) {
//	case PLAY_VOL: {
//		int progress = (int) msg->arg1;
//		bool effect = bt::is_calling() || (lk::is_connected() && lk::get_is_call_state() != CallState_Hang);
//		audio::set_system_vol(progress / 10.f, !effect);
//		setettingFtu_MediaSeekBar(progress);
//		MainFtu_MediaSeekBar(progress);
//		break;
//	}
//	case CALL_VOL: {
//		int progress = (int) msg->arg1;
//		bool effect = bt::is_calling() || (lk::is_connected() && lk::get_is_call_state() != CallState_Hang);
//		audio::set_lylink_call_vol(progress / 10.f, effect);
//		setettingFtu_CallSeekBar(progress);
//		break;
//	}
//	}
//}






static void _UI_init() {
	mfmTextViewPtr->setTouchPass(true);
	mTextView1Ptr->setTouchPass(true);
	mbtTextViewPtr->setTouchPass(true);
	mTextView2Ptr->setTouchPass(true);

	mfmTextViewPtr->setLongMode(ZKTextView::E_LONG_MODE_DOTS);
	mbtTextViewPtr->setLongMode(ZKTextView::E_LONG_MODE_DOTS);
}

static void sys_play_mode_cb_(audio_player_mode_e mode) {
	switch(mode) {
	case E_AUDIO_PLAYER_MODE_FM:     // fm发射
		mfmButtonPtr->setSelected(true);
		mfmTextViewPtr->setTextTr("Turn on");
		break;
	case E_AUDIO_PLAYER_MODE_SPK:    // 扬声器
	case E_AUDIO_PLAYER_MODE_HP:     // 耳机
	case E_AUDIO_PLAYER_MODE_BT: 	 // 蓝牙发射
		mfmButtonPtr->setSelected(false);
		mfmTextViewPtr->setTextTr("Turn off");
		break;
	}
}

static void fm_status_cb(bool status){
	mfmButtonPtr->setSelected(status);
	if(status) {
		mfmTextViewPtr->setTextTr("Turn on");
	} else {
		mfmTextViewPtr->setTextTr("Turn off");
	}
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
	_UI_init();

	mfmButtonPtr->setLongClickListener(&longButtonClickListenerWin);
	mbtButtonPtr->setLongClickListener(&longButtonClickListenerWin);

	navibar_is_load = true;
	mnavibarPtr->show();
	mPlayVolSeekBarPtr->setSeekBarChangeListener(&seekListener);
	mCpSeekBarPtr->setSeekBarChangeListener(&seekListener);
	mbriSeekBarPtr->setSeekBarChangeListener(&seekListener);


	LayoutPosition pos = mnavibarPtr->getPosition();
	pos.mTop = -pos.mHeight;
	mnavibarPtr->setPosition(pos);

	_bt_add_cb();

	// 收音机发射
	uart::fm_switch(sys::setting::get_fm_switch());
	uart::add_fm_state_cb(fm_status_cb);
	uart::query_fmswitch();
//	mfmButtonPtr->setSelected(sys::fm_is_enable());

	mfmTextViewPtr->setTextTr(mfmButtonPtr->isSelected() ? "Turn on" : "Turn off");
	sys::setting::add_play_state_cb(sys_play_mode_cb_);

	//注册定时器
	mnavibarPtr->registerUserTimer(DELAY_INIT_TIMER, 0);
}

/*
 * 当界面完全退出时触发
 */
static void onUI_quit() {
	navibar_is_load = false;
	mPlayVolSeekBarPtr->setSeekBarChangeListener(NULL);
	mCpSeekBarPtr->setSeekBarChangeListener(NULL);
	mbriSeekBarPtr->setSeekBarChangeListener(NULL);
	mfmButtonPtr->setLongClickListener(NULL);
	mbtButtonPtr->setLongClickListener(NULL);
	sys::setting::remove_play_state_cb(sys_play_mode_cb_);
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
		mCpSeekBarPtr->setProgress(audio::get_lylink_call_vol() * 10);
		EASYUICONTEXT->showNaviBar();
		return false;			//只执行一次
	case DELAY_CLEAR_TIMER:{
		static int value;
		if(++value <= useag_value){
			mcacheCircleBarPtr->setProgress(value);
		}else{
			value = 0;
			release_flag = false;
			return false;
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
static bool onnavibarActivityTouchEvent(const MotionEvent &ev) {
	 if (sys::reverse_does_enter_status()){
		return false;
	 }
	SLIDEMANAGER->onTouchEvent(ev, mnavibarPtr);
    switch (ev.mActionStatus) {
		case MotionEvent::E_ACTION_DOWN://触摸按下
			//LOGD("时刻 = %ld 坐标  x = %d, y = %d", ev.mEventTime, ev.mX, ev.mY);
			break;
		case MotionEvent::E_ACTION_MOVE://触摸滑动
			break;
		case MotionEvent::E_ACTION_UP:  //触摸抬起
			//LOGD("--%d-- --%s-- --isNaviBarShow:%d--", __LINE__, __FILE__, EASYUICONTEXT->isNaviBarShow());
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

static void onProgressChanged_CpSeekBar(ZKSeekBar *pSeekBar, int progress) {
    //LOGD(" ProgressChanged CpSeekBar %d !!!\n", progress);
//	mcallButtonPtr->setSelected(progress < 1);
//	fy::message _s_message(CALL_VOL, NULL, progress);
//	_s_handler.send_message(_s_message);
}

static void onProgressChanged_briSeekBar(ZKSeekBar *pSeekBar, int progress) {
    //LOGD(" ProgressChanged RecSeekBar %d !!!\n", progress);
//	mbriButtonPtr->setSelected(progress < 1);
//	sys::setting::set_brightness(progress*10);
//	setSettingFtu_BrillianceSeekBar(progress);
}

static bool onButtonClick_sys_home(ZKButton *pButton) {
    LOGD(" ButtonClick sys_home !!!\n");
	LayoutPosition pos = mnavibarPtr->getPosition();
	pos.mTop = -pos.mHeight;
	mnavibarPtr->setPosition(pos);
    return false;
}

static bool onButtonClick_soundButton(ZKButton *pButton) {
    LOGD(" ButtonClick soundButton !!!\n");
    return false;
}

static bool onButtonClick_callButton(ZKButton *pButton) {
    LOGD(" ButtonClick callButton !!!\n");
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

static bool onButtonClick_releaseButton(ZKButton *pButton) {
    LOGD(" ButtonClick releaseButton !!!\n");
    if(!release_flag){
    	mcacheCircleBarPtr->setProgress(0);
    	release_flag = true;
        fy::drop_caches();
        mnavibarPtr->registerUserTimer(DELAY_CLEAR_TIMER, 20);
    }
    return false;
}

static bool onButtonClick_fmButton(ZKButton *pButton) {
    LOGD(" ButtonClick fmButton !!!\n");
//    sys::fm_set_mute(sys::fm_is_enable());
    uart::fm_switch(!pButton->isSelected());
    sys::setting::set_fm_switch(!pButton->isSelected());
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
