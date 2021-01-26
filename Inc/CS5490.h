	/**
	!	

	*/
#ifndef CS5490_H
#define CS5490_H

#include "stm32l4xx_hal.h"
#include "math.h"
#include "stdint.h"
#include "stdio.h"

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

#define R_SHUNT_OHM   100  
#define V_ALFA        (1000.0/1689000.0) 
#define SYS_GAIN      1.25 
#define V_FS          0.6
#define V_FS_RMS_V    0.17676 
#define V_MAX_RMS     (V_FS_RMS_V/V_ALFA)
#define I_FS          0.6
#define I_FS_RMS_V    0.17676 
#define I_MAX_RMS_A   (I_FS_RMS_V/R_SHUNT_OHM)
#define P_FS          0.36
#define P_COEFF       ((V_MAX_RMS * I_MAX_RMS_A) / (P_FS * SYS_GAIN * SYS_GAIN))
#define SAMPLE_COUNT_DEFAULT  4000
#define I_CAL_RMS_A  1.3126 
#define SCALE_REGISTER_FRACTION  (0.6 * SYS_GAIN * (I_CAL_RMS_A / I_MAX_RMS_A)) 
#define SCALE_REGISTER_VALUE ((uint32_t)(SCALE_REGISTER_FRACTION * 0x800000)) 

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

uint8_t cs5490_first_calibrate( CS5490 *chip, uint8_t *calibrationData );

double cs5490_convert_to_double( CS5490 *chip, int LSB_pow, int MSB_option );
uint32_t cs5490_convert_to_binary(int LSB_pow, int MSB_option, double input);

//===================================================================================

double cs5490_get_gain_V( CS5490 *chip );
double cs5490_get_gain_I( CS5490 *chip );
void cs5490_set_gain_V( CS5490 *chip, double value );
void cs5490_set_gain_I( CS5490 *chip, double value );

void cs5490_reset( CS5490 *chip );
void cs5490_standby( CS5490 *chip );
void cs5490_wakeup( CS5490 *chip );
void cs5490_single_conversation( CS5490 *chip );
void cs5490_continious_conversation( CS5490 *chip );
void cs5490_halt_conversation( CS5490 *chip );

//===================================================================================

float cs5490_get_DС_Offset_V( CS5490 *chip );
void cs5490_set_DС_Offset_V( CS5490 *chip, float value);
double cs5490_get_DC_Offset_I( CS5490 *chip );
double cs5490_get_AC_Offset_I( CS5490 *chip );
void cs5490_set_DC_Offset_I( CS5490 *chip, double value );
void cs5490_set_AC_Offset_I( CS5490 *chip, double value );
double cs5490_get_Offset_T( CS5490 *chip );
void cs5490_set_Offset_T( CS5490 *chip, double value );

//===================================================================================

double cs5490_get_I( CS5490 *chip );
double cs5490_get_V( CS5490 *chip );
uint32_t cs5490_get_P( CS5490 *chip );

double cs5490_get_Irms( CS5490 *chip );
double cs5490_get_Vrms( CS5490 *chip );
double cs5490_get_PF( CS5490 *chip );

double cs5490_get_Pavg( CS5490 *chip );
double cs5490_get_Qavg( CS5490 *chip );
double cs5490_get_Savg( CS5490 *chip );

uint32_t cs5490_get_total_P( CS5490 *chip );
uint32_t cs5490_get_total_S( CS5490 *chip );
uint32_t cs5490_get_total_Q( CS5490 *chip );

double cs5490_get_freq( CS5490 *chip );

double cs5490_get_time( CS5490 *chip );

double cs5490_get_T( CS5490 *chip );

uint32_t cs5490_get_RegChk( CS5490 *chip );
#endif
