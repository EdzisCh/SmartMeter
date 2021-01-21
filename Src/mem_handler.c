#include "mem_handler.h" 

	/**
	!Сохранение данных из змериеля в промежуточные накопители
	*/
void mem_handler_set_data( data *data, uint32_t P, uint32_t Q, uint32_t S, uint32_t cosPhi, uint32_t I, uint32_t U, uint32_t freq )
{
	data->cosPhi = cosPhi;
	data->freq = freq;
	data->I = I;
	data->P = P;
	data->Q = Q;
	data->S = S;
	data->U = U;
}

void mem_handler_set_total_energy_register( void )
{
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
