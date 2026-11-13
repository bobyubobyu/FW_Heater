/*
 * stm32_uartmultiple.c
 *
 *  Created on: Nov 20, 2025
 *      Author: Bob Yu
 */

#include "main.h"

int16_t UART_command_counter = 0;

//將系統的UART物件綁定到結構陣列的物件上
void ARISE_UART_Bind(_ARISE_UART_OBJECT *obj, UART_HandleTypeDef *src)
{
    obj->huarts = src;
}

//初始化結構變數
void Uart_variable_Initial(_ARISE_UART_OBJECT *obj,uint8_t num)
{
	for(uint8_t i=0;i<num;i++)
	{
		obj[i].huarts = NULL;
		obj[i].UART_Get_Command_Flag = 0;
		obj[i].r_data = 0;
		obj[i].xVarUart_RxRead = 0;
		obj[i].xVarUart_RxWrite = 1;
		obj[i].xVarUart_Rx_Flag = 0;
		obj[i].xVarUart_TxChecksum = 0;
		obj[i].xVarUart_TxRead = 0;
		obj[i].xVarUart_TxWrite = 0;
		obj[i].xVarUart_Tx_WorkingFlag = 0;
		memset(obj[i].xVarUart_RxBuffer, 0, SIZE_OF_RX_BUF);
		memset(obj[i].xSysMain_Command, 0, SIZE_OF_RX_BUF);
		memset(obj[i].xVarUart_TxBuffer, 0, SIZE_OF_TX_BUF);
		memset(obj[i].xVarUart_Work, 0, 10);
	}
}

void Uart_Judge_State(_ARISE_UART_OBJECT *uart)
{
	if (uart->xVarUart_TxRead == uart->xVarUart_TxWrite)
		uart->xVarUart_Tx_WorkingFlag = 0;
}

void Uart_Transmit(_ARISE_UART_OBJECT *uart, uint8_t source)
{
    if (uart == NULL)
    {
        return;
    }

    if (uart->xVarUart_TxRead != uart->xVarUart_TxWrite)
    {
        if (uart->xVarUart_TxBuffer[uart->xVarUart_TxRead] == UART_MARK_START)
        {
            uart->xVarUart_TxChecksum = 0;
            HAL_UART_Transmit_IT(uart->huarts, &uart->xVarUart_TxBuffer[uart->xVarUart_TxRead], 1);
        }
        else if (uart->xVarUart_TxBuffer[uart->xVarUart_TxRead] == UART_MARK_CRC1)
        {
            uart->xVarUart_TxChecksum ^= 0xFF;
            uart->xVarUart_TxChecksum += 1;
            uint8_t CRC_temp1 = (uart->xVarUart_TxChecksum >> 4) + 'A';
            HAL_UART_Transmit_IT(uart->huarts, &CRC_temp1, 1);
        }
        else if (uart->xVarUart_TxBuffer[uart->xVarUart_TxRead] == UART_MARK_CRC2)
        {
            uint8_t CRC_temp2 = (uart->xVarUart_TxChecksum & 0x0F) + 'A';
            HAL_UART_Transmit_IT(uart->huarts, &CRC_temp2, 1);
        }
        else
        {
            uart->xVarUart_TxChecksum += uart->xVarUart_TxBuffer[uart->xVarUart_TxRead];
            HAL_UART_Transmit_IT(uart->huarts, &uart->xVarUart_TxBuffer[uart->xVarUart_TxRead], 1);
        }

        uart->xVarUart_TxRead++;
        uart->xVarUart_TxRead %= SIZE_OF_TX_BUF;
    }

    if (source == SOURCE_INTERRUPT)
    {
        Uart_Judge_State(uart);
    }
}

void Uart_Receive(_ARISE_UART_OBJECT *uart)
{
    uint8_t Code;
    if (uart == NULL)
        return;
    Code = uart->r_data;
    if (Code < 0x20)
        return;
    else if (Code > 0x80)
        return;

    if (uart->xVarUart_RxWrite == uart->xVarUart_RxRead)
    {
        if(uart->huarts == auart[UART_USB].huarts)
        {
            SysFlag_Set(FLAG_UART_USB_RECEIVE);
            SysFlag_Set(FLAG_UART_USB_BUSY);
        }

    }
    else
    {
        uart->xVarUart_RxBuffer[uart->xVarUart_RxWrite++] = Code;
        uart->xVarUart_RxWrite %= SIZE_OF_RX_BUF;
        if (Code != UART_MARK_STOP)
            return;
        if(uart->huarts == auart[UART_USB].huarts)
        {
            Set_Timer_Flag(FLAG_UART_RECEIVED);
            UART_command_counter++;
        }
    }
}

void Uart_out_char(uint8_t uart_port_no, uint8_t Code)
{
	if(uart_port_no >= UART_MAX)
		return;

	_ARISE_UART_OBJECT *uart = &auart[uart_port_no];
    uint16_t Index = (uart->xVarUart_TxWrite + 1) % SIZE_OF_TX_BUF;

    if (Index != uart->xVarUart_TxRead)
    {
        uart->xVarUart_TxBuffer[uart->xVarUart_TxWrite] = Code;
        uart->xVarUart_TxWrite = Index;
    }

    if (uart->xVarUart_Tx_WorkingFlag == 0)
    {
        uart->xVarUart_Tx_WorkingFlag = 1;
        Uart_Transmit(uart,SOURCE_FILL_BUFFER);
    }
}

void Uart_Send_Text(uint8_t uart_port_no,uint8_t *buff)
{
	if(uart_port_no >= UART_MAX)
		return; 

	uint8_t	i = 0;
	while (buff[i] != '\0')
	{
		Uart_out_char(uart_port_no,buff[i++]);
	}
}

void Uart_Send_i32(uint8_t uart_port_no,uint8_t *buff, int32_t Value, uint8_t Length, int32_t Base)
{
	if(uart_port_no >= UART_MAX)
		return; 

	_ARISE_UART_OBJECT *uart = &auart[uart_port_no];
	uint8_t	i, Remainer, Count = 0;

	Uart_Send_Text(uart_port_no,buff);
	if (Value < 0)
	{
		Uart_out_char(uart_port_no,'-');
		Value = -Value;
	}
	while (1)
	{
		Remainer = (uint8_t) (Value % Base);
		if (Remainer >= 10)
			Remainer += 7;
		uart->xVarUart_Work[Count++] = Remainer + '0';
		Value /= Base;
		if (Value <= 0L)
			break;
	}
	for (i = Count; i < Length; i++)
		Uart_out_char(uart_port_no,'0');

	for (i = Count; i > 0; i--)
		Uart_out_char(uart_port_no,uart->xVarUart_Work[i - 1]);
}

void Uart_Send_Text_Num(uint8_t uart_port_no,uint8_t *buff,uint8_t num)
{
	if(uart_port_no >= UART_MAX)
		return;     
	uint8_t	i = 0;
	
	while(i<num)
	{
		Uart_out_char(uart_port_no,buff[i++]);
	}
}

void Uart_Send_Hex(uint8_t uart_port_no,uint8_t *buff, int32_t Value, uint8_t Length)
{
	Uart_Send_i32(uart_port_no,buff,Value,Length,16L);
}

void Uart_Send_Value(uint8_t uart_port_no,uint8_t *buff, int32_t Value, uint8_t Length)
{
	Uart_Send_i32(uart_port_no,buff,Value,Length,10L);
}

void	Uart_USB_Process( uint8_t* Command)
{
  	_ARISE_UART_OBJECT *uart = &auart[UART_USB];
	uint8_t	Code = 0;
	uint16_t	Index = -1;
	uint16_t	Source = uart->xVarUart_RxRead + 1;
	uint16_t	Target = uart->xVarUart_RxWrite - 1;

    if (Target <= Source)	
      Target += SIZE_OF_RX_BUF;

	for (; Source <= Target; Source++)
	{
		Code = uart->xVarUart_RxBuffer[Source % SIZE_OF_RX_BUF];
		
		if (Code == UART_MARK_START)		
            Index = 1;
		else if (Code == UART_MARK_STOP)
        {
            // SysFlag_Signal(SIGNAL_UART_USB, -1, FLAG_UART_USB_RECEIVE);   
            UART_command_counter--;
            if(UART_command_counter<0)  
                UART_command_counter = 0;       
            else if(UART_command_counter > 0)
                Set_Timer_Flag(FLAG_UART_RECEIVED);
        }	
		
		if (Index < 0)	
            continue;
			
		Command[Index++] = Code;
			
		if (Code == UART_MARK_STOP)		
            break;           
	}    

	if ((Index < 0)	|| (Code != UART_MARK_STOP))
	{
		uart->xVarUart_RxRead = Target % SIZE_OF_RX_BUF;
	}
	else
	{
		uart->xVarUart_RxRead = Source % SIZE_OF_RX_BUF;	
		Command[0] = Index;			
		SysFlag_Set(FLAG_UART_USB_COMMAND);
	}
}
