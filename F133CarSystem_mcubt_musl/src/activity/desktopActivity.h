/***********************************************
/gen auto by zuitools
***********************************************/
#ifndef __DESKTOPACTIVITY_H__
#define __DESKTOPACTIVITY_H__


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
#include "control/ZKRadioGroup.h"
#include "window/ZKSlideWindow.h"

/*TAG:Macro宏ID*/
#define ID_DESKTOP_appSlideWindow    30001
#define ID_DESKTOP_TextView3    50006
#define ID_DESKTOP_Window1    110005
#define ID_DESKTOP_btTipsWindow    110004
#define ID_DESKTOP_TextView6    50013
#define ID_DESKTOP_Window5    110011
#define ID_DESKTOP_linkTipsWindow    110010
#define ID_DESKTOP_DigitalClock2    93002
#define ID_DESKTOP_soundButton    20034
#define ID_DESKTOP_NetButton    20033
#define ID_DESKTOP_btStatusButton    20032
#define ID_DESKTOP_usbButton    20031
#define ID_DESKTOP_sdButton    20030
#define ID_DESKTOP_linkButton    20029
#define ID_DESKTOP_fmButton    20007
#define ID_DESKTOP_auxButton    20022
#define ID_DESKTOP_headWindow    110002
#define ID_DESKTOP_Button2    20002
#define ID_DESKTOP_verTextView    50005
#define ID_DESKTOP_Button1    20001
#define ID_DESKTOP_PlayVolSeekBar    91001
#define ID_DESKTOP_Setting    20017
#define ID_DESKTOP_buttomWindow    110013
#define ID_DESKTOP_ToMusic    20021
#define ID_DESKTOP_musicPicTextView    50004
#define ID_DESKTOP_PlayProgressSeekbar    91002
#define ID_DESKTOP_PrevButton    20023
#define ID_DESKTOP_ButtonPlay    20026
#define ID_DESKTOP_NextButton    20027
#define ID_DESKTOP_artistTextView    50015
#define ID_DESKTOP_titleTextView    50001
#define ID_DESKTOP_musicWindow    110012
#define ID_DESKTOP_RadioButton1    22002
#define ID_DESKTOP_RadioButton0    22001
#define ID_DESKTOP_StatusRadioGroup    94001
#define ID_DESKTOP_TextViewBg    50007
/*TAG:Macro宏ID END*/

class desktopActivity : public Activity, 
                     public ZKSeekBar::ISeekBarChangeListener, 
                     public ZKListView::IItemClickListener,
                     public ZKListView::AbsListAdapter,
                     public ZKSlideWindow::ISlideItemClickListener,
                     public EasyUIContext::ITouchListener,
                     public ZKRadioGroup::ICheckedChangeListener,
                     public ZKEditText::ITextChangeListener,
                     public ZKVideoView::IVideoPlayerMessageListener
{
public:
    desktopActivity();
    virtual ~desktopActivity();

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
    virtual void onCheckedChanged(ZKRadioGroup* pRadioGroup, int checkedID);

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