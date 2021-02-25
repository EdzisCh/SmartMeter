#ifndef _TARIFFS_H_
#define _TARIFFS_H_

#include "stm32l4xx.h"
#include "RTC.h"
#include "M24M01.h"

#define MEM_MAX_ADDRESS_OF_TARIFFS_DAY_RETROSPEC 0x3178
#define MEM_MAX_ADDRESS_OF_TARIFFS_MONTH_RETROSPEC 0x3B78
#define MEM_MAX_ADDRESS_OF_TARIFFS_YEAR_RETROSPEC 0x3DF8
extern uint32_t *current_accum_for_P;
extern uint32_t *current_accum_for_Q;

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
tarrif_accum - указатель на тарифный накопитель 
*/
typedef struct shedule_item
{
	uint32_t start_time;
	uint32_t *active_tarif_accum;
	uint32_t *reactive_tarif_accum;
}shedule_item;

/*
!
*/
typedef struct daily_program 
{
	uint8_t day;
	uint8_t exeptioal;
	shedule_item* shedule;
}daily_program;


typedef struct month_program
{
	uint8_t month;
	uint8_t year;
	uint8_t bissextile;
	daily_program* daily_programs;
} month_program;

/*
!Тарифный план
updating_flag - флаг актуализации. 1 - фоновый план, ждет своей очереди
date_of_application - дата применения. 00ДДММГГ - ДД - день, ММ - месяц, ГГ - год
*/
typedef struct tariff_plan
{
	uint8_t updating_flag;
	uint32_t date_of_application;
	month_program* tarrif_program;
	uint32_t checksum;
	uint16_t count_of_exeptional_days;
} tariff_plan;


void tariffs_init( void );
void tariffs_make_basic_schedule( void );
void tariffs_make_basic_daily_programs( void );
void tariffs_make_basic_month_programs( void );

void tariffs_update_plan( void );
void tariffs_set_data( uint32_t P, uint32_t Q );
void tariffs_get_current_tariff( void );
void tariffs_send_retrospective_to_eeprom( uint8_t date, uint32_t *timestamp );

uint32_t tariffs_calculate_checksum( tariff_plan* plan );
uint16_t tariffs_get_count_of_exeptional_days( tariff_plan* plan );
uint8_t tariffs_to_BCD_format( uint32_t val );
uint8_t tariffs_from_BCD_format( uint32_t val );
#endif
