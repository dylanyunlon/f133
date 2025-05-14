/***********************************************
/gen auto by zuitools
***********************************************/
#include "desktopActivity.h"

/*TAG:GlobalVariable全局变量*/
static ZKSlideWindow* mappSlideWindowPtr;
static ZKTextView* mTextView3Ptr;
static ZKWindow* mWindow1Ptr;
static ZKWindow* mbtTipsWindowPtr;
static ZKTextView* mTextView6Ptr;
static ZKWindow* mWindow5Ptr;
static ZKWindow* mlinkTipsWindowPtr;
static ZKDigitalClock* mDigitalClock2Ptr;
static ZKButton* msoundButtonPtr;
static ZKButton* mNetButtonPtr;
static ZKButton* mbtStatusButtonPtr;
static ZKButton* musbButtonPtr;
static ZKButton* msdButtonPtr;
static ZKButton* mlinkButtonPtr;
static ZKButton* mfmButtonPtr;
static ZKButton* mauxButtonPtr;
static ZKWindow* mheadWindowPtr;
static ZKButton* mButton2Ptr;
static ZKTextView* mverTextViewPtr;
static ZKButton* mButton1Ptr;
static ZKSeekBar* mPlayVolSeekBarPtr;
static ZKButton* mSettingPtr;
static ZKWindow* mbuttomWindowPtr;
static ZKButton* mToMusicPtr;
static ZKTextView* mmusicPicTextViewPtr;
static ZKSeekBar* mPlayProgressSeekbarPtr;
static ZKButton* mPrevButtonPtr;
static ZKButton* mButtonPlayPtr;
static ZKButton* mNextButtonPtr;
static ZKTextView* martistTextViewPtr;
static ZKTextView* mtitleTextViewPtr;
static ZKWindow* mmusicWindowPtr;
static ZKRadioGroup* mStatusRadioGroupPtr;
static ZKTextView* mTextViewBgPtr;
static desktopActivity* mActivityPtr;

/*register activity*/
REGISTER_ACTIVITY(desktopActivity);

typedef struct {
	int id; // 定时器ID ， 不能重复
	int time; // 定时器  时间间隔  单位 毫秒
}S_ACTIVITY_TIMEER;

#include "logic/desktopLogic.cc"

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
    ID_DESKTOP_soundButton, onButtonClick_soundButton,
    ID_DESKTOP_NetButton, onButtonClick_NetButton,
    ID_DESKTOP_btStatusButton, onButtonClick_btStatusButton,
    ID_DESKTOP_usbButton, onButtonClick_usbButton,
    ID_DESKTOP_sdButton, onButtonClick_sdButton,
    ID_DESKTOP_linkButton, onButtonClick_linkButton,
    ID_DESKTOP_fmButton, onButtonClick_fmButton,
    ID_DESKTOP_auxButton, onButtonClick_auxButton,
    ID_DESKTOP_Button2, onButtonClick_Button2,
    ID_DESKTOP_Button1, onButtonClick_Button1,
    ID_DESKTOP_Setting, onButtonClick_Setting,
    ID_DESKTOP_ToMusic, onButtonClick_ToMusic,
    ID_DESKTOP_PrevButton, onButtonClick_PrevButton,
    ID_DESKTOP_ButtonPlay, onButtonClick_ButtonPlay,
    ID_DESKTOP_NextButton, onButtonClick_NextButton,
};
/***************/


typedef void (*SeekBarCallback)(ZKSeekBar *pSeekBar, int progress);
typedef struct {
    int id;
    SeekBarCallback callback;
}S_ZKSeekBarCallback;
/*TAG:SeekBarCallbackTab*/
static S_ZKSeekBarCallback SZKSeekBarCallbackTab[] = {
    ID_DESKTOP_PlayVolSeekBar, onProgressChanged_PlayVolSeekBar,
    ID_DESKTOP_PlayProgressSeekbar, onProgressChanged_PlayProgressSeekbar,
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
};


typedef void (*SlideWindowItemClickCallback)(ZKSlideWindow *pSlideWindow, int index);
typedef struct {
    int id;
    SlideWindowItemClickCallback onSlideItemClickCallback;
}S_SlideWindowItemClickCallback;
/*TAG:SlideWindowFunctionsCallbackTab*/
static S_SlideWindowItemClickCallback SSlideWindowItemClickCallbackTab[] = {
    ID_DESKTOP_appSlideWindow, onSlideItemClick_appSlideWindow,
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


typedef void (*RadioGroupCallback)(ZKRadioGroup*, int);
typedef struct {
  int id;
  RadioGroupCallback onCheckedChanged;
}S_RadioGroupCallback;
/*TAG:RadioGroupCallbackTab*/
static S_RadioGroupCallback SRadioGroupCallbackTab[] = {
    ID_DESKTOP_StatusRadioGroup, onCheckedChanged_StatusRadioGroup,
};

desktopActivity::desktopActivity() {
	//todo add init code here
	mVideoLoopIndex = -1;
	mVideoLoopErrorCount = 0;
}

desktopActivity::~desktopActivity() {
  //todo add init file here
  // 退出应用时需要反注册
    EASYUICONTEXT->unregisterGlobalTouchListener(this);
    unregisterProtocolDataUpdateListener(onProtocolDataUpdate);
    onUI_quit();
    mActivityPtr = NULL;
    mappSlideWindowPtr = NULL;
    mTextView3Ptr = NULL;
    mWindow1Ptr = NULL;
    mbtTipsWindowPtr = NULL;
    mTextView6Ptr = NULL;
    mWindow5Ptr = NULL;
    mlinkTipsWindowPtr = NULL;
    mDigitalClock2Ptr = NULL;
    msoundButtonPtr = NULL;
    mNetButtonPtr = NULL;
    mbtStatusButtonPtr = NULL;
    musbButtonPtr = NULL;
    msdButtonPtr = NULL;
    mlinkButtonPtr = NULL;
    mfmButtonPtr = NULL;
    mauxButtonPtr = NULL;
    mheadWindowPtr = NULL;
    mButton2Ptr = NULL;
    mverTextViewPtr = NULL;
    mButton1Ptr = NULL;
    mPlayVolSeekBarPtr = NULL;
    mSettingPtr = NULL;
    mbuttomWindowPtr = NULL;
    mToMusicPtr = NULL;
    mmusicPicTextViewPtr = NULL;
    mPlayProgressSeekbarPtr = NULL;
    mPrevButtonPtr = NULL;
    mButtonPlayPtr = NULL;
    mNextButtonPtr = NULL;
    martistTextViewPtr = NULL;
    mtitleTextViewPtr = NULL;
    mmusicWindowPtr = NULL;
    mStatusRadioGroupPtr = NULL;
    mTextViewBgPtr = NULL;
}

const char* desktopActivity::getAppName() const{
	return "desktop.ftu";
}

//TAG:onCreate
void desktopActivity::onCreate() {
	Activity::onCreate();
    mappSlideWindowPtr = (ZKSlideWindow*)findControlByID(ID_DESKTOP_appSlideWindow);if(mappSlideWindowPtr!= NULL){mappSlideWindowPtr->setSlideItemClickListener(this);}
    mTextView3Ptr = (ZKTextView*)findControlByID(ID_DESKTOP_TextView3);
    mWindow1Ptr = (ZKWindow*)findControlByID(ID_DESKTOP_Window1);
    mbtTipsWindowPtr = (ZKWindow*)findControlByID(ID_DESKTOP_btTipsWindow);
    mTextView6Ptr = (ZKTextView*)findControlByID(ID_DESKTOP_TextView6);
    mWindow5Ptr = (ZKWindow*)findControlByID(ID_DESKTOP_Window5);
    mlinkTipsWindowPtr = (ZKWindow*)findControlByID(ID_DESKTOP_linkTipsWindow);
    mDigitalClock2Ptr = (ZKDigitalClock*)findControlByID(ID_DESKTOP_DigitalClock2);
    msoundButtonPtr = (ZKButton*)findControlByID(ID_DESKTOP_soundButton);
    mNetButtonPtr = (ZKButton*)findControlByID(ID_DESKTOP_NetButton);
    mbtStatusButtonPtr = (ZKButton*)findControlByID(ID_DESKTOP_btStatusButton);
    musbButtonPtr = (ZKButton*)findControlByID(ID_DESKTOP_usbButton);
    msdButtonPtr = (ZKButton*)findControlByID(ID_DESKTOP_sdButton);
    mlinkButtonPtr = (ZKButton*)findControlByID(ID_DESKTOP_linkButton);
    mfmButtonPtr = (ZKButton*)findControlByID(ID_DESKTOP_fmButton);
    mauxButtonPtr = (ZKButton*)findControlByID(ID_DESKTOP_auxButton);
    mheadWindowPtr = (ZKWindow*)findControlByID(ID_DESKTOP_headWindow);
    mButton2Ptr = (ZKButton*)findControlByID(ID_DESKTOP_Button2);
    mverTextViewPtr = (ZKTextView*)findControlByID(ID_DESKTOP_verTextView);
    mButton1Ptr = (ZKButton*)findControlByID(ID_DESKTOP_Button1);
    mPlayVolSeekBarPtr = (ZKSeekBar*)findControlByID(ID_DESKTOP_PlayVolSeekBar);if(mPlayVolSeekBarPtr!= NULL){mPlayVolSeekBarPtr->setSeekBarChangeListener(this);}
    mSettingPtr = (ZKButton*)findControlByID(ID_DESKTOP_Setting);
    mbuttomWindowPtr = (ZKWindow*)findControlByID(ID_DESKTOP_buttomWindow);
    mToMusicPtr = (ZKButton*)findControlByID(ID_DESKTOP_ToMusic);
    mmusicPicTextViewPtr = (ZKTextView*)findControlByID(ID_DESKTOP_musicPicTextView);
    mPlayProgressSeekbarPtr = (ZKSeekBar*)findControlByID(ID_DESKTOP_PlayProgressSeekbar);if(mPlayProgressSeekbarPtr!= NULL){mPlayProgressSeekbarPtr->setSeekBarChangeListener(this);}
    mPrevButtonPtr = (ZKButton*)findControlByID(ID_DESKTOP_PrevButton);
    mButtonPlayPtr = (ZKButton*)findControlByID(ID_DESKTOP_ButtonPlay);
    mNextButtonPtr = (ZKButton*)findControlByID(ID_DESKTOP_NextButton);
    martistTextViewPtr = (ZKTextView*)findControlByID(ID_DESKTOP_artistTextView);
    mtitleTextViewPtr = (ZKTextView*)findControlByID(ID_DESKTOP_titleTextView);
    mmusicWindowPtr = (ZKWindow*)findControlByID(ID_DESKTOP_musicWindow);
    mStatusRadioGroupPtr = (ZKRadioGroup*)findControlByID(ID_DESKTOP_StatusRadioGroup);if(mStatusRadioGroupPtr!= NULL){mStatusRadioGroupPtr->setCheckedChangeListener(this);}
    mTextViewBgPtr = (ZKTextView*)findControlByID(ID_DESKTOP_TextViewBg);
	mActivityPtr = this;
	onUI_init();
  registerProtocolDataUpdateListener(onProtocolDataUpdate);
  rigesterActivityTimer();
}

void desktopActivity::onClick(ZKBase *pBase) {
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

void desktopActivity::onResume() {
	Activity::onResume();
	EASYUICONTEXT->registerGlobalTouchListener(this);
	startVideoLoopPlayback();
	onUI_show();
}

void desktopActivity::onPause() {
	Activity::onPause();
	EASYUICONTEXT->unregisterGlobalTouchListener(this);
	stopVideoLoopPlayback();
	onUI_hide();
}

void desktopActivity::onIntent(const Intent *intentPtr) {
	Activity::onIntent(intentPtr);
	onUI_intent(intentPtr);
}

bool desktopActivity::onTimer(int id) {
	return onUI_Timer(id);
}

void desktopActivity::onProgressChanged(ZKSeekBar *pSeekBar, int progress){

    int seekBarTablen = sizeof(SZKSeekBarCallbackTab) / sizeof(S_ZKSeekBarCallback);
    for (int i = 0; i < seekBarTablen; ++i) {
        if (SZKSeekBarCallbackTab[i].id == pSeekBar->getID()) {
            SZKSeekBarCallbackTab[i].callback(pSeekBar, progress);
            break;
        }
    }
}

int desktopActivity::getListItemCount(const ZKListView *pListView) const{
    int tablen = sizeof(SListViewFunctionsCallbackTab) / sizeof(S_ListViewFunctionsCallback);
    for (int i = 0; i < tablen; ++i) {
        if (SListViewFunctionsCallbackTab[i].id == pListView->getID()) {
            return SListViewFunctionsCallbackTab[i].getListItemCountCallback(pListView);
            break;
        }
    }
    return 0;
}

void desktopActivity::obtainListItemData(ZKListView *pListView,ZKListView::ZKListItem *pListItem, int index){
    int tablen = sizeof(SListViewFunctionsCallbackTab) / sizeof(S_ListViewFunctionsCallback);
    for (int i = 0; i < tablen; ++i) {
        if (SListViewFunctionsCallbackTab[i].id == pListView->getID()) {
            SListViewFunctionsCallbackTab[i].obtainListItemDataCallback(pListView, pListItem, index);
            break;
        }
    }
}

void desktopActivity::onItemClick(ZKListView *pListView, int index, int id){
    int tablen = sizeof(SListViewFunctionsCallbackTab) / sizeof(S_ListViewFunctionsCallback);
    for (int i = 0; i < tablen; ++i) {
        if (SListViewFunctionsCallbackTab[i].id == pListView->getID()) {
            SListViewFunctionsCallbackTab[i].onItemClickCallback(pListView, index, id);
            break;
        }
    }
}

void desktopActivity::onSlideItemClick(ZKSlideWindow *pSlideWindow, int index) {
    int tablen = sizeof(SSlideWindowItemClickCallbackTab) / sizeof(S_SlideWindowItemClickCallback);
    for (int i = 0; i < tablen; ++i) {
        if (SSlideWindowItemClickCallbackTab[i].id == pSlideWindow->getID()) {
            SSlideWindowItemClickCallbackTab[i].onSlideItemClickCallback(pSlideWindow, index);
            break;
        }
    }
}

bool desktopActivity::onTouchEvent(const MotionEvent &ev) {
    return ondesktopActivityTouchEvent(ev);
}

void desktopActivity::onTextChanged(ZKTextView *pTextView, const std::string &text) {
    int tablen = sizeof(SEditTextInputCallbackTab) / sizeof(S_EditTextInputCallback);
    for (int i = 0; i < tablen; ++i) {
        if (SEditTextInputCallbackTab[i].id == pTextView->getID()) {
            SEditTextInputCallbackTab[i].onEditTextChangedCallback(text);
            break;
        }
    }
}

void desktopActivity::rigesterActivityTimer() {
    int tablen = sizeof(REGISTER_ACTIVITY_TIMER_TAB) / sizeof(S_ACTIVITY_TIMEER);
    for (int i = 0; i < tablen; ++i) {
        S_ACTIVITY_TIMEER temp = REGISTER_ACTIVITY_TIMER_TAB[i];
        registerTimer(temp.id, temp.time);
    }
}


void desktopActivity::onVideoPlayerMessage(ZKVideoView *pVideoView, int msg) {
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

void desktopActivity::videoLoopPlayback(ZKVideoView *pVideoView, int msg, size_t callbackTabIndex) {

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

void desktopActivity::startVideoLoopPlayback() {
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

void desktopActivity::stopVideoLoopPlayback() {
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

bool desktopActivity::parseVideoFileList(const char *pFileListPath, std::vector<string>& mediaFileList) {
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
    LOGD("file[%d]:[%s]", int(i), mediaFileList[i].c_str());
  }
	is.close();

	return true;
}

int desktopActivity::removeCharFromString(string& nString, char c) {
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

void desktopActivity::registerUserTimer(int id, int time) {
	registerTimer(id, time);
}

void desktopActivity::unregisterUserTimer(int id) {
	unregisterTimer(id);
}

void desktopActivity::resetUserTimer(int id, int time) {
	resetTimer(id, time);
}
void desktopActivity::onCheckedChanged(ZKRadioGroup* pRadioGroup, int checkedID) {
    int tablen = sizeof(SRadioGroupCallbackTab) / sizeof(S_RadioGroupCallback);
    for (int i = 0; i < tablen; ++i) {
        if (SRadioGroupCallbackTab[i].id == pRadioGroup->getID()) {
        	SRadioGroupCallbackTab[i].onCheckedChanged(pRadioGroup, checkedID);
            break;
        }
    }
}