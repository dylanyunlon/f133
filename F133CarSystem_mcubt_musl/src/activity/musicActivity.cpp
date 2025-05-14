/***********************************************
/gen auto by zuitools
***********************************************/
#include "musicActivity.h"

/*TAG:GlobalVariable全局变量*/
static ZKTextView* mTextView7Ptr;
static ZKTextView* mTextView5Ptr;
static ZKTextView* mTextView6Ptr;
static ZKTextView* mTextView3Ptr;
static ZKTextView* mTextView4Ptr;
static ZKTextView* mTextView2Ptr;
static ZKTextView* mTextView1Ptr;
static ZKTextView* merrorTextViewPtr;
static ZKWindow* merrorWindowPtr;
static ZKTextView* mTextViewPtr;
static ZKTextView* mpicTextViewPtr;
static ZKTextView* malbumTextViewPtr;
static ZKTextView* martistTextViewPtr;
static ZKButton* mNextButtonPtr;
static ZKButton* mButtonPlayPtr;
static ZKButton* mPrevButtonPtr;
static ZKButton* mplayModeButtonPtr;
static ZKButton* mmusicListButtonPtr;
static ZKTextView* mDurationTextViewPtr;
static ZKSeekBar* mPlayProgressSeekbarPtr;
static ZKTextView* mCurPosTextViewPtr;
static ZKTextView* mtitleTextViewPtr;
static ZKButton* mmusicCloseButtonPtr;
static ZKWindow* mmusicWindowPtr;
static ZKPointer* msyncPointerPtr;
static ZKTextView* mscaningTextViewPtr;
static ZKWindow* mscaningWindowPtr;
static ZKButton* mmusicButtonPtr;
static ZKListView* mmusicListViewPtr;
static ZKButton* mUSB2ButtonPtr;
static ZKButton* mUSB1ButtonPtr;
static ZKButton* mSDButtonPtr;
static musicActivity* mActivityPtr;

/*register activity*/
REGISTER_ACTIVITY(musicActivity);

typedef struct {
	int id; // 定时器ID ， 不能重复
	int time; // 定时器  时间间隔  单位 毫秒
}S_ACTIVITY_TIMEER;

#include "logic/musicLogic.cc"

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
    ID_MUSIC_NextButton, onButtonClick_NextButton,
    ID_MUSIC_ButtonPlay, onButtonClick_ButtonPlay,
    ID_MUSIC_PrevButton, onButtonClick_PrevButton,
    ID_MUSIC_playModeButton, onButtonClick_playModeButton,
    ID_MUSIC_musicListButton, onButtonClick_musicListButton,
    ID_MUSIC_musicCloseButton, onButtonClick_musicCloseButton,
    ID_MUSIC_musicButton, onButtonClick_musicButton,
    ID_MUSIC_USB2Button, onButtonClick_USB2Button,
    ID_MUSIC_USB1Button, onButtonClick_USB1Button,
    ID_MUSIC_SDButton, onButtonClick_SDButton,
};
/***************/


typedef void (*SeekBarCallback)(ZKSeekBar *pSeekBar, int progress);
typedef struct {
    int id;
    SeekBarCallback callback;
}S_ZKSeekBarCallback;
/*TAG:SeekBarCallbackTab*/
static S_ZKSeekBarCallback SZKSeekBarCallbackTab[] = {
    ID_MUSIC_PlayProgressSeekbar, onProgressChanged_PlayProgressSeekbar,
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
    ID_MUSIC_musicListView, getListItemCount_musicListView, obtainListItemData_musicListView, onListItemClick_musicListView,
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
};


musicActivity::musicActivity() {
	//todo add init code here
	mVideoLoopIndex = -1;
	mVideoLoopErrorCount = 0;
}

musicActivity::~musicActivity() {
  //todo add init file here
  // 退出应用时需要反注册
    EASYUICONTEXT->unregisterGlobalTouchListener(this);
    unregisterProtocolDataUpdateListener(onProtocolDataUpdate);
    onUI_quit();
    mActivityPtr = NULL;
    mTextView7Ptr = NULL;
    mTextView5Ptr = NULL;
    mTextView6Ptr = NULL;
    mTextView3Ptr = NULL;
    mTextView4Ptr = NULL;
    mTextView2Ptr = NULL;
    mTextView1Ptr = NULL;
    merrorTextViewPtr = NULL;
    merrorWindowPtr = NULL;
    mpicTextViewPtr = NULL;
    malbumTextViewPtr = NULL;
    martistTextViewPtr = NULL;
    mNextButtonPtr = NULL;
    mButtonPlayPtr = NULL;
    mPrevButtonPtr = NULL;
    mplayModeButtonPtr = NULL;
    mmusicListButtonPtr = NULL;
    mDurationTextViewPtr = NULL;
    mPlayProgressSeekbarPtr = NULL;
    mCurPosTextViewPtr = NULL;
    mtitleTextViewPtr = NULL;
    mmusicCloseButtonPtr = NULL;
    mmusicWindowPtr = NULL;
    msyncPointerPtr = NULL;
    mscaningTextViewPtr = NULL;
    mscaningWindowPtr = NULL;
    mmusicButtonPtr = NULL;
    mmusicListViewPtr = NULL;
    mUSB2ButtonPtr = NULL;
    mUSB1ButtonPtr = NULL;
    mSDButtonPtr = NULL;
}

const char* musicActivity::getAppName() const{
	return "music.ftu";
}

//TAG:onCreate
void musicActivity::onCreate() {
	Activity::onCreate();
    mTextView7Ptr = (ZKTextView*)findControlByID(ID_MUSIC_TextView7);
    mTextView5Ptr = (ZKTextView*)findControlByID(ID_MUSIC_TextView5);
    mTextView6Ptr = (ZKTextView*)findControlByID(ID_MUSIC_TextView6);
    mTextView3Ptr = (ZKTextView*)findControlByID(ID_MUSIC_TextView3);
    mTextView4Ptr = (ZKTextView*)findControlByID(ID_MUSIC_TextView4);
    mTextView2Ptr = (ZKTextView*)findControlByID(ID_MUSIC_TextView2);
    mTextView1Ptr = (ZKTextView*)findControlByID(ID_MUSIC_TextView1);
    merrorTextViewPtr = (ZKTextView*)findControlByID(ID_MUSIC_errorTextView);
    merrorWindowPtr = (ZKWindow*)findControlByID(ID_MUSIC_errorWindow);
    mTextViewPtr = (ZKTextView*)findControlByID(ID_MUSIC_TextView);
    mpicTextViewPtr = (ZKTextView*)findControlByID(ID_MUSIC_picTextView);
    malbumTextViewPtr = (ZKTextView*)findControlByID(ID_MUSIC_albumTextView);
    martistTextViewPtr = (ZKTextView*)findControlByID(ID_MUSIC_artistTextView);
    mNextButtonPtr = (ZKButton*)findControlByID(ID_MUSIC_NextButton);
    mButtonPlayPtr = (ZKButton*)findControlByID(ID_MUSIC_ButtonPlay);
    mPrevButtonPtr = (ZKButton*)findControlByID(ID_MUSIC_PrevButton);
    mplayModeButtonPtr = (ZKButton*)findControlByID(ID_MUSIC_playModeButton);
    mmusicListButtonPtr = (ZKButton*)findControlByID(ID_MUSIC_musicListButton);
    mDurationTextViewPtr = (ZKTextView*)findControlByID(ID_MUSIC_DurationTextView);
    mPlayProgressSeekbarPtr = (ZKSeekBar*)findControlByID(ID_MUSIC_PlayProgressSeekbar);if(mPlayProgressSeekbarPtr!= NULL){mPlayProgressSeekbarPtr->setSeekBarChangeListener(this);}
    mCurPosTextViewPtr = (ZKTextView*)findControlByID(ID_MUSIC_CurPosTextView);
    mtitleTextViewPtr = (ZKTextView*)findControlByID(ID_MUSIC_titleTextView);
    mmusicCloseButtonPtr = (ZKButton*)findControlByID(ID_MUSIC_musicCloseButton);
    mmusicWindowPtr = (ZKWindow*)findControlByID(ID_MUSIC_musicWindow);
    msyncPointerPtr = (ZKPointer*)findControlByID(ID_MUSIC_syncPointer);
    mscaningTextViewPtr = (ZKTextView*)findControlByID(ID_MUSIC_scaningTextView);
    mscaningWindowPtr = (ZKWindow*)findControlByID(ID_MUSIC_scaningWindow);
    mmusicButtonPtr = (ZKButton*)findControlByID(ID_MUSIC_musicButton);
    mmusicListViewPtr = (ZKListView*)findControlByID(ID_MUSIC_musicListView);if(mmusicListViewPtr!= NULL){mmusicListViewPtr->setListAdapter(this);mmusicListViewPtr->setItemClickListener(this);}
    mUSB2ButtonPtr = (ZKButton*)findControlByID(ID_MUSIC_USB2Button);
    mUSB1ButtonPtr = (ZKButton*)findControlByID(ID_MUSIC_USB1Button);
    mSDButtonPtr = (ZKButton*)findControlByID(ID_MUSIC_SDButton);
	mActivityPtr = this;
	onUI_init();
  registerProtocolDataUpdateListener(onProtocolDataUpdate);
  rigesterActivityTimer();
}

void musicActivity::onClick(ZKBase *pBase) {
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

void musicActivity::onResume() {
	Activity::onResume();
	EASYUICONTEXT->registerGlobalTouchListener(this);
	startVideoLoopPlayback();
	onUI_show();
}

void musicActivity::onPause() {
	Activity::onPause();
	EASYUICONTEXT->unregisterGlobalTouchListener(this);
	stopVideoLoopPlayback();
	onUI_hide();
}

void musicActivity::onIntent(const Intent *intentPtr) {
	Activity::onIntent(intentPtr);
	onUI_intent(intentPtr);
}

bool musicActivity::onTimer(int id) {
	return onUI_Timer(id);
}

void musicActivity::onProgressChanged(ZKSeekBar *pSeekBar, int progress){

    int seekBarTablen = sizeof(SZKSeekBarCallbackTab) / sizeof(S_ZKSeekBarCallback);
    for (int i = 0; i < seekBarTablen; ++i) {
        if (SZKSeekBarCallbackTab[i].id == pSeekBar->getID()) {
            SZKSeekBarCallbackTab[i].callback(pSeekBar, progress);
            break;
        }
    }
}

int musicActivity::getListItemCount(const ZKListView *pListView) const{
    int tablen = sizeof(SListViewFunctionsCallbackTab) / sizeof(S_ListViewFunctionsCallback);
    for (int i = 0; i < tablen; ++i) {
        if (SListViewFunctionsCallbackTab[i].id == pListView->getID()) {
            return SListViewFunctionsCallbackTab[i].getListItemCountCallback(pListView);
            break;
        }
    }
    return 0;
}

void musicActivity::obtainListItemData(ZKListView *pListView,ZKListView::ZKListItem *pListItem, int index){
    int tablen = sizeof(SListViewFunctionsCallbackTab) / sizeof(S_ListViewFunctionsCallback);
    for (int i = 0; i < tablen; ++i) {
        if (SListViewFunctionsCallbackTab[i].id == pListView->getID()) {
            SListViewFunctionsCallbackTab[i].obtainListItemDataCallback(pListView, pListItem, index);
            break;
        }
    }
}

void musicActivity::onItemClick(ZKListView *pListView, int index, int id){
    int tablen = sizeof(SListViewFunctionsCallbackTab) / sizeof(S_ListViewFunctionsCallback);
    for (int i = 0; i < tablen; ++i) {
        if (SListViewFunctionsCallbackTab[i].id == pListView->getID()) {
            SListViewFunctionsCallbackTab[i].onItemClickCallback(pListView, index, id);
            break;
        }
    }
}

void musicActivity::onSlideItemClick(ZKSlideWindow *pSlideWindow, int index) {
    int tablen = sizeof(SSlideWindowItemClickCallbackTab) / sizeof(S_SlideWindowItemClickCallback);
    for (int i = 0; i < tablen; ++i) {
        if (SSlideWindowItemClickCallbackTab[i].id == pSlideWindow->getID()) {
            SSlideWindowItemClickCallbackTab[i].onSlideItemClickCallback(pSlideWindow, index);
            break;
        }
    }
}

bool musicActivity::onTouchEvent(const MotionEvent &ev) {
    return onmusicActivityTouchEvent(ev);
}

void musicActivity::onTextChanged(ZKTextView *pTextView, const std::string &text) {
    int tablen = sizeof(SEditTextInputCallbackTab) / sizeof(S_EditTextInputCallback);
    for (int i = 0; i < tablen; ++i) {
        if (SEditTextInputCallbackTab[i].id == pTextView->getID()) {
            SEditTextInputCallbackTab[i].onEditTextChangedCallback(text);
            break;
        }
    }
}

void musicActivity::rigesterActivityTimer() {
    int tablen = sizeof(REGISTER_ACTIVITY_TIMER_TAB) / sizeof(S_ACTIVITY_TIMEER);
    for (int i = 0; i < tablen; ++i) {
        S_ACTIVITY_TIMEER temp = REGISTER_ACTIVITY_TIMER_TAB[i];
        registerTimer(temp.id, temp.time);
    }
}


void musicActivity::onVideoPlayerMessage(ZKVideoView *pVideoView, int msg) {
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

void musicActivity::videoLoopPlayback(ZKVideoView *pVideoView, int msg, size_t callbackTabIndex) {

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

void musicActivity::startVideoLoopPlayback() {
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

void musicActivity::stopVideoLoopPlayback() {
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

bool musicActivity::parseVideoFileList(const char *pFileListPath, std::vector<string>& mediaFileList) {
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

int musicActivity::removeCharFromString(string& nString, char c) {
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

void musicActivity::registerUserTimer(int id, int time) {
	registerTimer(id, time);
}

void musicActivity::unregisterUserTimer(int id) {
	unregisterTimer(id);
}

void musicActivity::resetUserTimer(int id, int time) {
	resetTimer(id, time);
}