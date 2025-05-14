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

#include "bt/context.h"

static bt_cb_t _s_bt_cb;

static std::string _time_format(int time) {
	char buf[32] = { 0 };
	sprintf(buf, "%02d:%02d", time/60, time%60);
	return buf;
}

static void _update_music_info() {
	bt_music_t music_info = bt::get_music_info();
	mtittleTextViewPtr->setText(music_info.title);
	martistTextViewPtr->setText(music_info.artist);
	malbumTextViewPtr->setText(music_info.album);
	mlyricTextViewPtr->setText(music_info.lyric);
}

static void _update_music_progress() {
	bt_music_t music_info = bt::get_music_info();

	mmusicSeekBarPtr->setMax(music_info.duration);
	mmusicSeekBarPtr->setProgress(music_info.curpos);
	mcurposTextViewPtr->setText(_time_format(music_info.curpos));
	mdurationTextViewPtr->setText(_time_format(music_info.duration));
}


static void _bt_music_cb(bt_music_state_e state) {
	if (bt::music_is_playing()) {
		_update_music_info();
		_update_music_progress();
		mtittleTextViewPtr->setTextColor(0xFF00FF40);
		mplayButtonPtr->setSelected(true);
	} else {
		mtittleTextViewPtr->setTextColor(0xFFFFFFFF);
		mplayButtonPtr->setSelected(false);
	}
}

static void _bt_add_cb() {
	_s_bt_cb.music_cb = _bt_music_cb;
	bt::add_cb(&_s_bt_cb);
}

static void _bt_remove_cb() {
	bt::remove_cb(&_s_bt_cb);
}

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

	_bt_add_cb();
	mplayButtonPtr->setSelected(bt::music_is_playing());
	mtittleTextViewPtr->setTextColor(bt::music_is_playing() ? 0xFF00FF40 : 0xFFFFFFFF);
	_update_music_info();		// 音乐信息
	_update_music_progress();	// 进度信息
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
	_bt_remove_cb();
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
static bool onbtMusicActivityTouchEvent(const MotionEvent &ev) {
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
static bool onButtonClick_sys_back(ZKButton *pButton) {
    LOGD(" ButtonClick sys_back !!!\n");
    return false;
}

static void onProgressChanged_musicSeekBar(ZKSeekBar *pSeekBar, int progress) {
    //LOGD(" ProgressChanged musicSeekBar %d !!!\n", progress);
}

static bool onButtonClick_prevButton(ZKButton *pButton) {
    LOGD(" ButtonClick prevButton !!!\n");
    bt::music_prev();
    return false;
}

static bool onButtonClick_nextButton(ZKButton *pButton) {
    LOGD(" ButtonClick nextButton !!!\n");
    bt::music_next();
    return false;
}

static bool onButtonClick_playButton(ZKButton *pButton) {
    LOGD(" ButtonClick playButton !!!\n");
    bt::music_is_playing() ? bt::music_pause() : bt::music_play();
    return false;
}
