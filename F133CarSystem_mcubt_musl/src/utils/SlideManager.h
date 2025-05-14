/*
 * SlideManager.h
 *
 *  Created on: 2021年10月10日
 *      Author: Administrator
 */

#ifndef JNI_MANAGER_SLIDEMANAGER_H_
#define JNI_MANAGER_SLIDEMANAGER_H_

#define IN
#define OUT
#define INOUT

#include "control/ZKBase.h"
#include "control/common.h"
#include "utils/VelocityTracker.h" //用于获取滑动速度
#include "system/Thread.h"
#include "app/BaseApp.h"

//最低滑动速度，滑动大于此速度则触发滑屏
#define TURN_PAGE_MIN_SPEED		0.0001

class SlideManager : public Thread {
public:
	enum SLIDE_DIR{NO_DIR = 0, VERTICAL = 1 /*垂直方向滑动*/, HORIZONTAL = 2 /*水平方向滑动*/};

private:
	BaseApp* m_win; 		//窗口指针
	SLIDE_DIR m_dir; 	//滑动方向
	int m_orgPos; 		//滑动的起始点
	int m_desPos;		//滑动的终点
	const int step = 10; //窗口滑动步进
	bool couldSliderWin;//是否可以滑动窗口标志位
	bool global_is_Slider; 	//是否可以滑动		外部全局控制接口

	VelocityTracker m_velocityTracker; //滑动速度追踪
private:
	SlideManager();
	virtual ~SlideManager();

	/*
	 * @brife 标记是否可以滑动界面
	 * @param flag 是否可以滑动界面标记
	 * */
	void setSlideWinFlag(IN const bool flag);

	/*
	 * @brife 获取是否可以滑动界面标志
	 * @return ture 可以滑动窗口； false 不能滑动窗口
	 * */
	OUT const bool GetSlideWinFlag(void);

public:
	/*
	 * @brife 获取类实例化对象指针
	 * @return 类实例化对象指针
	 * */
	OUT static SlideManager* getInstance(void);

	/*
	 * @brife 滑动窗口
	 * @param win 窗口指针
	 * @param dir 滑动方向
	 * @param move2Pos 滑动到达的位置
	 * */
	void sliderWin(IN BaseApp* win, IN const SLIDE_DIR dir, IN const int orgPos, IN const int desPos, IN const bool isSlide = true);

	/**
	 * @brief 线程开始运行时回调该接口
	 */
	virtual bool readyToRun();

	/**
	 * @brief 线程循环调用该接口
	 * @return true 不退出线程，false 将退出线程
	 */
	virtual bool threadLoop();

	/*
	 * @brife 从顶部拉下下拉框
	 * @param ev 触摸的坐标
	 * @param win 下拉的窗口
	 * @return true 窗口下拉； false 窗口不下拉
	 * */
	OUT const bool onTouchEvent(IN const MotionEvent &ev, IN BaseApp* win);

	/*
	 * @brife 是否可以滑动界面
	 * @param 是否可以滑动界面标记
	 * */
	void setCanSlide(IN const bool isSlide);


};
#define SLIDEMANAGER SlideManager::getInstance()

#endif /* JNI_MANAGER_SLIDEMANAGER_H_ */




















