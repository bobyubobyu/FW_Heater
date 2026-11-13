/*
 * Fan.c
 *
 *  Created on: Nov 17, 2025
 *      Author: Bob Yu
 */
#include "main.h"

//設定風扇關閉/開啟
//0:關閉
//1:開啟
void set_FAN_on_off(uint8_t set_status)
{
	if(set_status == FAN_OFF)
		FAN_ON_L;
	else
		FAN_ON_H;
}

//設定風扇強度
//0-255
void set_FAN_power(uint8_t set_power)
{
	uint8_t pwr = set_power;
	if((pwr & 0x01) == 0x01)
		FAN_P1_H;
	else
		FAN_P1_L;

	pwr>>=1;
	if((pwr & 0x01) == 0x01)
		FAN_P2_H;
	else
		FAN_P2_L;

	pwr>>=1;
	if((pwr & 0x01) == 0x01)
		FAN_P3_H;
	else
		FAN_P3_L;

	pwr>>=1;
	if((pwr & 0x01) == 0x01)
		FAN_P4_H;
	else
		FAN_P4_L;

	pwr>>=1;
	if((pwr & 0x01) == 0x01)
		FAN_P5_H;
	else
		FAN_P5_L;

	pwr>>=1;
	if((pwr & 0x01) == 0x01)
		FAN_P6_H;
	else
		FAN_P6_L;

	pwr>>=1;
	if((pwr & 0x01) == 0x01)
		FAN_P7_H;
	else
		FAN_P7_L;

	pwr>>=1;
	if((pwr & 0x01) == 0x01)
		FAN_P8_H;
	else
		FAN_P8_L;
}

void set_FAN_total(uint8_t set_status , uint8_t set_power)
{
	set_FAN_on_off(set_status);
	set_FAN_power(set_power);
}

void FAN_init(void)
{
	set_FAN_total(FAN_OFF,0);
}
