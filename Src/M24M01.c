#include "M24M01.h"

uint32_t current_address_of_day_retrosective = 0x0000;
uint32_t current_address_of_month_retrosective = 0x0C00; //3072
uint32_t current_address_of_year_retrosective = 0x0FC0;

/**
!Запись данных в EEPROM
Возвращаемое значение: от 0х00 до 0х03 см. HAL_StatusTypeDef, 0x04 - выход за 
рамки значений

*/
uint8_t m24m01_save_to_mem( uint32_t address, uint8_t* data, uint8_t size_of_data )
{
	uint8_t return_status = 0x00;
	
	if(address >= MAX_MEM_ADDRESS || size_of_data > MAX_BYTES_SIZE_TO_RECEIVE)
	{
		return 0x04;
	}
	
	while(HAL_I2C_IsDeviceReady(&hi2c3, M24M01_DEVICE_ADDRESS, 10, 300) == HAL_TIMEOUT);
	
	return_status = HAL_I2C_Mem_Write(&hi2c3, M24M01_DEVICE_ADDRESS, address, I2C_MEMADD_SIZE_16BIT, data, size_of_data, HAL_MAX_DELAY);

	return return_status;
}

/**
!Чтение данных из EEPROM
Возвращаемое значение: от 0х00 до 0х03 см. HAL_StatusTypeDef, 0x04 - выход за 
рамки значений
*/
uint8_t m24m01_get_from_mem( uint32_t address, uint8_t* data, uint8_t size_of_data )
{
	uint8_t return_status = 0x00;
	
	if(address >= MAX_MEM_ADDRESS || size_of_data > MAX_BYTES_SIZE_TO_RECEIVE)
	{
		return 0x04;
	}
	
	while(hi2c3.State != HAL_I2C_STATE_READY);
	while(HAL_I2C_IsDeviceReady(&hi2c3, M24M01_DEVICE_ADDRESS, 10, 1000) == HAL_TIMEOUT);
	
	HAL_Delay(750);
	return_status = HAL_I2C_Mem_Read(&hi2c3, M24M01_DEVICE_ADDRESS, address, I2C_MEMADD_SIZE_16BIT, data, size_of_data, HAL_MAX_DELAY);
	
	return return_status;
}
