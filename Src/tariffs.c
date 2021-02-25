#include "tariffs.h"

tariffs_accum tariffs_accums; 

//Расписание
shedule_item shedule[48];
//Тарифная програма, номер програмы = индекс
daily_program daily_programs[32];
month_program month_programs[12];

tariff_plan first_plan;
tariff_plan second_plan;

tariff_plan* current_plan;
uint32_t *current_accum_for_P;
uint32_t *current_accum_for_Q;

/*
! Инициализация первоначальная 
*/
void tariffs_init( void )
{
	//Инициализация накопителей
	tariffs_accums.t_1_a = 0;
	tariffs_accums.t_1_r = 0;
	tariffs_accums.t_2_a = 0;
	tariffs_accums.t_2_r = 0;
	tariffs_accums.t_3_a = 0;
	tariffs_accums.t_3_r = 0;
	tariffs_accums.t_4_a = 0;
	tariffs_accums.t_4_r = 0;
	tariffs_accums.t_5_a = 0;
	tariffs_accums.t_6_a = 0;
	tariffs_accums.t_7_a = 0;
	tariffs_accums.t_8_a = 0;
	tariffs_accums.t_10 = 0;
	tariffs_accums.t_11 = 0;
	
	//инициализация програм, расписани, планов, етс.
	tariffs_make_basic_schedule();
	tariffs_make_basic_daily_programs();
	tariffs_make_basic_month_programs();
	
	uint32_t first_plan_start_time = rtc_get_date();
	
	first_plan.updating_flag = 0;
	first_plan.tarrif_program = month_programs;
	first_plan.date_of_application = first_plan_start_time;
	first_plan.count_of_exeptional_days = tariffs_get_count_of_exeptional_days(&first_plan);
	//first_plan.checksum = tariffs_calculate_checksum(&first_plan);
	current_plan = &first_plan;
	
	//TODO
	second_plan.updating_flag = 1;
	second_plan.tarrif_program = month_programs;
	second_plan.date_of_application = 0;
	second_plan.count_of_exeptional_days = tariffs_get_count_of_exeptional_days(&second_plan);
	//second_plan.checksum = tariffs_calculate_checksum(&second_plan);
}

/*
!Базовая программа.
[00:00 07:00) - ночь - активная энергия накапливается в t_1_a, реактивная в t_1_r
[07:00 11:00) - утро - активная энергия накапливается в t_2_a, реактивная в t_2_r
[11:00 17:00) - день - активная энергия накапливается в t_3_a, реактивная в t_3_r
[17:00 21:00) - вечер - активная энергия накапливается в t_4_a, реактивная в t_4_r
[21:00 00:00) - ночь - активная энергия накапливается в t_5_a, реактивная в t_1_r
*/
void tariffs_make_basic_schedule( void )
{
	uint8_t hour = 0;
	uint8_t temp = 0;
	uint8_t minutes = 0;
	uint32_t time = 0;
	for(uint8_t i = 0; i < 48; i++)
	{
		time = (time + hour) << 8;
		time = (time + minutes) << 8;
		shedule[i].start_time = time;
		
		//от 00:00 до 7:00 - ночь
		if(time < 0x00070000) shedule[i].active_tarif_accum = &tariffs_accums.t_1_a;
		if(time < 0x00070000) shedule[i].reactive_tarif_accum = &tariffs_accums.t_1_r;
		
		//от 07:00 до 11:00 - утро
		if(time >= 0x00070000 && time < 0x00110000) shedule[i].active_tarif_accum = &tariffs_accums.t_2_a;
		if(time >= 0x00070000 && time < 0x00110000) shedule[i].reactive_tarif_accum = &tariffs_accums.t_2_r;
		
		//от 11:00 до 17:00 - день
		if(time >= 0x00110000 && time < 0x00170000) shedule[i].active_tarif_accum = &tariffs_accums.t_3_a;
		if(time >= 0x00110000 && time < 0x00170000) shedule[i].reactive_tarif_accum = &tariffs_accums.t_3_r;
		
		//от 17:00 до 21:00 - вечер
		if(time >= 0x00170000 && time < 0x00210000) shedule[i].active_tarif_accum = &tariffs_accums.t_4_a;
		if(time >= 0x00170000 && time < 0x00210000) shedule[i].reactive_tarif_accum = &tariffs_accums.t_4_r;
		
		//от 21:00 до 00:00 - ночь
		if(time >= 0x00210000 && time < 0x00240000) shedule[i].active_tarif_accum = &tariffs_accums.t_5_a;
		if(time >= 0x00210000 && time < 0x00240000) shedule[i].reactive_tarif_accum = &tariffs_accums.t_1_r;
		
		if( (i % 2 == 0) && (i != 0) ) 
		{
			hour = tariffs_to_BCD_format(temp + 1);
			minutes = 0;
			temp++;
		}
		if( (i % 2 != 0) && (i != 0) ) minutes = tariffs_to_BCD_format(30);
		time = 0;
	}
}

/*
!Базовая дневная программа. Каждому дню месяца назначается программа, описанная 
в функции tariffs_make_basic_schedule(). //TODO Также ставиться один исключительный день 
*/
void tariffs_make_basic_daily_programs( void )
{
	uint8_t day = 0x01;
	uint8_t temp = day;
	for( uint8_t i = 0; i < 32; i++)
	{
		if (temp == 32) 
		{
			//TODO 
			daily_programs[i].day = 0x01;
			daily_programs[i].exeptioal = 1;
			daily_programs[i].shedule = shedule;
		} else {
			day = tariffs_to_BCD_format(temp++);
			daily_programs[i].day = day;
			daily_programs[i].exeptioal = 0;
			daily_programs[i].shedule = shedule;
		}
	}
}

/*
! Базовая месячная программа. Каждому месяцу присваивается программа, описанная в 
функции tariffs_make_basic_daily_programs(). Год присваивается тот, который указан в 
INITIAL_YEAR в RTC.h
*/
void tariffs_make_basic_month_programs( void )
{
	uint8_t month = 0x01;
	uint8_t temp = month;
	uint8_t year = INITIAL_YEAR;
	for(uint8_t i = 0; i < 12; i++)
	{
		month = tariffs_to_BCD_format(temp++);
		month_programs[i].month = month;
		month_programs[i].bissextile = 0;
		month_programs[i].year = year;
		month_programs[i].daily_programs = daily_programs;
	}
}

/*
!Подсчет количесва исключтельных дней в данном арфном плане
*/
uint16_t tariffs_get_count_of_exeptional_days( tariff_plan* plan )
{
	uint16_t count = 0;
	for(uint8_t i = 0; i < 12; i++)
	{
		for(uint8_t k = 0; k < 32; k++)
		{
			if(plan->tarrif_program->daily_programs[k].exeptioal == 1) count++;
		}
	}
	 
	return count;
}

/*
!Накопление измеренной энергии. Последоваельный поиск по дате
//TODO начинать с метки 
*/
void tariffs_set_data( uint32_t P, uint32_t Q )
{
	tariffs_get_current_tariff();
	printf("SET_P: %u, Q %u\r\n", P, Q);
	*current_accum_for_P += P;
	*current_accum_for_Q += Q;
	printf("AF_SET_P: %u, Q %u\r\n", *current_accum_for_P, *current_accum_for_Q);
}

/*
!Получаем через поиск текущий тарифный накопитель. Если таковой не находиться, тарифным
накопителем становиться общий накопитель
*/
void tariffs_get_current_tariff( void )
{
	uint32_t timestamp[2];
	rtc_get_timestamp(timestamp);

	//Для уменьшения поиска выбирается текущее время/дата
	uint8_t month = (timestamp[1] & 0x0000FF00) >> 8;
	uint8_t day = (timestamp[1] & 0x00FF0000) >> 16;
	month = tariffs_from_BCD_format(month);
	day = tariffs_from_BCD_format(day);
	
	//Если беда со значениями - общий накопитель
	if(month > 12 || day > 31 || month == 0 || day == 0)
	{
		current_accum_for_P = &tariffs_accums.t_10;
		current_accum_for_Q = &tariffs_accums.t_11;
		return;
	}
	
	uint32_t start_time = 0, end_time = 0, date = 0;
	for(uint8_t i = month-1; i < 12; i++)
	{
		for(uint8_t j = day-1; j < 32; j++)
		{
			date = (date + current_plan->tarrif_program[i].daily_programs[j].day) << 8;
			date = (date + current_plan->tarrif_program[i].month) << 8;
			date = (date + current_plan->tarrif_program[i].year);
			if(timestamp[1] == date)
			{
				for(uint8_t k = 0; k < 48; k++)
				{
					start_time = current_plan->tarrif_program[i].daily_programs[j].shedule[k].start_time;
					end_time = current_plan->tarrif_program[i].daily_programs[j].shedule[k+1].start_time;
					if((timestamp[0] >= start_time) && (timestamp[0] < end_time))
					{
						current_accum_for_P = current_plan->tarrif_program[i].daily_programs[j].shedule[k].active_tarif_accum;
						current_accum_for_Q = current_plan->tarrif_program[i].daily_programs[j].shedule[k].reactive_tarif_accum;
						return;
					}
				}
			}
			date = 0;
		}
	}
	current_accum_for_P = &tariffs_accums.t_10;
	current_accum_for_Q = &tariffs_accums.t_11;
}

/*
!Формирование ретроспективы, аналогично ретроспективы РОНЭ
*/
void tariffs_send_retrospective_to_eeprom( uint8_t date, uint32_t *timestamp )
{
	uint32_t data_to_mem[16];
	data_to_mem[0] = timestamp[0];
	data_to_mem[1] = timestamp[1];
	data_to_mem[2] = tariffs_accums.t_1_a;
	data_to_mem[3] = tariffs_accums.t_1_r;
	data_to_mem[4] = tariffs_accums.t_2_a;
	data_to_mem[5] = tariffs_accums.t_2_r;
	data_to_mem[6] = tariffs_accums.t_3_a;
	data_to_mem[7] = tariffs_accums.t_3_r;
	data_to_mem[8] = tariffs_accums.t_4_a;
	data_to_mem[9] = tariffs_accums.t_4_r;
	data_to_mem[10] = tariffs_accums.t_5_a;
	data_to_mem[11] = tariffs_accums.t_6_a;
	data_to_mem[12] = tariffs_accums.t_7_a;
	data_to_mem[13] = tariffs_accums.t_8_a;
	data_to_mem[14] = tariffs_accums.t_10;
	data_to_mem[15] = tariffs_accums.t_11;
	
	if(date == 1)
	{
		m24m01_save_to_mem(current_address_of_tariffs_day_retrosective, (uint8_t *) data_to_mem, 64);
		current_address_of_tariffs_day_retrosective += 64;
		if(current_address_of_tariffs_day_retrosective >= MEM_MAX_ADDRESS_OF_TARIFFS_DAY_RETROSPEC)
			current_address_of_tariffs_day_retrosective = 0x10F8;
	} else if (date == 2)
	{
		m24m01_save_to_mem(current_address_of_tariffs_month_retrosective, (uint8_t *) data_to_mem, 64);
		current_address_of_tariffs_month_retrosective += 64;
		if(current_address_of_tariffs_month_retrosective >= MEM_MAX_ADDRESS_OF_TARIFFS_MONTH_RETROSPEC)
			current_address_of_tariffs_month_retrosective = 0x3178;
	} else if (date == 3)
	{
		m24m01_save_to_mem(current_address_of_tariffs_year_retrosective, (uint8_t *) data_to_mem, 64);
		current_address_of_tariffs_year_retrosective += 64;
		if(current_address_of_tariffs_year_retrosective >= MEM_MAX_ADDRESS_OF_TARIFFS_YEAR_RETROSPEC)
			current_address_of_tariffs_year_retrosective = 0x3B78;
	}
}

/*
!
*/
void tariffs_update_plan( void )
{
	if(HAL_RTC_GetState(&hrtc) == HAL_OK)
	{
		
	} else {
		
	}
}

/*
! Функция для преобразования десятичного времени в удобочитаемый для человека и
понятный RTC. Если поступет на вход 15 часов (десятичный формат) - на выходе получаем 15
в 16ричном формате (21 в десятичном). Аналогично для даты
*/
uint8_t tariffs_to_BCD_format( uint32_t val )
{
	if(val > 9 && val < 20)
	{
		val += 6;
	} else if (val >= 20 && val < 30)
	{
		val += 12;
	} else if (val >= 30 && val < 40)
	{
		val += 18;
	} else if (val >= 40 && val < 50)
	{
		val += 24;
	} else if(val >= 50 && val < 60)
	{
		val += 30;
	}
	
	return val;
}

/*
! Функция обратная tariffs_to_BCD_format().
*/
uint8_t tariffs_from_BCD_format( uint32_t val )
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
