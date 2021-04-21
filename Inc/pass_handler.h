#ifndef _PASS_HANDLER_
#define _PASS_HANDLER_

#include "stm32l4xx_hal.h"
#include "M24M01.h"

uint8_t pass_handler_is_first_pass_on( void );
uint8_t pass_handler_is_second_pass_on( void );
uint8_t pass_handler_is_any_pass_on( void );
void pass_handler_passwords_init( void );
uint8_t pass_handler_check_pass( uint32_t pass );
void pass_handler_set_first_pass( uint32_t pass );
void pass_handler_set_second_pass( uint32_t pass );
void pass_handler_set_pass_on( uint8_t number_of_pass );
void pass_handler_set_pass_off( void );
uint32_t pass_handler_get_first_pass( void );
uint32_t pass_handler_get_second_pass( void );

#endif