#pragma once
#include <sys/types.h>
#include <iostream>
#include "uart/ProtocolSender.h"
#include "utils/SlidingManager.h"
#include "utils/BitmapHelper.h"
#include "manager/ConfigManager.h"
#include <os/MountMonitor.h>
#include <sys/sysinfo.h>
#include <system/Thread.h>
#include "utils/imageinfo.hpp"
#include "utils/Screenhelper.h"
#include "media/media_context.h"
#include "fy/os.hpp"
#include "fy/files.hpp"
#include "sysapp_context.h"

#define MAX_PATH_LEN  					256										//相片路径最大长度
#define STORAGE_TYPE_SIZE 				3										//外置存储类型数
#define SCREENRESOULUTION_WIDTH  		ScreenHelper::getScreenWidth()			//屏幕分辨率width
#define SCREENRESOULUTION_HEIGHT  		ScreenHelper::getScreenHeight()			//屏幕分辨率height
#define IMAGES_WIDTH_MAX				2048									//图片宽度限制
#define IMAGES_HEIGHT_MAX				2048									//图片高度限制
#define CAROUSELPICTIMER				1										//相册轮播定时器
#define REFRESHALUMTIMER				3										//刷新相册定时器
#define SLIDESWITCHENABLE				0										//触摸左右滑动切换上下图片使能 0：失能 1：使能
static int 	s_multiple = 1;
#define 	MAXMUL	5

#define 	LEFT_POS		100//每放大缩小一次横纵坐标的变换
#define		TOP_POS		    200//每放大缩小一次相片宽度的变换

/**
 * @图片信息结构体
 */
typedef struct{
	int mLeft;					//左边距
	int mTop;					//上边距
	int width;					//图片宽
	int height;					//图片高
	std::string path;			//图片路径
	bool Displayable = false;	//图片是否可显示 分辨率不得大于2048*2048
}Photos;

static bool SlideEnable = true;								//在刷新相册、导航栏下拉等情况下失能
static unsigned int IndexPic = 0;							//相册中间图片下标
static bool IsSlideLeftDone = false;						//左滑完成标志
static class slideWnd PAsw;									//滑动类
static storage_type_e albumtype = E_STORAGE_TYPE_SD;		//相册存储类型
static bool is_sel;

/**
 * @等比例缩小
 */
static void EqualProportionZoomOut(int *width, int *height){
	float w=*width,h=*height;
	if(w>SCREENRESOULUTION_WIDTH){
		h = h - ((w-SCREENRESOULUTION_WIDTH)/w)*h;
		w = SCREENRESOULUTION_WIDTH;
	}
	if(h>SCREENRESOULUTION_HEIGHT){
		w = w - ((h-SCREENRESOULUTION_HEIGHT)/h)*w;
		h = SCREENRESOULUTION_HEIGHT;
	}
	*width = w;
	*height = h;
}
/**
 * @控件水平垂直居中
 */
static void Horizontal_Vertical_Center_Pic(ZKTextView *zkbase,Photos pts){//const char *pPicPath
	fy::drop_caches();
	if(pts.Displayable){
		zkbase->setText("");
		zkbase->setBackgroundPic(pts.path.c_str());
		zkbase->setPosition(LayoutPosition(pts.mLeft,pts.mTop,pts.width,pts.height));
	}else{
		zkbase->setBackgroundPic("");
		zkbase->setPosition(LayoutPosition(0,0,SCREENRESOULUTION_WIDTH,SCREENRESOULUTION_HEIGHT));
		zkbase->setTextTr("This image is not supported for display!");
	}
}

/**
 * @滑动结束监听
 */
static void SlideListener(SLIDEDONETYPE SDT){
	if(media::get_image_list_size(albumtype) <= 3){
		char indextext[24] = {0};
		sprintf(indextext,"%d/%d",PAsw.Get_Curr_Wnd_Index()+1,(int)media::get_image_list_size(albumtype));
		mIndexPtr->setText(indextext);
		return;
	}
	if(SDT == SLIDEDONETYPE_LEFT){//下一张
		IndexPic += (IndexPic < (media::get_image_list_size(albumtype)-1))?1:(1-media::get_image_list_size(albumtype));
		IsSlideLeftDone = true;
		mActivityPtr->registerUserTimer(REFRESHALUMTIMER,20);
	}else if(SDT == SLIDEDONETYPE_RIGHT){//上一张
		IndexPic -= (IndexPic > 0)?1:(1 - media::get_image_list_size(albumtype));
		IsSlideLeftDone = false;
		mActivityPtr->registerUserTimer(REFRESHALUMTIMER,20);
	}
	char indextext[24] = {0};
	sprintf(indextext,"%d/%d",IndexPic+1,(int)media::get_image_list_size(albumtype));
	mIndexPtr->setText(indextext);
}

/**
 * @获取当前空闲内存
 */
static long GetMemFree(){
	struct sysinfo info;
	sysinfo(&info);
	long freeram =  info.freeram;
	return freeram/1024;
//    long long pages = sysconf(_SC_AVPHYS_PAGES);
//    long long page_size = sysconf(_SC_PAGE_SIZE) / 1024.0;// 以KB为单位
//    return pages * page_size;
}

/**
 * @控件是否显示取反
 */
//static void SetDisplayOpposite(){
//	mPrevPtr->setVisible(!mPrevPtr->isVisible());
//	mNextPtr->setVisible(!mNextPtr->isVisible());
//	mAlbumClassificationPtr->setVisible(!mAlbumClassificationPtr->isVisible());
//}
static void SetDisplayOpposite(bool isVisible){
	mIndexPtr->setVisible(isVisible);
	mPrevPtr->setVisible(isVisible);
	mNextPtr->setVisible(isVisible);
	mCarouselPicPtr->setVisible(isVisible);
	mamplifyBtnPtr->setVisible(isVisible);
	mreduceBtnPtr->setVisible(isVisible);
	mTextView5Ptr->setVisible(isVisible);
	mbackPtr->setVisible(isVisible);
}

/**
 * @获取图片宽高
 */
static int GetImageSize(std::string &path,int &w,int &h){
	auto imageInfo = getImageInfo<IIFilePathReader>(path);
	if(imageInfo.getErrorCode() != II_ERR_OK){
		LOGD("getImageInfo Failed file : %s\nErrorCode : %s",path.c_str(),imageInfo.getErrorMsg());
		return -2;
	}
	w = (int)imageInfo.getWidth();
	h = (int)imageInfo.getHeight();
	return 1;
}

/**
 * @图片是否合格筛查
 * @ret:
 */
static Photos ImagesFilter(std::string path) {
	Photos pts;
	pts.path = path;
	int ret = GetImageSize(pts.path, pts.width, pts.height);
	LOGD("Image : %s\nwidth : %d\nheight : %d", pts.path.c_str(),pts.width,pts.height);
	if(ret == -2){
		pts.Displayable = false;
		return pts;
	}
	int MemFree = GetMemFree();
	LOGD("MemFree : %d MemPicture : %d",MemFree,(pts.width*pts.height*4)/1024);
	if(((MemFree-(pts.width*pts.height*4)/1024) > 4096) && ret &&
			pts.width < IMAGES_WIDTH_MAX && pts.height < IMAGES_WIDTH_MAX){
		EqualProportionZoomOut(&pts.width, &pts.height);
		pts.mLeft = (SCREENRESOULUTION_WIDTH-pts.width)/2;
		pts.mTop = (SCREENRESOULUTION_HEIGHT-pts.height)/2;
		pts.Displayable = true;
		return pts;
	}else{
		LOGD("%s Picture resolution is too high : Out of memory",pts.path.c_str());
		if(!(pts.width < IMAGES_WIDTH_MAX && pts.height < IMAGES_WIDTH_MAX) &&
				(MemFree-(pts.width*pts.height*4)/1024) > 4096){
			LOGD("image's width or image's height is too large : width : %d height : %d",
				pts.width,pts.height);
			EqualProportionZoomOut(&pts.width, &pts.height);
			pts.mLeft = (SCREENRESOULUTION_WIDTH-pts.width)/2;
			pts.mTop = (SCREENRESOULUTION_HEIGHT-pts.height)/2;
			pts.Displayable = false;
			return pts;
		}
	}
	return pts;
}

static void UpdateStatus(){
	mImagesListViewPtr->setSelection(0);
	mSDPtr->setSelected(albumtype==E_STORAGE_TYPE_SD);
	mUSB1Ptr->setSelected(albumtype==E_STORAGE_TYPE_USB1);
	mUSB2Ptr->setSelected(albumtype==E_STORAGE_TYPE_USB2);
}

/**
 * @选择相册
 */
static void SelectAlbum(){
	LOGD("Debug SelectAlbum Start");
	PAsw.SetIsInitDone(false);
	UpdateStatus();
	vector<ZKWindow *> wnds;
	wnds.clear();
	int size = media::get_image_list_size(albumtype);
	if(size == 0){
		PAsw.slideWnd_Init(mFramePtr,wnds);
		mIndexPtr->setText("0/0");
		mIndexPic1Ptr->setBackgroundPic("");
		mIndexPic2Ptr->setBackgroundPic("");
		mIndexPic3Ptr->setBackgroundPic("");
		return;
	}
	if(size >= 1)
		wnds.push_back(mIndexWnd1Ptr);
	if(size >= 2)
		wnds.push_back(mIndexWnd2Ptr);
	if(size >= 3)
		wnds.push_back(mIndexWnd3Ptr);
	PAsw.Set_Slide_Cyclic((size >= 2)?true:false);
	PAsw.slideWnd_Init(mFramePtr,wnds);

	IndexPic = 0;
	if(size == 1){
		Horizontal_Vertical_Center_Pic(mIndexPic1Ptr,ImagesFilter(media::get_image_file(albumtype,0)));
		mIndexPtr->setText("1/1");
	}
	if(size == 2){
		Horizontal_Vertical_Center_Pic(mIndexPic1Ptr,ImagesFilter(media::get_image_file(albumtype,IndexPic)));
		Horizontal_Vertical_Center_Pic(mIndexPic2Ptr,ImagesFilter(media::get_image_file(albumtype,IndexPic+1)));
		mIndexPtr->setText("1/2");
	}
	if(size >= 3){
		Horizontal_Vertical_Center_Pic(mIndexPic1Ptr,ImagesFilter(media::get_image_file(albumtype,IndexPic)));
		Horizontal_Vertical_Center_Pic(mIndexPic2Ptr,ImagesFilter(media::get_image_file(albumtype,IndexPic+1)));
		Horizontal_Vertical_Center_Pic(mIndexPic3Ptr,ImagesFilter(media::get_image_file(albumtype,size-1)));
		char indextext[24] = {0};
		sprintf(indextext,"1/%d",size);
		mIndexPtr->setText(indextext);
	}
	LOGD("Debug SelectAlbum End");
	return;
}

//旋转同步图标
class IconRotate : public Thread
{
	virtual bool readyToRun() {
		mNoticePtr->showWnd();
		return true;
	}

	virtual bool threadLoop(void) {
		int angle = 0;
		while(!exitPending()) {
			++angle;
			msyncPointerPtr->setTargetAngle(angle%360);
			usleep(1000*2);
		}
	    SelectAlbum();
		mNoticePtr->hideWnd();
		return false;
	}
};
static IconRotate iconRotate;

static void _media_scan_cb(const char *dir, storage_type_e type, bool started) {

	if(mCarouselPicPtr->isSelected()){//如果选中 关闭轮播
		SlideEnable = true;
		mActivityPtr->unregisterUserTimer(CAROUSELPICTIMER);
		mCarouselPicPtr->setSelected(false);
	}
	if(!mAlbumClassificationPtr->isWndShow()){
		mAlbumClassificationPtr->showWnd();
//		app::show_topbar();
	}
	if (started) {
		iconRotate.run();
	}
	else {
		if(iconRotate.isRunning()){
		    albumtype = type;
		}
		mImagesListViewPtr->refreshListView();
		iconRotate.requestExit();
	}
}
/**
 * 注册定时器
 * 填充数组用于注册定时器
 * 注意：id不能重复
 */
static S_ACTIVITY_TIMEER REGISTER_ACTIVITY_TIMER_TAB[] = {
	//{0,  6000}, //定时器id=0, 时间间隔6秒
//	{2,500},
//	{3,20}
};

/**
 * 当界面构造时触发
 */
static void onUI_init(){
    //Tips :添加 UI初始化的显示代码到这里,如:mText1Ptr->setText("123");
	s_multiple = 1;
	fy::drop_caches();
	is_sel = false;
	mIndexWnd1Ptr->showWnd();
	mIndexWnd2Ptr->hideWnd();
	mIndexWnd3Ptr->hideWnd();
	PAsw.RegisterSlideListener(&SlideListener);
	PAsw.Set_SwitchSpeed(300);
	media::add_scan_cb(_media_scan_cb);
	SelectAlbum();

	mTextView2Ptr->setTouchPass(true);
	mTextView3Ptr->setTouchPass(true);
	mTextView4Ptr->setTouchPass(true);
}

/**
 * 当切换到该界面时触发
 */
static void onUI_intent(const Intent *intentPtr) {
    if (intentPtr != NULL) {
        //TODO
    }
}

/*
 * 当界面显示时触发
 */
static void onUI_show() {

}

/*
 * 当界面隐藏时触发
 */
static void onUI_hide() {

}

/*
 * 当界面完全退出时触发
 */
static void onUI_quit() {
	media::remove_scan_cb(_media_scan_cb);
	PAsw.SetIsInitDone(false);
	fy::drop_caches();
}

/**
 * 串口数据回调接口
 */
static void onProtocolDataUpdate(const SProtocolData &data) {

}

/**
 * 定时器触发函数
 * 不建议在此函数中写耗时操作，否则将影响UI刷新
 * 参数： id
 *         当前所触发定时器的id，与注册时的id相同
 * 返回值: true
 *             继续运行当前定时器
 *         false
 *             停止运行当前定时器
 */
static bool onUI_Timer(int id){
	switch (id) {
	case CAROUSELPICTIMER:{
		LOGD("Photo Player Timer is runing!");
		PAsw.Next_Wndow();
	}
	break;
	case REFRESHALUMTIMER:{
		ZKTextView *PhotoFrame[] = {mIndexPic1Ptr,mIndexPic2Ptr,mIndexPic3Ptr};
		if(IsSlideLeftDone){
			Horizontal_Vertical_Center_Pic(PhotoFrame[PAsw.Get_Next_Wnd_Index()],
				ImagesFilter(media::get_image_file(albumtype,(IndexPic < (media::get_image_list_size(albumtype)-1))?IndexPic+1:0)));
		}else{
			Horizontal_Vertical_Center_Pic(PhotoFrame[PAsw.Get_Pre_Wnd_Index()],
				ImagesFilter(media::get_image_file(albumtype,IndexPic>0?IndexPic-1:media::get_image_list_size(albumtype)-1)));
		}
		return false;
	}
	break;
		default:
			break;
	}
    return true;
}

/**
 * 有新的触摸事件时触发
 * 参数：ev
 *         新的触摸事件
 * 返回值：true
 *            表示该触摸事件在此被拦截，系统不再将此触摸事件传递到控件上
 *         false
 *            触摸事件将继续传递到控件上
 */
static bool onPhotoAlbumActivityTouchEvent(const MotionEvent &ev) {
#if SLIDESWITCHENABLE
	static bool AvoidRepeatOperate = false;
	if((EASYUICONTEXT->getNaviBar()->getPosition().mTop > -SCREENRESOULUTION_HEIGHT) && !AvoidRepeatOperate){
		SlideEnable = false;
		AvoidRepeatOperate = true;
	}
	if((EASYUICONTEXT->getNaviBar()->getPosition().mTop == -SCREENRESOULUTION_HEIGHT) && AvoidRepeatOperate){
		SlideEnable = true;
		AvoidRepeatOperate = false;
	}
	if(SlideEnable && !mCarouselPicPtr->isSelected() && !mAlbumClassificationPtr->isVisible()){
		PAsw.slideWnd_handle(ev);
	}
#endif
    switch (ev.mActionStatus) {
		case MotionEvent::E_ACTION_DOWN://触摸按下
			//LOGD("时刻 = %ld 坐标  x = %d, y = %d", ev.mEventTime, ev.mX, ev.mY);
			break;
		case MotionEvent::E_ACTION_MOVE://触摸滑动
//			LOGD("PhotoAlbumActivityTouch触摸滑动");
			break;
		case MotionEvent::E_ACTION_UP:  //触摸抬起
			break;
		default:
			break;
	}
	return false;
}

static void reset_pos(Photos pos){
	 ZKTextView *PhotoFrame[] = {mIndexPic1Ptr,mIndexPic2Ptr,mIndexPic3Ptr};
	 LayoutPosition real_pos = PhotoFrame[PAsw.Get_Curr_Wnd_Index()]->getPosition();
	 if(pos.mLeft != real_pos.mLeft){
		 s_multiple = 1;
		 PhotoFrame[PAsw.Get_Curr_Wnd_Index()]->setPosition(LayoutPosition(pos.mLeft,pos.mTop,pos.width,pos.height));
	 }
}
static bool onButtonClick_Prev(ZKButton *pButton) {
    LOGD(" ButtonClick Prev !!!\n");
    if(media::get_image_list_size(albumtype) <= 0 || mCarouselPicPtr->isSelected())
    	return false;
    reset_pos(ImagesFilter(media::get_image_file(albumtype,IndexPic)));
    PAsw.Pre_Wndow();
    return false;
}

static bool onButtonClick_Next(ZKButton *pButton) {
    LOGD(" ButtonClick Next !!!\n");
    if(media::get_image_list_size(albumtype) <= 0 || mCarouselPicPtr->isSelected())
    	return false;
    reset_pos(ImagesFilter(media::get_image_file(albumtype,IndexPic)));
    PAsw.Next_Wndow();
    return false;
}

static bool onButtonClick_SD(ZKButton *pButton) {
    LOGD(" ButtonClick SD !!!\n");
    albumtype = E_STORAGE_TYPE_SD;
    SelectAlbum();
	mImagesListViewPtr->refreshListView();
    return false;
}

static bool onButtonClick_USB1(ZKButton *pButton) {
    LOGD(" ButtonClick USB1 !!!\n");
    albumtype = E_STORAGE_TYPE_USB1;
    SelectAlbum();
	mImagesListViewPtr->refreshListView();
    return false;
}

static bool onButtonClick_USB2(ZKButton *pButton) {
    LOGD(" ButtonClick USB2 !!!\n");
    albumtype = E_STORAGE_TYPE_USB2;
    SelectAlbum();
	mImagesListViewPtr->refreshListView();
    return false;
}

static void displayopposite(){
	mIndexPtr->setVisible(!mIndexPtr->isVisible());
	mCarouselPicPtr->setVisible(mIndexPtr->isVisible());
	mamplifyBtnPtr->setVisible(mIndexPtr->isVisible());
	mreduceBtnPtr->setVisible(mIndexPtr->isVisible());
	mTextView5Ptr->setVisible(mIndexPtr->isVisible());
	mbackPtr->setVisible(mIndexPtr->isVisible());
	if (SlideEnable) {
		mPrevPtr->setVisible(mIndexPtr->isVisible());
		mNextPtr->setVisible(mIndexPtr->isVisible());
	}
}
static bool onButtonClick_IndexBt1(ZKButton *pButton) {
    LOGD(" ButtonClick IndexBt1 !!!\n");
    displayopposite();
    return false;
}
static bool onButtonClick_sys_back(ZKButton *pButton) {
    LOGD(" ButtonClick sys_back !!!\n");
    if(PAsw.isRunning())
    	PAsw.requestExitAndWait();

    return false;
}
static bool onButtonClick_IndexBt2(ZKButton *pButton) {
    LOGD(" ButtonClick IndexBt2 !!!\n");
    displayopposite();
    return false;
}

static bool onButtonClick_IndexBt3(ZKButton *pButton) {
    LOGD(" ButtonClick IndexBt3 !!!\n");
    displayopposite();
    return false;
}

#define DEBUG	LOGD("--%d-- --%s-- DEBUG!\n", __LINE__, __FILE__);

static bool onButtonClick_CarouselPic(ZKButton *pButton) {
    LOGD(" ButtonClick CarouselPic !!!\n");

    if(media::get_image_list_size(albumtype) <= 0) {DEBUG
    	return false;
    }
    DEBUG
    if(!mCarouselPicPtr->isSelected()){//如果未选中 开启轮播
    	DEBUG
		reset_pos(ImagesFilter(media::get_image_file(albumtype,IndexPic)));
    	SlideEnable = false;
    	mActivityPtr->registerUserTimer(CAROUSELPICTIMER,5000);
        SetDisplayOpposite(false);
    }
    else{//如果选中 关闭轮播
    	DEBUG
    	SlideEnable = true;
    	SetDisplayOpposite(true);
    	mActivityPtr->unregisterUserTimer(CAROUSELPICTIMER);
    }
    DEBUG
    mCarouselPicPtr->setSelected(!mCarouselPicPtr->isSelected());
    return false;
}

static int getListItemCount_ImagesListView(const ZKListView *pListView) {
    //LOGD("getListItemCount_ImagesListView !\n");
	if (media::get_image_list_size(albumtype) == 0) {
		mTextView6Ptr->setTextTr("No files");
	} else {
		mTextView6Ptr->setText("");
	}
    return media::get_image_list_size(albumtype);
}

static void obtainListItemData_ImagesListView(ZKListView *pListView,ZKListView::ZKListItem *pListItem, int index) {
    //LOGD(" obtainListItemData_ ImagesListView  !!!\n");
	ZKListView::ZKListSubItem *name = pListItem->findSubItemByID(ID_PHOTOALBUM_name);
	pListItem->setText(index+1);
	if(is_sel){
		pListItem->setSelected(IndexPic == index ? true : false);
		name->setSelected(IndexPic == index ? true : false);
	}
	name->setText(fy::files::get_file_name(media::get_image_file(albumtype,index)));
}

static void onListItemClick_ImagesListView(ZKListView *pListView, int index, int id) {
    //LOGD(" onListItemClick_ ImagesListView  !!!\n");
	is_sel = false;
	IndexPic = index;
	ZKTextView *PhotoFrame[] = {mIndexPic1Ptr,mIndexPic2Ptr,mIndexPic3Ptr};
	char indextext[24] = {0};
	int size = media::get_image_list_size(albumtype);
	sprintf(indextext,"%d/%d",IndexPic+1,size);
	mIndexPtr->setText(indextext);
	Horizontal_Vertical_Center_Pic(PhotoFrame[PAsw.Get_Pre_Wnd_Index()],
			ImagesFilter(media::get_image_file(albumtype,IndexPic>0?IndexPic-1:size-1)));
	Horizontal_Vertical_Center_Pic(PhotoFrame[PAsw.Get_Curr_Wnd_Index()],ImagesFilter(media::get_image_file(albumtype,IndexPic)));
	Horizontal_Vertical_Center_Pic(PhotoFrame[PAsw.Get_Next_Wnd_Index()],
			ImagesFilter(media::get_image_file(albumtype,(IndexPic < (size-1))?IndexPic+1:0)));
	SetDisplayOpposite(true);
	mAlbumClassificationPtr->hideWnd();
//	app::hide_topbar();
}

static bool onButtonClick_back(ZKButton *pButton) {
    LOGD(" ButtonClick back !!!\n");
    if(!mAlbumClassificationPtr->isWndShow()){
    	mAlbumClassificationPtr->showWnd();
//    	app::show_topbar();
        is_sel = true;
        mImagesListViewPtr->setSelection(IndexPic);

        //如果选中 关闭轮播
        if (mCarouselPicPtr->isSelected()) {
        	SlideEnable = true;
        	SetDisplayOpposite(true);
        	mActivityPtr->unregisterUserTimer(CAROUSELPICTIMER);

        	mCarouselPicPtr->setSelected(false);
        }
    }
    return false;
}


static bool onButtonClick_amplifyBtn(ZKButton *pButton) {
    if (mCarouselPicPtr->isSelected())
    	return false;
    LOGD(" ButtonClick amplifyBtn !!!\n");
    ZKTextView *PhotoFrame[] = {mIndexPic1Ptr,mIndexPic2Ptr,mIndexPic3Ptr};
    LayoutPosition pos = PhotoFrame[PAsw.Get_Curr_Wnd_Index()]->getPosition();
    if(s_multiple < MAXMUL){
    	PhotoFrame[PAsw.Get_Curr_Wnd_Index()]->setPosition(LayoutPosition(pos.mLeft - LEFT_POS,pos.mTop - LEFT_POS,pos.mWidth + TOP_POS,pos.mHeight + TOP_POS));
    	s_multiple++;
    }
    return false;
}

static bool onButtonClick_reduceBtn(ZKButton *pButton) {
    if (mCarouselPicPtr->isSelected())
    	return false;
    LOGD(" ButtonClick reduceBtn !!!\n");
    ZKTextView *PhotoFrame[] = {mIndexPic1Ptr,mIndexPic2Ptr,mIndexPic3Ptr};
    LayoutPosition pos = PhotoFrame[PAsw.Get_Curr_Wnd_Index()]->getPosition();
    if(s_multiple > 1){
    	PhotoFrame[PAsw.Get_Curr_Wnd_Index()]->setPosition(LayoutPosition(pos.mLeft + LEFT_POS,pos.mTop + LEFT_POS,pos.mWidth - TOP_POS,pos.mHeight - TOP_POS));
    	s_multiple--;
    }
    return false;
}

static bool onButtonClick_rotateBtn(ZKButton *pButton) {
    LOGD(" ButtonClick rotateBtn !!!\n");
    ZKTextView *PhotoFrame[] = {mIndexPic1Ptr,mIndexPic2Ptr,mIndexPic3Ptr};
    return false;
}
