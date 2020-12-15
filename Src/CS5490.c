#include "CS5490.h"

void write(CS5490 *chip, int page, int address, uint32_t value)
{
	
	HAL_UART_Transmit(chip->huart, (uint8_t *) &page, 1, 100);
	chip->selectedPage = page;
	HAL_UART_Transmit(chip->huart, (uint8_t *) &address, 1, 100);

	
	for(int i=2; i>=0 ; i--)
	{
		chip->data[i] = value & 0xFF;
		HAL_UART_Transmit(chip->huart, &(chip->data[i]), 1, 100);  
		value >>= 8;
	}
	
}


void read(CS5490 *chip, int page, int address)
{
	
	HAL_UART_Transmit(chip->huart, (uint8_t *) &page, 1, 100);
	HAL_UART_Transmit(chip->huart, (uint8_t *) &address, 1, 100);
	
	for(int i=2; i>=0; i--)
	{
		HAL_UART_Receive(chip->huart, &(chip->data[i]), 1, 100);
	}
}


void instruct(CS5490 *chip, int value)
{
	uint8_t buffer = (instructionByte | (uint8_t)value);
	HAL_UART_Transmit(chip->huart, &buffer, 1, 100);
}


uint32_t concatData(CS5490 *chip)
{
	uint32_t output;
	output = (output + chip->data[2]) << 8;
	output = (output + chip->data[1]) << 8;
	output = output + chip->data[0];
	return output;
}


uint32_t readReg(CS5490 *chip, int page, int address)
{
	read(chip, page, address);
	return concatData(chip);
}

 /*Instructions*/

void reset(CS5490 *chip){
	instruct(chip, 1);
}

void standby(CS5490 *chip){
	instruct(chip, 2);
}

void wakeUp(CS5490 *chip){
	instruct(chip, 3);
}

		/*MEASURMENT*/

uint32_t getPeakV(CS5490 *chip){
	//Page 0, Address 36
	read(chip, hardwarePageByte, 0x24);
	return concatData(chip);
}

uint32_t getPeakI(CS5490 *chip){
	//Page 0, Address 37
	read(chip, hardwarePageByte, 0x25);
	return concatData(chip);
}

uint32_t getInstI(CS5490 *chip){
	//Page 16, Address 2
	read(chip, softwarePageByte, 0x02);
	return concatData(chip);
}

uint32_t getInstV(CS5490 *chip){
	//Page 16, Address 3
	read(chip, softwarePageByte, 0x03);
	return concatData(chip);
}

uint32_t getInstP(CS5490 *chip){
	//Page 16, Address 4
	read(chip, softwarePageByte, 0x04);
	return concatData(chip);
}

uint32_t getRmsI(CS5490 *chip){
	//Page 16, Address 6
	read(chip, softwarePageByte, 0x06);
	return concatData(chip);
}

uint32_t getRmsV(CS5490 *chip){
	//Page 16, Address 7
	read(chip, softwarePageByte, 0x07);
	return concatData(chip);
}

uint32_t getAvgP(CS5490 *chip){
	//Page 16, Address 5
	read(chip, softwarePageByte, 0x05);
	return concatData(chip);
}

uint32_t getAvgQ(CS5490 *chip){
	//Page 16, Address 14
	read(chip, softwarePageByte, 0x0E);
	return concatData(chip);
}

uint32_t getAvgS(CS5490 *chip){
	//Page 16, Address 20
	read(chip, softwarePageByte, 0x14);
	return concatData(chip);
}

uint32_t getInstQ(CS5490 *chip){
	//Page 16, Address 15
	read(chip, softwarePageByte, 0x0F);
	return concatData(chip);
}

uint32_t getPF(CS5490 *chip){
	//Page 16, Address 21
	read(chip, softwarePageByte, 0x15);
	return concatData(chip);
}

uint32_t getTotalP(CS5490 *chip){
	//Page 16, Address 29
	read(chip, softwarePageByte, 0x1D);
	return concatData(chip);
}

uint32_t getTotalS(CS5490 *chip){
	//Page 16, Address 30
	read(chip, softwarePageByte, 0x1E);
	return concatData(chip);
}

uint32_t getTotalQ(CS5490 *chip){
	//Page 16, Address 31
	read(chip, softwarePageByte, 0x1F);
	return concatData(chip);
}

uint32_t getFreq(CS5490 *chip)
{
	read(chip, softwarePageByte, 0x31);
	return concatData(chip);
}

uint32_t getTime(CS5490 *chip){
	//Page 16, Address 61
	read(chip, softwarePageByte, 0x3D);
	return concatData(chip);
}
