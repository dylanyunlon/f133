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

#include <fy/handler.hpp>
#include <memory>
#include <fy/format.hpp>
#include "tire/tire_parse.h"

namespace {
enum Messages {
	E_TIRE_PRESSURE_INFO_UPDATE = 1,
};

typedef enum E_TIRE_POSITION {
	E_TIRE_FRONT,
	E_TIRE_REAR,
}TIRE_POSITION_E;

#define TIRE_MANAGER TireOptionManager::getInstance()

std::shared_ptr<fy::handler> handler;
TIRE_POSITION_E _s_tire_position = E_TIRE_FRONT;

void reflash_tire_option_info() {
	pressure_unit_e pressure_unit = TIRE_MANAGER.get_tire_pressure_unit();
	munitTextViewPtr->setText((pressure_unit == E_PRESSURE_BAR) ? "bar" : "psi");
	mhvTextViewPtr->setText((pressure_unit == E_PRESSURE_PSI) ? \
					fy::format("%d", TIRE_MANAGER.get_pressure_upper() / 100) : \
					fy::format("%.1f", TIRE_MANAGER.get_pressure_upper() / 100.0));
	mlvTextViewPtr->setText((pressure_unit == E_PRESSURE_PSI) ? \
					fy::format("%d", TIRE_MANAGER.get_pressure_lower() / 100) : \
					fy::format("%.1f", TIRE_MANAGER.get_pressure_lower() / 100.0));
	mCTextViewPtr->setText(TIRE_MANAGER.get_temp_limit());
}

void update_state_info(tire_dev_t& dev_info, ZKTextView* connectInfoPtr, ZKTextView* tempTextPtr,
											ZKTextView* pressurePtr, ZKTextView* tireStatePtr) {
	int pressure_upper = TIRE_MANAGER.get_pressure_upper();
	int pressure_lower = TIRE_MANAGER.get_pressure_lower();
	int temp_limit = TIRE_MANAGER.get_temp_limit();
	temp_unit_e temp_unit = TIRE_MANAGER.get_tire_temp_unit();
	pressure_unit_e pressure_unit = TIRE_MANAGER.get_tire_pressure_unit();
	bool is_normal_state = true;
	switch(dev_info.bind_state) {
	case E_BT_TIRE_BIND_STATE_UNMATCH:
		connectInfoPtr->setTextTr("UNMATCH");
		is_normal_state = false;
		break;
	case E_BT_TIRE_BIND_STATE_NOTFOUND:
		connectInfoPtr->setTextTr("NOTFOUND");
		is_normal_state = false;
		break;
	case E_BT_TIRE_BIND_STATE_CONNECTED:
		connectInfoPtr->setTextTr("CONNECTED");
		break;
	default:
		break;
	}
	if (pressure_unit == E_PRESSURE_PSI) {
		pressurePtr->setText(fy::format("%d psi", (int)dev_info.barometric));
	} else {
		pressurePtr->setText(fy::format("%.1f bar", (float)TIRE_MANAGER.convertPressure(dev_info.barometric * 10, E_PRESSURE_PSI, E_PRESSURE_BAR) / 10));
	}
	if (temp_unit == E_TEMP_CELSIUS) {
		tempTextPtr->setText(fy::format("%d ℃", dev_info.temperature));
	} else {
		tempTextPtr->setText(fy::format("%d ℉", TIRE_MANAGER.convertTemperature(dev_info.temperature, E_TEMP_CELSIUS, E_TEMP_FAHRENHEIT)));
	}
	if (TIRE_MANAGER.convertTemperature(dev_info.temperature, E_TEMP_CELSIUS, temp_unit) > temp_limit) {
		tempTextPtr->setTextColor(0xFF0000);
		is_normal_state = false;
	} else {
		tempTextPtr->setTextColor(0xFFFFFF);
	}
	int tmp = TIRE_MANAGER.convertPressure(dev_info.barometric * 100, E_PRESSURE_PSI, pressure_unit);
	LOGD("[tire data] pressure = %d, upper = %d, lower = %d", tmp, pressure_upper, pressure_lower);
	if ((tmp >= pressure_lower) && (tmp <= pressure_upper)) {
		pressurePtr->setTextColor(0xFFFFFF);
	} else {
		pressurePtr->setTextColor(0xFF0000);
		is_normal_state = false;
	}
	// TODO
	tireStatePtr->setTextTr(is_normal_state ? "normal" : "abnormal");
	tireStatePtr->setTextColor(is_normal_state ? 0xFFFFFF : 0xFF0000);
}

void reflash_tire_state_info() {
	tire_dev_t front_info;
	tire_dev_t rear_info;
	tire::get_front_tire_info(front_info);
	tire::get_rear_tire_info(rear_info);
	update_state_info(front_info, mfrontTextViewPtr, mfrontCViewPtr, mfrontpsiTextViewPtr, mfrontstateTextViewPtr);
	update_state_info(rear_info, mrearTextViewPtr, mrearCTextViewPtr, mrearpsiTextViewPtr, mrearstateTextViewPtr);
}

inline void pressure_state_update_delayed(int delay_time) {
    fy::message msg(E_TIRE_PRESSURE_INFO_UPDATE);
    handler->remove_messages(E_TIRE_PRESSURE_INFO_UPDATE);
    handler->send_message_delayed(msg, delay_time);
}

void handleMessage(const fy::message* msg, const void* user_data) {
	switch(msg->what) {
	case E_TIRE_PRESSURE_INFO_UPDATE: {
		reflash_tire_state_info();
	}
		break;
	default:
		break;
	}
}

void _tire_misc_callback(tire_misc_info_e info) {
	switch (info) {
	case E_MISC_INFO_TIRE_BIND_DEV_UPDATE: {
		pressure_state_update_delayed(300);
	}
	break;
	case E_MISC_INFO_TIRE_DEV_MAP_UPDATE: {
		mtireMatchListViewPtr->refreshListView();
	}
	break;
	default:
		break;
	}
}

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
	reflash_tire_state_info();
	reflash_tire_option_info();
	tire::add_tire_info_update_cb(_tire_misc_callback);
	handler = std::make_shared<fy::handler>(handleMessage, (void*) NULL);
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
	tire::remove_tire_info_update_cb(_tire_misc_callback);
	handler.reset();
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
static bool ontirePressureActivityTouchEvent(const MotionEvent &ev) {
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
static bool onButtonClick_clearButton(ZKButton *pButton) {
    LOGD(" ButtonClick clearButton !!!\n");
    tire::del_match_front_tire_dev();
    tire::del_match_rear_tire_dev();
    pressure_state_update_delayed(400);
    mtireMatchListViewPtr->setVisible(false);
    return false;
}

static bool onButtonClick_unitprevButton(ZKButton *pButton) {
    LOGD(" ButtonClick unitprevButton !!!\n");
    TIRE_MANAGER.set_tire_pressure_unit(pressure_unit_e((TIRE_MANAGER.get_tire_pressure_unit() + E_PRESSURE_MAX - 1) % E_PRESSURE_MAX));
    reflash_tire_option_info();
    pressure_state_update_delayed(400);
    return false;
}

static bool onButtonClick_unitnextButton(ZKButton *pButton) {
    LOGD(" ButtonClick unitnextButton !!!\n");
    TIRE_MANAGER.set_tire_pressure_unit(pressure_unit_e((TIRE_MANAGER.get_tire_pressure_unit() + 1) % E_PRESSURE_MAX));
    reflash_tire_option_info();
    pressure_state_update_delayed(400);
    return false;
}

static bool onButtonClick_hvprevButton(ZKButton *pButton) {
    LOGD(" ButtonClick hvprevButton !!!\n");
    TIRE_MANAGER.dec_pressure_upper();
    reflash_tire_option_info();
    pressure_state_update_delayed(400);
    return false;
}

static bool onButtonClick_hvnextButton(ZKButton *pButton) {
    LOGD(" ButtonClick hvnextButton !!!\n");
    TIRE_MANAGER.add_pressure_upper();
    reflash_tire_option_info();
    pressure_state_update_delayed(400);
    return false;
}

static bool onButtonClick_lvprevButton(ZKButton *pButton) {
    LOGD(" ButtonClick lvprevButton !!!\n");
    TIRE_MANAGER.dec_pressure_lower();
    reflash_tire_option_info();
    pressure_state_update_delayed(400);
    return false;
}

static bool onButtonClick_lvnextButton(ZKButton *pButton) {
    LOGD(" ButtonClick lvnextButton !!!\n");
    TIRE_MANAGER.add_pressure_lower();
    reflash_tire_option_info();
    pressure_state_update_delayed(400);
    return false;
}

static bool onButtonClick_CprevButton(ZKButton *pButton) {
    LOGD(" ButtonClick CprevButton !!!\n");
    TIRE_MANAGER.dec_temp_limit();
    reflash_tire_option_info();
    pressure_state_update_delayed(400);
    return false;
}

static bool onButtonClick_CnextButton(ZKButton *pButton) {
    LOGD(" ButtonClick CnextButton !!!\n");
    TIRE_MANAGER.add_temp_limit();
    reflash_tire_option_info();
    pressure_state_update_delayed(400);
    return false;
}

static bool onButtonClick_resetButton(ZKButton *pButton) {
    LOGD(" ButtonClick resetButton !!!\n");
    TIRE_MANAGER.reset_option();
    reflash_tire_option_info();
    pressure_state_update_delayed(400);
    return false;
}

static bool onButtonClick_frontButton(ZKButton *pButton) {
    LOGD(" ButtonClick frontButton !!!\n");
    _s_tire_position = E_TIRE_FRONT;
    mtireMatchListViewPtr->setVisible(true);
    return false;
}

static bool onButtonClick_rearButton(ZKButton *pButton) {
    LOGD(" ButtonClick rearButton !!!\n");
    _s_tire_position = E_TIRE_REAR;
    mtireMatchListViewPtr->setVisible(true);
    return false;
}
static int getListItemCount_tireMatchListView(const ZKListView *pListView) {
    //LOGD("getListItemCount_tireMatchListView !\n");
    return tire::get_scan_tire_dev_sise();
}

static void obtainListItemData_tireMatchListView(ZKListView *pListView,ZKListView::ZKListItem *pListItem, int index) {
    //LOGD(" obtainListItemData_ tireMatchListView  !!!\n");
	tire_dev_t tire_info;
	if (tire::get_scan_tire_dev_by_index(index, tire_info)) {
		pListItem->setText(fy::format("%02X %02X %02X", tire_info.addr[0], tire_info.addr[1], tire_info.addr[2]));
	}
}

static void onListItemClick_tireMatchListView(ZKListView *pListView, int index, int id) {
    //LOGD(" onListItemClick_ tireMatchListView  !!!\n");
	tire_dev_t tire_info;
	if (tire::get_scan_tire_dev_by_index(index, tire_info)) {
		if (_s_tire_position == E_TIRE_FRONT) {
			tire::match_front_tire_dev(tire_info);
		} else {
			tire::match_rear_tire_dev(tire_info);
		}
	}
	mtireMatchListViewPtr->setVisible(false);
	pressure_state_update_delayed(400);
}
static bool onButtonClick_saveDataButton(ZKButton *pButton) {
    LOGD(" ButtonClick saveDataButton !!!\n");
    tire::save_tire_data();
    return false;
}

static bool onButtonClick_saveOptionButton(ZKButton *pButton) {
    LOGD(" ButtonClick saveOptionButton !!!\n");
    TIRE_MANAGER.save_option();
    return false;
}
static bool onButtonClick_sys_back(ZKButton *pButton) {
    LOGD(" ButtonClick sys_back !!!\n");
    return false;
}
