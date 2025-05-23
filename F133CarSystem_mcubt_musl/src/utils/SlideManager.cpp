/*
 * SlideManager.cpp
 *
 *  Created on: 2021年10月10日
 *      Author: Administrator
 */

#include "SlideManager.h"

#include "utils/log.h"

#include <math.h>

#define DEBUG

SlideManager::SlideManager() {
	// TODO 自动生成的构造函数存根
	m_win = NULL;
	m_dir = NO_DIR;
	m_orgPos = 0;
	m_desPos = 0;
	couldSliderWin = true;
	global_is_Slider = true;
}

SlideManager::~SlideManager() {
	// TODO 自动生成的析构函数存根
	//等待退出线程
	if(true == isRunning())
	{
		requestExitAndWait();
	}
}

OUT SlideManager* SlideManager::getInstance(void)
{
	static SlideManager SM;
	return &SM;
}

void SlideManager::sliderWin(IN BaseApp* win, IN const SLIDE_DIR dir, IN const int orgPos, IN const int desPos, IN const bool isSlide)
{
	m_win = win;
	m_dir = dir;
	m_orgPos = orgPos;
	m_desPos = desPos;
//	LOGD("--sliderWin()---dir:%d-- --m_orgPos:%d--, --m_desPos:%d--", dir, m_orgPos, m_desPos);
	if(false == isSlide)
	{
		LayoutPosition pos = m_win->getPosition();
		//垂直方向滑动
		if(VERTICAL == m_dir)
		{
			pos.mTop = m_desPos;
			//移动窗口位置
			m_win->setPosition(pos);
		}
		//水平方向滑动
		else if(HORIZONTAL == m_dir)
		{
			pos.mLeft = m_desPos;
			//移动窗口位置
			m_win->setPosition(pos);
		}
		else
		{
			LOGD("--%d-- --%s--\n 未能识别方向：%d", __LINE__, __FILE__, m_dir);
		}
		//编辑可以滑屏
		setSlideWinFlag(true);
		return;
	}
	//启动线程
	run();
}

bool SlideManager::readyToRun()
{
	LayoutPosition pos = m_win->getPosition();
	//垂直方向滑动
	if(VERTICAL == m_dir)
	{
		pos.mTop = m_orgPos;
		//移动窗口位置
		m_win->setPosition(pos);
	}
	//水平方向滑动
	else if(HORIZONTAL == m_dir)
	{
		pos.mLeft = m_orgPos;
		//移动窗口位置
		m_win->setPosition(pos);
	}
	else
	{
		LOGD("--%d-- --%s--\n 未能处理滑动方向：%d", __LINE__, __FILE__, m_dir);
		return false;
	}

//	m_win->setVisible(true);

	//标记是否可以滑动窗口
	setSlideWinFlag(0 != m_desPos);
	DEBUG
	return true;
}

bool SlideManager::threadLoop()
{DEBUG
	usleep(1000);
	LayoutPosition pos = m_win->getPosition();
	//判断滑动方向
	if(VERTICAL == m_dir) //垂直方向滑动
	{
		//向上滑动
		if(m_desPos < pos.mTop)
		{
			pos.mTop -= step;
			if(m_desPos < pos.mTop)
			{
				//移动窗口位置
				m_win->setPosition(pos);
				return true;
			}
			else
			{
				pos.mTop = m_desPos;
				//移动窗口位置
				m_win->setPosition(pos);
				return false;
			}
		}
		//向下滑动
		else if(m_desPos > pos.mTop)
		{
			pos.mTop += step;
			if(m_desPos > pos.mTop)
			{
				//移动窗口位置
				m_win->setPosition(pos);
				return true;
			}
			else
			{
				pos.mTop = m_desPos;
				//移动窗口位置
				m_win->setPosition(pos);
				return false;
			}
		}
	}
	else if(HORIZONTAL == m_dir) //水平方向滑动
	{
		//向左滑动
		if(m_desPos < pos.mLeft)
		{
			pos.mLeft -= step;
			if(m_desPos < pos.mLeft)
			{
				//移动窗口位置
				m_win->setPosition(pos);
				return true;
			}
			else
			{
				pos.mLeft = m_desPos;
				//移动窗口位置
				m_win->setPosition(pos);
				return false;
			}
		}
		//向右滑动
		else if(m_desPos > pos.mLeft)
		{
			pos.mLeft += step;
			if(m_desPos > pos.mLeft)
			{
				//移动窗口位置
				m_win->setPosition(pos);
				return true;
			}
			else
			{
				pos.mLeft = m_desPos;
				//移动窗口位置
				m_win->setPosition(pos);
				return false;
			}
		}
	}
	else
	{
		LOGD("--%d-- --%s--\n 未能处理滑动方向：%d", __LINE__, __FILE__, m_dir);
	}

	return false;
}

OUT const bool SlideManager::onTouchEvent(IN const MotionEvent &ev, IN BaseApp* win)
{
//	LOGD("--%d-- --%s-- --global_is_Slider:%d--", __LINE__, __FILE__, global_is_Slider);
	if (!global_is_Slider) {
		return false;
	}
	//视为滑动距离
	const int jugdeSlider = 10;

	//滑动速度
	float speed = 0.0;
	//标记点击位置
	static MotionEvent clickPoint = ev;
	//界面初始位置
	static LayoutPosition winOrgPos = win->getPosition();

	//由于界面中存在横向滑动，因此判断先触发的是哪个方向的滑动
	static SLIDE_DIR dir = NO_DIR;

	//标记是否从顶部下拉,只有从顶部下拉才能下滑下拉框
	static bool isPullFromTop = false;

	switch (ev.mActionStatus) {
		case MotionEvent::E_ACTION_DOWN://触摸按下
			clickPoint = ev;
			winOrgPos = win->getPosition();

			if(50>ev.mY)
			{
				isPullFromTop = true;
				setSlideWinFlag(false);
			}
			else
			{
				isPullFromTop = false;
			}
			break;
		case MotionEvent::E_ACTION_MOVE://触摸滑动
			// 用于计算滑动速度
			m_velocityTracker.addMovement(ev);

			//界面跟着手滑动
			if(VERTICAL == dir)
			{
				//滑动距离
				int slideLength = ev.mY - clickPoint.mY;
				//向下滑动(当mTop为0时，第一次不能下滑)
//				LOGD("--slideLength:%d-- --winOrgPos.mHeight:%d-- --winOrgPos.mTop:%d-- --isPullFromTop:%d--",
//						slideLength, winOrgPos.mHeight, winOrgPos.mTop, isPullFromTop);
				if(0 < slideLength && -winOrgPos.mHeight == winOrgPos.mTop && true == isPullFromTop)
				{
					sliderWin(win, VERTICAL, -winOrgPos.mHeight, -winOrgPos.mHeight + slideLength, false);
				}
				//向上滑动(下滑了才能上滑)
				else if(0 > slideLength && 0 == winOrgPos.mTop)
				{
					sliderWin(win, VERTICAL, 0, slideLength, false);
				}
				else {
					sliderWin(win, VERTICAL, 0, winOrgPos.mTop, false);
				}
			}
			break;
		case MotionEvent::E_ACTION_UP:  //触摸抬起
		{
			setSlideWinFlag(true);
			// 获取y移动速度
			speed = m_velocityTracker.getYVelocity();
			//复位获取速度对象
			m_velocityTracker.reset();

			//滑动速度大于设定速度或滑动距离超过屏幕的1/3，则使能上下自动滑屏
			int slideLength = ev.mY - clickPoint.mY;

//			LOGD("--slideLength:%d-- --isPullFromTop:%d-- --dir:%d--", slideLength, isPullFromTop, dir);
			if((fabs(speed) > TURN_PAGE_MIN_SPEED || win->getPosition().mHeight/3 <= abs(slideLength)))
			{
				//开启滑动线程
				if(VERTICAL == dir)
				{
					LayoutPosition pos = win->getPosition();
					//向下滑动(当mTop为0时，第一次不能下滑)
					if(0 < slideLength && true == isPullFromTop)
					{
						sliderWin(win, VERTICAL, pos.mTop, 0, true);
					}
					//向上滑动
					else if(0 > slideLength)
					{
						sliderWin(win, VERTICAL, pos.mTop, -win->getPosition().mHeight, true);
					}
					else
					{
//						LOGD("--%d-- --%s-- slideLength:%d topPos:%d pos.mTop:%d isPullFromTop:%d \n",
//							__LINE__, __FILE__, slideLength, topPos, pos.mTop, isPullFromTop);
					}
				}
				else
				{
					//下拉框回位
					win->setPosition(winOrgPos);
					//LOGD("--%d-- --%s-- 复位！！ \n", __LINE__, __FILE__);
				}
			}
			else
			{
				//下拉框回位
				win->setPosition(winOrgPos);
				//LOGD("--%d-- --%s-- 复位！！ \n", __LINE__, __FILE__);
			}
		}
			break;
		default:
			win->setPosition(winOrgPos);
			LOGD("--%d-- --%s-- 复位！！ \n", __LINE__, __FILE__);
			break;
	}

	const int xMoveLen = ev.mX - clickPoint.mX;
	const int yMoveLen = ev.mY - clickPoint.mY;

	if(abs(xMoveLen) >= jugdeSlider && NO_DIR == dir)
	{
		dir = HORIZONTAL;
	}
	else if(abs(yMoveLen) >= jugdeSlider && NO_DIR == dir)
	{
		dir = VERTICAL;
	}
	else if(abs(xMoveLen) < jugdeSlider && abs(yMoveLen) < jugdeSlider)
	{
		dir = NO_DIR;
	}
//	LOGD("--%d-- --%s-- xMoveLen:%d yMoveLen：%d dir:%d \n", __LINE__, __FILE__, xMoveLen, yMoveLen,dir);
	if(VERTICAL == dir)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void SlideManager::setSlideWinFlag(IN const bool flag)
{
	couldSliderWin = flag;
}

OUT const bool SlideManager::GetSlideWinFlag(void)
{
	return couldSliderWin;
}

void SlideManager::setCanSlide(IN const bool isSlide) {
	global_is_Slider = isSlide;
}










