#ifndef _COMMANDS_HENDLER_H_
#define _COMMANDS_HENDLER_H_

#include "stm32l4xx_hal.h"
#include "string.h"
#include "ring_buffer.h"
#include "RTC.h"

#define MAX_CMD_COUNT 10

extern UART_HandleTypeDef huart5;

typedef void (*cmd_callback_ptr) ( uint32_t );

typedef struct command
{
	const char *command;
	cmd_callback_ptr callback_func;
} command_defenition;

void cmd_handler_commands_init( void );
void cmd_handler_execute_command( ring_buf *ring, uint8_t count );
uint8_t cmd_handler_get_command(uint8_t cmd[], uint32_t arg);
uint8_t cmd_handler_compare_strings(uint8_t str_1[], const char* str_2, uint32_t *arg);
uint8_t cmd_handler_to_BCD_format( uint8_t );
//test
void led_on_act_1( uint32_t arg );
void led_on_react_1( uint32_t arg );
void led_off_1( uint32_t arg );
void set_i_1( uint32_t arg );

void set_hour_callback( uint32_t arg );
void set_time_callback( uint32_t arg );
void set_date_callback( uint32_t arg );
#endif
