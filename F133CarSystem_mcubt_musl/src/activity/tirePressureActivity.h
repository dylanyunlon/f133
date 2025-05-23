/***********************************************
/gen auto by zuitools
***********************************************/
#ifndef __TIREPRESSUREACTIVITY_H__
#define __TIREPRESSUREACTIVITY_H__


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
#define ID_TIREPRESSURE_sys_back   100
#define ID_TIREPRESSURE_saveOptionButton    20014
#define ID_TIREPRESSURE_saveDataButton    20013
#define ID_TIREPRESSURE_tireMatchListView    80001
#define ID_TIREPRESSURE_rearButton    20012
#define ID_TIREPRESSURE_frontButton    20011
#define ID_TIREPRESSURE_resetButton    20010
#define ID_TIREPRESSURE_CnextButton    20009
#define ID_TIREPRESSURE_CprevButton    20008
#define ID_TIREPRESSURE_CTextView    50020
#define ID_TIREPRESSURE_TextView7    50019
#define ID_TIREPRESSURE_lvnextButton    20007
#define ID_TIREPRESSURE_lvprevButton    20006
#define ID_TIREPRESSURE_lvTextView    50018
#define ID_TIREPRESSURE_TextView8    50017
#define ID_TIREPRESSURE_hvnextButton    20005
#define ID_TIREPRESSURE_hvprevButton    20004
#define ID_TIREPRESSURE_hvTextView    50016
#define ID_TIREPRESSURE_TextView6    50015
#define ID_TIREPRESSURE_unitnextButton    20003
#define ID_TIREPRESSURE_unitprevButton    20002
#define ID_TIREPRESSURE_unitTextView    50014
#define ID_TIREPRESSURE_TextView5    50013
#define ID_TIREPRESSURE_TextView4    50012
#define ID_TIREPRESSURE_TextView3    50011
#define ID_TIREPRESSURE_TextView2    50010
#define ID_TIREPRESSURE_clearButton    20001
#define ID_TIREPRESSURE_rearstateTextView    50009
#define ID_TIREPRESSURE_frontstateTextView    50008
#define ID_TIREPRESSURE_rearCTextView    50007
#define ID_TIREPRESSURE_frontCView    50006
#define ID_TIREPRESSURE_rearpsiTextView    50005
#define ID_TIREPRESSURE_frontpsiTextView    50004
#define ID_TIREPRESSURE_rearTextView    50003
#define ID_TIREPRESSURE_frontTextView    50002
#define ID_TIREPRESSURE_TextView1    50001
/*TAG:Macro宏ID END*/

class tirePressureActivity : public Activity, 
                     public ZKSeekBar::ISeekBarChangeListener, 
                     public ZKListView::IItemClickListener,
                     public ZKListView::AbsListAdapter,
                     public ZKSlideWindow::ISlideItemClickListener,
                     public EasyUIContext::ITouchListener,
                     public ZKEditText::ITextChangeListener,
                     public ZKVideoView::IVideoPlayerMessageListener
{
public:
    tirePressureActivity();
    virtual ~tirePressureActivity();

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