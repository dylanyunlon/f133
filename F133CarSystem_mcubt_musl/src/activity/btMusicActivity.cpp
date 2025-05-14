/***********************************************
/gen auto by zuitools
***********************************************/
#include "btMusicActivity.h"

/*TAG:GlobalVariable全局变量*/
static ZKTextView* mTextView2Ptr;
static ZKTextView* mTextView14Ptr;
static ZKWindow* mbtTipsWindowPtr;
static ZKTextView* mTextView1Ptr;
static ZKTextView* mTextView20Ptr;
static ZKTextView* mTextView19Ptr;
static ZKTextView* mTextView18Ptr;
static ZKTextView* mTextView17Ptr;
static ZKTextView* mTextView16Ptr;
static ZKTextView* mTextView15Ptr;
static ZKButton* mbtsettingButtonPtr;
static ZKButton* mbtcontactsButtonPtr;
static ZKButton* mbtrecordButtonPtr;
static ZKButton* mphoneButtonPtr;
static ZKButton* mqueryMusicButtonPtr;
static ZKWindow* mbtAppWindowPtr;
static ZKTextView* mtittleTextViewPtr;
static ZKTextView* martistTextViewPtr;
static ZKTextView* malbumTextViewPtr;
static ZKTextView* mindexTextViewPtr;
static ZKTextView* mcurposTextViewPtr;
static ZKTextView* mdurationTextViewPtr;
static ZKButton* mnextButtonPtr;
static ZKButton* mprevButtonPtr;
static ZKButton* mplayButtonPtr;
static ZKSeekBar* mmusicSeekBarPtr;
static ZKTextView* mlyricTextViewPtr;
static btMusicActivity* mActivityPtr;

/*register activity*/
REGISTER_ACTIVITY(btMusicActivity);

typedef struct {
	int id; // 定时器ID ， 不能重复
	int time; // 定时器  时间间隔  单位 毫秒
}S_ACTIVITY_TIMEER;

#include "logic/btMusicLogic.cc"

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
    ID_BTMUSIC_btsettingButton, onButtonClick_btsettingButton,
    ID_BTMUSIC_btcontactsButton, onButtonClick_btcontactsButton,
    ID_BTMUSIC_btrecordButton, onButtonClick_btrecordButton,
    ID_BTMUSIC_phoneButton, onButtonClick_phoneButton,
    ID_BTMUSIC_queryMusicButton, onButtonClick_queryMusicButton,
    ID_BTMUSIC_nextButton, onButtonClick_nextButton,
    ID_BTMUSIC_prevButton, onButtonClick_prevButton,
    ID_BTMUSIC_playButton, onButtonClick_playButton,
};
/***************/


typedef void (*SeekBarCallback)(ZKSeekBar *pSeekBar, int progress);
typedef struct {
    int id;
    SeekBarCallback callback;
}S_ZKSeekBarCallback;
/*TAG:SeekBarCallbackTab*/
static S_ZKSeekBarCallback SZKSeekBarCallbackTab[] = {
    ID_BTMUSIC_musicSeekBar, onProgressChanged_musicSeekBar,
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


btMusicActivity::btMusicActivity() {
	//todo add init code here
	mVideoLoopIndex = -1;
	mVideoLoopErrorCount = 0;
}

btMusicActivity::~btMusicActivity() {
  //todo add init file here
  // 退出应用时需要反注册
    EASYUICONTEXT->unregisterGlobalTouchListener(this);
    unregisterProtocolDataUpdateListener(onProtocolDataUpdate);
    onUI_quit();
    mActivityPtr = NULL;
    mTextView2Ptr = NULL;
    mTextView14Ptr = NULL;
    mbtTipsWindowPtr = NULL;
    mTextView1Ptr = NULL;
    mTextView20Ptr = NULL;
    mTextView19Ptr = NULL;
    mTextView18Ptr = NULL;
    mTextView17Ptr = NULL;
    mTextView16Ptr = NULL;
    mTextView15Ptr = NULL;
    mbtsettingButtonPtr = NULL;
    mbtcontactsButtonPtr = NULL;
    mbtrecordButtonPtr = NULL;
    mphoneButtonPtr = NULL;
    mqueryMusicButtonPtr = NULL;
    mbtAppWindowPtr = NULL;
    mtittleTextViewPtr = NULL;
    martistTextViewPtr = NULL;
    malbumTextViewPtr = NULL;
    mindexTextViewPtr = NULL;
    mcurposTextViewPtr = NULL;
    mdurationTextViewPtr = NULL;
    mnextButtonPtr = NULL;
    mprevButtonPtr = NULL;
    mplayButtonPtr = NULL;
    mmusicSeekBarPtr = NULL;
    mlyricTextViewPtr = NULL;
}

const char* btMusicActivity::getAppName() const{
	return "btMusic.ftu";
}

//TAG:onCreate
void btMusicActivity::onCreate() {
	Activity::onCreate();
    mTextView2Ptr = (ZKTextView*)findControlByID(ID_BTMUSIC_TextView2);
    mTextView14Ptr = (ZKTextView*)findControlByID(ID_BTMUSIC_TextView14);
    mbtTipsWindowPtr = (ZKWindow*)findControlByID(ID_BTMUSIC_btTipsWindow);
    mTextView1Ptr = (ZKTextView*)findControlByID(ID_BTMUSIC_TextView1);
    mTextView20Ptr = (ZKTextView*)findControlByID(ID_BTMUSIC_TextView20);
    mTextView19Ptr = (ZKTextView*)findControlByID(ID_BTMUSIC_TextView19);
    mTextView18Ptr = (ZKTextView*)findControlByID(ID_BTMUSIC_TextView18);
    mTextView17Ptr = (ZKTextView*)findControlByID(ID_BTMUSIC_TextView17);
    mTextView16Ptr = (ZKTextView*)findControlByID(ID_BTMUSIC_TextView16);
    mTextView15Ptr = (ZKTextView*)findControlByID(ID_BTMUSIC_TextView15);
    mbtsettingButtonPtr = (ZKButton*)findControlByID(ID_BTMUSIC_btsettingButton);
    mbtcontactsButtonPtr = (ZKButton*)findControlByID(ID_BTMUSIC_btcontactsButton);
    mbtrecordButtonPtr = (ZKButton*)findControlByID(ID_BTMUSIC_btrecordButton);
    mphoneButtonPtr = (ZKButton*)findControlByID(ID_BTMUSIC_phoneButton);
    mqueryMusicButtonPtr = (ZKButton*)findControlByID(ID_BTMUSIC_queryMusicButton);
    mbtAppWindowPtr = (ZKWindow*)findControlByID(ID_BTMUSIC_btAppWindow);
    mtittleTextViewPtr = (ZKTextView*)findControlByID(ID_BTMUSIC_tittleTextView);
    martistTextViewPtr = (ZKTextView*)findControlByID(ID_BTMUSIC_artistTextView);
    malbumTextViewPtr = (ZKTextView*)findControlByID(ID_BTMUSIC_albumTextView);
    mindexTextViewPtr = (ZKTextView*)findControlByID(ID_BTMUSIC_indexTextView);
    mcurposTextViewPtr = (ZKTextView*)findControlByID(ID_BTMUSIC_curposTextView);
    mdurationTextViewPtr = (ZKTextView*)findControlByID(ID_BTMUSIC_durationTextView);
    mnextButtonPtr = (ZKButton*)findControlByID(ID_BTMUSIC_nextButton);
    mprevButtonPtr = (ZKButton*)findControlByID(ID_BTMUSIC_prevButton);
    mplayButtonPtr = (ZKButton*)findControlByID(ID_BTMUSIC_playButton);
    mmusicSeekBarPtr = (ZKSeekBar*)findControlByID(ID_BTMUSIC_musicSeekBar);if(mmusicSeekBarPtr!= NULL){mmusicSeekBarPtr->setSeekBarChangeListener(this);}
    mlyricTextViewPtr = (ZKTextView*)findControlByID(ID_BTMUSIC_lyricTextView);
	mActivityPtr = this;
	onUI_init();
  registerProtocolDataUpdateListener(onProtocolDataUpdate);
  rigesterActivityTimer();
}

void btMusicActivity::onClick(ZKBase *pBase) {
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

void btMusicActivity::onResume() {
	Activity::onResume();
	EASYUICONTEXT->registerGlobalTouchListener(this);
	startVideoLoopPlayback();
	onUI_show();
}

void btMusicActivity::onPause() {
	Activity::onPause();
	EASYUICONTEXT->unregisterGlobalTouchListener(this);
	stopVideoLoopPlayback();
	onUI_hide();
}

void btMusicActivity::onIntent(const Intent *intentPtr) {
	Activity::onIntent(intentPtr);
	onUI_intent(intentPtr);
}

bool btMusicActivity::onTimer(int id) {
	return onUI_Timer(id);
}

void btMusicActivity::onProgressChanged(ZKSeekBar *pSeekBar, int progress){

    int seekBarTablen = sizeof(SZKSeekBarCallbackTab) / sizeof(S_ZKSeekBarCallback);
    for (int i = 0; i < seekBarTablen; ++i) {
        if (SZKSeekBarCallbackTab[i].id == pSeekBar->getID()) {
            SZKSeekBarCallbackTab[i].callback(pSeekBar, progress);
            break;
        }
    }
}

int btMusicActivity::getListItemCount(const ZKListView *pListView) const{
    int tablen = sizeof(SListViewFunctionsCallbackTab) / sizeof(S_ListViewFunctionsCallback);
    for (int i = 0; i < tablen; ++i) {
        if (SListViewFunctionsCallbackTab[i].id == pListView->getID()) {
            return SListViewFunctionsCallbackTab[i].getListItemCountCallback(pListView);
            break;
        }
    }
    return 0;
}

void btMusicActivity::obtainListItemData(ZKListView *pListView,ZKListView::ZKListItem *pListItem, int index){
    int tablen = sizeof(SListViewFunctionsCallbackTab) / sizeof(S_ListViewFunctionsCallback);
    for (int i = 0; i < tablen; ++i) {
        if (SListViewFunctionsCallbackTab[i].id == pListView->getID()) {
            SListViewFunctionsCallbackTab[i].obtainListItemDataCallback(pListView, pListItem, index);
            break;
        }
    }
}

void btMusicActivity::onItemClick(ZKListView *pListView, int index, int id){
    int tablen = sizeof(SListViewFunctionsCallbackTab) / sizeof(S_ListViewFunctionsCallback);
    for (int i = 0; i < tablen; ++i) {
        if (SListViewFunctionsCallbackTab[i].id == pListView->getID()) {
            SListViewFunctionsCallbackTab[i].onItemClickCallback(pListView, index, id);
            break;
        }
    }
}

void btMusicActivity::onSlideItemClick(ZKSlideWindow *pSlideWindow, int index) {
    int tablen = sizeof(SSlideWindowItemClickCallbackTab) / sizeof(S_SlideWindowItemClickCallback);
    for (int i = 0; i < tablen; ++i) {
        if (SSlideWindowItemClickCallbackTab[i].id == pSlideWindow->getID()) {
            SSlideWindowItemClickCallbackTab[i].onSlideItemClickCallback(pSlideWindow, index);
            break;
        }
    }
}

bool btMusicActivity::onTouchEvent(const MotionEvent &ev) {
    return onbtMusicActivityTouchEvent(ev);
}

void btMusicActivity::onTextChanged(ZKTextView *pTextView, const std::string &text) {
    int tablen = sizeof(SEditTextInputCallbackTab) / sizeof(S_EditTextInputCallback);
    for (int i = 0; i < tablen; ++i) {
        if (SEditTextInputCallbackTab[i].id == pTextView->getID()) {
            SEditTextInputCallbackTab[i].onEditTextChangedCallback(text);
            break;
        }
    }
}

void btMusicActivity::rigesterActivityTimer() {
    int tablen = sizeof(REGISTER_ACTIVITY_TIMER_TAB) / sizeof(S_ACTIVITY_TIMEER);
    for (int i = 0; i < tablen; ++i) {
        S_ACTIVITY_TIMEER temp = REGISTER_ACTIVITY_TIMER_TAB[i];
        registerTimer(temp.id, temp.time);
    }
}


void btMusicActivity::onVideoPlayerMessage(ZKVideoView *pVideoView, int msg) {
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

void btMusicActivity::videoLoopPlayback(ZKVideoView *pVideoView, int msg, size_t callbackTabIndex) {

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

void btMusicActivity::startVideoLoopPlayback() {
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

void btMusicActivity::stopVideoLoopPlayback() {
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

bool btMusicActivity::parseVideoFileList(const char *pFileListPath, std::vector<string>& mediaFileList) {
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

int btMusicActivity::removeCharFromString(string& nString, char c) {
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

void btMusicActivity::registerUserTimer(int id, int time) {
	registerTimer(id, time);
}

void btMusicActivity::unregisterUserTimer(int id) {
	unregisterTimer(id);
}

void btMusicActivity::resetUserTimer(int id, int time) {
	resetTimer(id, time);
}