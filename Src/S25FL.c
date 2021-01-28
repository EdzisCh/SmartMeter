#include "S25FL.h"

/**
!Инициализация модуля работы с flash памятью

*/
void s25fl_init( void )
{
	
}	

uint8_t* s25fl_send_command( uint8_t command )
{
	uint8_t* result = NULL;
	

//	HAL_QSPI_Command();
	

//	if (bytesToRead > 0)
//	{
//	result = new uint8_t[bytesToRead];
//	for (int i = 0; i<bytesToRead; i++)
//	{
//	  result[i] = HAL_QSPI_Transmit(&hqspi, result, 255);
//	}
//	}
	
	return result;
}

uint8_t s25fl_send_to_mem( void )
{
	
}

uint8_t s25fl_get_from_mem( void )
{
	
}
