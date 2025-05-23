/***********************************************
/gen auto by zuitools
***********************************************/
#ifndef __SETFACTORYACTIVITY_H__
#define __SETFACTORYACTIVITY_H__


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
#define ID_SETFACTORY_TextView2    50003
#define ID_SETFACTORY_mcuUpdSeekBar    91004
#define ID_SETFACTORY_TextView37    50050
#define ID_SETFACTORY_TextView38    50051
#define ID_SETFACTORY_mcuWnd    110020
#define ID_SETFACTORY_TextView1    50002
#define ID_SETFACTORY_mcuButton    20010
#define ID_SETFACTORY_TextView18    50021
#define ID_SETFACTORY_TextView12    50017
#define ID_SETFACTORY_mcuInfoWindow    110018
#define ID_SETFACTORY_delButton    20041
#define ID_SETFACTORY_EnterButton    20034
#define ID_SETFACTORY_key0Button    20042
#define ID_SETFACTORY_key9Button    20030
#define ID_SETFACTORY_key7Button    20026
#define ID_SETFACTORY_key8Button    20039
#define ID_SETFACTORY_key6Button    20033
#define ID_SETFACTORY_key5Button    20038
#define ID_SETFACTORY_key4Button    20032
#define ID_SETFACTORY_key3Button    20027
#define ID_SETFACTORY_key2Button    20035
#define ID_SETFACTORY_key1Button    20040
#define ID_SETFACTORY_numTextView    50025
#define ID_SETFACTORY_passwordWindow    110021
#define ID_SETFACTORY_factoryWnd    110015
#define ID_SETFACTORY_systemButton    20006
#define ID_SETFACTORY_factoryButton    20005
#define ID_SETFACTORY_reverseButton    20004
#define ID_SETFACTORY_voiceButton    20003
#define ID_SETFACTORY_linkButton    20002
#define ID_SETFACTORY_showButton    20001
#define ID_SETFACTORY_setWnd    110001
#define ID_SETFACTORY_TextViewBg    50001
/*TAG:Macro宏ID END*/

class setfactoryActivity : public Activity, 
                     public ZKSeekBar::ISeekBarChangeListener, 
                     public ZKListView::IItemClickListener,
                     public ZKListView::AbsListAdapter,
                     public ZKSlideWindow::ISlideItemClickListener,
                     public EasyUIContext::ITouchListener,
                     public ZKEditText::ITextChangeListener,
                     public ZKVideoView::IVideoPlayerMessageListener
{
public:
    setfactoryActivity();
    virtual ~setfactoryActivity();

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