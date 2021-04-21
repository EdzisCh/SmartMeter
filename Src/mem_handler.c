#include "mem_handler.h" 
#include "stdio.h"

uint32_t current_address_of_day_retrosective;
uint32_t current_address_of_month_retrosective;
uint32_t current_address_of_year_retrosective;


void mem_handler_set_total_energy_register( total_energy_register *regs, data *data )
{
	regs->consumed_active_energy += data->P;
	regs->consumed_reactive_energy += data->Q;
	regs->released_active_energy = 0x00;
	regs->released_reactive_energy = 0x00;
}

//===================================================================================

/**
!Сохранение данных из змериеля в промежуточные накопители
*/
void mem_handler_set_I(data * data, double I)
{
	data->I = I;	
}

void mem_handler_set_U(data * data, double U)
{
	data->U = U;
}

void mem_handler_set_F(data * data, double freq)
{
	data->F = freq;
}

void mem_handler_set_P(data * data, double P)
{
	data->P = P;
}

void mem_handler_set_Q(data * data, double Q)
{
	data->Q = Q;
}

void mem_handler_set_S(data * data, double S)
{
	data->S = S;	
}

void mem_handler_set_PF(data * data, double PF)
{
	data->PF = PF;
}

//===================================================================================

void mem_handler_get_data( void )
{
}

void mem_handler_get_total_energy_register( void )
{
}
 
//===================================================================================
 
/**
!Формирование ретроспективы для каждого изменения даты (день, месяц, год) и отправка в память. 
date - отвечает за вид ретроспективы: 1 - дневная, 2 - месячная, 3 - годовая
*/
void mem_handler_send_retrospective_to_eeprom( uint8_t date, uint32_t *timestamp, total_energy_register *regs )
{
	uint32_t timestamp_to_mem[2];
	timestamp_to_mem[0] = timestamp[0];
	timestamp_to_mem[1] = timestamp[1];
	double data_to_mem[4];
	data_to_mem[0] = regs->consumed_active_energy;
	data_to_mem[1] = regs->consumed_reactive_energy;
	data_to_mem[2] = regs->released_active_energy;
	data_to_mem[3] = regs->released_reactive_energy;
	
	if(date == 1)
	{
		m24m01_save_to_mem(current_address_of_day_retrosective, (uint8_t *) timestamp_to_mem, 8);
		m24m01_save_to_mem(current_address_of_day_retrosective + 8, (uint8_t *) data_to_mem, 32);
		current_address_of_day_retrosective += 40;
		if(current_address_of_day_retrosective >= MEM_MAX_ADDRESS_OF_DAY_RETROSPEC)
			current_address_of_day_retrosective = MEM_INIT_ADDRESS_OF_DAY_RETROSPEC;
		
		m24m01_save_to_mem(MEM_ADDRESS_CURRENT_ADDRESS_OF_DAY_RERTOSPEC, (uint8_t*)&current_address_of_day_retrosective, 4);
	} else if (date == 2)
	{
		m24m01_save_to_mem(current_address_of_month_retrosective, (uint8_t *) timestamp_to_mem, 8);
		m24m01_save_to_mem(current_address_of_month_retrosective + 8, (uint8_t *) data_to_mem, 32);
		current_address_of_month_retrosective += 40;
		if(current_address_of_month_retrosective >= MEM_MAX_ADDRESS_OF_MONTH_RETROSPEC)
			current_address_of_month_retrosective = MEM_INIT_ADDRESS_OF_MONTH_RETROSPEC;
		
		m24m01_save_to_mem(MEM_ADDRESS_CURRENT_ADDRESS_OF_MONTH_RERTOSPEC, (uint8_t*)&current_address_of_month_retrosective, 4);
	} else if (date == 3)
	{
		m24m01_save_to_mem(current_address_of_year_retrosective, (uint8_t *) timestamp_to_mem, 8);
		m24m01_save_to_mem(current_address_of_year_retrosective + 8, (uint8_t *) data_to_mem, 24);
		current_address_of_year_retrosective += 40;
		if(current_address_of_year_retrosective >= MEM_MAX_ADDRESS_OF_YEAR_RETROSPEC)
			current_address_of_year_retrosective = MEM_INIT_ADDRESS_OF_YEAR_RETROSPEC;
		
		m24m01_save_to_mem(MEM_ADDRESS_CURRENT_ADDRESS_OF_YEAR_RERTOSPEC, (uint8_t*)&current_address_of_year_retrosective, 4);
	}
}

//===================================================================================


void mem_handler_get_current_address_from_eeprom( void )
{
	m24m01_get_from_mem(MEM_ADDRESS_CURRENT_ADDRESS_OF_DAY_RERTOSPEC, (uint8_t*)&current_address_of_day_retrosective, 4);
	m24m01_get_from_mem(MEM_ADDRESS_CURRENT_ADDRESS_OF_MONTH_RERTOSPEC, (uint8_t*)&current_address_of_month_retrosective, 4);
	m24m01_get_from_mem(MEM_ADDRESS_CURRENT_ADDRESS_OF_YEAR_RERTOSPEC, (uint8_t*)&current_address_of_year_retrosective, 4);
}
