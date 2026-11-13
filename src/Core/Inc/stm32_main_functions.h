/*
 * stm32_main_functions.h
 *
 *  Created on: Nov 20, 2025
 *      Author: Bob Yu
 */

#ifndef INC_STM32_MAIN_FUNCTIONS_H_
#define INC_STM32_MAIN_FUNCTIONS_H_

//=================================================================
//TIMER FUNCTION
//要預先把TIMER的HANDLE在main.h做EXTERN
//extern TIM_HandleTypeDef htim2;

#define TIMER2 2  //系統中斷
#define TIMER3 3  //蜂鳴器
#define TIMER4 4  //M1
#define TIMER5 5  //M2
#define TIMER6 6  //M3
#define TIMER7 7  //M4

#define TIMER_SET_RELOAD(timer_no , count_value) _TIMER_SET_RELOAD(timer_no , count_value)
#define _TIMER_SET_RELOAD(timer_no , count_value)  __HAL_TIM_SET_AUTORELOAD(&htim##timer_no, count_value);

#define TIMER_ON(timer_no)  _TIMER_ON(timer_no)
#define _TIMER_ON(timer_no)  HAL_TIM_Base_Start_IT(&htim##timer_no)

#define TIMER_OFF(timer_no)  _TIMER_OFF(timer_no)
#define _TIMER_OFF(timer_no)  HAL_TIM_Base_Stop_IT(&htim##timer_no)

#define FLAG_TIMER_1000mS	0x0001
#define FLAG_TIMER_500mS	0x0002
#define FLAG_TIMER_10mS		0x0004
#define FLAG_UART_RECEIVED  0x0010

#define HWI_REPAIR_1        (HAL_GPIO_ReadPin(Repair1_GPIO_Port, Repair1_Pin) == 0x01)
#define HWI_REPAIR_2        (HAL_GPIO_ReadPin(Repair2_GPIO_Port, Repair2_Pin) == 0x01)

//=================================================================================
//Set IO dir Function
void Set_IO_Output(GPIO_TypeDef *port, uint16_t pin);
void Set_IO_Input(GPIO_TypeDef *port, uint16_t pin);

//========================================================================================
// main systen timer
extern uint16_t Timer_Flag_0;

void Set_Timer_Flag(uint16_t timer_flag);
void Clear_Timer_Flag(uint16_t timer_flag);
uint16_t Get_Timer_Flag(uint16_t timer_flag);
uint32_t read_sensor_status(void);
//=================================================================

#endif /* INC_STM32_MAIN_FUNCTIONS_H_ */
