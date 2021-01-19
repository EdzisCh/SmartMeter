	/**
	!
	*/
#ifndef _MEM_HANDLER_H_
#define _MEM_HANDLER_H_

#include "stm32l4xx_hal.h"

	/**
	! Промежуточные данные из измерителей
	
	*/
typedef struct data
{
	uint32_t P;
	uint32_t Q;
	uint32_t S;
	uint32_t cosPhi;
	uint32_t I;
	uint32_t U;
	uint32_t freq;
	
	
} data;


	/**
	! Регистры общего накопления энергии
	
	*/
typedef struct total_energy_register
{	
	uint32_t consumed_active_energy;
	uint32_t released_active_energy;
	uint32_t consumed_reactive_energy;
	uint32_t released_reactive_energy;
	
} total_energy_register;

	/**
	! Тарифный накопитель
	*/
typedef struct tarrif_accumulator
{
	
} tarrif_accumulator;

void set_data();
void set_total_energy_register();
void set_tarrif_accumulator();
void get_data();
void get_total_energy_register();
void get_tarrif_accumulator();

#endif
