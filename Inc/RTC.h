#ifndef _RTC_H_
#define _RTC_H_

#include "stm32l4xx_hal.h"

extern RTC_HandleTypeDef hrtc;

uint64_t rtc_get_timestamp( void );

void rtc_set_time( void );
void rtc_set_date( void );

#endif
