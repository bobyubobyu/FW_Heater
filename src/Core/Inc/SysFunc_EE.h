/*
 * SysFunc_EE.h
 *
 *  Created on: Nov 25, 2025
 *      Author: Bob Yu
 */

#ifndef INC_SYSFUNC_EE_H_
#define INC_SYSFUNC_EE_H_

//void Set_IO_Output(GPIO_TypeDef *port, uint16_t pin);
//void Set_IO_Input(GPIO_TypeDef *port, uint16_t pin);
//#define		HWO_M3_LOW				    HAL_GPIO_WritePin (TOP_Y_PU_GPIO_Port, TOP_Y_PU_Pin,0)

#define		HW_EE_SCL_H		HAL_GPIO_WritePin (SCL_GPIO_Port, SCL_Pin,1)
#define		HW_EE_SCL_L		HAL_GPIO_WritePin (SCL_GPIO_Port, SCL_Pin,0)
#define		HW_EE_SDA_OUT	Set_IO_Output(SDA_GPIO_Port, SDA_Pin)
#define		HW_EE_SDA_IN	Set_IO_Input(SDA_GPIO_Port, SDA_Pin)
#define		HW_EE_SDO_H		HAL_GPIO_WritePin (SDA_GPIO_Port, SDA_Pin,1)
#define		HW_EE_SDO_L		HAL_GPIO_WritePin (SDA_GPIO_Port, SDA_Pin,0)
#define		HW_EE_SDI_S		HAL_GPIO_ReadPin(SDA_GPIO_Port, SDA_Pin)

#define		EE_PAGE_NUM				512
#define		EE_PAGE_SIZE			64

#define	    FW_EE_TIME_CLK		FuncEE_Delay(60)
#define 	FW_EE_TIME_HOLD		FuncEE_Delay(80)
#define	    FW_EE_TIME_READ		FuncEE_Delay(240)
#define	    FW_EE_TIME_WRITE	FuncEE_Delay(50000)

//extern	UINT8	gVarEE_PageBuff[];
//---------------------------------------------------------------
uint8_t	FuncEE_ReadData(uint16_t Addr, uint8_t Length);

uint8_t	FuncEE_WriteData(uint16_t Addr, uint8_t Length);

uint8_t	FuncEE_VerifyData(uint16_t Addr, uint8_t Length);

void	FuncEE_Initial();

extern uint8_t	gVarEE_PageBuff[EE_PAGE_SIZE];

#endif /* INC_SYSFUNC_EE_H_ */
