	/**
	!	

	*/
#ifndef CS5490_H
#define CS5490_H

#include "stm32l4xx_hal.h"
#include "math.h"
#include "stdint.h"

#define CALIBRATION_TYPE_DC_OFFSET 0x00
#define CALIBRATION_TYPE_AC_OFFSET 0x10
#define CALIBRATION_TYPE_GAIN 0x18

#define CALIBRATION_CHANNEL_CURRENT 0x01
#define CALIBRATION_CHANNEL_VOLTAGE 0x02
#define CALIBRATION_CHANNEL_CURRENT_AND_VOLTAGE 0x06

#define MCLK_DEFAULT_VALUE 4.096
#define BAUDRATE_DEFAULT_VALUE 600

#define HARDWARE_PAGE_BYTE 0x00
#define SOFTWARE_PAGE_BYTE 0x90
#define INSTRUCTION_BYTE 0xC0
#define CALIBRATION_BYTE 0x20


	/**
	! Структура измерителя
	
	*/
typedef struct CS5490
{
	UART_HandleTypeDef *cs5490_huart;
	uint8_t cs5490_data[3];
	int cs5490_selectedPage;
	float cs5490_MCLK;
} CS5490;

void cs5490_write( CS5490 *chip, int page, int address, uint32_t value );
void cs5490_read( CS5490 *chip, int page, int address );
void cs5490_instruct( CS5490 *chip, int instruction );
uint32_t cs5490_concatData( CS5490 *chip );
uint32_t cs5490_readReg( CS5490 *chip, int page, int address );
void cs5490_calibrate( CS5490 *chip, uint8_t type, uint8_t channel );

//===================================================================================

void cs5490_reset( CS5490 *chip );
void cs5490_standby( CS5490 *chip );
void cs5490_wakeup( CS5490 *chip );

//===================================================================================

uint32_t cs5490_getInstCurrent( CS5490 *chip );
uint32_t cs5490_getInstVoltage( CS5490 *chip );
uint32_t cs5490_getInstPower( CS5490 *chip );

uint32_t cs5490_getAvgPower( CS5490 *chip );
uint32_t cs5490_getAvgQ( CS5490 *chip );
uint32_t cs5490_getAvgS( CS5490 *chip );

uint32_t cs5490_getTotalPower( CS5490 *chip );
uint32_t cs5490_getTotalS( CS5490 *chip );
uint32_t cs5490_getTotalQ( CS5490 *chip );

uint32_t cs5490_getFreq( CS5490 *chip );

uint32_t cs5490_getTime( CS5490 *chip );
#endif
