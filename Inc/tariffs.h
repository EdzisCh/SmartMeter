#ifndef _TARIFFS_H_
#define _TARIFFS_H_

#include "stm32l4xx.h"
#include "RTC.h"

/*
! Тарифные накопители. t_x_e - x - номер накопителя, e - активная или реактивная энергии
t_10, t_11 - безусловный учет
*/
typedef struct tariffs_accum
{
	uint32_t t_1_a;
	uint32_t t_2_a;
	uint32_t t_3_a;
	uint32_t t_4_a;
	uint32_t t_5_a;
	uint32_t t_6_a;
	uint32_t t_7_a;
	uint32_t t_8_a;
	uint32_t t_1_r;
	uint32_t t_2_r;
	uint32_t t_3_r;
	uint32_t t_4_r;
	uint32_t t_10;
	uint32_t t_11;
} tariffs_accum;
 
/*
!Ячейка расписания (время - тариф)
start_time - время начала действия тарифа (включительно)
end_time - время окончания действия тарифа (не включительно)
tarrif_accum - указатель на тарифный накопитель 
*/
typedef struct shedule_item
{
	uint32_t start_time;
	uint32_t end_time;
	uint32_t *tarif_accum;
}shedule_item;

/*
!Тарифный план
updating_flag - флаг актуализации. 1 - фоновый план, ждет своей очереди
date_of_application - дата применения. 00ДДММГГ - ДД - день, ММ - месяц, ГГ - год
*/
typedef struct tariff_plan
{
	uint8_t updating_flag;
	uint32_t date_of_application;
	shedule_item* shedule;
	uint32_t checksum;
} tariff_plan;


void tarifs_init( void );
uint32_t tarifs_calculate_checksum( tariff_plan* plan );
#endif
