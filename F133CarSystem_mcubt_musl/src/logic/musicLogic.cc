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

#include "link/context.h"
#include "media/audio_context.h"
#include "media/media_context.h"
#include "media/media_parser.h"
#include "media/music_player.h"
#include "fy/files.hpp"
#include "media/media_parser.h"
#include "manager/ConfigManager.h"
#include "utils/Loading_icon.hpp"
#include "uart/context.h"
#define DEBUG	//LOGD("--%d-- --%s-- ---DEBUG!!!--", __LINE__, __FILE__);

static storage_type_e _s_select_storage = E_STORAGE_TYPE_SD; //E_STORAGE_TYPE_INVALID;	//选中的文件仓库
static storage_type_e _s_play_storage = E_STORAGE_TYPE_SD; //E_STORAGE_TYPE_INVALID;		//播放的文件仓库

static const char* play_mode_path[] = {"MusicPlayer/cycle_n.png", "MusicPlayer/single_n.png", "MusicPlayer/random_n.png"};
static const char* play_mode_press_path[] = {"MusicPlayer/cycle_p.png", "MusicPlayer/single_p.png", "MusicPlayer/random_p.png"};

static bool is_tracking = false;
static int track_progress = -1;

namespace {
class SeekbarChangeListener: public ZKSeekBar::ISeekBarChangeListener {
public:
	virtual void onProgressChanged(ZKSeekBar *pSeekBar, int progress) {
		track_progress = progress;
		if (media::music_get_play_index() != -1) {
	        char timeStr[10];
//	        int pos = pSeekBar->getProgress();
	        snprintf(timeStr, sizeof(timeStr), "%02d:%02d", progress / 60, progress % 60);
	        mCurPosTextViewPtr->setText(timeStr);
		}
	}
	virtual void onStartTrackingTouch(ZKSeekBar *pSeekBar) {
		is_tracking = true;
	}
	virtual void onStopTrackingTouch(ZKSeekBar *pSeekBar) {
		is_tracking = false;
		if (track_progress >= 0) {
			media::music_seek(track_progress * 1000);
			track_progress = -1;
		}
	}
};

}
static SeekbarChangeListener progressbar;

//旋转加载图标
static zkf::IconRotate iconRotate;

// 解析id3显示
static void refreshMusicInfo() {
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
		(strcmp(info.album, "") == 0) ? malbumTextViewPtr->setTextTr("Unknown") : malbumTextViewPtr->setText(info.album);
	}
	else {
		martistTextViewPtr->setTextTr("Unknown");
		malbumTextViewPtr->setTextTr("Unknown");
	}

	isTrue = media::parse_id3_pic(cur_play_file.c_str(), "/tmp/m.jpg");
	mpicTextViewPtr->setBackgroundPic(isTrue ? "/tmp/m.jpg" : CONFIGMANAGER->getResFilePath("MusicPlayer/icon.png").c_str());
	//mpicTextViewPtr->setPosition(LayoutPosition(166,190,120,120));
	mmusicListViewPtr->refreshListView();
}

static void setDuration() {
    int max = media::music_get_duration() / 1000;
    char timeStr[12] = {0};
    snprintf(timeStr, sizeof(timeStr), "%02d:%02d", max / 60, max % 60);
    mDurationTextViewPtr->setText(timeStr);
    mPlayProgressSeekbarPtr->setMax(max);
}

//按键单选
static void muont_path_button_selected(storage_type_e _s_select_type) {
	ZKButton** pBtnArr[] = {&mSDButtonPtr, &mUSB1ButtonPtr, &mUSB2ButtonPtr};
	for (size_t i=0; i<TABLESIZE(pBtnArr); i++) {
		(*pBtnArr[i])->setSelected(i == (int)_s_select_type);
	}
}

//刷新界面,跳转至当前播放
static void seek_to_current_play() {
    _s_select_storage = _s_play_storage;
    muont_path_button_selected(_s_play_storage);
    int play_index = media::music_get_play_index();
    mmusicListViewPtr->setSelection((play_index > (int)mmusicListViewPtr->getRows()-1) ? play_index-3 : 0);  //小于0时,跳转到0
    mmusicListViewPtr->refreshListView();
}

static void _music_play_status_cb(music_play_status_e status) {
	switch (status) {
	case E_MUSIC_PLAY_STATUS_STARTED:    // 播放开始
		setDuration();
		mPlayProgressSeekbarPtr->setProgress(0);
		refreshMusicInfo();
		mButtonPlayPtr->setSelected(true);
		mtitleTextViewPtr->setTextColor(0xFF00FF00);
		break;

	case E_MUSIC_PLAY_STATUS_RESUME:     // 恢复播放
		mButtonPlayPtr->setSelected(true);
		mtitleTextViewPtr->setTextColor(0xFF00FF00);
		break;

	case E_MUSIC_PLAY_STATUS_PAUSE:      // 暂停播放
		mButtonPlayPtr->setSelected(false);
		mtitleTextViewPtr->setTextColor(0xFFFFFFFF);
		break;

	case E_MUSIC_PLAY_STATUS_STOP:       // 停止播放
		setDuration();
		refreshMusicInfo();
		mButtonPlayPtr->setSelected(false);
		mtitleTextViewPtr->setTextColor(0xFFFFFFFF);
		mmusicWindowPtr->hideWnd();
		break;

	case E_MUSIC_PLAY_STATUS_ERROR:{// 播放错误
		merrorWindowPtr->showWnd();
	}
		break;

	case E_MUSIC_PLAY_STATUS_COMPLETED:  // 播放结束
		break;
	}
}

static void _media_scan_cb(const char *dir, storage_type_e type, bool started) {
	if (started) {
		iconRotate.run();
	} else {
//		iconRotate.requestExit();
		iconRotate.requestExitAndWait();
	}
	mscaningWindowPtr->setVisible(started);
	mmusicListViewPtr->refreshListView();
}

//控件初始化
static void ctrl_init() {
	mtitleTextViewPtr->setLongMode(ZKTextView::E_LONG_MODE_DOTS);
	martistTextViewPtr->setLongMode(ZKTextView::E_LONG_MODE_DOTS);
	malbumTextViewPtr->setLongMode(ZKTextView::E_LONG_MODE_DOTS);
}

/**
 * 注册定时器
 * 填充数组用于注册定时器
 * 注意：id不能重复
 */
static S_ACTIVITY_TIMEER REGISTER_ACTIVITY_TIMER_TAB[] = {
	//{0,  6000}, //定时器id=0, 时间间隔6秒
	{1,  1000},
};

/**
 * 当界面构造时触发
 */
static void onUI_init() {
    //Tips :添加 UI初始化的显示代码到这里,如:mText1Ptr->setText("123");
	ctrl_init();
	iconRotate.SetCtrl(msyncPointerPtr, mscaningWindowPtr);

	mplayModeButtonPtr->setButtonStatusPic(ZK_CONTROL_STATUS_NORMAL, play_mode_path[media::music_get_play_mode()]);
//	mplayModeButtonPtr->setButtonStatusPic(ZK_CONTROL_STATUS_PRESSED, play_mode_press_path[media::music_get_play_mode()]);

	media::music_add_play_status_cb(_music_play_status_cb);
	media::add_scan_cb(_media_scan_cb);
	mPlayProgressSeekbarPtr->setSeekBarChangeListener(&progressbar);
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
	seek_to_current_play();
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
	if (iconRotate.isRunning())
		iconRotate.requestExitAndWait();

	media::remove_scan_cb(_media_scan_cb);
	media::music_remove_play_status_cb(_music_play_status_cb);
	mPlayProgressSeekbarPtr->setSeekBarChangeListener(NULL);
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
	case 1:{
        int curPos = -1;
        if (media::music_is_playing()) {
            curPos = media::music_get_current_position() / 1000;
        }
        else if (media::music_get_play_index() == -1) {
        	curPos = 0;
            mDurationTextViewPtr->setText("00:00");
            mButtonPlayPtr->setSelected(false);
            mtitleTextViewPtr->setTextColor(0xFFFFFF);
        }
        if (curPos >= 0) {
//            char timeStr[10];
//            snprintf(timeStr, sizeof(timeStr), "%02d:%02d", curPos / 60, curPos % 60);
//            mCurPosTextViewPtr->setText(timeStr);
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
static bool onmusicActivityTouchEvent(const MotionEvent &ev) {
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

static bool onButtonClick_SDButton(ZKButton *pButton) {
    LOGD(" ButtonClick SDButton !!!\n");
    _s_select_storage = E_STORAGE_TYPE_SD;
    muont_path_button_selected(_s_select_storage);
    (_s_select_storage == _s_play_storage) ? seek_to_current_play() : mmusicListViewPtr->setSelection(0);
    mmusicListViewPtr->refreshListView();
    return false;
}

static bool onButtonClick_USB1Button(ZKButton *pButton) {
    LOGD(" ButtonClick USB1Button !!!\n");
    _s_select_storage = E_STORAGE_TYPE_USB1;
    muont_path_button_selected(_s_select_storage);
    (_s_select_storage == _s_play_storage) ? seek_to_current_play() : mmusicListViewPtr->setSelection(0);
    mmusicListViewPtr->refreshListView();
    return false;
}

static bool onButtonClick_USB2Button(ZKButton *pButton) {
    LOGD(" ButtonClick USB2Button !!!\n");
    _s_select_storage = E_STORAGE_TYPE_USB2;
    muont_path_button_selected(_s_select_storage);
    (_s_select_storage == _s_play_storage) ? seek_to_current_play() : mmusicListViewPtr->setSelection(0);
    mmusicListViewPtr->refreshListView();
    return false;
}

static int getListItemCount_musicListView(const ZKListView *pListView) {
    //LOGD("getListItemCount_musicListView !\n");
    return media::get_audio_list_size(_s_select_storage);
}

static void obtainListItemData_musicListView(ZKListView *pListView,ZKListView::ZKListItem *pListItem, int index) {
    //LOGD(" obtainListItemData_ musicListView  !!!\n");
	pListItem->setLongMode(ZKTextView::E_LONG_MODE_DOTS);
	ZKListView::ZKListSubItem *SubItemID = pListItem->findSubItemByID(ID_MUSIC_SubItemID);
	SubItemID->setText(index+1);
	std::string file = media::get_audio_file(_s_select_storage, index);
	pListItem->setText(file.substr(file.rfind("/")+1));
	pListItem->setSelected((_s_select_storage == _s_play_storage) ? (index == media::music_get_play_index()) : false);
	SubItemID->setSelected(pListItem->isSelected());
}

static void onListItemClick_musicListView(ZKListView *pListView, int index, int id) {
    LOGD(" onListItemClick_ musicListView  !!!\n");
//    uart::set_sound_channel(SOUND_CHANNEL_ARM);
    mmusicWindowPtr->showWnd();
	if (media::music_get_play_index() == index && _s_select_storage == _s_play_storage) {
	    refreshMusicInfo();
	    setDuration();
	    media::music_resume();
	}
	else {
		_s_play_storage = _s_select_storage;
		media::music_play(_s_play_storage, index);
	}
}

static bool onButtonClick_musicButton(ZKButton *pButton) {
    LOGD(" ButtonClick musicButton !!!\n");
    mmusicWindowPtr->showWnd();
    mButtonPlayPtr->setSelected(media::music_is_playing());
    media::music_is_playing() ?  mtitleTextViewPtr->setTextColor(0xFF00FF00) :  mtitleTextViewPtr->setTextColor(0xFFFFFF);
    if (media::music_get_play_index() != -1) {
    	refreshMusicInfo();
    	setDuration();
    	mPlayProgressSeekbarPtr->setProgress(media::music_get_current_position() / 1000);
    }
    return false;
}
static bool onButtonClick_musicCloseButton(ZKButton *pButton) {
    LOGD(" ButtonClick musicCloseButton !!!\n");
    mmusicWindowPtr->hideWnd();
    return false;
}

static void onProgressChanged_PlayProgressSeekbar(ZKSeekBar *pSeekBar, int progress) {
    //LOGD(" ProgressChanged PlayProgressSeekbar %d !!!\n", progress);
}

static bool onButtonClick_musicListButton(ZKButton *pButton) {
    LOGD(" ButtonClick musicListButton !!!\n");
    seek_to_current_play();
    mmusicListViewPtr->refreshListView();
    mmusicWindowPtr->hideWnd();
    return false;
}

static bool onButtonClick_playModeButton(ZKButton *pButton) {
    LOGD(" ButtonClick playModeButton !!!\n");
    media::music_set_play_mode((media_play_mode_e)((media::music_get_play_mode() + 1) % 3));
    pButton->setButtonStatusPic(ZK_CONTROL_STATUS_NORMAL, play_mode_path[media::music_get_play_mode()]);
    pButton->setButtonStatusPic(ZK_CONTROL_STATUS_PRESSED, play_mode_press_path[media::music_get_play_mode()]);
    return false;
}

static bool onButtonClick_PrevButton(ZKButton *pButton) {
    LOGD(" ButtonClick PrevButton !!!\n");
    media::music_prev(true);
    return false;
}

static bool onButtonClick_ButtonPlay(ZKButton *pButton) {
    LOGD(" ButtonClick ButtonPlay !!!\n");
    if (media::music_get_play_index() == -1) {
    	return false;
    }
    else if (media::music_is_playing()) {
        media::music_pause();
    } else {
    	media::music_resume();
    }
    return false;
}

static bool onButtonClick_NextButton(ZKButton *pButton) {
    LOGD(" ButtonClick NextButton !!!\n");
    media::music_next(true);
    return false;
}

static bool onButtonClick_PlayMode(ZKButton *pButton) {
    LOGD(" ButtonClick PlayMode !!!\n");
    return false;
}
