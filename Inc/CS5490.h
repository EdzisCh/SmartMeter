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

#define WRITE_BYTE 0x40
#define READ_BYTE 0x00
#define PAGE_BYTE 0x80
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

void cs5490_init( CS5490 *chip, uint8_t conv_type );
void cs5490_write( CS5490 *chip, int page, int address, uint32_t value );
void cs5490_read( CS5490 *chip, uint8_t page, uint8_t address );
void cs5490_instruct( CS5490 *chip, int instruction );
uint32_t cs5490_concatData( CS5490 *chip );
uint32_t cs5490_readReg( CS5490 *chip, int page, int address );
void cs5490_calibrate( CS5490 *chip, uint8_t type, uint8_t channel );
double cs5490_convert_to_double( CS5490 *chip, int LSBpow, int MSBoption );

//===================================================================================

void cs5490_reset( CS5490 *chip );
void cs5490_standby( CS5490 *chip );
void cs5490_wakeup( CS5490 *chip );
void cs5490_single_conversation( CS5490 *chip );
void cs5490_continious_conversation( CS5490 *chip );
void cs5490_halt_conversation( CS5490 *chip );

//===================================================================================

float cs5490_get_DC_Offset_V( CS5490 *chip );
void cs5490_set_DC_Offset_V( CS5490 *chip, float value );

//===================================================================================

uint32_t cs5490_get_I( CS5490 *chip );
double cs5490_get_V( CS5490 *chip );
uint32_t cs5490_get_P( CS5490 *chip );

double cs5490_get_Irms( CS5490 *chip );
double cs5490_get_Vrms( CS5490 *chip );

uint32_t cs5490_get_Pavg( CS5490 *chip );
uint32_t cs5490_get_Qavg( CS5490 *chip );
uint32_t cs5490_get_Savg( CS5490 *chip );

uint32_t cs5490_get_total_P( CS5490 *chip );
uint32_t cs5490_get_total_S( CS5490 *chip );
uint32_t cs5490_get_total_Q( CS5490 *chip );

double cs5490_get_freq( CS5490 *chip );

double cs5490_get_time( CS5490 *chip );

double cs5490_get_T( CS5490 *chip );
#endif
