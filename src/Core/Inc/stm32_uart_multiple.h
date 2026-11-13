/*
 * stm32_uartmultiple.h
 *
 *  Created on: Nov 20, 2025
 *      Author: Bob Yu
 */

#ifndef INC_STM32_UART_MULTIPLE_H_
#define INC_STM32_UART_MULTIPLE_H_


#define		SIZE_OF_TX_BUF			512
#define		SIZE_OF_RX_BUF			256

#define		UART_MARK_CRC1			'#'
#define		UART_MARK_CRC2			'%'
#define		UART_MARK_STOP			'!'
#define		UART_MARK_START			'*'

#define		UART_MARK_COMMA			','
#define		UART_MARK_OPEN			':'
#define		UART_MARK_CLOSE			':'

#define		SOURCE_FILL_BUFFER		0
#define		SOURCE_INTERRUPT		1

typedef struct
{
	//UART的物件
	UART_HandleTypeDef  *huarts;
	//接收實分析和戰存用
	uint8_t r_data;
	uint8_t xSysMain_Command[SIZE_OF_RX_BUF];

	//傳送接收的Buffer
	uint8_t xVarUart_TxBuffer[SIZE_OF_TX_BUF];
	uint8_t	xVarUart_RxBuffer[SIZE_OF_RX_BUF];

	//計算用的buffer
	uint8_t xVarUart_Work[10];

	//TX Buffer index
	uint16_t xVarUart_TxRead;
	uint16_t xVarUart_TxWrite;

	//RX Buffer index
	uint16_t xVarUart_RxRead;
	uint16_t xVarUart_RxWrite;

	//Check sum
	uint8_t	xVarUart_TxChecksum;

	//send flag
	uint8_t xVarUart_Tx_WorkingFlag;

	//get flag
	uint8_t xVarUart_Rx_Flag;
	uint8_t UART_Get_Command_Flag;


} _ARISE_UART_OBJECT;

enum
{
	UART_USB,
	UART_DEBUG,
	UART_MAX,
};

extern void ARISE_UART_Bind(_ARISE_UART_OBJECT *obj, UART_HandleTypeDef *src);
extern void Uart_variable_Initial(_ARISE_UART_OBJECT *obj,uint8_t num);
extern void Uart_Transmit(_ARISE_UART_OBJECT *uart, uint8_t source);
extern void Uart_out_char(uint8_t uart_port_no, uint8_t Code);
extern void Uart_Send_Text(uint8_t uart_port_no,uint8_t *buff);
extern void Uart_Send_Hex(uint8_t uart_port_no,uint8_t *buff, int32_t Value, uint8_t Length);
extern void Uart_Send_Value(uint8_t uart_port_no,uint8_t *buff, int32_t Value, uint8_t Length);
extern void	Uart_Send_Text_Num(uint8_t uart_port_no,uint8_t *buff,uint8_t num);

extern void Uart_Receive(_ARISE_UART_OBJECT *uart);
extern void	Uart_USB_Process(uint8_t* Command);

#endif /* INC_STM32_UART_MULTIPLE_H_ */
