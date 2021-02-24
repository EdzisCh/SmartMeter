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
#include "tariffs.h"

extern I2C_HandleTypeDef hi2c2;
extern I2C_HandleTypeDef hi2c3;
extern QSPI_HandleTypeDef hqspi;
extern RTC_HandleTypeDef hrtc;
extern TIM_HandleTypeDef htim5;
extern UART_HandleTypeDef huart5;
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;

uint8_t tests_run( void );

//===================================================================================

//rtc tests
uint8_t test_rtc_init( void );
uint8_t test_rtc_get_timestamp( void );
uint8_t test_rtc_date_update( void );
uint8_t test_rtc_set_time_and_date( void );

//===================================================================================

//cs5490 tests
uint8_t test_cs5490_init( void );

//===================================================================================

//lcd tests
uint8_t test_lcd_init( void );

//===================================================================================

uint8_t tests_day_retrospective( void );
uint8_t tests_retrospective_last_address( void );

uint8_t tests_day_tariffs_retrospective( void );
uint8_t tests_tariffs_retrospective_last_address( void );

#endif
