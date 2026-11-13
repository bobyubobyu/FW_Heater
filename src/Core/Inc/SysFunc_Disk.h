/*
 * SysFunc_Disk.h
 *
 *  Created on: Nov 25, 2025
 *      Author: Bob Yu
 */

#ifndef INC_SYSFUNC_DISK_H_
#define INC_SYSFUNC_DISK_H_

#define		EE_EYE_START	0xFA
#define		EE_EYE_FINAL	0xFE

#define		EE_ADDR_BASE	0x000


//-------------------------------------------------------------------------------
#define		EE_TEST_0_ADDR		EE_ADDR_BASE				//此為0~511，單位為page
#define		EE_TEST_0_DATA_LEN	10							//此為資料長度，範圍為1~62
#define		EE_TEST_0_TOTAL_LEN	(EE_TEST_0_DATA_LEN + 2)  	//此為總長度，範圍為3~64
#define		EE_TEST_0_HEAD_POS	0							//此頁的起始
#define		EE_TEST_0_TAIL_POS	(EE_TEST_0_HEAD_POS + EE_TEST_0_TOTAL_LEN -1)  //此頁的結束

#define		EE_TEST_1_ADDR	(EE_TEST_0_ADDR + 1) 
#define		EE_TEST_1_DATA_LEN	16
#define		EE_TEST_1_TOTAL_LEN	(EE_TEST_1_DATA_LEN + 2)


enum
{
	DISK_ACT_RESET = 0,
	DISK_ACT_SAVE,
	DISK_ACT_LOAD,

	NUM_OF_DISK_ACT
};

void Func_Disk_init(void);
uint32_t FuncDisk_Test0(uint8_t Action);
uint8_t read_eeprom_data_test0(uint8_t idx);
#endif /* INC_SYSFUNC_DISK_H_ */
