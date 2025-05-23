/***********************************************
/gen auto by zuitools
***********************************************/
#include "wifisettingActivity.h"

/*TAG:GlobalVariable全局变量*/
static ZKTextView* mTextMacAddrPtr;
static ZKTextView* mTextIPAddrPtr;
static ZKWindow* mWindowMenuMorePtr;
static ZKWindow* mWindowPasswordErrorPtr;
static ZKTextView* mTextConnectStatePtr;
static ZKTextView* mTextConnectSecTypePtr;
static ZKTextView* mTextConnectSsidPtr;
static ZKButton* mButtonDisconnectPtr;
static ZKWindow* mWindowDisconnectPtr;
static ZKButton* mButtonShowPwdPtr;
static ZKTextView* mTextSecTypePtr;
static ZKTextView* mTextSsidPtr;
static ZKEditText* mEdittextPwdPtr;
static ZKButton* mButtonConnectPtr;
static ZKWindow* mWindowSetPtr;
static ZKButton* mButtonMenuPtr;
static ZKButton* mButtonOnOffPtr;
static ZKListView* mListViewWifiInfoPtr;
static ZKTextView* mTextview6Ptr;
static ZKTextView* mTextview2Ptr;
static ZKTextView* mTextview27Ptr;
static ZKTextView* mTextview19Ptr;
static ZKTextView* mTextview17Ptr;
static ZKTextView* mTextview15Ptr;
static ZKTextView* mTextview9Ptr;
static ZKTextView* mTextview7Ptr;
static ZKTextView* mTextview5Ptr;
static ZKTextView* mTextview8Ptr;
static ZKTextView* mTextview1Ptr;
static ZKButton* msys_backPtr;
static ZKWindow* mWindow2Ptr;
static ZKWindow* mWindow1Ptr;

/*register activity*/
REGISTER_ACTIVITY(wifisettingActivity);

typedef struct {
	int id; // 定时器ID ， 不能重复
	int time; // 定时器  时间间隔  单位 毫秒
}S_ACTIVITY_TIMEER;

#include "logic/wifisettingLogic.cc"

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
    ID_WIFISETTING_ButtonDisconnect, onButtonClick_ButtonDisconnect,
    ID_WIFISETTING_ButtonShowPwd, onButtonClick_ButtonShowPwd,
    ID_WIFISETTING_ButtonConnect, onButtonClick_ButtonConnect,
    ID_WIFISETTING_ButtonMenu, onButtonClick_ButtonMenu,
    ID_WIFISETTING_ButtonOnOff, onButtonClick_ButtonOnOff,
    ID_WIFISETTING_sys_back, onButtonClick_sys_back,
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
    ID_WIFISETTING_ListViewWifiInfo, getListItemCount_ListViewWifiInfo, obtainListItemData_ListViewWifiInfo, onListItemClick_ListViewWifiInfo,
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
    ID_WIFISETTING_EdittextPwd, onEditTextChanged_EdittextPwd,
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


wifisettingActivity::wifisettingActivity() {
	//todo add init code here
	mVideoLoopIndex = 0;
	mVideoLoopErrorCount = 0;
}

wifisettingActivity::~wifisettingActivity() {
	//todo add init file here
    // 退出应用时需要反注册
    EASYUICONTEXT->unregisterGlobalTouchListener(this);
    onUI_quit();
    unregisterProtocolDataUpdateListener(onProtocolDataUpdate);
    mTextMacAddrPtr = NULL;
    mTextIPAddrPtr = NULL;
    mTextview6Ptr = NULL;
    mTextview2Ptr = NULL;
    mWindowMenuMorePtr = NULL;
    mTextview27Ptr = NULL;
    mWindowPasswordErrorPtr = NULL;
    mTextConnectStatePtr = NULL;
    mTextConnectSecTypePtr = NULL;
    mTextConnectSsidPtr = NULL;
    mTextview19Ptr = NULL;
    mTextview17Ptr = NULL;
    mTextview15Ptr = NULL;
    mButtonDisconnectPtr = NULL;
    mWindowDisconnectPtr = NULL;
    mButtonShowPwdPtr = NULL;
    mTextSecTypePtr = NULL;
    mTextSsidPtr = NULL;
    mTextview9Ptr = NULL;
    mTextview7Ptr = NULL;
    mTextview5Ptr = NULL;
    mEdittextPwdPtr = NULL;
    mButtonConnectPtr = NULL;
    mWindowSetPtr = NULL;
    mTextview8Ptr = NULL;
    mButtonMenuPtr = NULL;
    mTextview1Ptr = NULL;
    msys_backPtr = NULL;
    mButtonOnOffPtr = NULL;
    mWindow2Ptr = NULL;
    mListViewWifiInfoPtr = NULL;
    mWindow1Ptr = NULL;
}

const char* wifisettingActivity::getAppName() const{
	return "wifisetting.ftu";
}

//TAG:onCreate
void wifisettingActivity::onCreate() {
	Activity::onCreate();
    mTextMacAddrPtr = (ZKTextView*)findControlByID(ID_WIFISETTING_TextMacAddr);
    mTextIPAddrPtr = (ZKTextView*)findControlByID(ID_WIFISETTING_TextIPAddr);
    mWindowMenuMorePtr = (ZKWindow*)findControlByID(ID_WIFISETTING_WindowMenuMore);
    mWindowPasswordErrorPtr = (ZKWindow*)findControlByID(ID_WIFISETTING_WindowPasswordError);
    mTextConnectStatePtr = (ZKTextView*)findControlByID(ID_WIFISETTING_TextConnectState);
    mTextConnectSecTypePtr = (ZKTextView*)findControlByID(ID_WIFISETTING_TextConnectSecType);
    mTextConnectSsidPtr = (ZKTextView*)findControlByID(ID_WIFISETTING_TextConnectSsid);
    mButtonDisconnectPtr = (ZKButton*)findControlByID(ID_WIFISETTING_ButtonDisconnect);
    mWindowDisconnectPtr = (ZKWindow*)findControlByID(ID_WIFISETTING_WindowDisconnect);
    mButtonShowPwdPtr = (ZKButton*)findControlByID(ID_WIFISETTING_ButtonShowPwd);
    mTextSecTypePtr = (ZKTextView*)findControlByID(ID_WIFISETTING_TextSecType);
    mTextSsidPtr = (ZKTextView*)findControlByID(ID_WIFISETTING_TextSsid);
    mEdittextPwdPtr = (ZKEditText*)findControlByID(ID_WIFISETTING_EdittextPwd);if(mEdittextPwdPtr!= NULL){mEdittextPwdPtr->setTextChangeListener(this);}
    mButtonConnectPtr = (ZKButton*)findControlByID(ID_WIFISETTING_ButtonConnect);
    mWindowSetPtr = (ZKWindow*)findControlByID(ID_WIFISETTING_WindowSet);
    mButtonMenuPtr = (ZKButton*)findControlByID(ID_WIFISETTING_ButtonMenu);
    mButtonOnOffPtr = (ZKButton*)findControlByID(ID_WIFISETTING_ButtonOnOff);
    mListViewWifiInfoPtr = (ZKListView*)findControlByID(ID_WIFISETTING_ListViewWifiInfo);if(mListViewWifiInfoPtr!= NULL){mListViewWifiInfoPtr->setListAdapter(this);mListViewWifiInfoPtr->setItemClickListener(this);}
    mTextview6Ptr = (ZKTextView*)findControlByID(ID_WIFISETTING_Textview6);
    mTextview2Ptr = (ZKTextView*)findControlByID(ID_WIFISETTING_Textview2);
    mTextview27Ptr = (ZKTextView*)findControlByID(ID_WIFISETTING_Textview27);
    mTextview19Ptr = (ZKTextView*)findControlByID(ID_WIFISETTING_Textview19);
    mTextview17Ptr = (ZKTextView*)findControlByID(ID_WIFISETTING_Textview17);
    mTextview15Ptr = (ZKTextView*)findControlByID(ID_WIFISETTING_Textview15);
    mTextview9Ptr = (ZKTextView*)findControlByID(ID_WIFISETTING_Textview9);
    mTextview7Ptr = (ZKTextView*)findControlByID(ID_WIFISETTING_Textview7);
    mTextview5Ptr = (ZKTextView*)findControlByID(ID_WIFISETTING_Textview5);
    mTextview8Ptr = (ZKTextView*)findControlByID(ID_WIFISETTING_Textview8);
    mTextview1Ptr = (ZKTextView*)findControlByID(ID_WIFISETTING_Textview1);
    msys_backPtr = (ZKButton*)findControlByID(ID_WIFISETTING_sys_back);
    mWindow2Ptr = (ZKWindow*)findControlByID(ID_WIFISETTING_Window2);
    mWindow1Ptr = (ZKWindow*)findControlByID(ID_WIFISETTING_Window1);
	onUI_init();
        // 注册监听全局触摸
    EASYUICONTEXT->registerGlobalTouchListener(this);
    registerProtocolDataUpdateListener(onProtocolDataUpdate); 

    rigesterActivityTimer();
}

void wifisettingActivity::onClick(ZKBase *pBase) {
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

void wifisettingActivity::onResume() {
	Activity::onResume();
	startVideoLoopPlayback();
}

void wifisettingActivity::onPause() {
	Activity::onPause();
	stopVideoLoopPlayback();
}

void wifisettingActivity::onIntent(const Intent *intentPtr) {
	//TODO:  
	
	Activity::onIntent(intentPtr);
}

bool wifisettingActivity::onTimer(int id) {
	return onUI_Timer(id);
}

void wifisettingActivity::onProgressChanged(ZKSeekBar *pSeekBar, int progress){

    int seekBarTablen = sizeof(SZKSeekBarCallbackTab) / sizeof(S_ZKSeekBarCallback);
    for (int i = 0; i < seekBarTablen; ++i) {
        if (SZKSeekBarCallbackTab[i].id == pSeekBar->getID()) {
            SZKSeekBarCallbackTab[i].callback(pSeekBar, progress);
            break;
        }
    }
}

int wifisettingActivity::getListItemCount(const ZKListView *pListView) const{
    int tablen = sizeof(SListViewFunctionsCallbackTab) / sizeof(S_ListViewFunctionsCallback);
    for (int i = 0; i < tablen; ++i) {
        if (SListViewFunctionsCallbackTab[i].id == pListView->getID()) {
            return SListViewFunctionsCallbackTab[i].getListItemCountCallback(pListView);
            break;
        }
    }
    return 0;
}

void wifisettingActivity::obtainListItemData(ZKListView *pListView,ZKListView::ZKListItem *pListItem, int index){
    int tablen = sizeof(SListViewFunctionsCallbackTab) / sizeof(S_ListViewFunctionsCallback);
    for (int i = 0; i < tablen; ++i) {
        if (SListViewFunctionsCallbackTab[i].id == pListView->getID()) {
            SListViewFunctionsCallbackTab[i].obtainListItemDataCallback(pListView, pListItem, index);
            break;
        }
    }
}

void wifisettingActivity::onItemClick(ZKListView *pListView, int index, int id){
    int tablen = sizeof(SListViewFunctionsCallbackTab) / sizeof(S_ListViewFunctionsCallback);
    for (int i = 0; i < tablen; ++i) {
        if (SListViewFunctionsCallbackTab[i].id == pListView->getID()) {
            SListViewFunctionsCallbackTab[i].onItemClickCallback(pListView, index, id);
            break;
        }
    }
}

void wifisettingActivity::onSlideItemClick(ZKSlideWindow *pSlideWindow, int index) {
    int tablen = sizeof(SSlideWindowItemClickCallbackTab) / sizeof(S_SlideWindowItemClickCallback);
    for (int i = 0; i < tablen; ++i) {
        if (SSlideWindowItemClickCallbackTab[i].id == pSlideWindow->getID()) {
            SSlideWindowItemClickCallbackTab[i].onSlideItemClickCallback(pSlideWindow, index);
            break;
        }
    }
}

bool wifisettingActivity::onTouchEvent(const MotionEvent &ev) {
    return onwifisettingActivityTouchEvent(ev);
}

void wifisettingActivity::onTextChanged(ZKTextView *pTextView, const std::string &text) {
    int tablen = sizeof(SEditTextInputCallbackTab) / sizeof(S_EditTextInputCallback);
    for (int i = 0; i < tablen; ++i) {
        if (SEditTextInputCallbackTab[i].id == pTextView->getID()) {
            SEditTextInputCallbackTab[i].onEditTextChangedCallback(text);
            break;
        }
    }
}

void wifisettingActivity::rigesterActivityTimer() {
    int tablen = sizeof(REGISTER_ACTIVITY_TIMER_TAB) / sizeof(S_ACTIVITY_TIMEER);
    for (int i = 0; i < tablen; ++i) {
        S_ACTIVITY_TIMEER temp = REGISTER_ACTIVITY_TIMER_TAB[i];
        registerTimer(temp.id, temp.time);
    }
}


void wifisettingActivity::onVideoPlayerMessage(ZKVideoView *pVideoView, int msg) {
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

void wifisettingActivity::videoLoopPlayback(ZKVideoView *pVideoView, int msg, int callbackTabIndex) {

	switch (msg) {
	case ZKVideoView::E_MSGTYPE_VIDEO_PLAY_STARTED:
		LOGD("ZKVideoView::E_MSGTYPE_VIDEO_PLAY_STARTED\n");
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

void wifisettingActivity::startVideoLoopPlayback() {
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

void wifisettingActivity::stopVideoLoopPlayback() {
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

bool wifisettingActivity::parseVideoFileList(const char *pFileListPath, std::vector<string>& mediaFileList) {
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
	LOGD("(f:%s, l:%d) parse fileList[%s], get [%d]files\n", __FUNCTION__,
			__LINE__, pFileListPath, mediaFileList.size());
	for (size_t i = 0; i < mediaFileList.size(); i++) {
		LOGD("file[%d]:[%s]\n", i, mediaFileList[i].c_str());
	}
	is.close();

	return true;
}

int wifisettingActivity::removeCharFromString(string& nString, char c) {
    string::size_type   pos;
    while(1)
    {
        pos = nString.find(c);
        if(pos != string::npos)
        {
            nString.erase(pos, 1);
        }
        else
        {
            break;
        }
    }
    return (int)nString.size();
}