#ifndef _COMMANDS_HENDLER_H_
#define _COMMANDS_HENDLER_H_

#include "stm32l4xx_hal.h"
#include "string.h"
#include "ring_buffer.h"

extern UART_HandleTypeDef huart5;

typedef struct command
{
	uint8_t *command;
	//link to cmd
} command;

void execute_command( ring_buf *ring, uint8_t count );
uint8_t compare_strings(uint8_t str_1[], uint8_t str_2[]);
#endif
