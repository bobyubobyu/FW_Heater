/*
 * SysCtrl_Task.c
 *
 *  Created on: Nov 25, 2025
 *      Author: Bob Yu
 */

#include "main.h"

uint8_t xVarTask_Repeat;

uint16_t xVarTask_Token;

uint16_t xVarTask_Index;
uint8_t xVarTask_Action;
uint8_t xVarTask_Status;

uint16_t xVarTask_Count;
uint8_t *xVarTask_Table;

//---------------------------------------------
//TIME  Define / 變數
uint8_t  xVarSYS_Delay_Flag;
uint32_t xVarSYS_Delay_Target;
uint32_t xVarSYS_Delay_Now;

//===============================================================
uint8_t	TBTask_PROGRAMMABLE[128] = {0};
uint8_t	TBTask_PARAMETER[128] = {0};


const	uint8_t	TBTask_Idle[] =
{
	TASK_RUN_IDLE,
	TASK_RUN_IDLE
};
#define	STEP_OF_IDLE	(sizeof(TBTask_Idle) / sizeof(uint8_t))

//---------------------------------------------------------------------------------------
// M1 FUNCTION
const	uint8_t	TBTask_M1_GOTO_HOME[] =
{
	TASK_RUN_IDLE,
	TASK_RUN_M1_GOTO_HOME,
	TASK_WAIT_M1_STOP,
	TASK_RUN_OVER
};
#define	STEP_OF_M1_GOTO_HOME	(sizeof(TBTask_M1_GOTO_HOME) / sizeof(uint8_t))

const	uint8_t	TBTask_M1_GOTO_END[] =
{
	TASK_RUN_IDLE,
	TASK_RUN_M1_GOTO_END,
	TASK_WAIT_M1_STOP,
	TASK_RUN_OVER
};
#define	STEP_OF_M1_GOTO_END		(sizeof(TBTask_M1_GOTO_END) / sizeof(uint8_t))

const	uint8_t	TBTask_M1_GOTO_HOME_STEPS[] =
{
	TASK_RUN_IDLE,
	TASK_RUN_M1_GOTO_HOME_STEPS,
	TASK_VALUE,TASK_VALUE, //16bit，前面為H，後面為L
	TASK_WAIT_M1_STOP,
	TASK_RUN_OVER
};
#define	STEP_OF_M1_GOTO_HOME_STEPS	(sizeof(TBTask_M1_GOTO_HOME_STEPS) / sizeof(uint8_t))

const	uint8_t	TBTask_M1_GOTO_END_STEPS[] =
{
	TASK_RUN_IDLE,
	TASK_RUN_M1_GOTO_END_STEPS,
	TASK_VALUE,TASK_VALUE, //16bit，前面為H，後面為L
	TASK_WAIT_M1_STOP,
	TASK_RUN_OVER
};
#define	STEP_OF_M1_GOTO_END_STEPS	(sizeof(TBTask_M1_GOTO_END_STEPS) / sizeof(uint8_t))

//-----------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------
// M2 FUNCTION
const	uint8_t	TBTask_M2_GOTO_HOME[] =
{
	TASK_RUN_IDLE,
	TASK_RUN_M2_GOTO_HOME,
	TASK_WAIT_M2_STOP,
	TASK_RUN_OVER
};
#define	STEP_OF_M2_GOTO_HOME	(sizeof(TBTask_M2_GOTO_HOME) / sizeof(uint8_t))

const	uint8_t	TBTask_M2_GOTO_END[] =
{
	TASK_RUN_IDLE,
	TASK_RUN_M2_GOTO_END,
	TASK_WAIT_M2_STOP,
	TASK_RUN_OVER
};
#define	STEP_OF_M2_GOTO_END		(sizeof(TBTask_M2_GOTO_END) / sizeof(uint8_t))

const	uint8_t	TBTask_M2_GOTO_HOME_STEPS[] =
{
	TASK_RUN_IDLE,
	TASK_RUN_M2_GOTO_HOME_STEPS,
	TASK_VALUE,TASK_VALUE, //16bit，前面為H，後面為L
	TASK_WAIT_M2_STOP,
	TASK_RUN_OVER
};
#define	STEP_OF_M2_GOTO_HOME_STEPS	(sizeof(TBTask_M2_GOTO_HOME_STEPS) / sizeof(uint8_t))

const	uint8_t	TBTask_M2_GOTO_END_STEPS[] =
{
	TASK_RUN_IDLE,
	TASK_RUN_M2_GOTO_END_STEPS,
	TASK_VALUE,TASK_VALUE, //16bit，前面為H，後面為L
	TASK_WAIT_M2_STOP,
	TASK_RUN_OVER
};
#define	STEP_OF_M2_GOTO_END_STEPS	(sizeof(TBTask_M2_GOTO_END_STEPS) / sizeof(uint8_t))

//-----------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------
// M3 FUNCTION
const	uint8_t	TBTask_M3_GOTO_HOME[] =
{
	TASK_RUN_IDLE,
	TASK_RUN_M3_GOTO_HOME,
	TASK_WAIT_M3_STOP,
	TASK_RUN_OVER
};
#define	STEP_OF_M3_GOTO_HOME	(sizeof(TBTask_M3_GOTO_HOME) / sizeof(uint8_t))

const	uint8_t	TBTask_M3_GOTO_END[] =
{
	TASK_RUN_IDLE,
	TASK_RUN_M3_GOTO_END,
	TASK_WAIT_M3_STOP,
	TASK_RUN_OVER
};
#define	STEP_OF_M3_GOTO_END		(sizeof(TBTask_M3_GOTO_END) / sizeof(uint8_t))

const	uint8_t	TBTask_M3_GOTO_HOME_STEPS[] =
{
	TASK_RUN_IDLE,
	TASK_RUN_M3_GOTO_HOME_STEPS,
	TASK_VALUE,TASK_VALUE, //16bit，前面為H，後面為L
	TASK_WAIT_M3_STOP,
	TASK_RUN_OVER
};
#define	STEP_OF_M3_GOTO_HOME_STEPS	(sizeof(TBTask_M3_GOTO_HOME_STEPS) / sizeof(uint8_t))

const	uint8_t	TBTask_M3_GOTO_END_STEPS[] =
{
	TASK_RUN_IDLE,
	TASK_RUN_M3_GOTO_END_STEPS,
	TASK_VALUE,TASK_VALUE, //16bit，前面為H，後面為L
	TASK_WAIT_M3_STOP,
	TASK_RUN_OVER
};
#define	STEP_OF_M3_GOTO_END_STEPS	(sizeof(TBTask_M3_GOTO_END_STEPS) / sizeof(uint8_t))

//-----------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------
// M4 FUNCTION
const	uint8_t	TBTask_M4_GOTO_HOME[] =
{
	TASK_RUN_IDLE,
	TASK_RUN_M4_GOTO_HOME,
	TASK_WAIT_M4_STOP,
	TASK_RUN_OVER
};
#define	STEP_OF_M4_GOTO_HOME	(sizeof(TBTask_M4_GOTO_HOME) / sizeof(uint8_t))

const	uint8_t	TBTask_M4_GOTO_END[] =
{
	TASK_RUN_IDLE,
	TASK_RUN_M4_GOTO_END,
	TASK_WAIT_M4_STOP,
	TASK_RUN_OVER
};
#define	STEP_OF_M4_GOTO_END		(sizeof(TBTask_M4_GOTO_END) / sizeof(uint8_t))

const	uint8_t	TBTask_M4_GOTO_HOME_STEPS[] =
{
	TASK_RUN_IDLE,
	TASK_RUN_M4_GOTO_HOME_STEPS,
	TASK_VALUE,TASK_VALUE, //16bit，前面為H，後面為L
	TASK_WAIT_M4_STOP,
	TASK_RUN_OVER
};
#define	STEP_OF_M4_GOTO_HOME_STEPS	(sizeof(TBTask_M4_GOTO_HOME_STEPS) / sizeof(uint8_t))

const	uint8_t	TBTask_M4_GOTO_END_STEPS[] =
{
	TASK_RUN_IDLE,
	TASK_RUN_M4_GOTO_END_STEPS,
	TASK_VALUE,TASK_VALUE, //16bit，前面為H，後面為L
	TASK_WAIT_M4_STOP,
	TASK_RUN_OVER
};
#define	STEP_OF_M4_GOTO_END_STEPS	(sizeof(TBTask_M4_GOTO_END_STEPS) / sizeof(uint8_t))

//-----------------------------------------------------------------------------------------------

void CtrlTask_Error(uint8_t State)
{
	xVarTask_Action = TASK_RUN_IDLE;
	xVarTask_Status = TASK_STATUS_IDLE;
	// SysError_Set(State);
}

void CtrlTask_Fill_Parameter(uint8_t index,uint8_t val)
{
	TBTask_PARAMETER[index] = val;
}

void CtrlTask_Set_Delay(uint32_t var_delay)
{
	xVarSYS_Delay_Flag = 1;
	xVarSYS_Delay_Target = var_delay;
	xVarSYS_Delay_Now = 0;
}

void CtrlTask_Incrase_Delay(uint32_t var_add)
{
	if(xVarSYS_Delay_Flag)
	{
		xVarSYS_Delay_Now += var_add;
		if(xVarSYS_Delay_Now >= xVarSYS_Delay_Target)
			xVarSYS_Delay_Flag = 0;
	}
}

uint8_t	CtrlTask_isWork()
{
	if (xVarTask_Status == TASK_STATUS_WORK)
        return	TRUE;

	return	FALSE;
}

uint8_t	CtrlTask_Status()
{
	if (xVarTask_Token > 0)
        return	(uint8_t) xVarTask_Token;
	else
        return	(xVarTask_Status + '0');
}

void CtrlTask_Finish()
{
	xVarTask_Token = 0;
	xVarTask_Action = TASK_RUN_IDLE;
	xVarTask_Status = TASK_STATUS_IDLE;
}

void CtrlTask_Next(uint8_t Step)
{
	xVarTask_Index += Step;

	if (xVarTask_Index >= xVarTask_Count)
	{
		CtrlTask_Error(0);
	}
	else
	{
		xVarTask_Action = xVarTask_Table[xVarTask_Index];
		if (xVarTask_Action >= NUM_OF_TASK_CMD)
		{
			CtrlTask_Error(0);
		}
	}
}

void CtrlTask_Goto()
{
	if (xVarTask_Repeat)
	{
		xVarTask_Index = xVarTask_Table[xVarTask_Index + 1];
		CtrlTask_Next(0);
	}
	else
	{
		CtrlTask_Next(2);
	}
}

void CtrlTask_Stop(uint8_t Mode)
{
	if (Mode == 0)
	{
		CtrlM1_Stop();
		CtrlM2_Stop();
		CtrlM3_Stop();
		CtrlM4_Stop();
		xVarTask_Index = 0;
		xVarTask_Action = TASK_RUN_IDLE;
	}
	else if (xVarTask_Repeat)
	{
		xVarTask_Repeat = FALSE;
	}
}

void	CtrlTask_Start(uint8_t Mode, uint8_t Repeat)
{
	// if (SysError_isError())
	// 	return;

	if (Mode >= NUM_OF_TASK_MODE)
		return;

	if (CtrlTask_isWork())
		return;

	xVarTask_Index = 0;
	xVarTask_Repeat = Repeat;
	xVarTask_Status = TASK_STATUS_WORK;

	switch (Mode)
	{
		//================================================================
		//M1 FUNCTIONS
		case TASK_MODE_M1_GOTO_HOME:
			xVarTask_Table = (uint8_t *) TBTask_M1_GOTO_HOME;
			xVarTask_Count = (uint8_t) STEP_OF_M1_GOTO_HOME;
			break;

		case TASK_MODE_M1_GOTO_END:
			xVarTask_Table = (uint8_t *) TBTask_M1_GOTO_END;
			xVarTask_Count = (uint8_t) STEP_OF_M1_GOTO_END;
			break;

		case TASK_MODE_M1_GOTO_HOME_STEPS:
			for(uint8_t i=0,j=0;i<STEP_OF_M1_GOTO_HOME_STEPS;i++)
			{
				if(TBTask_M1_GOTO_HOME_STEPS[i] == TASK_VALUE)
				{
					TBTask_PROGRAMMABLE[i] = TBTask_PARAMETER[j];
					j++;
				}
				else
				{
					TBTask_PROGRAMMABLE[i] = TBTask_M1_GOTO_HOME_STEPS[i];
				}
			}
			xVarTask_Table = (uint8_t *) TBTask_PROGRAMMABLE;
			xVarTask_Count = (uint8_t) STEP_OF_M1_GOTO_HOME_STEPS;
			break;

		case TASK_MODE_M1_GOTO_END_STEPS:
			for(uint8_t i=0,j=0;i<STEP_OF_M1_GOTO_END_STEPS;i++)
			{
				if(TBTask_M1_GOTO_END_STEPS[i] == TASK_VALUE)
				{
					TBTask_PROGRAMMABLE[i] = TBTask_PARAMETER[j];
					j++;
				}
				else
				{
					TBTask_PROGRAMMABLE[i] = TBTask_M1_GOTO_END_STEPS[i];
				}
			}
			xVarTask_Table = (uint8_t *) TBTask_PROGRAMMABLE;
			xVarTask_Count = (uint8_t) STEP_OF_M1_GOTO_END_STEPS;
			break;

		//================================================================
		//M2 FUNCTIONS
		case TASK_MODE_M2_GOTO_HOME:
			xVarTask_Table = (uint8_t *) TBTask_M2_GOTO_HOME;
			xVarTask_Count = (uint8_t) STEP_OF_M2_GOTO_HOME;
			break;

		case TASK_MODE_M2_GOTO_END:
			xVarTask_Table = (uint8_t *) TBTask_M2_GOTO_END;
			xVarTask_Count = (uint8_t) STEP_OF_M2_GOTO_END;
			break;

		case TASK_MODE_M2_GOTO_HOME_STEPS:
			for(uint8_t i=0,j=0;i<STEP_OF_M2_GOTO_HOME_STEPS;i++)
			{
				if(TBTask_M2_GOTO_HOME_STEPS[i] == TASK_VALUE)
				{
					TBTask_PROGRAMMABLE[i] = TBTask_PARAMETER[j];
					j++;
				}
				else
				{
					TBTask_PROGRAMMABLE[i] = TBTask_M2_GOTO_HOME_STEPS[i];
				}
			}
			xVarTask_Table = (uint8_t *) TBTask_PROGRAMMABLE;
			xVarTask_Count = (uint8_t) STEP_OF_M2_GOTO_HOME_STEPS;
			break;

		case TASK_MODE_M2_GOTO_END_STEPS:
			for(uint8_t i=0,j=0;i<STEP_OF_M2_GOTO_END_STEPS;i++)
			{
				if(TBTask_M2_GOTO_END_STEPS[i] == TASK_VALUE)
				{
					TBTask_PROGRAMMABLE[i] = TBTask_PARAMETER[j];
					j++;
				}
				else
				{
					TBTask_PROGRAMMABLE[i] = TBTask_M2_GOTO_END_STEPS[i];
				}
			}
			xVarTask_Table = (uint8_t *) TBTask_PROGRAMMABLE;
			xVarTask_Count = (uint8_t) STEP_OF_M2_GOTO_END_STEPS;
			break;

			//================================================================
			//M3 FUNCTIONS
			case TASK_MODE_M3_GOTO_HOME:
				xVarTask_Table = (uint8_t *) TBTask_M3_GOTO_HOME;
				xVarTask_Count = (uint8_t) STEP_OF_M3_GOTO_HOME;
				break;

			case TASK_MODE_M3_GOTO_END:
				xVarTask_Table = (uint8_t *) TBTask_M3_GOTO_END;
				xVarTask_Count = (uint8_t) STEP_OF_M3_GOTO_END;
				break;

			case TASK_MODE_M3_GOTO_HOME_STEPS:
				for(uint8_t i=0,j=0;i<STEP_OF_M3_GOTO_HOME_STEPS;i++)
				{
					if(TBTask_M3_GOTO_HOME_STEPS[i] == TASK_VALUE)
					{
						TBTask_PROGRAMMABLE[i] = TBTask_PARAMETER[j];
						j++;
					}
					else
					{
						TBTask_PROGRAMMABLE[i] = TBTask_M3_GOTO_HOME_STEPS[i];
					}
				}
				xVarTask_Table = (uint8_t *) TBTask_PROGRAMMABLE;
				xVarTask_Count = (uint8_t) STEP_OF_M3_GOTO_HOME_STEPS;
				break;

			case TASK_MODE_M3_GOTO_END_STEPS:
				for(uint8_t i=0,j=0;i<STEP_OF_M3_GOTO_END_STEPS;i++)
				{
					if(TBTask_M3_GOTO_END_STEPS[i] == TASK_VALUE)
					{
						TBTask_PROGRAMMABLE[i] = TBTask_PARAMETER[j];
						j++;
					}
					else
					{
						TBTask_PROGRAMMABLE[i] = TBTask_M3_GOTO_END_STEPS[i];
					}
				}
				xVarTask_Table = (uint8_t *) TBTask_PROGRAMMABLE;
				xVarTask_Count = (uint8_t) STEP_OF_M3_GOTO_END_STEPS;
				break;

				//================================================================
				//M4 FUNCTIONS
				case TASK_MODE_M4_GOTO_HOME:
					xVarTask_Table = (uint8_t *) TBTask_M4_GOTO_HOME;
					xVarTask_Count = (uint8_t) STEP_OF_M4_GOTO_HOME;
					break;

				case TASK_MODE_M4_GOTO_END:
					xVarTask_Table = (uint8_t *) TBTask_M4_GOTO_END;
					xVarTask_Count = (uint8_t) STEP_OF_M4_GOTO_END;
					break;

				case TASK_MODE_M4_GOTO_HOME_STEPS:
					for(uint8_t i=0,j=0;i<STEP_OF_M4_GOTO_HOME_STEPS;i++)
					{
						if(TBTask_M4_GOTO_HOME_STEPS[i] == TASK_VALUE)
						{
							TBTask_PROGRAMMABLE[i] = TBTask_PARAMETER[j];
							j++;
						}
						else
						{
							TBTask_PROGRAMMABLE[i] = TBTask_M4_GOTO_HOME_STEPS[i];
						}
					}
					xVarTask_Table = (uint8_t *) TBTask_PROGRAMMABLE;
					xVarTask_Count = (uint8_t) STEP_OF_M4_GOTO_HOME_STEPS;
					break;

				case TASK_MODE_M4_GOTO_END_STEPS:
					for(uint8_t i=0,j=0;i<STEP_OF_M4_GOTO_END_STEPS;i++)
					{
						if(TBTask_M4_GOTO_END_STEPS[i] == TASK_VALUE)
						{
							TBTask_PROGRAMMABLE[i] = TBTask_PARAMETER[j];
							j++;
						}
						else
						{
							TBTask_PROGRAMMABLE[i] = TBTask_M4_GOTO_END_STEPS[i];
						}
					}
					xVarTask_Table = (uint8_t *) TBTask_PROGRAMMABLE;
					xVarTask_Count = (uint8_t) STEP_OF_M4_GOTO_END_STEPS;
					break;



        default :
			xVarTask_Table = (uint8_t *) TBTask_Idle;
			xVarTask_Count = (uint8_t) STEP_OF_IDLE;
			break;
	}

	CtrlTask_Next(1);
}

void	CtrlTask_Process()
{
	uint32_t temp;
	switch(xVarTask_Action)
	{
		//----------------------------------------
		//M1 FUNCTIONS
		case TASK_RUN_M1_GOTO_HOME:
			CtrlM1_Behavior_Goto_HOME();
			CtrlTask_Next(1);
		break;

		case TASK_RUN_M1_GOTO_END:
			CtrlM1_Behavior_Goto_END();
			CtrlTask_Next(1);
		break;

		case TASK_RUN_M1_GOTO_HOME_STEPS:
			temp = xVarTask_Table[xVarTask_Index + 1];
			temp<<=8;
			temp+= xVarTask_Table[xVarTask_Index + 2];
			CtrlM1_Behavior_Goto_HOME_STEPS(temp);
			CtrlTask_Next(3);
		break;

		case TASK_RUN_M1_GOTO_END_STEPS:
			temp = xVarTask_Table[xVarTask_Index + 1];
			temp<<=8;
			temp+= xVarTask_Table[xVarTask_Index + 2];
			CtrlM1_Behavior_Goto_END_STEPS(temp);
			CtrlTask_Next(3);
		break;

		case TASK_WAIT_M1_STOP:
			if (M1_Get_Stop())
			{
				CtrlM1_Stop();
				CtrlTask_Next(1);
			}
		break;

		//----------------------------------------
		//M2 FUNCTIONS
		case TASK_RUN_M2_GOTO_HOME:
			CtrlM2_Behavior_Goto_HOME();
			CtrlTask_Next(1);
		break;

		case TASK_RUN_M2_GOTO_END:
			CtrlM2_Behavior_Goto_END();
			CtrlTask_Next(1);
		break;

		case TASK_RUN_M2_GOTO_HOME_STEPS:
			temp = xVarTask_Table[xVarTask_Index + 1];
			temp<<=8;
			temp+= xVarTask_Table[xVarTask_Index + 2];
			CtrlM2_Behavior_Goto_HOME_STEPS(temp);
			CtrlTask_Next(3);
		break;

		case TASK_RUN_M2_GOTO_END_STEPS:
			temp = xVarTask_Table[xVarTask_Index + 1];
			temp<<=8;
			temp+= xVarTask_Table[xVarTask_Index + 2];
			CtrlM2_Behavior_Goto_END_STEPS(temp);
			CtrlTask_Next(3);
		break;

		case TASK_WAIT_M2_STOP:
			if (M2_Get_Stop())
			{
				CtrlM2_Stop();
				CtrlTask_Next(1);
			}
		break;

		//----------------------------------------
		//M3 FUNCTIONS
		case TASK_RUN_M3_GOTO_HOME:
			CtrlM3_Behavior_Goto_HOME();
			CtrlTask_Next(1);
		break;

		case TASK_RUN_M3_GOTO_END:
			CtrlM3_Behavior_Goto_END();
			CtrlTask_Next(1);
		break;

		case TASK_RUN_M3_GOTO_HOME_STEPS:
			temp = xVarTask_Table[xVarTask_Index + 1];
			temp<<=8;
			temp+= xVarTask_Table[xVarTask_Index + 2];
			CtrlM3_Behavior_Goto_HOME_STEPS(temp);
			CtrlTask_Next(3);
		break;

		case TASK_RUN_M3_GOTO_END_STEPS:
			temp = xVarTask_Table[xVarTask_Index + 1];
			temp<<=8;
			temp+= xVarTask_Table[xVarTask_Index + 2];
			CtrlM3_Behavior_Goto_END_STEPS(temp);
			CtrlTask_Next(3);
		break;

		case TASK_WAIT_M3_STOP:
			if (M3_Get_Stop())
			{
				CtrlM3_Stop();
				CtrlTask_Next(1);
			}
		break;

		//----------------------------------------
		//M4 FUNCTIONS
		case TASK_RUN_M4_GOTO_HOME:
			CtrlM4_Behavior_Goto_HOME();
			CtrlTask_Next(1);
		break;

		case TASK_RUN_M4_GOTO_END:
			CtrlM4_Behavior_Goto_END();
			CtrlTask_Next(1);
		break;

		case TASK_RUN_M4_GOTO_HOME_STEPS:
			temp = xVarTask_Table[xVarTask_Index + 1];
			temp<<=8;
			temp+= xVarTask_Table[xVarTask_Index + 2];
			CtrlM4_Behavior_Goto_HOME_STEPS(temp);
			CtrlTask_Next(3);
		break;

		case TASK_RUN_M4_GOTO_END_STEPS:
			temp = xVarTask_Table[xVarTask_Index + 1];
			temp<<=8;
			temp+= xVarTask_Table[xVarTask_Index + 2];
			CtrlM4_Behavior_Goto_END_STEPS(temp);
			CtrlTask_Next(3);
		break;

		case TASK_WAIT_M4_STOP:
			if (M4_Get_Stop())
			{
				CtrlM4_Stop();
				CtrlTask_Next(1);
			}
		break;

		case TASK_RUN_DELAY:
			temp = xVarTask_Table[xVarTask_Index + 1];
			temp<<=8;
			temp+= xVarTask_Table[xVarTask_Index + 2];
			 CtrlTask_Set_Delay(temp);
			CtrlTask_Next(3);
			break;

		case TASK_WAIT_DELAY:
		if (xVarSYS_Delay_Flag == 0)
			{
				CtrlTask_Next(1);
			}
			break;

		case TASK_RUN_GOTO :
			CtrlTask_Goto();
			break;

		case TASK_SOUND :
			Buzzer_Start(FUR_ELISE,FUR_ELISE_NUM);
			CtrlTask_Next(1);
			break;

//		case TASK_RELEASE_SPEED_FLAG:
//		    M1_Set_user_Speed(false,0);
//			CtrlTask_Next(1);
//			break;

		case TASK_RUN_OVER :
			CtrlTask_Finish();
			break;
	}
}

void	CtrlTask_Initial()
{
	xVarTask_Index = 0;
	xVarTask_Action = TASK_RUN_IDLE;
	xVarTask_Status = TASK_STATUS_IDLE;
	xVarTask_Token = 0;
	xVarSYS_Delay_Flag = 0;
	xVarSYS_Delay_Target = 0;
	xVarSYS_Delay_Now = 0;
}



