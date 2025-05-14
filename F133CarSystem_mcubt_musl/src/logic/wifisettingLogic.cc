#pragma once
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

#include "net/context.h"
#include "net/NetManager.h"

#define WIFIMANAGER			NETMANAGER->getWifiManager()

static std::vector<WifiInfo> sWifiInfos;
static std::map<std::string, std::string> sWifiChangeAps;
static Mutex sLock;

static void removeItemWifiChangeAps(const char *bssid) {
	Mutex::Autolock _l(sLock);
	std::map<std::string, std::string>::iterator it;
	it = sWifiChangeAps.find(bssid);
	if (it != sWifiChangeAps.end()) {
		sWifiChangeAps.erase(it);
	}
}

namespace {

class MyWifiListener : public WifiManager::IWifiListener {
public:
	virtual void handleWifiConnect(E_WIFI_CONNECT event, int args) {
		LOGD("MyWifiListener handleWifiConnect event: %d\n", event);
		mListViewWifiInfoPtr->refreshListView();
	}

	virtual void handleWifiErrorCode(E_WIFI_ERROR_CODE code) {
		mWindowPasswordErrorPtr->showWnd();
	}

	virtual void handleWifiScanResult(std::vector<WifiInfo>* wifiInfos) {
		if (wifiInfos) {
			Mutex::Autolock _l(sLock);
			sWifiInfos.assign(wifiInfos->begin(), wifiInfos->end());
		}

		mListViewWifiInfoPtr->refreshListView();
	}

	virtual void notifySupplicantStateChange(int networkid,
			const char* ssid, const char* bssid,
			E_SUPPLICATION_STATE newState) {
		switch (newState) {
		case AUTHENTICATING: {
			Mutex::Autolock _l(sLock);
			sWifiChangeAps[bssid] = "认证中...";
			break;
		}
		case ASSOCIATING:
			break;
		case ASSOCIATED:
			break;
		case FOUR_WAY_HANDSHAKE:
			break;
		case GROUP_HANDSHAKE: {
			Mutex::Autolock _l(sLock);
			sWifiChangeAps[bssid] = "正在获取IP";
			break;
		}
		case COMPLETED: {
			Mutex::Autolock _l(sLock);
			sWifiChangeAps.clear();
			break;
		}

		case DISCONNECTED:
		case INTERFACE_DISABLED:
		case INACTIVE:
		case SCANNING:
		case DORMANT:
		case UNINITIALIZED:
		case INVALID:
			removeItemWifiChangeAps(bssid);
			break;
		}

		mListViewWifiInfoPtr->refreshListView();
	}
};

}

static MyWifiListener sMyWifiListener;

static void _net_mode_cb(net_mode_e mode) {
	switch (mode) {
	case E_NET_MODE_NULL:
	case E_NET_MODE_AP:     // 热点
	case E_NET_MODE_P2P:    // wifi直连
		mButtonOnOffPtr->setInvalid(false);
		mButtonOnOffPtr->setSelected(false);
		break;
	case E_NET_MODE_WIFI:   // wifi sta
		mButtonOnOffPtr->setInvalid(false);
		mButtonOnOffPtr->setSelected(true);
		break;
	}
}

static std::string getEncryptionInfo(const WifiInfo &wi) {
	std::string info;

	const std::string &encryption = wi.getEncryption();

	if (encryption.find("WEP") != std::string::npos) {
		info = "WEP";
	}
	if (encryption.find("WPA") != std::string::npos) {
		info += info.empty() ? "WPA" : " WPA";
	}
	if (encryption.find("WPA2") != std::string::npos) {
		info += info.empty() ? "WPA2" : " WPA2";
	}
	if (encryption.find("WPS") != std::string::npos) {
		info += "(WPS 加密)";
	}
	if (info.empty()) {
		info = "开放";
	}

	return info;
}

/**
 * 注册定时器
 * 在此数组中添加即可
 */
static S_ACTIVITY_TIMEER REGISTER_ACTIVITY_TIMER_TAB[] = {
	//{0,  6000}, //定时器id=0, 时间间隔6秒
	//{1,  1000},
};

static void onUI_init() {
    //Tips :添加 UI初始化的显示代码到这里,如:mText1Ptr->setText("123");

	mButtonOnOffPtr->setSelected(WIFIMANAGER->isWifiEnable());

	mTextIPAddrPtr->setText(WIFIMANAGER->getIp());
	mTextMacAddrPtr->setText(WIFIMANAGER->getMacAddr());

	WIFIMANAGER->getWifiScanInfosLock(sWifiInfos);
	WIFIMANAGER->addWifiListener(&sMyWifiListener);

	net::add_mode_update_cb(_net_mode_cb);
}

static void onUI_quit() {
	net::remove_mode_update_cb(_net_mode_cb);
	WIFIMANAGER->removeWifiListener(&sMyWifiListener);

	sWifiInfos.clear();
	sWifiChangeAps.clear();
}

static void onProtocolDataUpdate(const SProtocolData &data) {
    // 串口数据回调接口
}

static bool onUI_Timer(int id) {
    //Tips:添加定时器响应的代码到这里,但是需要在本文件的 REGISTER_ACTIVITY_TIMER_TAB 数组中 注册
    //id 是定时器设置时候的标签,这里不要写耗时的操作，否则影响UI刷新,ruturn:[true] 继续运行定时器;[false] 停止运行当前定时器
    return true;
}

static bool onwifisettingActivityTouchEvent(const MotionEvent &ev) {
    // 返回false触摸事件将继续传递到控件上，返回true表示该触摸事件在此被拦截了，不再传递到控件上
    return false;
}

static int getListItemCount_ListViewWifiInfo(const ZKListView *pListView) {
    //LOGD(" getListItemCount_ ListViewWifiInfo  !!!\n");
	Mutex::Autolock _l(sLock);
	return sWifiInfos.size();
}

static void obtainListItemData_ListViewWifiInfo(ZKListView *pListView,ZKListView::ZKListItem *pListItem, int index) {
    //LOGD(" obtainListItemData_ ListViewWifiInfo  !!!\n");
	ZKListView::ZKListSubItem *pLevelItem = pListItem->findSubItemByID(ID_WIFISETTING_LISTSUBITEM_LEVEL);
	ZKListView::ZKListSubItem *pNameItem = pListItem->findSubItemByID(ID_WIFISETTING_LISTSUBITEM_NAME);
	ZKListView::ZKListSubItem *pSubItem = pListItem->findSubItemByID(ID_WIFISETTING_LISTSUBITEM_SUB);

	Mutex::Autolock _l(sLock);
	const WifiInfo &wi = sWifiInfos.at(index);
	if (wi.getEncryption().length() < 5) {
		pLevelItem->setText(5 + wi.getLevel());
	} else {
		pLevelItem->setText(wi.getLevel());
	}
	pNameItem->setText(wi.getSsid());

	if (WIFIMANAGER->isConnected() && (WIFIMANAGER->getConnectionInfo()->getBssid() == wi.getBssid())) {
		pNameItem->setSelected(true);
		pSubItem->setText("已连接");
	} else {
		pNameItem->setSelected(false);

		if (sWifiChangeAps.find(wi.getBssid()) != sWifiChangeAps.end()) {
			pSubItem->setText(sWifiChangeAps[wi.getBssid()]);
		} else {
			pSubItem->setText(getEncryptionInfo(wi));
		}
	}
}

static void onListItemClick_ListViewWifiInfo(ZKListView *pListView, int index, int id) {
    //LOGD(" onListItemClick_ ListViewWifiInfo  !!!\n");
	Mutex::Autolock _l(sLock);
	const WifiInfo &wi = sWifiInfos.at(index);
	if (WIFIMANAGER->isConnected() && (WIFIMANAGER->getConnectionInfo()->getBssid() == wi.getBssid())) {
		mTextConnectSsidPtr->setText(wi.getSsid());
		mTextConnectSecTypePtr->setText(getEncryptionInfo(wi));

		mWindowDisconnectPtr->showWnd();
	} else {
		mTextSsidPtr->setText(wi.getSsid());
		mTextSecTypePtr->setText(getEncryptionInfo(wi));
		mEdittextPwdPtr->setText("");

		mWindowSetPtr->showWnd();
	}
}

static bool onButtonClick_ButtonOnOff(ZKButton *pButton) {
    //LOGD(" ButtonClick ButtonOnOff !!!\n");
	net::change_mode(net::get_mode() == E_NET_MODE_WIFI ? E_NET_MODE_AP : E_NET_MODE_WIFI);
	pButton->setInvalid(true);
    return false;
}

static bool onButtonClick_sys_back(ZKButton *pButton) {
    //LOGD(" ButtonClick sys_back !!!\n");
    return false;
}

static bool onButtonClick_ButtonMenu(ZKButton *pButton) {
    //LOGD(" ButtonClick ButtonMenu !!!\n");
	mTextIPAddrPtr->setText(WIFIMANAGER->getIp());
	mTextMacAddrPtr->setText(WIFIMANAGER->getMacAddr());
	mWindowMenuMorePtr->showWnd();
	return false;
}

static bool onButtonClick_ButtonConnect(ZKButton *pButton) {
    //LOGD(" ButtonClick ButtonConnect !!!\n");
	WIFIMANAGER->connect(mTextSsidPtr->getText(), mEdittextPwdPtr->getText());
	mWindowSetPtr->hideWnd();
    return false;
}

static void onEditTextChanged_EdittextPwd(const std::string &text) {
    //LOGD(" onEditTextChanged_ EdittextPwd %s !!!\n", text.c_str());
}

static bool onButtonClick_ButtonShowPwd(ZKButton *pButton) {
    //LOGD(" ButtonClick ButtonShowPwd !!!\n");
	pButton->setSelected(!pButton->isSelected());
	mEdittextPwdPtr->setPassword(!pButton->isSelected());
    return false;
}

static bool onButtonClick_ButtonDisconnect(ZKButton *pButton) {
    //LOGD(" ButtonClick ButtonDisconnect !!!\n");
	WIFIMANAGER->disconnect();
	mWindowDisconnectPtr->hideWnd();
    return false;
}
