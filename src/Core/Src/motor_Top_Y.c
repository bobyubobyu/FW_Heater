/*
 * motor_Top_Y.c
 *
 *  Created on: Nov 25, 2025
 *      Author: Bob Yu
 */

#include "main.h"

const	uint16_t	CtrlM3_Accel[] =
{
	#include "motor_Top_Y.d"
};
#define	STEP_OF_ACCEL	(sizeof(CtrlM3_Accel) / sizeof(uint16_t))

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
	M3_ACT_IDLE = 0,
	M3_ACT_CW_LIMIT,
	M3_ACT_CW_COUNT,
	M3_ACT_CW_TIME,
	M3_ACT_CW_CONTINUOUS,

	M3_ACT_CCW_LIMIT,
	M3_ACT_CCW_COUNT,
	M3_ACT_CCW_TIME,
	M3_ACT_CCW_CONTINUOUS,

	M3_ACT_FINISH,

	NUM_OF_M3_ACT
};

uint8_t	 xVarM3_Action;
uint16_t xVarM3_Speed;

//---------------------------------------------
//  LIMIT Define / 變數
//---------------------------------------------
#define PROTECT_STEP_FLAG		BIT0
#define OVER_LIMIT_STEP_FLAG	BIT1
uint8_t  xVarM3_Flag_Switch = 0;
uint32_t xVarM3_Protect_Step_Target;
uint32_t xVarM3_Over_Limit_Step_Target;
uint32_t xVarM3_Protect_Step_Now;
uint32_t xVarM3_Over_Limit_Step_Now;
uint8_t  xVarM3_Stop_Flag;
//---------------------------------------------
// COUNT Define / 變數
uint32_t xVarM3_Count_Target;
uint32_t xVarM3_Count_Now;
//---------------------------------------------
// CONTINUOUS Define / 變數
// NONE
//---------------------------------------------
//TIME  Define / 變數
uint32_t xVarM3_Time_Target;
uint32_t xVarM3_Time_Now;

//---------------------------------------------
//速度控制
uint32_t xVarM3_user_Speed;
uint8_t xVarM3_user_Speed_Flag;

void M3_IO_Init(void)
{
    //Needn't
}


void M3_Set_Var_Total_Initial(void)
{
	xVarM3_Flag_Switch = 0;
	xVarM3_Stop_Flag = 0;
	xVarM3_Protect_Step_Target = 0;
	xVarM3_Over_Limit_Step_Target = 0;
	xVarM3_Protect_Step_Now = 0;
	xVarM3_Over_Limit_Step_Now = 0;
	xVarM3_Count_Target = 0;
	xVarM3_Count_Now = 0;
	xVarM3_Time_Target = 0;
	xVarM3_Time_Now = 0;
	xVarM3_Speed = 0;
	xVarM3_user_Speed = 0;
	xVarM3_user_Speed_Flag = 0;
}

void M3_Set_Var_Start_Initial(void)
{
	xVarM3_Speed = 0;
	xVarM3_Stop_Flag = 0;
	xVarM3_Protect_Step_Now = 0;
	xVarM3_Over_Limit_Step_Now = 0;
	xVarM3_Count_Now = 0;
	xVarM3_Time_Now = 0;
}

//-----------------------------------------------
//  LIMIT Function
//------------------------------------------------
//設定保護距離
//設定一次以後有效
void M3_Set_Protect_Step(uint8_t on_off,uint32_t val)
{
	if(on_off)
	{
		xVarM3_Flag_Switch |= PROTECT_STEP_FLAG;
		xVarM3_Protect_Step_Target = val;
	}
	else
	{
		xVarM3_Flag_Switch &= ~PROTECT_STEP_FLAG;
		xVarM3_Protect_Step_Target = val;
	}
}
//設定超越極限後多少距離停
//設定一次以後有效
void M3_Set_Over_Limit_Step(uint8_t on_off,uint32_t val)
{
	if(on_off)
	{
		xVarM3_Flag_Switch |= OVER_LIMIT_STEP_FLAG;
		xVarM3_Over_Limit_Step_Target = val;
	}
	else
	{
		xVarM3_Flag_Switch &= ~OVER_LIMIT_STEP_FLAG;
		xVarM3_Over_Limit_Step_Target = val;
	}
}

//--------------------------------------------------
void M3_Set_user_Speed(uint8_t user_spd_flag , uint32_t user_spd)
{
	xVarM3_user_Speed = user_spd;
	xVarM3_user_Speed_Flag = user_spd_flag;
}

//--------------------------------------------------

//--------------------------------------------------
//  COUNT Function
//--------------------------------------------------
//設定運動的COUNT數
void M3_Set_Count(uint32_t val)
{
	xVarM3_Count_Target = val;
}

//--------------------------------------------------------------
// Time Function
// 設定好Timer之後，把M3_Incrase_Time放進去
// 時間就會不斷增加，再比對xVarM3_Time_Target和xVarM3_Time_Now即可
//--------------------------------------------------------------
// 設定運動的時間
void M3_Set_Time(uint32_t val)
{
	xVarM3_Time_Target = val;
}
// 增加時間單位
void M3_Incrase_Time(uint32_t val)
{
	xVarM3_Time_Now += val;
}

uint8_t M3_Get_Stop(void)
{
	return xVarM3_Stop_Flag;
}

void CtrlM3_Start(uint8_t Action)
{
	xVarM3_Action = M3_ACT_IDLE;
	//決定方向
	if (Action == M3_ACT_CW_LIMIT  ||
		Action == M3_ACT_CW_COUNT  ||
		Action == M3_ACT_CW_TIME   ||
		Action == M3_ACT_CW_CONTINUOUS
	   )
	{
		HWO_M3_FORWARD;
	}
	else if (Action == M3_ACT_CCW_LIMIT  ||
			 Action == M3_ACT_CCW_COUNT  ||
			 Action == M3_ACT_CCW_TIME   ||
			 Action == M3_ACT_CCW_CONTINUOUS
			)
	{
	    HWO_M3_BACKWARD;
	}
	else
		return;

	//設定參數
	M3_Set_Var_Start_Initial();
	//設置PLUSE,電流IO
	HWO_M3_LOW;
	HWO_M3_ENABLE;

	xVarM3_Action = Action;
	//設置每一步的時間
	if(xVarM3_user_Speed_Flag == 1)
	{
		HWO_M3_TIME(xVarM3_user_Speed);
	}
	else
		HWO_M3_TIME(CtrlM3_Accel[xVarM3_Speed]);
	//開啟TIMER中斷
	HWO_M3_EINT;
}

void CtrlM3_Stop(void)
{
	HWO_M3_DINT;
	HWO_M3_DISABLE;
	xVarM3_Action = M3_ACT_IDLE;
}

void CtrlM3_Finish(void)
{
	xVarM3_Stop_Flag = 1;
	HWO_M3_DINT;
	xVarM3_Action = M3_ACT_FINISH;
}

void CtrlM3_Error(uint8_t State)
{
	CtrlM3_Stop();
	// SysError_Set(State);
}

void CtrlM3_Speed(void)
{
	if (xVarM3_Action != M3_ACT_IDLE)
	{
		if (xVarM3_Speed < (STEP_OF_ACCEL - 1))
		{
			xVarM3_Speed ++;
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
void CtrlM3_Trigger(void)
{
	switch (xVarM3_Action)
	{
		case M3_ACT_CW_LIMIT:
		case M3_ACT_CCW_LIMIT:
		{
			HWO_M3_TRIGGER;
			if(xVarM3_user_Speed_Flag == 1)
			{
				HWO_M3_TIME(xVarM3_user_Speed);
			}
			else
			{
				HWO_M3_TIME(CtrlM3_Accel[xVarM3_Speed]);
			}

			if(xVarM3_Flag_Switch & PROTECT_STEP_FLAG) //失步保護
			{
				xVarM3_Protect_Step_Now++;
				if(xVarM3_Protect_Step_Now >= xVarM3_Protect_Step_Target)  //抵達極限以前超過了保護步數，判定為失步
				{
					//增加保護功能旗標
					xVarM3_Stop_Flag = 1;  //xVarM3_Stop_Flag可依情況判斷是否要放入	CtrlM3_Stop() 中
					CtrlM3_Stop();
					break;
				}
			}

			if(xVarM3_Flag_Switch & OVER_LIMIT_STEP_FLAG)  //觸碰到極限後多走步數
			{
				if(xVarM3_Over_Limit_Step_Now > 0)
				{
					xVarM3_Over_Limit_Step_Now++;
				}
				else if( ((xVarM3_Action == M3_ACT_CW_LIMIT)  && HWI_M3_CW_LIMIT ) 	||
					   	 ((xVarM3_Action == M3_ACT_CCW_LIMIT) && HWI_M3_CCW_LIMIT)
					   )
				{
					xVarM3_Over_Limit_Step_Now = 1;
				}

				if(xVarM3_Over_Limit_Step_Now >= xVarM3_Over_Limit_Step_Target)
				{
					xVarM3_Stop_Flag = 1;
					CtrlM3_Stop();
				}
			}
			else  //觸碰到極限馬上停止
			{
				if( ((xVarM3_Action == M3_ACT_CW_LIMIT)  && HWI_M3_CW_LIMIT ) 	||
					((xVarM3_Action == M3_ACT_CCW_LIMIT) && HWI_M3_CCW_LIMIT)
				  )
				{
					xVarM3_Stop_Flag = 1;
					CtrlM3_Stop();
				}
			}
			break;
		}

		case M3_ACT_CW_COUNT:
		case M3_ACT_CCW_COUNT:
		{
			HWO_M3_TRIGGER;
			if(xVarM3_user_Speed_Flag == 1)
			{
				HWO_M3_TIME(xVarM3_user_Speed);
			}
			else
			{
				HWO_M3_TIME(CtrlM3_Accel[xVarM3_Speed]);
			}
			xVarM3_Count_Now++;
			if((xVarM3_Count_Now >= xVarM3_Count_Target) 					||
			   ((xVarM3_Action == M3_ACT_CW_COUNT)  && HWI_M3_CW_LIMIT ) 	||
			   ((xVarM3_Action == M3_ACT_CCW_COUNT) && HWI_M3_CCW_LIMIT)
			  )
			{
				xVarM3_Stop_Flag = 1;
				CtrlM3_Stop();
			}
			break;
		}

		case M3_ACT_CW_CONTINUOUS:
		case M3_ACT_CCW_CONTINUOUS:
		{
			HWO_M3_TRIGGER;
			if(xVarM3_user_Speed_Flag == 1)
			{
				HWO_M3_TIME(xVarM3_user_Speed);
			}
			else
			{
				HWO_M3_TIME(CtrlM3_Accel[xVarM3_Speed]);
			}
			break;
		}

		case M3_ACT_CW_TIME:
		case M3_ACT_CCW_TIME:
		{
			if(xVarM3_Time_Now >= xVarM3_Time_Target)
			{
				xVarM3_Stop_Flag = 1;
				CtrlM3_Stop();
				break;
			}
			HWO_M3_TRIGGER;
			if(xVarM3_user_Speed_Flag == 1)
			{
				HWO_M3_TIME(xVarM3_user_Speed);
			}
			else
			{
				HWO_M3_TIME(CtrlM3_Accel[xVarM3_Speed]);
			}
			break;
		}

		default:
			break;
	}
}

void CtrlM3_Initial(void)
{
	M3_IO_Init();
	M3_Set_Var_Total_Initial();
	CtrlM3_Stop();
}

void CtrlM3_Behavior_Goto_HOME()
{
	if (HWI_M3_CCW_LIMIT)
	{
		CtrlM3_Finish();
	}
	else
	{
		CtrlM3_Start(M3_ACT_CCW_LIMIT);
	}
}

void CtrlM3_Behavior_Goto_END()
{
	if (HWI_M3_CW_LIMIT)
	{
		CtrlM3_Finish();
	}
	else
	{
		CtrlM3_Start(M3_ACT_CW_LIMIT);
	}
}

void CtrlM3_Behavior_Goto_HOME_STEPS(uint32_t count)
{
	M3_Set_Count((count<<1));
	if (HWI_M3_CCW_LIMIT)
	{
		CtrlM3_Finish();
	}
	else
	{
		CtrlM3_Start(M3_ACT_CCW_COUNT);
	}
}

void CtrlM3_Behavior_Goto_END_STEPS(uint32_t count)
{
	M3_Set_Count((count<<1));
	if (HWI_M3_CW_LIMIT)
	{
		CtrlM3_Finish();
	}
	else
	{
		CtrlM3_Start(M3_ACT_CW_COUNT);
	}
}




