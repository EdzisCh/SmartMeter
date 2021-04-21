#ifndef _M24M01_H_
#define _M24M01_H_

#include "stm32l4xx_hal.h"

#define M24M01_DEVICE_ADDRESS 0xA0
#define MAX_MEM_ADDRESS 124999
#define MAX_BYTES_SIZE_TO_RECEIVE 240

extern I2C_HandleTypeDef hi2c3;

uint8_t m24m01_get_from_mem( uint32_t address, uint8_t* data, uint8_t size_of_data );
uint8_t m24m01_save_to_mem( uint32_t address, uint8_t* data, uint8_t size_of_data );

#endif
