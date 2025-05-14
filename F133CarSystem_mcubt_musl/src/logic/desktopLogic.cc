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

#include "net/context.h"
#include "link/context.h"
#include "uart/context.h"
#include "bt/context.h"
#include "media/audio_context.h"
#include "media/media_context.h"
#include "media/music_player.h"
#include "media/media_parser.h"
#include "system/setting.h"
#include "system/fm_emit.h"
#include "system/reverse.h"
#include "manager/LanguageManager.h"
#include "manager/ConfigManager.h"
#include "storage/StoragePreferences.h"
#include "misc/storage.h"
#include "fy/files.hpp"
#include "net/NetManager.h"
#include "os/MountMonitor.h"
#include "sysapp_context.h"

#define WIFIMANAGER			NETMANAGER->getWifiManager()

#define SOFT_VERSION	"2024.01.25 17:18"

//extern void set_navibar_PlayVolSeekBar(int progress

static bt_cb_t _s_bt_cb;
static void set_status_btn_visble_pos(ZKButton* pButton, bool isVisible);

static void parser() {
	std::string cur_play_file = media::music_get_current_play_file();
	id3_info_t info;
	bool isTrue = media::parse_id3_info(cur_play_file.c_str(), &info);
	if (isTrue && strcmp(info.title, "") != 0) {
		mtitleTextViewPtr->setText(info.title);
	} else {
		mtitleTextViewPtr->setText(fy::files::get_file_name(cur_play_file));
	}
	if (isTrue) {
		(strcmp(info.artist, "") == 0) ? martistTextViewPtr->setTextTr("Unknown") : martistTextViewPtr->setText(info.artist);
	} else {
		martistTextViewPtr->setTextTr("Unknown");
	}
	isTrue = media::parse_id3_pic(cur_play_file.c_str(), "/tmp/music.jpg");
	mmusicPicTextViewPtr->setBackgroundPic(isTrue ? "/tmp/music.jpg" : CONFIGMANAGER->getResFilePath("/HomePage/icon_media_cover_n.png").c_str());
}

static void _music_play_status_cb(music_play_status_e status) {
	switch (status) {
	case E_MUSIC_PLAY_STATUS_STARTED:     	// 播放开始
		parser();
		mButtonPlayPtr->setSelected(true);
		mtitleTextViewPtr->setLongMode(ZKTextView::E_LONG_MODE_SCROLL_CIRCULAR);
		mtitleTextViewPtr->setTextColor(0xFF00FF00);
		mPlayProgressSeekbarPtr->setMax(media::music_get_duration() / 1000);
		mPlayProgressSeekbarPtr->setProgress(0);
		break;
	case E_MUSIC_PLAY_STATUS_RESUME:     	// 恢复播放
		parser();
		mButtonPlayPtr->setSelected(true);
		mtitleTextViewPtr->setLongMode(ZKTextView::E_LONG_MODE_SCROLL_CIRCULAR);
		mtitleTextViewPtr->setTextColor(0xFF00FF00);
		break;
	case E_MUSIC_PLAY_STATUS_STOP:       	// 停止播放
		mPlayProgressSeekbarPtr->setMax(media::music_get_duration() / 1000);
		mButtonPlayPtr->setSelected(false);
		mtitleTextViewPtr->setLongMode(ZKTextView::E_LONG_MODE_DOTS);
		mtitleTextViewPtr->setTextColor(0xFF000000);
		mtitleTextViewPtr->setTextTr("Unknown");
		martistTextViewPtr->setTextTr("Unknown");
		mmusicPicTextViewPtr->setBackgroundPic("/HomePage/icon_media_cover_n.png");
		break;
	case E_MUSIC_PLAY_STATUS_PAUSE:      	// 暂停播放
		mtitleTextViewPtr->setLongMode(ZKTextView::E_LONG_MODE_NONE);
		mButtonPlayPtr->setSelected(false);
		mtitleTextViewPtr->setTextColor(0xFF000000);
		break;
	case E_MUSIC_PLAY_STATUS_COMPLETED:  	// 播放结束
//		media::music_next();
		break;
	case E_MUSIC_PLAY_STATUS_ERROR:      	// 播放错误
//		LOGE("music play error, will play next\n");
//		mActivityPtr->registerUserTimer(MUSIC_ERROR_TIMER, 3000);
		break;
	}
}

static void _lylink_callback(LYLINKAPI_EVENT evt, int para0, void *para1) {
	switch (evt) {
	case LYLINK_LINK_ESTABLISH:
		LOGD("[desktop] LYLINK_LINK_ESTABLISH %s", lk::_link_type_to_str((LYLINK_TYPE_E) para0));
		set_status_btn_visble_pos(mlinkButtonPtr, true);
		break;
	case LYLINK_LINK_DISCONN:
		LOGD("[desktop] LYLINK_LINK_DISCONN........... %s", lk::_link_type_to_str((LYLINK_TYPE_E) para0));
		set_status_btn_visble_pos(mlinkButtonPtr, false);
		break;
	default:
		break;
	}
}

/********************* 导航栏小图标——begin ***********************/
namespace {

class MainWifiListener : public WifiManager::IWifiListener {
public:
	virtual void handleWifiEnable(E_WIFI_ENABLE event, int args) {
		LOGD("MyWifiListener handleWifiEnable event: %d\n", event);
		switch (event) {
		case E_WIFI_ENABLE_ENABLE:
			break;
		case E_WIFI_ENABLE_DISABLE:
		case E_WIFI_ENABLE_DISABLEING:
			set_status_btn_visble_pos(mNetButtonPtr, false);
			break;
		case E_WIFI_ENABLE_ENABLEING:
		case E_WIFI_ENABLE_UNKNOW:
			break;
		}
	}
	virtual void handleWifiConnect(E_WIFI_CONNECT event, int args) {
		switch(event) {
		case E_WIFI_CONNECT_CONNECTED:
			set_status_btn_visble_pos(mNetButtonPtr, true);
			break;
		case E_WIFI_CONNECT_CONNECTING:
			break;
		case E_WIFI_CONNECT_DICONNECTING:
		case E_WIFI_CONNECT_DISCONNECT:
			set_status_btn_visble_pos(mNetButtonPtr, false);
			break;
		case E_WIFI_CONNECT_ERROR:
		case E_WIFI_CONNECT_UNKNOW:
			break;
		}
	}
};

class MYMountListener : public MountMonitor::IMountListener {
protected:
	virtual void notify(int what, int status, const char *msg) {
		switch (status) {
		case MountMonitor::E_MOUNT_STATUS_MOUNTED: {
			LOGD("media mount path: %s\n", msg);
			if (strcmp(msg, "/mnt/extsd") == 0) {
				set_status_btn_visble_pos(msdButtonPtr, true);
			} else if (strcmp(msg, "/mnt/usb1") == 0 || strcmp(msg, "/mnt/usbotg") == 0) {
				set_status_btn_visble_pos(musbButtonPtr, true);
			}
			break;
		}

		case MountMonitor::E_MOUNT_STATUS_UNMOUNTING: {
			LOGD("media remove path: %s\n", msg);
			if (strcmp(msg, "/mnt/extsd") == 0) {
				set_status_btn_visble_pos(msdButtonPtr, false);
			} else if (strcmp(msg, "/mnt/usbotg") == 0 && !MOUNTMONITOR->isMounted("/mnt/usb1")) {
				set_status_btn_visble_pos(musbButtonPtr, false);
			} else if (strcmp(msg, "/mnt/usb1") == 0 && !MOUNTMONITOR->isMounted("/mnt/usbotg")) {
				set_status_btn_visble_pos(musbButtonPtr, false);
			}
			break;
		}
		}
	}
};

class SeekListener : public ZKSeekBar::ISeekBarChangeListener {
public:
	virtual void onProgressChanged(ZKSeekBar *pSeekBar, int progress) {
	}
	virtual void onStopTrackingTouch(ZKSeekBar *pSeekBar) {
		int progress = pSeekBar->getProgress();
		if (pSeekBar->getID() == mPlayVolSeekBarPtr->getID()) {
			bool effect = bt::is_calling() || (lk::is_connected() && lk::get_is_call_state() != CallState_Hang);
			audio::set_system_vol(progress / 10.f, !effect);
//			set_navibar_PlayVolSeekBar(progress);
		}
	}
};

}
static MYMountListener _mount_listener;
static MainWifiListener mainWifiListener;
static SeekListener seekListener;

static void _bt_power_cb(bt_power_state_e state) {
	switch (state) {
	case E_BT_POWER_STATE_OFF:
		set_status_btn_visble_pos(mbtStatusButtonPtr, false);
		break;
	case E_BT_POWER_STATE_CHANGING:
		break;
	case E_BT_POWER_STATE_ON:
		set_status_btn_visble_pos(mbtStatusButtonPtr, true);
		break;
	default:
		break;
	}
}

static void _bt_connect_cb(bt_connect_state_e state) {
	switch (state) {
	case E_BT_CONNECT_STATE_IDLE:                // 未连接
		mbtStatusButtonPtr->setSelected(false);
		break;
	case E_BT_CONNECT_STATE_CONNECTING:          // 连接中
		break;
	case E_BT_CONNECT_STATE_CONNECTED:           // 已连接
		mbtStatusButtonPtr->setSelected(true);
		break;
	default:
		break;
	}
}

static void _bt_add_cb() {
	_s_bt_cb.power_cb = _bt_power_cb;
	_s_bt_cb.connect_cb = _bt_connect_cb;
	bt::add_cb(&_s_bt_cb);
}

static void _bt_remove_cb() {
	bt::remove_cb(&_s_bt_cb);
}

static void sys_play_mode_cb_(audio_player_mode_e mode) {
	switch(mode) {
	case E_AUDIO_PLAYER_MODE_FM:     // fm发射
		set_status_btn_visble_pos(mfmButtonPtr, true);
		break;
	case E_AUDIO_PLAYER_MODE_SPK:    // 扬声器
	case E_AUDIO_PLAYER_MODE_HP:     // 耳机
	case E_AUDIO_PLAYER_MODE_BT: 	 // 蓝牙发射
		set_status_btn_visble_pos(mfmButtonPtr, false);
		break;
	default:
		break;
	}
}

static ZKButton** BtnArr[] = {&msoundButtonPtr, &mNetButtonPtr, &mbtStatusButtonPtr, &musbButtonPtr,
		&msdButtonPtr, &mlinkButtonPtr, &mfmButtonPtr, &mauxButtonPtr};

static void set_status_btn_visble_pos(ZKButton* pButton, bool isVisible) {
	pButton->setVisible(isVisible);

	const int RIGHT = 1024 - 20; 	//图标右对齐边界
	const int INTERTVAL = 12; 		//图标对齐间隙

	int icon_left = RIGHT - (*BtnArr[TABLESIZE(BtnArr)-1])->getPosition().mWidth;		// 最后一个图标左边界

	for (size_t i=0; i<TABLESIZE(BtnArr); i++) {
		if ((*BtnArr[i])->isVisible()) {
			LayoutPosition pos = (*BtnArr[i])->getPosition();
			icon_left = icon_left - INTERTVAL - pos.mWidth;
			pos.mLeft = icon_left;
			(*BtnArr[i])->setPosition(pos);
		}
	}
}

static void Icon_Init() {
	set_status_btn_visble_pos(msoundButtonPtr, true); 		//常显
	set_status_btn_visble_pos(msdButtonPtr, MOUNTMONITOR->isMounted("/mnt/extsd"));
	set_status_btn_visble_pos(musbButtonPtr, MOUNTMONITOR->isMounted("/mnt/usb1") || MOUNTMONITOR->isMounted("/mnt/usbotg"));
	set_status_btn_visble_pos(mbtStatusButtonPtr, bt::is_on());
	mbtStatusButtonPtr->setSelected(bt::get_connect_state() == E_BT_CONNECT_STATE_CONNECTED);
	set_status_btn_visble_pos(mNetButtonPtr,WIFIMANAGER->isConnected());
	set_status_btn_visble_pos(mfmButtonPtr, sys::fm_is_enable());
	set_status_btn_visble_pos(mlinkButtonPtr, lk::is_connected());
	msoundButtonPtr->setSelected(audio::get_system_vol() < 0.1);

	WIFIMANAGER->addWifiListener(&mainWifiListener);
	MOUNTMONITOR->addMountListener(&_mount_listener);
	_bt_add_cb();
}
/********************* 导航栏小图标——end ***********************/

/**
 * 注册定时器
 * 填充数组用于注册定时器
 * 注意：id不能重复
 */
static S_ACTIVITY_TIMEER REGISTER_ACTIVITY_TIMER_TAB[] = {
//	{0,  1000*60}, //定时器id=0, 时间间隔60秒
	{1,  1000},
};

/**
 * 当界面构造时触发
 */
static void onUI_init() {
    //Tips :添加 UI初始化的显示代码到这里,如:mText1Ptr->setText("123");
	if (strcmp("UpgradeActivity", EASYUICONTEXT->currentAppName()) == 0) {
		LOGD("[desktop] currentAppName is UpgradeActivity");
		EASYUICONTEXT->goBack();
		return;
	}
	mStatusRadioGroupPtr->setCheckedID(ID_DESKTOP_RadioButton0);
	mPlayVolSeekBarPtr->setSeekBarChangeListener(&seekListener);
	media::music_add_play_status_cb(_music_play_status_cb);
	sys::setting::add_play_state_cb(sys_play_mode_cb_);

	mverTextViewPtr->setText(std::string("软件版本: ") + std::string(SOFT_VERSION));
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
	int curPos = -1;
	Icon_Init();
	parser();
	mPlayVolSeekBarPtr->setProgress(audio::get_system_vol() * 10);
//	mDigitalClock2Ptr->setVisible(true);

    if (media::music_is_playing()) {
    	mButtonPlayPtr->setSelected(true);
        curPos = media::music_get_current_position() / 1000;
        mPlayProgressSeekbarPtr->setMax(media::music_get_duration() / 1000);
    }
    if (curPos >= 0) {
    	mPlayProgressSeekbarPtr->setProgress(curPos);
    }
}

/*
 * 当界面隐藏时触发
 */
static void onUI_hide() {
//	mDigitalClock2Ptr->setVisible(false);
	// 隐藏时关闭该界面，释放界面资源。
//	EASYUICONTEXT->closeActivity("desktopActivity");
}

/*
 * 当界面完全退出时触发
 */
static void onUI_quit() {
	mPlayVolSeekBarPtr->setSeekBarChangeListener(NULL);
	lk::remove_lylink_callback(_lylink_callback);
	media::music_remove_play_status_cb(_music_play_status_cb);
	mPlayProgressSeekbarPtr->setSeekBarChangeListener(NULL);
	WIFIMANAGER->removeWifiListener(&mainWifiListener);
	MOUNTMONITOR->removeMountListener(&_mount_listener);
	sys::setting::remove_play_state_cb(sys_play_mode_cb_);
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
static bool onUI_Timer(int id) {
	switch (id) {
	case 1: {
		msoundButtonPtr->setSelected(audio::get_system_vol() < 0.1);
        int curPos = -1;
        if (media::music_is_playing()) {
            curPos = media::music_get_current_position() / 1000;
        }
        if (curPos >= 0) {
        	mPlayProgressSeekbarPtr->setProgress(curPos);
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
static bool ondesktopActivityTouchEvent(const MotionEvent &ev) {

	switch (ev.mActionStatus) {
	case MotionEvent::E_ACTION_DOWN:	//触摸按下
		LOGD("时刻 = %ld 坐标  x = %d, y = %d", ev.mEventTime, ev.mX, ev.mY);
		break;
	case MotionEvent::E_ACTION_MOVE:	//触摸滑动
		break;
	case MotionEvent::E_ACTION_UP:  	//触摸抬起
		break;
	default:
		break;
	}
	return false;
}

static bool onButtonClick_NextButton(ZKButton *pButton) {
    LOGD(" ButtonClick NextButton !!!\n");
	if (lk::is_connected()) {
		mlinkTipsWindowPtr->showWnd();
		return false;
	}
    media::music_next(true);
    return false;
}

static bool onButtonClick_ButtonPlay(ZKButton *pButton) {
    LOGD(" ButtonClick ButtonPlay !!!\n");

	if (lk::is_connected()) {
		mlinkTipsWindowPtr->showWnd();
		return false;
	}

    if (media::music_get_play_index() == -1) {
    	return false;
    } else if (media::music_is_playing()) {
        media::music_pause();
    } else {
    	media::music_resume();
    }
    return false;
}

static bool onButtonClick_PrevButton(ZKButton *pButton) {
    LOGD(" ButtonClick PrevButton !!!\n");
	if (lk::is_connected()) {
		mlinkTipsWindowPtr->showWnd();
		return false;
	}
    media::music_prev(true);
    return false;
}

static void onProgressChanged_PlayProgressSeekbar(ZKSeekBar *pSeekBar, int progress) {
    //LOGD(" ProgressChanged PlayProgressSeekbar %d !!!\n", progress);
}

static void onProgressChanged_PlayVolSeekBar(ZKSeekBar *pSeekBar, int progress) {
    //LOGD(" ProgressChanged PlayVolSeekBar %d !!!\n", progress);
}

static bool onButtonClick_Setting(ZKButton *pButton) {
    LOGD(" ButtonClick Setting !!!\n");
    EASYUICONTEXT->openActivity("settingsActivity");
    return false;
}

static bool onButtonClick_ToMusic(ZKButton *pButton) {
    LOGD(" ButtonClick ToMusic !!!\n");
	if (lk::is_connected()) {
		mlinkTipsWindowPtr->showWnd();
		return false;
	}
    EASYUICONTEXT->openActivity("musicActivity");
    return false;
}

// 不可触摸按键     状态栏小图标
static bool onButtonClick_auxButton(ZKButton *pButton) {
    return false;
}
static bool onButtonClick_fmButton(ZKButton *pButton) {
    return false;
}
static bool onButtonClick_linkButton(ZKButton *pButton) {
    return false;
}
static bool onButtonClick_sdButton(ZKButton *pButton) {
    return false;
}
static bool onButtonClick_usbButton(ZKButton *pButton) {
    return false;
}
static bool onButtonClick_soundButton(ZKButton *pButton) {
    return false;
}
static bool onButtonClick_btStatusButton(ZKButton *pButton) {
    return false;
}
static bool onButtonClick_NetButton(ZKButton *pButton) {
    return false;
}

static void open_linkhelp_activity(link_mode_e mode) {
	Intent *i = new Intent;
	i->putExtra("link_mode", fy::format("%d", mode));
	LOGD("[main] choose link mode %s\n", sys::setting::get_link_mode_str(mode));
	EASYUICONTEXT->openActivity("linkhelpActivity", i);
}

static void _change_link_app(link_mode_e mode) {
	switch (mode) {
	case E_LINK_MODE_HICAR:
	case E_LINK_MODE_ANDROIDAUTO:
	case E_LINK_MODE_CARPLAY:
		if (net::get_mode() != E_NET_MODE_AP) { net::change_mode(E_NET_MODE_AP);}
		if (!bt::is_on()) {bt::power_on();}
		break;
	case E_LINK_MODE_AIRPLAY:
		if (net::get_mode() != E_NET_MODE_WIFI) { net::change_mode(E_NET_MODE_WIFI); }
		break;
	case E_LINK_MODE_CARLIFE:
		if (net::get_mode() != E_NET_MODE_WIFI) { net::change_mode(E_NET_MODE_WIFI); }
		break;
	case E_LINK_MODE_MIRACAST:
	case E_LINK_MODE_LYLINK:
		if (net::get_mode() != E_NET_MODE_P2P) { net::change_mode(E_NET_MODE_P2P); }
		break;
	default:
		break;
	}
	open_linkhelp_activity(mode);
}

static void open_link_activity(link_mode_e mode) {
	LYLINK_TYPE_E link_type = lk::get_lylink_type();
	switch(mode) {
	case E_LINK_MODE_CARPLAY:
		if ((link_type == LINK_TYPE_WIFICP) || (link_type == LINK_TYPE_USBCP)) {
			EASYUICONTEXT->openActivity("lylinkviewActivity");
			return;
		}
		break;
	case E_LINK_MODE_ANDROIDAUTO:
		if ((link_type == LINK_TYPE_WIFIAUTO) || (link_type == LINK_TYPE_USBAUTO)) {
			EASYUICONTEXT->openActivity("lylinkviewActivity");
			return;
		}
		break;
	case E_LINK_MODE_CARLIFE:
		if ((link_type == LINK_TYPE_WIFILIFE) || (link_type == LINK_TYPE_USBLIFE)) {
			EASYUICONTEXT->openActivity("lylinkviewActivity");
			return;
		}
		break;
	case E_LINK_MODE_HICAR:
		if ((link_type == LINK_TYPE_WIFIHICAR) || (link_type == LINK_TYPE_USBHICAR)) {
			EASYUICONTEXT->openActivity("lylinkviewActivity");
			return;
		}
		break;
	case E_LINK_MODE_MIRACAST:
		if (link_type == LINK_TYPE_MIRACAST) {
			EASYUICONTEXT->openActivity("lylinkviewActivity");
			return;
		}
		break;
	case E_LINK_MODE_LYLINK:
		if (link_type == LINK_TYPE_WIFILY) {
			EASYUICONTEXT->openActivity("lylinkviewActivity");
			return;
		}
		break;
	case E_LINK_MODE_AIRPLAY:
		if (link_type == LINK_TYPE_AIRPLAY) {
			EASYUICONTEXT->openActivity("lylinkviewActivity");
			return;
		}
		break;
	default:
		break;
	}
	if (lk::is_connected()) {
		mlinkTipsWindowPtr->showWnd();
		return;
	}
	open_linkhelp_activity(mode);
//	_change_link_app(mode);
}

static bool onButtonClick_Button1(ZKButton *pButton) {
    LOGD(" ButtonClick Button1 !!!\n");
    EASYUICONTEXT->openActivity("mcubtUpdActivity");
//    EASYUICONTEXT->openActivity("TestRecordActivity");
    return false;
}

static bool onButtonClick_Button2(ZKButton *pButton) {
    LOGD(" ButtonClick Button2 !!!\n");
   // bt::query_mf();
    EASYUICONTEXT->openActivity("soundEffectActivity");
    return false;
}

static void onCheckedChanged_StatusRadioGroup(ZKRadioGroup* pRadioGroup, int checkedID) {
    LOGD(" RadioGroup StatusRadioGroup checked %d", checkedID);
}
static void onSlideItemClick_appSlideWindow(ZKSlideWindow *pSlideWindow, int index) {
    //LOGD(" onSlideItemClick_ appSlideWindow %d !!!\n", index);
	switch(index) {
	case 0:
	    LOGD(" ButtonClick CPButton !!!\n");
	    open_link_activity(E_LINK_MODE_CARPLAY);
		break;
	case 1:
		LOGD(" ButtonClick AAButton !!!\n");
		open_link_activity(E_LINK_MODE_ANDROIDAUTO);
		break;
	case 2:
	    LOGD(" ButtonClick APButton !!!\n");
	    open_link_activity(E_LINK_MODE_AIRPLAY);
    	break;
	case 3:
	    LOGD(" ButtonClick ACButton !!!\n");
	    open_link_activity(E_LINK_MODE_LYLINK);
 		break;
	case 4:
	    LOGD(" ButtonClick MCButton !!!\n");
	    open_link_activity(E_LINK_MODE_MIRACAST);
		break;
	case 5:
	    LOGD(" ButtonClick BtMusicButton !!!\n");
	    EASYUICONTEXT->openActivity("btsettingActivity");
 		break;
	case 6:  {
	    LOGD(" ButtonClick MusicButton !!!\n");
		if (lk::is_connected()) {
			mlinkTipsWindowPtr->showWnd();
			break;
		}
	    EASYUICONTEXT->openActivity("musicActivity");
	}
		break;
	case 7:{
	    LOGD(" ButtonClick VideoButton !!!\n");
		if (lk::is_connected()) {
			mlinkTipsWindowPtr->showWnd();
			break;
		}
	    EASYUICONTEXT->openActivity("videoActivity");
	}
		break;
	case 8: {
		LOGD(" ButtonClick PhotoAlbum !!!\n");
		if (lk::is_connected()) {
			mlinkTipsWindowPtr->showWnd();
			break;
		}
		EASYUICONTEXT->openActivity("PhotoAlbumActivity");
	}
		break;
	case 9:
	    LOGD(" ButtonClick setting !!!\n");
	    EASYUICONTEXT->openActivity("settingsActivity");
		break;
	case 10:
	    LOGD(" ButtonClick FM !!!\n");
	    EASYUICONTEXT->openActivity("FMemitActivity");
		break;
//	case 11:
//		LOGD(" ButtonClick Tire !!!\n");
//		EASYUICONTEXT->openActivity("tirePressureActivity");
//		break;
//	case 12:
//	    LOGD(" ButtonClick CLButton !!!\n");
//	    open_link_activity(E_LINK_MODE_CARLIFE);
//		break;
	default: break;
	}
}
