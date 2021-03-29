#ifndef _COMMANDS_HANDLER_H_
#define _COMMANDS_HANDLER_H_

#include "stm32l4xx_hal.h"
#include "string.h"
#include "ring_buffer.h"
#include "RTC.h"
#include "mem_handler.h"

//Максимальное количество команд 
#define MAX_CMD_COUNT 15

extern UART_HandleTypeDef huart5;
extern data current_data;
extern total_energy_register TER;

typedef void (*cmd_callback_ptr) ( uint32_t );

/*
!Определение команды. 
command - команда-строка
callback_func - ссылка на функцию с аргументом размерности uint32_t
*/
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
uint8_t cmd_handler_get_from_BCD_format( uint32_t val );

//===================================================================================

void connect_callback( uint32_t arg );
void get_time_callback( uint32_t arg );
void get_data_callback( uint32_t arg );
void set_hour_callback( uint32_t arg );
void set_minute_callback( uint32_t arg );
void set_day_callback( uint32_t arg );
void set_month_callback( uint32_t arg );
void set_year_callback( uint32_t arg );
void set_time_callback( uint32_t arg );
void set_date_callback( uint32_t arg );

#endif
