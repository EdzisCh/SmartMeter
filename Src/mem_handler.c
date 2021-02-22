#include "mem_handler.h" 
#include "stdio.h"

/**
!Сохранение данных из змериеля в промежуточные накопители
*/
void mem_handler_set_data( data *data, uint32_t P, 
						   uint32_t Q, uint32_t S, 
                           uint32_t cosPhi, uint32_t I, 
						   uint32_t U, uint32_t freq )
{
	data->cosPhi = cosPhi;
	data->freq = freq;
	data->I = I;
	data->P += P;
	data->Q += Q;
	data->S += S;
	data->U = U;
}

void mem_handler_set_total_energy_register( total_energy_register *regs, data *data )
{
	regs->consumed_active_energy = data->P;
	regs->consumed_reactive_energy = data->Q;
	regs->released_active_energy = 0x00;
	regs->released_reactive_energy = 0x00;
}

void mem_handler_get_data( void )
{
}

void mem_handler_get_total_energy_register( void )
{
}
 
/**
!Формирование ретроспективы для каждого изменения даты (день, месяц, год) и отправка в память. 
date - отвечает за вид ретроспективы: 1 - дневная, 2 - месячная, 3 - годовая
*/
void mem_handler_send_retrospective_to_eeprom( uint8_t date, uint32_t *timestamp, total_energy_register *regs )
{
	uint32_t data_to_mem[6];
	data_to_mem[0] = timestamp[0];
	data_to_mem[1] = timestamp[1];
	data_to_mem[2] = regs->consumed_active_energy;
	data_to_mem[3] = regs->consumed_reactive_energy;
	data_to_mem[4] = regs->released_active_energy;
	data_to_mem[5] = regs->released_reactive_energy;
	
	if(date == 1)
	{
		m24m01_save_to_mem(current_address_of_day_retrosective, (uint8_t *) data_to_mem, 24);
		current_address_of_day_retrosective += 24;
		if(current_address_of_day_retrosective >= MEM_MAX_ADDRESS_OF_DAY_RETROSPEC)
			current_address_of_day_retrosective = 0x00;
	} else if (date == 2)
	{
		m24m01_save_to_mem(current_address_of_month_retrosective, (uint8_t *) data_to_mem, 24);
		current_address_of_month_retrosective += 24;
		if(current_address_of_month_retrosective >= MEM_MAX_ADDRESS_OF_MONTH_RETROSPEC)
			current_address_of_month_retrosective = 0x0C00;
	} else if (date == 3)
	{
		m24m01_save_to_mem(current_address_of_year_retrosective, (uint8_t *) data_to_mem, 24);
		current_address_of_year_retrosective += 24;
		if(current_address_of_year_retrosective >= MEM_MAX_ADDRESS_OF_YEAR_RETROSPEC)
			current_address_of_year_retrosective = 0x0FC0;
	}
}
