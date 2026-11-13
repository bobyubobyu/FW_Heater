/*
 * stm32_main_functions.c
 *
 *  Created on: Nov 20, 2025
 *      Author: Bob Yu
 */

#include "main.h"

uint32_t read_sensor_status(void)
{
    uint32_t sensor_status = 0;
    
    if(HWI_M1_CCW_LIMIT)  //M1 HOME
        sensor_status |= 0x01;
    if(HWI_M1_CW_LIMIT)  //M1 END
        sensor_status |= 0x02;        

    if(HWI_M2_CCW_LIMIT)  //M2 HOME
        sensor_status |= 0x04;
    if(HWI_M2_CW_LIMIT)  //M2 END
        sensor_status |= 0x08; 

    if(HWI_M3_CCW_LIMIT)  //M3 HOME
        sensor_status |= 0x10;
    if(HWI_M3_CW_LIMIT)  //M3 END
        sensor_status |= 0x20; 

    if(HWI_M4_CCW_LIMIT)  //M4 HOME
        sensor_status |= 0x40;
    if(HWI_M4_CW_LIMIT)  //M4 END
        sensor_status |= 0x80;         

    if(HWI_REPAIR_1)  //REPAIR1
        sensor_status |= 0x100;
    if(HWI_REPAIR_2)  //REPAIR2
        sensor_status |= 0x200;          

    return sensor_status;
}

void Set_IO_Output(GPIO_TypeDef *port, uint16_t pin)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(port, &GPIO_InitStruct);
}

void Set_IO_Input(GPIO_TypeDef *port, uint16_t pin)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(port, &GPIO_InitStruct);
}

uint16_t Timer_Flag_0 = 0;

void Set_Timer_Flag(uint16_t timer_flag)
{
	Timer_Flag_0 |= timer_flag;
}
void Clear_Timer_Flag(uint16_t timer_flag)
{
	Timer_Flag_0 &= ~timer_flag;
}
uint16_t Get_Timer_Flag(uint16_t timer_flag)
{
	return Timer_Flag_0 & timer_flag;
}
