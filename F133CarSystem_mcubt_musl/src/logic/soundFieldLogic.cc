#pragma once
#include "uart/ProtocolSender.h"
/*
*此文件由GUI工具生成
*文件功能：用于处理用户的逻辑相应代码
*功能说明：
*========================onButtonClick_XXXX
当页面中的按键按下后系统会调用对应的函数，XXX代表GUI工具里面的[ID值]名称，
如Button1,当返回值为false的时候系统将不再处理这个按键，返回true的时候系统将会继续处理此按键。比如SYS_BACK.
*========================onSlideWindowItemClick_XXXX(int index) 
当页面中存在滑动窗口并且用户点击了滑动窗口的图标后系统会调用此函数,XXX代表GUI工具里面的[ID值]名称，
如slideWindow1;index 代表按下图标的偏移值
*========================onSeekBarChange_XXXX(int progress) 
当页面中存在滑动条并且用户改变了进度后系统会调用此函数,XXX代表GUI工具里面的[ID值]名称，
如SeekBar1;progress 代表当前的进度值
*========================ogetListItemCount_XXXX() 
当页面中存在滑动列表的时候，更新的时候系统会调用此接口获取列表的总数目,XXX代表GUI工具里面的[ID值]名称，
如List1;返回值为当前列表的总条数
*========================oobtainListItemData_XXXX(ZKListView::ZKListItem *pListItem, int index)
 当页面中存在滑动列表的时候，更新的时候系统会调用此接口获取列表当前条目下的内容信息,XXX代表GUI工具里面的[ID值]名称，
如List1;pListItem 是贴图中的单条目对象，index是列表总目的偏移量。具体见函数说明
*========================常用接口===============
*LOGD(...)  打印调试信息的接口
*mTextXXXPtr->setText("****") 在控件TextXXX上显示文字****
*mButton1Ptr->setSelected(true); 将控件mButton1设置为选中模式，图片会切换成选中图片，按钮文字会切换为选中后的颜色
*mSeekBarPtr->setProgress(12) 在控件mSeekBar上将进度调整到12
*mListView1Ptr->refreshListView() 让mListView1 重新刷新，当列表数据变化后调用
*mDashbroadView1Ptr->setTargetAngle(120) 在控件mDashbroadView1上指针显示角度调整到120度
*
* 在Eclipse编辑器中  使用 “alt + /”  快捷键可以打开智能提示
*/

#include "uart/context.h"

/**
 * 注册定时器
 * 填充数组用于注册定时器
 * 注意：id不能重复
 */
static S_ACTIVITY_TIMEER REGISTER_ACTIVITY_TIMER_TAB[] = {
	//{0,  6000}, //定时器id=0, 时间间隔6秒
	//{1,  1000},
};

/**
 * 当界面构造时触发
 */
static void onUI_init(){
    //Tips :添加 UI初始化的显示代码到这里,如:mText1Ptr->setText("123");

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
static bool onUI_Timer(int id) {
	switch (id) {

		default:
			break;
	}
    return true;
}

static void touch_adjust_sound_field(MotionEvent ev) {
	LayoutPosition pos = mfieldWindowPtr->getPosition();
	ev.mX -= pos.mLeft;
	ev.mY -= pos.mTop;

	static bool isSelect = false;
	static LayoutPosition tpos = mtouchTextViewPtr->getPosition();

    switch (ev.mActionStatus) {
		case MotionEvent::E_ACTION_DOWN://触摸按下
			isSelect = mtouchTextViewPtr->getPosition().isHit(ev.mX, ev.mY);
			//LOGD("时刻 = %ld 坐标  x = %d, y = %d", ev.mEventTime, ev.mX, ev.mY);
			break;
		case MotionEvent::E_ACTION_MOVE://触摸滑动
			if (isSelect) {
				tpos.mLeft = ev.mX - tpos.mWidth / 2;
				tpos.mTop = ev.mY - tpos.mHeight / 2;
				mtouchTextViewPtr->setPosition(tpos);
			}
			break;
		case MotionEvent::E_ACTION_UP: {  //触摸抬起
			if (isSelect) {
				isSelect = false;
//				int _X = ev.mX / 20;
//				int _Y = ev.mY / 20;

				int _X = (ev.mX / 10) - 20;
				int _Y = (ev.mY / 10) - 20;

				int LD = 20, RU = 20, LU = 20, RD = 20;
				if (_X > 0) {
					LD = 20 - _X;
				} else {
					RU = 20 - abs(_X);
				}

				if (_Y > 0) {
					LU = 20 - abs(_Y);
				} else {
					RD =  20 - _Y;
				}


				LOGD("触摸抬起: _X = %d, _Y = %d \n", _X, _Y);

				uart::set_sound_field(SOUND_FIELD_LU, LD); //20 - _X);	// LD
				uart::set_sound_field(SOUND_FIELD_RU, RU);//_X);
				uart::set_sound_field(SOUND_FIELD_LD, LU);//20 - _Y);	// LU
				uart::set_sound_field(SOUND_FIELD_RD, RD);//_Y);


				LOGD("触摸抬起: 左下 = %d, 右上 = %d, 右上 = %d, 右下 = %d \n", LD, RU, LU, RD);

			}


		}
			break;
		default:
			break;
	}


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
static bool onsoundFieldActivityTouchEvent(const MotionEvent &ev) {
	if (mfieldWindowPtr->isWndShow() && mfieldWindowPtr->getPosition().isHit(ev.mX, ev.mY)) {
		touch_adjust_sound_field(ev);
	}

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

static void onProgressChanged_fieldSeekBar0(ZKSeekBar *pSeekBar, int progress) {
    //LOGD(" ProgressChanged fieldSeekBar0 %d !!!\n", progress);
	if (!mfieldWindowPtr->isWndShow()) {
		uart::set_sound_field(SOUND_FIELD_LU, progress);
	}
}

static void onProgressChanged_fieldSeekBar1(ZKSeekBar *pSeekBar, int progress) {
    //LOGD(" ProgressChanged fieldSeekBar1 %d !!!\n", progress);
	if (!mfieldWindowPtr->isWndShow()) {
		uart::set_sound_field(SOUND_FIELD_RU, progress);
	}
}

static void onProgressChanged_fieldSeekBar2(ZKSeekBar *pSeekBar, int progress) {
    //LOGD(" ProgressChanged fieldSeekBar2 %d !!!\n", progress);
	if (!mfieldWindowPtr->isWndShow()) {
		uart::set_sound_field(SOUND_FIELD_LD, progress);
	}
}

static void onProgressChanged_fieldSeekBar3(ZKSeekBar *pSeekBar, int progress) {
    //LOGD(" ProgressChanged fieldSeekBar3 %d !!!\n", progress);
	if (!mfieldWindowPtr->isWndShow()) {
		uart::set_sound_field(SOUND_FIELD_RD, progress);
	}
}

static bool onButtonClick_sys_back(ZKButton *pButton) {
    LOGD(" ButtonClick sys_back !!!\n");
    return false;
}

static bool onButtonClick_Button1(ZKButton *pButton) {
    LOGD(" ButtonClick Button1 !!!\n");
    mfieldWindowPtr->showWnd();
    return false;
}
