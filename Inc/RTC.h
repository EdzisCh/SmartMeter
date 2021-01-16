#ifndef _RTC_H_
#define _RTC_H_

#include "stm32l4xx_hal.h"

extern RTC_HandleTypeDef hrtc;

uint8_t rtc_get_timestamp( RTC_TimeTypeDef *time );

#endif
