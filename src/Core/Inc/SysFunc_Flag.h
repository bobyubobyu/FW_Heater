/*
 * SysFunc_Flag.h
 *
 *  Created on: Nov 20, 2025
 *      Author: Bob Yu
 */

#ifndef INC_SYSFUNC_FLAG_H_
#define INC_SYSFUNC_FLAG_H_

enum
{
	SIGNAL_UART_USB = 0,
	SIGNAL_UART_DEBUG,
	SIGNAL_UART_ARM,
	NUM_OF_SIGNAL
};

enum
{
	FLAG_TYPE_SYS = 0,
	FLAG_TYPE_UART,
	FLAG_TYPE_CTRL,
	FLAG_UART_COMMAND,

	NUM_OF_FLAG_TYPE
};

#define	FLAG_SYS_HOST				FLAG_TYPE_SYS, 0x01
#define	_1_16_S			   			FLAG_TYPE_SYS, 0x02
#define	FLAG_SYS_1SEC				FLAG_TYPE_SYS, 0x04
#define	FLAG_SYS_BEEP				FLAG_TYPE_SYS, 0x08
#define	FLAG_SYS_ERROR				FLAG_TYPE_SYS, 0x10
#define	FLAG_SYS_WARNING			FLAG_TYPE_SYS, 0x20
#define	FLAG_ADC_DATA				FLAG_TYPE_SYS, 0x40
#define	FLAG_LED_PROCESS			FLAG_TYPE_SYS, 0x80

#define	FLAG_UART_USB_BUSY			FLAG_TYPE_UART, 0x0001
#define	FLAG_UART_USB_RECEIVE		FLAG_TYPE_UART, 0x0002
#define	FLAG_UART_USB_COMMAND		FLAG_TYPE_UART, 0x0004

#define	FLAG_COMMAND_BUZZER			FLAG_UART_COMMAND, 0x0001
#define	FLAG_REPORT_INFO			FLAG_UART_COMMAND, 0x0002
#define	FLAG_REPORT_SLOW			FLAG_UART_COMMAND, 0x0004

extern	int16_t	xVarFlag_Signal[NUM_OF_SIGNAL];

void	SysFlag_Signal(uint16_t, int16_t, uint16_t, uint16_t);
void	SysFlag_Set(uint16_t, uint16_t);
uint8_t	SysFlag_Get(uint16_t, uint16_t);
void	SysFlag_Initial();


#endif /* INC_SYSFUNC_FLAG_H_ */
