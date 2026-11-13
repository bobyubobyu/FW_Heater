/*
 * te.c
 *
 *  Created on: Nov 18, 2025
 *      Author: Bob Yu
 */

#include "main.h"

uint8_t TE_power_array[TE_CH_NUM] = {0};

//送出陣列中的資料
//因為單晶片很快，如果訊號太快造成錯誤，應增加DELAY，需要測試
void update_TE_power_HW(void)
{
	POE_TE_L;  //OE應該要常時保持為LOW
	PLAT_TE_L; //RCLK設定為LOW，暫存器的值不送出
	for(int i = TE_CH_NUM - 1; i >=  0; i--)  //順序為 TE6 -> TE1
	{
		for(int j = 7 ; j >=  0; j--) //順序為 MSB -> LSB
		{
			PCLK_TE_L; //SRCLK = 0 裝填資料	
			if((TE_power_array[i] & (1 << j)) > 0)
				PSDI_TE_H;
			else
				PSDI_TE_L;	
			PCLK_TE_H; //SRCLK = 1 位移資料					
		}
	}
	PLAT_TE_H; //RCLK設定為HI 送出暫存器的值
}

// 用陣列方式更新 6個CH的強度
// TE_pow_array[TE1] -> P1
// TE_pow_array[TE2] -> P2
// TE_pow_array[TE3] -> P3
// TE_pow_array[TE4] -> P4
// TE_pow_array[TE5] -> P5
// TE_pow_array[TE6] -> P6
void set_TE_power_array(uint8_t *TE_pow_array)
{
	for(uint8_t i=0 ; i<TE_CH_NUM ; i++)
	{
		TE_power_array[i] = TE_pow_array[i];
	}
	update_TE_power_HW();
}

// 更新單一CH的強度
// ch = TE1~TE6
// TE_pow_value_single = 0-255
void set_SINGLE_TE_power(uint8_t ch,uint8_t TE_pow_value_single)
{	
	TE_power_array[ch] = TE_pow_value_single;
	update_TE_power_HW();
}

// 用 0x000000000000 ~ 0xFFFFFFFFFFFF 的值一次更新6個CH的強度
void set_ALL_TE_power_value(uint64_t TE_pow_value)
{
	uint64_t temp0 = TE_pow_value;
	for(uint8_t i=0 ; i<TE_CH_NUM ; i++)
	{
		temp0 >>= (i * 8);
		TE_power_array[i] = (uint8_t)(temp0 & 0xFF);
	}
	update_TE_power_HW();
}

//設定各別CH的TE ON/OFF
//TE_OFF:關閉
//TE_ON:開啟
void set_TE_on_off(uint8_t ch,uint8_t set_status)
{
	if(ch == TE1)
	{
		if(set_status == TE_OFF)
		{
			TE1_PW_ON_L;
		}
		else if(set_status == TE_ON)
		{
			TE1_PW_ON_H;
		}
	}
	else if(ch == TE2)
	{
		if(set_status == TE_OFF)
		{
			TE2_PW_ON_L;
		}
		else if(set_status == TE_ON)
		{
			TE2_PW_ON_H;
		}
	}
	else if(ch == TE3)
	{
		if(set_status == TE_OFF)
		{
			TE3_PW_ON_L;
		}
		else if(set_status == TE_ON)
		{
			TE3_PW_ON_H;
		}
	}
	else if(ch == TE4)
	{
		if(set_status == TE_OFF)
		{
			TE4_PW_ON_L;
		}
		else if(set_status == TE_ON)
		{
			TE4_PW_ON_H;
		}
	}
	else if(ch == TE5)
	{
		if(set_status == TE_OFF)
		{
			TE5_PW_ON_L;
		}
		else if(set_status == TE_ON)
		{
			TE5_PW_ON_H;
		}
	}
	else if(ch == TE6)
	{
		if(set_status == TE_OFF)
		{
			TE6_PW_ON_L;
		}
		else if(set_status == TE_ON)
		{
			TE6_PW_ON_H;
		}
	}
}

//一次設定所有TE電源
void set_ALL_TE_on_off(uint8_t set_status)
{
	for(uint8_t i=0;i<TE_CH_NUM;i++)
	{
		set_TE_on_off(i,set_status);
	}
}

//設定各別CH的TE是製冷或是加熱，需先開啟TE電源才有作用
//set_dir = TE_COLD  製冷
//set_dir = TE_HOT  加熱
void set_TE_dir(uint8_t ch,uint8_t set_dir)
{
	if(ch == TE1)
	{
		if(set_dir == TE_HOT)
		{
			TE1_PW_SE_L;
		}
		else if(set_dir == TE_COLD)
		{
			TE1_PW_SE_H;
		}
	}
	else if(ch == TE2)
	{
		if(set_dir == TE_HOT)
		{
			TE2_PW_SE_L;
		}
		else if(set_dir == TE_COLD)
		{
			TE2_PW_SE_H;
		}
	}
	else if(ch == TE3)
	{
		if(set_dir == TE_HOT)
		{
			TE3_PW_SE_L;
		}
		else if(set_dir == TE_COLD)
		{
			TE3_PW_SE_H;
		}
	}
	else if(ch == TE4)
	{
		if(set_dir == TE_HOT)
		{
			TE4_PW_SE_L;
		}
		else if(set_dir == TE_COLD)
		{
			TE4_PW_SE_H;
		}
	}
	else if(ch == TE5)
	{
		if(set_dir == TE_HOT)
		{
			TE5_PW_SE_L;
		}
		else if(set_dir == TE_COLD)
		{
			TE5_PW_SE_H;
		}
	}
	else if(ch == TE6)
	{
		if(set_dir == TE_HOT)
		{
			TE6_PW_SE_L;
		}
		else if(set_dir == TE_COLD)
		{
			TE6_PW_SE_H;
		}
	}
}

//一次設定所有TE為加熱或是製冷
void set_ALL_TE_dir(uint8_t set_dir)
{
	for(uint8_t i=0;i<TE_CH_NUM;i++)
	{
		set_TE_dir(i,set_dir);
	}
}

//各別設定TE的電源和製冷或加熱
void set_SINGLE_TE_total(uint8_t ch,uint8_t set_status,uint8_t set_dir)
{
	set_TE_dir(ch,set_dir);
	set_TE_on_off(ch,set_status);
}

//一次設定所有TE的電源和製冷或加熱
void set_ALL_TE_total(uint8_t set_status,uint8_t set_dir)
{
	set_ALL_TE_on_off(set_status);	
	set_ALL_TE_dir(set_dir);
}

void TE_init(void)
{
	set_ALL_TE_total(TE_OFF,TE_HOT);
	POE_TE_L;  //74595 OE
	set_ALL_TE_power_value(0);
}

