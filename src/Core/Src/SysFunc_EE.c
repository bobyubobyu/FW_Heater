/*
 * SysFunc_EE.c
 *
 *  Created on: Nov 25, 2025
 *      Author: Bob Yu
 */

#include "main.h"
uint8_t	gVarEE_PageBuff[EE_PAGE_SIZE];

void FuncEE_Delay(uint64_t	Count)
{
	while (--Count != 0)
		__NOP();
}

void FuncEE_Start()
{
	HW_EE_SDA_OUT;
	FW_EE_TIME_READ;
	HW_EE_SDO_H;
	HW_EE_SCL_H;
	FW_EE_TIME_CLK;
	HW_EE_SDO_L;
	FW_EE_TIME_CLK;
	HW_EE_SCL_L;
	FW_EE_TIME_CLK;
}

void FuncEE_Stop()
{
	HW_EE_SDA_OUT;
	FW_EE_TIME_READ;
	HW_EE_SDO_L;
	HW_EE_SCL_H;
	FW_EE_TIME_CLK;
	HW_EE_SDO_H;
	FW_EE_TIME_CLK;
	HW_EE_SCL_L;
	FW_EE_TIME_CLK;
}

void FuncEE_Ack()
{
	HW_EE_SDA_OUT;
	FW_EE_TIME_READ;
	HW_EE_SDO_L;
	FW_EE_TIME_CLK;
	HW_EE_SCL_H;
	FW_EE_TIME_CLK;
	HW_EE_SCL_L;
	FW_EE_TIME_CLK;
}

void FuncEE_Nak()
{
	HW_EE_SDA_OUT;
	FW_EE_TIME_READ;
	HW_EE_SDO_H;
	FW_EE_TIME_CLK;
	HW_EE_SCL_H;
	FW_EE_TIME_CLK;
	HW_EE_SCL_L;
	FW_EE_TIME_CLK;
}

uint8_t	FuncEE_isAck()
{
	uint8_t	State;

	HW_EE_SDA_IN;
	//FW_EE_TIME_READ;
	HW_EE_SCL_H;
	FW_EE_TIME_CLK;
	State = HW_EE_SDI_S;
	HW_EE_SCL_L;
	FW_EE_TIME_CLK;

	if (State == 0)
		return	TRUE;
	else
		return	FALSE;
}

uint8_t	FuncEE_ReadByte()
{
	uint8_t	i;
	uint8_t	Result = 0;

	HW_EE_SDA_IN;
	FW_EE_TIME_READ;

	for (i = 0; i < 8; i++)
	{
		HW_EE_SCL_H;
		FW_EE_TIME_CLK;
		Result <<= 1;
		if (HW_EE_SDI_S != 0)
			Result++;
		HW_EE_SCL_L;
		FW_EE_TIME_CLK;
	}
	return	Result;
}

uint8_t	FuncEE_WriteByte(uint8_t Code)
{
	uint8_t	i;

	HW_EE_SDA_OUT;
	//FW_EE_TIME_READ;

	for (i = 0x80; i > 0; i >>= 1)
	{
		if ((Code & i) == 0)
			HW_EE_SDO_L;
		else
			HW_EE_SDO_H;

		FW_EE_TIME_HOLD;
		HW_EE_SCL_H;
		FW_EE_TIME_CLK;
		HW_EE_SCL_L;
		FW_EE_TIME_CLK;
	}
	return	FuncEE_isAck();
}

uint8_t	FuncEE_WriteAddr(uint16_t Addr)
{
	uint8_t	Code;
	FuncEE_Start();
	if (Addr < EE_PAGE_NUM)
		Code = 0xA0;
	else
		Code = 0xA2;

	if (FuncEE_WriteByte(Code) != TRUE)
		return	FALSE;

	Code = (uint8_t) (Addr >> 2) & 0x7F;

	if (FuncEE_WriteByte(Code) != TRUE)
		return	FALSE;

	Code = (uint8_t) (Addr & 0x03) << 6;

	return	FuncEE_WriteByte(Code);
}

uint8_t	FuncEE_ReadAddr(uint16_t Addr)
{
	uint8_t	Device;
	uint8_t	ret;
	if (FuncEE_WriteAddr(Addr) != TRUE)
	{
		return	FALSE;
	}


	FuncEE_Start();

	if (Addr < EE_PAGE_NUM)
		Device = 0xA1;
	else
		Device = 0xA3;

	ret = FuncEE_WriteByte(Device);
	return	ret;
}

uint8_t	FuncEE_WriteData(uint16_t Addr, uint8_t Length)
{
	uint8_t	i, Code;
	uint8_t	Success = FALSE;

	if (FuncEE_WriteAddr(Addr) == TRUE)
	{
		for (i = 0; i < Length; i++)
		{
			Code = gVarEE_PageBuff[i];
			Success = FuncEE_WriteByte(Code);
			if (Success != TRUE)
				break;
		}
		FuncEE_Stop();
		FW_EE_TIME_WRITE;
	}
	return	Success;
}


uint8_t	FuncEE_ReadData(uint16_t Addr, uint8_t Length)
{
	uint8_t	i;
	uint8_t	Success = FALSE;

	if (FuncEE_ReadAddr(Addr) == TRUE)
	{
		Success = TRUE;

		for (i = 0; i < Length; i++)
		{
			if (i != 0)
				FuncEE_Ack();

			gVarEE_PageBuff[i] = FuncEE_ReadByte();
		}

		FuncEE_Nak();
		FuncEE_Stop();
		FW_EE_TIME_READ;
	}
	if (Success != TRUE)
	{
		memset(gVarEE_PageBuff, 0, sizeof(uint8_t) * EE_PAGE_SIZE);
	}

	return	Success;
}

uint8_t	FuncEE_VerifyData(uint16_t Addr, uint8_t Length)
{
	uint8_t	i, Code;
	uint8_t	Success = FALSE;
	Success = FuncEE_ReadAddr(Addr);
	if (Success == TRUE)
	{
		Success = TRUE;

		for (i = 0; i < Length; i++)
		{
			if (i != 0)
				FuncEE_Ack();

			Code = FuncEE_ReadByte();

			if (gVarEE_PageBuff[i] != Code)
				Success = FALSE;

			if (Success != TRUE)
				break;
		}

		FuncEE_Nak();
		FuncEE_Stop();
		FW_EE_TIME_READ;
	}
	return	Success;
}

void	FuncEE_Tester()
{
	uint8_t	i;

	for (i = 0; i < EE_PAGE_SIZE; i++)
		gVarEE_PageBuff[i] = i;

	FuncEE_WriteData(12, EE_PAGE_SIZE);

	memset(gVarEE_PageBuff, 0, sizeof(uint8_t) * EE_PAGE_SIZE);

	FuncEE_ReadData(12, EE_PAGE_SIZE);
}

void	FuncEE_Initial()
{
	memset(gVarEE_PageBuff, 0, EE_PAGE_SIZE);

}




