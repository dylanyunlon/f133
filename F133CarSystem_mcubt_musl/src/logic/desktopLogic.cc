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
#include "utils/BitmapHelper.h"

static bt_cb_t _s_bt_cb;

static void parser() {
	std::string cur_play_file = media::music_get_current_play_file();
	id3_info_t info;
	memset(&info, 0, sizeof(id3_info_t));
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
	martistTextViewPtr->setLongMode(ZKTextView::E_LONG_MODE_SCROLL_CIRCULAR);
}

static void _update_music_info() {
	bt_music_t music_info = bt::get_music_info();
	mtitleTextViewPtr->setText(music_info.title);
	martistTextViewPtr->setText(music_info.artist);
	martistTextViewPtr->setLongMode(ZKTextView::E_LONG_MODE_SCROLL_CIRCULAR);
}

static void _update_music_progress() {
	bt_music_t music_info = bt::get_music_info();

	mPlayProgressSeekbarPtr->setMax(music_info.duration);
	mPlayProgressSeekbarPtr->setProgress(music_info.curpos);
}

static void _bt_music_cb(bt_music_state_e state) {
	if (bt::music_is_playing()) {
		_update_music_info();
		_update_music_progress();
		mtitleTextViewPtr->setLongMode(ZKTextView::E_LONG_MODE_SCROLL_CIRCULAR);
		mtitleTextViewPtr->setTextColor(0xFF00FF40);
		mButtonPlayPtr->setSelected(true);
		sys::setting::set_music_play_dev(E_AUDIO_TYPE_BT_MUSIC);
	} else {
		mtitleTextViewPtr->setLongMode(ZKTextView::E_LONG_MODE_SCROLL_CIRCULAR);
		mtitleTextViewPtr->setTextColor(0xFFFFFFFF);
		mButtonPlayPtr->setSelected(false);
	}
}

static void _music_play_status_cb(music_play_status_e status) {
	switch (status) {
	case E_MUSIC_PLAY_STATUS_STARTED:     	// 播放开始
		mmusicPicTextViewPtr->setBackgroundPic(CONFIGMANAGER->getResFilePath("/HomePage/icon_media_cover_n.png").c_str());
		parser();
		sys::setting::set_music_play_dev(E_AUDIO_TYPE_MUSIC);
		mButtonPlayPtr->setSelected(true);
		mtitleTextViewPtr->setLongMode(ZKTextView::E_LONG_MODE_SCROLL_CIRCULAR);
		mtitleTextViewPtr->setTextColor(0xFF00FF00);
		mPlayProgressSeekbarPtr->setMax(media::music_get_duration() / 1000);
		mPlayProgressSeekbarPtr->setProgress(0);
		break;
	case E_MUSIC_PLAY_STATUS_RESUME:     	// 恢复播放
		parser();
		sys::setting::set_music_play_dev(E_AUDIO_TYPE_MUSIC);
		mPlayProgressSeekbarPtr->setMax(media::music_get_duration() / 1000);
		mButtonPlayPtr->setSelected(true);
		mtitleTextViewPtr->setLongMode(ZKTextView::E_LONG_MODE_SCROLL_CIRCULAR);
		mtitleTextViewPtr->setTextColor(0xFF00FF00);
		break;
	case E_MUSIC_PLAY_STATUS_STOP:       	// 停止播放
		mPlayProgressSeekbarPtr->setMax(media::music_get_duration() / 1000);
		mButtonPlayPtr->setSelected(false);
		mtitleTextViewPtr->setLongMode(ZKTextView::E_LONG_MODE_SCROLL_CIRCULAR);
		mtitleTextViewPtr->setTextColor(0xFFFFFFFF);
		mtitleTextViewPtr->setTextTr("Unknown");
		martistTextViewPtr->setTextTr("Unknown");
		mmusicPicTextViewPtr->setBackgroundPic("/HomePage/icon_media_cover_n.png");
		break;
	case E_MUSIC_PLAY_STATUS_PAUSE:      	// 暂停播放
		mtitleTextViewPtr->setLongMode(ZKTextView::E_LONG_MODE_NONE);
		mButtonPlayPtr->setSelected(false);
		mtitleTextViewPtr->setTextColor(0xFFFFFFFF);
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

static void _bt_add_cb() {
	_s_bt_cb.music_cb = _bt_music_cb;
	bt::add_cb(&_s_bt_cb);
}

static void _bt_remove_cb() {
	bt::remove_cb(&_s_bt_cb);
}

static void Icon_Init() {
	_bt_add_cb();
}

namespace{
class AppSlidePageChangeListener : public ZKSlideWindow::ISlidePageChangeListener {
protected:
	virtual void onSlidePageChange(ZKSlideWindow *pSlideWindow, int page) {
		mStatusRadioGroupPtr->setCheckedID((page == 0) ? ID_DESKTOP_RadioButton0 : ID_DESKTOP_RadioButton1);
	}
};
}
static AppSlidePageChangeListener _s_app_slide_page_change_listener;

static void set_back_pic() {
	bitmap_t *bg_bmp = NULL;
	BitmapHelper::loadBitmapFromFile(bg_bmp, CONFIGMANAGER->getResFilePath("/HomePage/car_home_wallpaper_first.jpg").c_str(), 3);
	mTextViewBgPtr->setBackgroundBmp(bg_bmp);
}
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
		app::hide_topbar();
		EASYUICONTEXT->goBack();
		return;
	}

    set_back_pic();

	media::music_add_play_status_cb(_music_play_status_cb);
	mTextView1Ptr->setTouchPass(true);
	martistTextViewPtr->setTouchPass(true);

	mappSlideWindowPtr->setSlidePageChangeListener(&_s_app_slide_page_change_listener);
	mStatusRadioGroupPtr->setCheckedID(ID_DESKTOP_RadioButton0);
//	mverTextViewPtr->setText(std::string("软件版本: ") + std::string(SOFT_VERSION));
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
//	mDigitalClock2Ptr->setVisible(true);

	if (sys::setting::get_music_play_dev() == E_AUDIO_TYPE_BT_MUSIC) {
		_update_music_info();
		_update_music_progress();
		if (bt::music_is_playing()) {
			mButtonPlayPtr->setSelected(true);
		}
	} else if (sys::setting::get_music_play_dev() == E_AUDIO_TYPE_MUSIC) {
		parser();
		if (media::music_is_playing()) {
			mButtonPlayPtr->setSelected(true);
			curPos = media::music_get_current_position() / 1000;
			mPlayProgressSeekbarPtr->setMax(media::music_get_duration() / 1000);
			mtitleTextViewPtr->setLongMode(ZKTextView::E_LONG_MODE_SCROLL_CIRCULAR);
			mtitleTextViewPtr->setTextColor(0xFF00FF00);
		}
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
	media::music_remove_play_status_cb(_music_play_status_cb);
	mPlayProgressSeekbarPtr->setSeekBarChangeListener(NULL);
	mappSlideWindowPtr->setSlidePageChangeListener(NULL);
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
        if (sys::setting::get_music_play_dev() == E_AUDIO_TYPE_MUSIC) {
            if (media::music_is_playing()) {
                curPos = media::music_get_current_position() / 1000;
            }
            if (curPos >= 0) {
            	mPlayProgressSeekbarPtr->setProgress(curPos);
            }
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
//		LOGD("时刻 = %ld 坐标  x = %d, y = %d", ev.mEventTime, ev.mX, ev.mY);
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
	if (sys::setting::get_music_play_dev() == E_AUDIO_TYPE_MUSIC) {
	    media::music_next(true);
	} else if (sys::setting::get_music_play_dev() == E_AUDIO_TYPE_BT_MUSIC) {
		bt::music_next();
	}
    return false;
}

static bool onButtonClick_ButtonPlay(ZKButton *pButton) {
    LOGD(" ButtonClick ButtonPlay !!!\n");

	if (lk::is_connected()) {
		mlinkTipsWindowPtr->showWnd();
		return false;
	}

	if (sys::setting::get_music_play_dev() == E_AUDIO_TYPE_MUSIC) {
	    if (media::music_get_play_index() == -1) {
	    	return false;
	    } else if (media::music_is_playing()) {
	        media::music_pause();
	    } else {
	    	media::music_resume();
	    }
	} else if (sys::setting::get_music_play_dev() == E_AUDIO_TYPE_BT_MUSIC) {
	    bt::music_is_playing() ? bt::music_pause() : bt::music_play();
	}
    return false;
}

static bool onButtonClick_PrevButton(ZKButton *pButton) {
    LOGD(" ButtonClick PrevButton !!!\n");
	if (lk::is_connected()) {
		mlinkTipsWindowPtr->showWnd();
		return false;
	}
	if (sys::setting::get_music_play_dev() == E_AUDIO_TYPE_MUSIC) {
	    media::music_prev(true);
	} else if (sys::setting::get_music_play_dev() == E_AUDIO_TYPE_BT_MUSIC) {
		bt::music_prev();
	}
    return false;
}

static void onProgressChanged_PlayProgressSeekbar(ZKSeekBar *pSeekBar, int progress) {
    //LOGD(" ProgressChanged PlayProgressSeekbar %d !!!\n", progress);
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

	if (sys::setting::get_music_play_dev() == E_AUDIO_TYPE_MUSIC) {
	    EASYUICONTEXT->openActivity("musicActivity");
	} else if (sys::setting::get_music_play_dev() == E_AUDIO_TYPE_BT_MUSIC) {
	    EASYUICONTEXT->openActivity("btMusicActivity");
	}
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
		break;
	case E_LINK_MODE_AIRPLAY:
		if (net::get_mode() != E_NET_MODE_AP) { net::change_mode(E_NET_MODE_AP); }
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
	if (mode == E_LINK_MODE_AIRPLAY || mode == E_LINK_MODE_LYLINK || mode ==E_LINK_MODE_MIRACAST) {
		if (bt::is_on()) {
			bt::power_off();
		}
	} else {
		if (!bt::is_on()) {
			bt::power_on();
		}
	}
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
		if (lk::is_connected()) {
			mlinkTipsWindowPtr->showWnd();
			break;
		}
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
	    EASYUICONTEXT->openActivity("setshowActivity");
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
static void onProgressChanged_PlayVolSeekBar(ZKSeekBar *pSeekBar, int progress) {
    //LOGD(" ProgressChanged PlayVolSeekBar %d !!!\n", progress);
}
