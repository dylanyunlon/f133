/***********************************************
/gen auto by zuitools
***********************************************/
#ifndef __SOUNDEFFECTACTIVITY_H__
#define __SOUNDEFFECTACTIVITY_H__


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
#define ID_SOUNDEFFECT_TextView13    50013
#define ID_SOUNDEFFECT_TextView12    50012
#define ID_SOUNDEFFECT_TextView11    50011
#define ID_SOUNDEFFECT_TextView10    50010
#define ID_SOUNDEFFECT_TextView9    50009
#define ID_SOUNDEFFECT_TextView8    50008
#define ID_SOUNDEFFECT_TextView7    50007
#define ID_SOUNDEFFECT_TextView6    50006
#define ID_SOUNDEFFECT_TextView5    50005
#define ID_SOUNDEFFECT_TextView4    50004
#define ID_SOUNDEFFECT_sys_back   100
#define ID_SOUNDEFFECT_TextView3    50003
#define ID_SOUNDEFFECT_TextView2    50002
#define ID_SOUNDEFFECT_TextView1    50001
#define ID_SOUNDEFFECT_eqSeekBar9    91009
#define ID_SOUNDEFFECT_eqSeekBar8    91008
#define ID_SOUNDEFFECT_eqSeekBar7    91007
#define ID_SOUNDEFFECT_eqSeekBar6    91006
#define ID_SOUNDEFFECT_eqSeekBar5    91005
#define ID_SOUNDEFFECT_eqSeekBar4    91004
#define ID_SOUNDEFFECT_eqSeekBar3    91003
#define ID_SOUNDEFFECT_eqSeekBar2    91002
#define ID_SOUNDEFFECT_eqSeekBar1    91001
#define ID_SOUNDEFFECT_eqSeekBar0    91010
/*TAG:Macro宏ID END*/

class soundEffectActivity : public Activity, 
                     public ZKSeekBar::ISeekBarChangeListener, 
                     public ZKListView::IItemClickListener,
                     public ZKListView::AbsListAdapter,
                     public ZKSlideWindow::ISlideItemClickListener,
                     public EasyUIContext::ITouchListener,
                     public ZKEditText::ITextChangeListener,
                     public ZKVideoView::IVideoPlayerMessageListener
{
public:
    soundEffectActivity();
    virtual ~soundEffectActivity();

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