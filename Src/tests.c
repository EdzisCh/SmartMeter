#include "tests.h"

/**
!Стартовая точка всех тестов, при успешнов прохождении которых
выводиться 0
*/
uint8_t tests_run( void )
{
	uint8_t output = 0;
	
	output |= test_rtc_init();
	output |= test_rtc_get_timestamp();
	output |= tests_retrospective();
	
	return output;
}

//===================================================================================

uint8_t test_rtc_init( void )
{
	return rtc_set_init_dateTime();
}

/**
!Проверка отметки времени в почти самом начале работы
*/
uint8_t test_rtc_get_timestamp( void )
{
	uint32_t timestamp[2];
	
	if(rtc_get_timestamp(timestamp) != 0)
	{
		return 0x01;
	}
	
	uint8_t hour = (timestamp[0] & 0x00FF0000) >> 16;
	uint8_t minute = (timestamp[0] & 0x0000FF00) >> 8;
	uint8_t day = (timestamp[1] & 0x00FF0000) >> 16;
	uint8_t month = (timestamp[1] & 0x0000FF00) >> 8;
	uint8_t year = (timestamp[1] & 0x000000FF);
	
	if(hour != INITIAL_HOUR || minute != INITIAL_MINUTE)
	{
		return 0x02;
	} else if (day != INITIAL_DAY || month != INITIAL_MONTH || year != INITIAL_YEAR)
	{
		return 0x03;
	}
	
	return 0;
}

uint8_t tests_rtc_set_hours( void )
{
}

uint8_t tests_rtc_set_day( void )
{
}

uint8_t test_rtc_date_update( void )
{
}

//===================================================================================

uint8_t tests_retrospective( void )
{
	uint32_t timestamp_current[2];
	rtc_get_timestamp(timestamp_current);
	
	uint32_t Pcon = 0;
	uint32_t Prel = 0;
	uint32_t Qcon = 0;
	uint32_t Qrel = 0;
	
	total_energy_register TER;

	
	uint8_t day = 0x00;
	rtc_set_day(day);
	for(uint8_t i = 1; i<10; i++)
	{
		TER.consumed_active_energy = Pcon;
		TER.consumed_reactive_energy = Prel;
		TER.released_active_energy = Qcon;
		TER.released_reactive_energy = Qrel;
		
		uint8_t new_date = rtc_date_update(timestamp_current);
		if(new_date == 0)
		{
			return 0x01;
		}

		mem_handler_send_retrospective_to_eeprom(new_date, timestamp_current, &TER);
		
		day += i;
		rtc_set_day(day);
		Pcon++;
		Prel++;
		Qcon++;
		Qrel++;
	}
	
	uint32_t notes_from_mem[48];
	m24m01_get_from_mem(0x00, (uint8_t *) notes_from_mem, 48);
	
	for(uint8_t i = 0; i < 48; i++)
	{
		
	}
	
	return 0;
}