#include "CS5490.h"

extern UART_HandleTypeDef huart1;

void write(CS5490 chip, int page, int address, uint32_t value)
{
	uint8_t buffer;
	//Select page and address
	if(chip.selectedPage != page){
		buffer = (pageByte | (uint8_t)page);
		HAL_UART_Transmit(&huart1, &buffer, 1, 100);
		chip.selectedPage = page;
	}
	buffer = (writeByte | (uint8_t)address);
	HAL_UART_Transmit(&huart1, &buffer, 1, 100);

	//Send information
	for(int i=0; i<3 ; i++){
		chip.data[i] = value & 0xFF;
		HAL_UART_Transmit(&huart1, (uint8_t *) &(chip.data[i]), 1, 100);  
		value >>= 8;
	}
	
}


void read(CS5490 chip, int page, int address)
{
	uint8_t buffer;
	
	if(chip.selectedPage != page){
		buffer = (pageByte | (uint8_t)page);
		HAL_UART_Transmit(&huart1, &buffer, 1, 100);
		chip.selectedPage = page;
	}
	buffer = (readByte | (uint8_t)address);
	HAL_UART_Transmit(&huart1, &buffer, 1, 100);

	HAL_Delay(10);
	
	
	for(int i=2; i>=0; i++){
		if(HAL_UART_Receive(&huart1, (uint8_t *) &(chip.data[i]), 1, 100) != HAL_OK)
		{
			HAL_GPIO_TogglePin(LED_REACT_GPIO_Port, LED_REACT_Pin);
			HAL_GPIO_TogglePin(LED_ACT_GPIO_Port, LED_ACT_Pin);
		}
	}
}


void instruct(CS5490 chip, int value)
{
	uint8_t buffer = (instructionByte | (uint8_t)value);
	HAL_UART_Transmit(&huart1, &buffer, 1, 100);
}


uint32_t concatData(CS5490 chip){
	uint8_t output;
	output = (output + chip.data[0]) << 8;
	output = (output + chip.data[1]) << 8;
	output = output + chip.data[2];
	return output;
}


uint32_t getFreq(CS5490 chip){
	//Page 16, Address 49
	read(chip, 16,49);
	return concatData(chip);
}


uint32_t readReg(CS5490 chip, int page, int address){
	read(chip, page, address);
	return concatData(chip);
}
