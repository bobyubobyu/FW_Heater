/*
 * SysUart_Answer.c
 *
 *  Created on: Nov 21, 2025
 *      Author: Bob Yu
 */

#include "main.h"

uint8_t UartAnswer_Ack(uint8_t uart_port_no,uint8_t Status, int16_t Index)
{
	if(uart_port_no >= UART_MAX)
		return false;
    if (Status)
    {
        Uart_Send_Value(uart_port_no, (uint8_t *)"*AY:", Index, 2);
        Uart_Send_Text(uart_port_no, (uint8_t *)":#%!\r\n");
    }
    else
    {
        Uart_Send_Value(uart_port_no, (uint8_t *)"*AN:", Index, 2);
        Uart_Send_Text(uart_port_no, (uint8_t *)":#%!\r\n");
    }

    return Status;
}

void UartAnswer_System_Info(uint8_t uart_port_no)
{
	if(uart_port_no >= UART_MAX)
		return;
	Uart_Send_Text(uart_port_no,(uint8_t *)"*SYS");
	Uart_Send_Value(uart_port_no,(uint8_t *)":", HW_VERSION, 0);
	Uart_Send_Value(uart_port_no,(uint8_t *)".", FW_VERSION_MAJOR, 0);
	Uart_Send_Value(uart_port_no,(uint8_t *)".", FW_VERSION_MINOR, 0);
	if(FW_VERSION_BUILD)
        Uart_Send_Value(uart_port_no,(uint8_t *)".", FW_VERSION_BUILD, 1);

	Uart_Send_Text(uart_port_no,(uint8_t *)":#%!\r\n");
}

void Send_LOG(uint8_t uart_port_no)
{
	if(uart_port_no >= UART_MAX)
		return;

	Uart_Send_Text(uart_port_no,(uint8_t *)"*00LOG");
	Uart_Send_Value(uart_port_no,(uint8_t *)":", read_ADC_value(ADC_HEATER_1), 4);	
	Uart_Send_Value(uart_port_no,(uint8_t *)",", read_ADC_value(ADC_HEATER_2), 4);	
	Uart_Send_Value(uart_port_no,(uint8_t *)",", read_ADC_value(ADC_PT1000_1), 4);	
	Uart_Send_Value(uart_port_no,(uint8_t *)",", read_ADC_value(ADC_PT1000_2), 4);			
	Uart_Send_Value(uart_port_no,(uint8_t *)",", read_ADC_value(ADC_PT1000_3), 4);	
	Uart_Send_Value(uart_port_no,(uint8_t *)",", read_ADC_value(ADC_PT1000_4), 4);
	Uart_Send_Value(uart_port_no,(uint8_t *)",", read_ADC_value(ADC_PT1000_5), 4);	
	Uart_Send_Value(uart_port_no,(uint8_t *)",", read_ADC_value(ADC_PT1000_6), 4);	
	Uart_Send_Value(uart_port_no,(uint8_t *)",", read_sensor_status(), 4);
	Uart_Send_Text(uart_port_no,(uint8_t *)":#%!\r\n");
}

void Send_EEPROM(uint8_t uart_port_no)
{
	if(uart_port_no >= UART_MAX)
		return;	
	Uart_Send_Text(uart_port_no,(uint8_t *)"*00EEP");
	Uart_Send_Value(uart_port_no,(uint8_t *)":", read_eeprom_data_test0(0), 1);	
	Uart_Send_Value(uart_port_no,(uint8_t *)",", read_eeprom_data_test0(1), 1);	
	Uart_Send_Value(uart_port_no,(uint8_t *)",", read_eeprom_data_test0(2), 1);	
	Uart_Send_Value(uart_port_no,(uint8_t *)",", read_eeprom_data_test0(3), 1);
	Uart_Send_Value(uart_port_no,(uint8_t *)",", read_eeprom_data_test0(4), 1);	
	Uart_Send_Value(uart_port_no,(uint8_t *)",", read_eeprom_data_test0(5), 1);	
	Uart_Send_Value(uart_port_no,(uint8_t *)",", read_eeprom_data_test0(6), 1);	
	Uart_Send_Value(uart_port_no,(uint8_t *)",", read_eeprom_data_test0(7), 1);	
	Uart_Send_Value(uart_port_no,(uint8_t *)",", read_eeprom_data_test0(8), 1);	
	Uart_Send_Value(uart_port_no,(uint8_t *)",", read_eeprom_data_test0(9), 1);		
	Uart_Send_Text(uart_port_no,(uint8_t *)":#%!\r\n");
}
