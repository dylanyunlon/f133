/***********************************************
/gen auto by zuitools
***********************************************/
#ifndef __VIDEOACTIVITY_H__
#define __VIDEOACTIVITY_H__


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
#define ID_VIDEO_TipTextView    50004
#define ID_VIDEO_TipWindow    110007
#define ID_VIDEO_errorTextView    50014
#define ID_VIDEO_errorWindow    110005
#define ID_VIDEO_NextButton    20010
#define ID_VIDEO_PlayButton    20009
#define ID_VIDEO_PrevButton    20005
#define ID_VIDEO_playModeButton    20008
#define ID_VIDEO_videoListButton    20007
#define ID_VIDEO_DurationTextView    50007
#define ID_VIDEO_PlayProgressSeekbar    91001
#define ID_VIDEO_CurPosTextView    50002
#define ID_VIDEO_backButton    20001
#define ID_VIDEO_hideButton    20006
#define ID_VIDEO_bagTextView    50001
#define ID_VIDEO_tittleTextView    50003
#define ID_VIDEO_VideoCtrlWindow    110004
#define ID_VIDEO_VideoCtrlButton    20012
#define ID_VIDEO_WindowMask    110006
#define ID_VIDEO_VideoviewTT    95001
#define ID_VIDEO_videoPlayWindow    110002
#define ID_VIDEO_syncPointer    90001
#define ID_VIDEO_scaningTextView    50012
#define ID_VIDEO_scaningWindow    110003
#define ID_VIDEO_sys_back   100
#define ID_VIDEO_USB2Button    20004
#define ID_VIDEO_SubItemIcon    24002
#define ID_VIDEO_SubItemID    24001
#define ID_VIDEO_videoListView    80003
#define ID_VIDEO_USB1Button    20003
#define ID_VIDEO_SDButton    20002
#define ID_VIDEO_videoListWindow    110001
/*TAG:Macro宏ID END*/

class videoActivity : public Activity, 
                     public ZKSeekBar::ISeekBarChangeListener, 
                     public ZKListView::IItemClickListener,
                     public ZKListView::AbsListAdapter,
                     public ZKSlideWindow::ISlideItemClickListener,
                     public EasyUIContext::ITouchListener,
                     public ZKEditText::ITextChangeListener,
                     public ZKVideoView::IVideoPlayerMessageListener
{
public:
    videoActivity();
    virtual ~videoActivity();

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