/*
 * SlideManager.cpp
 *
 *  Created on: 2022年6月29日
 *      Author: chenxy
 */
#include "SlidingManager.h"
#include "utils/Log.h"

static Mutex SlideMutex;
slideWnd::slideWnd()
{

}

slideWnd::~slideWnd()
{
	EASYUICONTEXT->unregisterGlobalTouchListener(this);
}

void slideWnd::slideWnd_Init(ZKWindow *WndBox,vector<ZKWindow *> &wnds)
{
	Mutex::Autolock Slide_lock(SlideMutex);
	Wnds = wnds;
	if(wnds.size() == 0){
		LOGD("wnds.size must be greater than zero");
		return;
	}
    Wnd_l = WndBox->getPosition().mLeft;
    Wnd_t = WndBox->getPosition().mTop;
    Wnd_w = WndBox->getPosition().mWidth;
    Wnd_h = WndBox->getPosition().mHeight;
    if(Wnds.size() == 2)
    {
    	Pre_Wnd = 1;
    	Curr_Wnd = 0;
    	Next_Wnd = 1;
    }
    else if(Wnds.size() == 1){
    	Pre_Wnd = 0;
    	Curr_Wnd = 0;
    	Next_Wnd = 0;
    	Is_Cyclic = false;
    }else
    {
    	Pre_Wnd = Wnds.size()-1;
    	Curr_Wnd = 0;
    	Next_Wnd = 1;
    }
    slide_left = false;
    slide_right = false;
	Wnds[Pre_Wnd]->showWnd();
	Wnds[Pre_Wnd]->setPosition(LayoutPosition(-Wnd_w,0,Wnd_w,Wnd_h));
	Wnds[Next_Wnd]->showWnd();
	Wnds[Next_Wnd]->setPosition(LayoutPosition(Wnd_w,0,Wnd_w,Wnd_h));
	Wnds[Curr_Wnd]->showWnd();
	Wnds[Curr_Wnd]->setPosition(LayoutPosition(0,0,Wnd_w,Wnd_h));
	for(unsigned int i=0;i<Wnds.size();i++)
	{
		if(i==Pre_Wnd || i==Curr_Wnd || i==Next_Wnd)
			Wnds[i]->showWnd();
		else
			Wnds[i]->hideWnd();
	}
	IsInitDone = true;
}

void slideWnd::registerTouchListener(){
	EASYUICONTEXT->registerGlobalTouchListener(this);
	if(IsInitDone){
		Wnds[Curr_Wnd]->showWnd();
		Wnds[Curr_Wnd]->setPosition(LayoutPosition(0,0,Wnd_w,Wnd_h));
	}
}

void slideWnd::unregisterTouchListener(){
	EASYUICONTEXT->unregisterGlobalTouchListener(this);
	if(IsInitDone){
		Wnds[Curr_Wnd]->showWnd();
		Wnds[Curr_Wnd]->setPosition(LayoutPosition(0,0,Wnd_w,Wnd_h));
	}
}

bool slideWnd::readyToRun()
{
	return true;
}

bool slideWnd::threadLoop()
{
	Mutex::Autolock Slide_lock(SlideMutex);
	if(exitPending())
	{
		return false;
	}
	memset(slide_direct,0,12);
	LOGD("切换或复位 slide_left:%d slide_right:%d ",slide_left,slide_right);
	int breakflag = Wnd_w;
	//若当前显示为两端窗口且不可循环滑动或只有一个滑动窗口 则快速滑动无效
	if((!Is_Cyclic && fast_slide_flag &&
			((Curr_Wnd == 0)&&slide_right || (Curr_Wnd == (Wnds.size()-1))&&slide_left)) || Wnds.size() == 1)
		fast_slide_flag = false;
//	}
	if(((abs(slide_end_Posi_x - slide_start_Posi_x) > Wnd_w/3) && (Wnds.size() >= 2)) || fast_slide_flag)//满足切换条件
	{
		if(slide_left)//向左滑，下一个
		{
			while(1){
				if(Wnds[Next_Wnd]->getPosition().mLeft > 0)
				Wnds[Next_Wnd]->setPosition(LayoutPosition(Wnds[Next_Wnd]->getPosition().mLeft-1,0,Wnd_w,Wnd_h));
				if(Wnds[Curr_Wnd]->getPosition().mLeft > -Wnd_w)
				Wnds[Curr_Wnd]->setPosition(LayoutPosition(Wnds[Curr_Wnd]->getPosition().mLeft-1,0,Wnd_w,Wnd_h));
				usleep(SwitchSpeed);
				breakflag--;
				if((Wnds[Next_Wnd]->getPosition().mLeft == 0 && Wnds[Curr_Wnd]->getPosition().mLeft == -Wnd_w)
						|| breakflag==0)
					break;
			}
			Pre_Wnd = Curr_Wnd;
			Curr_Wnd = Next_Wnd;
			if(Next_Wnd < Wnds.size() - 1)
				Next_Wnd++;
			else
				Next_Wnd = 0;
		}
		else if(slide_right)//向右滑，上一个
		{
			while(1){
				if(Wnds[Pre_Wnd]->getPosition().mLeft < 0)
					Wnds[Pre_Wnd]->setPosition(LayoutPosition(Wnds[Pre_Wnd]->getPosition().mLeft+1,0,Wnd_w,Wnd_h));
				if(Wnds[Curr_Wnd]->getPosition().mLeft < Wnd_w)
					Wnds[Curr_Wnd]->setPosition(LayoutPosition(Wnds[Curr_Wnd]->getPosition().mLeft+1,0,Wnd_w,Wnd_h));
				usleep(SwitchSpeed);
				breakflag--;
				if((Wnds[Pre_Wnd]->getPosition().mLeft == 0 && Wnds[Curr_Wnd]->getPosition().mLeft == Wnd_w)
						|| breakflag==0)
					break;
			}
			Next_Wnd = Curr_Wnd;
			Curr_Wnd = Pre_Wnd;
			if(Pre_Wnd > 0)
				Pre_Wnd--;
			else
				Pre_Wnd = Wnds.size() - 1;
		}
		if(Wnds.size() > 2){
			Wnds[Pre_Wnd]->setPosition(LayoutPosition(-Wnd_w,0,Wnd_w,Wnd_h));
			Wnds[Next_Wnd]->setPosition(LayoutPosition(Wnd_w,0,Wnd_w,Wnd_h));
		}
		Wnds[Curr_Wnd]->setPosition(LayoutPosition(0,0,Wnd_w,Wnd_h));
		for(unsigned int i=0;i<Wnds.size();i++)
		{
			if(i == Pre_Wnd || i == Curr_Wnd || i == Next_Wnd){
				Wnds[i]->showWnd();
				continue;
			}
			Wnds[i]->hideWnd();
		}
		//滑动切换完成回调
		if(SlideListener != NULL){
			SlideListener(slide_right?SLIDEDONETYPE_RIGHT:SLIDEDONETYPE_LEFT);
		}
		slide_right = false;
		slide_left = false;
		slide_start_Posi_x = -1;
		slide_direct_index = 0;
		fast_slide_flag = false;

		return false;
	}
	else
	{
		if(slide_left)
		{
			while(1){
				breakflag--;
				if((!Is_Cyclic && (Curr_Wnd == (Wnds.size() - 1) || Curr_Wnd==0)) || (Wnds.size() == 1)){
					if(Wnds[Curr_Wnd]->getPosition().mLeft < 0)
					Wnds[Curr_Wnd]->setPosition(LayoutPosition(Wnds[Curr_Wnd]->getPosition().mLeft+1,0,Wnd_w,Wnd_h));
					if(Wnds[Curr_Wnd]->getPosition().mLeft == 0)
						break;
				}else{
					if(Wnds[Next_Wnd]->getPosition().mLeft < Wnd_w)
					Wnds[Next_Wnd]->setPosition(LayoutPosition(Wnds[Next_Wnd]->getPosition().mLeft+1,0,Wnd_w,Wnd_h));
					if(Wnds[Curr_Wnd]->getPosition().mLeft < 0)
					Wnds[Curr_Wnd]->setPosition(LayoutPosition(Wnds[Curr_Wnd]->getPosition().mLeft+1,0,Wnd_w,Wnd_h));
					if(Wnds[Next_Wnd]->getPosition().mLeft == Wnd_w && Wnds[Curr_Wnd]->getPosition().mLeft == 0)
						break;
				}
				if(breakflag == 0)
					break;
				usleep(SwitchSpeed);

			}
		}
		else if(slide_right)
		{
			while(1){
				breakflag--;
				if((!Is_Cyclic && ((Curr_Wnd == (Wnds.size() - 1)) || Curr_Wnd==0)) || (Wnds.size() == 1)){
					if(Wnds[Curr_Wnd]->getPosition().mLeft > 0)
					Wnds[Curr_Wnd]->setPosition(LayoutPosition(Wnds[Curr_Wnd]->getPosition().mLeft-1,0,Wnd_w,Wnd_h));
					if(Wnds[Curr_Wnd]->getPosition().mLeft == 0)
						break;
				}else{
					if(Wnds[Pre_Wnd]->getPosition().mLeft > -Wnd_w)
					Wnds[Pre_Wnd]->setPosition(LayoutPosition(Wnds[Pre_Wnd]->getPosition().mLeft-1,0,Wnd_w,Wnd_h));
					if(Wnds[Curr_Wnd]->getPosition().mLeft > 0)
					Wnds[Curr_Wnd]->setPosition(LayoutPosition(Wnds[Curr_Wnd]->getPosition().mLeft-1,0,Wnd_w,Wnd_h));
					if(Wnds[Pre_Wnd]->getPosition().mLeft == -Wnd_w && Wnds[Curr_Wnd]->getPosition().mLeft == 0)
						break;
				}
				if(breakflag==0)
					break;
				usleep(SwitchSpeed);

			}
		}
		Wnds[Pre_Wnd]->setPosition(LayoutPosition(-Wnd_w,0,Wnd_w,Wnd_h));
		Wnds[Next_Wnd]->setPosition(LayoutPosition(Wnd_w,0,Wnd_w,Wnd_h));
		Wnds[Curr_Wnd]->showWnd();
		Wnds[Curr_Wnd]->setPosition(LayoutPosition(0,0,Wnd_w,Wnd_h));
		slide_right = false;
		slide_left = false;
		slide_start_Posi_x = -1;
		slide_direct_index = 0;
		return false;
	}
	return true;
}

void slideWnd::RegisterSlideListener(void (*slidelistener)(SLIDEDONETYPE SDT)){
	SlideListener = slidelistener;
}

void slideWnd::slideWnd_handle(const MotionEvent &ev)
{
	if(isRunning() || !IsInitDone)
		return;
	Mutex::Autolock Slide_lock(SlideMutex);
	R_X = ev.mX - Wnd_l;//转换成Frame滑动框的x坐标
	R_Y = ev.mY - Wnd_t;//转换成Frame滑动框的y坐标
	addMovement(ev);
    switch (ev.mActionStatus) {
		case MotionEvent::E_ACTION_DOWN://触摸按下
			//LOGD("时刻 = %ld 坐标  x = %d, y = %d", ev.mEventTime, ev.mX, ev.mY);
			memset(slide_direct,0,12);
			break;
		case MotionEvent::E_ACTION_MOVE://触摸滑动
		{
		if(R_X >= 0 && R_X <= Wnd_w && R_Y >= 0 && R_Y <= Wnd_h)//触摸在滑动框内
		{
			static int LastR_X = R_X,LastR_Y = R_Y;
			if((abs(LastR_X-R_X) > 10) && (abs(LastR_Y-R_Y) > 10) && (slide_right || slide_left))
				return;
			LastR_X = R_X;
			LastR_Y = R_Y;

			if(slide_direct_index < 3)//滑动方向下标
			{
				slide_direct[slide_direct_index] = R_X;
				slide_direct_index++;
			}
			else{
				memset(slide_direct,0,12);
				slide_direct_index = 0;
			}

			if(slide_direct[0] < slide_direct[1] && slide_direct[1] < slide_direct[2])//向右滑
			{
				LOGD("向右滑！%d : %d : %d",slide_direct[0],slide_direct[1],slide_direct[2]);
				if(slide_left)//上一次左滑未松开则复位左滑相关变量
				{
					if(Wnds[Curr_Wnd]->getPosition().mLeft >= 0)
					{
						slide_left = false;
						slide_right = true;
						slide_start_Posi_x = -1;
						reset();
					}
				}
				else
					slide_right = true;


				if(slide_start_Posi_x < 0)//判断到开始右滑，每次滑动只记录一次起始位置X坐标
				{
					slide_start_Posi_x = R_X;//记录滑动起始位置X坐标
					//滑动开始设置窗口内所有控件不可触摸
					vector<ZKBase *> base;
					Wnds[Curr_Wnd]->getAllControls(base);
					for(unsigned int i=0;i<base.size();i++)
					base[i]->setInvalid(true);
				}
			}
			else if(slide_direct[0] > slide_direct[1] && slide_direct[1] > slide_direct[2])//向左滑
			{
				LOGD("向左滑！%d : %d : %d",slide_direct[0],slide_direct[1],slide_direct[2]);
				if(slide_right)//上一次右滑未松开则复位右滑相关变量
				{
					if(Wnds[Curr_Wnd]->getPosition().mLeft <= 0)
					{
						slide_left = true;
						slide_right = false;
						slide_start_Posi_x = -1;
						reset();
					}
				}
				else
					slide_left = true;
				if(slide_start_Posi_x < 0)//判断到开始左滑
				{
					slide_start_Posi_x = R_X;
					//滑动开始设置窗口内所有控件不可触摸
					vector<ZKBase *> base;
					Wnds[Curr_Wnd]->getAllControls(base);
					for(unsigned int i=0;i<base.size();i++)
						base[i]->setInvalid(true);
				}
			}

			if(slide_left)
			{
//				LOGD("-----slide_left-----\n");
				if(((Curr_Wnd == (Wnds.size() - 1)) && !Is_Cyclic) || (Wnds.size() == 1))//不可循环或窗口数小于2个
				{
					if((Wnd_w*(slide_start_Posi_x-R_X))/Wnd_w <= Wnd_w/4)//只可滑动1/4距离
					{
						slide_end_Posi_x = R_X;//Wnds[Next_Wnd]->setPosition(LayoutPosition(Wnd_w-(Wnd_w*(slide_start_Posi_x-R_X))/Wnd_w,0,Wnd_w,Wnd_h));
						Wnds[Curr_Wnd]->setPosition(LayoutPosition(-(Wnd_w*(slide_start_Posi_x-R_X))/Wnd_w,0,Wnd_w,Wnd_h));
					}
				}
				else
				{

				Wnds[Next_Wnd]->setPosition(LayoutPosition(Wnd_w-(Wnd_w*(slide_start_Posi_x-R_X))/Wnd_w,0,Wnd_w,Wnd_h));
				Wnds[Curr_Wnd]->setPosition(LayoutPosition(-(Wnd_w*(slide_start_Posi_x-R_X))/Wnd_w,0,Wnd_w,Wnd_h));
				}
			}
			else if(slide_right)
			{
//				LOGD("-----slide_right-----\n");
				if((Curr_Wnd == 0 && !Is_Cyclic) || (Wnds.size() == 1))//不可循环或窗口数小于2个
				{
					if(R_X-slide_start_Posi_x <= Wnd_w/4)//只可滑动1/4距离
					{
						slide_end_Posi_x = R_X;//记录结束位置X坐标
						Wnds[Curr_Wnd]->setPosition(LayoutPosition((Wnd_w*(R_X-slide_start_Posi_x))/Wnd_w,0,Wnd_w,Wnd_h));
					}
				}
				else
				{

				Wnds[Pre_Wnd]->setPosition(LayoutPosition(-Wnd_w+(Wnd_w*(R_X-slide_start_Posi_x))/Wnd_w,0,Wnd_w,Wnd_h));
				Wnds[Curr_Wnd]->setPosition(LayoutPosition((Wnd_w*(R_X-slide_start_Posi_x))/Wnd_w,0,Wnd_w,Wnd_h));
				}
			}
		}
		}
			break;
		case MotionEvent::E_ACTION_UP:  //触摸抬起
		{
			LOGD("触摸抬起！%d : %d : %d",slide_direct[0],slide_direct[1],slide_direct[2]);
			//恢复窗口内所有控件触摸状态
			vector<ZKBase *> base;
			Wnds[Curr_Wnd]->getAllControls(base);
			for(unsigned int i=0;i<base.size();i++)
				base[i]->setInvalid(false);
			memset(slide_direct,0,12);
			LOGD("触摸抬起！%d : %d : %d",slide_direct[0],slide_direct[1],slide_direct[2]);
			LOGD("触摸抬起! slide_left:%d slide_right:%d ",slide_left,slide_right);
			if(slide_left || slide_right)
			{
				LOGD("slide_left || slide_right");
				//获取滑动速度
				float vspeed = getXVelocity();
				if(vspeed < 0)
					vspeed -= 2*vspeed;
				LOGD("-----vspeed : %f-----\n",vspeed);
				if(Speed <= vspeed)
					fast_slide_flag = true;
				else
					fast_slide_flag = false;
				reset();

				if(!Is_Cyclic){//不可循环或滑动窗口只有1个时
//					if(R_X >= 0 && R_X <= Wnd_w)//X坐标在滑动框内   && R_Y >= 0 && R_Y <= Wnd_h
					if((Curr_Wnd == 0 && slide_right) || ((Curr_Wnd == Wnds.size()-1) && slide_left))
						slide_end_Posi_x = slide_start_Posi_x;
					else
						slide_end_Posi_x = R_X;
//					else
//						slide_end_Posi_x = slide_left?0:Wnd_w;
				}
				else
				{
					if(slide_right && Curr_Wnd == Wnds.size()-1)//最后一个窗口右滑
						slide_end_Posi_x = R_X;
					else if(Curr_Wnd == 0 && slide_left)//第一个窗口左滑
						slide_end_Posi_x = R_X;
					else if(Curr_Wnd != 0 && Curr_Wnd != Wnds.size()-1)//中间窗口滑动
						slide_end_Posi_x = R_X;
				}

				slideWnd::run();
			}else{
				if(Wnds.size() > 2){
					Wnds[Pre_Wnd]->setPosition(LayoutPosition(-Wnd_w,0,Wnd_w,Wnd_h));
					Wnds[Next_Wnd]->setPosition(LayoutPosition(Wnd_w,0,Wnd_w,Wnd_h));
				}
				Wnds[Curr_Wnd]->showWnd();
				Wnds[Curr_Wnd]->setPosition(LayoutPosition(0,0,Wnd_w,Wnd_h));
			}
			reset();
		}
			break;
		default:
			break;
    }
}

bool slideWnd::onTouchEvent(const MotionEvent &ev) {
	slideWnd_handle(ev);
	return false;
}

void slideWnd::SetIsInitDone(bool IsInitDone){
	this->IsInitDone = IsInitDone;
}

void slideWnd::Set_Slide_Cyclic(bool Is_cyclic)
{
	if(Wnds.size() < 2 && Is_cyclic){
		LOGD("Can't Set_Slide_Cyclic: Wnds.size() < 2");
		return;
	}

	this->Is_Cyclic = Is_cyclic;
}

void slideWnd::Set_SwitchSpeed(int switchspeed){
	this->SwitchSpeed = switchspeed;
}

int slideWnd::Get_Pre_Wnd_Index(){
	return Pre_Wnd;
}

int slideWnd::Get_Curr_Wnd_Index(){
	return Curr_Wnd;
}

int slideWnd::Get_Next_Wnd_Index(){
	return Next_Wnd;
}

void slideWnd::Next_Wndow()
{
	if(Wnds.size() < 2){
		LOGD("Can't Switch Next Wnd : Wnds.size() < 2");
		return;
	}
	if(isRunning()){
		LOGD("Can't Switch Pre Wnd : Is Switching");
		return;
	}
	slide_left = true;
	fast_slide_flag = true;
	slide_start_Posi_x = 0;
	slide_end_Posi_x = 0;
	slideWnd::run();
}

void slideWnd::Pre_Wndow()
{
	if(Wnds.size() < 2){
		LOGD("Can't Switch Pre Wnd : Wnds.size() < 2");
		return;
	}
	if(isRunning()){
		LOGD("Can't Switch Pre Wnd : Is Switching");
		return;
	}
	slide_right = true;
	fast_slide_flag = true;
	slide_start_Posi_x = 0;
	slide_end_Posi_x = 0;
	slideWnd::run();
}


