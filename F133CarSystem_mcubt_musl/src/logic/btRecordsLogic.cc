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
#include "Manager/ConfigManager.h"
#include "utils/Loading_icon.hpp"
#include "config.h"
#include "utils/BitmapHelper.h"

static const char* records_status_pic[] = {"bt_a/out.png", "bt_a/in.png", "bt_a/no.png"};

//旋转加载图标
static zkf::IconRotate iconRotate;

static bt_cb_t _s_bt_cb;

static void _bt_download_cb(bt_download_state_e state) {
	if (E_BT_DOWNLOAD_RECORD_DATA == state) {
		iconRotate.run();
	} else {			//E_BT_DOWNLOAD_RECORD_COMPLETED
		iconRotate.requestExit();
	}
	mrecordsListViewPtr->refreshListView();
}

static void _bt_add_cb() {
	_s_bt_cb.download_cb = _bt_download_cb;
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
	 iconRotate.SetCtrl(msyncPointerPtr, mscaningWindowPtr);
	_bt_add_cb();
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
	if (iconRotate.isRunning())
		iconRotate.requestExitAndWait();

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
static bool onbtRecordsActivityTouchEvent(const MotionEvent &ev) {
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

static int getListItemCount_recordsListView(const ZKListView *pListView) {
    //LOGD("getListItemCount_recordsListView !\n");
    return bt::get_record_size();
}

static void obtainListItemData_recordsListView(ZKListView *pListView,ZKListView::ZKListItem *pListItem, int index) {
    //LOGD(" obtainListItemData_ recordsListView  !!!\n");
	ZKListView::ZKListSubItem* nameSubItem = pListItem->findSubItemByID(ID_BTRECORDS_rNameSubItem);
	//ZKListView::ZKListSubItem* timeSubItem = pListItem->findSubItemByID(ID_BTRECORDS_rTimeSubItem);
	nameSubItem->setLongMode(ZKTextView::E_LONG_MODE_DOTS);

	bt_record_t record;
	bt::get_record_by_index(index, record);

	nameSubItem->setButtonStatusPic(ZK_CONTROL_STATUS_NORMAL, CONFIGMANAGER->getResFilePath(records_status_pic[record.type]).c_str());
	nameSubItem->setText(record.name == "" ? record.num : record.name);
	pListItem->setText(record.time);
}

static void onListItemClick_recordsListView(ZKListView *pListView, int index, int id) {
    //LOGD(" onListItemClick_ recordsListView  !!!\n");
	bt_record_t record;
	bt::get_record_by_index(index, record);

	Intent *pIntent = new Intent();
	pIntent->putExtra("number", record.num);
    EASYUICONTEXT->openActivity("btDialActivity", pIntent);
    EASYUICONTEXT->closeActivity("btRecordsActivity");
}

static bool onButtonClick_downloadButton(ZKButton *pButton) {
    LOGD(" ButtonClick downloadButton !!!\n");
    bt::download_call_record(BT_RECORD_RECENT);
    return false;
}
static bool onButtonClick_DeleteButton(ZKButton *pButton) {
    LOGD(" ButtonClick DeleteButton !!!\n");
    if (bt::get_record_size() <= 0) {
    	return false;
    }
    mDeleteTipsWindowPtr->showWnd();
    return false;
}

static bool onButtonClick_missButton(ZKButton *pButton) {
    LOGD(" ButtonClick missButton !!!\n");
    bt::download_call_record(BT_RECORD_MISSED);
    return false;
}

static bool onButtonClick_inButton(ZKButton *pButton) {
    LOGD(" ButtonClick inButton !!!\n");
    bt::download_call_record(BT_RECORD_INCOMING);
    return false;
}

static bool onButtonClick_outButton(ZKButton *pButton) {
    LOGD(" ButtonClick outButton !!!\n");
    bt::download_call_record(BT_RECORD_OUTGOING);
    return false;
}

static bool onButtonClick_queryMusicButton(ZKButton *pButton) {
    LOGD(" ButtonClick queryMusicButton !!!\n");
    EASYUICONTEXT->openActivity("btMusicActivity");
    EASYUICONTEXT->closeActivity("btRecordsActivity");
    EASYUICONTEXT->closeActivity("btContactsActivity");
    EASYUICONTEXT->closeActivity("btDialActivity");
    return false;
}

static bool onButtonClick_phoneButton(ZKButton *pButton) {
    LOGD(" ButtonClick phoneButton !!!\n");
    if (bt::is_calling()) {
    	EASYUICONTEXT->openActivity("callingActivity");
        EASYUICONTEXT->closeActivity("btDialActivity");
    } else {
        EASYUICONTEXT->openActivity("btDialActivity");
    }
    EASYUICONTEXT->closeActivity("btRecordsActivity");
    EASYUICONTEXT->closeActivity("btContactsActivity");
    EASYUICONTEXT->closeActivity("btMusicActivity");
    return false;
}

static bool onButtonClick_btrecordButton(ZKButton *pButton) {
    LOGD(" ButtonClick btrecordButton !!!\n");
    EASYUICONTEXT->closeActivity("btDialActivity");
    EASYUICONTEXT->closeActivity("btContactsActivity");
    EASYUICONTEXT->closeActivity("btMusicActivity");
    return false;
}

static bool onButtonClick_btcontactsButton(ZKButton *pButton) {
    LOGD(" ButtonClick btcontactsButton !!!\n");
    EASYUICONTEXT->closeActivity("btDialActivity");
    EASYUICONTEXT->closeActivity("btRecordsActivity");
    EASYUICONTEXT->openActivity("btContactsActivity");
    EASYUICONTEXT->closeActivity("btMusicActivity");
    return false;
}

static bool onButtonClick_btsettingButton(ZKButton *pButton) {
    LOGD(" ButtonClick btsettingButton !!!\n");
    EASYUICONTEXT->openActivity("btsettingActivity");
    EASYUICONTEXT->closeActivity("btRecordsActivity");
    EASYUICONTEXT->closeActivity("btContactsActivity");
    EASYUICONTEXT->closeActivity("btDialActivity");
    EASYUICONTEXT->closeActivity("btMusicActivity");
    return false;
}

static bool onButtonClick_cancelButton(ZKButton *pButton) {
    LOGD(" ButtonClick cancelButton !!!\n");
    mDeleteTipsWindowPtr->hideWnd();
    return false;
}

static bool onButtonClick_sureButton(ZKButton *pButton) {
    LOGD(" ButtonClick sureButton !!!\n");
    bt::delete_call_record();
    mrecordsListViewPtr->refreshListView();
    mDeleteTipsWindowPtr->hideWnd();
    return false;
}
