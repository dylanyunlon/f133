/***********************************************
/gen auto by zuitools
***********************************************/
#include "btRecordsActivity.h"

/*TAG:GlobalVariable全局变量*/
static ZKTextView* mTextViewBgPtr;
static ZKButton* msureButtonPtr;
static ZKButton* mcancelButtonPtr;
static ZKTextView* mTipsTextViewPtr;
static ZKWindow* mDeleteTipsWindowPtr;
static ZKTextView* mTextView15Ptr;
static ZKButton* mbtsettingButtonPtr;
static ZKButton* mbtcontactsButtonPtr;
static ZKButton* mbtrecordButtonPtr;
static ZKButton* mphoneButtonPtr;
static ZKButton* mqueryMusicButtonPtr;
static ZKWindow* mbtAppWindowPtr;
static ZKButton* moutButtonPtr;
static ZKButton* minButtonPtr;
static ZKButton* mmissButtonPtr;
static ZKButton* mDeleteButtonPtr;
static ZKPointer* msyncPointerPtr;
static ZKTextView* mscaningTextViewPtr;
static ZKWindow* mscaningWindowPtr;
static ZKButton* mdownloadButtonPtr;
static ZKListView* mrecordsListViewPtr;
static btRecordsActivity* mActivityPtr;

/*register activity*/
REGISTER_ACTIVITY(btRecordsActivity);

typedef struct {
	int id; // 定时器ID ， 不能重复
	int time; // 定时器  时间间隔  单位 毫秒
}S_ACTIVITY_TIMEER;

#include "logic/btRecordsLogic.cc"

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
    ID_BTRECORDS_sureButton, onButtonClick_sureButton,
    ID_BTRECORDS_cancelButton, onButtonClick_cancelButton,
    ID_BTRECORDS_btsettingButton, onButtonClick_btsettingButton,
    ID_BTRECORDS_btcontactsButton, onButtonClick_btcontactsButton,
    ID_BTRECORDS_btrecordButton, onButtonClick_btrecordButton,
    ID_BTRECORDS_phoneButton, onButtonClick_phoneButton,
    ID_BTRECORDS_queryMusicButton, onButtonClick_queryMusicButton,
    ID_BTRECORDS_outButton, onButtonClick_outButton,
    ID_BTRECORDS_inButton, onButtonClick_inButton,
    ID_BTRECORDS_missButton, onButtonClick_missButton,
    ID_BTRECORDS_DeleteButton, onButtonClick_DeleteButton,
    ID_BTRECORDS_downloadButton, onButtonClick_downloadButton,
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
    ID_BTRECORDS_recordsListView, getListItemCount_recordsListView, obtainListItemData_recordsListView, onListItemClick_recordsListView,
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


btRecordsActivity::btRecordsActivity() {
	//todo add init code here
	mVideoLoopIndex = -1;
	mVideoLoopErrorCount = 0;
}

btRecordsActivity::~btRecordsActivity() {
  //todo add init file here
  // 退出应用时需要反注册
    EASYUICONTEXT->unregisterGlobalTouchListener(this);
    unregisterProtocolDataUpdateListener(onProtocolDataUpdate);
    onUI_quit();
    mActivityPtr = NULL;
    mTextViewBgPtr = NULL;
    msureButtonPtr = NULL;
    mcancelButtonPtr = NULL;
    mTipsTextViewPtr = NULL;
    mDeleteTipsWindowPtr = NULL;
    mTextView15Ptr = NULL;
    mbtsettingButtonPtr = NULL;
    mbtcontactsButtonPtr = NULL;
    mbtrecordButtonPtr = NULL;
    mphoneButtonPtr = NULL;
    mqueryMusicButtonPtr = NULL;
    mbtAppWindowPtr = NULL;
    moutButtonPtr = NULL;
    minButtonPtr = NULL;
    mmissButtonPtr = NULL;
    mDeleteButtonPtr = NULL;
    msyncPointerPtr = NULL;
    mscaningTextViewPtr = NULL;
    mscaningWindowPtr = NULL;
    mdownloadButtonPtr = NULL;
    mrecordsListViewPtr = NULL;
}

const char* btRecordsActivity::getAppName() const{
	return "btRecords.ftu";
}

//TAG:onCreate
void btRecordsActivity::onCreate() {
	Activity::onCreate();
    mTextViewBgPtr = (ZKTextView*)findControlByID(ID_BTRECORDS_TextViewBg);
    msureButtonPtr = (ZKButton*)findControlByID(ID_BTRECORDS_sureButton);
    mcancelButtonPtr = (ZKButton*)findControlByID(ID_BTRECORDS_cancelButton);
    mTipsTextViewPtr = (ZKTextView*)findControlByID(ID_BTRECORDS_TipsTextView);
    mDeleteTipsWindowPtr = (ZKWindow*)findControlByID(ID_BTRECORDS_DeleteTipsWindow);
    mTextView15Ptr = (ZKTextView*)findControlByID(ID_BTRECORDS_TextView15);
    mbtsettingButtonPtr = (ZKButton*)findControlByID(ID_BTRECORDS_btsettingButton);
    mbtcontactsButtonPtr = (ZKButton*)findControlByID(ID_BTRECORDS_btcontactsButton);
    mbtrecordButtonPtr = (ZKButton*)findControlByID(ID_BTRECORDS_btrecordButton);
    mphoneButtonPtr = (ZKButton*)findControlByID(ID_BTRECORDS_phoneButton);
    mqueryMusicButtonPtr = (ZKButton*)findControlByID(ID_BTRECORDS_queryMusicButton);
    mbtAppWindowPtr = (ZKWindow*)findControlByID(ID_BTRECORDS_btAppWindow);
    moutButtonPtr = (ZKButton*)findControlByID(ID_BTRECORDS_outButton);
    minButtonPtr = (ZKButton*)findControlByID(ID_BTRECORDS_inButton);
    mmissButtonPtr = (ZKButton*)findControlByID(ID_BTRECORDS_missButton);
    mDeleteButtonPtr = (ZKButton*)findControlByID(ID_BTRECORDS_DeleteButton);
    msyncPointerPtr = (ZKPointer*)findControlByID(ID_BTRECORDS_syncPointer);
    mscaningTextViewPtr = (ZKTextView*)findControlByID(ID_BTRECORDS_scaningTextView);
    mscaningWindowPtr = (ZKWindow*)findControlByID(ID_BTRECORDS_scaningWindow);
    mdownloadButtonPtr = (ZKButton*)findControlByID(ID_BTRECORDS_downloadButton);
    mrecordsListViewPtr = (ZKListView*)findControlByID(ID_BTRECORDS_recordsListView);if(mrecordsListViewPtr!= NULL){mrecordsListViewPtr->setListAdapter(this);mrecordsListViewPtr->setItemClickListener(this);}
	mActivityPtr = this;
	onUI_init();
  registerProtocolDataUpdateListener(onProtocolDataUpdate);
  rigesterActivityTimer();
}

void btRecordsActivity::onClick(ZKBase *pBase) {
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

void btRecordsActivity::onResume() {
	Activity::onResume();
	EASYUICONTEXT->registerGlobalTouchListener(this);
	startVideoLoopPlayback();
	onUI_show();
}

void btRecordsActivity::onPause() {
	Activity::onPause();
	EASYUICONTEXT->unregisterGlobalTouchListener(this);
	stopVideoLoopPlayback();
	onUI_hide();
}

void btRecordsActivity::onIntent(const Intent *intentPtr) {
	Activity::onIntent(intentPtr);
	onUI_intent(intentPtr);
}

bool btRecordsActivity::onTimer(int id) {
	return onUI_Timer(id);
}

void btRecordsActivity::onProgressChanged(ZKSeekBar *pSeekBar, int progress){

    int seekBarTablen = sizeof(SZKSeekBarCallbackTab) / sizeof(S_ZKSeekBarCallback);
    for (int i = 0; i < seekBarTablen; ++i) {
        if (SZKSeekBarCallbackTab[i].id == pSeekBar->getID()) {
            SZKSeekBarCallbackTab[i].callback(pSeekBar, progress);
            break;
        }
    }
}

int btRecordsActivity::getListItemCount(const ZKListView *pListView) const{
    int tablen = sizeof(SListViewFunctionsCallbackTab) / sizeof(S_ListViewFunctionsCallback);
    for (int i = 0; i < tablen; ++i) {
        if (SListViewFunctionsCallbackTab[i].id == pListView->getID()) {
            return SListViewFunctionsCallbackTab[i].getListItemCountCallback(pListView);
            break;
        }
    }
    return 0;
}

void btRecordsActivity::obtainListItemData(ZKListView *pListView,ZKListView::ZKListItem *pListItem, int index){
    int tablen = sizeof(SListViewFunctionsCallbackTab) / sizeof(S_ListViewFunctionsCallback);
    for (int i = 0; i < tablen; ++i) {
        if (SListViewFunctionsCallbackTab[i].id == pListView->getID()) {
            SListViewFunctionsCallbackTab[i].obtainListItemDataCallback(pListView, pListItem, index);
            break;
        }
    }
}

void btRecordsActivity::onItemClick(ZKListView *pListView, int index, int id){
    int tablen = sizeof(SListViewFunctionsCallbackTab) / sizeof(S_ListViewFunctionsCallback);
    for (int i = 0; i < tablen; ++i) {
        if (SListViewFunctionsCallbackTab[i].id == pListView->getID()) {
            SListViewFunctionsCallbackTab[i].onItemClickCallback(pListView, index, id);
            break;
        }
    }
}

void btRecordsActivity::onSlideItemClick(ZKSlideWindow *pSlideWindow, int index) {
    int tablen = sizeof(SSlideWindowItemClickCallbackTab) / sizeof(S_SlideWindowItemClickCallback);
    for (int i = 0; i < tablen; ++i) {
        if (SSlideWindowItemClickCallbackTab[i].id == pSlideWindow->getID()) {
            SSlideWindowItemClickCallbackTab[i].onSlideItemClickCallback(pSlideWindow, index);
            break;
        }
    }
}

bool btRecordsActivity::onTouchEvent(const MotionEvent &ev) {
    return onbtRecordsActivityTouchEvent(ev);
}

void btRecordsActivity::onTextChanged(ZKTextView *pTextView, const std::string &text) {
    int tablen = sizeof(SEditTextInputCallbackTab) / sizeof(S_EditTextInputCallback);
    for (int i = 0; i < tablen; ++i) {
        if (SEditTextInputCallbackTab[i].id == pTextView->getID()) {
            SEditTextInputCallbackTab[i].onEditTextChangedCallback(text);
            break;
        }
    }
}

void btRecordsActivity::rigesterActivityTimer() {
    int tablen = sizeof(REGISTER_ACTIVITY_TIMER_TAB) / sizeof(S_ACTIVITY_TIMEER);
    for (int i = 0; i < tablen; ++i) {
        S_ACTIVITY_TIMEER temp = REGISTER_ACTIVITY_TIMER_TAB[i];
        registerTimer(temp.id, temp.time);
    }
}


void btRecordsActivity::onVideoPlayerMessage(ZKVideoView *pVideoView, int msg) {
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

void btRecordsActivity::videoLoopPlayback(ZKVideoView *pVideoView, int msg, size_t callbackTabIndex) {

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

void btRecordsActivity::startVideoLoopPlayback() {
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

void btRecordsActivity::stopVideoLoopPlayback() {
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

bool btRecordsActivity::parseVideoFileList(const char *pFileListPath, std::vector<string>& mediaFileList) {
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

int btRecordsActivity::removeCharFromString(string& nString, char c) {
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

void btRecordsActivity::registerUserTimer(int id, int time) {
	registerTimer(id, time);
}

void btRecordsActivity::unregisterUserTimer(int id) {
	unregisterTimer(id);
}

void btRecordsActivity::resetUserTimer(int id, int time) {
	resetTimer(id, time);
}