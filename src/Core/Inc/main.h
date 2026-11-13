/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32g4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "string.h"
#include "stdbool.h"

#include "SysFunc_Flag.h"
#include "stm32_main_functions.h"

#include "adc.h"
#include "fan.h"
#include "te.h"
#include "heater_PWM.h"
#include "Sys_buzzer.h"
#include "stm32_uart_multiple.h"
#include "SysCtrl_Task.h"
#include "SysUart_Order.h"
#include "SysUart_Answer.h"
#include "SysFunc_EE.h"
#include "SysFunc_DisK.h"
#include "motor_Botton_Z.h"
#include "motor_Botton_Y.h"
#include "motor_Top_Y.h"
#include "motor_Top_X.h"

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
extern ADC_HandleTypeDef hadc1;
extern ADC_HandleTypeDef hadc2;
extern DMA_HandleTypeDef hdma_adc1;
extern DMA_HandleTypeDef hdma_adc2;
extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;
extern TIM_HandleTypeDef htim5;
extern TIM_HandleTypeDef htim6;
extern TIM_HandleTypeDef htim7;

extern _ARISE_UART_OBJECT auart[UART_MAX];
/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define FAN_P1_Pin GPIO_PIN_2
#define FAN_P1_GPIO_Port GPIOE
#define FAN_P2_Pin GPIO_PIN_3
#define FAN_P2_GPIO_Port GPIOE
#define FAN_P3_Pin GPIO_PIN_4
#define FAN_P3_GPIO_Port GPIOE
#define FAN_P4_Pin GPIO_PIN_5
#define FAN_P4_GPIO_Port GPIOE
#define FAN_P5_Pin GPIO_PIN_6
#define FAN_P5_GPIO_Port GPIOE
#define FAN_P6_Pin GPIO_PIN_13
#define FAN_P6_GPIO_Port GPIOC
#define FAN_P7_Pin GPIO_PIN_9
#define FAN_P7_GPIO_Port GPIOF
#define FAN_P8_Pin GPIO_PIN_10
#define FAN_P8_GPIO_Port GPIOF
#define Heater1_NTC_Pin GPIO_PIN_0
#define Heater1_NTC_GPIO_Port GPIOC
#define Heater1_ON_Pin GPIO_PIN_1
#define Heater1_ON_GPIO_Port GPIOC
#define Heater2_NTC_Pin GPIO_PIN_2
#define Heater2_NTC_GPIO_Port GPIOC
#define Heater2_ON_Pin GPIO_PIN_3
#define Heater2_ON_GPIO_Port GPIOC
#define PCLK_TE_Pin GPIO_PIN_2
#define PCLK_TE_GPIO_Port GPIOF
#define PLAT_TE_Pin GPIO_PIN_0
#define PLAT_TE_GPIO_Port GPIOA
#define PSDI_TE_Pin GPIO_PIN_1
#define PSDI_TE_GPIO_Port GPIOA
#define POE_TE_Pin GPIO_PIN_2
#define POE_TE_GPIO_Port GPIOA
#define BUZZER_Pin GPIO_PIN_3
#define BUZZER_GPIO_Port GPIOA
#define PT1000_1_Pin GPIO_PIN_4
#define PT1000_1_GPIO_Port GPIOA
#define PT1000_2_Pin GPIO_PIN_5
#define PT1000_2_GPIO_Port GPIOA
#define PT1000_3_Pin GPIO_PIN_6
#define PT1000_3_GPIO_Port GPIOA
#define PT1000_4_Pin GPIO_PIN_7
#define PT1000_4_GPIO_Port GPIOA
#define PT1000_5_Pin GPIO_PIN_4
#define PT1000_5_GPIO_Port GPIOC
#define PT1000_6_Pin GPIO_PIN_5
#define PT1000_6_GPIO_Port GPIOC
#define TE1_PW_ON_Pin GPIO_PIN_1
#define TE1_PW_ON_GPIO_Port GPIOB
#define TE1_PW_SE_Pin GPIO_PIN_2
#define TE1_PW_SE_GPIO_Port GPIOB
#define TE2_PW_ON_Pin GPIO_PIN_7
#define TE2_PW_ON_GPIO_Port GPIOE
#define TE2_PW_SE_Pin GPIO_PIN_8
#define TE2_PW_SE_GPIO_Port GPIOE
#define TE3_PW_ON_Pin GPIO_PIN_9
#define TE3_PW_ON_GPIO_Port GPIOE
#define TE3_PW_SE_Pin GPIO_PIN_10
#define TE3_PW_SE_GPIO_Port GPIOE
#define TE4_PW_ON_Pin GPIO_PIN_11
#define TE4_PW_ON_GPIO_Port GPIOE
#define TE4_PW_SE_Pin GPIO_PIN_12
#define TE4_PW_SE_GPIO_Port GPIOE
#define TE5_PW_ON_Pin GPIO_PIN_13
#define TE5_PW_ON_GPIO_Port GPIOE
#define TE5_PW_SE_Pin GPIO_PIN_14
#define TE5_PW_SE_GPIO_Port GPIOE
#define TE6_PW_ON_Pin GPIO_PIN_15
#define TE6_PW_ON_GPIO_Port GPIOE
#define TE6_PW_SE_Pin GPIO_PIN_10
#define TE6_PW_SE_GPIO_Port GPIOB
#define TEST_PIN_Pin GPIO_PIN_11
#define TEST_PIN_GPIO_Port GPIOB
#define Botton_Z_LC_Pin GPIO_PIN_12
#define Botton_Z_LC_GPIO_Port GPIOB
#define Botton_Z_Flag_Pin GPIO_PIN_13
#define Botton_Z_Flag_GPIO_Port GPIOB
#define Botton_Z_DR_Pin GPIO_PIN_14
#define Botton_Z_DR_GPIO_Port GPIOB
#define Botton_Z_RESET_Pin GPIO_PIN_15
#define Botton_Z_RESET_GPIO_Port GPIOB
#define Botton_Z_PU_Pin GPIO_PIN_10
#define Botton_Z_PU_GPIO_Port GPIOD
#define Botton_Y_LC_Pin GPIO_PIN_11
#define Botton_Y_LC_GPIO_Port GPIOD
#define Botton_Y_Flag_Pin GPIO_PIN_12
#define Botton_Y_Flag_GPIO_Port GPIOD
#define Botton_Y_DR_Pin GPIO_PIN_13
#define Botton_Y_DR_GPIO_Port GPIOD
#define Botton_Y_RESET_Pin GPIO_PIN_14
#define Botton_Y_RESET_GPIO_Port GPIOD
#define Botton_Y_PU_Pin GPIO_PIN_15
#define Botton_Y_PU_GPIO_Port GPIOD
#define Repair1_Pin GPIO_PIN_6
#define Repair1_GPIO_Port GPIOC
#define Repair2_Pin GPIO_PIN_7
#define Repair2_GPIO_Port GPIOC
#define SCL_Pin GPIO_PIN_8
#define SCL_GPIO_Port GPIOC
#define SDA_Pin GPIO_PIN_9
#define SDA_GPIO_Port GPIOC
#define Backup1_Pin GPIO_PIN_8
#define Backup1_GPIO_Port GPIOA
#define Backup2_Pin GPIO_PIN_11
#define Backup2_GPIO_Port GPIOA
#define TOP_Y_LC_Pin GPIO_PIN_12
#define TOP_Y_LC_GPIO_Port GPIOA
#define TOP_Y_Flag_Pin GPIO_PIN_15
#define TOP_Y_Flag_GPIO_Port GPIOA
#define TOP_Y_DR_Pin GPIO_PIN_10
#define TOP_Y_DR_GPIO_Port GPIOC
#define TOP_Y_RESET_Pin GPIO_PIN_11
#define TOP_Y_RESET_GPIO_Port GPIOC
#define TOP_Y_PU_Pin GPIO_PIN_12
#define TOP_Y_PU_GPIO_Port GPIOC
#define TOP_X_LC_Pin GPIO_PIN_0
#define TOP_X_LC_GPIO_Port GPIOD
#define TOP_X_Flag_Pin GPIO_PIN_1
#define TOP_X_Flag_GPIO_Port GPIOD
#define TOP_X_DR_Pin GPIO_PIN_2
#define TOP_X_DR_GPIO_Port GPIOD
#define TOP_X_RESET_Pin GPIO_PIN_3
#define TOP_X_RESET_GPIO_Port GPIOD
#define TOP_X_PU_Pin GPIO_PIN_4
#define TOP_X_PU_GPIO_Port GPIOD
#define TOP_Y_END_Pin GPIO_PIN_7
#define TOP_Y_END_GPIO_Port GPIOD
#define TOP_Y_HOME_Pin GPIO_PIN_3
#define TOP_Y_HOME_GPIO_Port GPIOB
#define TOP_X_END_Pin GPIO_PIN_4
#define TOP_X_END_GPIO_Port GPIOB
#define TOP_X_HOME_Pin GPIO_PIN_5
#define TOP_X_HOME_GPIO_Port GPIOB
#define Botton_Z_END_Pin GPIO_PIN_6
#define Botton_Z_END_GPIO_Port GPIOB
#define Botton_Z_HOME_Pin GPIO_PIN_7
#define Botton_Z_HOME_GPIO_Port GPIOB
#define Botton_Y_END_Pin GPIO_PIN_9
#define Botton_Y_END_GPIO_Port GPIOB
#define Botton_Y_HOME_Pin GPIO_PIN_0
#define Botton_Y_HOME_GPIO_Port GPIOE
#define FAN_ON_Pin GPIO_PIN_1
#define FAN_ON_GPIO_Port GPIOE

/* USER CODE BEGIN Private defines */
#define		TRUE		1
#define		FALSE		0
#define		true		1
#define		false		0
#define		SUCCESS		1
#define		FAIL		0

#define 	HW_VERSION 			9
#define 	FW_VERSION_MAJOR	9
#define 	FW_VERSION_MINOR	9
#define 	FW_VERSION_BUILD  	9

#define 	BIT0                (0x0001)
#define 	BIT1                (0x0002)
#define 	BIT2                (0x0004)
#define 	BIT3                (0x0008)
#define 	BIT4                (0x0010)
#define 	BIT5                (0x0020)
#define 	BIT6                (0x0040)
#define 	BIT7                (0x0080)
#define 	BIT8                (0x0100)
#define 	BIT9                (0x0200)
#define 	BITA                (0x0400)
#define 	BITB                (0x0800)
#define 	BITC                (0x1000)
#define 	BITD                (0x2000)
#define 	BITE                (0x4000)
#define 	BITF                (0x8000)
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
