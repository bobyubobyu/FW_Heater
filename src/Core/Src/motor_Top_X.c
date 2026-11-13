/*
 * motor_Top_X.c
 *
 *  Created on: Nov 25, 2025
 *      Author: Bob Yu
 */
#include "main.h"

const	uint16_t	CtrlM4_Accel[] =
{
	#include "motor_Top_X.d"
};
#define	STEP_OF_ACCEL	(sizeof(CtrlM4_Accel) / sizeof(uint16_t))

//-----------------------------------------------
// LIMIT  運動到達極限停止
// STEP_OVER_LIMIT 抵達極限以後，多走N步
// PROTECT_STEP    尚未抵達極限前已經走了N步則保護
//-----------------------------------------------
// COUNT  運動幾步
//-----------------------------------------------
// TIME   運動多久
//-----------------------------------------------
// CONTINUOUS  持續運動
//-----------------------------------------------
// STOP  停止
//-----------------------------------------------
// 共通
// OCP   過流
//-----------------------------------------------
enum
{
	M4_ACT_IDLE = 0,
	M4_ACT_CW_LIMIT,
	M4_ACT_CW_COUNT,
	M4_ACT_CW_TIME,
	M4_ACT_CW_CONTINUOUS,

	M4_ACT_CCW_LIMIT,
	M4_ACT_CCW_COUNT,
	M4_ACT_CCW_TIME,
	M4_ACT_CCW_CONTINUOUS,

	M4_ACT_FINISH,

	NUM_OF_M4_ACT
};

uint8_t	 xVarM4_Action;
uint16_t xVarM4_Speed;

//---------------------------------------------
//  LIMIT Define / 變數
//---------------------------------------------
#define PROTECT_STEP_FLAG		BIT0
#define OVER_LIMIT_STEP_FLAG	BIT1
uint8_t  xVarM4_Flag_Switch = 0;
uint32_t xVarM4_Protect_Step_Target;
uint32_t xVarM4_Over_Limit_Step_Target;
uint32_t xVarM4_Protect_Step_Now;
uint32_t xVarM4_Over_Limit_Step_Now;
uint8_t  xVarM4_Stop_Flag;
//---------------------------------------------
// COUNT Define / 變數
uint32_t xVarM4_Count_Target;
uint32_t xVarM4_Count_Now;
//---------------------------------------------
// CONTINUOUS Define / 變數
// NONE
//---------------------------------------------
//TIME  Define / 變數
uint32_t xVarM4_Time_Target;
uint32_t xVarM4_Time_Now;

//---------------------------------------------
//速度控制
uint32_t xVarM4_user_Speed;
uint8_t xVarM4_user_Speed_Flag;

void M4_IO_Init(void)
{
    //Needn't
}


void M4_Set_Var_Total_Initial(void)
{
	xVarM4_Flag_Switch = 0;
	xVarM4_Stop_Flag = 0;
	xVarM4_Protect_Step_Target = 0;
	xVarM4_Over_Limit_Step_Target = 0;
	xVarM4_Protect_Step_Now = 0;
	xVarM4_Over_Limit_Step_Now = 0;
	xVarM4_Count_Target = 0;
	xVarM4_Count_Now = 0;
	xVarM4_Time_Target = 0;
	xVarM4_Time_Now = 0;
	xVarM4_Speed = 0;
	xVarM4_user_Speed = 0;
	xVarM4_user_Speed_Flag = 0;
}

void M4_Set_Var_Start_Initial(void)
{
	xVarM4_Speed = 0;
	xVarM4_Stop_Flag = 0;
	xVarM4_Protect_Step_Now = 0;
	xVarM4_Over_Limit_Step_Now = 0;
	xVarM4_Count_Now = 0;
	xVarM4_Time_Now = 0;
}

//-----------------------------------------------
//  LIMIT Function
//------------------------------------------------
//設定保護距離
//設定一次以後有效
void M4_Set_Protect_Step(uint8_t on_off,uint32_t val)
{
	if(on_off)
	{
		xVarM4_Flag_Switch |= PROTECT_STEP_FLAG;
		xVarM4_Protect_Step_Target = val;
	}
	else
	{
		xVarM4_Flag_Switch &= ~PROTECT_STEP_FLAG;
		xVarM4_Protect_Step_Target = val;
	}
}
//設定超越極限後多少距離停
//設定一次以後有效
void M4_Set_Over_Limit_Step(uint8_t on_off,uint32_t val)
{
	if(on_off)
	{
		xVarM4_Flag_Switch |= OVER_LIMIT_STEP_FLAG;
		xVarM4_Over_Limit_Step_Target = val;
	}
	else
	{
		xVarM4_Flag_Switch &= ~OVER_LIMIT_STEP_FLAG;
		xVarM4_Over_Limit_Step_Target = val;
	}
}

//--------------------------------------------------
void M4_Set_user_Speed(uint8_t user_spd_flag , uint32_t user_spd)
{
	xVarM4_user_Speed = user_spd;
	xVarM4_user_Speed_Flag = user_spd_flag;
}

//--------------------------------------------------

//--------------------------------------------------
//  COUNT Function
//--------------------------------------------------
//設定運動的COUNT數
void M4_Set_Count(uint32_t val)
{
	xVarM4_Count_Target = val;
}

//--------------------------------------------------------------
// Time Function
// 設定好Timer之後，把M4_Incrase_Time放進去
// 時間就會不斷增加，再比對xVarM4_Time_Target和xVarM4_Time_Now即可
//--------------------------------------------------------------
// 設定運動的時間
void M4_Set_Time(uint32_t val)
{
	xVarM4_Time_Target = val;
}
// 增加時間單位
void M4_Incrase_Time(uint32_t val)
{
	xVarM4_Time_Now += val;
}

uint8_t M4_Get_Stop(void)
{
	return xVarM4_Stop_Flag;
}

void CtrlM4_Start(uint8_t Action)
{
	xVarM4_Action = M4_ACT_IDLE;
	//決定方向
	if (Action == M4_ACT_CW_LIMIT  ||
		Action == M4_ACT_CW_COUNT  ||
		Action == M4_ACT_CW_TIME   ||
		Action == M4_ACT_CW_CONTINUOUS
	   )
	{
		HWO_M4_FORWARD;
	}
	else if (Action == M4_ACT_CCW_LIMIT  ||
			 Action == M4_ACT_CCW_COUNT  ||
			 Action == M4_ACT_CCW_TIME   ||
			 Action == M4_ACT_CCW_CONTINUOUS
			)
	{
	    HWO_M4_BACKWARD;
	}
	else
		return;

	//設定參數
	M4_Set_Var_Start_Initial();
	//設置PLUSE,電流IO
	HWO_M4_LOW;
	HWO_M4_ENABLE;

	xVarM4_Action = Action;
	//設置每一步的時間
	if(xVarM4_user_Speed_Flag == 1)
	{
		HWO_M4_TIME(xVarM4_user_Speed);
	}
	else
		HWO_M4_TIME(CtrlM4_Accel[xVarM4_Speed]);
	//開啟TIMER中斷
	HWO_M4_EINT;
}

void CtrlM4_Stop(void)
{
	HWO_M4_DINT;
	HWO_M4_DISABLE;
	xVarM4_Action = M4_ACT_IDLE;
}

void CtrlM4_Finish(void)
{
	xVarM4_Stop_Flag = 1;
	HWO_M4_DINT;
	xVarM4_Action = M4_ACT_FINISH;
}

void CtrlM4_Error(uint8_t State)
{
	CtrlM4_Stop();
	// SysError_Set(State);
}

void CtrlM4_Speed(void)
{
	if (xVarM4_Action != M4_ACT_IDLE)
	{
		if (xVarM4_Speed < (STEP_OF_ACCEL - 1))
		{
			xVarM4_Speed ++;
		}
	}
}

//--------------------------------------------------------
// 放在Timer裡，作為Toggle步進馬達的副程式
// Toggle是LOW動作，也就是兩個Toggle才走一步
// 可在收取變數的時候用副程式*2
// 或是修改此副程式為兩次才計數
// 因為若為四/六線式單步激磁的話，此處的每個Toggle就將會是一步
//--------------------------------------------------------
void CtrlM4_Trigger(void)
{
	switch (xVarM4_Action)
	{
		case M4_ACT_CW_LIMIT:
		case M4_ACT_CCW_LIMIT:
		{
			HWO_M4_TRIGGER;
			if(xVarM4_user_Speed_Flag == 1)
			{
				HWO_M4_TIME(xVarM4_user_Speed);
			}
			else
			{
				HWO_M4_TIME(CtrlM4_Accel[xVarM4_Speed]);
			}

			if(xVarM4_Flag_Switch & PROTECT_STEP_FLAG) //失步保護
			{
				xVarM4_Protect_Step_Now++;
				if(xVarM4_Protect_Step_Now >= xVarM4_Protect_Step_Target)  //抵達極限以前超過了保護步數，判定為失步
				{
					//增加保護功能旗標
					xVarM4_Stop_Flag = 1;  //xVarM4_Stop_Flag可依情況判斷是否要放入	CtrlM4_Stop() 中
					CtrlM4_Stop();
					break;
				}
			}

			if(xVarM4_Flag_Switch & OVER_LIMIT_STEP_FLAG)  //觸碰到極限後多走步數
			{
				if(xVarM4_Over_Limit_Step_Now > 0)
				{
					xVarM4_Over_Limit_Step_Now++;
				}
				else if( ((xVarM4_Action == M4_ACT_CW_LIMIT)  && HWI_M4_CW_LIMIT ) 	||
					   	 ((xVarM4_Action == M4_ACT_CCW_LIMIT) && HWI_M4_CCW_LIMIT)
					   )
				{
					xVarM4_Over_Limit_Step_Now = 1;
				}

				if(xVarM4_Over_Limit_Step_Now >= xVarM4_Over_Limit_Step_Target)
				{
					xVarM4_Stop_Flag = 1;
					CtrlM4_Stop();
				}
			}
			else  //觸碰到極限馬上停止
			{
				if( ((xVarM4_Action == M4_ACT_CW_LIMIT)  && HWI_M4_CW_LIMIT ) 	||
					((xVarM4_Action == M4_ACT_CCW_LIMIT) && HWI_M4_CCW_LIMIT)
				  )
				{
					xVarM4_Stop_Flag = 1;
					CtrlM4_Stop();
				}
			}
			break;
		}

		case M4_ACT_CW_COUNT:
		case M4_ACT_CCW_COUNT:
		{
			HWO_M4_TRIGGER;
			if(xVarM4_user_Speed_Flag == 1)
			{
				HWO_M4_TIME(xVarM4_user_Speed);
			}
			else
			{
				HWO_M4_TIME(CtrlM4_Accel[xVarM4_Speed]);
			}
			xVarM4_Count_Now++;
			if((xVarM4_Count_Now >= xVarM4_Count_Target) 					||
			   ((xVarM4_Action == M4_ACT_CW_COUNT)  && HWI_M4_CW_LIMIT ) 	||
			   ((xVarM4_Action == M4_ACT_CCW_COUNT) && HWI_M4_CCW_LIMIT)
			  )
			{
				xVarM4_Stop_Flag = 1;
				CtrlM4_Stop();
			}
			break;
		}

		case M4_ACT_CW_CONTINUOUS:
		case M4_ACT_CCW_CONTINUOUS:
		{
			HWO_M4_TRIGGER;
			if(xVarM4_user_Speed_Flag == 1)
			{
				HWO_M4_TIME(xVarM4_user_Speed);
			}
			else
			{
				HWO_M4_TIME(CtrlM4_Accel[xVarM4_Speed]);
			}
			break;
		}

		case M4_ACT_CW_TIME:
		case M4_ACT_CCW_TIME:
		{
			if(xVarM4_Time_Now >= xVarM4_Time_Target)
			{
				xVarM4_Stop_Flag = 1;
				CtrlM4_Stop();
				break;
			}
			HWO_M4_TRIGGER;
			if(xVarM4_user_Speed_Flag == 1)
			{
				HWO_M4_TIME(xVarM4_user_Speed);
			}
			else
			{
				HWO_M4_TIME(CtrlM4_Accel[xVarM4_Speed]);
			}
			break;
		}

		default:
			break;
	}
}

void CtrlM4_Initial(void)
{
	M4_IO_Init();
	M4_Set_Var_Total_Initial();
	CtrlM4_Stop();
}

void CtrlM4_Behavior_Goto_HOME()
{
	if (HWI_M4_CCW_LIMIT)
	{
		CtrlM4_Finish();
	}
	else
	{
		CtrlM4_Start(M4_ACT_CCW_LIMIT);
	}
}

void CtrlM4_Behavior_Goto_END()
{
	if (HWI_M4_CW_LIMIT)
	{
		CtrlM4_Finish();
	}
	else
	{
		CtrlM4_Start(M4_ACT_CW_LIMIT);
	}
}

void CtrlM4_Behavior_Goto_HOME_STEPS(uint32_t count)
{
	M4_Set_Count((count<<1));
	if (HWI_M4_CCW_LIMIT)
	{
		CtrlM4_Finish();
	}
	else
	{
		CtrlM4_Start(M4_ACT_CCW_COUNT);
	}
}

void CtrlM4_Behavior_Goto_END_STEPS(uint32_t count)
{
	M4_Set_Count((count<<1));
	if (HWI_M4_CW_LIMIT)
	{
		CtrlM4_Finish();
	}
	else
	{
		CtrlM4_Start(M4_ACT_CW_COUNT);
	}
}

