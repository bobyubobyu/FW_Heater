/*
 * heater_PWM.c
 *
 *  Created on: Nov 20, 2025
 *      Author: Bob Yu
 */
#include "main.h"

//初始化 PWM
void heater_PWM_init(void)
{	
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_4);
	__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,0);
	__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_4,0);
}

void set_ALL_heater_off(void)
{
	__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,0);
	__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_4,0);
}

//設定PWM的值
//ch = HEATER_1、HEATER_2
//pwm_value = 0% ~ 50% = 0 ~ 100
//設定值請用0~100，會自動對應到0~2500
void set_heater_PWM(uint8_t ch,uint16_t pwm_value)
{
	if(pwm_value > PWM_MAX)
		pwm_value = PWM_MAX;
	pwm_value*=25;
	if(ch == HEATER_1)
	{
		__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,pwm_value);
	}
	else if(ch == HEATER_2)
	{
		__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_4,pwm_value);
	}
}
