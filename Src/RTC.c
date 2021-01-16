#include "RTC.h"

uint8_t rtc_get_timestamp( RTC_TimeTypeDef *time )
{
	HAL_RTC_GetTime(&hrtc, time, 24);
}