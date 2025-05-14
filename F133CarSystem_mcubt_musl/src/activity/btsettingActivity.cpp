/***********************************************
/gen auto by zuitools
***********************************************/
#include "btsettingActivity.h"

/*TAG:GlobalVariable全局变量*/
static ZKPointer* msyncPointerPtr;
static ZKTextView* mscaningTextViewPtr;
static ZKWindow* mscaningWindowPtr;
static ZKPointer* mopeningPointerPtr;
static ZKWindow* mopeningWndPtr;
static ZKTextView* mTextView14Ptr;
static ZKWindow* mbtTipsWindowPtr;
static ZKTextView* mTextView6Ptr;
static ZKWindow* mnameLongTipsWindowPtr;
static ZKTextView* mconnectedNameTextViewPtr;
static ZKTextView* mTextView3Ptr;
static ZKButton* mdisconCancelButtonPtr;
static ZKButton* mdisConnectButtonPtr;
static ZKTextView* mTextView7Ptr;
static ZKWindow* mdisConnectWindowPtr;
static ZKTextView* mTextView9Ptr;
static ZKTextView* mbtSwitchTextViewPtr;
static ZKTextView* mconnDevTipsTextViewPtr;
static ZKButton* mconnDevButtonPtr;
static ZKButton* msys_backPtr;
static ZKButton* menableButtonPtr;
static ZKEditText* mbtNameEditTextPtr;
static ZKTextView* mdevNameTipsTextViewPtr;
static ZKTextView* mTextView1Ptr;
static ZKTextView* mTextView12Ptr;
static ZKTextView* mTextView11Ptr;
static ZKTextView* mTextView8Ptr;
static ZKTextView* mTextView5Ptr;
static ZKEditText* mPinEditTextPtr;
static ZKListView* mbtListViewPtr;
static ZKWindow* mWindow2Ptr;
static ZKListView* mmatchedListViewPtr;
static ZKWindow* mWindow1Ptr;
static ZKButton* mqueryMusicButtonPtr;
static ZKButton* mphoneButtonPtr;
static ZKWindow* mbtAppWindowPtr;
static ZKButton* mscanButtonPtr;
static ZKButton* mautoConnButtonPtr;
static ZKButton* mautoAnswerButtonPtr;
static ZKTextView* mTextView4Ptr;
static ZKTextView* mTextView2Ptr;
static ZKTextView* mTextView10Ptr;
static ZKTextView* mTextView13Ptr;
static ZKWindow* mautoWindowPtr;
static ZKTextView* mmatchTextViewPtr;
static ZKTextView* mscanDevTextViewPtr;
static ZKTextView* mbarTextViewPtr;
static ZKWindow* mbtEnableWindowPtr;
static ZKWindow* mbtSettingWindowPtr;
static btsettingActivity* mActivityPtr;

/*register activity*/
REGISTER_ACTIVITY(btsettingActivity);

typedef struct {
	int id; // 定时器ID ， 不能重复
	int time; // 定时器  时间间隔  单位 毫秒
}S_ACTIVITY_TIMEER;

#include "logic/btsettingLogic.cc"

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
    ID_BTSETTING_disconCancelButton, onButtonClick_disconCancelButton,
    ID_BTSETTING_disConnectButton, onButtonClick_disConnectButton,
    ID_BTSETTING_connDevButton, onButtonClick_connDevButton,
    ID_BTSETTING_sys_back, onButtonClick_sys_back,
    ID_BTSETTING_enableButton, onButtonClick_enableButton,
    ID_BTSETTING_queryMusicButton, onButtonClick_queryMusicButton,
    ID_BTSETTING_phoneButton, onButtonClick_phoneButton,
    ID_BTSETTING_scanButton, onButtonClick_scanButton,
    ID_BTSETTING_autoConnButton, onButtonClick_autoConnButton,
    ID_BTSETTING_autoAnswerButton, onButtonClick_autoAnswerButton,
};
/***************/


typedef void (*SeekBarCallback)(ZKSeekBar *pSeekBar, int progress);
typedef struct {
    int id;
    SeekBarCallback callback;
}S_ZKSeekBarCallback;
/*TAG:SeekBarCallbackTab*/
static S_ZKSeekBarCallback SZKSeekBarCallbackTab[] = {
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
    ID_BTSETTING_btListView, getListItemCount_btListView, obtainListItemData_btListView, onListItemClick_btListView,
    ID_BTSETTING_matchedListView, getListItemCount_matchedListView, obtainListItemData_matchedListView, onListItemClick_matchedListView,
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
    ID_BTSETTING_btNameEditText, onEditTextChanged_btNameEditText,
    ID_BTSETTING_PinEditText, onEditTextChanged_PinEditText,
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


btsettingActivity::btsettingActivity() {
	//todo add init code here
	mVideoLoopIndex = -1;
	mVideoLoopErrorCount = 0;
}

btsettingActivity::~btsettingActivity() {
  //todo add init file here
  // 退出应用时需要反注册
    EASYUICONTEXT->unregisterGlobalTouchListener(this);
    unregisterProtocolDataUpdateListener(onProtocolDataUpdate);
    onUI_quit();
    mActivityPtr = NULL;
    msyncPointerPtr = NULL;
    mscaningTextViewPtr = NULL;
    mscaningWindowPtr = NULL;
    mopeningPointerPtr = NULL;
    mopeningWndPtr = NULL;
    mTextView14Ptr = NULL;
    mbtTipsWindowPtr = NULL;
    mTextView6Ptr = NULL;
    mnameLongTipsWindowPtr = NULL;
    mconnectedNameTextViewPtr = NULL;
    mTextView3Ptr = NULL;
    mdisconCancelButtonPtr = NULL;
    mdisConnectButtonPtr = NULL;
    mTextView7Ptr = NULL;
    mdisConnectWindowPtr = NULL;
    mTextView9Ptr = NULL;
    mbtSwitchTextViewPtr = NULL;
    mconnDevTipsTextViewPtr = NULL;
    mconnDevButtonPtr = NULL;
    msys_backPtr = NULL;
    menableButtonPtr = NULL;
    mbtNameEditTextPtr = NULL;
    mdevNameTipsTextViewPtr = NULL;
    mTextView1Ptr = NULL;
    mTextView12Ptr = NULL;
    mTextView11Ptr = NULL;
    mTextView8Ptr = NULL;
    mTextView5Ptr = NULL;
    mPinEditTextPtr = NULL;
    mbtListViewPtr = NULL;
    mWindow2Ptr = NULL;
    mmatchedListViewPtr = NULL;
    mWindow1Ptr = NULL;
    mqueryMusicButtonPtr = NULL;
    mphoneButtonPtr = NULL;
    mbtAppWindowPtr = NULL;
    mscanButtonPtr = NULL;
    mautoConnButtonPtr = NULL;
    mautoAnswerButtonPtr = NULL;
    mTextView4Ptr = NULL;
    mTextView2Ptr = NULL;
    mTextView10Ptr = NULL;
    mTextView13Ptr = NULL;
    mautoWindowPtr = NULL;
    mmatchTextViewPtr = NULL;
    mscanDevTextViewPtr = NULL;
    mbarTextViewPtr = NULL;
    mbtEnableWindowPtr = NULL;
    mbtSettingWindowPtr = NULL;
}

const char* btsettingActivity::getAppName() const{
	return "btsetting.ftu";
}

//TAG:onCreate
void btsettingActivity::onCreate() {
	Activity::onCreate();
    msyncPointerPtr = (ZKPointer*)findControlByID(ID_BTSETTING_syncPointer);
    mscaningTextViewPtr = (ZKTextView*)findControlByID(ID_BTSETTING_scaningTextView);
    mscaningWindowPtr = (ZKWindow*)findControlByID(ID_BTSETTING_scaningWindow);
    mopeningPointerPtr = (ZKPointer*)findControlByID(ID_BTSETTING_openingPointer);
    mopeningWndPtr = (ZKWindow*)findControlByID(ID_BTSETTING_openingWnd);
    mTextView14Ptr = (ZKTextView*)findControlByID(ID_BTSETTING_TextView14);
    mbtTipsWindowPtr = (ZKWindow*)findControlByID(ID_BTSETTING_btTipsWindow);
    mTextView6Ptr = (ZKTextView*)findControlByID(ID_BTSETTING_TextView6);
    mnameLongTipsWindowPtr = (ZKWindow*)findControlByID(ID_BTSETTING_nameLongTipsWindow);
    mconnectedNameTextViewPtr = (ZKTextView*)findControlByID(ID_BTSETTING_connectedNameTextView);
    mTextView3Ptr = (ZKTextView*)findControlByID(ID_BTSETTING_TextView3);
    mdisconCancelButtonPtr = (ZKButton*)findControlByID(ID_BTSETTING_disconCancelButton);
    mdisConnectButtonPtr = (ZKButton*)findControlByID(ID_BTSETTING_disConnectButton);
    mTextView7Ptr = (ZKTextView*)findControlByID(ID_BTSETTING_TextView7);
    mdisConnectWindowPtr = (ZKWindow*)findControlByID(ID_BTSETTING_disConnectWindow);
    mTextView9Ptr = (ZKTextView*)findControlByID(ID_BTSETTING_TextView9);
    mbtSwitchTextViewPtr = (ZKTextView*)findControlByID(ID_BTSETTING_btSwitchTextView);
    mconnDevTipsTextViewPtr = (ZKTextView*)findControlByID(ID_BTSETTING_connDevTipsTextView);
    mconnDevButtonPtr = (ZKButton*)findControlByID(ID_BTSETTING_connDevButton);
    msys_backPtr = (ZKButton*)findControlByID(ID_BTSETTING_sys_back);
    menableButtonPtr = (ZKButton*)findControlByID(ID_BTSETTING_enableButton);
    mbtNameEditTextPtr = (ZKEditText*)findControlByID(ID_BTSETTING_btNameEditText);if(mbtNameEditTextPtr!= NULL){mbtNameEditTextPtr->setTextChangeListener(this);}
    mdevNameTipsTextViewPtr = (ZKTextView*)findControlByID(ID_BTSETTING_devNameTipsTextView);
    mTextView1Ptr = (ZKTextView*)findControlByID(ID_BTSETTING_TextView1);
    mTextView12Ptr = (ZKTextView*)findControlByID(ID_BTSETTING_TextView12);
    mTextView11Ptr = (ZKTextView*)findControlByID(ID_BTSETTING_TextView11);
    mTextView8Ptr = (ZKTextView*)findControlByID(ID_BTSETTING_TextView8);
    mTextView5Ptr = (ZKTextView*)findControlByID(ID_BTSETTING_TextView5);
    mPinEditTextPtr = (ZKEditText*)findControlByID(ID_BTSETTING_PinEditText);if(mPinEditTextPtr!= NULL){mPinEditTextPtr->setTextChangeListener(this);}
    mbtListViewPtr = (ZKListView*)findControlByID(ID_BTSETTING_btListView);if(mbtListViewPtr!= NULL){mbtListViewPtr->setListAdapter(this);mbtListViewPtr->setItemClickListener(this);}
    mWindow2Ptr = (ZKWindow*)findControlByID(ID_BTSETTING_Window2);
    mmatchedListViewPtr = (ZKListView*)findControlByID(ID_BTSETTING_matchedListView);if(mmatchedListViewPtr!= NULL){mmatchedListViewPtr->setListAdapter(this);mmatchedListViewPtr->setItemClickListener(this);}
    mWindow1Ptr = (ZKWindow*)findControlByID(ID_BTSETTING_Window1);
    mqueryMusicButtonPtr = (ZKButton*)findControlByID(ID_BTSETTING_queryMusicButton);
    mphoneButtonPtr = (ZKButton*)findControlByID(ID_BTSETTING_phoneButton);
    mbtAppWindowPtr = (ZKWindow*)findControlByID(ID_BTSETTING_btAppWindow);
    mscanButtonPtr = (ZKButton*)findControlByID(ID_BTSETTING_scanButton);
    mautoConnButtonPtr = (ZKButton*)findControlByID(ID_BTSETTING_autoConnButton);
    mautoAnswerButtonPtr = (ZKButton*)findControlByID(ID_BTSETTING_autoAnswerButton);
    mTextView4Ptr = (ZKTextView*)findControlByID(ID_BTSETTING_TextView4);
    mTextView2Ptr = (ZKTextView*)findControlByID(ID_BTSETTING_TextView2);
    mTextView10Ptr = (ZKTextView*)findControlByID(ID_BTSETTING_TextView10);
    mTextView13Ptr = (ZKTextView*)findControlByID(ID_BTSETTING_TextView13);
    mautoWindowPtr = (ZKWindow*)findControlByID(ID_BTSETTING_autoWindow);
    mmatchTextViewPtr = (ZKTextView*)findControlByID(ID_BTSETTING_matchTextView);
    mscanDevTextViewPtr = (ZKTextView*)findControlByID(ID_BTSETTING_scanDevTextView);
    mbarTextViewPtr = (ZKTextView*)findControlByID(ID_BTSETTING_barTextView);
    mbtEnableWindowPtr = (ZKWindow*)findControlByID(ID_BTSETTING_btEnableWindow);
    mbtSettingWindowPtr = (ZKWindow*)findControlByID(ID_BTSETTING_btSettingWindow);
	mActivityPtr = this;
	onUI_init();
  registerProtocolDataUpdateListener(onProtocolDataUpdate);
  rigesterActivityTimer();
}

void btsettingActivity::onClick(ZKBase *pBase) {
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

void btsettingActivity::onResume() {
	Activity::onResume();
	EASYUICONTEXT->registerGlobalTouchListener(this);
	startVideoLoopPlayback();
	onUI_show();
}

void btsettingActivity::onPause() {
	Activity::onPause();
	EASYUICONTEXT->unregisterGlobalTouchListener(this);
	stopVideoLoopPlayback();
	onUI_hide();
}

void btsettingActivity::onIntent(const Intent *intentPtr) {
	Activity::onIntent(intentPtr);
	onUI_intent(intentPtr);
}

bool btsettingActivity::onTimer(int id) {
	return onUI_Timer(id);
}

void btsettingActivity::onProgressChanged(ZKSeekBar *pSeekBar, int progress){

    int seekBarTablen = sizeof(SZKSeekBarCallbackTab) / sizeof(S_ZKSeekBarCallback);
    for (int i = 0; i < seekBarTablen; ++i) {
        if (SZKSeekBarCallbackTab[i].id == pSeekBar->getID()) {
            SZKSeekBarCallbackTab[i].callback(pSeekBar, progress);
            break;
        }
    }
}

int btsettingActivity::getListItemCount(const ZKListView *pListView) const{
    int tablen = sizeof(SListViewFunctionsCallbackTab) / sizeof(S_ListViewFunctionsCallback);
    for (int i = 0; i < tablen; ++i) {
        if (SListViewFunctionsCallbackTab[i].id == pListView->getID()) {
            return SListViewFunctionsCallbackTab[i].getListItemCountCallback(pListView);
            break;
        }
    }
    return 0;
}

void btsettingActivity::obtainListItemData(ZKListView *pListView,ZKListView::ZKListItem *pListItem, int index){
    int tablen = sizeof(SListViewFunctionsCallbackTab) / sizeof(S_ListViewFunctionsCallback);
    for (int i = 0; i < tablen; ++i) {
        if (SListViewFunctionsCallbackTab[i].id == pListView->getID()) {
            SListViewFunctionsCallbackTab[i].obtainListItemDataCallback(pListView, pListItem, index);
            break;
        }
    }
}

void btsettingActivity::onItemClick(ZKListView *pListView, int index, int id){
    int tablen = sizeof(SListViewFunctionsCallbackTab) / sizeof(S_ListViewFunctionsCallback);
    for (int i = 0; i < tablen; ++i) {
        if (SListViewFunctionsCallbackTab[i].id == pListView->getID()) {
            SListViewFunctionsCallbackTab[i].onItemClickCallback(pListView, index, id);
            break;
        }
    }
}

void btsettingActivity::onSlideItemClick(ZKSlideWindow *pSlideWindow, int index) {
    int tablen = sizeof(SSlideWindowItemClickCallbackTab) / sizeof(S_SlideWindowItemClickCallback);
    for (int i = 0; i < tablen; ++i) {
        if (SSlideWindowItemClickCallbackTab[i].id == pSlideWindow->getID()) {
            SSlideWindowItemClickCallbackTab[i].onSlideItemClickCallback(pSlideWindow, index);
            break;
        }
    }
}

bool btsettingActivity::onTouchEvent(const MotionEvent &ev) {
    return onbtsettingActivityTouchEvent(ev);
}

void btsettingActivity::onTextChanged(ZKTextView *pTextView, const std::string &text) {
    int tablen = sizeof(SEditTextInputCallbackTab) / sizeof(S_EditTextInputCallback);
    for (int i = 0; i < tablen; ++i) {
        if (SEditTextInputCallbackTab[i].id == pTextView->getID()) {
            SEditTextInputCallbackTab[i].onEditTextChangedCallback(text);
            break;
        }
    }
}

void btsettingActivity::rigesterActivityTimer() {
    int tablen = sizeof(REGISTER_ACTIVITY_TIMER_TAB) / sizeof(S_ACTIVITY_TIMEER);
    for (int i = 0; i < tablen; ++i) {
        S_ACTIVITY_TIMEER temp = REGISTER_ACTIVITY_TIMER_TAB[i];
        registerTimer(temp.id, temp.time);
    }
}


void btsettingActivity::onVideoPlayerMessage(ZKVideoView *pVideoView, int msg) {
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

void btsettingActivity::videoLoopPlayback(ZKVideoView *pVideoView, int msg, size_t callbackTabIndex) {

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

void btsettingActivity::startVideoLoopPlayback() {
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

void btsettingActivity::stopVideoLoopPlayback() {
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

bool btsettingActivity::parseVideoFileList(const char *pFileListPath, std::vector<string>& mediaFileList) {
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

int btsettingActivity::removeCharFromString(string& nString, char c) {
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

void btsettingActivity::registerUserTimer(int id, int time) {
	registerTimer(id, time);
}

void btsettingActivity::unregisterUserTimer(int id) {
	unregisterTimer(id);
}

void btsettingActivity::resetUserTimer(int id, int time) {
	resetTimer(id, time);
}