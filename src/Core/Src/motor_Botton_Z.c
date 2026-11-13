/*
 * motor_Botton_Z.c
 *
 *  Created on: Nov 24, 2025
 *      Author: Bob Yu
 */

#include "main.h"

const	uint16_t	CtrlM1_Accel[] =
{
	#include "motor_Botton_Z.d"
};
#define	STEP_OF_ACCEL	(sizeof(CtrlM1_Accel) / sizeof(uint16_t))

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
	M1_ACT_IDLE = 0,
	M1_ACT_CW_LIMIT,
	M1_ACT_CW_COUNT,
	M1_ACT_CW_TIME,
	M1_ACT_CW_CONTINUOUS,

	M1_ACT_CCW_LIMIT,
	M1_ACT_CCW_COUNT,
	M1_ACT_CCW_TIME,
	M1_ACT_CCW_CONTINUOUS,

	M1_ACT_FINISH,

	NUM_OF_M1_ACT
};

uint8_t	 xVarM1_Action;
uint16_t xVarM1_Speed;

//---------------------------------------------
//  LIMIT Define / 變數
//---------------------------------------------
#define PROTECT_STEP_FLAG		BIT0
#define OVER_LIMIT_STEP_FLAG	BIT1
uint8_t  xVarM1_Flag_Switch = 0;
uint32_t xVarM1_Protect_Step_Target;
uint32_t xVarM1_Over_Limit_Step_Target;
uint32_t xVarM1_Protect_Step_Now;
uint32_t xVarM1_Over_Limit_Step_Now;
uint8_t  xVarM1_Stop_Flag;
//---------------------------------------------
// COUNT Define / 變數
uint32_t xVarM1_Count_Target;
uint32_t xVarM1_Count_Now;
//---------------------------------------------
// CONTINUOUS Define / 變數
// NONE
//---------------------------------------------
//TIME  Define / 變數
uint32_t xVarM1_Time_Target;
uint32_t xVarM1_Time_Now;

//---------------------------------------------
//速度控制
uint32_t xVarM1_user_Speed;
uint8_t xVarM1_user_Speed_Flag;

void M1_IO_Init(void)
{
    //Needn't
}

void M1_Set_Var_Total_Initial(void)
{
	xVarM1_Flag_Switch = 0;
	xVarM1_Stop_Flag = 0;
	xVarM1_Protect_Step_Target = 0;
	xVarM1_Over_Limit_Step_Target = 0;
	xVarM1_Protect_Step_Now = 0;
	xVarM1_Over_Limit_Step_Now = 0;
	xVarM1_Count_Target = 0;
	xVarM1_Count_Now = 0;
	xVarM1_Time_Target = 0;
	xVarM1_Time_Now = 0;
	xVarM1_Speed = 0;
	xVarM1_user_Speed = 0;
	xVarM1_user_Speed_Flag = 0;
}

void M1_Set_Var_Start_Initial(void)
{
	xVarM1_Speed = 0;
	xVarM1_Stop_Flag = 0;
	xVarM1_Protect_Step_Now = 0;
	xVarM1_Over_Limit_Step_Now = 0;
	xVarM1_Count_Now = 0;
	xVarM1_Time_Now = 0;
}

//-----------------------------------------------
//  LIMIT Function
//------------------------------------------------
//設定保護距離
//設定一次以後有效
void M1_Set_Protect_Step(uint8_t on_off,uint32_t val)
{
	if(on_off)
	{
		xVarM1_Flag_Switch |= PROTECT_STEP_FLAG;
		xVarM1_Protect_Step_Target = val;
	}
	else
	{
		xVarM1_Flag_Switch &= ~PROTECT_STEP_FLAG;
		xVarM1_Protect_Step_Target = val;
	}
}

//設定超越極限後多少距離停
//設定一次以後有效
void M1_Set_Over_Limit_Step(uint8_t on_off,uint32_t val)
{
	if(on_off)
	{
		xVarM1_Flag_Switch |= OVER_LIMIT_STEP_FLAG;
		xVarM1_Over_Limit_Step_Target = val;
	}
	else
	{
		xVarM1_Flag_Switch &= ~OVER_LIMIT_STEP_FLAG;
		xVarM1_Over_Limit_Step_Target = val;
	}
}

//--------------------------------------------------
void M1_Set_user_Speed(uint8_t user_spd_flag , uint32_t user_spd)
{
	xVarM1_user_Speed = user_spd;
	xVarM1_user_Speed_Flag = user_spd_flag;
}

//--------------------------------------------------
//  COUNT Function
//--------------------------------------------------
//設定運動的COUNT數
void M1_Set_Count(uint32_t val)
{
	xVarM1_Count_Target = val;
}

//--------------------------------------------------------------
// Time Function
// 設定好Timer之後，把M1_Incrase_Time放進去
// 時間就會不斷增加，再比對xVarM1_Time_Target和xVarM1_Time_Now即可
//--------------------------------------------------------------
// 設定運動的時間
void M1_Set_Time(uint32_t val)
{
	xVarM1_Time_Target = val;
}
// 增加時間單位
void M1_Incrase_Time(uint32_t val)
{
	xVarM1_Time_Now += val;
}

uint8_t M1_Get_Stop(void)
{
	return xVarM1_Stop_Flag;
}

void CtrlM1_Start(uint8_t Action)
{
	xVarM1_Action = M1_ACT_IDLE;
	//決定方向
	if (Action == M1_ACT_CW_LIMIT  ||
		Action == M1_ACT_CW_COUNT  ||
		Action == M1_ACT_CW_TIME   ||
		Action == M1_ACT_CW_CONTINUOUS
	   )
	{
		HWO_M1_FORWARD;
	}
	else if (Action == M1_ACT_CCW_LIMIT  ||
			 Action == M1_ACT_CCW_COUNT  ||
			 Action == M1_ACT_CCW_TIME   ||
			 Action == M1_ACT_CCW_CONTINUOUS
			)
	{
	    HWO_M1_BACKWARD;
	}
	else
		return;

	//設定參數
	M1_Set_Var_Start_Initial();
	//設置PLUSE,電流IO
	HWO_M1_LOW;
	HWO_M1_ENABLE;

	xVarM1_Action = Action;
	//設置每一步的時間
	if(xVarM1_user_Speed_Flag == 1)
	{
		HWO_M1_TIME(xVarM1_user_Speed);
	}
	else
		HWO_M1_TIME(CtrlM1_Accel[xVarM1_Speed]);
	//開啟TIMER中斷
	HWO_M1_EINT;
}

void CtrlM1_Stop(void)
{
	HWO_M1_DINT;
	HWO_M1_DISABLE;
	xVarM1_Action = M1_ACT_IDLE;
}

void CtrlM1_Finish(void)
{
	xVarM1_Stop_Flag = 1;
	HWO_M1_DINT;
	xVarM1_Action = M1_ACT_FINISH;
}

void CtrlM1_Error(uint8_t State)
{
	CtrlM1_Stop();
	// SysError_Set(State);
}

void CtrlM1_Speed(void)
{
	if (xVarM1_Action != M1_ACT_IDLE)
	{
		if (xVarM1_Speed < (STEP_OF_ACCEL - 1))
		{
			xVarM1_Speed ++;
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
void CtrlM1_Trigger(void)
{
	switch (xVarM1_Action)
	{
		case M1_ACT_CW_LIMIT:
		case M1_ACT_CCW_LIMIT:
		{
			HWO_M1_TRIGGER;
			if(xVarM1_user_Speed_Flag == 1)
			{
				HWO_M1_TIME(xVarM1_user_Speed);
			}
			else
			{
				HWO_M1_TIME(CtrlM1_Accel[xVarM1_Speed]);
			}

			if(xVarM1_Flag_Switch & PROTECT_STEP_FLAG) //失步保護
			{
				xVarM1_Protect_Step_Now++;
				if(xVarM1_Protect_Step_Now >= xVarM1_Protect_Step_Target)  //抵達極限以前超過了保護步數，判定為失步
				{
					//增加保護功能旗標
					xVarM1_Stop_Flag = 1;  //xVarM1_Stop_Flag可依情況判斷是否要放入	CtrlM1_Stop() 中
					CtrlM1_Stop();
					break;
				}
			}

			if(xVarM1_Flag_Switch & OVER_LIMIT_STEP_FLAG)  //觸碰到極限後多走步數
			{
				if(xVarM1_Over_Limit_Step_Now > 0)
				{
					xVarM1_Over_Limit_Step_Now++;
				}
				else if( ((xVarM1_Action == M1_ACT_CW_LIMIT)  && HWI_M1_CW_LIMIT ) 	||
					   	 ((xVarM1_Action == M1_ACT_CCW_LIMIT) && HWI_M1_CCW_LIMIT)
					   )
				{
					xVarM1_Over_Limit_Step_Now = 1;
				}

				if(xVarM1_Over_Limit_Step_Now >= xVarM1_Over_Limit_Step_Target)
				{
					xVarM1_Stop_Flag = 1;
					CtrlM1_Stop();
				}
			}
			else  //觸碰到極限馬上停止
			{
				if( ((xVarM1_Action == M1_ACT_CW_LIMIT)  && HWI_M1_CW_LIMIT ) 	||
					((xVarM1_Action == M1_ACT_CCW_LIMIT) && HWI_M1_CCW_LIMIT)
				  )
				{
					xVarM1_Stop_Flag = 1;
					CtrlM1_Stop();
				}
			}
			break;
		}

		case M1_ACT_CW_COUNT:
		case M1_ACT_CCW_COUNT:
		{
			HWO_M1_TRIGGER;
			if(xVarM1_user_Speed_Flag == 1)
			{
				HWO_M1_TIME(xVarM1_user_Speed);
			}
			else
			{
				HWO_M1_TIME(CtrlM1_Accel[xVarM1_Speed]);
			}
			xVarM1_Count_Now++;
			if((xVarM1_Count_Now >= xVarM1_Count_Target) 					||
			   ((xVarM1_Action == M1_ACT_CW_COUNT)  && HWI_M1_CW_LIMIT ) 	||
			   ((xVarM1_Action == M1_ACT_CCW_COUNT) && HWI_M1_CCW_LIMIT)
			  )
			{
				xVarM1_Stop_Flag = 1;
				CtrlM1_Stop();
			}
			break;
		}

		case M1_ACT_CW_CONTINUOUS:
		case M1_ACT_CCW_CONTINUOUS:
		{
			HWO_M1_TRIGGER;
			if(xVarM1_user_Speed_Flag == 1)
			{
				HWO_M1_TIME(xVarM1_user_Speed);
			}
			else
			{
				HWO_M1_TIME(CtrlM1_Accel[xVarM1_Speed]);
			}
			break;
		}

		case M1_ACT_CW_TIME:
		case M1_ACT_CCW_TIME:
		{
			if(xVarM1_Time_Now >= xVarM1_Time_Target)
			{
				xVarM1_Stop_Flag = 1;
				CtrlM1_Stop();
				break;
			}
			HWO_M1_TRIGGER;
			if(xVarM1_user_Speed_Flag == 1)
			{
				HWO_M1_TIME(xVarM1_user_Speed);
			}
			else
			{
				HWO_M1_TIME(CtrlM1_Accel[xVarM1_Speed]);
			}
			break;
		}

		default:
			break;
	}
}

void CtrlM1_Initial(void)
{
	M1_IO_Init();
	M1_Set_Var_Total_Initial();
	CtrlM1_Stop();
}

void CtrlM1_Behavior_Goto_HOME()
{
	if (HWI_M1_CCW_LIMIT)
	{
		CtrlM1_Finish();
	}
	else
	{
		CtrlM1_Start(M1_ACT_CCW_LIMIT);
	}
}

void CtrlM1_Behavior_Goto_END()
{
	if (HWI_M1_CW_LIMIT)
	{
		CtrlM1_Finish();
	}
	else
	{
		CtrlM1_Start(M1_ACT_CW_LIMIT);
	}
}

void CtrlM1_Behavior_Goto_HOME_STEPS(uint32_t count)
{
	M1_Set_Count((count<<1));
	if (HWI_M1_CCW_LIMIT)
	{
		CtrlM1_Finish();
	}
	else
	{
		CtrlM1_Start(M1_ACT_CCW_COUNT);
	}
}

void CtrlM1_Behavior_Goto_END_STEPS(uint32_t count)
{
	M1_Set_Count((count<<1));
	if (HWI_M1_CW_LIMIT)
	{
		CtrlM1_Finish();
	}
	else
	{
		CtrlM1_Start(M1_ACT_CW_COUNT);
	}
}





