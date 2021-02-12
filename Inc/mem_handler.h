/**
!
*/
#ifndef _MEM_HANDLER_H_
#define _MEM_HANDLER_H_

#include "stm32l4xx_hal.h"
#include "M24M01.h"

#define MEM_MAX_ADDRESS_OF_DAY_RETROSPEC 0x0C00
#define MEM_MAX_ADDRESS_OF_MONTH_RETROSPEC 0x0FC0
#define MEM_MAX_ADDRESS_OF_YEAR_RETROSPEC 0x10B0

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

void mem_handler_send_retrospective_to_eeprom( uint8_t date, uint32_t *timestamp, total_energy_register *regs );

//тестовая функция
uint8_t mem_handler_get_all_stored_data( void );

#endif
