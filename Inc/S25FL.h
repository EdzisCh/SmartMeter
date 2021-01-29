#ifndef _S25FL_H_
#define _S25FL_H_

#include "stm32l4xx_hal.h"

#define WRITE_ENABLE 0x06
#define WRITE_DISABLE 0x04
#define READ_STATUS_REG 0x05
#define WRITE_STATUS_REG 0x01
#define READ_DATA 0x03
#define FAST_READ 0x0B
#define FAST_READ_DUAL_OUTPUT 0x3B
#define PAGE_PROGRAM 0x02
#define BLOCK_ERASE 0xD8 // 64kb
#define SECTOR_ERASE 0x20 // 4kb
#define CHIP_ERASE 0xC7 // or 0x60;
#define POWER_DOWN 0xB9
#define RELEASE_POWER_DOWN 0xAB
#define DEV_ID 0x90
#define JEDEC_ID 0x9F
#define DUMMY_VALUE 0x00
#define WIP_MASK 0x01

extern QSPI_HandleTypeDef hqspi;
QSPI_CommandTypeDef command;

void s25fl_init( void );
void s25fl_make_command( void );
uint8_t* s25fl_send_command( uint8_t command );
uint8_t s25fl_send_to_mem( uint8_t address, uint8_t *data, uint8_t size );
uint8_t s25fl_get_from_mem( uint8_t address, uint8_t *data );
uint8_t s25fl_send_write_enable( void );

#endif
