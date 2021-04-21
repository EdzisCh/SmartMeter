#ifndef _TARIFFS_H_
#define _TARIFFS_H_

#include "stm32l4xx.h"
#include "RTC.h"
#include "M24M01.h"
#include "lcd.h"

#define MEM_MAX_ADDRESS_OF_TARIFFS_DAY_RETROSPEC 22616
#define MEM_MAX_ADDRESS_OF_TARIFFS_MONTH_RETROSPEC 27416
#define MEM_MAX_ADDRESS_OF_TARIFFS_YEAR_RETROSPEC 28617

#define MEM_INIT_ADDRESS_OF_TARIFFS_DAY_RETROSPEC 7257
#define MEM_INIT_ADDRESS_OF_TARIFFS_MONTH_RETROSPEC 22617
#define MEM_INIT_ADDRESS_OF_TARIFFS_YEAR_RETROSPEC 27417

#define MEM_ADDRESS_TARIFFS_CURRENT_ADDRESS_OF_DAY_RERTOSPEC 40012
#define MEM_ADDRESS_TARIFFS_CURRENT_ADDRESS_OF_MONTH_RERTOSPEC 40016
#define MEM_ADDRESS_TARIFFS_CURRENT_ADDRESS_OF_YEAR_RERTOSPEC 40020 

extern double *current_accum_for_P;
extern double *current_accum_for_Q;
extern RTC_HandleTypeDef hrtc;

/*
! Тарифные накопители. t_x_e - x - номер накопителя, e - активная или реактивная энергии
t_10, t_11 - безусловный учет
*/
typedef struct tariffs_accum
{
	double t_1_a;
	double t_2_a;
	double t_3_a;
	double t_4_a;
	double t_5_a;
	double t_6_a;
	double t_7_a;
	double t_8_a;
	double t_1_r;
	double t_2_r;
	double t_3_r;
	double t_4_r;
	double t_10;
	double t_11;
} tariffs_accum;
 
/*
!Ячейка расписания на день(время - тариф)
start_time - время начала действия тарифа (включительно)
tarrif_accum - указатель на тарифный накопитель 
*/
typedef struct shedule_item
{
	uint32_t start_time;
	double *active_tarif_accum;
	double *reactive_tarif_accum;
}shedule_item;

/*
!Расписание на каждый день месяца
exeptioal - является ли день исключительным
* shedule - указатель на расписание
*/
typedef struct daily_program 
{
	uint8_t day;
	uint8_t exeptioal;
	shedule_item* shedule;
}daily_program;

/*
!Расписание на каждый день месяца
bissextile - флаг високосного года
* daily_programs - указатель на на программу по дням месяца
*/
typedef struct month_program
{
	uint8_t month;
	uint8_t year;
	uint8_t bissextile;
	daily_program* daily_programs;
} month_program;

/*
!Тарифный план
updating_flag - флаг актуализации; 1 - фоновый план, ждет своей очереди
date_of_application - дата применения; 00ДДММГГ - ДД - день, ММ - месяц, ГГ - год
*/
typedef struct tariff_plan
{
	uint8_t plan_numer;
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

void tariffs_update_plan( uint8_t event, uint8_t command );
void tariffs_set_data( double P, double Q );
void tariffs_get_current_tariff( void );
void tariffs_send_retrospective_to_eeprom( uint8_t date, uint32_t *timestamp );

uint32_t tariffs_calculate_checksum( tariff_plan* plan );
uint16_t tariffs_get_count_of_exeptional_days( tariff_plan* plan );
uint8_t tariffs_to_BCD_format( uint32_t val );
uint8_t tariffs_from_BCD_format( uint32_t val );
#endif
