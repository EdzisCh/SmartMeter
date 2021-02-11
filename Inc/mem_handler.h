	/**
	!
	*/
#ifndef _MEM_HANDLER_H_
#define _MEM_HANDLER_H_

#include "stm32l4xx_hal.h"
#include "RTC.h"
#include "M24M01.h"
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
	uint32_t temp;
} tarrif_accumulator;

void mem_handler_set_data( data *data, uint32_t P, uint32_t Q, uint32_t S, uint32_t cosPhi, uint32_t I, uint32_t U, uint32_t freq );
void mem_handler_set_total_energy_register( total_energy_register *regs, data *data );
void mem_handler_set_tarrif_accumulator( void );
void mem_handler_get_data( void );
void mem_handler_get_total_energy_register( void );
void mem_handler_get_tarrif_accumulator( void );

void mem_handler_send_retrospective_to_eeprom( total_energy_register *regs );

#endif
