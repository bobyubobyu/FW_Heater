/*
 * SysUart_Answer.h
 *
 *  Created on: Nov 21, 2025
 *      Author: Bob Yu
 */

#ifndef INC_SYSUART_ANSWER_H_
#define INC_SYSUART_ANSWER_H_

uint8_t UartAnswer_Ack(uint8_t uart_port_no,uint8_t Status, int16_t Index);
void UartAnswer_System_Info(uint8_t uart_port_no);

void UartAnswer_Log_Data(uint8_t Log_Flag);
void UartAnswer_Initial();

void Send_LOG(uint8_t uart_port_no);
void Send_EEPROM(uint8_t uart_port_no);
#endif /* INC_SYSUART_ANSWER_H_ */
