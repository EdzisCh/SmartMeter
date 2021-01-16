#include "lcd.h"

uint8_t display_init( void )
{
	
	uint8_t instruction = CMD_IVA | (CMD_IVA_DA_MASK & 7) | CMD_IVA_DE1_VE1;
	
	HAL_Delay(20);
	
	HAL_I2C_Master_Transmit(&hi2c2, LCD_ADDRESS, &instruction, 1, 100);
	
	instruction = CMD_MODE_SET | CMD_MODE_BIAS13 | CMD_DISP_ON_ENA_CLK | CMD_FRAME_FREQ_80HZ;
	
	HAL_I2C_Master_Transmit(&hi2c2, LCD_ADDRESS, &instruction, 1, 100);
	
	HAL_Delay(20);
	
	return 0;
}


uint8_t display_all_data_write( void )
{
	uint16_t address = 0x00;
	HAL_StatusTypeDef returnStatus;
	
	uint8_t data[21];
	uint8_t i = 0;
	
	for ( ; i < 21; i++ )
	{
		data[i] = 0xFF;
	}
	
	returnStatus = HAL_I2C_Mem_Write(&hi2c2, LCD_ADDRESS, address, 1, data, 21, 255);
	
	return (uint8_t) returnStatus;
}

uint8_t display_clear( void )
{	
	uint16_t address = 0x00;
	HAL_StatusTypeDef returnStatus;
	
	uint8_t clearData[21];
	uint8_t i = 0;
	
	for ( ; i < 21; i++ )
	{
		clearData[i] = 0x00;
	}
	
	returnStatus = HAL_I2C_Mem_Write(&hi2c2, LCD_ADDRESS, address, 1, clearData, 21, 255);
	
	return (uint8_t) returnStatus;
}

uint8_t display_data_write(uint16_t address, uint8_t *data, uint8_t sizeOfData)
{
	HAL_StatusTypeDef returnStatus;
	
	if ( address > 21 || sizeOfData > 21 )
	{
		return (uint8_t) HAL_ERROR;
	}

	returnStatus = HAL_I2C_Mem_Write(&hi2c2, LCD_ADDRESS, address, 1, data, sizeOfData, 255);
	
	return (uint8_t) returnStatus;
}
