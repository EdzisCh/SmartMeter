#include "tests.h"

/**
!Стартовая точка всех тестов, при успешнов прохождении которых
выводиться 0
*/
uint8_t tests_run( void )
{
	if(test_rtc_init())
	{
		return 1;
	}

	if(test_rtc_get_timestamp())
	{
		return 2;
	}
	if(test_rtc_set_time_and_date())
	{
		return 3;
	}
	if(test_rtc_set_day())
	{
		return 4;
	}
	if(test_rtc_date_update())
	{
		return 5;
	}
	if(tests_day_retrospective())
	{
		return 6;
	}
	if(tests_retrospective_last_address())
	{
		return 7;
	}
	
	return 0;
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

uint8_t test_rtc_set_time_and_date( void )
{
	uint32_t date_time[2] = { 0x00141500, 0x00211221 };
	rtc_set_date_and_time(date_time);
	
	uint32_t check_time = rtc_get_time();
	uint32_t check_date = rtc_get_date();
	
	if(check_date != date_time[1])
	{
		return 0x01;
	}
	
	check_time &= 0x00FFFF00;
	if(check_time != date_time[0])
	{
		return 0x02;
	}
	
	return 0;
}

uint8_t test_rtc_set_day( void )
{
	uint8_t day = 0x12;
	rtc_set_day(day);
	
	uint8_t check_day = rtc_get_day();
	
	if(check_day != day)
	{
		return 0x01;
	}
	
	return 0;
}

//todo
uint8_t test_rtc_date_update( void )
{
	rtc_set_day(0x00);
	rtc_set_month(0x00);
	rtc_set_year(0x00);
	
	uint32_t timestamp[2];

	rtc_get_timestamp(timestamp);
	rtc_set_day(0x01);
	uint8_t update = rtc_date_update(timestamp);
	if(update != 1)
	{
		return 0x02;
	}

	rtc_get_timestamp(timestamp);
	rtc_set_month(0x01);
	update = rtc_date_update(timestamp);
	if(update != 2)
	{
		return 0x03;
	}
	
	rtc_get_timestamp(timestamp);
	rtc_set_year(0x01);
	update = rtc_date_update(timestamp);
	if(update != 3)
	{
		return 0x04;
	}
	
	return 0;
}

//===================================================================================

//cs5490 tests
uint8_t test_cs5490_init( void )
{
	
	return 0;
}

//===================================================================================

//lcd tests
uint8_t test_lcd_init( void )
{
	return 0;
}

//===================================================================================

/**
!Тест дневной ретроспективы для РОН. 
*/
uint8_t tests_day_retrospective( void )
{
	uint32_t timestamp_current[2];
	rtc_get_timestamp(timestamp_current);
	
	uint32_t notes_from_mem[48];
	uint32_t notes_to_mem[48];
	
	uint8_t days[10] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x10};
	
	uint32_t Pcon = 0;
	uint32_t Prel = 0;
	uint32_t Qcon = 0;
	uint32_t Qrel = 0;
	
	total_energy_register TER;

	
	uint8_t notes_to_mem_addr = 0;
	rtc_set_day(0x00);
	for(uint8_t i = 1; i<10; i++)
	{
		TER.consumed_active_energy = Pcon;
		TER.consumed_reactive_energy = Prel;
		TER.released_active_energy = Qcon;
		TER.released_reactive_energy = Qrel;
		
		notes_to_mem[notes_to_mem_addr++] = timestamp_current[0];
		notes_to_mem[notes_to_mem_addr++] = timestamp_current[1];
		notes_to_mem[notes_to_mem_addr++] = Pcon;
		notes_to_mem[notes_to_mem_addr++] = Qcon;
		notes_to_mem[notes_to_mem_addr++] = Prel;
		notes_to_mem[notes_to_mem_addr++] = Qrel;
		
		uint8_t new_date = rtc_date_update(timestamp_current);
		if(new_date == 0)
		{
			return 0x01;
		}

		mem_handler_send_retrospective_to_eeprom(new_date, timestamp_current, &TER);
		
	    rtc_get_timestamp(timestamp_current);
		rtc_set_day(days[i]);
		
		Pcon++;
		Prel++;
		Qcon++;
		Qrel++;
	}
	
	m24m01_get_from_mem(0x00, (uint8_t *) notes_from_mem, 192);
	
	for(uint8_t i = 0; i < 48; i++)
	{
		if(notes_from_mem[i] != notes_to_mem[i])
		{
			return 0x02;
		}
	}
	
	return 0;
}

/**
!Тест на проверку заполняемости для дневной ретроспективы. Если
при выполнении не возращается в начало памяти - ошибка
*/
uint8_t tests_retrospective_last_address( void )
{
	uint32_t notes_from_mem[6];
	uint32_t notes_to_mem[6];
	total_energy_register TER;
	TER.consumed_active_energy = 13;
	TER.released_active_energy = 14;
	TER.consumed_reactive_energy = 15;
	TER.released_reactive_energy = 16;
	
	uint32_t timestamp[2];
	rtc_get_timestamp(timestamp);

	notes_to_mem[0] = timestamp[0];
	notes_to_mem[1] = timestamp[1];
	notes_to_mem[2] = TER.consumed_active_energy;
	notes_to_mem[3] = TER.consumed_reactive_energy;
	notes_to_mem[4] = TER.released_active_energy;
	notes_to_mem[5] = TER.released_reactive_energy; 
	
	while(current_address_of_day_retrosective != 0)
	{
		mem_handler_send_retrospective_to_eeprom(1, timestamp, &TER);
	}
	mem_handler_send_retrospective_to_eeprom(1, timestamp, &TER);
	
	m24m01_get_from_mem(0x00, (uint8_t *) notes_from_mem, 24);
	for(uint8_t i = 0; i < 6; i++)
	{
		if(notes_from_mem[i] != notes_to_mem[i])
		{
			return 0x02;
		}
	}
	
	return 0;
}
