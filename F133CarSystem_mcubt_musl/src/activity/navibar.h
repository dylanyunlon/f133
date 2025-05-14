#ifndef _SYSAPP_NAVIBAR_H_
#define _SYSAPP_NAVIBAR_H_

#include "entry/EasyUIContext.h"
#include "app/SysAppFactory.h"

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
#define ID_NAVIBAR_btButton    20007
#define ID_NAVIBAR_settingButton    20005
#define ID_NAVIBAR_lightTextView    50003
#define ID_NAVIBAR_volumeTextView    50002
#define ID_NAVIBAR_lightNumTextView    50001
#define ID_NAVIBAR_voiceNumTextView    50011
#define ID_NAVIBAR_SoundButton    20009
#define ID_NAVIBAR_screenoffBtn    20008
#define ID_NAVIBAR_TextView7    50010
#define ID_NAVIBAR_MeridiemTextView    50012
#define ID_NAVIBAR_weekTextView    50009
#define ID_NAVIBAR_dataTextView    50008
#define ID_NAVIBAR_btTextView    50005
#define ID_NAVIBAR_fmTextView    50004
#define ID_NAVIBAR_fmButton    20006
#define ID_NAVIBAR_TextviewDDR    50007
#define ID_NAVIBAR_TextView5    50006
#define ID_NAVIBAR_soundButton    20004
#define ID_NAVIBAR_briButton    20003
#define ID_NAVIBAR_PlayVolSeekBar    91001
#define ID_NAVIBAR_briSeekBar    91002
#define ID_NAVIBAR_buttomWindow    110001
/*TAG:Macro宏ID END*/
class navibar : public BaseApp,
                  public ZKSeekBar::ISeekBarChangeListener, 
                  public ZKListView::IItemClickListener,
                  public ZKListView::AbsListAdapter,
                  public ZKSlideWindow::ISlideItemClickListener,
                  public EasyUIContext::ITouchListener,
                  public ZKEditText::ITextChangeListener,
                  public ZKVideoView::IVideoPlayerMessageListener
{
public:
	navibar();
	virtual ~navibar();
	virtual void onCreate();
	virtual void onClick(ZKBase *pBase);
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


private:
    /*TAG:PRIVATE_VARIABLE*/
    int mVideoLoopIndex;
    int mVideoLoopErrorCount;
};

#endif