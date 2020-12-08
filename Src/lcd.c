#include "lcd.h"


extern I2C_HandleTypeDef hi2c2;

uint8_t lcd_init(void)
{
	uint8_t lcd_bias_set[2] = {0x82, 0x00};
	uint8_t frame_seq_set[2] = {0x86, 0x00};
	uint8_t blink_set[2] = {0x88, 0x00};
	uint8_t vlcd_set[2] = {0x8A, 0x28};
	uint8_t int_sys_clk_set[2] = {0x84, 0x03};
	
	/*Set LCD bias and duty*/
	if(HAL_I2C_Master_Transmit(&hi2c2, LCD_ADDRESS, lcd_bias_set, 2, 100) != HAL_OK)
	{
		return 1;
	}
	
	/*Set frame frequency*/
	if(HAL_I2C_Master_Transmit(&hi2c2, LCD_ADDRESS, frame_seq_set, 2, 100) != HAL_OK)
	{
		return 1;
	}
	
	/*Segment/VLCD shared pin*/
	if(HAL_I2C_Master_Transmit(&hi2c2, LCD_ADDRESS, vlcd_set, 2, 100) != HAL_OK)
	{
		return 1;
	}
		
	/*LCD blinking frequency*/
	if(HAL_I2C_Master_Transmit(&hi2c2, LCD_ADDRESS, blink_set, 2, 100) != HAL_OK)
	{
		return 1;
	}
	
	if(HAL_I2C_Master_Transmit(&hi2c2, LCD_ADDRESS, int_sys_clk_set, 2, 100) != HAL_OK)
	{
		return 1;
	}
	
	return 0;
}


uint8_t display_data_write(void)
{
	uint8_t data[21] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
											0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
											0xFF, 0xFF, 0xFF, 0xFF, 0x0F};
	
	//uint8_t int_sys_clk_set[2] = {0x84, 0x03};
	uint16_t address = 0x00;
	
	/*Display RAM single data byte*/
	//HAL_I2C_Master_Transmit(&hi2c2, LCD_ADDRESS, data, 3, 100);
	

	if(HAL_I2C_Mem_Write(&hi2c2, LCD_ADDRESS, address, 1, data, 15, 1) != HAL_OK)
	{
		return 1;
	}
	
	/*Enable internal system clock*/
	//HAL_I2C_Master_Transmit(&hi2c2, LCD_ADDRESS, int_sys_clk_set, 2, 100);
	
	return 0;
}

