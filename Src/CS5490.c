#include "CS5490.h"

void cs5490_write( CS5490 *chip, int page, int address, uint32_t value )
{
	
	HAL_UART_Transmit(chip->cs5490_huart, (uint8_t *) &page, 1, 100);
	chip->cs5490_selectedPage = page;
	HAL_UART_Transmit(chip->cs5490_huart, (uint8_t *) &address, 1, 100);
	
	for ( int i=2; i>=0 ; i-- )
	{
		chip->cs5490_data[i] = value & 0xFF;
		HAL_UART_Transmit(chip->cs5490_huart, &(chip->cs5490_data[i]), 1, 100);  
		value >>= 8;
	}
	
}


void cs5490_read( CS5490 *chip, int page, int address )
{
	
	HAL_UART_Transmit(chip->cs5490_huart, (uint8_t *) &page, 1, 100);
	HAL_UART_Transmit(chip->cs5490_huart, (uint8_t *) &address, 1, 100);
	
	for ( int i=2; i>=0; i-- )
	{
		HAL_UART_Receive(chip->cs5490_huart, &(chip->cs5490_data[i]), 1, 100);
	}
}


void cs5490_instruct( CS5490 *chip, int value )
{
	uint8_t buffer = (INSTRUCTION_BYTE | (uint8_t)value);
	HAL_UART_Transmit(chip->cs5490_huart, &buffer, 1, 100);
}


uint32_t cs5490_concatData( CS5490 *chip )
{
	uint32_t output = 0x00;
	output = (output + chip->cs5490_data[2]) << 8;
	output = (output + chip->cs5490_data[1]) << 8;
	output = output + chip->cs5490_data[0];
	return output;
}


uint32_t cs5490_readReg( CS5490 *chip, int page, int address )
{
	cs5490_read(chip, page, address);
	return cs5490_concatData(chip);
}

void cs5490_calibrate( CS5490 *chip, uint8_t type, uint8_t channel )
{
	HAL_Delay(2000);
	
	uint8_t calibration_byte = CALIBRATION_BYTE;
	calibration_byte |= (type | channel);
	cs5490_instruct(chip, calibration_byte);
	
	HAL_Delay(2000);
}

//===================================================================================

void cs5490_reset( CS5490 *chip )
{
	cs5490_instruct(chip, 1);
}

void cs5490_standby( CS5490 *chip )
{
	cs5490_instruct(chip, 2);
}

void cs5490_wakeUp( CS5490 *chip )
{
	cs5490_instruct(chip, 3);
}

//===================================================================================

uint32_t cs5490_getInstCurrent( CS5490 *chip )
{
	//Page 16, Address 2
	cs5490_read(chip, SOFTWARE_PAGE_BYTE, 0x02);
	return cs5490_concatData(chip);
}

uint32_t cs5490_getInstVoltage( CS5490 *chip )
{
	//Page 16, Address 3
	cs5490_read(chip, SOFTWARE_PAGE_BYTE, 0x03);
	return cs5490_concatData(chip);
}

uint32_t cs5490_getInstPower( CS5490 *chip )
{
	//Page 16, Address 4
	cs5490_read(chip, SOFTWARE_PAGE_BYTE, 0x04);
	return cs5490_concatData(chip);
}

uint32_t cs5490_getAvgPower( CS5490 *chip )
{
	//Page 16, Address 5
	cs5490_read(chip, SOFTWARE_PAGE_BYTE, 0x05);
	return cs5490_concatData(chip);
}

uint32_t cs5490_getAvgQ( CS5490 *chip )
{
	//Page 16, Address 14
	cs5490_read(chip, SOFTWARE_PAGE_BYTE, 0x0E);
	return cs5490_concatData(chip);
}

uint32_t cs5490_getAvgS( CS5490 *chip )
{
	//Page 16, Address 20
	cs5490_read(chip, SOFTWARE_PAGE_BYTE, 0x14);
	return cs5490_concatData(chip);
}

uint32_t cs5490_getTotalPower( CS5490 *chip )
{
	//Page 16, Address 29
	cs5490_read(chip, SOFTWARE_PAGE_BYTE, 0x1D);
	return cs5490_concatData(chip);
}

uint32_t cs5490_getTotalS( CS5490 *chip )
{
	//Page 16, Address 30
	cs5490_read(chip, SOFTWARE_PAGE_BYTE, 0x1E);
	return cs5490_concatData(chip);
}

uint32_t cs5490_getTotalQ( CS5490 *chip )
{
	//Page 16, Address 31
	cs5490_read(chip, SOFTWARE_PAGE_BYTE, 0x1F);
	return cs5490_concatData(chip);
}

uint32_t cs5490_getFreq( CS5490 *chip )
{
	cs5490_read(chip, SOFTWARE_PAGE_BYTE, 0x31);
	return cs5490_concatData(chip);
}

uint32_t cs5490_getTime( CS5490 *chip )
{
	//Page 16, Address 61
	cs5490_read(chip, SOFTWARE_PAGE_BYTE, 0x3D);
	return cs5490_concatData(chip);
}
