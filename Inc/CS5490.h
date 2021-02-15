/**
!	
*/
#ifndef CS5490_H
#define CS5490_H

#include "stm32l4xx_hal.h"
#include "math.h"
#include "stdint.h"
#include "stdio.h"
#include "M24M01.h"

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
#define READ_OPERATION_FAILURE 0
#define READ_OPERATION_SUCCESS 1

#define SHUNT_OHM 0.0025
#define VOLTAGE_DEVIDER_V 1000
#define REGISTER_FULLSCALE 0.6
#define VOLTAGE_FULLSCALE_MAX 240  
#define VOLTAGE_FULLSCALE (VOLTAGE_FULLSCALE_MAX/VOLTAGE_DEVIDER_V)
#define CURRENT_FULLSCALE_MAX 100  
#define CURRENT_FULLSCALE (CURRENT_FULLSCALE_MAX/SHUNT_OHM)
#define POWER_REGISTER_FULLSCALE 0.36
#define POWER_FULLSCALE ((VOLTAGE_FULLSCALE * CURRENT_FULLSCALE) / (POWER_REGISTER_FULLSCALE))
#define SAMPLE_COUNT_DEFAULT  4000

#define CURRENT_CALIBRATION_REF  0.35
#define SCALE_REGISTER_FRACTION  (0.6 * (CURRENT_CALIBRATION_REF / CURRENT_FULLSCALE)) 
#define SCALE_REGISTER_VALUE ((uint32_t)(SCALE_REGISTER_FRACTION * 0x800000)) //pow(2, 23) = 0x800000

#define MEM_ADDRESS_I_GAIN_L_1 124947
#define MEM_ADDRESS_V_GAIN_L_1 124951
#define MEM_ADDRESS_I_AC_OFF_L_1 124955
#define MEM_ADDRESS_P_OFF_L_1 124959
#define MEM_ADDRESS_PF_L_1 124963
#define MEM_ADDRESS_REGCHECK_L_1 124967

#define MEM_ADDRESS_I_GAIN_L_2 124923
#define MEM_ADDRESS_V_GAIN_L_2 124927
#define MEM_ADDRESS_I_AC_OFF_L_2 124931
#define MEM_ADDRESS_P_OFF_L_2 124935
#define MEM_ADDRESS_PF_L_2 124939
#define MEM_ADDRESS_REGCHECK_L_2 124943

#define MEM_ADDRESS_I_GAIN_L_3 124899
#define MEM_ADDRESS_V_GAIN_L_3 124903
#define MEM_ADDRESS_I_AC_OFF_L_3 124907
#define MEM_ADDRESS_P_OFF_L_3 124911
#define MEM_ADDRESS_PF_L_3 124915
#define MEM_ADDRESS_REGCHECK_L_3 124919

/**
! Структура измерителя

*/
typedef struct CS5490
{
	UART_HandleTypeDef *cs5490_huart;
	uint8_t cs5490_data[3];
	uint8_t cs5490_read_OK;
	float cs5490_MCLK;
} CS5490;

uint8_t cs5490_init( CS5490 *chip );
void cs5490_write( CS5490 *chip, int page, int address, uint32_t value );
void cs5490_read( CS5490 *chip, uint8_t page, uint8_t address );
void cs5490_instruct( CS5490 *chip, int instruction );
uint32_t cs5490_concatData( CS5490 *chip );
uint32_t cs5490_readReg( CS5490 *chip, uint8_t page, uint8_t address );
void cs5490_calibrate( CS5490 *chip, uint8_t type, uint8_t channel );
uint8_t cs5490_full_callibration( CS5490 *chip );
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
double cs5490_get_P( CS5490 *chip );

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

uint32_t cs5490_get_time( CS5490 *chip );

uint32_t cs5490_get_RegChk( CS5490 *chip );
#endif
