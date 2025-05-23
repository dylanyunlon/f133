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
#include "utils/Loading_icon.hpp"

static bt_cb_t _s_bt_cb;

namespace {

class ListTouch :public ZKBase::ITouchListener {
public:
	virtual void onTouchEvent(ZKBase *pBase, const MotionEvent &ev) {
		switch (ev.mActionStatus) {
		case MotionEvent::E_ACTION_DOWN:
			mindexTipPtr->setVisible(false);
			mcharJumpPtr->refreshListView();
			break;
		case MotionEvent::E_ACTION_NONE:
		case MotionEvent::E_ACTION_UP:
		case MotionEvent::E_ACTION_MOVE:
		case MotionEvent::E_ACTION_CANCEL:
			break;
		}
	}
};

}

//旋转加载图标
static zkf::IconRotate iconRotate;

static ListTouch sListTouch;

static void _bt_download_cb(bt_download_state_e state) {
	if (E_BT_DOWNLOAD_CONTACT_DATA == state) {
		iconRotate.run();
	} else {			//E_BT_DOWNLOAD_RECORD_COMPLETED
		iconRotate.requestExit();
	}
	mcontactsListViewPtr->refreshListView();
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
	mindexTipPtr->setVisible(false);
	_bt_add_cb();
	mcontactsListViewPtr->setTouchListener(&sListTouch);
	iconRotate.SetCtrl(msyncPointerPtr, mscaningWindowPtr);
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
	mcontactsListViewPtr->setTouchListener(NULL);
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
static bool onbtContactsActivityTouchEvent(const MotionEvent &ev) {
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

static int getListItemCount_contactsListView(const ZKListView *pListView) {
    //LOGD("getListItemCount_contactsListView !\n");
	if (bt::get_contact_size() <= 0) {
		mnumberTextViewPtr->setText("");
	} else {
		mnumberTextViewPtr->setText((int)bt::get_contact_size());
	}
	return bt::get_contact_size();
}
static void obtainListItemData_contactsListView(ZKListView *pListView,ZKListView::ZKListItem *pListItem, int index) {
    //LOGD(" obtainListItemData_ contactsListView  !!!\n");
	pListItem->setLongMode(ZKTextView::E_LONG_MODE_DOTS);

	bt_contact_t contact;
	bt::get_contact_by_index(index, contact);

	pListItem->setText(contact.name == "" ? contact.num : contact.name);
}
static void onListItemClick_contactsListView(ZKListView *pListView, int index, int id) {
    //LOGD(" onListItemClick_ contactsListView  !!!\n");
	bt_contact_t contact;
	bt::get_contact_by_index(index, contact);

	Intent *pIntent = new Intent();
	pIntent->putExtra("number", contact.num);
    EASYUICONTEXT->openActivity("btDialActivity", pIntent);
    EASYUICONTEXT->closeActivity("btContactsActivity");
}

static int getListItemCount_charJump(const ZKListView *pListView) {
    //LOGD("getListItemCount_charJump !\n");
	return 27;
}
static void obtainListItemData_charJump(ZKListView *pListView,ZKListView::ZKListItem *pListItem, int index) {
    //LOGD(" obtainListItemData_ charJump  !!!\n");
	if (index == 0) {
		pListItem->setText("#");
	} else if (index <= 26) {
		char ch = 'A' + index - 1;
		pListItem->setText(ch);
	}
	pListItem->setSelected(mindexTipPtr->isVisible() && mindexTipPtr->getText() == pListItem->getText());
}
static void onListItemClick_charJump(ZKListView *pListView, int index, int id) {
    //LOGD(" onListItemClick_ charJump  !!!\n");

}

static bool onButtonClick_downloadButton(ZKButton *pButton) {
    LOGD(" ButtonClick downloadButton !!!\n");
    bt::download_phone_book();
    return false;
}

static void onProgressChanged_charSeekBar(ZKSeekBar *pSeekBar, int progress) {
    //LOGD(" ProgressChanged charSeekBar %d !!!\n", progress);
	int index = (pSeekBar->getMax() - progress) / 4;

	LOGD("--%s-- --index(%d) = Max(%d) - progress(%d) \n", __FUNCTION__, index, pSeekBar->getMax(), progress);

	if (index == 0) {
		mcontactsListViewPtr->setSelection(0);
		mindexTipPtr->setText("#");
	}
	else {
		int ret = bt::find_first_contact_by_initial('A' + index - 1);
		if (ret >= 0) {
			mcontactsListViewPtr->setSelection(ret);
		}
		mindexTipPtr->setText((char)('A' + index - 1));
	}
	mindexTipPtr->setVisible(true);
	mcharJumpPtr->refreshListView();
}
static bool onButtonClick_deleteButton(ZKButton *pButton) {
    LOGD(" ButtonClick deleteButton !!!\n");
    if (bt::get_contact_size() == 0) {
    	return false;
    }
    mDeleteTipsWindowPtr->showWnd();
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
    EASYUICONTEXT->openActivity("btRecordsActivity");
    EASYUICONTEXT->closeActivity("btContactsActivity");
    EASYUICONTEXT->closeActivity("btMusicActivity");
    return false;
}

static bool onButtonClick_btcontactsButton(ZKButton *pButton) {
    LOGD(" ButtonClick btcontactsButton !!!\n");
    EASYUICONTEXT->closeActivity("btDialActivity");
    EASYUICONTEXT->closeActivity("btRecordsActivity");
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
    bt::delete_phone_book();
    mcontactsListViewPtr->refreshListView();
    mDeleteTipsWindowPtr->hideWnd();
    return false;
}
