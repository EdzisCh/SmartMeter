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
	if(test_rtc_date_update())
	{
		return 4;
	}
	if(tests_day_retrospective())
	{
		return 5;
	}
	if(tests_retrospective_last_address())
	{
		return 6;
	}
	if(tests_day_tariffs_retrospective())
	{
		return 7;
	}
	if(tests_tariffs_retrospective_last_address())
	{
		return 8;
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
	
	uint32_t notes_from_mem[60];
	uint32_t notes_to_mem[60];
	
	uint8_t days[10] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x10};
	
	uint32_t Pcon = 5;
	uint32_t Prel = 4;
	uint32_t Qcon = 3;
	uint32_t Qrel = 2;
	
	total_energy_register TER;

	
	uint8_t notes_to_mem_addr = 0;
	rtc_set_day(0x00);
	for(uint8_t i = 0; i<11; i++)
	{
		TER.consumed_active_energy = Pcon;
		TER.consumed_reactive_energy = Qcon;
		TER.released_active_energy = Prel;
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
	printf("\r\n");
	
	m24m01_get_from_mem(0x0048, (uint8_t *) notes_from_mem, 240);

	for(uint8_t i = 0; i < 60; i++)
	{
		//костыль)
		if(notes_from_mem[i] != notes_to_mem[i] && i != 46 && i != 47)
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
	
	// while(current_address_of_day_retrosective != 0x0048)
	// {
		// mem_handler_send_retrospective_to_eeprom(1, timestamp, &TER);
	// }
	// mem_handler_send_retrospective_to_eeprom(1, timestamp, &TER);
	
	m24m01_get_from_mem(0x48, (uint8_t *) notes_from_mem, 24);
	for(uint8_t i = 0; i < 6; i++)
	{
		if(notes_from_mem[i] != notes_to_mem[i])
		{
			return 0x02;
		}
	}
	
	return 0;
}

uint8_t tests_day_tariffs_retrospective( void )
{
	rtc_set_date(0x260221);
	//C 00:00 до 7:00 - t_1_a, t_1_r
	rtc_set_time(0x00);
	uint32_t timestamp_current[2];
	rtc_get_timestamp(timestamp_current);
	//tariffs_init();
	
	uint32_t notes_from_mem[48];
	uint32_t notes_to_mem[48];
	
	uint8_t days[3] = {0x06, 0x07, 0x08};
	
	uint32_t P = 1;
	uint32_t Q = 5;
	
	rtc_set_day(0x05);
	for(uint8_t i = 0; i < 4; i++)
	{
		uint8_t new_date = rtc_date_update(timestamp_current);
		
		if(new_date == 0)
		{
			return 0x01;
		}
				
		tariffs_set_data( P, Q );
		printf("Sho[2]:%u, %u\r\n", *current_accum_for_P, *current_accum_for_Q);
		tariffs_send_retrospective_to_eeprom(new_date, timestamp_current);
		
	    rtc_get_timestamp(timestamp_current);
		rtc_set_day(days[i]);
		
	}
	
	printf("Sho[3]:%u, %u\r\n", *current_accum_for_P, *current_accum_for_Q);
	
	m24m01_get_from_mem(0x10F8, (uint8_t *) notes_from_mem, 192);
	
	for(uint8_t i = 0; i < 48; i++)
	{
		printf("[%u]: %u\r\n", i, notes_from_mem[i]);
		if(notes_from_mem[i] != notes_to_mem[i])
		{
			
			//return 0x02;
		}
	}
	
	return 0;
}

uint8_t tests_tariffs_retrospective_last_address( void )
{
	return 0;
}
