#ifndef _COMMANDS_HANDLER_H_
#define _COMMANDS_HANDLER_H_

#include "string.h"
#include "ring_buffer.h"
#include "RTC.h"
#include "mem_handler.h"
#include "CS5490.h"
#include "lcd.h"
#include "pass_handler.h"
#include "M24M01.h"

//Максимальное количество команд 
#define MAX_CMD_COUNT 20

extern UART_HandleTypeDef huart5;
extern data current_data_1;
extern data current_data_2;
extern data current_data_3;
extern total_energy_register TER;
extern CS5490 chip_L1;
extern CS5490 chip_L2;
extern CS5490 chip_L3;
extern uint8_t phase_1_enable;
extern uint8_t phase_2_enable;
extern uint8_t phase_3_enable;
extern uint32_t current_address_of_day_retrosective;
extern uint32_t current_address_of_month_retrosective;
extern uint32_t current_address_of_year_retrosective;

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
void end_connection_callback( uint32_t arg );
void set_first_pass_callback( uint32_t arg );
void set_second_pass_callback( uint32_t arg );
void reset_meters_callback( uint32_t arg );

void calibrate_callback( uint32_t arg );

void get_time_callback( uint32_t arg );
void get_data_callback( uint32_t arg );
void set_time_callback( uint32_t arg );
void set_date_callback( uint32_t arg );
void set_daylight_callback( uint32_t arg );
void change_daylight_callback( uint32_t arg );
void get_event_notes_callback( uint32_t arg );
void get_ter_data_callback( uint32_t arg );

#endif
