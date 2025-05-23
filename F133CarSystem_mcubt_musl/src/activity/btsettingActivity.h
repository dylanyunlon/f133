/***********************************************
/gen auto by zuitools
***********************************************/
#ifndef __BTSETTINGACTIVITY_H__
#define __BTSETTINGACTIVITY_H__


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
#define ID_BTSETTING_connDevTextView    50024
#define ID_BTSETTING_TextViewBg    50023
#define ID_BTSETTING_btsettingButton    20007
#define ID_BTSETTING_btcontactsButton    20006
#define ID_BTSETTING_btrecordButton    20002
#define ID_BTSETTING_TextView15    50001
#define ID_BTSETTING_btsetWindow    110010
#define ID_BTSETTING_syncPointer    90001
#define ID_BTSETTING_scaningTextView    50012
#define ID_BTSETTING_scaningWindow    110008
#define ID_BTSETTING_openingPointer    90002
#define ID_BTSETTING_openingWnd    110009
#define ID_BTSETTING_TextView14    50011
#define ID_BTSETTING_btTipsWindow    110006
#define ID_BTSETTING_TextView6    50004
#define ID_BTSETTING_nameLongTipsWindow    110003
#define ID_BTSETTING_connectedNameTextView    50021
#define ID_BTSETTING_TextView3    50019
#define ID_BTSETTING_disconCancelButton    20034
#define ID_BTSETTING_disConnectButton    20033
#define ID_BTSETTING_TextView7    50020
#define ID_BTSETTING_disConnectWindow    110002
#define ID_BTSETTING_TextView9    50006
#define ID_BTSETTING_btSwitchTextView    50013
#define ID_BTSETTING_connDevTipsTextView    50018
#define ID_BTSETTING_connDevButton    20009
#define ID_BTSETTING_enableButton    20011
#define ID_BTSETTING_btNameEditText    51001
#define ID_BTSETTING_devNameTipsTextView    50015
#define ID_BTSETTING_TextView1    50017
#define ID_BTSETTING_TextView12    50009
#define ID_BTSETTING_TextView11    50008
#define ID_BTSETTING_TextView8    50005
#define ID_BTSETTING_TextView5    50022
#define ID_BTSETTING_PinEditText    51002
#define ID_BTSETTING_btBarSubItem    24006
#define ID_BTSETTING_btListView    80001
#define ID_BTSETTING_Window2    110005
#define ID_BTSETTING_matchBarSubItem    24001
#define ID_BTSETTING_matchedListView    80002
#define ID_BTSETTING_Window1    110004
#define ID_BTSETTING_queryMusicButton    20010
#define ID_BTSETTING_phoneButton    20008
#define ID_BTSETTING_btAppWindow    110001
#define ID_BTSETTING_scanButton    20004
#define ID_BTSETTING_autoConnButton    20028
#define ID_BTSETTING_autoAnswerButton    20027
#define ID_BTSETTING_TextView4    50003
#define ID_BTSETTING_TextView2    50002
#define ID_BTSETTING_TextView10    50007
#define ID_BTSETTING_TextView13    50010
#define ID_BTSETTING_autoWindow    110007
#define ID_BTSETTING_matchTextView    50014
#define ID_BTSETTING_scanDevTextView    50016
#define ID_BTSETTING_btEnableWindow    110012
#define ID_BTSETTING_btSettingWindow    110011
/*TAG:Macro宏ID END*/

class btsettingActivity : public Activity, 
                     public ZKSeekBar::ISeekBarChangeListener, 
                     public ZKListView::IItemClickListener,
                     public ZKListView::AbsListAdapter,
                     public ZKSlideWindow::ISlideItemClickListener,
                     public EasyUIContext::ITouchListener,
                     public ZKEditText::ITextChangeListener,
                     public ZKVideoView::IVideoPlayerMessageListener
{
public:
    btsettingActivity();
    virtual ~btsettingActivity();

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