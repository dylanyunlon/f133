#include "navibar.h"
#include "entry/EasyUIContext.h"

/*TAG:GlobalVariable全局变量*/
static ZKButton* mscreenoffBtnPtr;
static ZKDigitalClock* mDigitalClock1Ptr;
static ZKTextView* mTextView7Ptr;
static ZKTextView* mweekTextViewPtr;
static ZKTextView* mdataTextViewPtr;
static ZKTextView* mbtTextViewPtr;
static ZKTextView* mfmTextViewPtr;
static ZKCircleBar* mcacheCircleBarPtr;
static ZKButton* mreleaseButtonPtr;
static ZKTextView* mTextView2Ptr;
static ZKButton* mbtButtonPtr;
static ZKTextView* mTextView1Ptr;
static ZKButton* mfmButtonPtr;
static ZKTextView* mTextviewDDRPtr;
static ZKTextView* mTextView5Ptr;
static ZKButton* msys_homePtr;
static ZKButton* mbriButtonPtr;
static ZKButton* mcallButtonPtr;
static ZKButton* msoundButtonPtr;
static ZKSeekBar* mbriSeekBarPtr;
static ZKSeekBar* mCpSeekBarPtr;
static ZKSeekBar* mPlayVolSeekBarPtr;
static ZKWindow* mbarWindowPtr;
static ZKWindow* mbuttomWindowPtr;
static navibar* mnavibarPtr;
/* register sysapp */
REGISTER_SYSAPP(APP_TYPE_SYS_NAVIBAR, navibar);

typedef struct {
	int id; // 定时器ID ， 不能重复
	int time; // 定时器  时间间隔  单位 毫秒
}S_ACTIVITY_TIMEER;

#include "logic/navibar.cc"


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
    ID_NAVIBAR_screenoffBtn, onButtonClick_screenoffBtn,
    ID_NAVIBAR_releaseButton, onButtonClick_releaseButton,
    ID_NAVIBAR_btButton, onButtonClick_btButton,
    ID_NAVIBAR_fmButton, onButtonClick_fmButton,
    ID_NAVIBAR_sys_home, onButtonClick_sys_home,
    ID_NAVIBAR_briButton, onButtonClick_briButton,
    ID_NAVIBAR_callButton, onButtonClick_callButton,
    ID_NAVIBAR_soundButton, onButtonClick_soundButton,
};
/***************/


typedef void (*SeekBarCallback)(ZKSeekBar *pSeekBar, int progress);
typedef struct {
    int id;
    SeekBarCallback callback;
}S_ZKSeekBarCallback;
/*TAG:SeekBarCallbackTab*/
static S_ZKSeekBarCallback SZKSeekBarCallbackTab[] = {
    ID_NAVIBAR_briSeekBar, onProgressChanged_briSeekBar,
    ID_NAVIBAR_CpSeekBar, onProgressChanged_CpSeekBar,
    ID_NAVIBAR_PlayVolSeekBar, onProgressChanged_PlayVolSeekBar,
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

navibar::navibar() {
	mVideoLoopIndex = -1;
	mVideoLoopErrorCount = 0;
}

navibar::~navibar() {
    // 退出应用时需要反注册
    EASYUICONTEXT->unregisterGlobalTouchListener(this);
    unregisterProtocolDataUpdateListener(onProtocolDataUpdate);
    onUI_quit();
    mnavibarPtr = NULL;
    mscreenoffBtnPtr = NULL;
    mDigitalClock1Ptr = NULL;
    mTextView7Ptr = NULL;
    mweekTextViewPtr = NULL;
    mdataTextViewPtr = NULL;
    mbtTextViewPtr = NULL;
    mfmTextViewPtr = NULL;
    mcacheCircleBarPtr = NULL;
    mreleaseButtonPtr = NULL;
    mTextView2Ptr = NULL;
    mbtButtonPtr = NULL;
    mTextView1Ptr = NULL;
    mfmButtonPtr = NULL;
    mTextviewDDRPtr = NULL;
    mTextView5Ptr = NULL;
    msys_homePtr = NULL;
    mbriButtonPtr = NULL;
    mcallButtonPtr = NULL;
    msoundButtonPtr = NULL;
    mbriSeekBarPtr = NULL;
    mCpSeekBarPtr = NULL;
    mPlayVolSeekBarPtr = NULL;
    mbarWindowPtr = NULL;
    mbuttomWindowPtr = NULL;
}

const char* navibar::getAppName() const{
    return "navibar.ftu";
}

//TAG:onCreate
void navibar::onCreate() {
	BaseApp::onCreate();
    mscreenoffBtnPtr = (ZKButton*)findControlByID(ID_NAVIBAR_screenoffBtn);
    mDigitalClock1Ptr = (ZKDigitalClock*)findControlByID(ID_NAVIBAR_DigitalClock1);
    mTextView7Ptr = (ZKTextView*)findControlByID(ID_NAVIBAR_TextView7);
    mweekTextViewPtr = (ZKTextView*)findControlByID(ID_NAVIBAR_weekTextView);
    mdataTextViewPtr = (ZKTextView*)findControlByID(ID_NAVIBAR_dataTextView);
    mbtTextViewPtr = (ZKTextView*)findControlByID(ID_NAVIBAR_btTextView);
    mfmTextViewPtr = (ZKTextView*)findControlByID(ID_NAVIBAR_fmTextView);
    mcacheCircleBarPtr = (ZKCircleBar*)findControlByID(ID_NAVIBAR_cacheCircleBar);
    mreleaseButtonPtr = (ZKButton*)findControlByID(ID_NAVIBAR_releaseButton);
    mTextView2Ptr = (ZKTextView*)findControlByID(ID_NAVIBAR_TextView2);
    mbtButtonPtr = (ZKButton*)findControlByID(ID_NAVIBAR_btButton);
    mTextView1Ptr = (ZKTextView*)findControlByID(ID_NAVIBAR_TextView1);
    mfmButtonPtr = (ZKButton*)findControlByID(ID_NAVIBAR_fmButton);
    mTextviewDDRPtr = (ZKTextView*)findControlByID(ID_NAVIBAR_TextviewDDR);
    mTextView5Ptr = (ZKTextView*)findControlByID(ID_NAVIBAR_TextView5);
    msys_homePtr = (ZKButton*)findControlByID(ID_NAVIBAR_sys_home);
    mbriButtonPtr = (ZKButton*)findControlByID(ID_NAVIBAR_briButton);
    mcallButtonPtr = (ZKButton*)findControlByID(ID_NAVIBAR_callButton);
    msoundButtonPtr = (ZKButton*)findControlByID(ID_NAVIBAR_soundButton);
    mbriSeekBarPtr = (ZKSeekBar*)findControlByID(ID_NAVIBAR_briSeekBar);if(mbriSeekBarPtr!= NULL){mbriSeekBarPtr->setSeekBarChangeListener(this);}
    mCpSeekBarPtr = (ZKSeekBar*)findControlByID(ID_NAVIBAR_CpSeekBar);if(mCpSeekBarPtr!= NULL){mCpSeekBarPtr->setSeekBarChangeListener(this);}
    mPlayVolSeekBarPtr = (ZKSeekBar*)findControlByID(ID_NAVIBAR_PlayVolSeekBar);if(mPlayVolSeekBarPtr!= NULL){mPlayVolSeekBarPtr->setSeekBarChangeListener(this);}
    mbarWindowPtr = (ZKWindow*)findControlByID(ID_NAVIBAR_barWindow);
    mbuttomWindowPtr = (ZKWindow*)findControlByID(ID_NAVIBAR_buttomWindow);
	mnavibarPtr = this;
	onUI_init();
    // 注册监听全局触摸
    EASYUICONTEXT->registerGlobalTouchListener(this);
    registerProtocolDataUpdateListener(onProtocolDataUpdate); 
    rigesterActivityTimer();
    startVideoLoopPlayback();
}

void navibar::onClick(ZKBase *pBase) {
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

	BaseApp::onClick(pBase);
}

bool navibar::onTimer(int id) {
    return onUI_Timer(id);
}


void navibar::onProgressChanged(ZKSeekBar *pSeekBar, int progress){

    int seekBarTablen = sizeof(SZKSeekBarCallbackTab) / sizeof(S_ZKSeekBarCallback);
    for (int i = 0; i < seekBarTablen; ++i) {
        if (SZKSeekBarCallbackTab[i].id == pSeekBar->getID()) {
            SZKSeekBarCallbackTab[i].callback(pSeekBar, progress);
            break;
        }
    }
}

int navibar::getListItemCount(const ZKListView *pListView) const{
    int tablen = sizeof(SListViewFunctionsCallbackTab) / sizeof(S_ListViewFunctionsCallback);
    for (int i = 0; i < tablen; ++i) {
        if (SListViewFunctionsCallbackTab[i].id == pListView->getID()) {
            return SListViewFunctionsCallbackTab[i].getListItemCountCallback(pListView);
            break;
        }
    }
    return 0;
}
void navibar::obtainListItemData(ZKListView *pListView,ZKListView::ZKListItem *pListItem, int index){
    int tablen = sizeof(SListViewFunctionsCallbackTab) / sizeof(S_ListViewFunctionsCallback);
    for (int i = 0; i < tablen; ++i) {
        if (SListViewFunctionsCallbackTab[i].id == pListView->getID()) {
            SListViewFunctionsCallbackTab[i].obtainListItemDataCallback(pListView, pListItem, index);
            break;
        }
    }
}
void navibar::onItemClick(ZKListView *pListView, int index, int id){
    int tablen = sizeof(SListViewFunctionsCallbackTab) / sizeof(S_ListViewFunctionsCallback);
    for (int i = 0; i < tablen; ++i) {
        if (SListViewFunctionsCallbackTab[i].id == pListView->getID()) {
            SListViewFunctionsCallbackTab[i].onItemClickCallback(pListView, index, id);
            break;
        }
    }
}

void navibar::onSlideItemClick(ZKSlideWindow *pSlideWindow, int index) {
    int tablen = sizeof(SSlideWindowItemClickCallbackTab) / sizeof(S_SlideWindowItemClickCallback);
    for (int i = 0; i < tablen; ++i) {
        if (SSlideWindowItemClickCallbackTab[i].id == pSlideWindow->getID()) {
            SSlideWindowItemClickCallbackTab[i].onSlideItemClickCallback(pSlideWindow, index);
            break;
        }
    }
}

bool navibar::onTouchEvent(const MotionEvent &ev) {
    return onnavibarActivityTouchEvent(ev);
}

void navibar::onTextChanged(ZKTextView *pTextView, const std::string &text) {
    int tablen = sizeof(SEditTextInputCallbackTab) / sizeof(S_EditTextInputCallback);
    for (int i = 0; i < tablen; ++i) {
        if (SEditTextInputCallbackTab[i].id == pTextView->getID()) {
            SEditTextInputCallbackTab[i].onEditTextChangedCallback(text);
            break;
        }
    }
}

void navibar::rigesterActivityTimer() {
    int tablen = sizeof(REGISTER_ACTIVITY_TIMER_TAB) / sizeof(S_ACTIVITY_TIMEER);
    for (int i = 0; i < tablen; ++i) {
        S_ACTIVITY_TIMEER temp = REGISTER_ACTIVITY_TIMER_TAB[i];
        registerTimer(temp.id, temp.time);
    }
}


void navibar::onVideoPlayerMessage(ZKVideoView *pVideoView, int msg) {
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

void navibar::videoLoopPlayback(ZKVideoView *pVideoView, int msg, size_t callbackTabIndex) {

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

void navibar::startVideoLoopPlayback() {
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

void navibar::stopVideoLoopPlayback() {
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

bool navibar::parseVideoFileList(const char *pFileListPath, std::vector<string>& mediaFileList) {
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

int navibar::removeCharFromString(string& nString, char c)
{
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

void navibar::registerUserTimer(int id, int time) {
	registerTimer(id, time);
}

void navibar::unregisterUserTimer(int id) {
	unregisterTimer(id);
}

void navibar::resetUserTimer(int id, int time) {
	resetTimer(id, time);
}
