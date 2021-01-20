	/**
	!Обработчик RS485
	
	*/
#ifndef _RS485_H_
#define _RS485_H_

#include "stm32l4xx_hal.h"

extern UART_HandleTypeDef huart5;

void rs485_send_byte( uint8_t byte );
void rs485_send_message( uint8_t *message, uint8_t size );
void rs485_get_message( uint8_t *message, uint8_t size );

#endif
