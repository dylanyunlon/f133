/***********************************************
/gen auto by zuitools
***********************************************/
#include "settingsActivity.h"

/*TAG:GlobalVariable全局变量*/
static ZKTextView* motpTextViewPtr;
static ZKWindow* motpWindowPtr;
static ZKWindow* mdeveloperWndPtr;
static ZKListView* mmoreListViewPtr;
static ZKWindow* mmoreWndPtr;
static ZKTextView* mtTextViewPtr;
static ZKButton* mtimeCancelButtonPtr;
static ZKButton* mSetTimeSureBtnPtr;
static ZKListView* mdayListViewPtr;
static ZKListView* mmonthListViewPtr;
static ZKListView* myearListViewPtr;
static ZKListView* mminListViewPtr;
static ZKListView* mhourListViewPtr;
static ZKTextView* mTextView3Ptr;
static ZKTextView* mTextView5Ptr;
static ZKTextView* mTextView4Ptr;
static ZKTextView* mTextView6Ptr;
static ZKTextView* mTextView7Ptr;
static ZKTextView* mTextView8Ptr;
static ZKTextView* mTextView9Ptr;
static ZKTextView* mTextView10Ptr;
static ZKTextView* mTextView13Ptr;
static ZKTextView* mTextView14Ptr;
static ZKTextView* mTextView15Ptr;
static ZKTextView* mTextView16Ptr;
static ZKTextView* mTextView17Ptr;
static ZKTextView* mTextView18Ptr;
static ZKTextView* mTextView19Ptr;
static ZKWindow* msetWindowPtr;
static ZKWindow* mSetdataWndPtr;
static ZKListView* mLanguageListViewPtr;
static ZKWindow* mLanguageWndPtr;
static ZKTextView* mTextView12Ptr;
static ZKTextView* mVersionTextPtr;
static ZKTextView* mTextView11Ptr;
static ZKTextView* mTextView40Ptr;
static ZKButton* mresetCancelButtonPtr;
static ZKButton* mresetConfirmButtonPtr;
static ZKWindow* mresetWindowPtr;
static ZKButton* mresetButtonPtr;
static ZKListView* msystemListViewPtr;
static ZKWindow* msystemWndPtr;
static ZKButton* mcvbsButtonPtr;
static ZKTextView* mTextView28Ptr;
static ZKTextView* mTextView27Ptr;
static ZKEditText* mrotvalueEditTextPtr;
static ZKButton* mSetRotBtnPtr;
static ZKButton* mSkipSureBtnPtr;
static ZKEditText* mSkipEditTextPtr;
static ZKTextView* mTextView1Ptr;
static ZKButton* mCutCameraBtnPtr;
static ZKTextView* mCameraTypeTextPtr;
static ZKTextView* mTextView2Ptr;
static ZKTextView* mformatTextViewPtr;
static ZKListView* mChannelListViewPtr;
static ZKListView* mcaminfoListViewPtr;
static ZKWindow* mCameraWndPtr;
static ZKListView* mReverseListViewPtr;
static ZKWindow* mreverseWndPtr;
static ZKTextView* mTextView24Ptr;
static ZKTextView* mTextView25Ptr;
static ZKTextView* mTextView26Ptr;
static ZKButton* msoundeffectBtnPtr;
static ZKTextView* mTextView23Ptr;
static ZKTextView* mTextView22Ptr;
static ZKTextView* mphonevaluePtr;
static ZKTextView* mmediavaluePtr;
static ZKTextView* mTextView21Ptr;
static ZKTextView* mTextView20Ptr;
static ZKSeekBar* mPhoneSeekBarPtr;
static ZKSeekBar* mMediaSeekBarPtr;
static ZKWindow* mvoiceWndPtr;
static ZKTextView* mcarBtTipsTextViewPtr;
static ZKButton* mcarBtCancelButtonPtr;
static ZKButton* mcarBtOkButtonPtr;
static ZKWindow* mlinkVoiceWindowPtr;
static ZKButton* mcfButtonPtr;
static ZKButton* motpButtonPtr;
static ZKTextView* mmfiTextViewPtr;
static ZKButton* mlinkAuthorizeButtonPtr;
static ZKButton* mbtAuthorizeButtonPtr;
static ZKButton* mSetLinkTypeBtnPtr;
static ZKButton* mSetLinkVoiceBtnPtr;
static ZKTextView* mLink_typeTextPtr;
static ZKTextView* mTextView30Ptr;
static ZKTextView* mTextView38Ptr;
static ZKTextView* mTextView31Ptr;
static ZKTextView* mTextView41Ptr;
static ZKWindow* mlinkWndPtr;
static ZKButton* mBtnRightPtr;
static ZKButton* mBtnLeftPtr;
static ZKTextView* mBrillianceValTextViewPtr;
static ZKTextView* mShowTextPtr;
static ZKSeekBar* mBrillianceSeekBarPtr;
static ZKWindow* mBrightnessWndPtr;
static ZKTextView* mBrightnessTextPtr;
static ZKTextView* mTextView29Ptr;
static ZKSeekBar* mBrightnessSeekBarPtr;
static ZKListView* mshowListViewPtr;
static ZKButton* mcolorButtonPtr;
static ZKTextView* mTextView36Ptr;
static ZKTextView* mTextView37Ptr;
static ZKTextView* mTextView39Ptr;
static ZKTextView* mbrightnessTextViewPtr;
static ZKWindow* mshowWndPtr;
static ZKListView* mSettingListViewPtr;
static ZKWindow* mWindow1Ptr;
static ZKButton* mReturnBtnPtr;
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
    ID_SETTINGS_timeCancelButton, onButtonClick_timeCancelButton,
    ID_SETTINGS_SetTimeSureBtn, onButtonClick_SetTimeSureBtn,
    ID_SETTINGS_resetCancelButton, onButtonClick_resetCancelButton,
    ID_SETTINGS_resetConfirmButton, onButtonClick_resetConfirmButton,
    ID_SETTINGS_resetButton, onButtonClick_resetButton,
    ID_SETTINGS_cvbsButton, onButtonClick_cvbsButton,
    ID_SETTINGS_SetRotBtn, onButtonClick_SetRotBtn,
    ID_SETTINGS_SkipSureBtn, onButtonClick_SkipSureBtn,
    ID_SETTINGS_CutCameraBtn, onButtonClick_CutCameraBtn,
    ID_SETTINGS_soundeffectBtn, onButtonClick_soundeffectBtn,
    ID_SETTINGS_carBtCancelButton, onButtonClick_carBtCancelButton,
    ID_SETTINGS_carBtOkButton, onButtonClick_carBtOkButton,
    ID_SETTINGS_cfButton, onButtonClick_cfButton,
    ID_SETTINGS_otpButton, onButtonClick_otpButton,
    ID_SETTINGS_linkAuthorizeButton, onButtonClick_linkAuthorizeButton,
    ID_SETTINGS_btAuthorizeButton, onButtonClick_btAuthorizeButton,
    ID_SETTINGS_SetLinkTypeBtn, onButtonClick_SetLinkTypeBtn,
    ID_SETTINGS_SetLinkVoiceBtn, onButtonClick_SetLinkVoiceBtn,
    ID_SETTINGS_BtnRight, onButtonClick_BtnRight,
    ID_SETTINGS_BtnLeft, onButtonClick_BtnLeft,
    ID_SETTINGS_colorButton, onButtonClick_colorButton,
    ID_SETTINGS_ReturnBtn, onButtonClick_ReturnBtn,
};
/***************/


typedef void (*SeekBarCallback)(ZKSeekBar *pSeekBar, int progress);
typedef struct {
    int id;
    SeekBarCallback callback;
}S_ZKSeekBarCallback;
/*TAG:SeekBarCallbackTab*/
static S_ZKSeekBarCallback SZKSeekBarCallbackTab[] = {
    ID_SETTINGS_PhoneSeekBar, onProgressChanged_PhoneSeekBar,
    ID_SETTINGS_MediaSeekBar, onProgressChanged_MediaSeekBar,
    ID_SETTINGS_BrillianceSeekBar, onProgressChanged_BrillianceSeekBar,
    ID_SETTINGS_BrightnessSeekBar, onProgressChanged_BrightnessSeekBar,
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
    ID_SETTINGS_moreListView, getListItemCount_moreListView, obtainListItemData_moreListView, onListItemClick_moreListView,
    ID_SETTINGS_dayListView, getListItemCount_dayListView, obtainListItemData_dayListView, onListItemClick_dayListView,
    ID_SETTINGS_monthListView, getListItemCount_monthListView, obtainListItemData_monthListView, onListItemClick_monthListView,
    ID_SETTINGS_yearListView, getListItemCount_yearListView, obtainListItemData_yearListView, onListItemClick_yearListView,
    ID_SETTINGS_minListView, getListItemCount_minListView, obtainListItemData_minListView, onListItemClick_minListView,
    ID_SETTINGS_hourListView, getListItemCount_hourListView, obtainListItemData_hourListView, onListItemClick_hourListView,
    ID_SETTINGS_LanguageListView, getListItemCount_LanguageListView, obtainListItemData_LanguageListView, onListItemClick_LanguageListView,
    ID_SETTINGS_systemListView, getListItemCount_systemListView, obtainListItemData_systemListView, onListItemClick_systemListView,
    ID_SETTINGS_ChannelListView, getListItemCount_ChannelListView, obtainListItemData_ChannelListView, onListItemClick_ChannelListView,
    ID_SETTINGS_caminfoListView, getListItemCount_caminfoListView, obtainListItemData_caminfoListView, onListItemClick_caminfoListView,
    ID_SETTINGS_ReverseListView, getListItemCount_ReverseListView, obtainListItemData_ReverseListView, onListItemClick_ReverseListView,
    ID_SETTINGS_showListView, getListItemCount_showListView, obtainListItemData_showListView, onListItemClick_showListView,
    ID_SETTINGS_SettingListView, getListItemCount_SettingListView, obtainListItemData_SettingListView, onListItemClick_SettingListView,
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
    ID_SETTINGS_rotvalueEditText, onEditTextChanged_rotvalueEditText,
    ID_SETTINGS_SkipEditText, onEditTextChanged_SkipEditText,
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
    motpTextViewPtr = NULL;
    motpWindowPtr = NULL;
    mdeveloperWndPtr = NULL;
    mmoreListViewPtr = NULL;
    mmoreWndPtr = NULL;
    mtTextViewPtr = NULL;
    mtimeCancelButtonPtr = NULL;
    mSetTimeSureBtnPtr = NULL;
    mdayListViewPtr = NULL;
    mmonthListViewPtr = NULL;
    myearListViewPtr = NULL;
    mminListViewPtr = NULL;
    mhourListViewPtr = NULL;
    mTextView3Ptr = NULL;
    mTextView5Ptr = NULL;
    mTextView4Ptr = NULL;
    mTextView6Ptr = NULL;
    mTextView7Ptr = NULL;
    mTextView8Ptr = NULL;
    mTextView9Ptr = NULL;
    mTextView10Ptr = NULL;
    mTextView13Ptr = NULL;
    mTextView14Ptr = NULL;
    mTextView15Ptr = NULL;
    mTextView16Ptr = NULL;
    mTextView17Ptr = NULL;
    mTextView18Ptr = NULL;
    mTextView19Ptr = NULL;
    msetWindowPtr = NULL;
    mSetdataWndPtr = NULL;
    mLanguageListViewPtr = NULL;
    mLanguageWndPtr = NULL;
    mTextView12Ptr = NULL;
    mVersionTextPtr = NULL;
    mTextView11Ptr = NULL;
    mTextView40Ptr = NULL;
    mresetCancelButtonPtr = NULL;
    mresetConfirmButtonPtr = NULL;
    mresetWindowPtr = NULL;
    mresetButtonPtr = NULL;
    msystemListViewPtr = NULL;
    msystemWndPtr = NULL;
    mcvbsButtonPtr = NULL;
    mTextView28Ptr = NULL;
    mTextView27Ptr = NULL;
    mrotvalueEditTextPtr = NULL;
    mSetRotBtnPtr = NULL;
    mSkipSureBtnPtr = NULL;
    mSkipEditTextPtr = NULL;
    mTextView1Ptr = NULL;
    mCutCameraBtnPtr = NULL;
    mCameraTypeTextPtr = NULL;
    mTextView2Ptr = NULL;
    mformatTextViewPtr = NULL;
    mChannelListViewPtr = NULL;
    mcaminfoListViewPtr = NULL;
    mCameraWndPtr = NULL;
    mReverseListViewPtr = NULL;
    mreverseWndPtr = NULL;
    mTextView24Ptr = NULL;
    mTextView25Ptr = NULL;
    mTextView26Ptr = NULL;
    msoundeffectBtnPtr = NULL;
    mTextView23Ptr = NULL;
    mTextView22Ptr = NULL;
    mphonevaluePtr = NULL;
    mmediavaluePtr = NULL;
    mTextView21Ptr = NULL;
    mTextView20Ptr = NULL;
    mPhoneSeekBarPtr = NULL;
    mMediaSeekBarPtr = NULL;
    mvoiceWndPtr = NULL;
    mcarBtTipsTextViewPtr = NULL;
    mcarBtCancelButtonPtr = NULL;
    mcarBtOkButtonPtr = NULL;
    mlinkVoiceWindowPtr = NULL;
    mcfButtonPtr = NULL;
    motpButtonPtr = NULL;
    mmfiTextViewPtr = NULL;
    mlinkAuthorizeButtonPtr = NULL;
    mbtAuthorizeButtonPtr = NULL;
    mSetLinkTypeBtnPtr = NULL;
    mSetLinkVoiceBtnPtr = NULL;
    mLink_typeTextPtr = NULL;
    mTextView30Ptr = NULL;
    mTextView38Ptr = NULL;
    mTextView31Ptr = NULL;
    mTextView41Ptr = NULL;
    mlinkWndPtr = NULL;
    mBtnRightPtr = NULL;
    mBtnLeftPtr = NULL;
    mBrillianceValTextViewPtr = NULL;
    mShowTextPtr = NULL;
    mBrillianceSeekBarPtr = NULL;
    mBrightnessWndPtr = NULL;
    mBrightnessTextPtr = NULL;
    mTextView29Ptr = NULL;
    mBrightnessSeekBarPtr = NULL;
    mshowListViewPtr = NULL;
    mcolorButtonPtr = NULL;
    mTextView36Ptr = NULL;
    mTextView37Ptr = NULL;
    mTextView39Ptr = NULL;
    mbrightnessTextViewPtr = NULL;
    mshowWndPtr = NULL;
    mSettingListViewPtr = NULL;
    mWindow1Ptr = NULL;
    mReturnBtnPtr = NULL;
}

const char* settingsActivity::getAppName() const{
	return "settings.ftu";
}

//TAG:onCreate
void settingsActivity::onCreate() {
	Activity::onCreate();
    motpTextViewPtr = (ZKTextView*)findControlByID(ID_SETTINGS_otpTextView);
    motpWindowPtr = (ZKWindow*)findControlByID(ID_SETTINGS_otpWindow);
    mdeveloperWndPtr = (ZKWindow*)findControlByID(ID_SETTINGS_developerWnd);
    mmoreListViewPtr = (ZKListView*)findControlByID(ID_SETTINGS_moreListView);if(mmoreListViewPtr!= NULL){mmoreListViewPtr->setListAdapter(this);mmoreListViewPtr->setItemClickListener(this);}
    mmoreWndPtr = (ZKWindow*)findControlByID(ID_SETTINGS_moreWnd);
    mtTextViewPtr = (ZKTextView*)findControlByID(ID_SETTINGS_tTextView);
    mtimeCancelButtonPtr = (ZKButton*)findControlByID(ID_SETTINGS_timeCancelButton);
    mSetTimeSureBtnPtr = (ZKButton*)findControlByID(ID_SETTINGS_SetTimeSureBtn);
    mdayListViewPtr = (ZKListView*)findControlByID(ID_SETTINGS_dayListView);if(mdayListViewPtr!= NULL){mdayListViewPtr->setListAdapter(this);mdayListViewPtr->setItemClickListener(this);}
    mmonthListViewPtr = (ZKListView*)findControlByID(ID_SETTINGS_monthListView);if(mmonthListViewPtr!= NULL){mmonthListViewPtr->setListAdapter(this);mmonthListViewPtr->setItemClickListener(this);}
    myearListViewPtr = (ZKListView*)findControlByID(ID_SETTINGS_yearListView);if(myearListViewPtr!= NULL){myearListViewPtr->setListAdapter(this);myearListViewPtr->setItemClickListener(this);}
    mminListViewPtr = (ZKListView*)findControlByID(ID_SETTINGS_minListView);if(mminListViewPtr!= NULL){mminListViewPtr->setListAdapter(this);mminListViewPtr->setItemClickListener(this);}
    mhourListViewPtr = (ZKListView*)findControlByID(ID_SETTINGS_hourListView);if(mhourListViewPtr!= NULL){mhourListViewPtr->setListAdapter(this);mhourListViewPtr->setItemClickListener(this);}
    mTextView3Ptr = (ZKTextView*)findControlByID(ID_SETTINGS_TextView3);
    mTextView5Ptr = (ZKTextView*)findControlByID(ID_SETTINGS_TextView5);
    mTextView4Ptr = (ZKTextView*)findControlByID(ID_SETTINGS_TextView4);
    mTextView6Ptr = (ZKTextView*)findControlByID(ID_SETTINGS_TextView6);
    mTextView7Ptr = (ZKTextView*)findControlByID(ID_SETTINGS_TextView7);
    mTextView8Ptr = (ZKTextView*)findControlByID(ID_SETTINGS_TextView8);
    mTextView9Ptr = (ZKTextView*)findControlByID(ID_SETTINGS_TextView9);
    mTextView10Ptr = (ZKTextView*)findControlByID(ID_SETTINGS_TextView10);
    mTextView13Ptr = (ZKTextView*)findControlByID(ID_SETTINGS_TextView13);
    mTextView14Ptr = (ZKTextView*)findControlByID(ID_SETTINGS_TextView14);
    mTextView15Ptr = (ZKTextView*)findControlByID(ID_SETTINGS_TextView15);
    mTextView16Ptr = (ZKTextView*)findControlByID(ID_SETTINGS_TextView16);
    mTextView17Ptr = (ZKTextView*)findControlByID(ID_SETTINGS_TextView17);
    mTextView18Ptr = (ZKTextView*)findControlByID(ID_SETTINGS_TextView18);
    mTextView19Ptr = (ZKTextView*)findControlByID(ID_SETTINGS_TextView19);
    msetWindowPtr = (ZKWindow*)findControlByID(ID_SETTINGS_setWindow);
    mSetdataWndPtr = (ZKWindow*)findControlByID(ID_SETTINGS_SetdataWnd);
    mLanguageListViewPtr = (ZKListView*)findControlByID(ID_SETTINGS_LanguageListView);if(mLanguageListViewPtr!= NULL){mLanguageListViewPtr->setListAdapter(this);mLanguageListViewPtr->setItemClickListener(this);}
    mLanguageWndPtr = (ZKWindow*)findControlByID(ID_SETTINGS_LanguageWnd);
    mTextView12Ptr = (ZKTextView*)findControlByID(ID_SETTINGS_TextView12);
    mVersionTextPtr = (ZKTextView*)findControlByID(ID_SETTINGS_VersionText);
    mTextView11Ptr = (ZKTextView*)findControlByID(ID_SETTINGS_TextView11);
    mTextView40Ptr = (ZKTextView*)findControlByID(ID_SETTINGS_TextView40);
    mresetCancelButtonPtr = (ZKButton*)findControlByID(ID_SETTINGS_resetCancelButton);
    mresetConfirmButtonPtr = (ZKButton*)findControlByID(ID_SETTINGS_resetConfirmButton);
    mresetWindowPtr = (ZKWindow*)findControlByID(ID_SETTINGS_resetWindow);
    mresetButtonPtr = (ZKButton*)findControlByID(ID_SETTINGS_resetButton);
    msystemListViewPtr = (ZKListView*)findControlByID(ID_SETTINGS_systemListView);if(msystemListViewPtr!= NULL){msystemListViewPtr->setListAdapter(this);msystemListViewPtr->setItemClickListener(this);}
    msystemWndPtr = (ZKWindow*)findControlByID(ID_SETTINGS_systemWnd);
    mcvbsButtonPtr = (ZKButton*)findControlByID(ID_SETTINGS_cvbsButton);
    mTextView28Ptr = (ZKTextView*)findControlByID(ID_SETTINGS_TextView28);
    mTextView27Ptr = (ZKTextView*)findControlByID(ID_SETTINGS_TextView27);
    mrotvalueEditTextPtr = (ZKEditText*)findControlByID(ID_SETTINGS_rotvalueEditText);if(mrotvalueEditTextPtr!= NULL){mrotvalueEditTextPtr->setTextChangeListener(this);}
    mSetRotBtnPtr = (ZKButton*)findControlByID(ID_SETTINGS_SetRotBtn);
    mSkipSureBtnPtr = (ZKButton*)findControlByID(ID_SETTINGS_SkipSureBtn);
    mSkipEditTextPtr = (ZKEditText*)findControlByID(ID_SETTINGS_SkipEditText);if(mSkipEditTextPtr!= NULL){mSkipEditTextPtr->setTextChangeListener(this);}
    mTextView1Ptr = (ZKTextView*)findControlByID(ID_SETTINGS_TextView1);
    mCutCameraBtnPtr = (ZKButton*)findControlByID(ID_SETTINGS_CutCameraBtn);
    mCameraTypeTextPtr = (ZKTextView*)findControlByID(ID_SETTINGS_CameraTypeText);
    mTextView2Ptr = (ZKTextView*)findControlByID(ID_SETTINGS_TextView2);
    mformatTextViewPtr = (ZKTextView*)findControlByID(ID_SETTINGS_formatTextView);
    mChannelListViewPtr = (ZKListView*)findControlByID(ID_SETTINGS_ChannelListView);if(mChannelListViewPtr!= NULL){mChannelListViewPtr->setListAdapter(this);mChannelListViewPtr->setItemClickListener(this);}
    mcaminfoListViewPtr = (ZKListView*)findControlByID(ID_SETTINGS_caminfoListView);if(mcaminfoListViewPtr!= NULL){mcaminfoListViewPtr->setListAdapter(this);mcaminfoListViewPtr->setItemClickListener(this);}
    mCameraWndPtr = (ZKWindow*)findControlByID(ID_SETTINGS_CameraWnd);
    mReverseListViewPtr = (ZKListView*)findControlByID(ID_SETTINGS_ReverseListView);if(mReverseListViewPtr!= NULL){mReverseListViewPtr->setListAdapter(this);mReverseListViewPtr->setItemClickListener(this);}
    mreverseWndPtr = (ZKWindow*)findControlByID(ID_SETTINGS_reverseWnd);
    mTextView24Ptr = (ZKTextView*)findControlByID(ID_SETTINGS_TextView24);
    mTextView25Ptr = (ZKTextView*)findControlByID(ID_SETTINGS_TextView25);
    mTextView26Ptr = (ZKTextView*)findControlByID(ID_SETTINGS_TextView26);
    msoundeffectBtnPtr = (ZKButton*)findControlByID(ID_SETTINGS_soundeffectBtn);
    mTextView23Ptr = (ZKTextView*)findControlByID(ID_SETTINGS_TextView23);
    mTextView22Ptr = (ZKTextView*)findControlByID(ID_SETTINGS_TextView22);
    mphonevaluePtr = (ZKTextView*)findControlByID(ID_SETTINGS_phonevalue);
    mmediavaluePtr = (ZKTextView*)findControlByID(ID_SETTINGS_mediavalue);
    mTextView21Ptr = (ZKTextView*)findControlByID(ID_SETTINGS_TextView21);
    mTextView20Ptr = (ZKTextView*)findControlByID(ID_SETTINGS_TextView20);
    mPhoneSeekBarPtr = (ZKSeekBar*)findControlByID(ID_SETTINGS_PhoneSeekBar);if(mPhoneSeekBarPtr!= NULL){mPhoneSeekBarPtr->setSeekBarChangeListener(this);}
    mMediaSeekBarPtr = (ZKSeekBar*)findControlByID(ID_SETTINGS_MediaSeekBar);if(mMediaSeekBarPtr!= NULL){mMediaSeekBarPtr->setSeekBarChangeListener(this);}
    mvoiceWndPtr = (ZKWindow*)findControlByID(ID_SETTINGS_voiceWnd);
    mcarBtTipsTextViewPtr = (ZKTextView*)findControlByID(ID_SETTINGS_carBtTipsTextView);
    mcarBtCancelButtonPtr = (ZKButton*)findControlByID(ID_SETTINGS_carBtCancelButton);
    mcarBtOkButtonPtr = (ZKButton*)findControlByID(ID_SETTINGS_carBtOkButton);
    mlinkVoiceWindowPtr = (ZKWindow*)findControlByID(ID_SETTINGS_linkVoiceWindow);
    mcfButtonPtr = (ZKButton*)findControlByID(ID_SETTINGS_cfButton);
    motpButtonPtr = (ZKButton*)findControlByID(ID_SETTINGS_otpButton);
    mmfiTextViewPtr = (ZKTextView*)findControlByID(ID_SETTINGS_mfiTextView);
    mlinkAuthorizeButtonPtr = (ZKButton*)findControlByID(ID_SETTINGS_linkAuthorizeButton);
    mbtAuthorizeButtonPtr = (ZKButton*)findControlByID(ID_SETTINGS_btAuthorizeButton);
    mSetLinkTypeBtnPtr = (ZKButton*)findControlByID(ID_SETTINGS_SetLinkTypeBtn);
    mSetLinkVoiceBtnPtr = (ZKButton*)findControlByID(ID_SETTINGS_SetLinkVoiceBtn);
    mLink_typeTextPtr = (ZKTextView*)findControlByID(ID_SETTINGS_Link_typeText);
    mTextView30Ptr = (ZKTextView*)findControlByID(ID_SETTINGS_TextView30);
    mTextView38Ptr = (ZKTextView*)findControlByID(ID_SETTINGS_TextView38);
    mTextView31Ptr = (ZKTextView*)findControlByID(ID_SETTINGS_TextView31);
    mTextView41Ptr = (ZKTextView*)findControlByID(ID_SETTINGS_TextView41);
    mlinkWndPtr = (ZKWindow*)findControlByID(ID_SETTINGS_linkWnd);
    mBtnRightPtr = (ZKButton*)findControlByID(ID_SETTINGS_BtnRight);
    mBtnLeftPtr = (ZKButton*)findControlByID(ID_SETTINGS_BtnLeft);
    mBrillianceValTextViewPtr = (ZKTextView*)findControlByID(ID_SETTINGS_BrillianceValTextView);
    mShowTextPtr = (ZKTextView*)findControlByID(ID_SETTINGS_ShowText);
    mBrillianceSeekBarPtr = (ZKSeekBar*)findControlByID(ID_SETTINGS_BrillianceSeekBar);if(mBrillianceSeekBarPtr!= NULL){mBrillianceSeekBarPtr->setSeekBarChangeListener(this);}
    mBrightnessWndPtr = (ZKWindow*)findControlByID(ID_SETTINGS_BrightnessWnd);
    mBrightnessTextPtr = (ZKTextView*)findControlByID(ID_SETTINGS_BrightnessText);
    mTextView29Ptr = (ZKTextView*)findControlByID(ID_SETTINGS_TextView29);
    mBrightnessSeekBarPtr = (ZKSeekBar*)findControlByID(ID_SETTINGS_BrightnessSeekBar);if(mBrightnessSeekBarPtr!= NULL){mBrightnessSeekBarPtr->setSeekBarChangeListener(this);}
    mshowListViewPtr = (ZKListView*)findControlByID(ID_SETTINGS_showListView);if(mshowListViewPtr!= NULL){mshowListViewPtr->setListAdapter(this);mshowListViewPtr->setItemClickListener(this);}
    mcolorButtonPtr = (ZKButton*)findControlByID(ID_SETTINGS_colorButton);
    mTextView36Ptr = (ZKTextView*)findControlByID(ID_SETTINGS_TextView36);
    mTextView37Ptr = (ZKTextView*)findControlByID(ID_SETTINGS_TextView37);
    mTextView39Ptr = (ZKTextView*)findControlByID(ID_SETTINGS_TextView39);
    mbrightnessTextViewPtr = (ZKTextView*)findControlByID(ID_SETTINGS_brightnessTextView);
    mshowWndPtr = (ZKWindow*)findControlByID(ID_SETTINGS_showWnd);
    mSettingListViewPtr = (ZKListView*)findControlByID(ID_SETTINGS_SettingListView);if(mSettingListViewPtr!= NULL){mSettingListViewPtr->setListAdapter(this);mSettingListViewPtr->setItemClickListener(this);}
    mWindow1Ptr = (ZKWindow*)findControlByID(ID_SETTINGS_Window1);
    mReturnBtnPtr = (ZKButton*)findControlByID(ID_SETTINGS_ReturnBtn);
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