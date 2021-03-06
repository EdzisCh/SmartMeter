/**
!Обработчик RS485
*/
#ifndef _RS485_H_
#define _RS485_H_

#include "stm32l4xx_hal.h"
#include "stdio.h"
#include "lcd.h"
#include "commands_handler.h"
#include "ring_buffer.h"

#define RX_BUFF_SIZE 80

extern UART_HandleTypeDef huart5;

void UART5_IRQHandler(void);
void rs485_rx_byte_handler(UART_HandleTypeDef* huart);
int rs485_send_byte( int byte );
uint8_t rs485_send_message( uint8_t *message, uint8_t size );
void rs485_start( void );

#endif
