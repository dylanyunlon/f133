/*
 * SlideManager.h
 *
 *  Created on: 2022年6月29日
 *      Author: chenxy
 */

#ifndef JNI_SLIDE_MANAGER_SLIDEMANAGER_H_
#define JNI_SLIDE_MANAGER_SLIDEMANAGER_H_
#include "app/BaseApp.h"
#include "system/Thread.h"
#include "math.h"
#include "utils/VelocityTracker.h"
#include "entry/EasyUIContext.h"

typedef enum{
	SLIDEDONETYPE_LEFT,					//左滑完成
	SLIDEDONETYPE_RIGHT,				//右滑完成
}SLIDEDONETYPE;

class slideWnd : public Thread,
				 public VelocityTracker,
				 public EasyUIContext::ITouchListener
{
private:
	int slide_direct[3],slide_direct_index = 0,slide_start_Posi_x = -1,slide_end_Posi_x,R_X,R_Y,Wnd_l,Wnd_t,Wnd_w,Wnd_h;
	bool slide_left=false,slide_right=false,fast_slide_flag = false,Is_Cyclic = true;
	int Pre_Wnd = 0,Curr_Wnd = 1,Next_Wnd = 2;
	int SwitchSpeed = 1000;		//切换速度
	float Speed = 0.0001;
	bool IsInitDone = false;	//初始化完成
	vector<ZKWindow *> Wnds;
	void (*SlideListener)(SLIDEDONETYPE SDT) = NULL;
protected:
	/**
	 * @重写触摸事件接口
	 */
	virtual bool onTouchEvent(const MotionEvent &ev);

public:
	slideWnd();
	~slideWnd();

	/**
	 * @brief 滑动窗口初始化
	 * @param WndBox 外框窗口
	 * @param wnds 滑动窗口容器
	 */
	void slideWnd_Init(ZKWindow *WndBox,vector<ZKWindow *> &wnds);

	/**
	 * @注册触摸监听
	 */
	void registerTouchListener();

	/**
	 * @brief 注销触摸监听
	 */
	void unregisterTouchListener();

	virtual bool readyToRun();

	virtual bool threadLoop();

	/**
	 * @brief 注册滑动结束监听
	 */
	void RegisterSlideListener(void (*slidelistener)(SLIDEDONETYPE SDT));

	/**
	 * @brief 滑动窗口触摸处理
	 * @param ev 新触摸事件
	 */
	void slideWnd_handle(const MotionEvent &ev);

	/**
	 * @brief 设置IsInitDone属性值
	 */
	void SetIsInitDone(bool IsInitDone);

	/**
	 * @brief 设置滑动循环
	 * @param Is_cyclic true:循环,false:不循环,默认循环
	 */
	void Set_Slide_Cyclic(bool Is_cyclic);

	/**
	 * @brief 设置松开时切换速度
	 * @SwitchSpeed 默认为1000(us)即1ms移动一个像素位置 值越小松开滑动切换速度越快
	 */
	void Set_SwitchSpeed(int switchspeed);

	/**
	 * @brief 获取上一个窗口下标
	 */
	int Get_Pre_Wnd_Index();

	/**
	 * @brief 获取当前窗口下标
	 */
	int Get_Curr_Wnd_Index();

	/**
	 * @brief 获取下一个窗口下标
	 */
	int Get_Next_Wnd_Index();

	/**
	 * @brief 切换显示下一个窗口
	 */
	void Next_Wndow();

	/**
	 * @brief 切换显示上一个窗口
	 */
	void Pre_Wndow();
};

#endif /* JNI_SLIDE_MANAGER_SLIDEMANAGER_H_ */
