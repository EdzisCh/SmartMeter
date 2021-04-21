#include "event_handler.h"

event_log logs[EVENT_COUNT];

uint32_t current_address_of_event;

uint32_t count_of_all_notes_in_mem;

void event_handler_init_logs( void )
{
	m24m01_get_from_mem(50000, (uint8_t*)&count_of_all_notes_in_mem, 4);
	
	//берем из памяти текущий адрес журналов
	m24m01_get_from_mem(EVENT_CURRENT_ADDRESS_MEMORY_ADRESS, (uint8_t*)&current_address_of_event, 4);
	
	for(uint8_t i = 0; i < EVENT_COUNT; i++)
	{
		if(i < 4)
		{
			logs[i].max_count_of_notes = EVENT_MAX_NOTES_20;
		} else if( i >= 4 && i < 31) {
			logs[i].max_count_of_notes = EVENT_MAX_NOTES_12;
		} else if( i >= 31 && i <  42) {
			logs[i].max_count_of_notes = EVENT_MAX_NOTES_4;
		}
		
		logs[i].count_of_notes = 0;
	}
	
	
	for(uint8_t i = 0; i < EVENT_COUNT; i++)
	{
		logs[i].note_size = EVENT_NOTE_SIZE;	
	}
	
	//инициализация флагов инфо-поля
	for(uint8_t i = 0; i < EVENT_COUNT; i++)
	{
		if(i == 0){
			logs[i].percent_flag = 0;
			logs[i].seconds_flag = 1;
			logs[i].password_flag = 1;
			logs[i].sign_flag = 0;
			logs[i].port_flag = 0;
		} else if(i >= 15 && i <= 20)
		{
			logs[i].percent_flag = 1;
			logs[i].seconds_flag = 0;
			logs[i].password_flag = 0;
			logs[i].sign_flag = 0;
			logs[i].port_flag = 0;
		} else if((i >= 21 && i <= 26) || (i >= 28 && i <= 30) 
									   || i == 9 || i == 5 
									   || i == 7 || i == 38 
									   || i == 40)
		{
			logs[i].percent_flag = 0;
			logs[i].seconds_flag = 0;
			logs[i].password_flag = 0;
			logs[i].sign_flag = 0;
			logs[i].port_flag = 0;
		} else if(i == 4 || i == 6)
		{
			logs[i].percent_flag = 0;
			logs[i].seconds_flag = 1;
			logs[i].password_flag = 0;
			logs[i].sign_flag = 0;
			logs[i].port_flag = 0;
		} else if(i == 33 || i == 2 || i == 3 || i == 8 || (i >= 10 && i <= 13) || i == 37)
		{
			logs[i].percent_flag = 0;
			logs[i].seconds_flag = 0;
			logs[i].password_flag = 1;
			logs[i].sign_flag = 1;
			logs[i].port_flag = 1;
		} else if(i == 32 || (i >= 34 && i <=36) || i == 14)
		{
			logs[i].percent_flag = 0;
			logs[i].seconds_flag = 0;
			logs[i].password_flag = 1;
			logs[i].sign_flag = 0;
			logs[i].port_flag = 1;
		} else if(i == 41 || i == 3 || i == 27 || i == 31 || i == 39)
		{
			logs[i].percent_flag = 0;
			logs[i].seconds_flag = 0;
			logs[i].password_flag = 0;
			logs[i].sign_flag = 1;
			logs[i].port_flag = 0;
		}
	}
}

void event_handler_make_note( uint8_t event, info_field s_info )
{
	if(event >= EVENT_COUNT) return;
	if(logs[event].count_of_notes > logs[event].max_count_of_notes) return;

	
	uint32_t address = current_address_of_event;

	uint32_t time = get_seconds_from_2000();
	
	//make info field
	uint8_t info = event_handler_make_info_field(event, s_info);
	
	m24m01_save_to_mem(address, (uint8_t*)&time, 4);
	address += 4;
	m24m01_save_to_mem(address, &info, 1);
	address += 1;
	m24m01_save_to_mem(address, (uint8_t*)&time, 4);
	address += 4;
	m24m01_save_to_mem(address, &event, 1);
	address += 1;
	
	current_address_of_event = address;
	m24m01_save_to_mem(EVENT_CURRENT_ADDRESS_MEMORY_ADRESS, (uint8_t*)&current_address_of_event, 4);
	
	logs[event].count_of_notes++;
	count_of_all_notes_in_mem++;
	m24m01_save_to_mem(50000, (uint8_t*)&count_of_all_notes_in_mem, 4);
}

uint8_t event_handler_make_info_field( uint8_t event, info_field info )
{
	if( logs[event].seconds_flag == 1)
	{
		return info.seconds;
	}
	
	if ( logs[event].percent_flag  == 1)
	{
		return info.percent;
	}

	uint8_t info_output = 0;
		
	if (logs[event].port_flag == 1 ) 
	{
		info_output += info.port;
	}
	if(logs[event].password_flag == 1 )
	{
		info_output += (info.password << 2);
	}
	if(logs[event].sign_flag == 1)
	{
		info_output += (info.sign << 3);
	} 
	
	return info_output;
}

//===================================================================================

uint32_t get_seconds_from_2000()
{
	uint32_t timestamp[2];
	rtc_get_timestamp(timestamp);
	
	uint8_t hour = (timestamp[0] & 0x00FF0000) >> 16;
	hour = from_BCD_format(hour);
	uint8_t minute = (timestamp[0] & 0x0000FF00) >> 8;
	minute = from_BCD_format(minute);
	uint8_t second = (timestamp[0] & 0x000000FF);
	second = from_BCD_format(second);
	
	uint8_t days = (timestamp[1] & 0x00FF0000) >> 16;
	days = from_BCD_format(days);
	uint8_t month = (timestamp[1] & 0x0000FF00) >> 8;
	month = from_BCD_format(month);
	uint8_t year = (timestamp[1] & 0x000000FF);
	year = from_BCD_format(year);
	
	uint32_t seconds_from_midnight = (hour * 60 * 60) + (minute * 60) + second;
	
	uint8_t count_of_leap_years = (year / 4) + 1;
	uint32_t seconds_from_2000 = ((year - count_of_leap_years) * 31536000) + (count_of_leap_years * 31622400);
	
	uint32_t days_from_current_year = 0;
	for(uint8_t i = 1; i < month; i++)
	{
		if(i % 2 != 0)
		{
			days_from_current_year += 31;
		} else if( i == 2 && year % 4 == 0 ) {
			days_from_current_year += 29;
		} else if( i == 2 && year % 4 != 0 ) {
			days_from_current_year += 28;
		} else {
			days_from_current_year += 30;
		}
	}
	days_from_current_year += days - 1;
	seconds_from_2000 += (days_from_current_year * 24 * 3600);
	seconds_from_2000 += seconds_from_midnight;
	
	return seconds_from_2000;
}

uint8_t from_BCD_format( uint32_t val )
{
	if(val > 0x9 && val < 0x20)
	{
		val -= 6;
	} else if (val >= 0x20 && val < 0x30)
	{
		val -= 12;
	} else if (val >= 0x30 && val < 0x40)
	{
		val -= 18;
	} else if (val >= 0x40 && val < 0x50)
	{
		val -= 24;
	} else if(val >= 0x50 && val < 0x60)
	{
		val -= 30;
	}
	
	return val;
}

/**
! Включаем звук (сигнал 4кГц). После вызова необходимо вызывать
выключение!
*/
void event_handler_beep_on( void )
{
	uint16_t value = 0;
	int8_t step = 0;
	
	if( value == 0 ) step = 1;
	if( value == 2 ) step = 0;
	
	value += step;
	TIM5->CCR4 = value;
}

/**
! Выключение звука.
*/
void event_handler_beep_off( void )
{

	TIM5->CCR4 = 0;
}
