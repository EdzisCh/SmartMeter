#ifndef _RTC_H_
#define _RTC_H_

#include "stm32l4xx_hal.h"

extern RTC_HandleTypeDef hrtc;

static RTC_TimeTypeDef current_time;
static RTC_DateTypeDef current_date;

void rtc_set_init_dateTime( void );

uint64_t rtc_get_timestamp( void );

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
