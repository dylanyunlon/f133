#pragma once
#include "uart/ProtocolSender.h"
#include "system/setting.h"
#include "misc/storage.h"
#include "common.h"
#include "utils/BrightnessHelper.h"

static int _s_signal_check_count;
extern void screenOn_event();
extern void fold_statusbar();
namespace {

class MyPictureCallback : public ZKCameraView::IPictureCallback {
private:
	virtual const char* onPictureSavePath() {
		LOGD("onPictureSavePath!");
		return "/tmp/cam.jpg";
	}
};

class MyErrorCodeCallback : public ZKCameraView::IErrorCodeCallback {
private:
	virtual void onErrorCode(int error) {
//		LOGD("@@ onErrorCode error: %d\n", error);
		if (error == E_CAMERA_STATUS_CODE_NO_SIGNAL) {
			if (_s_signal_check_count < 2) {   // 检测几次无信号才出提示
				_s_signal_check_count++;
			} else {
				mSignTextViewPtr->setVisible(true);
			}
		} else if (error == E_CAMERA_STATUS_CODE_HAS_SIGNAL) {
			_s_signal_check_count = 0;
			mSignTextViewPtr->setVisible(false);
		}
	}
};
struct cam_info_t {
	const char *name;
	int w;
	int h;
	int rate;
	bool di_enable;   // 使能硬件奇偶合并
	bool checked;
};

static cam_info_t _s_cam_info_tab[] = {
	{ "AHD 720P 25", 1280, 720, 25, false, false },
	{ "AHD 720P 30", 1280, 720, 30, false, false },
	{ "TVI 720P 25", 1280, 720, 24, false, false },
	{ "TVI 720P 30", 1280, 720, 29, false, false },
	{ "AHD 1080P 25", 1920, 1080, 25, false, false },
	{ "AHD 1080P 30", 1920, 1080, 30, false, false },
	{ "TVI 1080P 25", 1920, 1080, 24, false, false },
	{ "TVI 1080P 30", 1920, 1080, 29, false, false },
	{ "CVBS PAL 50", 960, 576, 50, true, false },
	{ "CVBS NTSC 60", 960, 480, 60, true, false },
	{ "DM5885 50", 720, 480, 50, false, false },   // 逐行
	{ "DM5885 49", 720, 480, 49, true, false },    // 隔行
};

}

static MyPictureCallback sMyPictureCallback;
static MyErrorCodeCallback sMyErrorCodeCallback;


/**
 * 注册定时器
 * 填充数组用于注册定时器
 * 注意：id不能重复
 */
static S_ACTIVITY_TIMEER REGISTER_ACTIVITY_TIMER_TAB[] = {
	//{0,  6000}, //定时器id=0, 时间间隔6秒
};

/**
 * 当界面构造时触发
 */
static void onUI_init() {
    //Tips :添加 UI初始化的显示代码到这里,如:mText1Ptr->setText("123");
	fold_statusbar();
	screenOn_event();
	mCameraViewReversePtr->setErrorCodeCallback(&sMyErrorCodeCallback);
	mCameraViewReversePtr->setPictureCallback(&sMyPictureCallback);
	mCameraViewReversePtr->setDevPath(sys::setting::get_camera_dev());
	mCameraViewReversePtr->setChannel(sys::setting::get_camera_chn());
	mCameraViewReversePtr->setFormatSize(sys::setting::get_camera_wide(),sys::setting::get_camera_high());
	mCameraViewReversePtr->setFrameRate(sys::setting::get_camera_rate());
	mCameraViewReversePtr->setOption("req_bufs_count", "3");
	mCameraViewReversePtr->setOption("mem_type", "2");
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
	WAIT(!mCameraViewReversePtr->isPreviewing(), 100, 50);
}

/*
 * 当界面完全退出时触发
 */
static void onUI_quit() {
	mCameraViewReversePtr->setErrorCodeCallback(NULL);
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
static bool onreverseActivityTouchEvent(const MotionEvent &ev) {

    switch (ev.mActionStatus) {
		case MotionEvent::E_ACTION_DOWN://触摸按下
			//LOGD("时刻 = %ld 坐标  x = %d, y = %d", ev.mEventTime, ev.mX, ev.mY);
			break;
		case MotionEvent::E_ACTION_MOVE://触摸滑动
			break;
		case MotionEvent::E_ACTION_UP:  //触摸抬起
			break;
		default:
			break;
	}
	return false;
}

static int getListItemCount_ListView1(const ZKListView *pListView) {
    //LOGD("getListItemCount_ListView1 !\n");
	 return TABLESIZE(_s_cam_info_tab);
}

static void obtainListItemData_ListView1(ZKListView *pListView,ZKListView::ZKListItem *pListItem, int index) {
    //LOGD(" obtainListItemData_ ListView1  !!!\n");
	pListItem->setText(_s_cam_info_tab[index].name);
	pListItem->setSelected(_s_cam_info_tab[index].checked);

}

static void onListItemClick_ListView1(ZKListView *pListView, int index, int id) {
    //LOGD(" onListItemClick_ ListView1  !!!\n");

	mCameraViewReversePtr->stopPreview();
	WAIT(!mCameraViewReversePtr->isPreviewing(), 100, 50);

	mCameraViewReversePtr->setFormatSize(_s_cam_info_tab[index].w, _s_cam_info_tab[index].h);
	mCameraViewReversePtr->setFrameRate(_s_cam_info_tab[index].rate);

#if 1
	// 通过环境变量设置开启关闭奇偶合并功能，目前N制P制才使能，切换到其他格式前需要置为0，可对比开启关闭后的效果，权衡下是否打开
	setenv("ZKCAMERA_DI_ENABLE", _s_cam_info_tab[index].di_enable ? "1" : "0", 1);
#endif

	mCameraViewReversePtr->startPreview();

	for (int i = 0; i < (int)TABLESIZE(_s_cam_info_tab); ++i) {
		_s_cam_info_tab[i].checked = (i == index);
	}
}
