#pragma once
#include "uart/ProtocolSender.h"
#include "system/Mutex.h"
#include "utils/ScreenHelper.h"
#include "manager/ConfigManager.h"
#include "link/context.h"
#include "bt/context.h"
#include "fy/os.hpp"
#include "common.h"

#define DELAY_PLAY_TIMER     1

#define FRAME_VIEW_SYNC      1
#define TOUCH_MOVE_THRESHOLD 5

static bt_cb_t _s_bt_cb;

typedef struct {
	const char *data;
	LYLINK_KEYCODE_E keycode;
} square_data;

static square_data _s_square_data_tab[] = {

	"HiCar200",LY_dpadActionRight,//hicar右移键值
	"HiCar100",LY_dpadActionLeft,//hicar左移键值
	"HiCar040",LY_knobActionSelect,//hicar选择键值
	"HiCar020",LY_knobActionHome,//hicar home键值
	"HiCar010",LY_knobActionBack,//hicar 返回键值

	"CpAuto200",LY_knobActionClockWise,//carplay,auto右移键值
	"CpAuto100",LY_knobActionCounterClockWise,//carplay,auto左移键值
	"CpAuto040",LY_knobActionSelect,//carplay,auto 选择键值
	"CpAuto020",LY_knobActionHome,//carplay,auto home键值
	"CpAuto010",LY_knobActionBack,//carplay,auto 返回键值

	"CpAuto0800",LY_knobActionClockWise,
	"CpAuto1000",LY_knobActionSelect,
	"CpAuto0400",LY_knobActionCounterClockWise,
};

/**
 * 注册定时器
 * 填充数组用于注册定时器
 * 注意：id不能重复
 */
static S_ACTIVITY_TIMEER REGISTER_ACTIVITY_TIMER_TAB[] = {
	//{0,  6000}, //定时器id=0, 时间间隔6秒
	//{1,  1000},
};

static void _link_view_show(const LYLINKAPI_VIDEO_PARAM *param) {
	int w = param->width, h = param->height;
	int sw = LINK_VIEW_WIDTH, sh = LINK_VIEW_HEIGHT;
	int vx = 0, vy = 0, vw = sw, vh = sh;  // view pos
	int rot = CONFIGMANAGER->getScreenRotate();

	switch (lk::get_lylink_type()) {
	case LINK_TYPE_AIRPLAY:
	case LINK_TYPE_MIRACAST:
	case LINK_TYPE_WIFILY:
		if ((float) sw / w > (float) sh / h) {
			vw = sh * w / h;
			vx = (sw - vw) / 2;
		} else {
			vh = sw * h / w;
			vy = (sh - vh) / 2;
		}

		if ((rot == 90) || (rot == 270)) {
			std::swap(vx, vy);
			std::swap(vw, vh);
		}
		break;

	default:
		if ((rot == 90) || (rot == 270)) {
			std::swap(vw, vh);
		}
		break;
	}

	lk::video_show(vx, vy, vw, vh);
}

static void _link_view_hide() {
	lk::video_hide();
}

static void _link_touch(LYLINK_TOUCHMODE_E mode, int x, int y) {
	lylinkapi_touch(0, mode, x, y);
}

static void _lylink_video_callback(LYLINKAPI_EVENT evt, int para0, void *para1) {
	if(evt == LYLINK_VIDEO_START) {
		const LYLINKAPI_VIDEO_PARAM *param = (const LYLINKAPI_VIDEO_PARAM *) para1;
		LOGD("[lylinkview] LYLINK_VIDEO_START %d %d\n", param->width, param->height);
		_link_view_show(param);
	} else if(evt == LYLINK_VIDEO_DATA){
#if FRAME_VIEW_SYNC
		mVideoView1Ptr->setVisible(true);
#endif
	}
}


static const char* _link_key_to_str(LYLINK_KEYCODE_E type){
	switch(type){
	case LY_knobActionClockWise:
	case LY_dpadActionRight:
		return "Right";
	case LY_knobActionSelect:
		return "Select";
	case LY_knobActionCounterClockWise:
	case LY_dpadActionLeft:
		return "Left";
	case LY_knobActionHome:
		return "Home";
	case LY_knobActionBack:
		return "Back";
	default:
		return "unknown";
	}
}

static void _square_data_cb(const char *data){
	char keydata[24] = {0};
	if(lk::get_lylink_type() == LINK_TYPE_WIFIHICAR){
		sprintf(keydata,"HiCar%s",data);
	}else if(lk::get_lylink_type() == LINK_TYPE_WIFICP||lk::get_lylink_type() == LINK_TYPE_WIFIAUTO){
		sprintf(keydata,"CpAuto%s",data);
	}
	for(size_t i = 0; i < TAB_SIZE(_s_square_data_tab);i++){
		if (strcmp(_s_square_data_tab[i].data, keydata) == 0) {
			LOGD("[lylinkview] control    %s",_link_key_to_str(_s_square_data_tab[i].keycode));
			lylinkapi_key(_s_square_data_tab[i].keycode, KeyMode_Down);
			lylinkapi_key(_s_square_data_tab[i].keycode, KeyMode_Up);
			break;
		}
	}
}

static void _bt_add_cb() {
	_s_bt_cb.square_data_cb = _square_data_cb;
	bt::add_cb(&_s_bt_cb);
}

static void _bt_remove_cb() {
	bt::remove_cb(&_s_bt_cb);
}

static void _link_start() {
	lk::add_lylink_callback(_lylink_video_callback);

	LYLINKAPI_VIDEO_PARAM param;
	if (lk::get_video_param(param)) {
		// 有视频信息直接显示
		_link_view_show(&param);
	} else {
		// 没有视频信息，启动下视频
		lk::video_start();
	}
//	if(lk::get_lylink_type() == LINK_TYPE_WIFILY || lk::get_lylink_type() == LINK_TYPE_USBLY) {
		lylinkapi_cmd(LYLINK_AUDIO_LYLINK);
//		audio::change_audio_type(E_AUDIO_TYPE_LYLINK_MUSIC);
//	}
}

static void _link_stop() {
	LOGD("[lylinkview] link stop");
	LYLINK_TYPE_E link_type = lk::get_lylink_type();
	if(link_type != LINK_TYPE_WIFILY) {
		// 回到本地模式
		lk::video_stop();
	}
	lk::remove_lylink_callback(_lylink_video_callback);
	_link_view_hide();
}

#if 1
namespace {

class ViewTouchListener: public ZKBase::ITouchListener {
public:
	virtual void onTouchEvent(ZKBase *pBase, const MotionEvent &ev) {
		if (lk::get_lylink_type() == LINK_TYPE_MIRACAST) {
			return;
		}

		switch (ev.mActionStatus) {
		case MotionEvent::E_ACTION_DOWN:
			_link_touch(TouchMode_Down, ev.mX, ev.mY);
			mLastEv = ev;
			break;
		case MotionEvent::E_ACTION_MOVE:	// 触摸滑动
			if ((abs(ev.mX - mLastEv.mX) >= TOUCH_MOVE_THRESHOLD) ||
					(abs(ev.mY - mLastEv.mY) >= TOUCH_MOVE_THRESHOLD)) {
				mLastEv = ev;
				_link_touch(TouchMode_Move, ev.mX , ev.mY);
			}
			break;
		default:
			_link_touch(TouchMode_Up, ev.mX, ev.mY);
			break;
		}
	}

private:
	MotionEvent mLastEv;
};

}

static ViewTouchListener _s_view_touch_listener;
#endif
/**
 * 当界面构造时触发
 */
static void onUI_init() {
    //Tips :添加 UI初始化的显示代码到这里,如:mText1Ptr->setText("123");
	_bt_add_cb();
	LayoutPosition pos = LayoutPosition(0, 0, ScreenHelper::getScreenWidth(), ScreenHelper::getScreenHeight());
	mVideoView1Ptr->setPosition(pos);
	mActivityPtr->setPosition(pos);
#if FRAME_VIEW_SYNC
	mVideoView1Ptr->setVisible(false);
#endif
	mVideoView1Ptr->setTouchListener(&_s_view_touch_listener);
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
	// 推迟检测播放，处理倒车界面未完全退出，播放异常问题
	//usleep(1000*1000);
	mActivityPtr->registerUserTimer(DELAY_PLAY_TIMER, 0);
  	// 进入互联界面释放一下内存
//	system("echo 3 > /proc/sys/vm/drop_caches");
	fy::drop_caches();
}

/*
 * 当界面隐藏时触发
 */
static void onUI_hide() {
	mActivityPtr->unregisterUserTimer(DELAY_PLAY_TIMER);
	_link_stop();
}

/*
 * 当界面完全退出时触发
 */
static void onUI_quit() {
	_bt_remove_cb();
	mVideoView1Ptr->setTouchListener(NULL);
	_link_stop();
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
	case DELAY_PLAY_TIMER:
		_link_start();
		// 只执行一次，停掉该定时器
		return false;
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
static bool onlylinkviewActivityTouchEvent(const MotionEvent &ev) {
	static MotionEvent last_ev;
	int status = ev.mActionStatus;
	LayoutPosition pos = EASYUICONTEXT->getNaviBar()->getPosition();
	if (pos.mTop != -pos.mHeight) {	//导航栏下滑了
		if (last_ev.mActionStatus == MotionEvent::E_ACTION_NONE) {
			return false;
		}
		status = MotionEvent::E_ACTION_UP;
	}

	if (lk::get_lylink_type() == LINK_TYPE_MIRACAST) {
		return false;
	}
	switch (ev.mActionStatus) {
	case MotionEvent::E_ACTION_DOWN:	//触摸按下
		//LOGD("时刻 = %ld 坐标  x = %d, y = %d", ev.mEventTime, ev.mX, ev.mY);
		break;
	case MotionEvent::E_ACTION_MOVE:	        //触摸滑动
		break;
	case MotionEvent::E_ACTION_UP:  //触摸抬起
		break;
	default:
		break;
	}
	return false;
}




static bool onButtonClick_sys_home(ZKButton *pButton) {
    LOGD(" ButtonClick sys_home !!!\n");
    return false;
}
static bool onButtonClick_sys_back(ZKButton *pButton) {
    LOGD(" ButtonClick sys_back !!!\n");
    return false;
}
