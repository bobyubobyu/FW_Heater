/*
 * SysUart_Order.h
 *
 *  Created on: Nov 21, 2025
 *      Author: Bob Yu
 */

#ifndef INC_SYSUART_ORDER_H_
#define INC_SYSUART_ORDER_H_

#define BOTTON_Z	'1'
#define BOTTON_Y	'2'
#define TOP_Y		'3'
#define TOP_X		'4'

extern void UartOrder_Initial();
extern void Uart_USB_Order(uint8_t *Command);

#endif /* INC_SYSUART_ORDER_H_ */
