/*
 * SysFunc_Disk.c
 *
 *  Created on: Nov 25, 2025
 *      Author: Bob Yu
 */

#include "main.h"

uint8_t eeprom_data_test0[EE_TEST_0_DATA_LEN] = {0};
uint8_t read_eeprom_data_test0(uint8_t idx)
{
	return eeprom_data_test0[idx];
}


void Func_Disk_init(void)
{
}

uint32_t FuncDisk_Test0(uint8_t Action)
{
	switch(Action)
	{
		case DISK_ACT_SAVE:
			gVarEE_PageBuff[EE_TEST_0_HEAD_POS] = EE_EYE_START;
			gVarEE_PageBuff[EE_TEST_0_TAIL_POS] = EE_EYE_FINAL;

			for(uint8_t i=1;i<=EE_TEST_0_DATA_LEN;i++)
			{
				gVarEE_PageBuff[i] = i-1;
			}
			FuncEE_WriteData(EE_TEST_0_ADDR, EE_TEST_0_TOTAL_LEN);
			if (FuncEE_VerifyData(EE_TEST_0_ADDR, EE_TEST_0_TOTAL_LEN))
			{
				return	SUCCESS;
			}
			else
			{
				return	FAIL;
			}

			break;

		case DISK_ACT_LOAD:
			FuncEE_ReadData(EE_TEST_0_ADDR, EE_TEST_0_TOTAL_LEN);
			if(gVarEE_PageBuff[EE_TEST_0_HEAD_POS] != EE_EYE_START || gVarEE_PageBuff[EE_TEST_0_TAIL_POS] != EE_EYE_FINAL)	//表示EEPROM 錯誤
				return FAIL;
			else
			{
				for(uint8_t i=1;i<=EE_TEST_0_DATA_LEN;i++)
				{
					eeprom_data_test0[i-1] = gVarEE_PageBuff[i];
				}
				return SUCCESS;				
			}

			break;

		default:
			return FALSE;
			break;
	}
}
