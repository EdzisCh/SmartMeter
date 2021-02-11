#include "mem_handler.h" 

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

void mem_handler_set_tarrif_accumulator( void )
{
}

void mem_handler_get_data( void )
{
}

void mem_handler_get_total_energy_register( void )
{
}
 
void mem_handler_get_tarrif_accumulator( void )
{
}

/**
!Формирование ретроспективы и отправка в память. 

*/
void mem_handler_send_retrospective_to_eeprom( total_energy_register *regs )
{
	uint32_t data_to_mem[6];
	rtc_get_timestamp(data_to_mem);
	data_to_mem[2] = regs->consumed_active_energy;
	data_to_mem[3] = regs->consumed_reactive_energy;
	data_to_mem[4] = regs->released_active_energy;
	data_to_mem[5] = regs->released_reactive_energy;
	
	m24m01_save_to_mem(current_address, (uint8_t *) data_to_mem, 24);
}
