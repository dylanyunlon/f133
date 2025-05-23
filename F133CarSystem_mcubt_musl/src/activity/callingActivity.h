/***********************************************
/gen auto by zuitools
***********************************************/
#ifndef __CALLINGACTIVITY_H__
#define __CALLINGACTIVITY_H__


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
#define ID_CALLING_numberTextView    50006
#define ID_CALLING_TextView15    50005
#define ID_CALLING_btsettingButton    20007
#define ID_CALLING_btcontactsButton    20004
#define ID_CALLING_btrecordButton    20002
#define ID_CALLING_phoneButton    20008
#define ID_CALLING_queryMusicButton    20010
#define ID_CALLING_btAppWindow    110002
#define ID_CALLING_outhangupButton    20012
#define ID_CALLING_outWindow    110005
#define ID_CALLING_inanswerButton    20011
#define ID_CALLING_inhangupButton    20009
#define ID_CALLING_inWindow    110004
#define ID_CALLING_InfoWnd    110003
#define ID_CALLING_delButton    20025
#define ID_CALLING_keyXINGButton    20024
#define ID_CALLING_keyJINGButton    20023
#define ID_CALLING_key0Button    20022
#define ID_CALLING_key9Button    20021
#define ID_CALLING_key8Button    20020
#define ID_CALLING_key7Button    20019
#define ID_CALLING_key6Button    20018
#define ID_CALLING_key5Button    20017
#define ID_CALLING_key4Button    20016
#define ID_CALLING_key3Button    20015
#define ID_CALLING_key2Button    20014
#define ID_CALLING_key1Button    20013
#define ID_CALLING_numTextView    50007
#define ID_CALLING_keyBoardWindow    110007
#define ID_CALLING_stopMicButton    20003
#define ID_CALLING_keyBoardButton    20001
#define ID_CALLING_soundButton    20005
#define ID_CALLING_communicateWindow    110001
#define ID_CALLING_callTimeTextView    50003
#define ID_CALLING_nameTextView    50004
#define ID_CALLING_headTextView    50001
#define ID_CALLING_hangupButton    20006
/*TAG:Macro宏ID END*/

class callingActivity : public Activity, 
                     public ZKSeekBar::ISeekBarChangeListener, 
                     public ZKListView::IItemClickListener,
                     public ZKListView::AbsListAdapter,
                     public ZKSlideWindow::ISlideItemClickListener,
                     public EasyUIContext::ITouchListener,
                     public ZKEditText::ITextChangeListener,
                     public ZKVideoView::IVideoPlayerMessageListener
{
public:
    callingActivity();
    virtual ~callingActivity();

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