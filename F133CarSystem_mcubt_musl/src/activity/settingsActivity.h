/***********************************************
/gen auto by zuitools
***********************************************/
#ifndef __SETTINGSACTIVITY_H__
#define __SETTINGSACTIVITY_H__


#include "app/Activity.h"
#include "entry/EasyUIContext.h"

#include "uart/ProtocolData.h"
#include "uart/ProtocolParser.h"

#include "utils/Log.h"
#include "control/ZKDigitalClock.h"
#include "control/ZKButton.h"
#include "control/ZKCircleBar.h"
#include "control/ZKDiagram.h"
#include "control/ZKListView.h"
#include "control/ZKPointer.h"
#include "control/ZKQRCode.h"
#include "control/ZKTextView.h"
#include "control/ZKSeekBar.h"
#include "control/ZKEditText.h"
#include "control/ZKVideoView.h"
#include "window/ZKSlideWindow.h"

/*TAG:Macro宏ID*/
#define ID_SETTINGS_otpTextView    50056
#define ID_SETTINGS_otpWindow    110016
#define ID_SETTINGS_developerWnd    110012
#define ID_SETTINGS_moreTextSub    24020
#define ID_SETTINGS_SubItem6    24019
#define ID_SETTINGS_SubItem5    24018
#define ID_SETTINGS_moreListView    80008
#define ID_SETTINGS_moreWnd    110006
#define ID_SETTINGS_tTextView    50001
#define ID_SETTINGS_timeCancelButton    20014
#define ID_SETTINGS_SetTimeSureBtn    20006
#define ID_SETTINGS_Subd    24017
#define ID_SETTINGS_dayListView    80019
#define ID_SETTINGS_Subm    24016
#define ID_SETTINGS_monthListView    80021
#define ID_SETTINGS_Suby    24015
#define ID_SETTINGS_yearListView    80005
#define ID_SETTINGS_Subs    24014
#define ID_SETTINGS_minListView    80016
#define ID_SETTINGS_SubItem4    24013
#define ID_SETTINGS_Subh    24012
#define ID_SETTINGS_hourListView    80015
#define ID_SETTINGS_TextView3    50003
#define ID_SETTINGS_TextView5    50005
#define ID_SETTINGS_TextView4    50004
#define ID_SETTINGS_TextView6    50006
#define ID_SETTINGS_TextView7    50007
#define ID_SETTINGS_TextView8    50010
#define ID_SETTINGS_TextView9    50011
#define ID_SETTINGS_TextView10    50012
#define ID_SETTINGS_TextView13    50013
#define ID_SETTINGS_TextView14    50014
#define ID_SETTINGS_TextView15    50015
#define ID_SETTINGS_TextView16    50022
#define ID_SETTINGS_TextView17    50023
#define ID_SETTINGS_TextView18    50024
#define ID_SETTINGS_TextView19    50025
#define ID_SETTINGS_setWindow    110011
#define ID_SETTINGS_SetdataWnd    110008
#define ID_SETTINGS_languagePicSub    24011
#define ID_SETTINGS_languageTextSub    24010
#define ID_SETTINGS_subitem    24009
#define ID_SETTINGS_LanguageListView    80004
#define ID_SETTINGS_LanguageWnd    110007
#define ID_SETTINGS_TextView12    50021
#define ID_SETTINGS_VersionText    50017
#define ID_SETTINGS_TextView11    50016
#define ID_SETTINGS_TextView40    50057
#define ID_SETTINGS_resetCancelButton    20019
#define ID_SETTINGS_resetConfirmButton    20018
#define ID_SETTINGS_resetWindow    110017
#define ID_SETTINGS_resetButton    20017
#define ID_SETTINGS_SubItemAdb    24030
#define ID_SETTINGS_systemSubText    24024
#define ID_SETTINGS_SubItemSysMore    24023
#define ID_SETTINGS_SubItem8    24022
#define ID_SETTINGS_systemListView    80009
#define ID_SETTINGS_systemWnd    110005
#define ID_SETTINGS_cvbsButton    20013
#define ID_SETTINGS_TextView28    50037
#define ID_SETTINGS_TextView27    50036
#define ID_SETTINGS_rotvalueEditText    51002
#define ID_SETTINGS_SetRotBtn    20009
#define ID_SETTINGS_SkipSureBtn    20008
#define ID_SETTINGS_SkipEditText    51001
#define ID_SETTINGS_TextView1    50020
#define ID_SETTINGS_CutCameraBtn    20007
#define ID_SETTINGS_CameraTypeText    50019
#define ID_SETTINGS_TextView2    50018
#define ID_SETTINGS_formatTextView    50002
#define ID_SETTINGS_ChannelListView    80007
#define ID_SETTINGS_caminfoListView    80006
#define ID_SETTINGS_CameraWnd    110010
#define ID_SETTINGS_reverseTextSub    24008
#define ID_SETTINGS_SubItem3    24007
#define ID_SETTINGS_SubItem2    24006
#define ID_SETTINGS_ReverseListView    80003
#define ID_SETTINGS_reverseWnd    110004
#define ID_SETTINGS_TextView24    50033
#define ID_SETTINGS_TextView25    50034
#define ID_SETTINGS_TextView26    50035
#define ID_SETTINGS_soundeffectBtn    20005
#define ID_SETTINGS_TextView23    50032
#define ID_SETTINGS_TextView22    50031
#define ID_SETTINGS_phonevalue    50030
#define ID_SETTINGS_mediavalue    50029
#define ID_SETTINGS_TextView21    50028
#define ID_SETTINGS_TextView20    50027
#define ID_SETTINGS_PhoneSeekBar    91003
#define ID_SETTINGS_MediaSeekBar    91002
#define ID_SETTINGS_voiceWnd    110003
#define ID_SETTINGS_carBtTipsTextView    50065
#define ID_SETTINGS_carBtCancelButton    20029
#define ID_SETTINGS_carBtOkButton    20028
#define ID_SETTINGS_linkVoiceWindow    110019
#define ID_SETTINGS_cfButton    20020
#define ID_SETTINGS_otpButton    20016
#define ID_SETTINGS_mfiTextView    50058
#define ID_SETTINGS_linkAuthorizeButton    20037
#define ID_SETTINGS_btAuthorizeButton    20036
#define ID_SETTINGS_SetLinkTypeBtn    20001
#define ID_SETTINGS_SetLinkVoiceBtn    20010
#define ID_SETTINGS_Link_typeText    50054
#define ID_SETTINGS_TextView30    50039
#define ID_SETTINGS_TextView38    50053
#define ID_SETTINGS_TextView31    50040
#define ID_SETTINGS_TextView41    50055
#define ID_SETTINGS_linkWnd    110002
#define ID_SETTINGS_BtnRight    20004
#define ID_SETTINGS_BtnLeft    20002
#define ID_SETTINGS_BrillianceValTextView    50009
#define ID_SETTINGS_ShowText    50008
#define ID_SETTINGS_BrillianceSeekBar    91005
#define ID_SETTINGS_BrightnessWnd    110009
#define ID_SETTINGS_BrightnessText    50026
#define ID_SETTINGS_TextView29    50038
#define ID_SETTINGS_BrightnessSeekBar    91001
#define ID_SETTINGS_showtext    24005
#define ID_SETTINGS_showpic    24004
#define ID_SETTINGS_SubItem1    24003
#define ID_SETTINGS_showListView    80002
#define ID_SETTINGS_colorButton    20015
#define ID_SETTINGS_TextView36    50046
#define ID_SETTINGS_TextView37    50047
#define ID_SETTINGS_TextView39    50049
#define ID_SETTINGS_brightnessTextView    50048
#define ID_SETTINGS_showWnd    110001
#define ID_SETTINGS_SubItem10    24025
#define ID_SETTINGS_SettingpicSub    24002
#define ID_SETTINGS_settingTextSub    24001
#define ID_SETTINGS_SubItem7    24021
#define ID_SETTINGS_SettingListView    80001
#define ID_SETTINGS_Window1    110014
#define ID_SETTINGS_ReturnBtn    20003
/*TAG:Macro宏ID END*/

class settingsActivity : public Activity, 
                     public ZKSeekBar::ISeekBarChangeListener, 
                     public ZKListView::IItemClickListener,
                     public ZKListView::AbsListAdapter,
                     public ZKSlideWindow::ISlideItemClickListener,
                     public EasyUIContext::ITouchListener,
                     public ZKEditText::ITextChangeListener,
                     public ZKVideoView::IVideoPlayerMessageListener
{
public:
    settingsActivity();
    virtual ~settingsActivity();

    /**
     * 注册定时器
     */
	void registerUserTimer(int id, int time);
	/**
	 * 取消定时器
	 */
	void unregisterUserTimer(int id);
	/**
	 * 重置定时器
	 */
	void resetUserTimer(int id, int time);

protected:
    /*TAG:PROTECTED_FUNCTION*/
    virtual const char* getAppName() const;
    virtual void onCreate();
    virtual void onClick(ZKBase *pBase);
    virtual void onResume();
    virtual void onPause();
    virtual void onIntent(const Intent *intentPtr);
    virtual bool onTimer(int id);

    virtual void onProgressChanged(ZKSeekBar *pSeekBar, int progress);

    virtual int getListItemCount(const ZKListView *pListView) const;
    virtual void obtainListItemData(ZKListView *pListView, ZKListView::ZKListItem *pListItem, int index);
    virtual void onItemClick(ZKListView *pListView, int index, int subItemIndex);

    virtual void onSlideItemClick(ZKSlideWindow *pSlideWindow, int index);

    virtual bool onTouchEvent(const MotionEvent &ev);

    virtual void onTextChanged(ZKTextView *pTextView, const string &text);

    void rigesterActivityTimer();

    virtual void onVideoPlayerMessage(ZKVideoView *pVideoView, int msg);
    void videoLoopPlayback(ZKVideoView *pVideoView, int msg, size_t callbackTabIndex);
    void startVideoLoopPlayback();
    void stopVideoLoopPlayback();
    bool parseVideoFileList(const char *pFileListPath, std::vector<string>& mediaFileList);
    int removeCharFromString(string& nString, char c);


private:
    /*TAG:PRIVATE_VARIABLE*/
    int mVideoLoopIndex;
    int mVideoLoopErrorCount;

};

#endif