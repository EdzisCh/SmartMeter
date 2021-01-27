#include "lcd.h"

	/**
	!Инициализация дисплея. Установка значений для Internal Voltage adjustment, 
	bias, frame freq и включаем клокирование

	*/
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

	/**
	! Включение всех элементов дисплея

	*/
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

/**
!Полная очистка дисплея

*/
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

	/**
!Запись данных в паямять ЖКИ по адресу address количеством sizeOfData

	*/
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

	/**
	!Чтение одного байта из памяти ЖКИ по адресу address

	*/
uint8_t display_byte_read( uint16_t address )
{
	uint8_t byte;
	HAL_I2C_Mem_Read(&hi2c2, LCD_ADDRESS, address, 1, &byte, 1, 255);
	
	return byte;
}

/**
! Вывод числа на основное поле дисплее (8 сегментов)
*/
void display_main_numbers(uint32_t number)
{
	uint8_t value_1 = 0xD0;
	uint8_t value_2 = 0x07;
	HAL_I2C_Mem_Write(&hi2c2, LCD_ADDRESS, 0x01, 1, &value_1, 1, 255);
	HAL_I2C_Mem_Write(&hi2c2, LCD_ADDRESS, 0x02, 1, &value_2, 1, 255);
}

//===================================================================================

// Далее ряд функций для включения определенных элементов ЖКИ

void display_L1( void )
{
	uint8_t value = 128;
	
	uint8_t current_value = display_byte_read( 0 );
	
	value |= current_value;
	
	HAL_I2C_Mem_Write(&hi2c2, LCD_ADDRESS, 0x00, 1, &value, 1, 255);
}

void display_L2( void )
{
	uint8_t value = 64;
	
	uint8_t current_value = display_byte_read( 0 );
	
	value |= current_value;
	
	HAL_I2C_Mem_Write(&hi2c2, LCD_ADDRESS, 0, 1, &value, 1, 255);
}

void display_L3( void )
{
	uint8_t value = 32;
	
	uint8_t current_value = display_byte_read( 0 );
	
	value |= current_value;
	
	HAL_I2C_Mem_Write(&hi2c2, LCD_ADDRESS, 0, 1, &value, 1, 255);
}

void display_R1( void )
{
	uint8_t value = 4;
	
	uint8_t current_value = display_byte_read(1);
	
	value |= current_value;
	
	HAL_I2C_Mem_Write(&hi2c2, LCD_ADDRESS, 1, 1, &value, 1, 255);
}

void display_R2( void )
{
	uint8_t value = 8;
	
	uint8_t current_value = display_byte_read(1);
	
	value |= current_value;
	
	HAL_I2C_Mem_Write(&hi2c2, LCD_ADDRESS, 1, 1, &value, 1, 255);
}

void display_P( void )
{
	uint8_t value = 1;
	
	uint8_t current_value = display_byte_read(1);
	
	value |= current_value;
	
	HAL_I2C_Mem_Write(&hi2c2, LCD_ADDRESS, 0x01, 1, &value, 1, 255);
}

void display_N( void )
{
	uint8_t value = 2;
	
	uint8_t current_value = display_byte_read( 1 );
	
	value |= current_value;
	
	HAL_I2C_Mem_Write(&hi2c2, LCD_ADDRESS, 0x01, 1, &value, 1, 255);
}

void display_battery( void )
{
	uint8_t value = 8;
	
	uint8_t current_value = display_byte_read( 19 );
	
	value |= current_value;
	
	HAL_I2C_Mem_Write(&hi2c2, LCD_ADDRESS, 19, 1, &value, 1, 255);
}

void display_ExMark( void )
{
	uint8_t value = 8;
	
	uint8_t current_value = display_byte_read( 20 );
	
	value |= current_value;
	
	HAL_I2C_Mem_Write(&hi2c2, LCD_ADDRESS, 20, 1, &value, 1, 255);
}

void display_lock_1( void )
{
	uint8_t value = 4;
	
	uint8_t current_value = display_byte_read( 20);
	
	value |= current_value;
	
	HAL_I2C_Mem_Write(&hi2c2, LCD_ADDRESS, 20, 1, &value, 1, 255);
}

void display_lock_2( void )
{
	uint8_t value = 2;
	
	uint8_t current_value = display_byte_read( 20 );
	
	value |= current_value;
	
	HAL_I2C_Mem_Write(&hi2c2, LCD_ADDRESS, 20, 1, &value, 1, 255);
}

void display_level( uint8_t level )
{
	uint8_t value = 0x00;
	
	uint8_t current_value = display_byte_read( 0 );
	
	value |= current_value;
	
	switch(level)
	{
		case 0:
			HAL_I2C_Mem_Write(&hi2c2, LCD_ADDRESS, 0, 1, &value, 1, 255);
			break;
		case 0x01:
			value = 16;
			value |= current_value;
			HAL_I2C_Mem_Write(&hi2c2, LCD_ADDRESS, 0, 1, &value, 1, 255);	
			break;
		case 0x02:
			value = 17;
			value |= current_value;
			HAL_I2C_Mem_Write(&hi2c2, LCD_ADDRESS, 0, 1, &value, 1, 255);
			break;
		case 0x03:
			value = 19;
			value |= current_value;
			HAL_I2C_Mem_Write(&hi2c2, LCD_ADDRESS, 0, 1, &value, 1, 255);
			break;
		case 0x04:
			value = 23;
			value |= current_value;
			HAL_I2C_Mem_Write(&hi2c2, LCD_ADDRESS, 0, 1, &value, 1, 255);
			break;
		case 0x05:
			value = 31;
			value |= current_value;
			HAL_I2C_Mem_Write(&hi2c2, LCD_ADDRESS, 0, 1, &value, 1, 255);
			break;
		
		default:
			value = 0x00;
			value |= current_value;
			HAL_I2C_Mem_Write(&hi2c2, LCD_ADDRESS, 0, 1, &value, 1, 255);
			break;
	}
}

void display_WIFI( void )
{
	uint8_t value = 128;
		
	uint8_t current_value = display_byte_read( 6 );
	
	value |= current_value;
	
	HAL_I2C_Mem_Write(&hi2c2, LCD_ADDRESS, 6, 1, &value, 1, 255);
}

void display_GR( void )
{
	uint8_t value = 1;
		
	uint8_t current_value = display_byte_read( 11 );
	
	value |= current_value;
	
	HAL_I2C_Mem_Write(&hi2c2, LCD_ADDRESS, 11, 1, &value, 1, 255);
}

void display_PR( void )
{
	uint8_t value = 1;
		
	uint8_t current_value = display_byte_read( 10 );
	
	value |= current_value;
	
	HAL_I2C_Mem_Write(&hi2c2, LCD_ADDRESS, 10, 1, &value, 1, 255);
}

void display_T( void )
{
	uint8_t value = 1;
		
	uint8_t current_value = display_byte_read( 8 );
	
	value |= current_value;
	
	HAL_I2C_Mem_Write(&hi2c2, LCD_ADDRESS, 8, 1, &value, 1, 255);
}
