/***********************************************
/gen auto by zuitools
***********************************************/
#ifndef __BTMUSICACTIVITY_H__
#define __BTMUSICACTIVITY_H__


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
#define ID_BTMUSIC_TextViewBg    50001
#define ID_BTMUSIC_TextView15    50002
#define ID_BTMUSIC_btsettingButton    20007
#define ID_BTMUSIC_btcontactsButton    20006
#define ID_BTMUSIC_btrecordButton    20002
#define ID_BTMUSIC_TextView2    50007
#define ID_BTMUSIC_TextView14    50011
#define ID_BTMUSIC_btTipsWindow    110006
#define ID_BTMUSIC_TextView1    50003
#define ID_BTMUSIC_phoneButton    20008
#define ID_BTMUSIC_queryMusicButton    20010
#define ID_BTMUSIC_btAppWindow    110001
#define ID_BTMUSIC_tittleTextView    50004
#define ID_BTMUSIC_artistTextView    50005
#define ID_BTMUSIC_albumTextView    50006
#define ID_BTMUSIC_indexTextView    50008
#define ID_BTMUSIC_curposTextView    50009
#define ID_BTMUSIC_durationTextView    50010
#define ID_BTMUSIC_nextButton    20030
#define ID_BTMUSIC_prevButton    20031
#define ID_BTMUSIC_playButton    20032
#define ID_BTMUSIC_musicSeekBar    91002
#define ID_BTMUSIC_lyricTextView    50023
/*TAG:Macro宏ID END*/

class btMusicActivity : public Activity, 
                     public ZKSeekBar::ISeekBarChangeListener, 
                     public ZKListView::IItemClickListener,
                     public ZKListView::AbsListAdapter,
                     public ZKSlideWindow::ISlideItemClickListener,
                     public EasyUIContext::ITouchListener,
                     public ZKEditText::ITextChangeListener,
                     public ZKVideoView::IVideoPlayerMessageListener
{
public:
    btMusicActivity();
    virtual ~btMusicActivity();

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