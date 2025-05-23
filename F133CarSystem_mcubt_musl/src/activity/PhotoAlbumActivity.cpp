/***********************************************
/gen auto by zuitools
***********************************************/
#include "PhotoAlbumActivity.h"

/*TAG:GlobalVariable全局变量*/
static ZKTextView* mTextViewBgPtr;
static ZKTextView* mTextView6Ptr;
static ZKTextView* mTextView4Ptr;
static ZKTextView* mTextView1Ptr;
static ZKTextView* mTextView3Ptr;
static ZKTextView* mTextView2Ptr;
static ZKTextView* mTextView5Ptr;
static ZKButton* mbackPtr;
static ZKPointer* msyncPointerPtr;
static ZKTextView* mscaningTextViewPtr;
static ZKWindow* mNoticePtr;
static ZKListView* mImagesListViewPtr;
static ZKButton* mUSB2Ptr;
static ZKButton* mUSB1Ptr;
static ZKButton* mSDPtr;
static ZKWindow* mAlbumClassificationPtr;
static ZKTextView* mIndexPtr;
static ZKButton* mrotateBtnPtr;
static ZKButton* mreduceBtnPtr;
static ZKButton* mamplifyBtnPtr;
static ZKButton* mCarouselPicPtr;
static ZKButton* mPrevPtr;
static ZKButton* mNextPtr;
static ZKButton* mIndexBt3Ptr;
static ZKTextView* mIndexPic3Ptr;
static ZKWindow* mIndexWnd3Ptr;
static ZKButton* mIndexBt2Ptr;
static ZKTextView* mIndexPic2Ptr;
static ZKWindow* mIndexWnd2Ptr;
static ZKButton* mIndexBt1Ptr;
static ZKTextView* mIndexPic1Ptr;
static ZKWindow* mIndexWnd1Ptr;
static ZKWindow* mFramePtr;
static PhotoAlbumActivity* mActivityPtr;

/*register activity*/
REGISTER_ACTIVITY(PhotoAlbumActivity);

typedef struct {
	int id; // 定时器ID ， 不能重复
	int time; // 定时器  时间间隔  单位 毫秒
}S_ACTIVITY_TIMEER;

#include "logic/PhotoAlbumLogic.cc"

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
    ID_PHOTOALBUM_back, onButtonClick_back,
    ID_PHOTOALBUM_USB2, onButtonClick_USB2,
    ID_PHOTOALBUM_USB1, onButtonClick_USB1,
    ID_PHOTOALBUM_SD, onButtonClick_SD,
    ID_PHOTOALBUM_rotateBtn, onButtonClick_rotateBtn,
    ID_PHOTOALBUM_reduceBtn, onButtonClick_reduceBtn,
    ID_PHOTOALBUM_amplifyBtn, onButtonClick_amplifyBtn,
    ID_PHOTOALBUM_CarouselPic, onButtonClick_CarouselPic,
    ID_PHOTOALBUM_Prev, onButtonClick_Prev,
    ID_PHOTOALBUM_Next, onButtonClick_Next,
    ID_PHOTOALBUM_IndexBt3, onButtonClick_IndexBt3,
    ID_PHOTOALBUM_IndexBt2, onButtonClick_IndexBt2,
    ID_PHOTOALBUM_IndexBt1, onButtonClick_IndexBt1,
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
    ID_PHOTOALBUM_ImagesListView, getListItemCount_ImagesListView, obtainListItemData_ImagesListView, onListItemClick_ImagesListView,
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


PhotoAlbumActivity::PhotoAlbumActivity() {
	//todo add init code here
	mVideoLoopIndex = -1;
	mVideoLoopErrorCount = 0;
}

PhotoAlbumActivity::~PhotoAlbumActivity() {
  //todo add init file here
  // 退出应用时需要反注册
    EASYUICONTEXT->unregisterGlobalTouchListener(this);
    unregisterProtocolDataUpdateListener(onProtocolDataUpdate);
    onUI_quit();
    mActivityPtr = NULL;
    mTextViewBgPtr = NULL;
    mTextView6Ptr = NULL;
    mTextView4Ptr = NULL;
    mTextView1Ptr = NULL;
    mTextView3Ptr = NULL;
    mTextView2Ptr = NULL;
    mTextView5Ptr = NULL;
    mbackPtr = NULL;
    msyncPointerPtr = NULL;
    mscaningTextViewPtr = NULL;
    mNoticePtr = NULL;
    mImagesListViewPtr = NULL;
    mUSB2Ptr = NULL;
    mUSB1Ptr = NULL;
    mSDPtr = NULL;
    mAlbumClassificationPtr = NULL;
    mIndexPtr = NULL;
    mrotateBtnPtr = NULL;
    mreduceBtnPtr = NULL;
    mamplifyBtnPtr = NULL;
    mCarouselPicPtr = NULL;
    mPrevPtr = NULL;
    mNextPtr = NULL;
    mIndexBt3Ptr = NULL;
    mIndexPic3Ptr = NULL;
    mIndexWnd3Ptr = NULL;
    mIndexBt2Ptr = NULL;
    mIndexPic2Ptr = NULL;
    mIndexWnd2Ptr = NULL;
    mIndexBt1Ptr = NULL;
    mIndexPic1Ptr = NULL;
    mIndexWnd1Ptr = NULL;
    mFramePtr = NULL;
}

const char* PhotoAlbumActivity::getAppName() const{
	return "PhotoAlbum.ftu";
}

//TAG:onCreate
void PhotoAlbumActivity::onCreate() {
	Activity::onCreate();
    mTextViewBgPtr = (ZKTextView*)findControlByID(ID_PHOTOALBUM_TextViewBg);
    mTextView6Ptr = (ZKTextView*)findControlByID(ID_PHOTOALBUM_TextView6);
    mTextView4Ptr = (ZKTextView*)findControlByID(ID_PHOTOALBUM_TextView4);
    mTextView1Ptr = (ZKTextView*)findControlByID(ID_PHOTOALBUM_TextView1);
    mTextView3Ptr = (ZKTextView*)findControlByID(ID_PHOTOALBUM_TextView3);
    mTextView2Ptr = (ZKTextView*)findControlByID(ID_PHOTOALBUM_TextView2);
    mTextView5Ptr = (ZKTextView*)findControlByID(ID_PHOTOALBUM_TextView5);
    mbackPtr = (ZKButton*)findControlByID(ID_PHOTOALBUM_back);
    msyncPointerPtr = (ZKPointer*)findControlByID(ID_PHOTOALBUM_syncPointer);
    mscaningTextViewPtr = (ZKTextView*)findControlByID(ID_PHOTOALBUM_scaningTextView);
    mNoticePtr = (ZKWindow*)findControlByID(ID_PHOTOALBUM_Notice);
    mImagesListViewPtr = (ZKListView*)findControlByID(ID_PHOTOALBUM_ImagesListView);if(mImagesListViewPtr!= NULL){mImagesListViewPtr->setListAdapter(this);mImagesListViewPtr->setItemClickListener(this);}
    mUSB2Ptr = (ZKButton*)findControlByID(ID_PHOTOALBUM_USB2);
    mUSB1Ptr = (ZKButton*)findControlByID(ID_PHOTOALBUM_USB1);
    mSDPtr = (ZKButton*)findControlByID(ID_PHOTOALBUM_SD);
    mAlbumClassificationPtr = (ZKWindow*)findControlByID(ID_PHOTOALBUM_AlbumClassification);
    mIndexPtr = (ZKTextView*)findControlByID(ID_PHOTOALBUM_Index);
    mrotateBtnPtr = (ZKButton*)findControlByID(ID_PHOTOALBUM_rotateBtn);
    mreduceBtnPtr = (ZKButton*)findControlByID(ID_PHOTOALBUM_reduceBtn);
    mamplifyBtnPtr = (ZKButton*)findControlByID(ID_PHOTOALBUM_amplifyBtn);
    mCarouselPicPtr = (ZKButton*)findControlByID(ID_PHOTOALBUM_CarouselPic);
    mPrevPtr = (ZKButton*)findControlByID(ID_PHOTOALBUM_Prev);
    mNextPtr = (ZKButton*)findControlByID(ID_PHOTOALBUM_Next);
    mIndexBt3Ptr = (ZKButton*)findControlByID(ID_PHOTOALBUM_IndexBt3);
    mIndexPic3Ptr = (ZKTextView*)findControlByID(ID_PHOTOALBUM_IndexPic3);
    mIndexWnd3Ptr = (ZKWindow*)findControlByID(ID_PHOTOALBUM_IndexWnd3);
    mIndexBt2Ptr = (ZKButton*)findControlByID(ID_PHOTOALBUM_IndexBt2);
    mIndexPic2Ptr = (ZKTextView*)findControlByID(ID_PHOTOALBUM_IndexPic2);
    mIndexWnd2Ptr = (ZKWindow*)findControlByID(ID_PHOTOALBUM_IndexWnd2);
    mIndexBt1Ptr = (ZKButton*)findControlByID(ID_PHOTOALBUM_IndexBt1);
    mIndexPic1Ptr = (ZKTextView*)findControlByID(ID_PHOTOALBUM_IndexPic1);
    mIndexWnd1Ptr = (ZKWindow*)findControlByID(ID_PHOTOALBUM_IndexWnd1);
    mFramePtr = (ZKWindow*)findControlByID(ID_PHOTOALBUM_Frame);
	mActivityPtr = this;
	onUI_init();
  registerProtocolDataUpdateListener(onProtocolDataUpdate);
  rigesterActivityTimer();
}

void PhotoAlbumActivity::onClick(ZKBase *pBase) {
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

void PhotoAlbumActivity::onResume() {
	Activity::onResume();
	EASYUICONTEXT->registerGlobalTouchListener(this);
	startVideoLoopPlayback();
	onUI_show();
}

void PhotoAlbumActivity::onPause() {
	Activity::onPause();
	EASYUICONTEXT->unregisterGlobalTouchListener(this);
	stopVideoLoopPlayback();
	onUI_hide();
}

void PhotoAlbumActivity::onIntent(const Intent *intentPtr) {
	Activity::onIntent(intentPtr);
	onUI_intent(intentPtr);
}

bool PhotoAlbumActivity::onTimer(int id) {
	return onUI_Timer(id);
}

void PhotoAlbumActivity::onProgressChanged(ZKSeekBar *pSeekBar, int progress){

    int seekBarTablen = sizeof(SZKSeekBarCallbackTab) / sizeof(S_ZKSeekBarCallback);
    for (int i = 0; i < seekBarTablen; ++i) {
        if (SZKSeekBarCallbackTab[i].id == pSeekBar->getID()) {
            SZKSeekBarCallbackTab[i].callback(pSeekBar, progress);
            break;
        }
    }
}

int PhotoAlbumActivity::getListItemCount(const ZKListView *pListView) const{
    int tablen = sizeof(SListViewFunctionsCallbackTab) / sizeof(S_ListViewFunctionsCallback);
    for (int i = 0; i < tablen; ++i) {
        if (SListViewFunctionsCallbackTab[i].id == pListView->getID()) {
            return SListViewFunctionsCallbackTab[i].getListItemCountCallback(pListView);
            break;
        }
    }
    return 0;
}

void PhotoAlbumActivity::obtainListItemData(ZKListView *pListView,ZKListView::ZKListItem *pListItem, int index){
    int tablen = sizeof(SListViewFunctionsCallbackTab) / sizeof(S_ListViewFunctionsCallback);
    for (int i = 0; i < tablen; ++i) {
        if (SListViewFunctionsCallbackTab[i].id == pListView->getID()) {
            SListViewFunctionsCallbackTab[i].obtainListItemDataCallback(pListView, pListItem, index);
            break;
        }
    }
}

void PhotoAlbumActivity::onItemClick(ZKListView *pListView, int index, int id){
    int tablen = sizeof(SListViewFunctionsCallbackTab) / sizeof(S_ListViewFunctionsCallback);
    for (int i = 0; i < tablen; ++i) {
        if (SListViewFunctionsCallbackTab[i].id == pListView->getID()) {
            SListViewFunctionsCallbackTab[i].onItemClickCallback(pListView, index, id);
            break;
        }
    }
}

void PhotoAlbumActivity::onSlideItemClick(ZKSlideWindow *pSlideWindow, int index) {
    int tablen = sizeof(SSlideWindowItemClickCallbackTab) / sizeof(S_SlideWindowItemClickCallback);
    for (int i = 0; i < tablen; ++i) {
        if (SSlideWindowItemClickCallbackTab[i].id == pSlideWindow->getID()) {
            SSlideWindowItemClickCallbackTab[i].onSlideItemClickCallback(pSlideWindow, index);
            break;
        }
    }
}

bool PhotoAlbumActivity::onTouchEvent(const MotionEvent &ev) {
    return onPhotoAlbumActivityTouchEvent(ev);
}

void PhotoAlbumActivity::onTextChanged(ZKTextView *pTextView, const std::string &text) {
    int tablen = sizeof(SEditTextInputCallbackTab) / sizeof(S_EditTextInputCallback);
    for (int i = 0; i < tablen; ++i) {
        if (SEditTextInputCallbackTab[i].id == pTextView->getID()) {
            SEditTextInputCallbackTab[i].onEditTextChangedCallback(text);
            break;
        }
    }
}

void PhotoAlbumActivity::rigesterActivityTimer() {
    int tablen = sizeof(REGISTER_ACTIVITY_TIMER_TAB) / sizeof(S_ACTIVITY_TIMEER);
    for (int i = 0; i < tablen; ++i) {
        S_ACTIVITY_TIMEER temp = REGISTER_ACTIVITY_TIMER_TAB[i];
        registerTimer(temp.id, temp.time);
    }
}


void PhotoAlbumActivity::onVideoPlayerMessage(ZKVideoView *pVideoView, int msg) {
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

void PhotoAlbumActivity::videoLoopPlayback(ZKVideoView *pVideoView, int msg, size_t callbackTabIndex) {

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

void PhotoAlbumActivity::startVideoLoopPlayback() {
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

void PhotoAlbumActivity::stopVideoLoopPlayback() {
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

bool PhotoAlbumActivity::parseVideoFileList(const char *pFileListPath, std::vector<string>& mediaFileList) {
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

int PhotoAlbumActivity::removeCharFromString(string& nString, char c) {
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

void PhotoAlbumActivity::registerUserTimer(int id, int time) {
	registerTimer(id, time);
}

void PhotoAlbumActivity::unregisterUserTimer(int id) {
	unregisterTimer(id);
}

void PhotoAlbumActivity::resetUserTimer(int id, int time) {
	resetTimer(id, time);
}