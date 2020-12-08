#ifndef __LCD_H__
#define __LCD_H__
	
#include "stm32l4xx_hal.h"
#include "stdint.h"	
	
#define LCD_ADDRESS 0x7E

uint8_t lcd_init(void);
uint8_t display_data_write(void);

#endif
