/*
 * heater_PWM.h
 *
 *  Created on: Nov 20, 2025
 *      Author: Bob Yu
 */

#ifndef INC_HEATER_PWM_H_
#define INC_HEATER_PWM_H_

#define PWM_MAX	100

enum
{
	HEATER_1 = 0,
	HEATER_2,
};

void heater_PWM_init(void);
void set_ALL_heater_off(void);
void set_heater_PWM(uint8_t ch,uint16_t pwm_value);

#endif /* INC_HEATER_PWM_H_ */
