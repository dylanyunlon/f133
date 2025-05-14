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
#include "media/media_context.h"
#include "media/audio_context.h"
#include "media/music_player.h"
#include "media/media_parser.h"
#include "os/MountMonitor.h"
#include "utils/Loading_icon.hpp"
#include "uart/context.h"
#include "fy/strings.hpp"
#include <sys/sysinfo.h>
#include "net/context.h"

#include "fy/os.hpp"

#define LOOP_TIMER            1
#define DELAY_CHECK_TIMER     2

#define VIDEO_ERROR_TIMER  3
#define VIDEO_AUTO_NEXT_FORMAT	      4
#define VIDEO_AUTO_NEXT_ERROR		  5
#define DROP_CACHES_TIMER             6

static storage_type_e _s_select_storage = E_STORAGE_TYPE_SD;		//选中的文件仓库
static storage_type_e _s_play_storage = E_STORAGE_TYPE_SD;			//播放的文件仓库

static int _s_play_index = -1;
static media_play_mode_e _s_play_mode = E_MEDIA_PLAY_MODE_CYCLE;	//播放模式

static std::string play_mode_path[] = {"MusicPlayer/cycle_n.png", "MusicPlayer/single_n.png", "MusicPlayer/random_n.png"};
static std::string play_mode_press_path[] = {"MusicPlayer/cycle_p.png", "MusicPlayer/single_p.png", "MusicPlayer/random_p.png"};

static bool is_pause = false;
static bool is_tracking_ = false;
static int track_progress_;
static bool is_video_activity_show = false;		//视频界面是否显示
static bool is_play_error = false;				//视频是否播放出错

static void unregister_play_error_timer() {
	if (is_play_error) {
		mActivityPtr->unregisterUserTimer(VIDEO_ERROR_TIMER);
		is_play_error = false;
	}
}

namespace {
class SeekbarChangeListener: public ZKSeekBar::ISeekBarChangeListener {
public:
	virtual void onProgressChanged(ZKSeekBar *pSeekBar, int progress) {
		char timeStr[10];
		sprintf(timeStr, "%02d:%02d", progress / 60, progress % 60);
		mCurPosTextViewPtr->setText(timeStr);
	}
	virtual void onStartTrackingTouch(ZKSeekBar *pSeekBar) {
		is_tracking_ = true;
	}
	virtual void onStopTrackingTouch(ZKSeekBar *pSeekBar) {
		if (is_tracking_) {
			mVideoviewTTPtr->seekTo(pSeekBar->getProgress() * 1000);
		}
		is_tracking_ = false;
	}
};

class VideoMountListener : public MountMonitor::IMountListener {
protected:
	virtual void notify(int what, int status, const char *msg) {
		switch (status) {
		case MountMonitor::E_MOUNT_STATUS_UNMOUNTING: {
			std::string play_file = media::get_video_file(_s_play_storage, _s_play_index);
			std::string root_path = std::string(msg) + "/";
			if (root_path.compare(0, root_path.size(), play_file, 0, root_path.size()) == 0) {
				LOGE("[video] need to stop\n");
				mVideoviewTTPtr->stop();
				unregister_play_error_timer();
				mvideoPlayWindowPtr->hideWnd();
				mPlayButtonPtr->setSelected(false);
				mWindowMaskPtr->setVisible(true);
				_s_play_index = -1;
			}
			mvideoListViewPtr->refreshListView();
		}
			break;
		}
	}
};

}
static SeekbarChangeListener progressbar;
static VideoMountListener videoMountListener;
//旋转加载图标
static zkf::IconRotate iconRotate;

static void _media_scan_cb(const char *dir, storage_type_e type, bool started) {
	if (started) {
		iconRotate.run();
	} else {
//		iconRotate.requestExit();
		iconRotate.requestExitAndWait();
	}
	mscaningWindowPtr->setVisible(started);
	mvideoListViewPtr->refreshListView();
}
static bool video_ctrl_play(const char* file, int msec = 0) {
	video_info_t info;
	std::string filestr(file);
//	mtitleTextViewPtr->setText(filestr.substr(filestr.rfind("/")+1));
	bool ret = media::parse_video_info(file, &info);
	if (!ret) {		// 解析失败
		LOGD("Video parsing failure!   %s\n", file);
		goto FAIL;
	} else if (info.width >= 2560 || info.height >= 1440 || info.bit_rate >= 5000*1000) {		// 分辨率过大
		LOGD("max support: 1920*1080, current file: %d*%d, bit_rate:%d, Excessive video resolution!\n", info.width, info.height, info.bit_rate);
		goto FAIL;
	} else if (fy::strings::endswith(file, "wmv") || fy::strings::endswith(file, "avi")) { 	// 不支持文件格式
		mVideoviewTTPtr->pause();
		mPlayButtonPtr->setSelected(false);
		mTipWindowPtr->showWnd();
		merrorWindowPtr->showWnd();
		mActivityPtr->registerUserTimer(VIDEO_AUTO_NEXT_FORMAT, 2500);
		is_play_error = true;
		return false;
	} else {
		if(!mvideoPlayWindowPtr->isWndShow()) {
			mvideoPlayWindowPtr->showWnd();
		}
		mVideoviewTTPtr->setVisible(true);
//		mDisplayButton1Ptr->setVisible(true);
		mVideoviewTTPtr->play(file, msec);
		return true;
	}

	LOGD("--其他播放情况--!");
	return false;

FAIL:
	mVideoviewTTPtr->pause();
	mPlayButtonPtr->setSelected(false);
	merrorWindowPtr->showWnd();
	mActivityPtr->registerUserTimer(VIDEO_AUTO_NEXT_ERROR, 2500);
	is_play_error = true;
	return false;
}

static bool video_next(bool isConvert = false) {		//isConvert:单曲模式下是否进行顺序播放		应用场景:1.单曲手动切, 2.单曲出错
	mPlayProgressSeekbarPtr->setProgress(0);
	int list_size = media::get_video_list_size(_s_play_storage);
	int ret = false;
	LOGD("[video] video_next start");
	if (!is_video_activity_show || 0 == list_size) {
		LOGD("[video] video is hide or list size==0");
		return ret;
	}
	LOGD("[video] video_next mode: %d\n", _s_play_mode);
	if (_s_play_mode == E_MEDIA_PLAY_MODE_CYCLE
			|| (isConvert && _s_play_mode == E_MEDIA_PLAY_MODE_SINGLE)) {
		_s_play_index = (_s_play_index + 1) % list_size;
		 ret = video_ctrl_play(media::get_video_file(_s_play_storage, _s_play_index).c_str());
	} else if (_s_play_mode == E_MEDIA_PLAY_MODE_RANDOM) {
		int rand_index = rand() % list_size;
		_s_play_index = (rand_index != _s_play_index) ? rand_index : ((_s_play_index + 1) % list_size);
		ret = video_ctrl_play(media::get_video_file(_s_play_storage, _s_play_index).c_str());
	} else if (_s_play_mode == E_MEDIA_PLAY_MODE_SINGLE) {
		ret= video_ctrl_play(media::get_video_file(_s_play_storage, _s_play_index).c_str());
	}
	return ret;
}

static void video_prev(bool isConvert = false) {
	mPlayProgressSeekbarPtr->setProgress(0);
	int list_size = media::get_video_list_size(_s_play_storage);
	if (0 == list_size) {
		return;
	}
	if (_s_play_mode == E_MEDIA_PLAY_MODE_CYCLE
			|| (isConvert && _s_play_mode == E_MEDIA_PLAY_MODE_SINGLE)) {
		_s_play_index = (_s_play_index - 1 + list_size) % list_size;
		video_ctrl_play(media::get_video_file(_s_play_storage, _s_play_index).c_str());
	} else if (_s_play_mode == E_MEDIA_PLAY_MODE_RANDOM || _s_play_mode == E_MEDIA_PLAY_MODE_SINGLE) {
		video_next(isConvert);
	}
}

//按键单选
static void mount_path_button_selected(storage_type_e _s_select_type) {
	ZKButton** pBtnArr[] = {&mSDButtonPtr, &mUSB1ButtonPtr, &mUSB2ButtonPtr};
	for (size_t i=0; i<TABLESIZE(pBtnArr); i++) {
		(*pBtnArr[i])->setSelected(i == (int)_s_select_type);
	}
}

//刷新界面,跳转至当前播放
static void seek_to_current_play() {
    _s_select_storage = _s_play_storage;
    mount_path_button_selected(_s_play_storage);
    mvideoListViewPtr->setSelection((_s_play_index > (int)mvideoListViewPtr->getRows()-1) ? _s_play_index-3 : 0);  //小于0时,跳转到0
    mvideoListViewPtr->refreshListView();
}

//控件初始化
static void ctrl_init() {
	mtittleTextViewPtr->setLongMode(ZKTextView::E_LONG_MODE_SCROLL_CIRCULAR);
}

static bool _show_sys_info(unsigned long *freeram) {
	struct sysinfo info;
	int ret = 0;
	ret = sysinfo(&info);
	if(ret != 0) {
		return false;
	}
	*freeram = info.freeram;
	return true;
}

/**
 * 注册定时器
 * 填充数组用于注册定时器
 * 注意：id不能重复
 */
static S_ACTIVITY_TIMEER REGISTER_ACTIVITY_TIMER_TAB[] = {
	//{0,  6000}, //定时器id=0, 时间间隔6秒
	{LOOP_TIMER,  1000},
	{DROP_CACHES_TIMER, 3000},
};

/**
 * 当界面构造时触发
 */
static void onUI_init(){
    //Tips :添加 UI初始化的显示代码到这里,如:mText1Ptr->setText("123");
	ctrl_init();
	iconRotate.SetCtrl(msyncPointerPtr, mscaningWindowPtr);

	mVideoviewTTPtr->setVolume(audio::get_system_vol());
	mWindowMaskPtr->setPosition(mVideoviewTTPtr->getPosition());

	mount_path_button_selected(_s_select_storage);
	mplayModeButtonPtr->setButtonStatusPic(ZK_CONTROL_STATUS_NORMAL, play_mode_path[_s_play_mode].c_str());
	mplayModeButtonPtr->setButtonStatusPic(ZK_CONTROL_STATUS_PRESSED, play_mode_press_path[_s_play_mode].c_str());

	media::add_scan_cb(_media_scan_cb);
	mPlayProgressSeekbarPtr->setSeekBarChangeListener(&progressbar);
	MOUNTMONITOR->addMountListener(&videoMountListener);

	is_pause = false;
	track_progress_ = 0;
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
	LOGD("[video] onUI_show");
	is_video_activity_show = true;
	// 推迟检测播放，处理倒车界面未完全退出，视频播放异常问题
	mActivityPtr->registerUserTimer(DELAY_CHECK_TIMER, 0);
}

/*
 * 当界面隐藏时触发
 */
static void onUI_hide() {
	LOGD("[video] onUI_hide+++\n");
	is_video_activity_show = false;

	if (mvideoPlayWindowPtr->isWndShow()) {
		is_pause = !mVideoviewTTPtr->isPlaying();
		track_progress_ = mVideoviewTTPtr->getCurrentPosition();
	} else {
		is_pause = false;
	}

	mVideoviewTTPtr->stop();
    mPlayButtonPtr->setSelected(false);
    mWindowMaskPtr->setVisible(true);
    LOGD("[video] onUI_hide---\n");
}

/*
 * 当界面完全退出时触发
 */
static void onUI_quit() {
	if (iconRotate.isRunning())
		iconRotate.requestExitAndWait();

	media::remove_scan_cb(_media_scan_cb);
	mPlayProgressSeekbarPtr->setSeekBarChangeListener(NULL);
	MOUNTMONITOR->removeMountListener(&videoMountListener);
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
	case LOOP_TIMER: {
		int curPos = -1;
		if (mVideoviewTTPtr->isPlaying()) {
			curPos = mVideoviewTTPtr->getCurrentPosition() / 1000;
//			LOGD("curPos = %d", curPos);
		}
		if (curPos >= 0) {
			if (!is_tracking_) {
				mPlayProgressSeekbarPtr->setProgress(curPos);
			}
			if (curPos > mPlayProgressSeekbarPtr->getMax() && is_video_activity_show) {
				video_next();
			}
		}
	}
		break;
	case DELAY_CHECK_TIMER:
		if (is_video_activity_show && mvideoPlayWindowPtr->isWndShow()) {
			LOGD("[video] delay check timer\n");
			if (_s_play_index != -1) {
				mVideoviewTTPtr->setVolume(0.0);
				std::string file = media::get_video_file(_s_select_storage, _s_play_index);
				video_ctrl_play(file.c_str(), track_progress_);
//				if (is_pause) {
//					mVideoviewTTPtr->pause();
//					mVideoviewTTPtr->setVolume(audio::get_system_vol());
//					mPlayButtonPtr->setSelected(false);
//				}
			}
		}

		net::change_mode(E_NET_MODE_NULL, true);
		lk::stop_lylink();
		fy::pkill("/res/bin/mdnsd");
		fy::drop_caches();

		// 只执行一次，停掉该定时器
		return false;
	case VIDEO_ERROR_TIMER:
		LOGD("--%d-- --%s-- 执行定时器 id = %d \n", __LINE__, __FILE__, VIDEO_ERROR_TIMER);
		is_play_error = false;
//		video_next(true);
//		return false;	// 执行一次
		return !(video_next(true));
	case VIDEO_AUTO_NEXT_FORMAT:
	case VIDEO_AUTO_NEXT_ERROR:
		LOGD("[video] VIDEO_AUTO_NEXT_ERROR");
		mTipWindowPtr->hideWnd();
		merrorWindowPtr->hideWnd();
		is_play_error = false;
//		video_next(true);
//		mvideoListViewPtr->refreshListView();
		return !(video_next(true));
	case DROP_CACHES_TIMER: {
		unsigned long freeram = 0;
		bool ret = _show_sys_info(&freeram);
		if(ret) {
			freeram >>= 10;
			if(freeram < 2 * 1024) {
				LOGD("[video] -----------Current MemFree: %ldKB < 2M---------------", freeram);
				fy::drop_caches();
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
static bool onvideoActivityTouchEvent(const MotionEvent &ev) {
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

static bool onButtonClick_SDButton(ZKButton *pButton) {
    LOGD(" ButtonClick SDButton !!!\n");
    _s_select_storage = E_STORAGE_TYPE_SD;
    mount_path_button_selected(_s_select_storage);
    (_s_select_storage == _s_play_storage) ? seek_to_current_play() : mvideoListViewPtr->setSelection(0);	//小于0时,跳转到0
    mvideoListViewPtr->refreshListView();
    return false;
}

static bool onButtonClick_USB1Button(ZKButton *pButton) {
    LOGD(" ButtonClick USB1Button !!!\n");
    _s_select_storage = E_STORAGE_TYPE_USB1;
    mount_path_button_selected(_s_select_storage);
    (_s_select_storage == _s_play_storage) ? seek_to_current_play() : mvideoListViewPtr->setSelection(0);
    mvideoListViewPtr->refreshListView();
    return false;
}

static bool onButtonClick_USB2Button(ZKButton *pButton) {
    LOGD(" ButtonClick USB2Button !!!\n");
    _s_select_storage = E_STORAGE_TYPE_USB2;
    mount_path_button_selected(_s_select_storage);
    (_s_select_storage == _s_play_storage) ? seek_to_current_play() : mvideoListViewPtr->setSelection(0);
    mvideoListViewPtr->refreshListView();
    return false;
}

static int getListItemCount_videoListView(const ZKListView *pListView) {
    //LOGD("getListItemCount_videoListView !\n");
    return media::get_video_list_size(_s_select_storage);
}

static void obtainListItemData_videoListView(ZKListView *pListView,ZKListView::ZKListItem *pListItem, int index) {
    //LOGD(" obtainListItemData_ videoListView  !!!\n");
	pListItem->setLongMode(ZKTextView::E_LONG_MODE_DOTS);
	ZKListView::ZKListSubItem *SubItemID = pListItem->findSubItemByID(ID_VIDEO_SubItemID);
	SubItemID->setText(index+1);
	std::string file = media::get_video_file(_s_select_storage, index);
	pListItem->setText(file.substr(file.rfind("/")+1));
	pListItem->setSelected((_s_select_storage == _s_play_storage) ? (index == _s_play_index) : false);
	SubItemID->setSelected(pListItem->isSelected());
}

static void onListItemClick_videoListView(ZKListView *pListView, int index, int id) {
    //LOGD(" onListItemClick_ videoListView  !!!\n");
	uart::set_sound_channel(SOUND_CHANNEL_ARM);
	std::string file = media::get_video_file(_s_select_storage, index);
	if (_s_play_index == index && _s_select_storage == _s_play_storage) {
		video_ctrl_play(file.c_str(), track_progress_);
	} else {
		_s_play_index = index;
		_s_play_storage = _s_select_storage;
	    video_ctrl_play(file.c_str());
	}
    mtittleTextViewPtr->setText(file.substr(file.rfind("/")+1));
//    mvideoPlayWindowPtr->showWnd();
}

static bool onButtonClick_sys_back(ZKButton *pButton) {
    LOGD(" ButtonClick sys_back !!!\n");
    return false;
}

static void onVideoViewPlayerMessageListener_VideoviewTT(ZKVideoView *pVideoView, int msg) {
	switch (msg) {
	case ZKVideoView::E_MSGTYPE_VIDEO_PLAY_STARTED: {
    	if (media::music_is_playing()) {
    		media::music_pause();
    	}

    	std::string file = media::get_video_file(_s_select_storage, _s_play_index);
    	mtittleTextViewPtr->setText(file.substr(file.rfind("/")+1));
    	audio::change_audio_type(E_AUDIO_TYPE_VIDEO);
        int max = pVideoView->getDuration() / 1000;
        char timeStr[12] = {0};
        snprintf(timeStr, sizeof(timeStr), "%02d:%02d", max / 60, max % 60);
        mDurationTextViewPtr->setText(timeStr);
        mPlayProgressSeekbarPtr->setMax(max);
        mPlayButtonPtr->setSelected(true);

		if (mWindowMaskPtr->isVisible()) {
			if (track_progress_ > 0) {
				// 有seek动作，等待画面跳转完成
				int pos = pVideoView->getCurrentPosition();
				for (int i = 0; i < 100; ++i) {
					if (pVideoView->getCurrentPosition() - pos > 16) {
						break;
					}
					DELAY(10);
				}
			}

			// 隐藏遮罩窗口
			mWindowMaskPtr->setVisible(false);
		}
    }
		break;
	case ZKVideoView::E_MSGTYPE_VIDEO_PLAY_COMPLETED:
		if (!is_play_error) {
			LOGD("播放完成");
			video_next();
			mvideoListViewPtr->refreshListView();
		}
		break;
	case ZKVideoView::E_MSGTYPE_VIDEO_PLAY_ERROR:
		LOGE("play error");
		is_play_error = true;
		LOGD("--%d-- --%s-- 注册定时器 id = %d\n", __LINE__, __FILE__, VIDEO_ERROR_TIMER);
		mActivityPtr->registerUserTimer(VIDEO_ERROR_TIMER, 3000);
		break;
	}
}

static bool onButtonClick_VideoCtrlButton(ZKButton *pButton) {
    LOGD(" ButtonClick VideoCtrlButton !!!\n");
    mVideoCtrlWindowPtr->showWnd();
    return false;
}

static bool onButtonClick_backButton(ZKButton *pButton) {
    LOGD(" ButtonClick backButton !!!\n");
    track_progress_ = mVideoviewTTPtr->getCurrentPosition();
    mVideoviewTTPtr->stop();
    mPlayButtonPtr->setSelected(false);
    mWindowMaskPtr->setVisible(true);
    mvideoPlayWindowPtr->hideWnd();
    mVideoCtrlWindowPtr->hideWnd();
    return false;
}

static void onProgressChanged_PlayProgressSeekbar(ZKSeekBar *pSeekBar, int progress) {
    //LOGD(" ProgressChanged PlayProgressSeekbar %d !!!\n", progress);
}

static bool onButtonClick_videoListButton(ZKButton *pButton) {
    LOGD(" ButtonClick videoListButton !!!\n");
    track_progress_ = mVideoviewTTPtr->getCurrentPosition();
    mVideoviewTTPtr->stop();
    mPlayButtonPtr->setSelected(false);
    mWindowMaskPtr->setVisible(true);
    seek_to_current_play();
    mvideoListViewPtr->refreshListView();
    mvideoPlayWindowPtr->hideWnd();
    return false;
}

static bool onButtonClick_playModeButton(ZKButton *pButton) {
    LOGD(" ButtonClick playModeButton !!!\n");
    _s_play_mode =  (media_play_mode_e)((_s_play_mode + 1) % 3);
    pButton->setButtonStatusPic(ZK_CONTROL_STATUS_NORMAL, play_mode_path[_s_play_mode].c_str());
    pButton->setButtonStatusPic(ZK_CONTROL_STATUS_PRESSED, play_mode_press_path[_s_play_mode].c_str());
    return false;
}

static bool onButtonClick_PrevButton(ZKButton *pButton) {
    LOGD(" ButtonClick PrevButton !!!\n");
    video_prev(true);
    return false;
}

static bool onButtonClick_PlayButton(ZKButton *pButton) {
    LOGD(" ButtonClick PlayButton !!!\n");
//    if (bt::is_calling()) {
//    	return false;
//    }

    if (-1 == _s_play_index) {
    	return false;
    } else if (mVideoviewTTPtr->isPlaying()) {
        mVideoviewTTPtr->pause();
        pButton->setSelected(false);
    } else {
        mVideoviewTTPtr->resume();
        audio::change_audio_type(E_AUDIO_TYPE_VIDEO);
        pButton->setSelected(true);
    }
    return false;
}

static bool onButtonClick_NextButton(ZKButton *pButton) {
    LOGD(" ButtonClick NextButton !!!\n");
    video_next(true);
    return false;
}

static bool onButtonClick_hideButton(ZKButton *pButton) {
    LOGD(" ButtonClick hideButton !!!\n");
    mVideoCtrlWindowPtr->hideWnd();
    return false;
}
