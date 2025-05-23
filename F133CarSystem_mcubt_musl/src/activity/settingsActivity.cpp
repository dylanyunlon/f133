/***********************************************
/gen auto by zuitools
***********************************************/
#include "settingsActivity.h"

/*TAG:GlobalVariable全局变量*/
static ZKButton* msystemButtonPtr;
static ZKButton* mfactoryButtonPtr;
static ZKButton* mreverseButtonPtr;
static ZKButton* mvoiceButtonPtr;
static ZKButton* mlinkButtonPtr;
static ZKButton* mshowButtonPtr;
static ZKWindow* msetWndPtr;
static ZKTextView* mmcuTextViewPtr;
static ZKTextView* mappTextViewPtr;
static ZKTextView* mosTextViewPtr;
static ZKTextView* mTextView17Ptr;
static ZKTextView* mTextView16Ptr;
static ZKTextView* mTextView15Ptr;
static ZKTextView* mTextView14Ptr;
static ZKTextView* mTextView13Ptr;
static ZKTextView* mTextView10Ptr;
static ZKWindow* mversionWindowPtr;
static ZKButton* mhourButton24Ptr;
static ZKButton* mhourButton12Ptr;
static ZKTextView* mTextView9Ptr;
static ZKTextView* mTextView8Ptr;
static ZKTextView* mTextView7Ptr;
static ZKTextView* mTextView6Ptr;
static ZKTextView* mTextView5Ptr;
static ZKTextView* mTextView4Ptr;
static ZKListView* mdayListViewPtr;
static ZKListView* mmonthListViewPtr;
static ZKListView* myearListViewPtr;
static ZKListView* mminListViewPtr;
static ZKListView* mhourListViewPtr;
static ZKTextView* mTextView3Ptr;
static ZKWindow* msetWindowPtr;
static ZKWindow* mSetdataWndPtr;
static ZKListView* mLanguageListViewPtr;
static ZKWindow* mLanguageWndPtr;
static ZKTextView* mTextView40Ptr;
static ZKButton* mresetCancelButtonPtr;
static ZKButton* mresetConfirmButtonPtr;
static ZKWindow* mresetWindowPtr;
static ZKButton* mresetButtonPtr;
static ZKListView* msystemListViewPtr;
static ZKWindow* msystemWndPtr;
static settingsActivity* mActivityPtr;

/*register activity*/
REGISTER_ACTIVITY(settingsActivity);

typedef struct {
	int id; // 定时器ID ， 不能重复
	int time; // 定时器  时间间隔  单位 毫秒
}S_ACTIVITY_TIMEER;

#include "logic/settingsLogic.cc"

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
    ID_SETTINGS_systemButton, onButtonClick_systemButton,
    ID_SETTINGS_factoryButton, onButtonClick_factoryButton,
    ID_SETTINGS_reverseButton, onButtonClick_reverseButton,
    ID_SETTINGS_voiceButton, onButtonClick_voiceButton,
    ID_SETTINGS_linkButton, onButtonClick_linkButton,
    ID_SETTINGS_showButton, onButtonClick_showButton,
    ID_SETTINGS_hourButton24, onButtonClick_hourButton24,
    ID_SETTINGS_hourButton12, onButtonClick_hourButton12,
    ID_SETTINGS_resetCancelButton, onButtonClick_resetCancelButton,
    ID_SETTINGS_resetConfirmButton, onButtonClick_resetConfirmButton,
    ID_SETTINGS_resetButton, onButtonClick_resetButton,
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
    ID_SETTINGS_dayListView, getListItemCount_dayListView, obtainListItemData_dayListView, onListItemClick_dayListView,
    ID_SETTINGS_monthListView, getListItemCount_monthListView, obtainListItemData_monthListView, onListItemClick_monthListView,
    ID_SETTINGS_yearListView, getListItemCount_yearListView, obtainListItemData_yearListView, onListItemClick_yearListView,
    ID_SETTINGS_minListView, getListItemCount_minListView, obtainListItemData_minListView, onListItemClick_minListView,
    ID_SETTINGS_hourListView, getListItemCount_hourListView, obtainListItemData_hourListView, onListItemClick_hourListView,
    ID_SETTINGS_LanguageListView, getListItemCount_LanguageListView, obtainListItemData_LanguageListView, onListItemClick_LanguageListView,
    ID_SETTINGS_systemListView, getListItemCount_systemListView, obtainListItemData_systemListView, onListItemClick_systemListView,
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


settingsActivity::settingsActivity() {
	//todo add init code here
	mVideoLoopIndex = -1;
	mVideoLoopErrorCount = 0;
}

settingsActivity::~settingsActivity() {
  //todo add init file here
  // 退出应用时需要反注册
    EASYUICONTEXT->unregisterGlobalTouchListener(this);
    unregisterProtocolDataUpdateListener(onProtocolDataUpdate);
    onUI_quit();
    mActivityPtr = NULL;
    msystemButtonPtr = NULL;
    mfactoryButtonPtr = NULL;
    mreverseButtonPtr = NULL;
    mvoiceButtonPtr = NULL;
    mlinkButtonPtr = NULL;
    mshowButtonPtr = NULL;
    msetWndPtr = NULL;
    mmcuTextViewPtr = NULL;
    mappTextViewPtr = NULL;
    mosTextViewPtr = NULL;
    mTextView17Ptr = NULL;
    mTextView16Ptr = NULL;
    mTextView15Ptr = NULL;
    mTextView14Ptr = NULL;
    mTextView13Ptr = NULL;
    mTextView10Ptr = NULL;
    mversionWindowPtr = NULL;
    mhourButton24Ptr = NULL;
    mhourButton12Ptr = NULL;
    mTextView9Ptr = NULL;
    mTextView8Ptr = NULL;
    mTextView7Ptr = NULL;
    mTextView6Ptr = NULL;
    mTextView5Ptr = NULL;
    mTextView4Ptr = NULL;
    mdayListViewPtr = NULL;
    mmonthListViewPtr = NULL;
    myearListViewPtr = NULL;
    mminListViewPtr = NULL;
    mhourListViewPtr = NULL;
    mTextView3Ptr = NULL;
    msetWindowPtr = NULL;
    mSetdataWndPtr = NULL;
    mLanguageListViewPtr = NULL;
    mLanguageWndPtr = NULL;
    mTextView40Ptr = NULL;
    mresetCancelButtonPtr = NULL;
    mresetConfirmButtonPtr = NULL;
    mresetWindowPtr = NULL;
    mresetButtonPtr = NULL;
    msystemListViewPtr = NULL;
    msystemWndPtr = NULL;
}

const char* settingsActivity::getAppName() const{
	return "settings.ftu";
}

//TAG:onCreate
void settingsActivity::onCreate() {
	Activity::onCreate();
    msystemButtonPtr = (ZKButton*)findControlByID(ID_SETTINGS_systemButton);
    mfactoryButtonPtr = (ZKButton*)findControlByID(ID_SETTINGS_factoryButton);
    mreverseButtonPtr = (ZKButton*)findControlByID(ID_SETTINGS_reverseButton);
    mvoiceButtonPtr = (ZKButton*)findControlByID(ID_SETTINGS_voiceButton);
    mlinkButtonPtr = (ZKButton*)findControlByID(ID_SETTINGS_linkButton);
    mshowButtonPtr = (ZKButton*)findControlByID(ID_SETTINGS_showButton);
    msetWndPtr = (ZKWindow*)findControlByID(ID_SETTINGS_setWnd);
    mmcuTextViewPtr = (ZKTextView*)findControlByID(ID_SETTINGS_mcuTextView);
    mappTextViewPtr = (ZKTextView*)findControlByID(ID_SETTINGS_appTextView);
    mosTextViewPtr = (ZKTextView*)findControlByID(ID_SETTINGS_osTextView);
    mTextView17Ptr = (ZKTextView*)findControlByID(ID_SETTINGS_TextView17);
    mTextView16Ptr = (ZKTextView*)findControlByID(ID_SETTINGS_TextView16);
    mTextView15Ptr = (ZKTextView*)findControlByID(ID_SETTINGS_TextView15);
    mTextView14Ptr = (ZKTextView*)findControlByID(ID_SETTINGS_TextView14);
    mTextView13Ptr = (ZKTextView*)findControlByID(ID_SETTINGS_TextView13);
    mTextView10Ptr = (ZKTextView*)findControlByID(ID_SETTINGS_TextView10);
    mversionWindowPtr = (ZKWindow*)findControlByID(ID_SETTINGS_versionWindow);
    mhourButton24Ptr = (ZKButton*)findControlByID(ID_SETTINGS_hourButton24);
    mhourButton12Ptr = (ZKButton*)findControlByID(ID_SETTINGS_hourButton12);
    mTextView9Ptr = (ZKTextView*)findControlByID(ID_SETTINGS_TextView9);
    mTextView8Ptr = (ZKTextView*)findControlByID(ID_SETTINGS_TextView8);
    mTextView7Ptr = (ZKTextView*)findControlByID(ID_SETTINGS_TextView7);
    mTextView6Ptr = (ZKTextView*)findControlByID(ID_SETTINGS_TextView6);
    mTextView5Ptr = (ZKTextView*)findControlByID(ID_SETTINGS_TextView5);
    mTextView4Ptr = (ZKTextView*)findControlByID(ID_SETTINGS_TextView4);
    mdayListViewPtr = (ZKListView*)findControlByID(ID_SETTINGS_dayListView);if(mdayListViewPtr!= NULL){mdayListViewPtr->setListAdapter(this);mdayListViewPtr->setItemClickListener(this);}
    mmonthListViewPtr = (ZKListView*)findControlByID(ID_SETTINGS_monthListView);if(mmonthListViewPtr!= NULL){mmonthListViewPtr->setListAdapter(this);mmonthListViewPtr->setItemClickListener(this);}
    myearListViewPtr = (ZKListView*)findControlByID(ID_SETTINGS_yearListView);if(myearListViewPtr!= NULL){myearListViewPtr->setListAdapter(this);myearListViewPtr->setItemClickListener(this);}
    mminListViewPtr = (ZKListView*)findControlByID(ID_SETTINGS_minListView);if(mminListViewPtr!= NULL){mminListViewPtr->setListAdapter(this);mminListViewPtr->setItemClickListener(this);}
    mhourListViewPtr = (ZKListView*)findControlByID(ID_SETTINGS_hourListView);if(mhourListViewPtr!= NULL){mhourListViewPtr->setListAdapter(this);mhourListViewPtr->setItemClickListener(this);}
    mTextView3Ptr = (ZKTextView*)findControlByID(ID_SETTINGS_TextView3);
    msetWindowPtr = (ZKWindow*)findControlByID(ID_SETTINGS_setWindow);
    mSetdataWndPtr = (ZKWindow*)findControlByID(ID_SETTINGS_SetdataWnd);
    mLanguageListViewPtr = (ZKListView*)findControlByID(ID_SETTINGS_LanguageListView);if(mLanguageListViewPtr!= NULL){mLanguageListViewPtr->setListAdapter(this);mLanguageListViewPtr->setItemClickListener(this);}
    mLanguageWndPtr = (ZKWindow*)findControlByID(ID_SETTINGS_LanguageWnd);
    mTextView40Ptr = (ZKTextView*)findControlByID(ID_SETTINGS_TextView40);
    mresetCancelButtonPtr = (ZKButton*)findControlByID(ID_SETTINGS_resetCancelButton);
    mresetConfirmButtonPtr = (ZKButton*)findControlByID(ID_SETTINGS_resetConfirmButton);
    mresetWindowPtr = (ZKWindow*)findControlByID(ID_SETTINGS_resetWindow);
    mresetButtonPtr = (ZKButton*)findControlByID(ID_SETTINGS_resetButton);
    msystemListViewPtr = (ZKListView*)findControlByID(ID_SETTINGS_systemListView);if(msystemListViewPtr!= NULL){msystemListViewPtr->setListAdapter(this);msystemListViewPtr->setItemClickListener(this);}
    msystemWndPtr = (ZKWindow*)findControlByID(ID_SETTINGS_systemWnd);
	mActivityPtr = this;
	onUI_init();
  registerProtocolDataUpdateListener(onProtocolDataUpdate);
  rigesterActivityTimer();
}

void settingsActivity::onClick(ZKBase *pBase) {
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

void settingsActivity::onResume() {
	Activity::onResume();
	EASYUICONTEXT->registerGlobalTouchListener(this);
	startVideoLoopPlayback();
	onUI_show();
}

void settingsActivity::onPause() {
	Activity::onPause();
	EASYUICONTEXT->unregisterGlobalTouchListener(this);
	stopVideoLoopPlayback();
	onUI_hide();
}

void settingsActivity::onIntent(const Intent *intentPtr) {
	Activity::onIntent(intentPtr);
	onUI_intent(intentPtr);
}

bool settingsActivity::onTimer(int id) {
	return onUI_Timer(id);
}

void settingsActivity::onProgressChanged(ZKSeekBar *pSeekBar, int progress){

    int seekBarTablen = sizeof(SZKSeekBarCallbackTab) / sizeof(S_ZKSeekBarCallback);
    for (int i = 0; i < seekBarTablen; ++i) {
        if (SZKSeekBarCallbackTab[i].id == pSeekBar->getID()) {
            SZKSeekBarCallbackTab[i].callback(pSeekBar, progress);
            break;
        }
    }
}

int settingsActivity::getListItemCount(const ZKListView *pListView) const{
    int tablen = sizeof(SListViewFunctionsCallbackTab) / sizeof(S_ListViewFunctionsCallback);
    for (int i = 0; i < tablen; ++i) {
        if (SListViewFunctionsCallbackTab[i].id == pListView->getID()) {
            return SListViewFunctionsCallbackTab[i].getListItemCountCallback(pListView);
            break;
        }
    }
    return 0;
}

void settingsActivity::obtainListItemData(ZKListView *pListView,ZKListView::ZKListItem *pListItem, int index){
    int tablen = sizeof(SListViewFunctionsCallbackTab) / sizeof(S_ListViewFunctionsCallback);
    for (int i = 0; i < tablen; ++i) {
        if (SListViewFunctionsCallbackTab[i].id == pListView->getID()) {
            SListViewFunctionsCallbackTab[i].obtainListItemDataCallback(pListView, pListItem, index);
            break;
        }
    }
}

void settingsActivity::onItemClick(ZKListView *pListView, int index, int id){
    int tablen = sizeof(SListViewFunctionsCallbackTab) / sizeof(S_ListViewFunctionsCallback);
    for (int i = 0; i < tablen; ++i) {
        if (SListViewFunctionsCallbackTab[i].id == pListView->getID()) {
            SListViewFunctionsCallbackTab[i].onItemClickCallback(pListView, index, id);
            break;
        }
    }
}

void settingsActivity::onSlideItemClick(ZKSlideWindow *pSlideWindow, int index) {
    int tablen = sizeof(SSlideWindowItemClickCallbackTab) / sizeof(S_SlideWindowItemClickCallback);
    for (int i = 0; i < tablen; ++i) {
        if (SSlideWindowItemClickCallbackTab[i].id == pSlideWindow->getID()) {
            SSlideWindowItemClickCallbackTab[i].onSlideItemClickCallback(pSlideWindow, index);
            break;
        }
    }
}

bool settingsActivity::onTouchEvent(const MotionEvent &ev) {
    return onsettingsActivityTouchEvent(ev);
}

void settingsActivity::onTextChanged(ZKTextView *pTextView, const std::string &text) {
    int tablen = sizeof(SEditTextInputCallbackTab) / sizeof(S_EditTextInputCallback);
    for (int i = 0; i < tablen; ++i) {
        if (SEditTextInputCallbackTab[i].id == pTextView->getID()) {
            SEditTextInputCallbackTab[i].onEditTextChangedCallback(text);
            break;
        }
    }
}

void settingsActivity::rigesterActivityTimer() {
    int tablen = sizeof(REGISTER_ACTIVITY_TIMER_TAB) / sizeof(S_ACTIVITY_TIMEER);
    for (int i = 0; i < tablen; ++i) {
        S_ACTIVITY_TIMEER temp = REGISTER_ACTIVITY_TIMER_TAB[i];
        registerTimer(temp.id, temp.time);
    }
}


void settingsActivity::onVideoPlayerMessage(ZKVideoView *pVideoView, int msg) {
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

void settingsActivity::videoLoopPlayback(ZKVideoView *pVideoView, int msg, size_t callbackTabIndex) {

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

void settingsActivity::startVideoLoopPlayback() {
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

void settingsActivity::stopVideoLoopPlayback() {
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

bool settingsActivity::parseVideoFileList(const char *pFileListPath, std::vector<string>& mediaFileList) {
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

int settingsActivity::removeCharFromString(string& nString, char c) {
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

void settingsActivity::registerUserTimer(int id, int time) {
	registerTimer(id, time);
}

void settingsActivity::unregisterUserTimer(int id) {
	unregisterTimer(id);
}

void settingsActivity::resetUserTimer(int id, int time) {
	resetTimer(id, time);
}