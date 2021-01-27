#ifndef _S25FL_H_
#define _S25FL_H_

#include "stm32l4xx_hal.h"

extern QSPI_HandleTypeDef hqspi;

uint8_t s25fl_send_to_mem( void );
uint8_t s25fl_get_from_mem( void );

#endif
