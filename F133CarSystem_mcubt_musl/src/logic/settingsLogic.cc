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
#define DEFAULT_BT_NAME "BT_BOX"
#define TIMEBIGSIZE 	45 	//时间设置中间字体大小
#define TIMESMALLSIZE 	30	//时间设置两边字体大小

#define UPDATE_DAY 1
//起始年份
#define ORG_YEAR 1970
//年份总个数
#define YEAR_NUM 67

static bt_cb_t _s_bt_cb;

static const char* settingsTextTab[] = {"show", "link", "voice", "reverse", "factory", "system"};//,"more"
static const char* showTextTab[] = {"brightness","color system"};
static const char* reverseTextTab[] = {"Camera parameters"};
static const char* systemTextTab[] = {"language","data","wifi","Debug"};

static const char* moreTextTab[] = {"developer"};
static const char* languaueTextTab[] = {"简体中文", "English(US)"};
typedef struct{
	int wide;
	int high;
	std::string format;
	int rate;
}c_cam_format_tab;
static c_cam_format_tab cam_format_tab[] = {
	  1280,720,"AHD 720P 25",25,
	  1280,720,"AHD 720P 30",30,
	  1280,720,"TVI 720P 25",24,
	  1280,720,"TVI 720P 30",29,
	  1920,1080,"AHD 1080P 25",25,
	  1920,1080,"AHD 1080P 30",30,
	  1920,1080,"TVI 1080P 25",24,
	  1920,1080,"TVI 1080P 30",29,
	  960,576,"CVBS PAL 50",50,
	  960,480,"CVBS NTSC 60",60,
	  720,480,"DM5885 50",50,
	  720,480,"DM5885 49",49
};

static std::string adjustText[] = {
	"luminance","hue","saturation","contrast"
};
static ZKWindow **wndtab[] = {&mshowWndPtr,&mlinkWndPtr,&mvoiceWndPtr,&mreverseWndPtr,&mmoreWndPtr,&msystemWndPtr};
static ZKWindow **systemwndtab[] = {&mLanguageWndPtr,&mSetdataWndPtr};
static ZKScrollWindow **reversewndtab[] = {&mCameraScrollWindowPtr};  // mCameraWndPtr
static ZKWindow **morewndtab[] = {&mdeveloperWndPtr};

typedef enum {
	LUMINACE,		// 明度
	COLORFULNESS,	// 色彩度
	SATURATION,		// 饱和度
	CONTRASTRATIO,	// 对比度

}SHOWTYPE;

typedef enum {
	SHOWINDEX_E,	// 显示
	LINKINDEX_E,	// 互联
	VOICEINDEX_E,	// 声音
	REVERSEINDEX_E,	// 倒车
	FACTORY_E,		// 工厂
	SYSTEMINDEX_E,	// 系统
	MOREINDEX_E,	// 更多
}SETTINGTYPE;

typedef enum {
	LANGUAGE_E,		// 语言
	DATA_E,			// 时间日期
	WIFI_E,			// wifi
	ADB_E,			// adb
}SYSTEMSHOWTYPE;

typedef enum {
	BRIGHTNESS_E,	// 亮度
	COLOR_E,		// 丽色系统
}SETTINGSHOWTYPE;

typedef enum {
	DEVELOPER_E,	// 开发者选项
}MORETYPE;

typedef enum {
	CH_E,//中文
	US_E,//英文
}LANGUAGETYPE;

typedef enum {
	LINK_TYPE,		// 互联版本类型
	LINK_VOICE,		// 互联发声类型
	LINK_AP_CF,		// 国内版本airplay与carlife切换
} LINK_SETTING;

static SETTINGTYPE s_setting_type;
static SETTINGSHOWTYPE s_show_type;
static LANGUAGETYPE s_language_type;
static MORETYPE s_more_type;
static SHOWTYPE showtype;//显示类型：亮度，对比度等
static SYSTEMSHOWTYPE s_system_type;
static struct tm ptm;    //日期时间结构体
static bool ThreeFalg = false;
static int s_format_index;

static LINK_SETTING link_setting = LINK_TYPE;


extern void set_navibar_brightnessBar(int progress);
extern void set_navibar_PlayVolSeekBar(int progress);
extern void set_ctrlbar_lightSeekBar(int progress);
extern void set_ctrlbar_volumSeekBar(int progress);
//extern void set_navibar_CpSeekBar(int progress);

static bool setting_ftu_isLoad = false;

void setSettingFtu_BrillianceSeekBar(int progress) {
	if (setting_ftu_isLoad){
		mBrightnessTextPtr->setText(progress*10);
		mBrightnessSeekBarPtr->setProgress(progress);
	}
}
void setSettingFtu_CallSeekBar(int progress) {
	if (setting_ftu_isLoad)
		mPhoneSeekBarPtr->setProgress(progress);
}
void setSettingFtu_MediaSeekBar(int progress) {
	if (setting_ftu_isLoad)
		mMediaSeekBarPtr->setProgress(progress);
}

class SeekBarChangeListener : public ZKSeekBar::ISeekBarChangeListener {
public:
	virtual void onProgressChanged(ZKSeekBar *pSeekBar, int progress) {
		if(pSeekBar->getID() == mMediaSeekBarPtr->getID()){
			bool effect = bt::is_calling() || (lk::is_connected() && lk::get_is_call_state() != CallState_Hang);
			audio::set_system_vol(progress / 10.f, !effect);
			set_navibar_PlayVolSeekBar(progress);
			set_ctrlbar_volumSeekBar(progress);
		}else if(pSeekBar->getID() == mPhoneSeekBarPtr->getID()){
			audio::set_lylink_call_vol(progress / 10.f, false);
			if(bt::is_calling()){
				bt::call_vol(progress / 10.f);
			}else if((lk::is_connected() && lk::get_is_call_state() != CallState_Hang)){
				audio::set_lylink_call_vol(progress / 10.f, true);
			}
//			set_navibar_CpSeekBar(progress);
		}else if(pSeekBar->getID() == mBrightnessSeekBarPtr->getID()){
			sys::setting::set_brightness(progress*10);
			mBrightnessTextPtr->setText(progress*10);
			set_navibar_brightnessBar(progress);
			set_ctrlbar_lightSeekBar(progress);
		}
	}
	virtual void onStopTrackingTouch(ZKSeekBar *pSeekBar) {
		int progress = pSeekBar->getProgress();
		if (pSeekBar->getID() == mMediaSeekBarPtr->getID()) {

		} else if (pSeekBar->getID() == mPhoneSeekBarPtr->getID()) {

		} else if (pSeekBar->getID() == mBrightnessSeekBarPtr->getID()) {


		} else if (pSeekBar->getID() == mBrillianceSeekBarPtr->getID()) {
			mBrillianceValTextViewPtr->setText(progress);
			if(showtype == CONTRASTRATIO){
				LOGD("--%d-- --%s-- 设置对比度值为 =%d \n", __LINE__, __FILE__,progress);
				sys::setting::set_contrast(progress);
			}else if(showtype == COLORFULNESS){
				LOGD("--%d-- --%s-- 设置色彩度值为 =%d \n", __LINE__, __FILE__,progress);
				sys::setting::set_hue(progress);
			}else if(showtype == SATURATION){
				LOGD("--%d-- --%s-- 设置饱和度值为 =%d \n", __LINE__, __FILE__,progress);
				sys::setting::set_saturation(progress);
			}else if(showtype == LUMINACE){
				LOGD("--%d-- --%s-- 设置明度值为 =%d \n", __LINE__, __FILE__,progress);
				sys::setting::set_luminance(progress);
			}
		}
	}
};

static SeekBarChangeListener sSeekBarChangeListener;

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

// 查询授权状态
static void query_authorization_state() {
	bool blink_is_authorized = bt::get_is_authorized();
	bool lylink_is_authorized = lk::get_is_authorized();
//	bool mfi_is_ok = lk::get_mfi_is_OK();
	bool is_use_carlife = sys::setting::get_is_use_link_carlife();

	mbtAuthorizeButtonPtr->setSelected(blink_is_authorized);
	mbtAuthorizeButtonPtr->setText(blink_is_authorized ? "蓝牙已授权" : "蓝牙未授权");
	mlinkAuthorizeButtonPtr->setSelected(lylink_is_authorized);
	mlinkAuthorizeButtonPtr->setTextTr(lylink_is_authorized ? "Lylink is auto" : "Lylink is not auto");
//	mmfiTextViewPtr->setVisible(!mfi_is_ok);
	mcfButtonPtr->setSelected(is_use_carlife);
}


static void _bt_misc_info_cb(bt_misc_info_e info) {
	switch (info) {
	case E_BT_MISC_INFO_AUTH:
		mbtAuthorizeButtonPtr->setSelected(true);
		mbtAuthorizeButtonPtr->setText("蓝牙已授权");
		break;
	default:
		break;
	}
}

static void _bt_add_cb() {
	_s_bt_cb.misc_info_cb = _bt_misc_info_cb;
	bt::add_cb(&_s_bt_cb);
}

static void _bt_remove_cb() {
	bt::remove_cb(&_s_bt_cb);
}

static void _lylink_callback(LYLINKAPI_EVENT evt, int para0, void *para1) {
	if (evt == LYLINK_VERSION_CODE) {
		switch ((LYLINK_ERRCODE) para0) {
		case LINK_ERRCODE_NONE:
			mlinkAuthorizeButtonPtr->setSelected(true);
			mlinkAuthorizeButtonPtr->setTextTr("Lylink is auto");
			break;
		case LINK_ERRCODE_MFI:
			mmfiTextViewPtr->setVisible(true);
			break;
		default:
			break;
		}
	}
}

static void setting_camera() {
	mskipTextViewPtr->setText(sys::setting::get_camera_skip());
	switch(sys::setting::get_camera_rot()) {
	case 0:
		mrotTextViewPtr->setText("0°");
		break;
	case 1:
		mrotTextViewPtr->setText("90°");
		break;
	case 2:
		mrotTextViewPtr->setText("180°");
		break;
	case 3:
		mrotTextViewPtr->setText("270°");
		break;
	}
	mcaminfoListViewPtr->setSelection(s_format_index);
	if(strcmp(sys::setting::get_camera_dev(),"/dev/video4") == 0){
		mCutcvbsButtonPtr->setSelected(true);
		mCutCameraBtnPtr->setSelected(false);
		mcvbsButtonPtr->setVisible(true);
	}else if(strcmp(sys::setting::get_camera_dev(),"/dev/video0") == 0){
		mCutCameraBtnPtr->setSelected(true);
		mCutcvbsButtonPtr->setSelected(false);
		mcvbsButtonPtr->setVisible(false);
	}
}

static void set_skip_or_rot(bool _s_skip, bool _s_skip_add, bool _s_rot, bool _s_rot_add) {
	if (_s_skip) {
		int skip = std::stoi(mskipTextViewPtr->getText());
		if (_s_skip_add) {
			skip++;
			sys::setting::set_camera_skip(skip);
		} else {
			skip--;
			if (skip <= 0) {
				skip = 0;
				sys::setting::set_camera_skip(skip);
			} else {
				sys::setting::set_camera_skip(skip);
			}
		}
		mskipTextViewPtr->setText(skip);
	}
	if (_s_rot) {
		int rot = sys::setting::get_camera_rot();
		if (_s_rot_add) {
			rot++;
			if (rot >= 3) {
				rot = 3;
				sys::setting::set_camera_rot(rot);
			} else if (rot <= 0) {
				rot = 0;
				sys::setting::set_camera_rot(rot);
			} else {
				sys::setting::set_camera_rot(rot);
			}
		} else {
			rot--;
			if (rot >= 3) {
				rot = 3;
				sys::setting::set_camera_rot(rot);
			} else if (rot <= 0) {
				rot = 0;
				sys::setting::set_camera_rot(rot);
			} else {
				sys::setting::set_camera_rot(rot);
			}
		}
		switch(rot) {
		case 0:
			mrotTextViewPtr->setText("0°");
			break;
		case 1:
			mrotTextViewPtr->setText("90°");
			break;
		case 2:
			mrotTextViewPtr->setText("180°");
			break;
		case 3:
			mrotTextViewPtr->setText("270°");
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
	query_authorization_state();

	_bt_add_cb();
	lk::add_lylink_callback(_lylink_callback);

	setting_ftu_isLoad = true;
//	mSetLinkVoiceBtnPtr->setSelected(sys::setting::get_linksound_type() == E_TYPE_ENGINE);
//	mSetLinkTypeBtnPtr->setSelected(sys::setting::get_link_type() == E_TYPE_CN);
	mcfButtonPtr->setVisible(mSetLinkTypeBtnPtr->isSelected());

	s_setting_type = SHOWINDEX_E;
	mBrillianceSeekBarPtr->setSeekBarChangeListener(&sSeekBarChangeListener);
	mBrightnessSeekBarPtr->setSeekBarChangeListener(&sSeekBarChangeListener);
	mMediaSeekBarPtr->setSeekBarChangeListener(&sSeekBarChangeListener);
	mPhoneSeekBarPtr->setSeekBarChangeListener(&sSeekBarChangeListener);

	int brightness = sys::setting::get_brightness();
	mBrightnessSeekBarPtr->setProgress(brightness / 10);
	mBrightnessTextPtr->setText(brightness);

	Get_CurrentCode(LANGUAGEMANAGER->getCurrentCode());

	mMediaSeekBarPtr->setProgress(audio::get_system_vol() * 10);
	mPhoneSeekBarPtr->setProgress(audio::get_lylink_call_vol() * 10);

	mhourButton12Ptr->setSelected(!sys::setting::is_time_format_24h());
	mhourButton24Ptr->setSelected(sys::setting::is_time_format_24h());

	setting_camera();
	for (unsigned int i=0; i<TABLESIZE(cam_format_tab); i++) {
		if(cam_format_tab[i].wide == sys::setting::get_camera_wide() &&
			cam_format_tab[i].high == sys::setting::get_camera_high()&&cam_format_tab[i].rate == sys::setting::get_camera_rate()){
			s_format_index = i;
			break;
		}
	}
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
	lk::remove_lylink_callback(_lylink_callback);
	EASYUICONTEXT->showNaviBar();
	setting_ftu_isLoad = false;
	mBrillianceSeekBarPtr->setSeekBarChangeListener(NULL);
	mBrightnessSeekBarPtr->setSeekBarChangeListener(NULL);
	mMediaSeekBarPtr->setSeekBarChangeListener(NULL);
	mPhoneSeekBarPtr->setSeekBarChangeListener(NULL);
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

static void systemWnd_index(int &index){
	for (unsigned int i=0; i<TABLESIZE(systemwndtab); i++) {
		if((*systemwndtab[i])->isVisible()){
			index = i;
		}
	}
}

static void moreWnd_index(int &index){
	for (unsigned int i=0; i<TABLESIZE(morewndtab); i++) {
		if((*morewndtab[i])->isVisible()){
			index = i;
		}
	}
}

static void Wnd_Hidden(int &index){
    switch(s_setting_type){
    case SHOWINDEX_E:
    	if(mBrightnessWndPtr->isWndShow()){
    		index = 0;
    		mBrightnessWndPtr->hideWnd();
    	}
    	break;
    case VOICEINDEX_E:
    	break;
    case REVERSEINDEX_E:
        break;
    case FACTORY_E:
    	break;
    case SYSTEMINDEX_E:
    	systemWnd_index(index);
        if(index >= 0){
        	(*systemwndtab[index])->setVisible(false);
        }
        break;
    case MOREINDEX_E:
    	moreWnd_index(index);
        if(index >= 0){
        	(*morewndtab[index])->setVisible(false);
        }
        break;
    default:
    	break;
    }
}
static bool onButtonClick_ReturnBtn(ZKButton *pButton) {
    LOGD(" ButtonClick ReturnBtn !!!\n");
    int showindex = -1;
    Wnd_Hidden(showindex);
    if(showindex < 0)
//    EASYUICONTEXT->openActivity("mainActivity");
    EASYUICONTEXT->goBack();
    return false;
}

static void Get_picpath(const char *settings,bool isselect,char *realpic){
	char picpath[30] = {0};
	if(isselect){
		sprintf(picpath,"set/%s_p.png",settings);
	}else{
		sprintf(picpath,"set/%s_n.png",settings);
	}
	strcpy(realpic,picpath);
}

static void Open_SecWnd(SETTINGTYPE type){
	switch(type){
	case SHOWINDEX_E:
		break;
	case SYSTEMINDEX_E:
		 char version[32];
		 SystemProperties::getString("ro.build.git", version, "");
		 mVersionTextPtr->setText(version);
		break;
	default:
		break;
	}
}
static int getListItemCount_SettingListView(const ZKListView *pListView) {
    //LOGD("getListItemCount_SettingListView !\n");
	return TABLESIZE(settingsTextTab);
}

static void obtainListItemData_SettingListView(ZKListView *pListView,ZKListView::ZKListItem *pListItem, int index) {
    //LOGD(" obtainListItemData_ SettingListView  !!!\n");
	char spicpath[30] = {0};
	char npicpath[30] = {0};
	Get_picpath(settingsTextTab[index],true,spicpath);
	Get_picpath(settingsTextTab[index],false,npicpath);
	ZKListView::ZKListSubItem* SettingpicSub = pListItem->findSubItemByID(ID_SETTINGS_SettingpicSub);
	ZKListView::ZKListSubItem* settingTextSub = pListItem->findSubItemByID(ID_SETTINGS_settingTextSub);
//	ZKListView::ZKListSubItem* SubItem7 = pListItem->findSubItemByID(ID_SETTINGS_SubItem7);
	settingTextSub->setTextTr(settingsTextTab[index]);
	SettingpicSub->setButtonStatusPic(ZK_CONTROL_STATUS_SELECTED,spicpath);
	SettingpicSub->setButtonStatusPic(ZK_CONTROL_STATUS_NORMAL,npicpath);
//	SubItem7->setSelected(s_setting_type == index);
	SettingpicSub->setSelected(s_setting_type == index);
	settingTextSub->setSelected(s_setting_type == index);
}

static void onListItemClick_SettingListView(ZKListView *pListView, int index, int id) {
    //LOGD(" onListItemClick_ SettingListView  !!!\n");
    int showindex = -1;
    Wnd_Hidden(showindex);
	s_setting_type = (SETTINGTYPE)index;
	for (unsigned int i=0; i<TABLESIZE(wndtab); i++) {
		(*wndtab[i])->setVisible(s_setting_type == i);
	}
	Open_SecWnd(s_setting_type);
}

static int getListItemCount_showListView(const ZKListView *pListView) {
    //LOGD("getListItemCount_showListView !\n");
    return TABLESIZE(showTextTab);
}

static void obtainListItemData_showListView(ZKListView *pListView,ZKListView::ZKListItem *pListItem, int index) {
    //LOGD(" obtainListItemData_ showListView  !!!\n");
	ZKListView::ZKListSubItem* showtext = pListItem->findSubItemByID(ID_SETTINGS_showtext);
	ZKListView::ZKListSubItem* showpic = pListItem->findSubItemByID(ID_SETTINGS_showpic);
	showtext->setTextTr(showTextTab[index]);
	showpic->setVisible(index == 0 ? false : true);
}

static void onListItemClick_showListView(ZKListView *pListView, int index, int id) {
    //LOGD(" onListItemClick_ showListView  !!!\n");
	s_show_type = (SETTINGSHOWTYPE)index;
	if(s_show_type == COLOR_E){
		showtype = LUMINACE;
		mShowTextPtr->setTextTr(adjustText[showtype].c_str());
		mBrillianceSeekBarPtr->setProgress(sys::setting::get_luminance());
		mBrillianceValTextViewPtr->setText(sys::setting::get_luminance());
		mBrightnessWndPtr->showWnd();
	}
}

static bool onButtonClick_colorButton(ZKButton *pButton) {
    LOGD(" ButtonClick colorButton !!!\n");
    showtype = LUMINACE;
	mShowTextPtr->setTextTr(adjustText[showtype].c_str());
	mBrillianceSeekBarPtr->setProgress(sys::setting::get_luminance());
	mBrillianceValTextViewPtr->setText(sys::setting::get_luminance());
	mBrightnessWndPtr->showWnd();
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

static bool onButtonClick_SetTimeSureBtn(ZKButton *pButton) {
    LOGD(" ButtonClick SetTimeSureBtn !!!\n");
    sys::setting::set_DateTime(&ptm);
    mSetdataWndPtr->hideWnd();
    return false;
}

static bool onButtonClick_timeCancelButton(ZKButton *pButton) {
    LOGD(" ButtonClick timeCancelButton !!!\n");
    mSetdataWndPtr->hideWnd();
    return false;
}

static int getListItemCount_caminfoListView(const ZKListView *pListView) {
    //LOGD("getListItemCount_caminfoListView !\n");
	 return TABLESIZE(cam_format_tab);
}

static void obtainListItemData_caminfoListView(ZKListView *pListView,ZKListView::ZKListItem *pListItem, int index) {
    //LOGD(" obtainListItemData_ caminfoListView  !!!\n");
	ZKListView::ZKListSubItem* SubItem9 = pListItem->findSubItemByID(ID_SETTINGS_SubItem9);
	SubItem9->setText(cam_format_tab[index].format);
	SubItem9->setSelected(s_format_index == index);
}

static void onListItemClick_caminfoListView(ZKListView *pListView, int index, int id) {
    //LOGD(" onListItemClick_ caminfoListView  !!!\n");
	s_format_index = index;
	sys::setting::set_camera_size(cam_format_tab[index].wide,cam_format_tab[index].high);
	sys::setting::set_camera_rate(cam_format_tab[index].rate);
}

static int getListItemCount_ChannelListView(const ZKListView *pListView) {
    //LOGD("getListItemCount_ChannelListView !\n");
    return 4;
}

static void obtainListItemData_ChannelListView(ZKListView *pListView,ZKListView::ZKListItem *pListItem, int index) {
    //LOGD(" obtainListItemData_ ChannelListView  !!!\n");
	ZKListView::ZKListSubItem* SubItem10 = pListItem->findSubItemByID(ID_SETTINGS_SubItem10);
	SubItem10->setText(index);
	SubItem10->setSelected(sys::setting::get_camera_chn() == index);
}

static void onListItemClick_ChannelListView(ZKListView *pListView, int index, int id) {
    //LOGD(" onListItemClick_ ChannelListView  !!!\n");
	sys::setting::set_camera_chn(index);
}

static bool onButtonClick_CutCameraBtn(ZKButton *pButton) {
    LOGD(" ButtonClick CutCameraBtn !!!\n");
    sys::setting::set_camera_dev("/dev/video0");
    LOGD("设置为AHD");
    mcvbsButtonPtr->setVisible(false);
    mCutcvbsButtonPtr->setSelected(false);
    pButton->setSelected(!mCutcvbsButtonPtr->isSelected());
    return false;
}

static bool onButtonClick_CutcvbsButton(ZKButton *pButton) {
    LOGD(" ButtonClick CutcvbsButton !!!\n");
    sys::setting::set_camera_dev("/dev/video4");
    LOGD("设置为CVBS");
    mcvbsButtonPtr->setVisible(true);
    mCutCameraBtnPtr->setSelected(false);
    pButton->setSelected(!mCutCameraBtnPtr->isSelected());
    return false;
}

static int getListItemCount_moreListView(const ZKListView *pListView) {
    //LOGD("getListItemCount_moreListView !\n");
	return TABLESIZE(moreTextTab);
}

static void obtainListItemData_moreListView(ZKListView *pListView,ZKListView::ZKListItem *pListItem, int index) {
    //LOGD(" obtainListItemData_ moreListView  !!!\n");
	ZKListView::ZKListSubItem* moreTextSub = pListItem->findSubItemByID(ID_SETTINGS_moreTextSub);
	moreTextSub->setTextTr(moreTextTab[index]);

}

static void onListItemClick_moreListView(ZKListView *pListView, int index, int id) {
    //LOGD(" onListItemClick_ moreListView  !!!\n");
	s_more_type = (MORETYPE)index;
	for (unsigned int i=0; i<TABLESIZE(morewndtab); i++) {
		(*morewndtab[i])->setVisible(s_more_type == i);
	}
}

static void onProgressChanged_BrillianceSeekBar(ZKSeekBar *pSeekBar, int progress) {
    //LOGD(" ProgressChanged BrillianceSeekBar %d !!!\n", progress);
}

static bool onButtonClick_BtnLeft(ZKButton *pButton) {
    LOGD(" ButtonClick BtnLeft !!!\n");
    if(showtype == CONTRASTRATIO){
    	showtype = COLORFULNESS;
    	mBrillianceSeekBarPtr->setProgress(sys::setting::get_hue());
    	mBrillianceValTextViewPtr->setText(sys::setting::get_hue());
    }
    else if(showtype == COLORFULNESS){
    	showtype = SATURATION;
    	mBrillianceSeekBarPtr->setProgress(sys::setting::get_saturation());
    	mBrillianceValTextViewPtr->setText(sys::setting::get_saturation());
    }
    else if(showtype == SATURATION){
    	showtype = LUMINACE;
    	mBrillianceSeekBarPtr->setProgress(sys::setting::get_luminance());
    	mBrillianceValTextViewPtr->setText(sys::setting::get_luminance());

    }else if(showtype == LUMINACE){
    	showtype = CONTRASTRATIO;
    	mBrillianceSeekBarPtr->setProgress(sys::setting::get_contrast());
    	mBrillianceValTextViewPtr->setText(sys::setting::get_contrast());
    }
    mShowTextPtr->setTextTr(adjustText[showtype].c_str());
    return false;
}

static bool onButtonClick_BtnRight(ZKButton *pButton) {
    LOGD(" ButtonClick BtnRight !!!\n");
    if(showtype == LUMINACE){
    	showtype = SATURATION;
    	mBrillianceSeekBarPtr->setProgress(sys::setting::get_saturation());
    	mBrillianceValTextViewPtr->setText(sys::setting::get_saturation());
    }
    else if(showtype == SATURATION){
    	showtype = COLORFULNESS;
    	mBrillianceSeekBarPtr->setProgress(sys::setting::get_hue());
    	mBrillianceValTextViewPtr->setText(sys::setting::get_hue());
    }
    else if(showtype == COLORFULNESS){
    	showtype = CONTRASTRATIO;
    	mBrillianceSeekBarPtr->setProgress(sys::setting::get_contrast());
    	mBrillianceValTextViewPtr->setText(sys::setting::get_contrast());
    }
    else if(showtype == CONTRASTRATIO){
    	showtype = LUMINACE;
    	mBrillianceSeekBarPtr->setProgress(sys::setting::get_luminance());
    	mBrillianceValTextViewPtr->setText(sys::setting::get_luminance());
    }
    mShowTextPtr->setTextTr(adjustText[showtype].c_str());
    return false;
}

static void onProgressChanged_BrightnessSeekBar(ZKSeekBar *pSeekBar, int progress) {
    //LOGD(" ProgressChanged BrightnessSeekBar %d !!!\n", progress);
	set_navibar_brightnessBar(progress);
	set_ctrlbar_lightSeekBar(progress);
}
static void onProgressChanged_MediaSeekBar(ZKSeekBar *pSeekBar, int progress) {
    //LOGD(" ProgressChanged MediaSeekBar %d !!!\n", progress);
	set_navibar_PlayVolSeekBar(progress);
	set_ctrlbar_volumSeekBar(progress);
}
static void onProgressChanged_PhoneSeekBar(ZKSeekBar *pSeekBar, int progress) {
    //LOGD(" ProgressChanged PhoneSeekBar %d !!!\n", progress);
//	set_navibar_CpSeekBar(progress);
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

	switch (s_system_type) {
	case LANGUAGE_E:		// 语言
		break;
	case DATA_E:			// 时间日期
		InitTimeList();
		break;
	case WIFI_E:			// wifi
		EASYUICONTEXT->openActivity("wifisettingActivity");
		break;
	case ADB_E:				// adb
		sys::setting::enable_usb_adb(!sys::setting::is_usb_adb_enabled());
		break;
	}

	for (unsigned int i=0; i<TABLESIZE(systemwndtab); i++) {
		(*systemwndtab[i])->setVisible(s_system_type == i);
	}
}

static bool onButtonClick_skipreduceButton(ZKButton *pButton) {
    LOGD(" ButtonClick skipreduceButton !!!\n");
	set_skip_or_rot(true, false, false, false);
    return false;
}
static bool onButtonClick_skipaddButton(ZKButton *pButton) {
    LOGD(" ButtonClick skipaddButton !!!\n");
    set_skip_or_rot(true, true, false, false);
    return false;
}
static bool onButtonClick_rotdownButton(ZKButton *pButton) {
    LOGD(" ButtonClick rotdownButton !!!\n");
    set_skip_or_rot(false, false, true, false);
    return false;
}
static bool onButtonClick_rotnextButton(ZKButton *pButton) {
    LOGD(" ButtonClick rotnextButton !!!\n");
    set_skip_or_rot(false, false, true, true);
    return false;
}

static bool onButtonClick_SetLinkVoiceBtn(ZKButton *pButton) {
    LOGD(" ButtonClick SetLinkVoiceBtn !!!\n");
    link_setting = LINK_VOICE;
    mlinkVoiceWindowPtr->showWnd();
    return false;
}

static bool onButtonClick_cvbsButton(ZKButton *pButton) {
    LOGD(" ButtonClick cvbsButton !!!\n");
    pButton->setSelected(!pButton->isSelected());
    setenv("ZKCAMERA_CHN", pButton->isSelected() ? "1" : "0", 1);
    pButton->setText(pButton->isSelected() ? "cvbs1" : "cvbs0");
    return false;
}
static void onProgressChanged_QSeekBar(ZKSeekBar *pSeekBar, int progress) {
    //LOGD(" ProgressChanged QSeekBar %d !!!\n", progress);
}

static void onProgressChanged_GSeekBar(ZKSeekBar *pSeekBar, int progress) {
    //LOGD(" ProgressChanged GSeekBar %d !!!\n", progress);
}
static bool onButtonClick_btAuthorizeButton(ZKButton *pButton) {
    LOGD(" ButtonClick btAuthorizeButton !!!\n");
    return false;
}

static bool onButtonClick_linkAuthorizeButton(ZKButton *pButton) {
    LOGD(" ButtonClick linkAuthorizeButton !!!\n");
    return false;
}

static bool onButtonClick_carBtOkButton(ZKButton *pButton) {
#if 0
    LOGD(" ButtonClick carBtOkButton !!!\n");
    lk::stop_lylink();
    switch (link_setting) {
    case LINK_TYPE:
    	mSetLinkTypeBtnPtr->setSelected(!mSetLinkTypeBtnPtr->isSelected());
        sys::setting::set_link_type(mSetLinkTypeBtnPtr->isSelected() ? E_TYPE_CN : E_TYPE_AB);
        mcfButtonPtr->setVisible(mSetLinkTypeBtnPtr->isSelected());
        if(!mSetLinkTypeBtnPtr->isSelected()){
            sys::setting::set_is_use_link_carlife(false);
            mcfButtonPtr->setSelected(false);
        }
    	break;
    case LINK_VOICE:
    	mSetLinkVoiceBtnPtr->setSelected(!mSetLinkVoiceBtnPtr->isSelected());
        sys::setting::set_linksound_type(mSetLinkVoiceBtnPtr->isSelected() ? E_TYPE_ENGINE : E_TYPE_PHONE);
    	break;
    case LINK_AP_CF:
        mcfButtonPtr->setSelected(!mcfButtonPtr->isSelected());
        sys::setting::set_is_use_link_carlife(mcfButtonPtr->isSelected());
    	break;
    }
    mlinkVoiceWindowPtr->hideWnd();
    lk::start_lylink();
#endif
    return false;
}

static bool onButtonClick_carBtCancelButton(ZKButton *pButton) {
    LOGD(" ButtonClick carBtCancelButton !!!\n");
    mlinkVoiceWindowPtr->hideWnd();
    return false;
}

static bool onButtonClick_SetLinkTypeBtn(ZKButton *pButton) {
    LOGD(" ButtonClick SetLinkTypeBtn !!!\n");
    link_setting = LINK_TYPE;
    mlinkVoiceWindowPtr->showWnd();
    return false;
}


#include "misc/storage.h"
const int LEN = 255;
const int OFFSET = 0;		//lyink (0 + 32), blink(32 + 220)

static bool read_otp_len(int offset, int len) {
	uint8_t otp_data[len];
	if (!storage::otp_read_data(offset, otp_data, len)) {
		LOGE("[bt] otp read data err !!!\n");
		return false;
	}
	LOGD("read_otp_len ------------------------");
	std::string otpstr;
	char buf[64];
	for (int i=0; i<len; i++) {
		if (i == 32) {
			snprintf(buf, TABLESIZE(buf), "\n%x ", otp_data[i]);
		} else {
			snprintf(buf, TABLESIZE(buf), "%x ", otp_data[i]);
		}
		otpstr += buf;
	}

	motpTextViewPtr->setText(otpstr);
	return true;
}

static bool onButtonClick_otpButton(ZKButton *pButton) {
    LOGD(" ButtonClick otpButton !!!\n");
    motpWindowPtr->showWnd();
    read_otp_len(OFFSET, LEN);
    return false;
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

static bool onButtonClick_cfButton(ZKButton *pButton) {
    LOGD(" ButtonClick cfButton !!!\n");
    link_setting = LINK_AP_CF;
    mlinkVoiceWindowPtr->showWnd();
    return false;
}
static bool onButtonClick_soundeffectBtn(ZKButton *pButton) {
    LOGD(" ButtonClick soundeffectBtn !!!\n");
    EASYUICONTEXT->openActivity("soundEffectActivity");
    return false;
}
static int getListItemCount_EQListView(const ZKListView *pListView) {
    //LOGD("getListItemCount_EQListView !\n");
    return 10;
}

static void obtainListItemData_EQListView(ZKListView *pListView,ZKListView::ZKListItem *pListItem, int index) {
    //LOGD(" obtainListItemData_ EQListView  !!!\n");
}

static void onListItemClick_EQListView(ZKListView *pListView, int index, int id) {
    //LOGD(" onListItemClick_ EQListView  !!!\n");
}

static void onEditTextChanged_ValueEditType(const std::string &text) {
    //LOGD(" onEditTextChanged_ ValueEditType %s !!!\n", text.c_str());
}

static bool onButtonClick_SureBtn(ZKButton *pButton) {
    LOGD(" ButtonClick SureBtn !!!\n");
    return false;
}

static bool onButtonClick_Button5(ZKButton *pButton) {
    LOGD(" ButtonClick Button5 !!!\n");
    return false;
}

static int getListItemCount_TypeListView(const ZKListView *pListView) {
    //LOGD("getListItemCount_TypeListView !\n");
    return 5;
}

static void obtainListItemData_TypeListView(ZKListView *pListView,ZKListView::ZKListItem *pListItem, int index) {
    //LOGD(" obtainListItemData_ TypeListView  !!!\n");
}

static void onListItemClick_TypeListView(ZKListView *pListView, int index, int id) {
    //LOGD(" onListItemClick_ TypeListView  !!!\n");
}

static void onEditTextChanged_ValueEditTextF(const std::string &text) {
    //LOGD(" onEditTextChanged_ ValueEditTextF %s !!!\n", text.c_str());
}

static void onEditTextChanged_ValueEditTextG(const std::string &text) {
    //LOGD(" onEditTextChanged_ ValueEditTextG %s !!!\n", text.c_str());
}

static void onEditTextChanged_ValueEditTextQ(const std::string &text) {
    //LOGD(" onEditTextChanged_ ValueEditTextQ %s !!!\n", text.c_str());
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
