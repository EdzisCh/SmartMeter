#ifndef _TESTS_H_
#define _TESTS_H_

#include "stm32l4xx_hal.h"
#include "lcd.h"
#include "RTC.h"
#include "CS5490.h"
#include "event_handler.h"
#include "mem_handler.h"
#include "RS485.h"
#include "stdio.h"
#include "M24M01.h"
#include "S25FL.h"

uint8_t tests_run( void );

//===================================================================================

//rtc tests
uint8_t test_rtc_init( void );
uint8_t test_rtc_get_timestamp( void );
uint8_t test_rtc_date_update( void );
uint8_t tests_rtc_set_hours( void );
uint8_t tests_rtc_set_day( void );

//===================================================================================

uint8_t tests_retrospective( void );

#endif
