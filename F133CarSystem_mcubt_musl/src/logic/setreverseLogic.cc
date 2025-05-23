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
#include "system/setting.h"

static int s_format_index;

typedef struct{
	std::string dev;
	int wide;
	int high;
	std::string format;
	int rate;
}c_cam_format_tab;
static c_cam_format_tab cam_format_tab[] = {
	  "/dev/video0",1280,720,"AHD 720P 25",25,
	  "/dev/video0",1280,720,"AHD 720P 30",30,
	  "/dev/video0",1280,720,"TVI 720P 25",24,
	  "/dev/video0",1280,720,"TVI 720P 30",29,
	  "/dev/video0",1920,1080,"AHD 1080P 25",25,
	  "/dev/video0",1920,1080,"AHD 1080P 30",30,
	  "/dev/video0",1920,1080,"TVI 1080P 25",24,
	  "/dev/video0",1920,1080,"TVI 1080P 30",29,
	  "/dev/video4",960,576,"CVBS PAL 50",50,
	  "/dev/video4",960,480,"CVBS NTSC 60",60,
//	  720,480,"DM5885 50",50,
//	  720,480,"DM5885 49",49
};

static void setting_camera() {
	mskipTextViewPtr->setText(sys::setting::get_camera_skip());
	switch(sys::setting::get_camera_rot()) {
	case 0:
		mrotButtonPtr->setSelected(false);
		break;
	case 2:
		mrotButtonPtr->setSelected(true);
		break;
	}
	mcaminfoListViewPtr->setSelection(s_format_index);
	mlineButtonPtr->setSelected(sys::setting::is_reverse_line_view());
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

static void set_skip_or_rot(bool _s_skip, bool _s_skip_add, bool _s_rot) {
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
		if (rot == 0) {
			mrotButtonPtr->setSelected(true);
			sys::setting::set_camera_rot(rot + 2);
		} else if (rot == 2) {
			mrotButtonPtr->setSelected(false);
			sys::setting::set_camera_rot(rot - 2);
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
static bool onsetreverseActivityTouchEvent(const MotionEvent &ev) {
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
static bool onButtonClick_showButton(ZKButton *pButton) {
    LOGD(" ButtonClick showButton !!!\n");
    EASYUICONTEXT->openActivity("setshowActivity");
    EASYUICONTEXT->closeActivity("setlinkActivity");
    EASYUICONTEXT->closeActivity("setvoiceActivity");
    EASYUICONTEXT->closeActivity("setreverseActivity");
    EASYUICONTEXT->closeActivity("setfactoryActivity");
    EASYUICONTEXT->closeActivity("settingsActivity");
    return false;
}

static bool onButtonClick_linkButton(ZKButton *pButton) {
    LOGD(" ButtonClick linkButton !!!\n");
    EASYUICONTEXT->closeActivity("setshowActivity");
    EASYUICONTEXT->openActivity("setlinkActivity");
    EASYUICONTEXT->closeActivity("setvoiceActivity");
    EASYUICONTEXT->closeActivity("setreverseActivity");
    EASYUICONTEXT->closeActivity("setfactoryActivity");
    EASYUICONTEXT->closeActivity("settingsActivity");
    return false;
}

static bool onButtonClick_voiceButton(ZKButton *pButton) {
    LOGD(" ButtonClick voiceButton !!!\n");
    EASYUICONTEXT->openActivity("setvoiceActivity");
    EASYUICONTEXT->closeActivity("setshowActivity");
    EASYUICONTEXT->closeActivity("setlinkActivity");
    EASYUICONTEXT->closeActivity("setreverseActivity");
    EASYUICONTEXT->closeActivity("setfactoryActivity");
    EASYUICONTEXT->closeActivity("settingsActivity");
    return false;
}

static bool onButtonClick_reverseButton(ZKButton *pButton) {
    LOGD(" ButtonClick reverseButton !!!\n");
    EASYUICONTEXT->closeActivity("setshowActivity");
    EASYUICONTEXT->closeActivity("setlinkActivity");
    EASYUICONTEXT->closeActivity("setvoiceActivity");
    EASYUICONTEXT->closeActivity("setfactoryActivity");
    EASYUICONTEXT->closeActivity("settingsActivity");
    return false;
}

static bool onButtonClick_factoryButton(ZKButton *pButton) {
    LOGD(" ButtonClick factoryButton !!!\n");
    EASYUICONTEXT->openActivity("setfactoryActivity");
    EASYUICONTEXT->closeActivity("setshowActivity");
    EASYUICONTEXT->closeActivity("setlinkActivity");
    EASYUICONTEXT->closeActivity("setvoiceActivity");
    EASYUICONTEXT->closeActivity("setreverseActivity");
    EASYUICONTEXT->closeActivity("settingsActivity");
    return false;
}

static bool onButtonClick_systemButton(ZKButton *pButton) {
    LOGD(" ButtonClick systemButton !!!\n");
    EASYUICONTEXT->openActivity("settingsActivity");
    EASYUICONTEXT->closeActivity("setshowActivity");
    EASYUICONTEXT->closeActivity("setlinkActivity");
    EASYUICONTEXT->closeActivity("setvoiceActivity");
    EASYUICONTEXT->closeActivity("setreverseActivity");
    EASYUICONTEXT->closeActivity("setfactoryActivity");
    return false;
}

static int getListItemCount_caminfoListView(const ZKListView *pListView) {
    //LOGD("getListItemCount_caminfoListView !\n");
	 return TABLESIZE(cam_format_tab);
}

static void obtainListItemData_caminfoListView(ZKListView *pListView,ZKListView::ZKListItem *pListItem, int index) {
    //LOGD(" obtainListItemData_ caminfoListView  !!!\n");
	ZKListView::ZKListSubItem* SubItem9 = pListItem->findSubItemByID(ID_SETREVERSE_SubItem9);
	SubItem9->setText(cam_format_tab[index].format);
	SubItem9->setSelected(s_format_index == index);
}

static void onListItemClick_caminfoListView(ZKListView *pListView, int index, int id) {
    //LOGD(" onListItemClick_ caminfoListView  !!!\n");
	s_format_index = index;
	sys::setting::set_camera_dev(cam_format_tab[index].dev.c_str());
	sys::setting::set_camera_size(cam_format_tab[index].wide,cam_format_tab[index].high);
	sys::setting::set_camera_rate(cam_format_tab[index].rate);
}

static bool onButtonClick_skipreduceButton(ZKButton *pButton) {
    LOGD(" ButtonClick skipreduceButton !!!\n");
	set_skip_or_rot(true, false, false);
    return false;
}

static bool onButtonClick_skipaddButton(ZKButton *pButton) {
    LOGD(" ButtonClick skipaddButton !!!\n");
    set_skip_or_rot(true, true, false);
    return false;
}

static bool onButtonClick_rotButton(ZKButton *pButton) {
    LOGD(" ButtonClick rotnextButton !!!\n");
    set_skip_or_rot(false, false, true);
    return false;
}

static bool onButtonClick_lineButton(ZKButton *pButton) {
    LOGD(" ButtonClick lineButton !!!\n");
    pButton->setSelected(!pButton->isSelected());
    sys::setting::set_reverse_line_view(pButton->isSelected());
    return false;
}

static int getListItemCount_ChannelListView(const ZKListView *pListView) {
    //LOGD("getListItemCount_ChannelListView !\n");
    return 4;
}

static void obtainListItemData_ChannelListView(ZKListView *pListView,ZKListView::ZKListItem *pListItem, int index) {
    //LOGD(" obtainListItemData_ ChannelListView  !!!\n");
	ZKListView::ZKListSubItem* SubItem10 = pListItem->findSubItemByID(ID_SETREVERSE_SubItem10);
	SubItem10->setText(index);
	SubItem10->setSelected(sys::setting::get_camera_chn() == index);
}

static void onListItemClick_ChannelListView(ZKListView *pListView, int index, int id) {
    //LOGD(" onListItemClick_ ChannelListView  !!!\n");
	sys::setting::set_camera_chn(index);
}

static bool onButtonClick_cvbsButton(ZKButton *pButton) {
    LOGD(" ButtonClick cvbsButton !!!\n");
    pButton->setSelected(!pButton->isSelected());
    setenv("ZKCAMERA_CHN", pButton->isSelected() ? "1" : "0", 1);
    pButton->setText(pButton->isSelected() ? "cvbs1" : "cvbs0");
    return false;
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
