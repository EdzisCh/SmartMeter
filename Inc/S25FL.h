#ifndef _S25FL_H_
#define _S25FL_H_

#include "stm32l4xx_hal.h"

#define S25FL_WRITE_ENABLE 0x06
#define S25FL_WRITE_DISABLE 0x04
#define S25FL_READ_STATUS_REG_1 0x05
#define S25FL_READ_STATUS_REG_2 0x35
#define S25FL_WRITE_STATUS_REG 0x01
#define S25FL_READ_DATA 0x03
#define S25FL_FAST_READ 0x0B
#define S25FL_FAST_READ_DUAL_OUTPUT 0x3B
#define S25FL_PAGE_PROGRAM 0x02
#define S25FL_BLOCK_ERASE 0xD8 // 64kb
#define S25FL_SECTOR_ERASE 0x20 // 4kb
#define S25FL_CHIP_ERASE 0xC7 // or 0x60;
#define S25FL_POWER_DOWN 0xB9
#define S25FL_RELEASE_POWER_DOWN 0xAB
#define S25FL_DEV_ID 0x90
#define S25FL_JEDEC_ID 0x9F
#define S25FL_DUMMY_VALUE 0x00
#define S25FL_WIP_MASK 0x01

extern QSPI_HandleTypeDef hqspi;

uint8_t s25fl_send_write_enable( void );
uint8_t s25fl_send_write_disable( void );
uint8_t s25fl_read_status_register( uint8_t *data );//name

uint8_t s25fl_sector_erase( uint8_t address );
uint8_t s25fl_block_erase( uint8_t address );
uint8_t s25fl_chip_erase( void );

uint8_t s25fl_send_to_mem( uint32_t address, uint8_t *data, uint32_t size );
uint8_t s25fl_get_from_mem( uint32_t address, uint8_t *data, uint32_t size );

uint8_t s25fl_read_device_ID( uint8_t *data );
uint8_t s25fl_release_from_DPD( uint8_t *data );

#endif
