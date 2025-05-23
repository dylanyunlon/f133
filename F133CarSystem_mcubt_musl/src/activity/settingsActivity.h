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
#define ID_SETTINGS_systemButton    20007
#define ID_SETTINGS_factoryButton    20005
#define ID_SETTINGS_reverseButton    20004
#define ID_SETTINGS_voiceButton    20003
#define ID_SETTINGS_linkButton    20002
#define ID_SETTINGS_showButton    20001
#define ID_SETTINGS_setWnd    110001
#define ID_SETTINGS_mcuTextView    50024
#define ID_SETTINGS_appTextView    50023
#define ID_SETTINGS_osTextView    50022
#define ID_SETTINGS_TextView17    50019
#define ID_SETTINGS_TextView16    50015
#define ID_SETTINGS_TextView15    50014
#define ID_SETTINGS_TextView14    50013
#define ID_SETTINGS_TextView13    50012
#define ID_SETTINGS_TextView10    50011
#define ID_SETTINGS_versionWindow    110013
#define ID_SETTINGS_hourButton24    20009
#define ID_SETTINGS_hourButton12    20008
#define ID_SETTINGS_TextView9    50010
#define ID_SETTINGS_TextView8    50007
#define ID_SETTINGS_TextView7    50006
#define ID_SETTINGS_TextView6    50005
#define ID_SETTINGS_TextView5    50004
#define ID_SETTINGS_TextView4    50003
#define ID_SETTINGS_TextView3    50001
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
#define ID_SETTINGS_setWindow    110011
#define ID_SETTINGS_SetdataWnd    110008
#define ID_SETTINGS_languagePicSub    24011
#define ID_SETTINGS_languageTextSub    24010
#define ID_SETTINGS_subitem    24009
#define ID_SETTINGS_LanguageListView    80004
#define ID_SETTINGS_LanguageWnd    110007
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