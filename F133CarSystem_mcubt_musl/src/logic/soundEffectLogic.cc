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

#define SET_EFFECT_VLAUE 12

struct C_effect{
	int effect_vlaue;
};

static ZKSeekBar **seekbar[] = {&meqSeekBar0Ptr,&meqSeekBar1Ptr,&meqSeekBar2Ptr,&meqSeekBar3Ptr,
		&meqSeekBar4Ptr,&meqSeekBar5Ptr,&meqSeekBar6Ptr,&meqSeekBar7Ptr,&meqSeekBar8Ptr,&meqSeekBar9Ptr,};

static C_effect s_effect[10] = {12,12,12,12,12,12,12,12,12,12};
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
	meqSeekBar0Ptr->setProgress(s_effect[0].effect_vlaue);
	meqSeekBar1Ptr->setProgress(s_effect[1].effect_vlaue);
	meqSeekBar2Ptr->setProgress(s_effect[2].effect_vlaue);
	meqSeekBar3Ptr->setProgress(s_effect[3].effect_vlaue);
	meqSeekBar4Ptr->setProgress(s_effect[4].effect_vlaue);
	meqSeekBar5Ptr->setProgress(s_effect[5].effect_vlaue);
	meqSeekBar6Ptr->setProgress(s_effect[6].effect_vlaue);
	meqSeekBar7Ptr->setProgress(s_effect[7].effect_vlaue);
	meqSeekBar8Ptr->setProgress(s_effect[8].effect_vlaue);
	meqSeekBar9Ptr->setProgress(s_effect[9].effect_vlaue);

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
static bool onsoundEffectActivityTouchEvent(const MotionEvent &ev) {
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

static void onProgressChanged_eqSeekBar0(ZKSeekBar *pSeekBar, int progress) {
    //LOGD(" ProgressChanged eqSeekBar0 %d !!!\n", progress);
	uart::set_sound_effect(0, progress);
	s_effect[0].effect_vlaue = progress;
}

static void onProgressChanged_eqSeekBar1(ZKSeekBar *pSeekBar, int progress) {
    //LOGD(" ProgressChanged eqSeekBar1 %d !!!\n", progress);
	uart::set_sound_effect(1, progress);
	s_effect[1].effect_vlaue = progress;
}

static void onProgressChanged_eqSeekBar2(ZKSeekBar *pSeekBar, int progress) {
    //LOGD(" ProgressChanged eqSeekBar2 %d !!!\n", progress);
	uart::set_sound_effect(2, progress);
	s_effect[2].effect_vlaue = progress;
}

static void onProgressChanged_eqSeekBar3(ZKSeekBar *pSeekBar, int progress) {
    //LOGD(" ProgressChanged eqSeekBar3 %d !!!\n", progress);
	uart::set_sound_effect(3, progress);
	s_effect[3].effect_vlaue = progress;
}

static void onProgressChanged_eqSeekBar4(ZKSeekBar *pSeekBar, int progress) {
    //LOGD(" ProgressChanged eqSeekBar4 %d !!!\n", progress);
	uart::set_sound_effect(4, progress);
	s_effect[4].effect_vlaue = progress;
}

static void onProgressChanged_eqSeekBar5(ZKSeekBar *pSeekBar, int progress) {
    //LOGD(" ProgressChanged eqSeekBar5 %d !!!\n", progress);
	uart::set_sound_effect(5, progress);
	s_effect[5].effect_vlaue = progress;
}

static void onProgressChanged_eqSeekBar6(ZKSeekBar *pSeekBar, int progress) {
    //LOGD(" ProgressChanged eqSeekBar6 %d !!!\n", progress);
	uart::set_sound_effect(6, progress);
	s_effect[6].effect_vlaue = progress;
}

static void onProgressChanged_eqSeekBar7(ZKSeekBar *pSeekBar, int progress) {
    //LOGD(" ProgressChanged eqSeekBar7 %d !!!\n", progress);
	uart::set_sound_effect(7, progress);
	s_effect[7].effect_vlaue = progress;
}

static void onProgressChanged_eqSeekBar8(ZKSeekBar *pSeekBar, int progress) {
    //LOGD(" ProgressChanged eqSeekBar8 %d !!!\n", progress);
	uart::set_sound_effect(8, progress);
	s_effect[8].effect_vlaue = progress;
}

static void onProgressChanged_eqSeekBar9(ZKSeekBar *pSeekBar, int progress) {
    //LOGD(" ProgressChanged eqSeekBar9 %d !!!\n", progress);
	uart::set_sound_effect(9, progress);
	s_effect[9].effect_vlaue = progress;
}

static bool onButtonClick_sys_back(ZKButton *pButton) {
    LOGD(" ButtonClick sys_back !!!\n");
    return false;
}
static bool onButtonClick_returnButton(ZKButton *pButton) {
    LOGD(" ButtonClick returnButton !!!\n");
	for (unsigned int i=0; i<TABLESIZE(seekbar); i++) {
		(*seekbar[i])->setProgress(SET_EFFECT_VLAUE);
		uart::set_sound_effect(i, SET_EFFECT_VLAUE);
		s_effect[i].effect_vlaue = SET_EFFECT_VLAUE;
	}
    return false;
}
