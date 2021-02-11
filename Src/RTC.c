#include "RTC.h"

RTC_TimeTypeDef current_time;
RTC_DateTypeDef current_date;

	/**
	!Установка первоночальной даты на 10:44:00 19.01.21
	
	*/
void rtc_set_init_dateTime( void )
{
	current_time.Hours = 0x16;
	current_time.Minutes = 0x57;
	current_time.Seconds = 0x0;
	current_time.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
	current_time.StoreOperation = RTC_STOREOPERATION_RESET;
	HAL_RTC_SetTime(&hrtc, &current_time, RTC_FORMAT_BCD);

	current_date.WeekDay = RTC_WEEKDAY_THURSDAY;
	current_date.Month = RTC_MONTH_FEBRUARY;
	current_date.Date = 0x04;
	current_date.Year = 0x21;

	HAL_RTC_SetDate(&hrtc, &current_date, RTC_FORMAT_BCD);
}

	/**
	!Функция возвращает текущее время и дату в качестве идентификатора
	в формате чч:мм:сс дд:мм:гг, при этом в байтах возвращаестя следующая
	последовательность 00 00 ЧЧ ММ СС ДД ММ ГГ, где 00 - старшие байты.
	Пример: 00 00 19 48 00 21 01 01

	*/
void rtc_get_timestamp( uint32_t *timestamp )
{
	HAL_RTC_GetTime(&hrtc, &current_time, RTC_FORMAT_BCD);
    HAL_RTC_GetDate(&hrtc, &current_date, RTC_FORMAT_BCD);
	
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
}

	/**
	!
	*/
void rtc_set_hours( uint8_t hours )
{
	HAL_RTC_GetTime(&hrtc, &current_time, RTC_FORMAT_BCD);
	current_time.Hours = hours;
	
	HAL_RTC_SetTime(&hrtc, &current_time, RTC_FORMAT_BCD);
}


void rtc_set_minutes( uint8_t minutes )
{
	HAL_RTC_GetTime(&hrtc, &current_time, RTC_FORMAT_BCD);
	current_time.Minutes = minutes;
	
	HAL_RTC_SetTime(&hrtc, &current_time, RTC_FORMAT_BCD);
}

void rtc_set_seconds( uint8_t seconds )
{
	HAL_RTC_GetTime(&hrtc, &current_time, RTC_FORMAT_BCD);
	current_time.Seconds = seconds;
	
	HAL_RTC_SetTime(&hrtc, &current_time, RTC_FORMAT_BCD);
}

void rtc_set_day( uint8_t day )
{
	HAL_RTC_GetDate(&hrtc, &current_date, RTC_FORMAT_BCD);
	current_date.Date = day;
	
	HAL_RTC_SetDate(&hrtc, &current_date, RTC_FORMAT_BCD);
}

void rtc_set_month( uint8_t month )
{
	HAL_RTC_GetDate(&hrtc, &current_date, RTC_FORMAT_BCD);
	current_date.Month = month;
	
	HAL_RTC_SetDate(&hrtc, &current_date, RTC_FORMAT_BCD);
}

void rtc_set_year( uint8_t year )
{
	HAL_RTC_GetDate(&hrtc, &current_date, RTC_FORMAT_BCD);
	current_date.Year = year;
	
	HAL_RTC_SetDate(&hrtc, &current_date, RTC_FORMAT_BCD);
}

void rtc_set_date_and_time( uint64_t dateTime )
{
	HAL_RTC_GetTime(&hrtc, &current_time, RTC_FORMAT_BCD);
    HAL_RTC_GetDate(&hrtc, &current_date, RTC_FORMAT_BCD);


}

void rtc_set_date( uint32_t date )
{
	
}

void rtc_set_time( uint32_t time )
{
}

uint8_t rtc_get_hours( void )
{
	HAL_RTC_GetTime(&hrtc, &current_time, RTC_FORMAT_BCD);
	
	return current_time.Hours;
}

uint8_t rtc_get_minutes( void )
{
	HAL_RTC_GetTime(&hrtc, &current_time, RTC_FORMAT_BCD);
	
	return current_time.Minutes;
}

uint8_t rtc_get_seconds( void )
{
	HAL_RTC_GetTime(&hrtc, &current_time, RTC_FORMAT_BCD);
	
	return current_time.Seconds;
}

uint8_t rtc_get_day( void )
{
	HAL_RTC_GetDate(&hrtc, &current_date, RTC_FORMAT_BCD);
	
	return current_date.Date;
}

uint8_t rtc_get_month( void )
{
	HAL_RTC_GetDate(&hrtc, &current_date, RTC_FORMAT_BCD);
	
	return current_date.Month;
}

uint8_t rtc_get_year( void )
{
	HAL_RTC_GetDate(&hrtc, &current_date, RTC_FORMAT_BCD);
	
	return current_date.Year;
}

uint64_t rtc_get_date_and_time( void )
{
}

uint32_t rtc_get_date( void )
{
}

uint32_t rtc_get_time( void )
{
}

