/***********************************************
/gen auto by zuitools
***********************************************/
#include "callingActivity.h"

/*TAG:GlobalVariable全局变量*/
static ZKTextView* mnumberTextViewPtr;
static ZKTextView* mTextView15Ptr;
static ZKButton* mbtsettingButtonPtr;
static ZKButton* mbtcontactsButtonPtr;
static ZKButton* mbtrecordButtonPtr;
static ZKButton* mphoneButtonPtr;
static ZKButton* mqueryMusicButtonPtr;
static ZKWindow* mbtAppWindowPtr;
static ZKButton* mouthangupButtonPtr;
static ZKWindow* moutWindowPtr;
static ZKButton* minanswerButtonPtr;
static ZKButton* minhangupButtonPtr;
static ZKWindow* minWindowPtr;
static ZKWindow* mInfoWndPtr;
static ZKButton* mdelButtonPtr;
static ZKButton* mkeyXINGButtonPtr;
static ZKButton* mkeyJINGButtonPtr;
static ZKButton* mkey0ButtonPtr;
static ZKButton* mkey9ButtonPtr;
static ZKButton* mkey8ButtonPtr;
static ZKButton* mkey7ButtonPtr;
static ZKButton* mkey6ButtonPtr;
static ZKButton* mkey5ButtonPtr;
static ZKButton* mkey4ButtonPtr;
static ZKButton* mkey3ButtonPtr;
static ZKButton* mkey2ButtonPtr;
static ZKButton* mkey1ButtonPtr;
static ZKTextView* mnumTextViewPtr;
static ZKWindow* mkeyBoardWindowPtr;
static ZKButton* mstopMicButtonPtr;
static ZKButton* mkeyBoardButtonPtr;
static ZKButton* msoundButtonPtr;
static ZKWindow* mcommunicateWindowPtr;
static ZKTextView* mcallTimeTextViewPtr;
static ZKTextView* mnameTextViewPtr;
static ZKTextView* mheadTextViewPtr;
static ZKButton* mhangupButtonPtr;
static callingActivity* mActivityPtr;

/*register activity*/
REGISTER_ACTIVITY(callingActivity);

typedef struct {
	int id; // 定时器ID ， 不能重复
	int time; // 定时器  时间间隔  单位 毫秒
}S_ACTIVITY_TIMEER;

#include "logic/callingLogic.cc"

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
    ID_CALLING_btsettingButton, onButtonClick_btsettingButton,
    ID_CALLING_btcontactsButton, onButtonClick_btcontactsButton,
    ID_CALLING_btrecordButton, onButtonClick_btrecordButton,
    ID_CALLING_phoneButton, onButtonClick_phoneButton,
    ID_CALLING_queryMusicButton, onButtonClick_queryMusicButton,
    ID_CALLING_outhangupButton, onButtonClick_outhangupButton,
    ID_CALLING_inanswerButton, onButtonClick_inanswerButton,
    ID_CALLING_inhangupButton, onButtonClick_inhangupButton,
    ID_CALLING_delButton, onButtonClick_delButton,
    ID_CALLING_keyXINGButton, onButtonClick_keyXINGButton,
    ID_CALLING_keyJINGButton, onButtonClick_keyJINGButton,
    ID_CALLING_key0Button, onButtonClick_key0Button,
    ID_CALLING_key9Button, onButtonClick_key9Button,
    ID_CALLING_key8Button, onButtonClick_key8Button,
    ID_CALLING_key7Button, onButtonClick_key7Button,
    ID_CALLING_key6Button, onButtonClick_key6Button,
    ID_CALLING_key5Button, onButtonClick_key5Button,
    ID_CALLING_key4Button, onButtonClick_key4Button,
    ID_CALLING_key3Button, onButtonClick_key3Button,
    ID_CALLING_key2Button, onButtonClick_key2Button,
    ID_CALLING_key1Button, onButtonClick_key1Button,
    ID_CALLING_stopMicButton, onButtonClick_stopMicButton,
    ID_CALLING_keyBoardButton, onButtonClick_keyBoardButton,
    ID_CALLING_soundButton, onButtonClick_soundButton,
    ID_CALLING_hangupButton, onButtonClick_hangupButton,
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


callingActivity::callingActivity() {
	//todo add init code here
	mVideoLoopIndex = -1;
	mVideoLoopErrorCount = 0;
}

callingActivity::~callingActivity() {
  //todo add init file here
  // 退出应用时需要反注册
    EASYUICONTEXT->unregisterGlobalTouchListener(this);
    unregisterProtocolDataUpdateListener(onProtocolDataUpdate);
    onUI_quit();
    mActivityPtr = NULL;
    mnumberTextViewPtr = NULL;
    mTextView15Ptr = NULL;
    mbtsettingButtonPtr = NULL;
    mbtcontactsButtonPtr = NULL;
    mbtrecordButtonPtr = NULL;
    mphoneButtonPtr = NULL;
    mqueryMusicButtonPtr = NULL;
    mbtAppWindowPtr = NULL;
    mouthangupButtonPtr = NULL;
    moutWindowPtr = NULL;
    minanswerButtonPtr = NULL;
    minhangupButtonPtr = NULL;
    minWindowPtr = NULL;
    mInfoWndPtr = NULL;
    mdelButtonPtr = NULL;
    mkeyXINGButtonPtr = NULL;
    mkeyJINGButtonPtr = NULL;
    mkey0ButtonPtr = NULL;
    mkey9ButtonPtr = NULL;
    mkey8ButtonPtr = NULL;
    mkey7ButtonPtr = NULL;
    mkey6ButtonPtr = NULL;
    mkey5ButtonPtr = NULL;
    mkey4ButtonPtr = NULL;
    mkey3ButtonPtr = NULL;
    mkey2ButtonPtr = NULL;
    mkey1ButtonPtr = NULL;
    mnumTextViewPtr = NULL;
    mkeyBoardWindowPtr = NULL;
    mstopMicButtonPtr = NULL;
    mkeyBoardButtonPtr = NULL;
    msoundButtonPtr = NULL;
    mcommunicateWindowPtr = NULL;
    mcallTimeTextViewPtr = NULL;
    mnameTextViewPtr = NULL;
    mheadTextViewPtr = NULL;
    mhangupButtonPtr = NULL;
}

const char* callingActivity::getAppName() const{
	return "calling.ftu";
}

//TAG:onCreate
void callingActivity::onCreate() {
	Activity::onCreate();
    mnumberTextViewPtr = (ZKTextView*)findControlByID(ID_CALLING_numberTextView);
    mTextView15Ptr = (ZKTextView*)findControlByID(ID_CALLING_TextView15);
    mbtsettingButtonPtr = (ZKButton*)findControlByID(ID_CALLING_btsettingButton);
    mbtcontactsButtonPtr = (ZKButton*)findControlByID(ID_CALLING_btcontactsButton);
    mbtrecordButtonPtr = (ZKButton*)findControlByID(ID_CALLING_btrecordButton);
    mphoneButtonPtr = (ZKButton*)findControlByID(ID_CALLING_phoneButton);
    mqueryMusicButtonPtr = (ZKButton*)findControlByID(ID_CALLING_queryMusicButton);
    mbtAppWindowPtr = (ZKWindow*)findControlByID(ID_CALLING_btAppWindow);
    mouthangupButtonPtr = (ZKButton*)findControlByID(ID_CALLING_outhangupButton);
    moutWindowPtr = (ZKWindow*)findControlByID(ID_CALLING_outWindow);
    minanswerButtonPtr = (ZKButton*)findControlByID(ID_CALLING_inanswerButton);
    minhangupButtonPtr = (ZKButton*)findControlByID(ID_CALLING_inhangupButton);
    minWindowPtr = (ZKWindow*)findControlByID(ID_CALLING_inWindow);
    mInfoWndPtr = (ZKWindow*)findControlByID(ID_CALLING_InfoWnd);
    mdelButtonPtr = (ZKButton*)findControlByID(ID_CALLING_delButton);
    mkeyXINGButtonPtr = (ZKButton*)findControlByID(ID_CALLING_keyXINGButton);
    mkeyJINGButtonPtr = (ZKButton*)findControlByID(ID_CALLING_keyJINGButton);
    mkey0ButtonPtr = (ZKButton*)findControlByID(ID_CALLING_key0Button);
    mkey9ButtonPtr = (ZKButton*)findControlByID(ID_CALLING_key9Button);
    mkey8ButtonPtr = (ZKButton*)findControlByID(ID_CALLING_key8Button);
    mkey7ButtonPtr = (ZKButton*)findControlByID(ID_CALLING_key7Button);
    mkey6ButtonPtr = (ZKButton*)findControlByID(ID_CALLING_key6Button);
    mkey5ButtonPtr = (ZKButton*)findControlByID(ID_CALLING_key5Button);
    mkey4ButtonPtr = (ZKButton*)findControlByID(ID_CALLING_key4Button);
    mkey3ButtonPtr = (ZKButton*)findControlByID(ID_CALLING_key3Button);
    mkey2ButtonPtr = (ZKButton*)findControlByID(ID_CALLING_key2Button);
    mkey1ButtonPtr = (ZKButton*)findControlByID(ID_CALLING_key1Button);
    mnumTextViewPtr = (ZKTextView*)findControlByID(ID_CALLING_numTextView);
    mkeyBoardWindowPtr = (ZKWindow*)findControlByID(ID_CALLING_keyBoardWindow);
    mstopMicButtonPtr = (ZKButton*)findControlByID(ID_CALLING_stopMicButton);
    mkeyBoardButtonPtr = (ZKButton*)findControlByID(ID_CALLING_keyBoardButton);
    msoundButtonPtr = (ZKButton*)findControlByID(ID_CALLING_soundButton);
    mcommunicateWindowPtr = (ZKWindow*)findControlByID(ID_CALLING_communicateWindow);
    mcallTimeTextViewPtr = (ZKTextView*)findControlByID(ID_CALLING_callTimeTextView);
    mnameTextViewPtr = (ZKTextView*)findControlByID(ID_CALLING_nameTextView);
    mheadTextViewPtr = (ZKTextView*)findControlByID(ID_CALLING_headTextView);
    mhangupButtonPtr = (ZKButton*)findControlByID(ID_CALLING_hangupButton);
	mActivityPtr = this;
	onUI_init();
  registerProtocolDataUpdateListener(onProtocolDataUpdate);
  rigesterActivityTimer();
}

void callingActivity::onClick(ZKBase *pBase) {
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

void callingActivity::onResume() {
	Activity::onResume();
	EASYUICONTEXT->registerGlobalTouchListener(this);
	startVideoLoopPlayback();
	onUI_show();
}

void callingActivity::onPause() {
	Activity::onPause();
	EASYUICONTEXT->unregisterGlobalTouchListener(this);
	stopVideoLoopPlayback();
	onUI_hide();
}

void callingActivity::onIntent(const Intent *intentPtr) {
	Activity::onIntent(intentPtr);
	onUI_intent(intentPtr);
}

bool callingActivity::onTimer(int id) {
	return onUI_Timer(id);
}

void callingActivity::onProgressChanged(ZKSeekBar *pSeekBar, int progress){

    int seekBarTablen = sizeof(SZKSeekBarCallbackTab) / sizeof(S_ZKSeekBarCallback);
    for (int i = 0; i < seekBarTablen; ++i) {
        if (SZKSeekBarCallbackTab[i].id == pSeekBar->getID()) {
            SZKSeekBarCallbackTab[i].callback(pSeekBar, progress);
            break;
        }
    }
}

int callingActivity::getListItemCount(const ZKListView *pListView) const{
    int tablen = sizeof(SListViewFunctionsCallbackTab) / sizeof(S_ListViewFunctionsCallback);
    for (int i = 0; i < tablen; ++i) {
        if (SListViewFunctionsCallbackTab[i].id == pListView->getID()) {
            return SListViewFunctionsCallbackTab[i].getListItemCountCallback(pListView);
            break;
        }
    }
    return 0;
}

void callingActivity::obtainListItemData(ZKListView *pListView,ZKListView::ZKListItem *pListItem, int index){
    int tablen = sizeof(SListViewFunctionsCallbackTab) / sizeof(S_ListViewFunctionsCallback);
    for (int i = 0; i < tablen; ++i) {
        if (SListViewFunctionsCallbackTab[i].id == pListView->getID()) {
            SListViewFunctionsCallbackTab[i].obtainListItemDataCallback(pListView, pListItem, index);
            break;
        }
    }
}

void callingActivity::onItemClick(ZKListView *pListView, int index, int id){
    int tablen = sizeof(SListViewFunctionsCallbackTab) / sizeof(S_ListViewFunctionsCallback);
    for (int i = 0; i < tablen; ++i) {
        if (SListViewFunctionsCallbackTab[i].id == pListView->getID()) {
            SListViewFunctionsCallbackTab[i].onItemClickCallback(pListView, index, id);
            break;
        }
    }
}

void callingActivity::onSlideItemClick(ZKSlideWindow *pSlideWindow, int index) {
    int tablen = sizeof(SSlideWindowItemClickCallbackTab) / sizeof(S_SlideWindowItemClickCallback);
    for (int i = 0; i < tablen; ++i) {
        if (SSlideWindowItemClickCallbackTab[i].id == pSlideWindow->getID()) {
            SSlideWindowItemClickCallbackTab[i].onSlideItemClickCallback(pSlideWindow, index);
            break;
        }
    }
}

bool callingActivity::onTouchEvent(const MotionEvent &ev) {
    return oncallingActivityTouchEvent(ev);
}

void callingActivity::onTextChanged(ZKTextView *pTextView, const std::string &text) {
    int tablen = sizeof(SEditTextInputCallbackTab) / sizeof(S_EditTextInputCallback);
    for (int i = 0; i < tablen; ++i) {
        if (SEditTextInputCallbackTab[i].id == pTextView->getID()) {
            SEditTextInputCallbackTab[i].onEditTextChangedCallback(text);
            break;
        }
    }
}

void callingActivity::rigesterActivityTimer() {
    int tablen = sizeof(REGISTER_ACTIVITY_TIMER_TAB) / sizeof(S_ACTIVITY_TIMEER);
    for (int i = 0; i < tablen; ++i) {
        S_ACTIVITY_TIMEER temp = REGISTER_ACTIVITY_TIMER_TAB[i];
        registerTimer(temp.id, temp.time);
    }
}


void callingActivity::onVideoPlayerMessage(ZKVideoView *pVideoView, int msg) {
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

void callingActivity::videoLoopPlayback(ZKVideoView *pVideoView, int msg, size_t callbackTabIndex) {

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

void callingActivity::startVideoLoopPlayback() {
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

void callingActivity::stopVideoLoopPlayback() {
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

bool callingActivity::parseVideoFileList(const char *pFileListPath, std::vector<string>& mediaFileList) {
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

int callingActivity::removeCharFromString(string& nString, char c) {
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

void callingActivity::registerUserTimer(int id, int time) {
	registerTimer(id, time);
}

void callingActivity::unregisterUserTimer(int id) {
	unregisterTimer(id);
}

void callingActivity::resetUserTimer(int id, int time) {
	resetTimer(id, time);
}