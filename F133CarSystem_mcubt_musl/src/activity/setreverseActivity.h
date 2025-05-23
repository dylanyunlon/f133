/***********************************************
/gen auto by zuitools
***********************************************/
#ifndef __SETREVERSEACTIVITY_H__
#define __SETREVERSEACTIVITY_H__


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
#define ID_SETREVERSE_CutcvbsButton    20011
#define ID_SETREVERSE_cvbsButton    20013
#define ID_SETREVERSE_TextView1    50020
#define ID_SETREVERSE_CutCameraBtn    20007
#define ID_SETREVERSE_TextView2    50018
#define ID_SETREVERSE_SubItem10    24026
#define ID_SETREVERSE_ChannelListView    80007
#define ID_SETREVERSE_CameraWnd    110010
#define ID_SETREVERSE_lineButton    20022
#define ID_SETREVERSE_TextView36    50046
#define ID_SETREVERSE_TextView43    50059
#define ID_SETREVERSE_TextView42    50053
#define ID_SETREVERSE_TextView39    50052
#define ID_SETREVERSE_rotButton    20023
#define ID_SETREVERSE_skipTextView    50045
#define ID_SETREVERSE_skipaddButton    20021
#define ID_SETREVERSE_skipreduceButton    20012
#define ID_SETREVERSE_TextView28    50037
#define ID_SETREVERSE_TextView27    50036
#define ID_SETREVERSE_formatTextView    50002
#define ID_SETREVERSE_SubItem9    24025
#define ID_SETREVERSE_caminfoListView    80006
#define ID_SETREVERSE_CameraWindow    110023
#define ID_SETREVERSE_reverseWnd    110004
#define ID_SETREVERSE_systemButton    20006
#define ID_SETREVERSE_factoryButton    20005
#define ID_SETREVERSE_reverseButton    20004
#define ID_SETREVERSE_voiceButton    20003
#define ID_SETREVERSE_linkButton    20002
#define ID_SETREVERSE_showButton    20001
#define ID_SETREVERSE_setWnd    110001
#define ID_SETREVERSE_TextViewBg    50001
/*TAG:Macro宏ID END*/

class setreverseActivity : public Activity, 
                     public ZKSeekBar::ISeekBarChangeListener, 
                     public ZKListView::IItemClickListener,
                     public ZKListView::AbsListAdapter,
                     public ZKSlideWindow::ISlideItemClickListener,
                     public EasyUIContext::ITouchListener,
                     public ZKEditText::ITextChangeListener,
                     public ZKVideoView::IVideoPlayerMessageListener
{
public:
    setreverseActivity();
    virtual ~setreverseActivity();

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