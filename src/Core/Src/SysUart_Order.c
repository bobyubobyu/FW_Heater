/*
 * SysUart_Order.c
 *
 *  Created on: Nov 21, 2025
 *      Author: Bob Yu
 */

#include "main.h"

//===============================================================================================
#define		NUM_OF_COMMAND		3

#define		SRC_LEN				xVarOrder_Source[0]
#define		SRC_HEAD(x)			xVarOrder_Source[x]
#define		SRC_TAIL(x)			xVarOrder_Source[xVarOrder_Source[0] - x]
#define		COMMAND(x)			(x < NUM_OF_COMMAND ? xVarOrder_Command[x] : 0)

#define		ARG_isHex(x)		UartOrder_Value(x, 16)
#define		ARG_isValue(x)		UartOrder_Value(x, 10)
//===============================================================================================

uint16_t xVarHost_Token;
int32_t	xVarArg_Value;
uint8_t	xVarOrder_pData;
uint8_t	*xVarOrder_Source;
uint8_t	xVarOrder_Command[NUM_OF_COMMAND];
uint8_t _COMMAND = 0;

uint8_t UartOrder_CheckSum()
{
	if (SRC_LEN < 9)    //判斷收到的命令長度是否有大於最小長度 len(*00SRZZ!)+1
        return FALSE;
	if (SRC_TAIL(1) != UART_MARK_STOP)
        return FALSE;

	uint8_t	LByteCS = SRC_TAIL(2);
	uint8_t	HByteCS = SRC_TAIL(3);

	SRC_LEN -= 2;  //去除CRC

	SRC_TAIL(1) = UART_MARK_STOP;  //去除CRC後，"!"也不見了，把最後一個恢復成"!"

    //改成四種組合都可以
	if (((HByteCS == 'Z') || (HByteCS == 'z')) &&
        ((LByteCS == 'Z') || (LByteCS == 'z'))
       )
        return TRUE;

	LByteCS -= 'A';
	LByteCS &= 0x0F;

	HByteCS -= 'A';
	HByteCS <<= 4;
	HByteCS &= 0xF0;

	uint8_t	Sum = HByteCS + LByteCS;

	for (uint8_t i = 2; i < (SRC_LEN - 1); i++)
        Sum += SRC_HEAD(i);

	return (Sum == 0 ? TRUE : FALSE);
}

uint8_t	UartOrder_Review(uint16_t *Token)
{
	if (SRC_LEN < 7)  //經過計算CRC後，必須>=6  len(*00SR!)+1
        return FALSE;
	if (SRC_HEAD(1) != UART_MARK_START)
        return FALSE;

	int16_t	Index;

	xVarOrder_Command[0] = 0;

	if ((SRC_HEAD(2) != '-') || (SRC_HEAD(3) != '-'))
	{
		if (SRC_HEAD(2) < '0' ||
            SRC_HEAD(2) > '9' ||
            SRC_HEAD(3) < '0' ||
            SRC_HEAD(3) > '9'
           )
            return FALSE;

		Index = (int16_t) (SRC_HEAD(2) - '0') * 10;
		Index += (int16_t) (SRC_HEAD(3) - '0');

		if (*Token != Index)
            *Token = Index;
		else if (Index == 0)
            *Token = 0;
		else
            return	TRUE;  //應該要FALSE
	}

	for (Index = 1; Index < NUM_OF_COMMAND; Index ++)
		xVarOrder_Command[Index] = 0;

	xVarOrder_pData = 0xFF;

	uint8_t	Length = SRC_LEN - 1;

	for (uint8_t i = 4; i < Length; i++)
	{
		if (SRC_HEAD(i) == UART_MARK_OPEN)
		{
			xVarOrder_pData = i + 1;  //找到':'，他的下一個就是資料，看到:就離開了
			break;
		}
		else if (i < (NUM_OF_COMMAND + 4))  //NUM_OF_COMMAND設定COMMAND總數
		{
			xVarOrder_Command[i - 4] = SRC_HEAD(i);
		}
	}

	return TRUE;
}

uint8_t	UartOrder_Value(uint8_t Index, uint8_t Base)
{
	int16_t	Sign = 1;
	uint8_t	Count = 0;
	uint8_t	isOK = FALSE;

	xVarArg_Value = 0L;

	for (uint8_t i = xVarOrder_pData; i < SRC_LEN; i++)  //xVarOrder_pData的位置由UartOrder_Review決定
	{
		uint8_t	Code = SRC_HEAD(i);
		if (Code == UART_MARK_STOP)
            break;
		if (Code == UART_MARK_CLOSE)
            break;

		if (Count == Index)  //找到第Index個數
		{
			if (Code == UART_MARK_COMMA)  //到","為止
                break;
			if (Code == '+')  //正數
                Sign = 1;
			if (Code == '-')  //負數
            	Sign = -1;
			if (Code < '0')	  //不是數字，跳過
                continue;

			if (Base == 10)
			{
				if (Code > '9')  //10進制下，大於9非數字，跳過
                    continue;
				xVarArg_Value *= 10L;
				xVarArg_Value += (int32_t) (Code - '0');
				isOK = TRUE;
			}
			else
			{
				if (Code > 'F')	//16進制下，大於F非數字，跳過
                    continue;
				if (Code >= 'A') //16進制下，A~F要處理
                    Code -= 7;
				else if (Code > '9') //其他大於9為非數字，跳過
                    continue;
				xVarArg_Value *= 16L;
				xVarArg_Value += (int32_t) (Code - '0');
				isOK = TRUE;
			}
		}
		else if (Code == UART_MARK_COMMA)
            Count ++;
	}

	if (isOK)
        xVarArg_Value *= Sign;

	return	isOK;
}

uint8_t	UART_USB_Order_System()
{
    uint8_t ret = TRUE;
	_COMMAND = COMMAND(1);
	switch (_COMMAND)
	{
		case 'R' :
			SysFlag_Set(FLAG_REPORT_INFO);
			break;
		case 'B' :
//			SysFlag_Set(FLAG_COMMAND_BUZZER);
			break;
		default:
            ret = FALSE;
			break;
	}
	return ret;
}

uint8_t	UART_USB_Order_Control()
{
	uint8_t ret = TRUE;
	uint8_t temp0 = 0;
    switch (COMMAND(1))
    {
    	case BOTTON_Z:
    	{
			if(ARG_isValue(0))
			{
				temp0 = xVarArg_Value;
				if(temp0 == 0)
				{
					CtrlTask_Start(TASK_MODE_M1_GOTO_HOME,FALSE);
				}
				else if(temp0 == 1)
				{
					CtrlTask_Start(TASK_MODE_M1_GOTO_END,FALSE);
				}
				else if(temp0 == 2 || temp0 == 3)
				{
					if(ARG_isValue(1))
					{
						if(xVarArg_Value < 0x10000)
						{
							CtrlTask_Fill_Parameter(0,(xVarArg_Value>>8));
							CtrlTask_Fill_Parameter(1,(xVarArg_Value&0xFF));
							if(temp0 == 2)
								CtrlTask_Start(TASK_MODE_M1_GOTO_HOME_STEPS,FALSE);
							else if(temp0 == 3)
								CtrlTask_Start(TASK_MODE_M1_GOTO_END_STEPS,FALSE);
						}
						else
							ret = FALSE;
					}
					else
						ret = FALSE;
				}
			}
			else
				ret = FALSE;
    	}
    	break;

    	case BOTTON_Y:
    	{
			if(ARG_isValue(0))
			{
				temp0 = xVarArg_Value;
				if(temp0 == 0)
				{
					CtrlTask_Start(TASK_MODE_M2_GOTO_HOME,FALSE);
				}
				else if(temp0 == 1)
				{
					CtrlTask_Start(TASK_MODE_M2_GOTO_END,FALSE);
				}
				else if(temp0 == 2 || temp0 == 3)
				{
					if(ARG_isValue(1))
					{
						if(xVarArg_Value < 0x10000)
						{
							CtrlTask_Fill_Parameter(0,(xVarArg_Value>>8));
							CtrlTask_Fill_Parameter(1,(xVarArg_Value&0xFF));
							if(temp0 == 2)
								CtrlTask_Start(TASK_MODE_M2_GOTO_HOME_STEPS,FALSE);
							else if(temp0 == 3)
								CtrlTask_Start(TASK_MODE_M2_GOTO_END_STEPS,FALSE);
						}
						else
							ret = FALSE;
					}
					else
						ret = FALSE;
				}
			}
			else
				ret = FALSE;
    	}
    	break;

    	case TOP_Y:
    	{
			if(ARG_isValue(0))
			{
				temp0 = xVarArg_Value;
				if(temp0 == 0)
				{
					CtrlTask_Start(TASK_MODE_M3_GOTO_HOME,FALSE);
				}
				else if(temp0 == 1)
				{
					CtrlTask_Start(TASK_MODE_M3_GOTO_END,FALSE);
				}
				else if(temp0 == 2 || temp0 == 3)
				{
					if(ARG_isValue(1))
					{
						if(xVarArg_Value < 0x10000)
						{
							CtrlTask_Fill_Parameter(0,(xVarArg_Value>>8));
							CtrlTask_Fill_Parameter(1,(xVarArg_Value&0xFF));
							if(temp0 == 2)
								CtrlTask_Start(TASK_MODE_M3_GOTO_HOME_STEPS,FALSE);
							else if(temp0 == 3)
								CtrlTask_Start(TASK_MODE_M3_GOTO_END_STEPS,FALSE);
						}
						else
							ret = FALSE;
					}
					else
						ret = FALSE;
				}
			}
			else
				ret = FALSE;
    	}
    	break;

    	case TOP_X:
    	{
			if(ARG_isValue(0))
			{
				temp0 = xVarArg_Value;
				if(temp0 == 0)
				{
					CtrlTask_Start(TASK_MODE_M4_GOTO_HOME,FALSE);
				}
				else if(temp0 == 1)
				{
					CtrlTask_Start(TASK_MODE_M4_GOTO_END,FALSE);
				}
				else if(temp0 == 2 || temp0 == 3)
				{
					if(ARG_isValue(1))
					{
						if(xVarArg_Value < 0x10000)
						{
							CtrlTask_Fill_Parameter(0,(xVarArg_Value>>8));
							CtrlTask_Fill_Parameter(1,(xVarArg_Value&0xFF));
							if(temp0 == 2)
								CtrlTask_Start(TASK_MODE_M4_GOTO_HOME_STEPS,FALSE);
							else if(temp0 == 3)
								CtrlTask_Start(TASK_MODE_M4_GOTO_END_STEPS,FALSE);
						}
						else
							ret = FALSE;
					}
					else
						ret = FALSE;
				}
			}
			else
				ret = FALSE;
    	}
    	break;

		default:
			ret = FALSE;
			break;
	}
	return ret;
}

uint8_t	UART_USB_Order_Test(void)
{
    uint8_t ret = TRUE;
	uint8_t temp0 = 0;
	uint8_t temp1 = 0;	
	uint8_t temp2 = 0;
	uint8_t temp3 = 0;	
	uint16_t temp4 = 0;		
	_COMMAND = COMMAND(1);
	switch (_COMMAND)
	{
		case 'T' :  //製冷晶片
			if(ARG_isValue(0) && (ret == TRUE))  //CH
				temp0 = (uint8_t)xVarArg_Value;
			else
            	ret = FALSE;	

			if(ARG_isValue(1) && (ret == TRUE))  //ON OFF
				temp1 = (uint8_t)xVarArg_Value;	
			else
            	ret = FALSE;

			if(ARG_isValue(2) && (ret == TRUE))  //DIR
				temp2 = (uint8_t)xVarArg_Value;	
			else
            	ret = FALSE;	

			if(ARG_isValue(3) && (ret == TRUE))  //POWER 0-255
				temp3 = (uint8_t)xVarArg_Value;	
			else
            	ret = FALSE;

			if(ret == TRUE)
			{
				set_SINGLE_TE_total(temp0,temp1,temp2);
				if(temp1 == 0)
					set_SINGLE_TE_power(temp0,0);
				else
					set_SINGLE_TE_power(temp0,temp3);						
			}
			break;
		case 'H' :  //HEATER
			if(ARG_isValue(0) && (ret == TRUE))  //CH
				temp0 = (uint8_t)xVarArg_Value;
			else
            	ret = FALSE;	

			if(ARG_isValue(1) && (ret == TRUE))  //PWM 0-100
				temp4 = (uint16_t)xVarArg_Value;	
			else
            	ret = FALSE;			
			if(ret == TRUE)
			{
				set_heater_PWM(temp0,temp4);
			}			
			break;	

		case 'F' :  //風扇
			if(ARG_isValue(0) && (ret == TRUE))  //ON OFF
				temp0 = (uint8_t)xVarArg_Value;
			else
            	ret = FALSE;	

			if(ARG_isValue(1) && (ret == TRUE))  //POWER 0-255
				temp1 = (uint8_t)xVarArg_Value;	
			else
            	ret = FALSE;			
			if(ret == TRUE)
			{
				set_FAN_total(temp0,temp1);
			}				
			break;

		default:
            ret = FALSE;
			break;
	}
	return ret;
}

void Uart_USB_Order(uint8_t *Command)
{
	xVarOrder_Source = Command;

	if (!UartOrder_CheckSum())
		return;
	if (!UartOrder_Review(&xVarHost_Token))
		return;

	switch (COMMAND(0))
	{
		case 'S' :
			UartAnswer_Ack(UART_USB,UART_USB_Order_System(), xVarHost_Token);
			break;

		case 'C' :
			UartAnswer_Ack(UART_USB,UART_USB_Order_Control(), xVarHost_Token);
			break;

		case 'T' :
			UartAnswer_Ack(UART_USB,UART_USB_Order_Test(), xVarHost_Token);
			break;	

        default:
            break;
	}
}

void	UartOrder_Initial()
{
	xVarHost_Token = 0;
}
