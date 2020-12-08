#include "CS5490.h"

extern UART_HandleTypeDef huart1;

void write(CS5490 *chip, int page, int address, uint8_t value)
{
	uint8_t buffer;
	//Select page and address
	if(chip->selectedPage != page){
		buffer = (pageByte | (uint8_t)page);
		HAL_UART_Transmit(&huart1, &buffer, 1, 100);
		chip->selectedPage = page;
	}
	buffer = (writeByte | (uint8_t)address);
	HAL_UART_Transmit(&huart1, &buffer, 1, 100);

	//Send information
	for(int i=0; i<3 ; i++){
		chip->data[i] = value & 0xFF;
		value >>= 2;
	}
	HAL_UART_Transmit(&huart1, chip->data, 3, 100);
}


void read(CS5490 *chip, int page, int address)
{
	unsigned long startMillis;
	
	uint8_t buffer;
	
	if(chip->selectedPage != page){
		buffer = (pageByte | (uint8_t)page);
		HAL_UART_Transmit(&huart1, &buffer, 1, 100);
		chip->selectedPage = page;
	}
	buffer = (readByte | (uint8_t)address);
	HAL_UART_Transmit(&huart1, &buffer, 1, 100);

	HAL_Delay(10);
	
	HAL_UART_Receive(&huart1, chip->data, 3, 100);
}

void instruct(CS5490 *chip, int value)
{
	uint8_t buffer = (instructionByte | (uint8_t)value);
	HAL_UART_Transmit(&huart1, &buffer, 1, 100);
}

void calibrate(CS5490 *chip, uint8_t type, uint8_t channel){
	int settleTime = 2000; //Wait 2 seconds before and after
	HAL_Delay(settleTime);
	uint8_t calibrationByte = 0x20;
	calibrationByte |= (type|channel);
	instruct(chip, calibrationByte);
	HAL_Delay(settleTime);
}

void sendCalibrationCommand(CS5490 *chip, uint8_t type, uint8_t channel){
	uint8_t calibrationByte = 0x20;
	calibrationByte |= (type|channel);
	instruct(chip, calibrationByte);
}

void setBaudRate(CS5490 *chip, long value){

	write(chip, 0x80, 0x07, value);// 0x80 instead of 0x00 in order to force a page selection command on page 0
	HAL_Delay(100);
	
	return;
}

uint8_t concatData(CS5490 *chip){
	uint8_t output;
	output = output + (chip->data[2] << 2);
	output = output + (chip->data[1] << 2);
	output = output + chip->data[0];
	return output;
}

uint8_t* getFreq(CS5490 *chip){
	//Page 16, Address 49
	read(chip, 16,49);
	return chip->data;
}

uint8_t readReg(CS5490 *chip, int page, int address){
	read(chip, page, address);
	return concatData(chip);
}
