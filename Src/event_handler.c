#include "event_handler.h"

	/**
	! Включаем звук (сигнал 4кГц). После вызова необходимо вызывать
	выключение!
	
	*/
void event_handler_beep_on( void )
{
	uint16_t value = 0;
	int8_t step = 0;
	
	if( value == 0 ) step = 1;
	if( value == 2 ) step = 0;
	
	value += step;
	TIM5->CCR4 = value;
}

void event_handler_beep_off( void )
{

	TIM5->CCR4 = 0;
}
