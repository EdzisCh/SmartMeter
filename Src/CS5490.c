#include "CS5490.h"

void write(CS5490 chip, int page, int address, uint32_t value)
{
	uint8_t buffer;
	
	if(chip.selectedPage != page)
	{
		buffer = (pageByte | (uint8_t)page);
		HAL_UART_Transmit(&(chip.huart), &buffer, 1, 100);
		chip.selectedPage = page;
	}
	buffer = (writeByte | (uint8_t)address);
	HAL_UART_Transmit(&(chip.huart), &buffer, 1, 100);

	
	for(int i=2; i>=0 ; i--)
	{
		chip.data[i] = value & 0xFF;
		HAL_UART_Transmit(&(chip.huart), (uint8_t *) &(chip.data[i]), 1, 100);  
		value >>= 8;
	}
	
}


void read(CS5490 chip, int page, int address)
{
	uint8_t buffer;
	
	if(chip.selectedPage != page)
	{
		buffer = (pageByte | (uint8_t)page);
		HAL_UART_Transmit(&(chip.huart), &buffer, 1, 100);
		chip.selectedPage = page;
	}
	buffer = (readByte | (uint8_t)address);
	HAL_UART_Transmit(&(chip.huart), &buffer, 1, 100);

	HAL_Delay(10);
	
	for(int i=2; i>=0; i++)
	{
		if(HAL_UART_Receive(&(chip.huart), (uint8_t *) &(chip.data[i]), 1, 100) != HAL_OK)
		{
			HAL_GPIO_TogglePin(LED_REACT_GPIO_Port, LED_REACT_Pin);
			HAL_GPIO_TogglePin(LED_ACT_GPIO_Port, LED_ACT_Pin);
		}
	}
}


void instruct(CS5490 chip, int value)
{
	uint8_t buffer = (instructionByte | (uint8_t)value);
	HAL_UART_Transmit(&(chip.huart), &buffer, 1, 100);
}


uint32_t concatData(CS5490 chip)
{
	uint8_t output;
	output = (output + chip.data[0]) << 8;
	output = (output + chip.data[1]) << 8;
	output = output + chip.data[2];
	return output;
}


uint32_t readReg(CS5490 chip, int page, int address)
{
	read(chip, page, address);
	return concatData(chip);
}

 /*Instructions*/

void reset(CS5490 chip){
	instruct(chip, 1);
}

void standby(CS5490 chip){
	instruct(chip, 2);
}

void wakeUp(CS5490 chip){
	instruct(chip, 3);
}

		/*MEASURMENT*/

uint32_t getPeakV(CS5490 chip){
	//Page 0, Address 36
	read(chip, 0,36);
	return concatData(chip);
}

uint32_t getPeakI(CS5490 chip){
	//Page 0, Address 37
	read(chip, 0,37);
	return concatData(chip);
}

uint32_t getInstI(CS5490 chip){
	//Page 16, Address 2
	read(chip, 16,2);
	return concatData(chip);
}

uint32_t getInstV(CS5490 chip){
	//Page 16, Address 3
	read(chip, 16,3);
	return concatData(chip);
}

uint32_t getInstP(CS5490 chip){
	//Page 16, Address 4
	read(chip, 16,4);
	return concatData(chip);
}

uint32_t getRmsI(CS5490 chip){
	//Page 16, Address 6
	read(chip, 16,6);
	return concatData(chip);
}

uint32_t getRmsV(CS5490 chip){
	//Page 16, Address 7
	read(chip, 16,7);
	return concatData(chip);
}

uint32_t getAvgP(CS5490 chip){
	//Page 16, Address 5
	read(chip, 16,5);
	return concatData(chip);
}

uint32_t getAvgQ(CS5490 chip){
	//Page 16, Address 14
	read(chip, 16,14);
	return concatData(chip);
}

uint32_t getAvgS(CS5490 chip){
	//Page 16, Address 20
	read(chip, 16,20);
	return concatData(chip);
}

uint32_t getInstQ(CS5490 chip){
	//Page 16, Address 15
	read(chip, 16,15);
	return concatData(chip);
}

uint32_t getPF(CS5490 chip){
	//Page 16, Address 21
	read(chip, 16,21);
	return concatData(chip);
}

uint32_t getTotalP(CS5490 chip){
	//Page 16, Address 29
	read(chip, 16,29);
	return concatData(chip);
}

uint32_t getTotalS(CS5490 chip){
	//Page 16, Address 30
	read(chip, 16,30);
	return concatData(chip);
}

uint32_t getTotalQ(CS5490 chip){
	//Page 16, Address 31
	read(chip, 16,31);
	return concatData(chip);
}

uint32_t getFreq(CS5490 chip)
{
	read(chip, 16,49);
	return concatData(chip);
}

uint32_t getTime(CS5490 chip){
	//Page 16, Address 61
	read(chip, 16,61);
	return concatData(chip);
}

