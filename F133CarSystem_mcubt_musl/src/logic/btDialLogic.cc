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

static std::string dial_munber_str;

static void put_number_str(const std::string input) {
	if (input == "del") {
		if (dial_munber_str.length() == 0) {
			return;
		}
		dial_munber_str.erase(dial_munber_str.end() - 1);
	}
	else {
		if (dial_munber_str.length() > 20) {
			mLongTipsWindowPtr->showWnd();
			return;
		}
		dial_munber_str += input;
	}
	mnumTextViewPtr->setText(dial_munber_str);
}

namespace {

class LongClickListener : public ZKBase::ILongClickListener {
public:
	virtual void onLongClick(ZKBase *pBase) {
		dial_munber_str = "";
		mnumTextViewPtr->setText(dial_munber_str);
	}
};

}
static LongClickListener delLongClickListener;
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
	dial_munber_str = "";
	mnumTextViewPtr->setText(dial_munber_str);
	mdelButtonPtr->setLongClickListener(&delLongClickListener);
}

/**
 * 当切换到该界面时触发
 */
static void onUI_intent(const Intent *intentPtr) {
    if (intentPtr != NULL) {
        //TODO
    	dial_munber_str = intentPtr->getExtra("number");
    	mnumTextViewPtr->setText(dial_munber_str);
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
	if (dial_munber_str != "") {
		dial_munber_str = "";
		mnumTextViewPtr->setText(dial_munber_str);
	}
}

/*
 * 当界面完全退出时触发
 */
static void onUI_quit() {
	dial_munber_str = "";
	mdelButtonPtr->setLongClickListener(NULL);
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
static bool onbtDialActivityTouchEvent(const MotionEvent &ev) {
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
static bool onButtonClick_dialButton(ZKButton *pButton) {
    LOGD(" ButtonClick dialButton !!!\n");
    if(mnumTextViewPtr->getText().empty()) { // 电话回拨功能，需要同步通话记录后才能回拨
    	bt_record_t record;
    	bt::get_record_by_index(0, record);
    	dial_munber_str = record.num;
    	mnumTextViewPtr->setText(record.num);
    } else {
        bt::dial(mnumTextViewPtr->getText().c_str());
        dial_munber_str = "";
        mnumTextViewPtr->setText("");
    }
    return false;
}

static bool onButtonClick_key1Button(ZKButton *pButton) {
    LOGD(" ButtonClick key1Button !!!\n");
    put_number_str("1");
    return false;
}

static bool onButtonClick_key2Button(ZKButton *pButton) {
    LOGD(" ButtonClick key2Button !!!\n");
    put_number_str("2");
    return false;
}

static bool onButtonClick_key3Button(ZKButton *pButton) {
    LOGD(" ButtonClick key3Button !!!\n");
    put_number_str("3");
    return false;
}

static bool onButtonClick_key4Button(ZKButton *pButton) {
    LOGD(" ButtonClick key4Button !!!\n");
    put_number_str("4");
    return false;
}

static bool onButtonClick_key5Button(ZKButton *pButton) {
    LOGD(" ButtonClick key5Button !!!\n");
    put_number_str("5");
    return false;
}

static bool onButtonClick_key6Button(ZKButton *pButton) {
    LOGD(" ButtonClick key6Button !!!\n");
    put_number_str("6");
    return false;
}

static bool onButtonClick_key7Button(ZKButton *pButton) {
    LOGD(" ButtonClick key7Button !!!\n");
    put_number_str("7");
    return false;
}

static bool onButtonClick_key8Button(ZKButton *pButton) {
    LOGD(" ButtonClick key8Button !!!\n");
    put_number_str("8");
    return false;
}

static bool onButtonClick_key9Button(ZKButton *pButton) {
    LOGD(" ButtonClick key9Button !!!\n");
    put_number_str("9");
    return false;
}

static bool onButtonClick_key0Button(ZKButton *pButton) {
    LOGD(" ButtonClick key0Button !!!\n");
    put_number_str("0");
    return false;
}

static bool onButtonClick_keyJINGButton(ZKButton *pButton) {
    LOGD(" ButtonClick keyJINGButton !!!\n");
    put_number_str("#");
    return false;
}

static bool onButtonClick_keyXINGButton(ZKButton *pButton) {
    LOGD(" ButtonClick keyXINGButton !!!\n");
    put_number_str("*");
    return false;
}

static bool onButtonClick_delButton(ZKButton *pButton) {
    LOGD(" ButtonClick delButton !!!\n");
    put_number_str("del");
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
    EASYUICONTEXT->closeActivity("btContactsActivity");
    EASYUICONTEXT->closeActivity("btRecordslActivity");
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
