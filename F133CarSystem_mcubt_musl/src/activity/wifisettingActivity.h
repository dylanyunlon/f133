/***********************************************
/gen auto by zuitools
***********************************************/
#ifndef __WIFISETTINGACTIVITY_H__
#define __WIFISETTINGACTIVITY_H__


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
#define ID_WIFISETTING_TextMacAddr    50006
#define ID_WIFISETTING_TextIPAddr    50004
#define ID_WIFISETTING_WindowMenuMore    110001
#define ID_WIFISETTING_WindowPasswordError    100012
#define ID_WIFISETTING_TextConnectState    50032
#define ID_WIFISETTING_TextConnectSecType    50031
#define ID_WIFISETTING_TextConnectSsid    50030
#define ID_WIFISETTING_ButtonDisconnect    20010
#define ID_WIFISETTING_WindowDisconnect    100011
#define ID_WIFISETTING_ButtonShowPwd    20001
#define ID_WIFISETTING_TextSecType    50025
#define ID_WIFISETTING_TextSsid    50024
#define ID_WIFISETTING_EdittextPwd    51000
#define ID_WIFISETTING_ButtonConnect    20009
#define ID_WIFISETTING_WindowSet    100010
#define ID_WIFISETTING_ButtonMenu    20002
#define ID_WIFISETTING_ButtonOnOff    20005
#define ID_WIFISETTING_ListViewWifiInfo    80000
#define ID_WIFISETTING_Textview6    50003
#define ID_WIFISETTING_Textview2    50001
#define ID_WIFISETTING_Textview27    50038
#define ID_WIFISETTING_Textview19    50029
#define ID_WIFISETTING_Textview17    50028
#define ID_WIFISETTING_Textview15    50027
#define ID_WIFISETTING_Textview9    50023
#define ID_WIFISETTING_Textview7    50022
#define ID_WIFISETTING_Textview5    50021
#define ID_WIFISETTING_Textview8    50005
#define ID_WIFISETTING_Textview1    50019
#define ID_WIFISETTING_sys_back   100
#define ID_WIFISETTING_Window2    100006
#define ID_WIFISETTING_SubItem1    20003
#define ID_WIFISETTING_LISTSUBITEM_SUB    70002
#define ID_WIFISETTING_LISTSUBITEM_NAME    70001
#define ID_WIFISETTING_LISTSUBITEM_LEVEL    70000
#define ID_WIFISETTING_Window1    100004
/*TAG:Macro宏ID END*/

class wifisettingActivity : public Activity, 
                     public ZKSeekBar::ISeekBarChangeListener, 
                     public ZKListView::IItemClickListener,
                     public ZKListView::AbsListAdapter,
                     public ZKSlideWindow::ISlideItemClickListener,
                     public EasyUIContext::ITouchListener,
                     public ZKEditText::ITextChangeListener,
                     public ZKVideoView::IVideoPlayerMessageListener
{
public:
    wifisettingActivity();
    virtual ~wifisettingActivity();

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
    void videoLoopPlayback(ZKVideoView *pVideoView, int msg, int callbackTabIndex);
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
