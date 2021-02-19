/**
!Обработчик RS485

*/
#ifndef _RS485_H_
#define _RS485_H_

#include "stm32l4xx_hal.h"
#include "stdio.h"
#include "lcd.h"
#include "commands_hendler.h"
#include "ring_buffer.h"


#define RX_BUFF_SIZE 80

extern UART_HandleTypeDef huart5;

void UART5_IRQHandler(void);
void UART_byte_proceed(UART_HandleTypeDef* huart);
int rs485_send_byte( uint8_t byte );
uint8_t rs485_send_message( uint8_t *message, uint8_t size );
void rs485_start( void );

#endif
