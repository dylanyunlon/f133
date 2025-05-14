/***********************************************
/gen auto by zuitools
***********************************************/
#ifndef __MUSICACTIVITY_H__
#define __MUSICACTIVITY_H__


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
#define ID_MUSIC_errorTextView    50014
#define ID_MUSIC_errorWindow    110002
#define ID_MUSIC_PlayMode    20011
#define ID_MUSIC_TextView    50013
#define ID_MUSIC_picTextView    50011
#define ID_MUSIC_albumTextView    50008
#define ID_MUSIC_artistTextView    50005
#define ID_MUSIC_NextButton    20010
#define ID_MUSIC_ButtonPlay    20009
#define ID_MUSIC_PrevButton    20006
#define ID_MUSIC_playModeButton    20008
#define ID_MUSIC_musicListButton    20007
#define ID_MUSIC_DurationTextView    50007
#define ID_MUSIC_PlayProgressSeekbar    91002
#define ID_MUSIC_CurPosTextView    50001
#define ID_MUSIC_titleTextView    50003
#define ID_MUSIC_musicCloseButton    20005
#define ID_MUSIC_musicWindow    110001
#define ID_MUSIC_syncPointer    90001
#define ID_MUSIC_scaningTextView    50012
#define ID_MUSIC_scaningWindow    110003
#define ID_MUSIC_musicButton    20001
#define ID_MUSIC_SubItemIcon    24002
#define ID_MUSIC_SubItemID    24001
#define ID_MUSIC_musicListView    80003
#define ID_MUSIC_USB2Button    20004
#define ID_MUSIC_USB1Button    20003
#define ID_MUSIC_SDButton    20002
#define ID_MUSIC_sys_back   100
/*TAG:Macro宏ID END*/

class musicActivity : public Activity, 
                     public ZKSeekBar::ISeekBarChangeListener, 
                     public ZKListView::IItemClickListener,
                     public ZKListView::AbsListAdapter,
                     public ZKSlideWindow::ISlideItemClickListener,
                     public EasyUIContext::ITouchListener,
                     public ZKEditText::ITextChangeListener,
                     public ZKVideoView::IVideoPlayerMessageListener
{
public:
    musicActivity();
    virtual ~musicActivity();

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