#ifndef _MEM_HANDLER_H_
#define _MEM_HANDLER_H_

#include "stm32l4xx_hal.h"
#include "M24M01.h"

#define MEM_MAX_ADDRESS_OF_DAY_RETROSPEC 5256
#define MEM_MAX_ADDRESS_OF_MONTH_RETROSPEC 6856
#define MEM_MAX_ADDRESS_OF_YEAR_RETROSPEC 6857

#define MEM_INIT_ADDRESS_OF_DAY_RETROSPEC 137
#define MEM_INIT_ADDRESS_OF_MONTH_RETROSPEC 5257
#define MEM_INIT_ADDRESS_OF_YEAR_RETROSPEC 6857

#define MEM_ADDRESS_CURRENT_ADDRESS_OF_DAY_RERTOSPEC 40000
#define MEM_ADDRESS_CURRENT_ADDRESS_OF_MONTH_RERTOSPEC 40004
#define MEM_ADDRESS_CURRENT_ADDRESS_OF_YEAR_RERTOSPEC 40008 

/**
! Промежуточные данные из измерителей
*/
typedef struct data
{
	double P;
	double Q;
	double S;
	double PF;
	double I;
	double U;
	double F;
} data;


/**
! Регистры общего накопления энергии
*/
typedef struct total_energy_register
{	
	double consumed_active_energy;
	double released_active_energy;
	double consumed_reactive_energy;
	double released_reactive_energy;
	
} total_energy_register;


void mem_handler_set_I(data * data, double I);
void mem_handler_set_U(data * data, double U);
void mem_handler_set_F(data * data, double freq);
void mem_handler_set_P(data * data, double P);
void mem_handler_set_Q(data * data, double Q);
void mem_handler_set_S(data * data, double S);
void mem_handler_set_PF(data * data, double PF);

void mem_handler_set_total_energy_register( total_energy_register *regs, data *data );

void mem_handler_get_data( void );
void mem_handler_get_total_energy_register( void );

void mem_handler_send_retrospective_to_eeprom( uint8_t date, uint32_t *timestamp, total_energy_register *regs );

void mem_handler_get_current_address_from_eeprom( void );
#endif
