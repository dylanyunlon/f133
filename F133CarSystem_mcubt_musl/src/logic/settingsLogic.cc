#pragma once
#include "uart/ProtocolSender.h"
#include "system/setting.h"
#include "os/SystemProperties.h"
#include "manager/LanguageManager.h"
#include "utils/TimeHelper.h"
#include "system/usb_monitor.h"
#include "bt/context.h"
#include "link/context.h"
#include "media/media_context.h"
#include "media/audio_context.h"
#include "utils/BrightnessHelper.h"
#include "fy/date.hpp"
#include "fy/format.hpp"
#include "uart/context.h"
#include "version.h"
#include "misc/storage.h"
#include "utils/BitmapHelper.h"
#include "manager/ConfigManager.h"

#define DEFAULT_BT_NAME "BT_BOX"
#define TIMEBIGSIZE 	45 	//时间设置中间字体大小
#define TIMESMALLSIZE 	30	//时间设置两边字体大小

#define UPDATE_DAY 1
//起始年份
#define ORG_YEAR 1970
//年份总个数
#define YEAR_NUM 67

static const char* systemTextTab[] = {"language","data","wifi","Debug","version"};

static const char* languaueTextTab[] = {"简体中文", "English(US)"};


static std::string adjustText[] = {
	"luminance","hue","saturation","contrast"
};
static ZKWindow **systemwndtab[] = {&mLanguageWndPtr,&mSetdataWndPtr,NULL,NULL,&mversionWindowPtr};


typedef enum {
	LANGUAGE_E,		// 语言
	DATA_E,			// 时间日期
	WIFI_E,			// wifi
	ADB_E,			// adb
	SYSTEM_E,		// 系统版本
}SYSTEMSHOWTYPE;

typedef enum {
	CH_E,//中文
	US_E,//英文
}LANGUAGETYPE;


static LANGUAGETYPE s_language_type;
static SYSTEMSHOWTYPE s_system_type;
static struct tm ptm;    //日期时间结构体
static bool ThreeFalg = false;

static void Get_CurrentCode(const std::string Code){
	if(Code == "zh_CN"){
		s_language_type = CH_E;
	}else if(Code == "en_US"){
		s_language_type = US_E;
	}
}

static void Update_CurrentCode(LANGUAGETYPE type){
	std::string Code;
	switch(type){
	case CH_E:
		Code = "zh_CN";
		break;
	case US_E:
		Code = "en_US";
		break;
	default:
		break;
	}
	sys::setting::update_localescode(Code.c_str());
}

/*
 * 初始化时间列表
 */
static void InitTimeList() {
    struct tm *t = TimeHelper::getDateTime();
	int year = t->tm_year + 1900;
	int mon = t->tm_mon + 1;
	int day = t->tm_mday;
	int hour = t->tm_hour;
	int min = t->tm_min;
	year - 1970 <= 0 ? myearListViewPtr->setSelection(YEAR_NUM-1) : myearListViewPtr->setSelection(year - 1971);
	mon - 2 < 0 ? mmonthListViewPtr->setSelection(11) : mmonthListViewPtr->setSelection(mon - 2);
	day - 2 < 0 ? mdayListViewPtr->setSelection(fy::date::get_days(day, mon) - 1) : mdayListViewPtr->setSelection(day - 2);
	hour == 0 ? mhourListViewPtr->setSelection(23) : mhourListViewPtr->setSelection(hour - 1);
	min == 0 ? mminListViewPtr->setSelection(59) : mminListViewPtr->setSelection(min - 1);
	ThreeFalg = true;
}

static void set_dayList() {
	const int firstVisible = mdayListViewPtr->getFirstVisibleItemIndex();
	int sel = ptm.tm_mday == 1 ? mdayListViewPtr->getListItemCount()-1 : fy::date::get_days(ptm.tm_year, ptm.tm_mon + 1) - 2;
	LOGD("--%d-- --%s-- mon:%d sel:%d\n", __LINE__, __FILE__, ptm.tm_mon, sel);
	sel = (0 < firstVisible && firstVisible < sel)? firstVisible : sel;
	LOGD("--%d-- --%s-- firstVisible:%d mon:%d sel:%d\n", __LINE__, __FILE__, firstVisible, ptm.tm_mon, sel);
	//对齐位置
	mdayListViewPtr->setSelection(sel);
	//刷新列表
	mdayListViewPtr->refreshListView();
}

static void updateDay(void) {
	mActivityPtr->unregisterUserTimer(UPDATE_DAY);
	mActivityPtr->registerUserTimer(UPDATE_DAY, 20);
}

static void _mcu_cb(MCU_EVENT event, void* data, uint32_t len) {
	std::string txt;
	switch (event) {
	case MCU_VERSION:
		txt = fy::format("MCU: %s", std::string((char*)data, len).c_str());
		mmcuTextViewPtr->setText(txt);
		break;
	}
}

static void systemWnd(bool show) {
	msystemListViewPtr->setVisible(show);
	mresetButtonPtr->setVisible(show);
	mLanguageWndPtr->setVisible(!show);
	mSetdataWndPtr->setVisible(!show);
	mversionWindowPtr->setVisible(!show);
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
	Get_CurrentCode(LANGUAGEMANAGER->getCurrentCode());

	mhourButton12Ptr->setSelected(!sys::setting::is_time_format_24h());
	mhourButton24Ptr->setSelected(sys::setting::is_time_format_24h());

	uart::add_mcubt_cb(_mcu_cb);
	systemWnd(true);
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
	case UPDATE_DAY: {
		if(!ThreeFalg) {
			set_dayList();
		}
		return false;
	}
	break;
		default:
			break;
	}
    return true;
}

static void sonCheck_dateListIsMove(const MotionEvent &ev){
	switch (ev.mActionStatus) {
		case MotionEvent::E_ACTION_DOWN://触摸按下
			break;
		case MotionEvent::E_ACTION_MOVE://触摸滑动
		case MotionEvent::E_ACTION_UP:  //触摸抬起
			if(mSetdataWndPtr->isVisible()) {
				ThreeFalg = false;
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
static bool onsettingsActivityTouchEvent(const MotionEvent &ev) {
    switch (ev.mActionStatus) {
		case MotionEvent::E_ACTION_DOWN://触摸按下
			//LOGD("时刻 = %ld 坐标  x = %d, y = %d", ev.mEventTime, ev.mX, ev.mY);
			break;
		case MotionEvent::E_ACTION_MOVE://触摸滑动
			sonCheck_dateListIsMove(ev);
			break;
		case MotionEvent::E_ACTION_UP:  //触摸抬起
			break;
			break;
		default:
			break;
	}
	return false;

}
static int getListItemCount_LanguageListView(const ZKListView *pListView) {
    //LOGD("getListItemCount_LanguageListView !\n");
	return TABLESIZE(languaueTextTab);
}

static void obtainListItemData_LanguageListView(ZKListView *pListView,ZKListView::ZKListItem *pListItem, int index) {
    //LOGD(" obtainListItemData_ LanguageListView  !!!\n");
	ZKListView::ZKListSubItem* languagePicSub = pListItem->findSubItemByID(ID_SETTINGS_languagePicSub);
	ZKListView::ZKListSubItem* languageTextSub = pListItem->findSubItemByID(ID_SETTINGS_languageTextSub);
	languageTextSub->setText(languaueTextTab[index]);
	languagePicSub->setSelected(s_language_type == index);
	languageTextSub->setSelected(s_language_type == index);
}

static void onListItemClick_LanguageListView(ZKListView *pListView, int index, int id) {
    //LOGD(" onListItemClick_ LanguageListView  !!!\n");
	s_language_type = (LANGUAGETYPE)index;
	Update_CurrentCode(s_language_type);
}

static int getListItemCount_hourListView(const ZKListView *pListView) {
    //LOGD("getListItemCount_hourListView !\n");
	return 24;
}

static void obtainListItemData_hourListView(ZKListView *pListView,ZKListView::ZKListItem *pListItem, int index) {
    //LOGD(" obtainListItemData_ hourListView  !!!\n");
	char buf [10]={0};
	snprintf(buf,sizeof(buf),"%02d",index);
	pListItem->setText(buf);
	pListItem->setTextColor(index == (pListView->getFirstVisibleItemIndex()+1) % pListView->getListItemCount() ? 0xFFFFFF : 0x9F9F9F);
	pListItem->setTextSize(index == (pListView->getFirstVisibleItemIndex()+1)%pListView->getListItemCount() ? TIMEBIGSIZE : TIMESMALLSIZE);
	//保存选中项日期
	if(index == (pListView->getFirstVisibleItemIndex()+1) % pListView->getListItemCount()) {
		ptm.tm_hour = index;
		sys::setting::set_DateTime(&ptm);
	}
}

static void onListItemClick_hourListView(ZKListView *pListView, int index, int id) {
    //LOGD(" onListItemClick_ hourListView  !!!\n");
	pListView->setSelection(((0 == index) ? pListView->getListItemCount() : index)-1);
}

static int getListItemCount_minListView(const ZKListView *pListView) {
    //LOGD("getListItemCount_minListView !\n");
	return 60;
}

static void obtainListItemData_minListView(ZKListView *pListView,ZKListView::ZKListItem *pListItem, int index) {
    //LOGD(" obtainListItemData_ minListView  !!!\n");
	char minBuf[10] = {0};
	sprintf(minBuf,"%02d",index);    //设置显示内容
	pListItem->setText(minBuf);
	pListItem->setTextColor(index == (pListView->getFirstVisibleItemIndex()+1) % pListView->getListItemCount() ? 0xFFFFFF : 0x9F9F9F);
	pListItem->setTextSize(index == (pListView->getFirstVisibleItemIndex()+1)%pListView->getListItemCount() ? TIMEBIGSIZE : TIMESMALLSIZE);
	//保存选中项日期
	if(index == (pListView->getFirstVisibleItemIndex()+1) % pListView->getListItemCount()) {
		ptm.tm_min = index;
		sys::setting::set_DateTime(&ptm);
	}
}

static void onListItemClick_minListView(ZKListView *pListView, int index, int id) {
    //LOGD(" onListItemClick_ minListView  !!!\n");
	pListView->setSelection(((0 == index) ? pListView->getListItemCount() : index)-1);
}

static int getListItemCount_yearListView(const ZKListView *pListView) {
    //LOGD("getListItemCount_yearListView !\n");
	return YEAR_NUM;
}

static void obtainListItemData_yearListView(ZKListView *pListView,ZKListView::ZKListItem *pListItem, int index) {
    //LOGD(" obtainListItemData_ yearListView  !!!\n");
	pListItem->setText(index + ORG_YEAR);
	pListItem->setTextColor(index == (pListView->getFirstVisibleItemIndex()+1) % pListView->getListItemCount() ? 0xFFFFFF : 0x9F9F9F);
	pListItem->setTextSize(index == (pListView->getFirstVisibleItemIndex()+1)%pListView->getListItemCount() ? TIMEBIGSIZE : TIMESMALLSIZE);
	if(index == (pListView->getFirstVisibleItemIndex()+1) % pListView->getListItemCount()) {
		ptm.tm_year = atoi(pListItem->getText().c_str()) - 1900;
		sys::setting::set_DateTime(&ptm);
	}
	updateDay();
}

static void onListItemClick_yearListView(ZKListView *pListView, int index, int id) {
    //LOGD(" onListItemClick_ yearListView  !!!\n");
	pListView->setSelection(((0 == index)? pListView->getListItemCount() : index)-1);
}

static int getListItemCount_monthListView(const ZKListView *pListView) {
    //LOGD("getListItemCount_monthListView !\n");
	return 12;
}

static void obtainListItemData_monthListView(ZKListView *pListView,ZKListView::ZKListItem *pListItem, int index) {
    //LOGD(" obtainListItemData_ monthListView  !!!\n");
	char buf[10] = {0};
	sprintf(buf, "%02d", index+1);
	pListItem->setText(buf);
	pListItem->setTextColor(index == (pListView->getFirstVisibleItemIndex()+1) % pListView->getListItemCount() ? 0xFFFFFF : 0x9F9F9F);
	pListItem->setTextSize(index == (pListView->getFirstVisibleItemIndex()+1)%pListView->getListItemCount() ? TIMEBIGSIZE : TIMESMALLSIZE);
	//保存选中项月份
	if(index == (pListView->getFirstVisibleItemIndex()+1)%pListView->getListItemCount()) {
		ptm.tm_mon = atoi(pListItem->getText().c_str()) - 1;
		sys::setting::set_DateTime(&ptm);
	}
	updateDay();
}

static void onListItemClick_monthListView(ZKListView *pListView, int index, int id) {
    //LOGD(" onListItemClick_ monthListView  !!!\n");
	pListView->setSelection(((0 == index)? pListView->getListItemCount() : index)-1);
}

static int getListItemCount_dayListView(const ZKListView *pListView) {
    //LOGD("getListItemCount_dayListView !\n");
	return fy::date::get_days(ptm.tm_year, ptm.tm_mon + 1);
}



static void obtainListItemData_dayListView(ZKListView *pListView,ZKListView::ZKListItem *pListItem, int index) {
    //LOGD(" obtainListItemData_ dayListView  !!!\n");
	char buf[32] = {0};
	sprintf(buf,"%02d",index+1);
	pListItem->setText(buf);
	pListItem->setTextSize(index == (pListView->getFirstVisibleItemIndex()+1)%pListView->getListItemCount() ? TIMEBIGSIZE : TIMESMALLSIZE);
	pListItem->setTextColor(index == (pListView->getFirstVisibleItemIndex()+1)%pListView->getListItemCount() ? 0xFFFFFF : 0x9F9F9F);
	//保存选中项日期
	if(index == (pListView->getFirstVisibleItemIndex()+1)%pListView->getListItemCount()) {
		ptm.tm_mday = index+1;
		sys::setting::set_DateTime(&ptm);
	}
}

static void onListItemClick_dayListView(ZKListView *pListView, int index, int id) {
    //LOGD(" onListItemClick_ dayListView  !!!\n");
	pListView->setSelection(((0 == index)? pListView->getListItemCount() : index)-1);
}

static int getListItemCount_systemListView(const ZKListView *pListView) {
    //LOGD("getListItemCount_systemListView !\n");
	return TABLESIZE(systemTextTab);
}

static void obtainListItemData_systemListView(ZKListView *pListView,ZKListView::ZKListItem *pListItem, int index) {
    //LOGD(" obtainListItemData_ systemListView  !!!\n");
	ZKListView::ZKListSubItem* systemSubText = pListItem->findSubItemByID(ID_SETTINGS_systemSubText);
	ZKListView::ZKListSubItem* SubItemSysMore = pListItem->findSubItemByID(ID_SETTINGS_SubItemSysMore);
	ZKListView::ZKListSubItem* SubItemAdb = pListItem->findSubItemByID(ID_SETTINGS_SubItemAdb);
	SubItemSysMore->setVisible(index != 3);
	SubItemAdb->setVisible(index == 3);
	SubItemAdb->setSelected(sys::setting::is_usb_adb_enabled());
	systemSubText->setTextTr(systemTextTab[index]);
}

static void onListItemClick_systemListView(ZKListView *pListView, int index, int id) {
    //LOGD(" onListItemClick_ systemListView  !!!\n");
	s_system_type = (SYSTEMSHOWTYPE)index;
	std::string txt;
	std::string apptxt;
	switch (s_system_type) {
	case LANGUAGE_E:		// 语言
		msystemListViewPtr->setVisible(false);
		mresetButtonPtr->setVisible(false);
		(*systemwndtab[LANGUAGE_E])->setVisible(true);
		break;
	case DATA_E:			// 时间日期
		InitTimeList();
		msystemListViewPtr->setVisible(false);
		mresetButtonPtr->setVisible(false);
		(*systemwndtab[DATA_E])->setVisible(true);
		break;
	case WIFI_E:			// wifi
		EASYUICONTEXT->openActivity("wifisettingActivity");
		break;
	case ADB_E:				// adb
		sys::setting::enable_usb_adb(!sys::setting::is_usb_adb_enabled());
		break;
	case SYSTEM_E:			// version
		char version[32];
		uart::query_mcubt_version();
		SystemProperties::getString("ro.build.date", version, "");
		txt = fy::format("OS: %s", version);
		apptxt = fy::format("APP: V%d.%d.%d",APP_MAJOR, APP_MINOR, APP_PATCH);
		mosTextViewPtr->setText(txt);
		mappTextViewPtr->setText(apptxt);

		msystemListViewPtr->setVisible(false);
		mresetButtonPtr->setVisible(false);
		(*systemwndtab[SYSTEM_E])->setVisible(true);
		break;
	}
}

static bool onButtonClick_resetButton(ZKButton *pButton) {
    LOGD(" ButtonClick resetButton !!!\n");
    mresetWindowPtr->showWnd();
    return false;
}

static bool onButtonClick_resetConfirmButton(ZKButton *pButton) {
    LOGD(" ButtonClick resetConfirmButton !!!\n");
    bt::disconnect();
    bt::clear_match_device();
    bt::modify_name(DEFAULT_BT_NAME);
    bt::set_auto_answer(false);
    bt::set_auto_connect(false);
    storage::clear();
    system("rm -rf /data/*");
    usleep(1000);
    system("reboot");
    return false;
}

static bool onButtonClick_resetCancelButton(ZKButton *pButton) {
    LOGD(" ButtonClick resetCancelButton !!!\n");
    mresetWindowPtr->hideWnd();
    return false;
}

static bool onButtonClick_hourButton12(ZKButton *pButton) {
    LOGD(" ButtonClick hourButton12 !!!\n");
    sys::setting::set_time_format_24h(false);
    mhourButton24Ptr->setSelected(false);
    pButton->setSelected(!sys::setting::is_time_format_24h());
    return false;
}

static bool onButtonClick_hourButton24(ZKButton *pButton) {
    LOGD(" ButtonClick hourButton24 !!!\n");
    sys::setting::set_time_format_24h(true);
    mhourButton12Ptr->setSelected(false);
    pButton->setSelected(sys::setting::is_time_format_24h());
    return false;
}

static bool onButtonClick_showButton(ZKButton *pButton) {
    LOGD(" ButtonClick showButton !!!\n");
    EASYUICONTEXT->openActivity("setshowActivity");
    EASYUICONTEXT->closeActivity("setlinkActivity");
    EASYUICONTEXT->closeActivity("setvoiceActivity");
    EASYUICONTEXT->closeActivity("setreverseActivity");
    EASYUICONTEXT->closeActivity("setfactoryActivity");
    EASYUICONTEXT->closeActivity("settingActivity");
    return false;
}

static bool onButtonClick_linkButton(ZKButton *pButton) {
    LOGD(" ButtonClick linkButton !!!\n");
    EASYUICONTEXT->closeActivity("setshowActivity");
    EASYUICONTEXT->openActivity("setlinkActivity");
    EASYUICONTEXT->closeActivity("setvoiceActivity");
    EASYUICONTEXT->closeActivity("setreverseActivity");
    EASYUICONTEXT->closeActivity("setfactoryActivity");
    EASYUICONTEXT->closeActivity("settingActivity");
    return false;
}

static bool onButtonClick_voiceButton(ZKButton *pButton) {
    LOGD(" ButtonClick voiceButton !!!\n");
    EASYUICONTEXT->openActivity("setvoiceActivity");
    EASYUICONTEXT->closeActivity("setshowActivity");
    EASYUICONTEXT->closeActivity("setlinkActivity");
    EASYUICONTEXT->closeActivity("setreverseActivity");
    EASYUICONTEXT->closeActivity("setfactoryActivity");
    EASYUICONTEXT->closeActivity("settingActivity");
    return false;
}

static bool onButtonClick_reverseButton(ZKButton *pButton) {
    LOGD(" ButtonClick reverseButton !!!\n");
    EASYUICONTEXT->closeActivity("setshowActivity");
    EASYUICONTEXT->closeActivity("setlinkActivity");
    EASYUICONTEXT->closeActivity("setvoiceActivity");
    EASYUICONTEXT->openActivity("setreverseActivity");
    EASYUICONTEXT->closeActivity("setfactoryActivity");
    EASYUICONTEXT->closeActivity("settingActivity");
    return false;
}

static bool onButtonClick_factoryButton(ZKButton *pButton) {
    LOGD(" ButtonClick factoryButton !!!\n");
    EASYUICONTEXT->closeActivity("setshowActivity");
    EASYUICONTEXT->closeActivity("setlinkActivity");
    EASYUICONTEXT->closeActivity("setvoiceActivity");
    EASYUICONTEXT->closeActivity("setreverseActivity");
    EASYUICONTEXT->openActivity("setfactoryActivity");
    EASYUICONTEXT->closeActivity("settingActivity");
    return false;
}

static bool onButtonClick_systemButton(ZKButton *pButton) {
    LOGD(" ButtonClick systemButton !!!\n");
    systemWnd(true);
    EASYUICONTEXT->closeActivity("setshowActivity");
    EASYUICONTEXT->closeActivity("setlinkActivity");
    EASYUICONTEXT->closeActivity("setvoiceActivity");
    EASYUICONTEXT->closeActivity("setreverseActivity");
    EASYUICONTEXT->closeActivity("setfactoryActivity");
    return false;
}
