#ifndef __LCD_H__
#define __LCD_H__
	
#include "stm32l4xx_hal.h"
#include "stdint.h"	
	
#define LCD_ADDRESS 0x7E

#define CMD_MODE_SET 0x80
		#define CMD_MODE_BIAS13 0x00
		#define CMD_MODE_BIAS12 0x01
		#define CMD_DISP_OFF_DIS_CLK 0x00 
		#define CMD_DISP_OFF_ENA_CLK 0x02 
		#define CMD_DISP_ON_ENA_CLK 0x0C 
		#define CMD_FRAME_FREQ_80HZ 0x00 
		#define CMD_FRAME_FREQ_160HZ 0x10 
		
#define CMD_BLINK 0xC0
		#define CMD_BLINK_OFF 0x00
		#define CMD_BLINK_2HZ 0x01
		#define CMD_BLINK_1HZ 0x02
		#define CMD_BLINK_05HZ 0x03
		
#define CMD_SET_ADDR 0x00
		#define CMD_ADDR_MASK 0x1f
		
#define CMD_IVA 0x40
		#define CMD_IVA_DA_MASK 0x0f
		#define CMD_IVA_DE1_VE1 0x28 

extern I2C_HandleTypeDef hi2c2;

uint8_t display_init( void );
uint8_t display_all_data_write( void );
uint8_t display_clear( void );
uint8_t display_data_write( uint16_t address, uint8_t *data, uint8_t sizeOfData );
uint8_t display_byte_read( uint16_t address );

void display_L1( void );
void display_L2( void );
void display_L3( void );
void display_R1( void );
void display_R2( void );
void display_P( void );
void display_N( void );
void display_battery( void );
void display_ExMark( void );
void display_lock_1( void );
void display_lock_2( void );
void display_level( uint8_t level );
void display_WIFI( void );
void display_GR( void );
void display_PR( void );
void display_T( void );

#endif
