/*
 * Loading_icon.hpp
 *
 *  Created on: 2023年12月14日
 *      Author: Admin
 */

#ifndef JNI_UTILS_LOADING_ICON_HPP_
#define JNI_UTILS_LOADING_ICON_HPP_

#include "system/Thread.h"
#include "utils/log.h"

namespace zkf {

// 利用仪表控件实现旋转加载效果
class IconRotate : public Thread
{
public:
	/*
	 * mpointerPtr, 仪表控件
	 * basePtr, 仪表所在的窗口控件，控制显示隐藏，，(模态)
	 */
	void SetCtrl(ZKPointer* mpointerPtr, ZKWindow* windowPtr) {
		_mpointerPtr = mpointerPtr;
		_windowPtr = windowPtr;
	}

private:
	virtual bool readyToRun() {
		if (_windowPtr) {
			_windowPtr->setVisible(true);
		}
		return true;
	}

	virtual bool threadLoop(void) {
		int angle = 0;
		while(!exitPending()) {
			++angle;
			if (_mpointerPtr) {
				_mpointerPtr->setTargetAngle(angle%360);
			}
			usleep(1000*2);
		}
		_windowPtr->setVisible(false);
		return false;
	}

private:
	ZKPointer* _mpointerPtr;
	ZKWindow* _windowPtr;
};

}



#endif /* JNI_UTILS_LOADING_ICON_HPP_ */
