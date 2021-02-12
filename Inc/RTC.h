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

uint8_t rtc_set_init_dateTime( void );

uint8_t rtc_get_timestamp( uint32_t *timestamp );
uint8_t rtc_date_update( uint32_t *timestamp );

void rtc_set_hours( uint8_t hours );
void rtc_set_minutes( uint8_t minutes );
void rtc_set_seconds( uint8_t seconds );
void rtc_set_day( uint8_t day );
void rtc_set_month( uint8_t month );
void rtc_set_year( uint8_t year );
void rtc_set_date_and_time( uint64_t dateTime );
void rtc_set_date( uint32_t date );
void rtc_set_time( uint32_t time );

uint8_t rtc_get_hours( void );
uint8_t rtc_get_minutes( void );
uint8_t rtc_get_seconds( void );
uint8_t rtc_get_day( void );
uint8_t rtc_get_month( void );
uint8_t rtc_get_year( void );
uint64_t rtc_get_date_and_time( void );
uint32_t rtc_get_date( void );
uint32_t rtc_get_time( void );
#endif
