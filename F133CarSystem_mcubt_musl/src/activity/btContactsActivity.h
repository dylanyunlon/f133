/***********************************************
/gen auto by zuitools
***********************************************/
#ifndef __BTCONTACTSACTIVITY_H__
#define __BTCONTACTSACTIVITY_H__


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
#define ID_BTCONTACTS_TipsTextView    50011
#define ID_BTCONTACTS_DeleteTipsWindow    110010
#define ID_BTCONTACTS_sureButton    20004
#define ID_BTCONTACTS_cancelButton    20003
#define ID_BTCONTACTS_numberTextView    50005
#define ID_BTCONTACTS_TextView15    50002
#define ID_BTCONTACTS_btsettingButton    20007
#define ID_BTCONTACTS_btcontactsButton    20006
#define ID_BTCONTACTS_btrecordButton    20002
#define ID_BTCONTACTS_phoneButton    20008
#define ID_BTCONTACTS_queryMusicButton    20010
#define ID_BTCONTACTS_btAppWindow    110001
#define ID_BTCONTACTS_deleteButton    20009
#define ID_BTCONTACTS_SubItem1    24001
#define ID_BTCONTACTS_syncPointer    90001
#define ID_BTCONTACTS_scaningTextView    50001
#define ID_BTCONTACTS_scaningWindow    110003
#define ID_BTCONTACTS_charSeekBar    91001
#define ID_BTCONTACTS_downloadButton    20001
#define ID_BTCONTACTS_cIdSubItem    24002
#define ID_BTCONTACTS_contactsListView    80003
#define ID_BTCONTACTS_charJump    80006
#define ID_BTCONTACTS_indexTip    50012
/*TAG:Macro宏ID END*/

class btContactsActivity : public Activity, 
                     public ZKSeekBar::ISeekBarChangeListener, 
                     public ZKListView::IItemClickListener,
                     public ZKListView::AbsListAdapter,
                     public ZKSlideWindow::ISlideItemClickListener,
                     public EasyUIContext::ITouchListener,
                     public ZKEditText::ITextChangeListener,
                     public ZKVideoView::IVideoPlayerMessageListener
{
public:
    btContactsActivity();
    virtual ~btContactsActivity();

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