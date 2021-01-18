#include "RTC.h"

	/**
	!Функция возвращает текущее время и дату в качестве идентификатора
	в формате чч:мм:сс дд:мм:гг, при этом в байтах возвращаестя следующая
	последовательность 00 ЧЧ ММ СС 00 ДД ММ ГГ, где 00 - разделители.
	Пример: 00 19 48 00 00 21 01 01

	*/
uint64_t rtc_get_timestamp( void )
{
	RTC_TimeTypeDef time;
	RTC_DateTypeDef date;
	
	HAL_RTC_GetTime(&hrtc, &time, 24);
	
	HAL_RTC_GetTime(&hrtc, &time, RTC_FORMAT_BCD);
    HAL_RTC_GetDate(&hrtc, &date, RTC_FORMAT_BCD);
	
    uint64_t timestamp = 0x00;
  
    timestamp = (timestamp + time.Hours) << 8;
    timestamp = (timestamp + time.Minutes) << 8;
    timestamp = (timestamp + time.Seconds) << 8;
	
	//разделение времени и даты
	timestamp = (timestamp + 0x00) << 8;
	
    timestamp = (timestamp + date.Date) << 8;
    timestamp = (timestamp + date.Month) << 8;
    timestamp = timestamp + date.Year;
  

    return timestamp;
}