#include "tariffs.h"

//Расписание
shedule_item shedule[48];
//Тарифная програма, номер програмы = индекс
daily_program programs[32];

tariffs_accum tariffs_accums; 

tariff_plan first_plan;
tariff_plan second_plan;

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
	uint8_t hour = 0;
	uint8_t temp = 0;
	uint8_t minutes = 0;
	uint32_t time = 0;
	for(uint8_t i = 0; i < 48; i++)
	{
		time = (time + hour) << 8;
		time = (time + minutes) << 8;
		shedule[i].start_time = time;
		
		//от 00:00 до 6:00
		if(time < 0x00070000) shedule[i].active_tarif_accum = &tariffs_accums.t_1_a;
		if(time < 0x00070000) shedule[i].reactive_tarif_accum = &tariffs_accums.t_1_r;
		
		//от 21:00 до 00:00
		if(time >= 0x00210000 && time < 0x00240000) shedule[i].active_tarif_accum = &tariffs_accums.t_1_a;
		if(time >= 0x00210000 && time < 0x00240000) shedule[i].reactive_tarif_accum = &tariffs_accums.t_1_r;
		
		//от 07:00 до 10:00
		if(time >= 0x00070000 && time < 0x00110000) shedule[i].active_tarif_accum = &tariffs_accums.t_2_a;
		if(time >= 0x00070000 && time < 0x00110000) shedule[i].reactive_tarif_accum = &tariffs_accums.t_2_r;
		
		//от 11:00 до 17:00
		if(time >= 0x00110000 && time < 0x00170000) shedule[i].active_tarif_accum = &tariffs_accums.t_3_a;
		if(time >= 0x00110000 && time < 0x00170000) shedule[i].reactive_tarif_accum = &tariffs_accums.t_3_r;
		
		//от 17:00 до 21:00
		if(time >= 0x00170000 && time < 0x00210000) shedule[i].active_tarif_accum = &tariffs_accums.t_4_a;
		if(time >= 0x00170000 && time < 0x00210000) shedule[i].reactive_tarif_accum = &tariffs_accums.t_4_r;
		
		if( (i % 2 == 0) && (i != 0) ) 
		{
			hour = tariffs_to_BCD_format(temp + 1);
			minutes = 0;
			temp++;
		}
		if( (i % 2 != 0) && (i != 0) ) minutes = tariffs_to_BCD_format(30);
		time = 0;
	}
	
	for( uint8_t i = 0; i < 32; i++)
	{
		programs[i].shedule = shedule;
	}
	
	uint32_t first_plan_start_time = rtc_get_date();
	
	first_plan.updating_flag = 0;
	first_plan.tarrif_program = programs;
	first_plan.date_of_application = first_plan_start_time;
	
	//TODO
	second_plan.updating_flag = 1;
	second_plan.date_of_application = 0;
}

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
