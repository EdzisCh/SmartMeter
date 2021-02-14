#include "RTC.h"

//RTC_TimeTypeDef current_time;
//RTC_DateTypeDef current_date;

/**
!Установка первоночальной даты 
*/
uint8_t rtc_set_init_dateTime( void )
{
	RTC_TimeTypeDef current_time;
    RTC_DateTypeDef current_date;
	current_time.Hours = INITIAL_HOUR;
	current_time.Minutes = INITIAL_MINUTE;
	current_time.Seconds = INITIAL_SECUNDE;
	
	current_time.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
	current_time.StoreOperation = RTC_STOREOPERATION_SET;
	
	if(HAL_RTC_SetTime(&hrtc, &current_time, RTC_FORMAT_BCD) != HAL_OK)
	{
		return 0x01;
	}

	current_date.WeekDay = RTC_WEEKDAY_THURSDAY;
	current_date.Month = INITIAL_MONTH;
	current_date.Date = INITIAL_DAY;
	current_date.Year = INITIAL_YEAR;

	if(HAL_RTC_SetDate(&hrtc, &current_date, RTC_FORMAT_BCD) != HAL_OK)
	{
		return 0x02;
	}
	
	return 0x00;
}

/**
!Функция возвращает текущее время и дату в качестве идентификатора
в формате чч:мм:сс дд:мм:гг, при этом в байтах возвращаестя следующая
последовательность 00 00 ЧЧ ММ СС ДД ММ ГГ, где 00 - старшие байты.
Пример: 00 00 19 48 00 01 01 21
*/
uint8_t rtc_get_timestamp( uint32_t *timestamp )
{
	RTC_TimeTypeDef current_time;
	RTC_DateTypeDef current_date;
	if(HAL_RTC_GetTime(&hrtc, &current_time, RTC_FORMAT_BCD) != HAL_OK)
	{
		return 0x01;
	}
    if(HAL_RTC_GetDate(&hrtc, &current_date, RTC_FORMAT_BCD) != HAL_OK)
	{
		return 0x02;
	}
	
	uint32_t time = 0x00;
	
    time = (time + current_time.Hours) << 8;
    time = (time + current_time.Minutes) << 8;
    time = time + current_time.Seconds;
	
	timestamp[0] = time;
	time = 0x00;
	
    time = (time + current_date.Date) << 8;
    time = (time + current_date.Month) << 8;
    time = time + current_date.Year;

	timestamp[1] = time;
	
	return 0;
}

/**
!Функция проверяет измение даты и выдает число соответствующее 
определенной дате
0 -> нет изменений
1 -> новый день
2 -> новый месяц
3 -> новый год
*/
uint8_t rtc_date_update( uint32_t *timestamp )
{
	RTC_DateTypeDef current_date;
	HAL_RTC_GetDate(&hrtc, &current_date, RTC_FORMAT_BCD);
	
	uint8_t day = (timestamp[1] & 0x00FF0000) >> 16;
	uint8_t month = (timestamp[1] & 0x0000FF00) >> 8;
	uint8_t year = (timestamp[1] & 0x000000FF);
	
	if(current_date.Date != day)
	{
		return 1;
	} else if (current_date.Month != month)
	{
		return 2;
	} else if (current_date.Year != year)
	{
		return 3;
	}
	
	return 0;
}

/**
!
*/
uint8_t rtc_set_hours( uint8_t hours )
{
	RTC_TimeTypeDef current_time;
	if(HAL_RTC_GetTime(&hrtc, &current_time, RTC_FORMAT_BCD) != HAL_OK)
	{
		return 0x01;
	}
	current_time.Hours = hours;
	current_time.Seconds = 31;
	
	if(HAL_RTC_SetTime(&hrtc, &current_time, RTC_FORMAT_BCD) != HAL_OK)
	{
		return 0x02;
	}
	
	return 0;
}

uint8_t rtc_set_minutes( uint8_t minutes )
{
	RTC_TimeTypeDef current_time;
	if(HAL_RTC_GetTime(&hrtc, &current_time, RTC_FORMAT_BCD) != HAL_OK)
	{
		return 0x01;
	}
	current_time.Minutes = minutes;
	current_time.Seconds = 31;
	
	if(HAL_RTC_SetTime(&hrtc, &current_time, RTC_FORMAT_BCD) != HAL_OK)
	{
		return 0x02;
	}
	
	return 0;
}

uint8_t rtc_set_seconds( uint8_t seconds )
{
	RTC_TimeTypeDef current_time;
	if(HAL_RTC_GetTime(&hrtc, &current_time, RTC_FORMAT_BCD) != HAL_OK)
	{
		return 0x01;
	}
	current_time.Seconds = seconds;
	
	if(HAL_RTC_SetTime(&hrtc, &current_time, RTC_FORMAT_BCD) != HAL_OK)
	{
		return 0x02;
	}
	
	return 0;
}

uint8_t rtc_set_day( uint8_t day )
{
	RTC_DateTypeDef current_date;
	if(HAL_RTC_GetDate(&hrtc, &current_date, RTC_FORMAT_BCD) != HAL_OK)
	{
		return 0x01;
	}
	current_date.Date = day;
	
	if(HAL_RTC_SetDate(&hrtc, &current_date, RTC_FORMAT_BCD) != HAL_OK)
	{
		return 0x02;
	}
	
	return 0;
}

uint8_t rtc_set_month( uint8_t month )
{
	RTC_DateTypeDef current_date;
	if(HAL_RTC_GetDate(&hrtc, &current_date, RTC_FORMAT_BCD) != HAL_OK)
	{
		return 0x01;
	}
	current_date.Month = month;
	
	if(HAL_RTC_SetDate(&hrtc, &current_date, RTC_FORMAT_BCD) != HAL_OK)
	{
		return 0x02;
	}
	
	return 0;
}

uint8_t rtc_set_year( uint8_t year )
{
	RTC_DateTypeDef current_date;
	if(HAL_RTC_GetDate(&hrtc, &current_date, RTC_FORMAT_BCD) != HAL_OK)
	{
		return 0x01;
	}
	current_date.Year = year;
	
	if(HAL_RTC_SetDate(&hrtc, &current_date, RTC_FORMAT_BCD) != HAL_OK)
	{
		return 0x02;
	}
	
	return 0;
}

//TODO
uint8_t rtc_set_date_and_time( uint64_t dateTime )
{
	RTC_TimeTypeDef current_time;
	RTC_DateTypeDef current_date;
	HAL_RTC_GetTime(&hrtc, &current_time, RTC_FORMAT_BCD);
    HAL_RTC_GetDate(&hrtc, &current_date, RTC_FORMAT_BCD);

	return 0;
}

uint8_t rtc_set_date( uint32_t date )
{
	return 0;
}

uint8_t rtc_set_time( uint32_t time )
{
	return 0;
}

uint8_t rtc_get_hours( void )
{
	RTC_TimeTypeDef current_time;
	if(HAL_RTC_GetTime(&hrtc, &current_time, RTC_FORMAT_BCD) != HAL_OK)
	{
		return 0xFF;
	}

	return current_time.Hours;
}

uint8_t rtc_get_minutes( void )
{
	RTC_TimeTypeDef current_time;
	if(HAL_RTC_GetTime(&hrtc, &current_time, RTC_FORMAT_BCD) != HAL_OK)
	{
		return 0xFF;
	}

	return current_time.Minutes;
}

uint8_t rtc_get_seconds( void )
{
	RTC_TimeTypeDef current_time;
	if(HAL_RTC_GetTime(&hrtc, &current_time, RTC_FORMAT_BCD) != HAL_OK)
	{
		return 0xFF;
	}
	
	return current_time.Seconds;
}

uint8_t rtc_get_day( void )
{
	RTC_DateTypeDef current_date;
	if(HAL_RTC_GetDate(&hrtc, &current_date, RTC_FORMAT_BCD) != HAL_OK)
	{
		return 0xFF;
	}
	
	return current_date.Date;
}

uint8_t rtc_get_month( void )
{
	RTC_DateTypeDef current_date;
	if(HAL_RTC_GetDate(&hrtc, &current_date, RTC_FORMAT_BCD) != HAL_OK)
	{
		return 0xFF;
	}
	
	return current_date.Month;
}

uint8_t rtc_get_year( void )
{
	RTC_DateTypeDef current_date;
	if(HAL_RTC_GetDate(&hrtc, &current_date, RTC_FORMAT_BCD) != HAL_OK)
	{
		return 0xFF;
	}
	
	return current_date.Year;
}

uint32_t rtc_get_date( void )
{
	return 0;
}

uint32_t rtc_get_time( void )
{
	return 0;
}

