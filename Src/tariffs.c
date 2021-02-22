#include "tariffs.h"

//Расписание
shedule_item shedule[48];

tariffs_accum tariffs_accums; 

tariff_plan first_plan;
tariff_plan second_plan;

/*
! Инициализация первоначальная 
*/
void tarifs_init( void )
{
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
	
	uint32_t first_plan_start_time = rtc_get_date();
	
	first_plan.updating_flag = 0;
	first_plan.shedule = shedule;
	first_plan.date_of_application = first_plan_start_time;
	
	//TODO
	second_plan.updating_flag = 1;
	second_plan.date_of_application = 0;
}

