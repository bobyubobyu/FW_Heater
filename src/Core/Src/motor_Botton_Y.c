/*
 * motor_Botton_Y.c
 *
 *  Created on: Nov 24, 2025
 *      Author: Bob Yu
 */

#include "main.h"

const	uint16_t	CtrlM2_Accel[] =
{
	#include "motor_Botton_Y.d"
};
#define	STEP_OF_ACCEL	(sizeof(CtrlM2_Accel) / sizeof(uint16_t))

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
	M2_ACT_IDLE = 0,
	M2_ACT_CW_LIMIT,
	M2_ACT_CW_COUNT,
	M2_ACT_CW_TIME,
	M2_ACT_CW_CONTINUOUS,

	M2_ACT_CCW_LIMIT,
	M2_ACT_CCW_COUNT,
	M2_ACT_CCW_TIME,
	M2_ACT_CCW_CONTINUOUS,

	M2_ACT_FINISH,

	NUM_OF_M2_ACT
};

uint8_t	 xVarM2_Action;
uint16_t xVarM2_Speed;

//---------------------------------------------
//  LIMIT Define / 變數
//---------------------------------------------
#define PROTECT_STEP_FLAG		BIT0
#define OVER_LIMIT_STEP_FLAG	BIT1
uint8_t  xVarM2_Flag_Switch = 0;
uint32_t xVarM2_Protect_Step_Target;
uint32_t xVarM2_Over_Limit_Step_Target;
uint32_t xVarM2_Protect_Step_Now;
uint32_t xVarM2_Over_Limit_Step_Now;
uint8_t  xVarM2_Stop_Flag;
//---------------------------------------------
// COUNT Define / 變數
uint32_t xVarM2_Count_Target;
uint32_t xVarM2_Count_Now;
//---------------------------------------------
// CONTINUOUS Define / 變數
// NONE
//---------------------------------------------
//TIME  Define / 變數
uint32_t xVarM2_Time_Target;
uint32_t xVarM2_Time_Now;

//---------------------------------------------
//速度控制
uint32_t xVarM2_user_Speed;
uint8_t xVarM2_user_Speed_Flag;


void M2_IO_Init(void)
{
    //Needn't
}

void M2_Set_Var_Total_Initial(void)
{
	xVarM2_Flag_Switch = 0;
	xVarM2_Stop_Flag = 0;
	xVarM2_Protect_Step_Target = 0;
	xVarM2_Over_Limit_Step_Target = 0;
	xVarM2_Protect_Step_Now = 0;
	xVarM2_Over_Limit_Step_Now = 0;
	xVarM2_Count_Target = 0;
	xVarM2_Count_Now = 0;
	xVarM2_Time_Target = 0;
	xVarM2_Time_Now = 0;
	xVarM2_Speed = 0;
	xVarM2_user_Speed = 0;
	xVarM2_user_Speed_Flag = 0;
}

void M2_Set_Var_Start_Initial(void)
{
	xVarM2_Speed = 0;
	xVarM2_Stop_Flag = 0;
	xVarM2_Protect_Step_Now = 0;
	xVarM2_Over_Limit_Step_Now = 0;
	xVarM2_Count_Now = 0;
	xVarM2_Time_Now = 0;
}

//-----------------------------------------------
//  LIMIT Function
//------------------------------------------------
//設定保護距離
//設定一次以後有效
void M2_Set_Protect_Step(uint8_t on_off,uint32_t val)
{
	if(on_off)
	{
		xVarM2_Flag_Switch |= PROTECT_STEP_FLAG;
		xVarM2_Protect_Step_Target = val;
	}
	else
	{
		xVarM2_Flag_Switch &= ~PROTECT_STEP_FLAG;
		xVarM2_Protect_Step_Target = val;
	}
}
//設定超越極限後多少距離停
//設定一次以後有效
void M2_Set_Over_Limit_Step(uint8_t on_off,uint32_t val)
{
	if(on_off)
	{
		xVarM2_Flag_Switch |= OVER_LIMIT_STEP_FLAG;
		xVarM2_Over_Limit_Step_Target = val;
	}
	else
	{
		xVarM2_Flag_Switch &= ~OVER_LIMIT_STEP_FLAG;
		xVarM2_Over_Limit_Step_Target = val;
	}
}

//--------------------------------------------------
void M2_Set_user_Speed(uint8_t user_spd_flag , uint32_t user_spd)
{
	xVarM2_user_Speed = user_spd;
	xVarM2_user_Speed_Flag = user_spd_flag;
}

//--------------------------------------------------

//--------------------------------------------------
//  COUNT Function
//--------------------------------------------------
//設定運動的COUNT數
void M2_Set_Count(uint32_t val)
{
	xVarM2_Count_Target = val;
}

//--------------------------------------------------------------
// Time Function
// 設定好Timer之後，把M2_Incrase_Time放進去
// 時間就會不斷增加，再比對xVarM2_Time_Target和xVarM2_Time_Now即可
//--------------------------------------------------------------
// 設定運動的時間
void M2_Set_Time(uint32_t val)
{
	xVarM2_Time_Target = val;
}
// 增加時間單位
void M2_Incrase_Time(uint32_t val)
{
	xVarM2_Time_Now += val;
}

uint8_t M2_Get_Stop(void)
{
	return xVarM2_Stop_Flag;
}

void CtrlM2_Start(uint8_t Action)
{
	xVarM2_Action = M2_ACT_IDLE;
	//決定方向
	if (Action == M2_ACT_CW_LIMIT  ||
		Action == M2_ACT_CW_COUNT  ||
		Action == M2_ACT_CW_TIME   ||
		Action == M2_ACT_CW_CONTINUOUS
	   )
	{
		HWO_M2_FORWARD;
	}
	else if (Action == M2_ACT_CCW_LIMIT  ||
			 Action == M2_ACT_CCW_COUNT  ||
			 Action == M2_ACT_CCW_TIME   ||
			 Action == M2_ACT_CCW_CONTINUOUS
			)
	{
	    HWO_M2_BACKWARD;
	}
	else
		return;

	//設定參數
	M2_Set_Var_Start_Initial();
	//設置PLUSE,電流IO
	HWO_M2_LOW;
	HWO_M2_ENABLE;

	xVarM2_Action = Action;
	//設置每一步的時間
	if(xVarM2_user_Speed_Flag == 1)
	{
		HWO_M2_TIME(xVarM2_user_Speed);
	}
	else
		HWO_M2_TIME(CtrlM2_Accel[xVarM2_Speed]);
	//開啟TIMER中斷
	HWO_M2_EINT;
}

void CtrlM2_Stop(void)
{
	HWO_M2_DINT;
	HWO_M2_DISABLE;
	xVarM2_Action = M2_ACT_IDLE;
}

void CtrlM2_Finish(void)
{
	xVarM2_Stop_Flag = 1;
	HWO_M2_DINT;
	xVarM2_Action = M2_ACT_FINISH;
}

void CtrlM2_Error(uint8_t State)
{
	CtrlM2_Stop();
	// SysError_Set(State);
}

void CtrlM2_Speed(void)
{
	if (xVarM2_Action != M2_ACT_IDLE)
	{
		if (xVarM2_Speed < (STEP_OF_ACCEL - 1))
		{
			xVarM2_Speed ++;
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
void CtrlM2_Trigger(void)
{
	switch (xVarM2_Action)
	{
		case M2_ACT_CW_LIMIT:
		case M2_ACT_CCW_LIMIT:
		{
			HWO_M2_TRIGGER;
			if(xVarM2_user_Speed_Flag == 1)
			{
				HWO_M2_TIME(xVarM2_user_Speed);
			}
			else
			{
				HWO_M2_TIME(CtrlM2_Accel[xVarM2_Speed]);
			}

			if(xVarM2_Flag_Switch & PROTECT_STEP_FLAG) //失步保護
			{
				xVarM2_Protect_Step_Now++;
				if(xVarM2_Protect_Step_Now >= xVarM2_Protect_Step_Target)  //抵達極限以前超過了保護步數，判定為失步
				{
					//增加保護功能旗標
					xVarM2_Stop_Flag = 1;  //xVarM2_Stop_Flag可依情況判斷是否要放入	CtrlM2_Stop() 中
					CtrlM2_Stop();
					break;
				}
			}

			if(xVarM2_Flag_Switch & OVER_LIMIT_STEP_FLAG)  //觸碰到極限後多走步數
			{
				if(xVarM2_Over_Limit_Step_Now > 0)
				{
					xVarM2_Over_Limit_Step_Now++;
				}
				else if( ((xVarM2_Action == M2_ACT_CW_LIMIT)  && HWI_M2_CW_LIMIT ) 	||
					   	 ((xVarM2_Action == M2_ACT_CCW_LIMIT) && HWI_M2_CCW_LIMIT)
					   )
				{
					xVarM2_Over_Limit_Step_Now = 1;
				}

				if(xVarM2_Over_Limit_Step_Now >= xVarM2_Over_Limit_Step_Target)
				{
					xVarM2_Stop_Flag = 1;
					CtrlM2_Stop();
				}
			}
			else  //觸碰到極限馬上停止
			{
				if( ((xVarM2_Action == M2_ACT_CW_LIMIT)  && HWI_M2_CW_LIMIT ) 	||
					((xVarM2_Action == M2_ACT_CCW_LIMIT) && HWI_M2_CCW_LIMIT)
				  )
				{
					xVarM2_Stop_Flag = 1;
					CtrlM2_Stop();
				}
			}
			break;
		}

		case M2_ACT_CW_COUNT:
		case M2_ACT_CCW_COUNT:
		{
			HWO_M2_TRIGGER;
			if(xVarM2_user_Speed_Flag == 1)
			{
				HWO_M2_TIME(xVarM2_user_Speed);
			}
			else
			{
				HWO_M2_TIME(CtrlM2_Accel[xVarM2_Speed]);
			}
			xVarM2_Count_Now++;
			if((xVarM2_Count_Now >= xVarM2_Count_Target) 					||
			   ((xVarM2_Action == M2_ACT_CW_COUNT)  && HWI_M2_CW_LIMIT ) 	||
			   ((xVarM2_Action == M2_ACT_CCW_COUNT) && HWI_M2_CCW_LIMIT)
			  )
			{
				xVarM2_Stop_Flag = 1;
				CtrlM2_Stop();
			}
			break;
		}

		case M2_ACT_CW_CONTINUOUS:
		case M2_ACT_CCW_CONTINUOUS:
		{
			HWO_M2_TRIGGER;
			if(xVarM2_user_Speed_Flag == 1)
			{
				HWO_M2_TIME(xVarM2_user_Speed);
			}
			else
			{
				HWO_M2_TIME(CtrlM2_Accel[xVarM2_Speed]);
			}
			break;
		}

		case M2_ACT_CW_TIME:
		case M2_ACT_CCW_TIME:
		{
			if(xVarM2_Time_Now >= xVarM2_Time_Target)
			{
				xVarM2_Stop_Flag = 1;
				CtrlM2_Stop();
				break;
			}
			HWO_M2_TRIGGER;
			if(xVarM2_user_Speed_Flag == 1)
			{
				HWO_M2_TIME(xVarM2_user_Speed);
			}
			else
			{
				HWO_M2_TIME(CtrlM2_Accel[xVarM2_Speed]);
			}
			break;
		}

		default:
			break;
	}
}


void CtrlM2_Initial(void)
{
	M2_IO_Init();
	M2_Set_Var_Total_Initial();
	CtrlM2_Stop();
}

void CtrlM2_Behavior_Goto_HOME()
{
	if (HWI_M2_CCW_LIMIT)
	{
		CtrlM2_Finish();
	}
	else
	{
		CtrlM2_Start(M2_ACT_CCW_LIMIT);
	}
}

void CtrlM2_Behavior_Goto_END()
{
	if (HWI_M2_CW_LIMIT)
	{
		CtrlM2_Finish();
	}
	else
	{
		CtrlM2_Start(M2_ACT_CW_LIMIT);
	}
}

void CtrlM2_Behavior_Goto_HOME_STEPS(uint32_t count)
{
	M2_Set_Count((count<<1));
	if (HWI_M2_CCW_LIMIT)
	{
		CtrlM2_Finish();
	}
	else
	{
		CtrlM2_Start(M2_ACT_CCW_COUNT);
	}
}

void CtrlM2_Behavior_Goto_END_STEPS(uint32_t count)
{
	M2_Set_Count((count<<1));
	if (HWI_M2_CW_LIMIT)
	{
		CtrlM2_Finish();
	}
	else
	{
		CtrlM2_Start(M2_ACT_CW_COUNT);
	}
}

