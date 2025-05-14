/***********************************************
/gen auto by zuitools
***********************************************/
#include "videoActivity.h"

/*TAG:GlobalVariable全局变量*/
static ZKTextView* mTipTextViewPtr;
static ZKWindow* mTipWindowPtr;
static ZKTextView* merrorTextViewPtr;
static ZKWindow* merrorWindowPtr;
static ZKButton* mNextButtonPtr;
static ZKButton* mPlayButtonPtr;
static ZKButton* mPrevButtonPtr;
static ZKButton* mplayModeButtonPtr;
static ZKButton* mvideoListButtonPtr;
static ZKTextView* mDurationTextViewPtr;
static ZKSeekBar* mPlayProgressSeekbarPtr;
static ZKTextView* mCurPosTextViewPtr;
static ZKButton* mbackButtonPtr;
static ZKButton* mhideButtonPtr;
static ZKTextView* mbagTextViewPtr;
static ZKTextView* mtittleTextViewPtr;
static ZKWindow* mVideoCtrlWindowPtr;
static ZKButton* mVideoCtrlButtonPtr;
static ZKWindow* mWindowMaskPtr;
static ZKVideoView* mVideoviewTTPtr;
static ZKWindow* mvideoPlayWindowPtr;
static ZKPointer* msyncPointerPtr;
static ZKTextView* mscaningTextViewPtr;
static ZKWindow* mscaningWindowPtr;
static ZKButton* msys_backPtr;
static ZKButton* mUSB2ButtonPtr;
static ZKListView* mvideoListViewPtr;
static ZKButton* mUSB1ButtonPtr;
static ZKButton* mSDButtonPtr;
static ZKWindow* mvideoListWindowPtr;
static videoActivity* mActivityPtr;

/*register activity*/
REGISTER_ACTIVITY(videoActivity);

typedef struct {
	int id; // 定时器ID ， 不能重复
	int time; // 定时器  时间间隔  单位 毫秒
}S_ACTIVITY_TIMEER;

#include "logic/videoLogic.cc"

/***********/
typedef struct {
    int id;
    const char *pApp;
} SAppInfo;

/**
 *点击跳转window
 */
static SAppInfo sAppInfoTab[] = {
//  { ID_MAIN_TEXT, "TextViewActivity" },
};

/***************/
typedef bool (*ButtonCallback)(ZKButton *pButton);
/**
 * button onclick表
 */
typedef struct {
    int id;
    ButtonCallback callback;
}S_ButtonCallback;

/*TAG:ButtonCallbackTab按键映射表*/
static S_ButtonCallback sButtonCallbackTab[] = {
    ID_VIDEO_NextButton, onButtonClick_NextButton,
    ID_VIDEO_PlayButton, onButtonClick_PlayButton,
    ID_VIDEO_PrevButton, onButtonClick_PrevButton,
    ID_VIDEO_playModeButton, onButtonClick_playModeButton,
    ID_VIDEO_videoListButton, onButtonClick_videoListButton,
    ID_VIDEO_backButton, onButtonClick_backButton,
    ID_VIDEO_hideButton, onButtonClick_hideButton,
    ID_VIDEO_VideoCtrlButton, onButtonClick_VideoCtrlButton,
    ID_VIDEO_sys_back, onButtonClick_sys_back,
    ID_VIDEO_USB2Button, onButtonClick_USB2Button,
    ID_VIDEO_USB1Button, onButtonClick_USB1Button,
    ID_VIDEO_SDButton, onButtonClick_SDButton,
};
/***************/


typedef void (*SeekBarCallback)(ZKSeekBar *pSeekBar, int progress);
typedef struct {
    int id;
    SeekBarCallback callback;
}S_ZKSeekBarCallback;
/*TAG:SeekBarCallbackTab*/
static S_ZKSeekBarCallback SZKSeekBarCallbackTab[] = {
    ID_VIDEO_PlayProgressSeekbar, onProgressChanged_PlayProgressSeekbar,
};


typedef int (*ListViewGetItemCountCallback)(const ZKListView *pListView);
typedef void (*ListViewobtainListItemDataCallback)(ZKListView *pListView,ZKListView::ZKListItem *pListItem, int index);
typedef void (*ListViewonItemClickCallback)(ZKListView *pListView, int index, int id);
typedef struct {
    int id;
    ListViewGetItemCountCallback getListItemCountCallback;
    ListViewobtainListItemDataCallback obtainListItemDataCallback;
    ListViewonItemClickCallback onItemClickCallback;
}S_ListViewFunctionsCallback;
/*TAG:ListViewFunctionsCallback*/
static S_ListViewFunctionsCallback SListViewFunctionsCallbackTab[] = {
    ID_VIDEO_videoListView, getListItemCount_videoListView, obtainListItemData_videoListView, onListItemClick_videoListView,
};


typedef void (*SlideWindowItemClickCallback)(ZKSlideWindow *pSlideWindow, int index);
typedef struct {
    int id;
    SlideWindowItemClickCallback onSlideItemClickCallback;
}S_SlideWindowItemClickCallback;
/*TAG:SlideWindowFunctionsCallbackTab*/
static S_SlideWindowItemClickCallback SSlideWindowItemClickCallbackTab[] = {
};


typedef void (*EditTextInputCallback)(const std::string &text);
typedef struct {
    int id;
    EditTextInputCallback onEditTextChangedCallback;
}S_EditTextInputCallback;
/*TAG:EditTextInputCallback*/
static S_EditTextInputCallback SEditTextInputCallbackTab[] = {
};

typedef void (*VideoViewCallback)(ZKVideoView *pVideoView, int msg);
typedef struct {
    int id; //VideoView ID
    bool loop; // 是否是轮播类型
    int defaultvolume;//轮播类型时,默认视频音量
    VideoViewCallback onVideoViewCallback;
}S_VideoViewCallback;
/*TAG:VideoViewCallback*/
static S_VideoViewCallback SVideoViewCallbackTab[] = {
    ID_VIDEO_VideoviewTT, false, 5, onVideoViewPlayerMessageListener_VideoviewTT,
};


videoActivity::videoActivity() {
	//todo add init code here
	mVideoLoopIndex = -1;
	mVideoLoopErrorCount = 0;
}

videoActivity::~videoActivity() {
  //todo add init file here
  // 退出应用时需要反注册
    EASYUICONTEXT->unregisterGlobalTouchListener(this);
    unregisterProtocolDataUpdateListener(onProtocolDataUpdate);
    onUI_quit();
    mActivityPtr = NULL;
    mTipTextViewPtr = NULL;
    mTipWindowPtr = NULL;
    merrorTextViewPtr = NULL;
    merrorWindowPtr = NULL;
    mNextButtonPtr = NULL;
    mPlayButtonPtr = NULL;
    mPrevButtonPtr = NULL;
    mplayModeButtonPtr = NULL;
    mvideoListButtonPtr = NULL;
    mDurationTextViewPtr = NULL;
    mPlayProgressSeekbarPtr = NULL;
    mCurPosTextViewPtr = NULL;
    mbackButtonPtr = NULL;
    mhideButtonPtr = NULL;
    mbagTextViewPtr = NULL;
    mtittleTextViewPtr = NULL;
    mVideoCtrlWindowPtr = NULL;
    mVideoCtrlButtonPtr = NULL;
    mWindowMaskPtr = NULL;
    mVideoviewTTPtr = NULL;
    mvideoPlayWindowPtr = NULL;
    msyncPointerPtr = NULL;
    mscaningTextViewPtr = NULL;
    mscaningWindowPtr = NULL;
    msys_backPtr = NULL;
    mUSB2ButtonPtr = NULL;
    mvideoListViewPtr = NULL;
    mUSB1ButtonPtr = NULL;
    mSDButtonPtr = NULL;
    mvideoListWindowPtr = NULL;
}

const char* videoActivity::getAppName() const{
	return "video.ftu";
}

//TAG:onCreate
void videoActivity::onCreate() {
	Activity::onCreate();
    mTipTextViewPtr = (ZKTextView*)findControlByID(ID_VIDEO_TipTextView);
    mTipWindowPtr = (ZKWindow*)findControlByID(ID_VIDEO_TipWindow);
    merrorTextViewPtr = (ZKTextView*)findControlByID(ID_VIDEO_errorTextView);
    merrorWindowPtr = (ZKWindow*)findControlByID(ID_VIDEO_errorWindow);
    mNextButtonPtr = (ZKButton*)findControlByID(ID_VIDEO_NextButton);
    mPlayButtonPtr = (ZKButton*)findControlByID(ID_VIDEO_PlayButton);
    mPrevButtonPtr = (ZKButton*)findControlByID(ID_VIDEO_PrevButton);
    mplayModeButtonPtr = (ZKButton*)findControlByID(ID_VIDEO_playModeButton);
    mvideoListButtonPtr = (ZKButton*)findControlByID(ID_VIDEO_videoListButton);
    mDurationTextViewPtr = (ZKTextView*)findControlByID(ID_VIDEO_DurationTextView);
    mPlayProgressSeekbarPtr = (ZKSeekBar*)findControlByID(ID_VIDEO_PlayProgressSeekbar);if(mPlayProgressSeekbarPtr!= NULL){mPlayProgressSeekbarPtr->setSeekBarChangeListener(this);}
    mCurPosTextViewPtr = (ZKTextView*)findControlByID(ID_VIDEO_CurPosTextView);
    mbackButtonPtr = (ZKButton*)findControlByID(ID_VIDEO_backButton);
    mhideButtonPtr = (ZKButton*)findControlByID(ID_VIDEO_hideButton);
    mbagTextViewPtr = (ZKTextView*)findControlByID(ID_VIDEO_bagTextView);
    mtittleTextViewPtr = (ZKTextView*)findControlByID(ID_VIDEO_tittleTextView);
    mVideoCtrlWindowPtr = (ZKWindow*)findControlByID(ID_VIDEO_VideoCtrlWindow);
    mVideoCtrlButtonPtr = (ZKButton*)findControlByID(ID_VIDEO_VideoCtrlButton);
    mWindowMaskPtr = (ZKWindow*)findControlByID(ID_VIDEO_WindowMask);
    mVideoviewTTPtr = (ZKVideoView*)findControlByID(ID_VIDEO_VideoviewTT);if(mVideoviewTTPtr!= NULL){mVideoviewTTPtr->setVideoPlayerMessageListener(this);}
    mvideoPlayWindowPtr = (ZKWindow*)findControlByID(ID_VIDEO_videoPlayWindow);
    msyncPointerPtr = (ZKPointer*)findControlByID(ID_VIDEO_syncPointer);
    mscaningTextViewPtr = (ZKTextView*)findControlByID(ID_VIDEO_scaningTextView);
    mscaningWindowPtr = (ZKWindow*)findControlByID(ID_VIDEO_scaningWindow);
    msys_backPtr = (ZKButton*)findControlByID(ID_VIDEO_sys_back);
    mUSB2ButtonPtr = (ZKButton*)findControlByID(ID_VIDEO_USB2Button);
    mvideoListViewPtr = (ZKListView*)findControlByID(ID_VIDEO_videoListView);if(mvideoListViewPtr!= NULL){mvideoListViewPtr->setListAdapter(this);mvideoListViewPtr->setItemClickListener(this);}
    mUSB1ButtonPtr = (ZKButton*)findControlByID(ID_VIDEO_USB1Button);
    mSDButtonPtr = (ZKButton*)findControlByID(ID_VIDEO_SDButton);
    mvideoListWindowPtr = (ZKWindow*)findControlByID(ID_VIDEO_videoListWindow);
	mActivityPtr = this;
	onUI_init();
  registerProtocolDataUpdateListener(onProtocolDataUpdate);
  rigesterActivityTimer();
}

void videoActivity::onClick(ZKBase *pBase) {
	//TODO: add widget onClik code 
    int buttonTablen = sizeof(sButtonCallbackTab) / sizeof(S_ButtonCallback);
    for (int i = 0; i < buttonTablen; ++i) {
        if (sButtonCallbackTab[i].id == pBase->getID()) {
            if (sButtonCallbackTab[i].callback((ZKButton*)pBase)) {
            	return;
            }
            break;
        }
    }


    int len = sizeof(sAppInfoTab) / sizeof(sAppInfoTab[0]);
    for (int i = 0; i < len; ++i) {
        if (sAppInfoTab[i].id == pBase->getID()) {
            EASYUICONTEXT->openActivity(sAppInfoTab[i].pApp);
            return;
        }
    }

	Activity::onClick(pBase);
}

void videoActivity::onResume() {
	Activity::onResume();
	EASYUICONTEXT->registerGlobalTouchListener(this);
	startVideoLoopPlayback();
	onUI_show();
}

void videoActivity::onPause() {
	Activity::onPause();
	EASYUICONTEXT->unregisterGlobalTouchListener(this);
	stopVideoLoopPlayback();
	onUI_hide();
}

void videoActivity::onIntent(const Intent *intentPtr) {
	Activity::onIntent(intentPtr);
	onUI_intent(intentPtr);
}

bool videoActivity::onTimer(int id) {
	return onUI_Timer(id);
}

void videoActivity::onProgressChanged(ZKSeekBar *pSeekBar, int progress){

    int seekBarTablen = sizeof(SZKSeekBarCallbackTab) / sizeof(S_ZKSeekBarCallback);
    for (int i = 0; i < seekBarTablen; ++i) {
        if (SZKSeekBarCallbackTab[i].id == pSeekBar->getID()) {
            SZKSeekBarCallbackTab[i].callback(pSeekBar, progress);
            break;
        }
    }
}

int videoActivity::getListItemCount(const ZKListView *pListView) const{
    int tablen = sizeof(SListViewFunctionsCallbackTab) / sizeof(S_ListViewFunctionsCallback);
    for (int i = 0; i < tablen; ++i) {
        if (SListViewFunctionsCallbackTab[i].id == pListView->getID()) {
            return SListViewFunctionsCallbackTab[i].getListItemCountCallback(pListView);
            break;
        }
    }
    return 0;
}

void videoActivity::obtainListItemData(ZKListView *pListView,ZKListView::ZKListItem *pListItem, int index){
    int tablen = sizeof(SListViewFunctionsCallbackTab) / sizeof(S_ListViewFunctionsCallback);
    for (int i = 0; i < tablen; ++i) {
        if (SListViewFunctionsCallbackTab[i].id == pListView->getID()) {
            SListViewFunctionsCallbackTab[i].obtainListItemDataCallback(pListView, pListItem, index);
            break;
        }
    }
}

void videoActivity::onItemClick(ZKListView *pListView, int index, int id){
    int tablen = sizeof(SListViewFunctionsCallbackTab) / sizeof(S_ListViewFunctionsCallback);
    for (int i = 0; i < tablen; ++i) {
        if (SListViewFunctionsCallbackTab[i].id == pListView->getID()) {
            SListViewFunctionsCallbackTab[i].onItemClickCallback(pListView, index, id);
            break;
        }
    }
}

void videoActivity::onSlideItemClick(ZKSlideWindow *pSlideWindow, int index) {
    int tablen = sizeof(SSlideWindowItemClickCallbackTab) / sizeof(S_SlideWindowItemClickCallback);
    for (int i = 0; i < tablen; ++i) {
        if (SSlideWindowItemClickCallbackTab[i].id == pSlideWindow->getID()) {
            SSlideWindowItemClickCallbackTab[i].onSlideItemClickCallback(pSlideWindow, index);
            break;
        }
    }
}

bool videoActivity::onTouchEvent(const MotionEvent &ev) {
    return onvideoActivityTouchEvent(ev);
}

void videoActivity::onTextChanged(ZKTextView *pTextView, const std::string &text) {
    int tablen = sizeof(SEditTextInputCallbackTab) / sizeof(S_EditTextInputCallback);
    for (int i = 0; i < tablen; ++i) {
        if (SEditTextInputCallbackTab[i].id == pTextView->getID()) {
            SEditTextInputCallbackTab[i].onEditTextChangedCallback(text);
            break;
        }
    }
}

void videoActivity::rigesterActivityTimer() {
    int tablen = sizeof(REGISTER_ACTIVITY_TIMER_TAB) / sizeof(S_ACTIVITY_TIMEER);
    for (int i = 0; i < tablen; ++i) {
        S_ACTIVITY_TIMEER temp = REGISTER_ACTIVITY_TIMER_TAB[i];
        registerTimer(temp.id, temp.time);
    }
}


void videoActivity::onVideoPlayerMessage(ZKVideoView *pVideoView, int msg) {
    int tablen = sizeof(SVideoViewCallbackTab) / sizeof(S_VideoViewCallback);
    for (int i = 0; i < tablen; ++i) {
        if (SVideoViewCallbackTab[i].id == pVideoView->getID()) {
        	if (SVideoViewCallbackTab[i].loop) {
                //循环播放
        		videoLoopPlayback(pVideoView, msg, i);
        	} else if (SVideoViewCallbackTab[i].onVideoViewCallback != NULL){
        	    SVideoViewCallbackTab[i].onVideoViewCallback(pVideoView, msg);
        	}
            break;
        }
    }
}

void videoActivity::videoLoopPlayback(ZKVideoView *pVideoView, int msg, size_t callbackTabIndex) {

	switch (msg) {
	case ZKVideoView::E_MSGTYPE_VIDEO_PLAY_STARTED:
		LOGD("ZKVideoView::E_MSGTYPE_VIDEO_PLAY_STARTED\n");
    if (callbackTabIndex >= (sizeof(SVideoViewCallbackTab)/sizeof(S_VideoViewCallback))) {
      break;
    }
		pVideoView->setVolume(SVideoViewCallbackTab[callbackTabIndex].defaultvolume / 10.0);
		mVideoLoopErrorCount = 0;
		break;
	case ZKVideoView::E_MSGTYPE_VIDEO_PLAY_ERROR:
		/**错误处理 */
		++mVideoLoopErrorCount;
		if (mVideoLoopErrorCount > 100) {
			LOGD("video loop error counts > 100, quit loop playback !");
            break;
		} //不用break, 继续尝试播放下一个
	case ZKVideoView::E_MSGTYPE_VIDEO_PLAY_COMPLETED:
		LOGD("ZKVideoView::E_MSGTYPE_VIDEO_PLAY_COMPLETED\n");
        std::vector<std::string> videolist;
        std::string fileName(getAppName());
        if (fileName.size() < 4) {
             LOGD("getAppName size < 4, ignore!");
             break;
        }
        fileName = fileName.substr(0, fileName.length() - 4) + "_video_list.txt";
        fileName = "/mnt/extsd/" + fileName;
        if (!parseVideoFileList(fileName.c_str(), videolist)) {
            LOGD("parseVideoFileList failed !");
		    break;
        }
		if (pVideoView && !videolist.empty()) {
			mVideoLoopIndex = (mVideoLoopIndex + 1) % videolist.size();
			pVideoView->play(videolist[mVideoLoopIndex].c_str());
		}
		break;
	}
}

void videoActivity::startVideoLoopPlayback() {
    int tablen = sizeof(SVideoViewCallbackTab) / sizeof(S_VideoViewCallback);
    for (int i = 0; i < tablen; ++i) {
    	if (SVideoViewCallbackTab[i].loop) {
    		ZKVideoView* videoView = (ZKVideoView*)findControlByID(SVideoViewCallbackTab[i].id);
    		if (!videoView) {
    			return;
    		}
    		//循环播放
    		videoLoopPlayback(videoView, ZKVideoView::E_MSGTYPE_VIDEO_PLAY_COMPLETED, i);
    		return;
    	}
    }
}

void videoActivity::stopVideoLoopPlayback() {
    int tablen = sizeof(SVideoViewCallbackTab) / sizeof(S_VideoViewCallback);
    for (int i = 0; i < tablen; ++i) {
    	if (SVideoViewCallbackTab[i].loop) {
    		ZKVideoView* videoView = (ZKVideoView*)findControlByID(SVideoViewCallbackTab[i].id);
    		if (!videoView) {
    			return;
    		}
    		if (videoView->isPlaying()) {
    		    videoView->stop();
    		}
    		return;
    	}
    }
}

bool videoActivity::parseVideoFileList(const char *pFileListPath, std::vector<string>& mediaFileList) {
	mediaFileList.clear();
	if (NULL == pFileListPath || 0 == strlen(pFileListPath)) {
        LOGD("video file list is null!");
		return false;
	}

	ifstream is(pFileListPath, ios_base::in);
	if (!is.is_open()) {
		LOGD("cann't open file %s \n", pFileListPath);
		return false;
	}
	char tmp[1024] = {0};
	while (is.getline(tmp, sizeof(tmp))) {
		string str = tmp;
		removeCharFromString(str, '\"');
		removeCharFromString(str, '\r');
		removeCharFromString(str, '\n');
		if (str.size() > 1) {
     		mediaFileList.push_back(str.c_str());
		}
	}
  LOGD("(f:%s, l:%d) parse fileList[%s], get [%d]files", __FUNCTION__,
      __LINE__, pFileListPath, int(mediaFileList.size()));
  for (std::vector<string>::size_type i = 0; i < mediaFileList.size(); i++) {
    LOGD("file[%u]:[%s]", int(i), mediaFileList[i].c_str());
  }
	is.close();

	return true;
}

int videoActivity::removeCharFromString(string& nString, char c) {
    string::size_type   pos;
    while(1) {
        pos = nString.find(c);
        if(pos != string::npos) {
            nString.erase(pos, 1);
        } else {
            break;
        }
    }
    return (int)nString.size();
}

void videoActivity::registerUserTimer(int id, int time) {
	registerTimer(id, time);
}

void videoActivity::unregisterUserTimer(int id) {
	unregisterTimer(id);
}

void videoActivity::resetUserTimer(int id, int time) {
	resetTimer(id, time);
}