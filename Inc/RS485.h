/**
!Обработчик RS485

*/
#ifndef _RS485_H_
#define _RS485_H_

#include "stm32l4xx_hal.h"
#include "stdio.h"
#include "string.h"

extern UART_HandleTypeDef huart5;

uint8_t rs485_proceed( void );
int rs485_send_byte( uint8_t byte );
uint8_t rs485_send_message( uint8_t *message, uint8_t size );
uint8_t rs485_get_message( uint8_t *message );
uint8_t rs485_is_received( void );

#endif
