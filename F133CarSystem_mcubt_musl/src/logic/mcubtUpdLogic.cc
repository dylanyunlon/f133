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
#include "config.h"


static void _mcu_cb(MCU_EVENT event, void* data, uint32_t len) {
	switch (event) {
	case MCU_UPD_MODE_ENTRY:
		mversionTextViewPtr->setText("进入升级模式");
		mmcuUpdSeekBarPtr->setMax(0);
		mmcuUpdSeekBarPtr->setProgress(0);
		mupdateButtonPtr->setInvalid(false);
		mup_modeButtonPtr->setInvalid(true);
		break;
	case MCU_UPD_PRGGRESS: {
		mupdateButtonPtr->setInvalid(true);
		UpdProgress* updp =  (UpdProgress*)data;
		mmcuUpdSeekBarPtr->setMax(updp->total);
		if (updp->cur_pos > mmcuUpdSeekBarPtr->getProgress()) {
			mmcuUpdSeekBarPtr->setProgress(updp->cur_pos);
		}
	}
		break;
	case MCU_UPD_FINISHED:
		mmcuUpdSeekBarPtr->setProgress(mmcuUpdSeekBarPtr->getMax());
		mupdTextViewPtr->setVisible(true);
		mversionTextViewPtr->setText("升级完成!");
		mupdateButtonPtr->setInvalid(true);
		break;
	case MCU_VERSION:
		mversionTextViewPtr->setText(std::string((char*)data, len));
		break;
	}
}

static void UI_ctrl_init() {
	mmcuUpdSeekBarPtr->setProgress(0);
	mupdTextViewPtr->setVisible(false);
	mupdateButtonPtr->setInvalid(true);
	mup_modeButtonPtr->setInvalid(true);
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
	uart::add_mcubt_cb(_mcu_cb);
	UI_ctrl_init();
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
	uart::remove_mcubt_cb(_mcu_cb);
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
static bool onmcubtUpdActivityTouchEvent(const MotionEvent &ev) {
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

static bool onButtonClick_armButton(ZKButton *pButton) {
    LOGD(" ButtonClick armButton !!!\n");
    uart::set_sound_channel(SOUND_CHANNEL_ARM);
    return false;
}

static bool onButtonClick_btButton(ZKButton *pButton) {
    LOGD(" ButtonClick btButton !!!\n");
    uart::set_sound_channel(SOUND_CHANNEL_BT);
    return false;
}

static bool onButtonClick_fm_Button(ZKButton *pButton) {
    LOGD(" ButtonClick fm_Button !!!\n");
	uart::set_sound_channel(SOUND_CHANNEL_FM);
    return false;
}

static bool onButtonClick_aux_Button(ZKButton *pButton) {
    LOGD(" ButtonClick aux_Button !!!\n");
    uart::set_sound_channel(SOUND_CHANNEL_AUX);
    return false;
}

static bool onButtonClick_eqButton(ZKButton *pButton) {
    LOGD(" ButtonClick eqButton !!!\n");
    EASYUICONTEXT->openActivity("soundEffectActivity");
    return false;
}

static bool onButtonClick_fieldButton(ZKButton *pButton) {
    LOGD(" ButtonClick fieldButton !!!\n");
    EASYUICONTEXT->openActivity("soundFieldActivity");
    return false;
}

static bool onButtonClick_updateButton(ZKButton *pButton) {
    LOGD(" ButtonClick updateButton !!!\n");
    uart::start_mcu_update();
    return false;
}

static bool onButtonClick_up_modeButton(ZKButton *pButton) {
    LOGD(" ButtonClick up_modeButton !!!\n");
	uart::entry_update_mode();
    return false;
}

static bool onButtonClick_qVerButton(ZKButton *pButton) {
    LOGD(" ButtonClick qVerButton !!!\n");
    uart::query_mcubt_version();
    return false;
}

static void onProgressChanged_mcuUpdSeekBar(ZKSeekBar *pSeekBar, int progress) {
    //LOGD(" ProgressChanged mcuUpdSeekBar %d !!!\n", progress);
}

static bool onButtonClick_sys_back(ZKButton *pButton) {
    LOGD(" ButtonClick sys_back !!!\n");
    return false;
}

static bool onButtonClick_queryButton(ZKButton *pButton) {
    LOGD(" ButtonClick queryButton !!!\n");
    if (uart::query_update_file()) {
        mversionTextViewPtr->setText("有可用升级文件!");
        mup_modeButtonPtr->setInvalid(false);
    }
    else {
    	mversionTextViewPtr->setText("未发现升级文件");
    }
	mmcuUpdSeekBarPtr->setMax(0);
	mmcuUpdSeekBarPtr->setProgress(0);
    return false;
}
