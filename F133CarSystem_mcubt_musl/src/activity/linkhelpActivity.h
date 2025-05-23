/***********************************************
/gen auto by zuitools
***********************************************/
#ifndef __LINKHELPACTIVITY_H__
#define __LINKHELPACTIVITY_H__


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
#define ID_LINKHELP_TextViewBg    50010
#define ID_LINKHELP_aaTipTextView    50008
#define ID_LINKHELP_apTipTextView    50006
#define ID_LINKHELP_cpTipTextView    50005
#define ID_LINKHELP_mcTipTextView    50004
#define ID_LINKHELP_TextView2    50033
#define ID_LINKHELP_acTipTextView    50034
#define ID_LINKHELP_clStepTextView    50032
#define ID_LINKHELP_acWindow    110006
#define ID_LINKHELP_acStepTextView    50049
#define ID_LINKHELP_aaStepTextView    50043
#define ID_LINKHELP_apStepTextView    50042
#define ID_LINKHELP_cpStepTextView    50041
#define ID_LINKHELP_mcStepTextView    50040
#define ID_LINKHELP_TextView1    50031
#define ID_LINKHELP_cf4TextView    50029
#define ID_LINKHELP_cf3TextView    50025
#define ID_LINKHELP_cf2TextView    50026
#define ID_LINKHELP_cf1TextView    50028
#define ID_LINKHELP_cfTextView    50009
#define ID_LINKHELP_cfWindow    110001
#define ID_LINKHELP_aa6TextView    50027
#define ID_LINKHELP_aa5TextView    50018
#define ID_LINKHELP_aa4TextView    50017
#define ID_LINKHELP_aa3TextView    50024
#define ID_LINKHELP_aa2TextView    50021
#define ID_LINKHELP_aa1TextView    50016
#define ID_LINKHELP_aaTextView    50019
#define ID_LINKHELP_aaWindow    110004
#define ID_LINKHELP_ap5TextView    50014
#define ID_LINKHELP_ap4TextView    50012
#define ID_LINKHELP_ap3TextView    50023
#define ID_LINKHELP_ap2TextView    50022
#define ID_LINKHELP_ap1TextView    50011
#define ID_LINKHELP_apTextView    50020
#define ID_LINKHELP_apWindow    110005
#define ID_LINKHELP_cp6TextView    50030
#define ID_LINKHELP_cp5TextView    50001
#define ID_LINKHELP_cp4TextView    50003
#define ID_LINKHELP_cp3TextView    50013
#define ID_LINKHELP_cp2TextView    50002
#define ID_LINKHELP_cp1TextView    50007
#define ID_LINKHELP_cpTextView    50015
#define ID_LINKHELP_cpWindow    110002
#define ID_LINKHELP_mcWindow    110007
/*TAG:Macro宏ID END*/

class linkhelpActivity : public Activity, 
                     public ZKSeekBar::ISeekBarChangeListener, 
                     public ZKListView::IItemClickListener,
                     public ZKListView::AbsListAdapter,
                     public ZKSlideWindow::ISlideItemClickListener,
                     public EasyUIContext::ITouchListener,
                     public ZKEditText::ITextChangeListener,
                     public ZKVideoView::IVideoPlayerMessageListener
{
public:
    linkhelpActivity();
    virtual ~linkhelpActivity();

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