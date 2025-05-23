/***********************************************
/gen auto by zuitools
***********************************************/
#include "tirePressureActivity.h"

/*TAG:GlobalVariable全局变量*/
static ZKButton* msys_backPtr;
static ZKButton* msaveOptionButtonPtr;
static ZKButton* msaveDataButtonPtr;
static ZKListView* mtireMatchListViewPtr;
static ZKButton* mrearButtonPtr;
static ZKButton* mfrontButtonPtr;
static ZKButton* mresetButtonPtr;
static ZKButton* mCnextButtonPtr;
static ZKButton* mCprevButtonPtr;
static ZKTextView* mCTextViewPtr;
static ZKTextView* mTextView7Ptr;
static ZKButton* mlvnextButtonPtr;
static ZKButton* mlvprevButtonPtr;
static ZKTextView* mlvTextViewPtr;
static ZKTextView* mTextView8Ptr;
static ZKButton* mhvnextButtonPtr;
static ZKButton* mhvprevButtonPtr;
static ZKTextView* mhvTextViewPtr;
static ZKTextView* mTextView6Ptr;
static ZKButton* munitnextButtonPtr;
static ZKButton* munitprevButtonPtr;
static ZKTextView* munitTextViewPtr;
static ZKTextView* mTextView5Ptr;
static ZKTextView* mTextView4Ptr;
static ZKTextView* mTextView3Ptr;
static ZKTextView* mTextView2Ptr;
static ZKButton* mclearButtonPtr;
static ZKTextView* mrearstateTextViewPtr;
static ZKTextView* mfrontstateTextViewPtr;
static ZKTextView* mrearCTextViewPtr;
static ZKTextView* mfrontCViewPtr;
static ZKTextView* mrearpsiTextViewPtr;
static ZKTextView* mfrontpsiTextViewPtr;
static ZKTextView* mrearTextViewPtr;
static ZKTextView* mfrontTextViewPtr;
static ZKTextView* mTextView1Ptr;
static tirePressureActivity* mActivityPtr;

/*register activity*/
REGISTER_ACTIVITY(tirePressureActivity);

typedef struct {
	int id; // 定时器ID ， 不能重复
	int time; // 定时器  时间间隔  单位 毫秒
}S_ACTIVITY_TIMEER;

#include "logic/tirePressureLogic.cc"

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
    ID_TIREPRESSURE_sys_back, onButtonClick_sys_back,
    ID_TIREPRESSURE_saveOptionButton, onButtonClick_saveOptionButton,
    ID_TIREPRESSURE_saveDataButton, onButtonClick_saveDataButton,
    ID_TIREPRESSURE_rearButton, onButtonClick_rearButton,
    ID_TIREPRESSURE_frontButton, onButtonClick_frontButton,
    ID_TIREPRESSURE_resetButton, onButtonClick_resetButton,
    ID_TIREPRESSURE_CnextButton, onButtonClick_CnextButton,
    ID_TIREPRESSURE_CprevButton, onButtonClick_CprevButton,
    ID_TIREPRESSURE_lvnextButton, onButtonClick_lvnextButton,
    ID_TIREPRESSURE_lvprevButton, onButtonClick_lvprevButton,
    ID_TIREPRESSURE_hvnextButton, onButtonClick_hvnextButton,
    ID_TIREPRESSURE_hvprevButton, onButtonClick_hvprevButton,
    ID_TIREPRESSURE_unitnextButton, onButtonClick_unitnextButton,
    ID_TIREPRESSURE_unitprevButton, onButtonClick_unitprevButton,
    ID_TIREPRESSURE_clearButton, onButtonClick_clearButton,
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
    ID_TIREPRESSURE_tireMatchListView, getListItemCount_tireMatchListView, obtainListItemData_tireMatchListView, onListItemClick_tireMatchListView,
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


tirePressureActivity::tirePressureActivity() {
	//todo add init code here
	mVideoLoopIndex = -1;
	mVideoLoopErrorCount = 0;
}

tirePressureActivity::~tirePressureActivity() {
  //todo add init file here
  // 退出应用时需要反注册
    EASYUICONTEXT->unregisterGlobalTouchListener(this);
    unregisterProtocolDataUpdateListener(onProtocolDataUpdate);
    onUI_quit();
    mActivityPtr = NULL;
    msys_backPtr = NULL;
    msaveOptionButtonPtr = NULL;
    msaveDataButtonPtr = NULL;
    mtireMatchListViewPtr = NULL;
    mrearButtonPtr = NULL;
    mfrontButtonPtr = NULL;
    mresetButtonPtr = NULL;
    mCnextButtonPtr = NULL;
    mCprevButtonPtr = NULL;
    mCTextViewPtr = NULL;
    mTextView7Ptr = NULL;
    mlvnextButtonPtr = NULL;
    mlvprevButtonPtr = NULL;
    mlvTextViewPtr = NULL;
    mTextView8Ptr = NULL;
    mhvnextButtonPtr = NULL;
    mhvprevButtonPtr = NULL;
    mhvTextViewPtr = NULL;
    mTextView6Ptr = NULL;
    munitnextButtonPtr = NULL;
    munitprevButtonPtr = NULL;
    munitTextViewPtr = NULL;
    mTextView5Ptr = NULL;
    mTextView4Ptr = NULL;
    mTextView3Ptr = NULL;
    mTextView2Ptr = NULL;
    mclearButtonPtr = NULL;
    mrearstateTextViewPtr = NULL;
    mfrontstateTextViewPtr = NULL;
    mrearCTextViewPtr = NULL;
    mfrontCViewPtr = NULL;
    mrearpsiTextViewPtr = NULL;
    mfrontpsiTextViewPtr = NULL;
    mrearTextViewPtr = NULL;
    mfrontTextViewPtr = NULL;
    mTextView1Ptr = NULL;
}

const char* tirePressureActivity::getAppName() const{
	return "tirePressure.ftu";
}

//TAG:onCreate
void tirePressureActivity::onCreate() {
	Activity::onCreate();
    msys_backPtr = (ZKButton*)findControlByID(ID_TIREPRESSURE_sys_back);
    msaveOptionButtonPtr = (ZKButton*)findControlByID(ID_TIREPRESSURE_saveOptionButton);
    msaveDataButtonPtr = (ZKButton*)findControlByID(ID_TIREPRESSURE_saveDataButton);
    mtireMatchListViewPtr = (ZKListView*)findControlByID(ID_TIREPRESSURE_tireMatchListView);if(mtireMatchListViewPtr!= NULL){mtireMatchListViewPtr->setListAdapter(this);mtireMatchListViewPtr->setItemClickListener(this);}
    mrearButtonPtr = (ZKButton*)findControlByID(ID_TIREPRESSURE_rearButton);
    mfrontButtonPtr = (ZKButton*)findControlByID(ID_TIREPRESSURE_frontButton);
    mresetButtonPtr = (ZKButton*)findControlByID(ID_TIREPRESSURE_resetButton);
    mCnextButtonPtr = (ZKButton*)findControlByID(ID_TIREPRESSURE_CnextButton);
    mCprevButtonPtr = (ZKButton*)findControlByID(ID_TIREPRESSURE_CprevButton);
    mCTextViewPtr = (ZKTextView*)findControlByID(ID_TIREPRESSURE_CTextView);
    mTextView7Ptr = (ZKTextView*)findControlByID(ID_TIREPRESSURE_TextView7);
    mlvnextButtonPtr = (ZKButton*)findControlByID(ID_TIREPRESSURE_lvnextButton);
    mlvprevButtonPtr = (ZKButton*)findControlByID(ID_TIREPRESSURE_lvprevButton);
    mlvTextViewPtr = (ZKTextView*)findControlByID(ID_TIREPRESSURE_lvTextView);
    mTextView8Ptr = (ZKTextView*)findControlByID(ID_TIREPRESSURE_TextView8);
    mhvnextButtonPtr = (ZKButton*)findControlByID(ID_TIREPRESSURE_hvnextButton);
    mhvprevButtonPtr = (ZKButton*)findControlByID(ID_TIREPRESSURE_hvprevButton);
    mhvTextViewPtr = (ZKTextView*)findControlByID(ID_TIREPRESSURE_hvTextView);
    mTextView6Ptr = (ZKTextView*)findControlByID(ID_TIREPRESSURE_TextView6);
    munitnextButtonPtr = (ZKButton*)findControlByID(ID_TIREPRESSURE_unitnextButton);
    munitprevButtonPtr = (ZKButton*)findControlByID(ID_TIREPRESSURE_unitprevButton);
    munitTextViewPtr = (ZKTextView*)findControlByID(ID_TIREPRESSURE_unitTextView);
    mTextView5Ptr = (ZKTextView*)findControlByID(ID_TIREPRESSURE_TextView5);
    mTextView4Ptr = (ZKTextView*)findControlByID(ID_TIREPRESSURE_TextView4);
    mTextView3Ptr = (ZKTextView*)findControlByID(ID_TIREPRESSURE_TextView3);
    mTextView2Ptr = (ZKTextView*)findControlByID(ID_TIREPRESSURE_TextView2);
    mclearButtonPtr = (ZKButton*)findControlByID(ID_TIREPRESSURE_clearButton);
    mrearstateTextViewPtr = (ZKTextView*)findControlByID(ID_TIREPRESSURE_rearstateTextView);
    mfrontstateTextViewPtr = (ZKTextView*)findControlByID(ID_TIREPRESSURE_frontstateTextView);
    mrearCTextViewPtr = (ZKTextView*)findControlByID(ID_TIREPRESSURE_rearCTextView);
    mfrontCViewPtr = (ZKTextView*)findControlByID(ID_TIREPRESSURE_frontCView);
    mrearpsiTextViewPtr = (ZKTextView*)findControlByID(ID_TIREPRESSURE_rearpsiTextView);
    mfrontpsiTextViewPtr = (ZKTextView*)findControlByID(ID_TIREPRESSURE_frontpsiTextView);
    mrearTextViewPtr = (ZKTextView*)findControlByID(ID_TIREPRESSURE_rearTextView);
    mfrontTextViewPtr = (ZKTextView*)findControlByID(ID_TIREPRESSURE_frontTextView);
    mTextView1Ptr = (ZKTextView*)findControlByID(ID_TIREPRESSURE_TextView1);
	mActivityPtr = this;
	onUI_init();
  registerProtocolDataUpdateListener(onProtocolDataUpdate);
  rigesterActivityTimer();
}

void tirePressureActivity::onClick(ZKBase *pBase) {
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

void tirePressureActivity::onResume() {
	Activity::onResume();
	EASYUICONTEXT->registerGlobalTouchListener(this);
	startVideoLoopPlayback();
	onUI_show();
}

void tirePressureActivity::onPause() {
	Activity::onPause();
	EASYUICONTEXT->unregisterGlobalTouchListener(this);
	stopVideoLoopPlayback();
	onUI_hide();
}

void tirePressureActivity::onIntent(const Intent *intentPtr) {
	Activity::onIntent(intentPtr);
	onUI_intent(intentPtr);
}

bool tirePressureActivity::onTimer(int id) {
	return onUI_Timer(id);
}

void tirePressureActivity::onProgressChanged(ZKSeekBar *pSeekBar, int progress){

    int seekBarTablen = sizeof(SZKSeekBarCallbackTab) / sizeof(S_ZKSeekBarCallback);
    for (int i = 0; i < seekBarTablen; ++i) {
        if (SZKSeekBarCallbackTab[i].id == pSeekBar->getID()) {
            SZKSeekBarCallbackTab[i].callback(pSeekBar, progress);
            break;
        }
    }
}

int tirePressureActivity::getListItemCount(const ZKListView *pListView) const{
    int tablen = sizeof(SListViewFunctionsCallbackTab) / sizeof(S_ListViewFunctionsCallback);
    for (int i = 0; i < tablen; ++i) {
        if (SListViewFunctionsCallbackTab[i].id == pListView->getID()) {
            return SListViewFunctionsCallbackTab[i].getListItemCountCallback(pListView);
            break;
        }
    }
    return 0;
}

void tirePressureActivity::obtainListItemData(ZKListView *pListView,ZKListView::ZKListItem *pListItem, int index){
    int tablen = sizeof(SListViewFunctionsCallbackTab) / sizeof(S_ListViewFunctionsCallback);
    for (int i = 0; i < tablen; ++i) {
        if (SListViewFunctionsCallbackTab[i].id == pListView->getID()) {
            SListViewFunctionsCallbackTab[i].obtainListItemDataCallback(pListView, pListItem, index);
            break;
        }
    }
}

void tirePressureActivity::onItemClick(ZKListView *pListView, int index, int id){
    int tablen = sizeof(SListViewFunctionsCallbackTab) / sizeof(S_ListViewFunctionsCallback);
    for (int i = 0; i < tablen; ++i) {
        if (SListViewFunctionsCallbackTab[i].id == pListView->getID()) {
            SListViewFunctionsCallbackTab[i].onItemClickCallback(pListView, index, id);
            break;
        }
    }
}

void tirePressureActivity::onSlideItemClick(ZKSlideWindow *pSlideWindow, int index) {
    int tablen = sizeof(SSlideWindowItemClickCallbackTab) / sizeof(S_SlideWindowItemClickCallback);
    for (int i = 0; i < tablen; ++i) {
        if (SSlideWindowItemClickCallbackTab[i].id == pSlideWindow->getID()) {
            SSlideWindowItemClickCallbackTab[i].onSlideItemClickCallback(pSlideWindow, index);
            break;
        }
    }
}

bool tirePressureActivity::onTouchEvent(const MotionEvent &ev) {
    return ontirePressureActivityTouchEvent(ev);
}

void tirePressureActivity::onTextChanged(ZKTextView *pTextView, const std::string &text) {
    int tablen = sizeof(SEditTextInputCallbackTab) / sizeof(S_EditTextInputCallback);
    for (int i = 0; i < tablen; ++i) {
        if (SEditTextInputCallbackTab[i].id == pTextView->getID()) {
            SEditTextInputCallbackTab[i].onEditTextChangedCallback(text);
            break;
        }
    }
}

void tirePressureActivity::rigesterActivityTimer() {
    int tablen = sizeof(REGISTER_ACTIVITY_TIMER_TAB) / sizeof(S_ACTIVITY_TIMEER);
    for (int i = 0; i < tablen; ++i) {
        S_ACTIVITY_TIMEER temp = REGISTER_ACTIVITY_TIMER_TAB[i];
        registerTimer(temp.id, temp.time);
    }
}


void tirePressureActivity::onVideoPlayerMessage(ZKVideoView *pVideoView, int msg) {
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

void tirePressureActivity::videoLoopPlayback(ZKVideoView *pVideoView, int msg, size_t callbackTabIndex) {

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

void tirePressureActivity::startVideoLoopPlayback() {
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

void tirePressureActivity::stopVideoLoopPlayback() {
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

bool tirePressureActivity::parseVideoFileList(const char *pFileListPath, std::vector<string>& mediaFileList) {
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

int tirePressureActivity::removeCharFromString(string& nString, char c) {
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

void tirePressureActivity::registerUserTimer(int id, int time) {
	registerTimer(id, time);
}

void tirePressureActivity::unregisterUserTimer(int id) {
	unregisterTimer(id);
}

void tirePressureActivity::resetUserTimer(int id, int time) {
	resetTimer(id, time);
}