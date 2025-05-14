/***********************************************
/gen auto by zuitools
***********************************************/
#ifndef __PHOTOALBUMACTIVITY_H__
#define __PHOTOALBUMACTIVITY_H__


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
#define ID_PHOTOALBUM_TextView6    50010
#define ID_PHOTOALBUM_TextView4    50009
#define ID_PHOTOALBUM_TextView1    50007
#define ID_PHOTOALBUM_TextView3    50006
#define ID_PHOTOALBUM_TextView2    50005
#define ID_PHOTOALBUM_SubItem2    24003
#define ID_PHOTOALBUM_TextView5    50008
#define ID_PHOTOALBUM_back    20012
#define ID_PHOTOALBUM_syncPointer    90001
#define ID_PHOTOALBUM_scaningTextView    50012
#define ID_PHOTOALBUM_Notice    110006
#define ID_PHOTOALBUM_SubItem1    24001
#define ID_PHOTOALBUM_name    24002
#define ID_PHOTOALBUM_ImagesListView    80001
#define ID_PHOTOALBUM_USB2    20005
#define ID_PHOTOALBUM_USB1    20004
#define ID_PHOTOALBUM_SD    20003
#define ID_PHOTOALBUM_AlbumClassification    110001
#define ID_PHOTOALBUM_Index    50003
#define ID_PHOTOALBUM_rotateBtn    20013
#define ID_PHOTOALBUM_reduceBtn    20010
#define ID_PHOTOALBUM_amplifyBtn    20011
#define ID_PHOTOALBUM_CarouselPic    20009
#define ID_PHOTOALBUM_Prev    20001
#define ID_PHOTOALBUM_Next    20002
#define ID_PHOTOALBUM_IndexBt3    20008
#define ID_PHOTOALBUM_IndexPic3    50004
#define ID_PHOTOALBUM_IndexWnd3    110005
#define ID_PHOTOALBUM_IndexBt2    20007
#define ID_PHOTOALBUM_IndexPic2    50002
#define ID_PHOTOALBUM_IndexWnd2    110004
#define ID_PHOTOALBUM_IndexBt1    20006
#define ID_PHOTOALBUM_IndexPic1    50001
#define ID_PHOTOALBUM_IndexWnd1    110003
#define ID_PHOTOALBUM_Frame    110002
/*TAG:Macro宏ID END*/

class PhotoAlbumActivity : public Activity, 
                     public ZKSeekBar::ISeekBarChangeListener, 
                     public ZKListView::IItemClickListener,
                     public ZKListView::AbsListAdapter,
                     public ZKSlideWindow::ISlideItemClickListener,
                     public EasyUIContext::ITouchListener,
                     public ZKEditText::ITextChangeListener,
                     public ZKVideoView::IVideoPlayerMessageListener
{
public:
    PhotoAlbumActivity();
    virtual ~PhotoAlbumActivity();

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