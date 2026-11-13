/*
 * Fan.h
 *
 *  Created on: Nov 17, 2025
 *      Author: Bob Yu
 */

#ifndef INC_FAN_H_
#define INC_FAN_H_

#define	FAN_ON_H		HAL_GPIO_WritePin (FAN_ON_GPIO_Port, FAN_ON_Pin,1)
#define	FAN_ON_L		HAL_GPIO_WritePin (FAN_ON_GPIO_Port, FAN_ON_Pin,0)

#define	FAN_P1_H		HAL_GPIO_WritePin (FAN_P1_GPIO_Port, FAN_P1_Pin,1)
#define	FAN_P1_L		HAL_GPIO_WritePin (FAN_P1_GPIO_Port, FAN_P1_Pin,0)

#define	FAN_P2_H		HAL_GPIO_WritePin (FAN_P2_GPIO_Port, FAN_P2_Pin,1)
#define	FAN_P2_L		HAL_GPIO_WritePin (FAN_P2_GPIO_Port, FAN_P2_Pin,0)

#define	FAN_P3_H		HAL_GPIO_WritePin (FAN_P3_GPIO_Port, FAN_P3_Pin,1)
#define	FAN_P3_L		HAL_GPIO_WritePin (FAN_P3_GPIO_Port, FAN_P3_Pin,0)

#define	FAN_P4_H		HAL_GPIO_WritePin (FAN_P4_GPIO_Port, FAN_P4_Pin,1)
#define	FAN_P4_L		HAL_GPIO_WritePin (FAN_P4_GPIO_Port, FAN_P4_Pin,0)

#define	FAN_P5_H		HAL_GPIO_WritePin (FAN_P5_GPIO_Port, FAN_P5_Pin,1)
#define	FAN_P5_L		HAL_GPIO_WritePin (FAN_P5_GPIO_Port, FAN_P5_Pin,0)

#define	FAN_P6_H		HAL_GPIO_WritePin (FAN_P6_GPIO_Port, FAN_P6_Pin,1)
#define	FAN_P6_L		HAL_GPIO_WritePin (FAN_P6_GPIO_Port, FAN_P6_Pin,0)

#define	FAN_P7_H		HAL_GPIO_WritePin (FAN_P7_GPIO_Port, FAN_P7_Pin,1)
#define	FAN_P7_L		HAL_GPIO_WritePin (FAN_P7_GPIO_Port, FAN_P7_Pin,0)

#define	FAN_P8_H		HAL_GPIO_WritePin (FAN_P8_GPIO_Port, FAN_P8_Pin,1)
#define	FAN_P8_L		HAL_GPIO_WritePin (FAN_P8_GPIO_Port, FAN_P8_Pin,0)


enum
{
	FAN_OFF = 0,
	FAN_ON,
};

void set_FAN_on_off(uint8_t set_status);
void set_FAN_power(uint8_t set_power);
void set_FAN_total(uint8_t set_status , uint8_t set_power);
void FAN_init(void);
#endif /* INC_FAN_H_ */
