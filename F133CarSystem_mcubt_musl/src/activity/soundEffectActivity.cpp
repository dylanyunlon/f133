/***********************************************
/gen auto by zuitools
***********************************************/
#include "soundEffectActivity.h"

/*TAG:GlobalVariable全局变量*/
static ZKButton* mreturnButtonPtr;
static ZKTextView* mTextView13Ptr;
static ZKTextView* mTextView12Ptr;
static ZKTextView* mTextView11Ptr;
static ZKTextView* mTextView10Ptr;
static ZKTextView* mTextView9Ptr;
static ZKTextView* mTextView8Ptr;
static ZKTextView* mTextView7Ptr;
static ZKTextView* mTextView6Ptr;
static ZKTextView* mTextView5Ptr;
static ZKTextView* mTextView4Ptr;
static ZKButton* msys_backPtr;
static ZKTextView* mTextView3Ptr;
static ZKTextView* mTextView2Ptr;
static ZKTextView* mTextView1Ptr;
static ZKSeekBar* meqSeekBar9Ptr;
static ZKSeekBar* meqSeekBar8Ptr;
static ZKSeekBar* meqSeekBar7Ptr;
static ZKSeekBar* meqSeekBar6Ptr;
static ZKSeekBar* meqSeekBar5Ptr;
static ZKSeekBar* meqSeekBar4Ptr;
static ZKSeekBar* meqSeekBar3Ptr;
static ZKSeekBar* meqSeekBar2Ptr;
static ZKSeekBar* meqSeekBar1Ptr;
static ZKSeekBar* meqSeekBar0Ptr;
static soundEffectActivity* mActivityPtr;

/*register activity*/
REGISTER_ACTIVITY(soundEffectActivity);

typedef struct {
	int id; // 定时器ID ， 不能重复
	int time; // 定时器  时间间隔  单位 毫秒
}S_ACTIVITY_TIMEER;

#include "logic/soundEffectLogic.cc"

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
    ID_SOUNDEFFECT_returnButton, onButtonClick_returnButton,
    ID_SOUNDEFFECT_sys_back, onButtonClick_sys_back,
};
/***************/


typedef void (*SeekBarCallback)(ZKSeekBar *pSeekBar, int progress);
typedef struct {
    int id;
    SeekBarCallback callback;
}S_ZKSeekBarCallback;
/*TAG:SeekBarCallbackTab*/
static S_ZKSeekBarCallback SZKSeekBarCallbackTab[] = {
    ID_SOUNDEFFECT_eqSeekBar9, onProgressChanged_eqSeekBar9,
    ID_SOUNDEFFECT_eqSeekBar8, onProgressChanged_eqSeekBar8,
    ID_SOUNDEFFECT_eqSeekBar7, onProgressChanged_eqSeekBar7,
    ID_SOUNDEFFECT_eqSeekBar6, onProgressChanged_eqSeekBar6,
    ID_SOUNDEFFECT_eqSeekBar5, onProgressChanged_eqSeekBar5,
    ID_SOUNDEFFECT_eqSeekBar4, onProgressChanged_eqSeekBar4,
    ID_SOUNDEFFECT_eqSeekBar3, onProgressChanged_eqSeekBar3,
    ID_SOUNDEFFECT_eqSeekBar2, onProgressChanged_eqSeekBar2,
    ID_SOUNDEFFECT_eqSeekBar1, onProgressChanged_eqSeekBar1,
    ID_SOUNDEFFECT_eqSeekBar0, onProgressChanged_eqSeekBar0,
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


soundEffectActivity::soundEffectActivity() {
	//todo add init code here
	mVideoLoopIndex = -1;
	mVideoLoopErrorCount = 0;
}

soundEffectActivity::~soundEffectActivity() {
  //todo add init file here
  // 退出应用时需要反注册
    EASYUICONTEXT->unregisterGlobalTouchListener(this);
    unregisterProtocolDataUpdateListener(onProtocolDataUpdate);
    onUI_quit();
    mActivityPtr = NULL;
    mreturnButtonPtr = NULL;
    mTextView13Ptr = NULL;
    mTextView12Ptr = NULL;
    mTextView11Ptr = NULL;
    mTextView10Ptr = NULL;
    mTextView9Ptr = NULL;
    mTextView8Ptr = NULL;
    mTextView7Ptr = NULL;
    mTextView6Ptr = NULL;
    mTextView5Ptr = NULL;
    mTextView4Ptr = NULL;
    msys_backPtr = NULL;
    mTextView3Ptr = NULL;
    mTextView2Ptr = NULL;
    mTextView1Ptr = NULL;
    meqSeekBar9Ptr = NULL;
    meqSeekBar8Ptr = NULL;
    meqSeekBar7Ptr = NULL;
    meqSeekBar6Ptr = NULL;
    meqSeekBar5Ptr = NULL;
    meqSeekBar4Ptr = NULL;
    meqSeekBar3Ptr = NULL;
    meqSeekBar2Ptr = NULL;
    meqSeekBar1Ptr = NULL;
    meqSeekBar0Ptr = NULL;
}

const char* soundEffectActivity::getAppName() const{
	return "soundEffect.ftu";
}

//TAG:onCreate
void soundEffectActivity::onCreate() {
	Activity::onCreate();
    mreturnButtonPtr = (ZKButton*)findControlByID(ID_SOUNDEFFECT_returnButton);
    mTextView13Ptr = (ZKTextView*)findControlByID(ID_SOUNDEFFECT_TextView13);
    mTextView12Ptr = (ZKTextView*)findControlByID(ID_SOUNDEFFECT_TextView12);
    mTextView11Ptr = (ZKTextView*)findControlByID(ID_SOUNDEFFECT_TextView11);
    mTextView10Ptr = (ZKTextView*)findControlByID(ID_SOUNDEFFECT_TextView10);
    mTextView9Ptr = (ZKTextView*)findControlByID(ID_SOUNDEFFECT_TextView9);
    mTextView8Ptr = (ZKTextView*)findControlByID(ID_SOUNDEFFECT_TextView8);
    mTextView7Ptr = (ZKTextView*)findControlByID(ID_SOUNDEFFECT_TextView7);
    mTextView6Ptr = (ZKTextView*)findControlByID(ID_SOUNDEFFECT_TextView6);
    mTextView5Ptr = (ZKTextView*)findControlByID(ID_SOUNDEFFECT_TextView5);
    mTextView4Ptr = (ZKTextView*)findControlByID(ID_SOUNDEFFECT_TextView4);
    msys_backPtr = (ZKButton*)findControlByID(ID_SOUNDEFFECT_sys_back);
    mTextView3Ptr = (ZKTextView*)findControlByID(ID_SOUNDEFFECT_TextView3);
    mTextView2Ptr = (ZKTextView*)findControlByID(ID_SOUNDEFFECT_TextView2);
    mTextView1Ptr = (ZKTextView*)findControlByID(ID_SOUNDEFFECT_TextView1);
    meqSeekBar9Ptr = (ZKSeekBar*)findControlByID(ID_SOUNDEFFECT_eqSeekBar9);if(meqSeekBar9Ptr!= NULL){meqSeekBar9Ptr->setSeekBarChangeListener(this);}
    meqSeekBar8Ptr = (ZKSeekBar*)findControlByID(ID_SOUNDEFFECT_eqSeekBar8);if(meqSeekBar8Ptr!= NULL){meqSeekBar8Ptr->setSeekBarChangeListener(this);}
    meqSeekBar7Ptr = (ZKSeekBar*)findControlByID(ID_SOUNDEFFECT_eqSeekBar7);if(meqSeekBar7Ptr!= NULL){meqSeekBar7Ptr->setSeekBarChangeListener(this);}
    meqSeekBar6Ptr = (ZKSeekBar*)findControlByID(ID_SOUNDEFFECT_eqSeekBar6);if(meqSeekBar6Ptr!= NULL){meqSeekBar6Ptr->setSeekBarChangeListener(this);}
    meqSeekBar5Ptr = (ZKSeekBar*)findControlByID(ID_SOUNDEFFECT_eqSeekBar5);if(meqSeekBar5Ptr!= NULL){meqSeekBar5Ptr->setSeekBarChangeListener(this);}
    meqSeekBar4Ptr = (ZKSeekBar*)findControlByID(ID_SOUNDEFFECT_eqSeekBar4);if(meqSeekBar4Ptr!= NULL){meqSeekBar4Ptr->setSeekBarChangeListener(this);}
    meqSeekBar3Ptr = (ZKSeekBar*)findControlByID(ID_SOUNDEFFECT_eqSeekBar3);if(meqSeekBar3Ptr!= NULL){meqSeekBar3Ptr->setSeekBarChangeListener(this);}
    meqSeekBar2Ptr = (ZKSeekBar*)findControlByID(ID_SOUNDEFFECT_eqSeekBar2);if(meqSeekBar2Ptr!= NULL){meqSeekBar2Ptr->setSeekBarChangeListener(this);}
    meqSeekBar1Ptr = (ZKSeekBar*)findControlByID(ID_SOUNDEFFECT_eqSeekBar1);if(meqSeekBar1Ptr!= NULL){meqSeekBar1Ptr->setSeekBarChangeListener(this);}
    meqSeekBar0Ptr = (ZKSeekBar*)findControlByID(ID_SOUNDEFFECT_eqSeekBar0);if(meqSeekBar0Ptr!= NULL){meqSeekBar0Ptr->setSeekBarChangeListener(this);}
	mActivityPtr = this;
	onUI_init();
  registerProtocolDataUpdateListener(onProtocolDataUpdate);
  rigesterActivityTimer();
}

void soundEffectActivity::onClick(ZKBase *pBase) {
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

void soundEffectActivity::onResume() {
	Activity::onResume();
	EASYUICONTEXT->registerGlobalTouchListener(this);
	startVideoLoopPlayback();
	onUI_show();
}

void soundEffectActivity::onPause() {
	Activity::onPause();
	EASYUICONTEXT->unregisterGlobalTouchListener(this);
	stopVideoLoopPlayback();
	onUI_hide();
}

void soundEffectActivity::onIntent(const Intent *intentPtr) {
	Activity::onIntent(intentPtr);
	onUI_intent(intentPtr);
}

bool soundEffectActivity::onTimer(int id) {
	return onUI_Timer(id);
}

void soundEffectActivity::onProgressChanged(ZKSeekBar *pSeekBar, int progress){

    int seekBarTablen = sizeof(SZKSeekBarCallbackTab) / sizeof(S_ZKSeekBarCallback);
    for (int i = 0; i < seekBarTablen; ++i) {
        if (SZKSeekBarCallbackTab[i].id == pSeekBar->getID()) {
            SZKSeekBarCallbackTab[i].callback(pSeekBar, progress);
            break;
        }
    }
}

int soundEffectActivity::getListItemCount(const ZKListView *pListView) const{
    int tablen = sizeof(SListViewFunctionsCallbackTab) / sizeof(S_ListViewFunctionsCallback);
    for (int i = 0; i < tablen; ++i) {
        if (SListViewFunctionsCallbackTab[i].id == pListView->getID()) {
            return SListViewFunctionsCallbackTab[i].getListItemCountCallback(pListView);
            break;
        }
    }
    return 0;
}

void soundEffectActivity::obtainListItemData(ZKListView *pListView,ZKListView::ZKListItem *pListItem, int index){
    int tablen = sizeof(SListViewFunctionsCallbackTab) / sizeof(S_ListViewFunctionsCallback);
    for (int i = 0; i < tablen; ++i) {
        if (SListViewFunctionsCallbackTab[i].id == pListView->getID()) {
            SListViewFunctionsCallbackTab[i].obtainListItemDataCallback(pListView, pListItem, index);
            break;
        }
    }
}

void soundEffectActivity::onItemClick(ZKListView *pListView, int index, int id){
    int tablen = sizeof(SListViewFunctionsCallbackTab) / sizeof(S_ListViewFunctionsCallback);
    for (int i = 0; i < tablen; ++i) {
        if (SListViewFunctionsCallbackTab[i].id == pListView->getID()) {
            SListViewFunctionsCallbackTab[i].onItemClickCallback(pListView, index, id);
            break;
        }
    }
}

void soundEffectActivity::onSlideItemClick(ZKSlideWindow *pSlideWindow, int index) {
    int tablen = sizeof(SSlideWindowItemClickCallbackTab) / sizeof(S_SlideWindowItemClickCallback);
    for (int i = 0; i < tablen; ++i) {
        if (SSlideWindowItemClickCallbackTab[i].id == pSlideWindow->getID()) {
            SSlideWindowItemClickCallbackTab[i].onSlideItemClickCallback(pSlideWindow, index);
            break;
        }
    }
}

bool soundEffectActivity::onTouchEvent(const MotionEvent &ev) {
    return onsoundEffectActivityTouchEvent(ev);
}

void soundEffectActivity::onTextChanged(ZKTextView *pTextView, const std::string &text) {
    int tablen = sizeof(SEditTextInputCallbackTab) / sizeof(S_EditTextInputCallback);
    for (int i = 0; i < tablen; ++i) {
        if (SEditTextInputCallbackTab[i].id == pTextView->getID()) {
            SEditTextInputCallbackTab[i].onEditTextChangedCallback(text);
            break;
        }
    }
}

void soundEffectActivity::rigesterActivityTimer() {
    int tablen = sizeof(REGISTER_ACTIVITY_TIMER_TAB) / sizeof(S_ACTIVITY_TIMEER);
    for (int i = 0; i < tablen; ++i) {
        S_ACTIVITY_TIMEER temp = REGISTER_ACTIVITY_TIMER_TAB[i];
        registerTimer(temp.id, temp.time);
    }
}


void soundEffectActivity::onVideoPlayerMessage(ZKVideoView *pVideoView, int msg) {
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

void soundEffectActivity::videoLoopPlayback(ZKVideoView *pVideoView, int msg, size_t callbackTabIndex) {

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

void soundEffectActivity::startVideoLoopPlayback() {
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

void soundEffectActivity::stopVideoLoopPlayback() {
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

bool soundEffectActivity::parseVideoFileList(const char *pFileListPath, std::vector<string>& mediaFileList) {
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

int soundEffectActivity::removeCharFromString(string& nString, char c) {
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

void soundEffectActivity::registerUserTimer(int id, int time) {
	registerTimer(id, time);
}

void soundEffectActivity::unregisterUserTimer(int id) {
	unregisterTimer(id);
}

void soundEffectActivity::resetUserTimer(int id, int time) {
	resetTimer(id, time);
}