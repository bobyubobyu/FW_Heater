/*
 * te.h
 *
 *  Created on: Nov 18, 2025
 *      Author: Bob Yu
 */

#ifndef INC_TE_H_
#define INC_TE_H_

#define  TE1_PW_SE_H	HAL_GPIO_WritePin (TE1_PW_SE_GPIO_Port, TE1_PW_SE_Pin,1)
#define  TE1_PW_SE_L	HAL_GPIO_WritePin (TE1_PW_SE_GPIO_Port, TE1_PW_SE_Pin,0)
#define  TE1_PW_ON_H	HAL_GPIO_WritePin (TE1_PW_ON_GPIO_Port, TE1_PW_ON_Pin,1)
#define  TE1_PW_ON_L	HAL_GPIO_WritePin (TE1_PW_ON_GPIO_Port, TE1_PW_ON_Pin,0)

#define  TE2_PW_SE_H	HAL_GPIO_WritePin (TE2_PW_SE_GPIO_Port, TE2_PW_SE_Pin,1)
#define  TE2_PW_SE_L	HAL_GPIO_WritePin (TE2_PW_SE_GPIO_Port, TE2_PW_SE_Pin,0)
#define  TE2_PW_ON_H	HAL_GPIO_WritePin (TE2_PW_ON_GPIO_Port, TE2_PW_ON_Pin,1)
#define  TE2_PW_ON_L	HAL_GPIO_WritePin (TE2_PW_ON_GPIO_Port, TE2_PW_ON_Pin,0)

#define  TE3_PW_SE_H	HAL_GPIO_WritePin (TE3_PW_SE_GPIO_Port, TE3_PW_SE_Pin,1)
#define  TE3_PW_SE_L	HAL_GPIO_WritePin (TE3_PW_SE_GPIO_Port, TE3_PW_SE_Pin,0)
#define  TE3_PW_ON_H	HAL_GPIO_WritePin (TE3_PW_ON_GPIO_Port, TE3_PW_ON_Pin,1)
#define  TE3_PW_ON_L	HAL_GPIO_WritePin (TE3_PW_ON_GPIO_Port, TE3_PW_ON_Pin,0)

#define  TE4_PW_SE_H	HAL_GPIO_WritePin (TE4_PW_SE_GPIO_Port, TE4_PW_SE_Pin,1)
#define  TE4_PW_SE_L	HAL_GPIO_WritePin (TE4_PW_SE_GPIO_Port, TE4_PW_SE_Pin,0)
#define  TE4_PW_ON_H	HAL_GPIO_WritePin (TE4_PW_ON_GPIO_Port, TE4_PW_ON_Pin,1)
#define  TE4_PW_ON_L	HAL_GPIO_WritePin (TE4_PW_ON_GPIO_Port, TE4_PW_ON_Pin,0)

#define  TE5_PW_SE_H	HAL_GPIO_WritePin (TE5_PW_SE_GPIO_Port, TE5_PW_SE_Pin,1)
#define  TE5_PW_SE_L	HAL_GPIO_WritePin (TE5_PW_SE_GPIO_Port, TE5_PW_SE_Pin,0)
#define  TE5_PW_ON_H	HAL_GPIO_WritePin (TE5_PW_ON_GPIO_Port, TE5_PW_ON_Pin,1)
#define  TE5_PW_ON_L	HAL_GPIO_WritePin (TE5_PW_ON_GPIO_Port, TE5_PW_ON_Pin,0)

#define  TE6_PW_SE_H	HAL_GPIO_WritePin (TE6_PW_SE_GPIO_Port, TE6_PW_SE_Pin,1)
#define  TE6_PW_SE_L	HAL_GPIO_WritePin (TE6_PW_SE_GPIO_Port, TE6_PW_SE_Pin,0)
#define  TE6_PW_ON_H	HAL_GPIO_WritePin (TE6_PW_ON_GPIO_Port, TE6_PW_ON_Pin,1)
#define  TE6_PW_ON_L	HAL_GPIO_WritePin (TE6_PW_ON_GPIO_Port, TE6_PW_ON_Pin,0)

#define  POE_TE_H		HAL_GPIO_WritePin (POE_TE_GPIO_Port, POE_TE_Pin,0)
#define  POE_TE_L		HAL_GPIO_WritePin (POE_TE_GPIO_Port, POE_TE_Pin,1)

#define  PCLK_TE_H		HAL_GPIO_WritePin (PCLK_TE_GPIO_Port, PCLK_TE_Pin,1)
#define  PCLK_TE_L		HAL_GPIO_WritePin (PCLK_TE_GPIO_Port, PCLK_TE_Pin,0)

#define  PLAT_TE_H		HAL_GPIO_WritePin (PLAT_TE_GPIO_Port, PLAT_TE_Pin,1)
#define  PLAT_TE_L		HAL_GPIO_WritePin (PLAT_TE_GPIO_Port, PLAT_TE_Pin,0)

#define  PSDI_TE_H		HAL_GPIO_WritePin (PSDI_TE_GPIO_Port, PSDI_TE_Pin,1)
#define  PSDI_TE_L		HAL_GPIO_WritePin (PSDI_TE_GPIO_Port, PSDI_TE_Pin,0)

#define	 TE_CH_NUM		6

enum
{
	TE_OFF = 0,
	TE_ON,
};

enum
{
	TE_HOT = 0,
	TE_COLD,
};

enum
{
	TE1 = 0,
	TE2,
	TE3,
	TE4,
	TE5,
	TE6,
};

void set_TE_on_off(uint8_t ch,uint8_t set_status);
void set_ALL_TE_on_off(uint8_t set_status);
void set_TE_dir(uint8_t ch,uint8_t set_dir);
void set_ALL_TE_dir(uint8_t set_dir);
void set_SINGLE_TE_total(uint8_t ch,uint8_t set_status,uint8_t set_dir);
void set_ALL_TE_total(uint8_t set_status,uint8_t set_dir);
void TE_init(void);
void set_TE_power_array(uint8_t *TE_pow_array);
void set_SINGLE_TE_power(uint8_t ch,uint8_t TE_pow_value_single);
void set_ALL_TE_power_value(uint64_t TE_pow_value);
#endif /* INC_TE_H_ */
