/*
 * adc_values.c
 *
 *  Created on: Nov 17, 2025
 *      Author: Bob Yu
 */

#include "main.h"

uint8_t ADC_Heater_Flag = 0;
uint16_t ADC_Heater_value[2];
uint32_t ADC_Heater_result[2][ADC_TOTAL];
uint8_t ADC_Heater_first_flag = 0;
uint8_t ADC_Heater_index = 0;

uint8_t ADC_PT1000_Flag = 0;
uint16_t ADC_PT1000_value[6];
uint32_t ADC_PT1000_result[6][ADC_TOTAL];
uint8_t ADC_PT1000_first_flag = 0;
uint8_t ADC_PT1000_index = 0;

//ADC初始化，執行一次即可
void ADC_init(void)
{
	  HAL_ADCEx_Calibration_Start(&hadc1, ADC_SINGLE_ENDED);
	  HAL_ADCEx_Calibration_Start(&hadc2, ADC_SINGLE_ENDED);
	  HAL_ADC_Start_DMA(&hadc1,(uint32_t*)ADC_Heater_value,sizeof(ADC_Heater_value)/sizeof(uint16_t));
	  HAL_ADC_Start_DMA(&hadc2,(uint32_t*)ADC_PT1000_value,sizeof(ADC_PT1000_value)/sizeof(uint16_t));
}

//DMA中斷副程式
//中斷了表示ADC1的2通道或是ADC2的6通道轉換完畢
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
	if(hadc == &hadc1)
	{
		ADC_Heater_Flag = 1;
		if(ADC_Heater_first_flag == 0)
		{
			ADC_Heater_result[0][ADC_Heater_index] = ADC_Heater_value[0];
			ADC_Heater_result[0][ADC_SUM] += ADC_Heater_result[0][ADC_Heater_index];
			ADC_Heater_result[1][ADC_Heater_index] = ADC_Heater_value[1];
			ADC_Heater_result[1][ADC_SUM] += ADC_Heater_result[1][ADC_Heater_index];
			ADC_Heater_index++;
			ADC_Heater_index%=ADC_WIN;
			if(ADC_Heater_index == 0)
				ADC_Heater_first_flag = 1;
		}
		else
		{
			ADC_Heater_result[0][ADC_SUM]-=ADC_Heater_result[0][ADC_Heater_index];
			ADC_Heater_result[0][ADC_Heater_index] = ADC_Heater_value[0];
			ADC_Heater_result[0][ADC_SUM]+=ADC_Heater_result[0][ADC_Heater_index];
			ADC_Heater_result[0][ADC_AVG]=ADC_Heater_result[0][ADC_SUM] / ADC_WIN;

			ADC_Heater_result[1][ADC_SUM]-=ADC_Heater_result[1][ADC_Heater_index];
			ADC_Heater_result[1][ADC_Heater_index] = ADC_Heater_value[1];
			ADC_Heater_result[1][ADC_SUM]+=ADC_Heater_result[1][ADC_Heater_index];
			ADC_Heater_result[1][ADC_AVG]=ADC_Heater_result[1][ADC_SUM] / ADC_WIN;

			ADC_Heater_index++;
			ADC_Heater_index%=ADC_WIN;
		}
	}

	if(hadc == &hadc2)
	{
		ADC_PT1000_Flag = 1;
		if(ADC_PT1000_first_flag == 0)
		{
			for(int i=0;i<6;i++)
			{
				ADC_PT1000_result[i][ADC_PT1000_index] = ADC_PT1000_value[i];
				ADC_PT1000_result[i][ADC_SUM] += ADC_PT1000_result[i][ADC_PT1000_index];
			}
			ADC_PT1000_index++;
			ADC_PT1000_index%=ADC_WIN;
			if(ADC_PT1000_index == 0)
				ADC_PT1000_first_flag = 1;
		}
		else
		{
			for(int i=0;i<6;i++)
			{
				ADC_PT1000_result[i][ADC_SUM]-=ADC_PT1000_result[i][ADC_PT1000_index];
				ADC_PT1000_result[i][ADC_PT1000_index] = ADC_PT1000_value[i];
				ADC_PT1000_result[i][ADC_SUM]+=ADC_PT1000_result[i][ADC_PT1000_index];
				ADC_PT1000_result[i][ADC_AVG]=ADC_PT1000_result[i][ADC_SUM] / ADC_WIN;
			}
			ADC_PT1000_index++;
			ADC_PT1000_index%=ADC_WIN;
		}
	}
}

//轉換完畢以後重新啟動DAM，這應該放在main loop下持續運作，但不可放在中斷副程式中
void reflush_ADC_DMA(void)
{
	if(ADC_Heater_Flag)
	{
		ADC_Heater_Flag = 0;
		HAL_ADC_Start_DMA(&hadc1,(uint32_t*)ADC_Heater_value,sizeof(ADC_Heater_value)/sizeof(uint16_t));

	}
	if(ADC_PT1000_Flag)
	{
		ADC_PT1000_Flag = 0;
		HAL_ADC_Start_DMA(&hadc2,(uint32_t*)ADC_PT1000_value,sizeof(ADC_PT1000_value)/sizeof(uint16_t));
	}
}

//讀取ADC的數值
//有需要代公式換算的話也可以寫在這邊
//要取資料的時候才算一次就好，不需要放在中斷裡面算
//溫度需要K，要再另外根據狀態補程式
uint32_t read_ADC_value(uint8_t ch)
{
	if(ch == ADC_HEATER_1)
		return ADC_Heater_result[0][ADC_AVG];
	else if(ch == ADC_HEATER_2)
		return ADC_Heater_result[1][ADC_AVG];
	else if(ch == ADC_PT1000_1)
		return ADC_PT1000_result[0][ADC_AVG];
	else if(ch == ADC_PT1000_2)
		return ADC_PT1000_result[1][ADC_AVG];
	else if(ch == ADC_PT1000_3)
		return ADC_PT1000_result[2][ADC_AVG];
	else if(ch == ADC_PT1000_4)
		return ADC_PT1000_result[3][ADC_AVG];
	else if(ch == ADC_PT1000_5)
		return ADC_PT1000_result[4][ADC_AVG];
	else if(ch == ADC_PT1000_6)
		return ADC_PT1000_result[5][ADC_AVG];
	else
		return 0xFFFF;


}
