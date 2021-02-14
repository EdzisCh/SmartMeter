#ifndef _RTC_H_
#define _RTC_H_

#include "stm32l4xx_hal.h"
#include "stdio.h"

//Ничальная дата и время в BCD формате
#define INITIAL_HOUR 0x10
#define INITIAL_MINUTE 0x10
#define INITIAL_SECUNDE 0x00
#define INITIAL_DAY 0x12
#define INITIAL_MONTH 0x02
#define INITIAL_YEAR 0x21

extern RTC_HandleTypeDef hrtc;
//extern RTC_TimeTypeDef current_time;
//extern RTC_DateTypeDef current_date;

uint8_t rtc_set_init_dateTime( void );

uint8_t rtc_get_timestamp( uint32_t *timestamp );
uint8_t rtc_date_update( uint32_t *timestamp );

uint8_t rtc_set_hours( uint8_t hours );
uint8_t rtc_set_minutes( uint8_t minutes );
uint8_t rtc_set_seconds( uint8_t seconds );
uint8_t rtc_set_day( uint8_t day );
uint8_t rtc_set_month( uint8_t month );
uint8_t rtc_set_year( uint8_t year );
uint8_t rtc_set_date_and_time( uint64_t dateTime );
uint8_t rtc_set_date( uint32_t date );
uint8_t rtc_set_time( uint32_t time );

uint8_t rtc_get_hours( void );
uint8_t rtc_get_minutes( void );
uint8_t rtc_get_seconds( void );
uint8_t rtc_get_day( void );
uint8_t rtc_get_month( void );
uint8_t rtc_get_year( void );
uint32_t rtc_get_date( void );
uint32_t rtc_get_time( void );
#endif
