#include "lcd.h"
#include "stdio.h"

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
!Очистка цифр основного поля (8 больших сегментов)

*/
void display_clear_main_numbers( void )
{
	uint8_t clear_byte = 0x00;
	uint8_t current_data = 0x00;
	
	for(uint8_t i = 0x02; i < 0x05; i++)
	{
		HAL_I2C_Mem_Write(&hi2c2, LCD_ADDRESS, i, 1, &clear_byte, 1, 255);
	}
	for(uint8_t i = 0x10; i < 0x13; i++)
	{
		HAL_I2C_Mem_Write(&hi2c2, LCD_ADDRESS, i, 1, &clear_byte, 1, 255);
	}
	
	//очистка 1 сегмента с сохранением символа батареи, если есть
	HAL_I2C_Mem_Read(&hi2c2, LCD_ADDRESS, 0x13, 1, &current_data, 1, 255);
	
	clear_byte = current_data & 0x08;
	HAL_I2C_Mem_Write(&hi2c2, LCD_ADDRESS, 0x13, 1, &clear_byte, 1, 255);
	
	//очистка 5 сегмента с сохранением символов P N R1 R2, если есть
	HAL_I2C_Mem_Read(&hi2c2, LCD_ADDRESS, 0x01, 1, &current_data, 1, 255);
	
	clear_byte = current_data & 0x0F;
	HAL_I2C_Mem_Write(&hi2c2, LCD_ADDRESS, 0x01, 1, &clear_byte, 1, 255);
	
	//очистка 1 сегмента с сохранением символов единиц измерения, если есть
	HAL_I2C_Mem_Read(&hi2c2, LCD_ADDRESS, 0x05, 1, &current_data, 1, 255);
	
	clear_byte = current_data & 0xF8;
	HAL_I2C_Mem_Write(&hi2c2, LCD_ADDRESS, 0x05, 1, &clear_byte, 1, 255);
}

/**
! Вывод числа на основное поле дисплее (8 сегментов)
На вход принимается число (не более 8 разрядов), парсится для 
получения массива цифр в обратном порядке, размер массива на 1 больше, т.к.
последний байт - порядковый номер плавающей запятой (либо 0х00, если 
запятая отсутствует)
*/
void display_main_numbers(uint32_t number, uint8_t count, uint8_t dot_addr )
{
	if(count > 8 || count < 1)
	{
		return;
	}

	uint8_t num_buff[count + 1];
	//count + 1 = кол-во задействованых сегментов + 1 (номер точки, если есть)
	
	display_parse_number( number, num_buff, count);

	num_buff[count] = dot_addr; 
	
	display_clear_main_numbers();

	uint8_t current_number;
	uint8_t address;
	
	for(uint8_t i = 0x00; i < count; i++)
	{
		current_number = num_buff[i];
		
		//рассчеn позиции (адрес байта в памяти ЖКИ) 
		address = 0x08 - i;
		switch(address)
		{
			case 0x08:
				address = 0x05;
				break;
			case 0x07:
				address = 0x04;
				break;
			case 0x06:
				address = 0x03;
				break;
			case 0x05:
				address = 0x02;
				break;
			case 0x04:
				address = 0x10;
				break;
			case 0x03:
				address = 0x11;
				break;
			case 0x02:
				address = 0x12;
				break;
			case 0x01:
				address = 0x13;
				break;
			
			default:
				break;
		}
		
		//вызов функции, которая вписывает байт, пропущенный через маску, в определенный адрес
		display_write_one_number(current_number, address);
		
		//если есть запятая - выводим
		if(num_buff[count] > 0)
		{
			switch(num_buff[count])
			{
				case 1:
					display_write_one_number(0x10, 0x10);
					break;
				case 2:
					display_write_one_number(0x10, 0x02);
					break;
				case 3:
					display_write_one_number(0x10, 0x03);
					break;
				case 4:
					display_write_one_number(0x10, 0x04);
					break;
				
				default:
					break;
			}
		}
	}

}

/*
!Аналог функции display_main_numbers(). Преобразует входящий double в формат, удобный для вывода
*/
void display_main_numbers_double( double number )
{
	uint32_t transform_number = 0;
	uint8_t dot_addr = 0;
	uint8_t count_of_numbers = 0;
	
	//преобразование
	uint32_t integer_part = (uint32_t) number;
	uint32_t fract_part = (uint32_t) ((number - integer_part) * 10000);

	uint32_t temp_integer_part = integer_part;
	uint32_t temp_fract_part = fract_part;
	
	//узнаем количесво знаков в целой части
	while(temp_integer_part > 0)
	{
		temp_integer_part /= 10;
		count_of_numbers++;
	}
	if(count_of_numbers == 0)
	{
		count_of_numbers = 1;
	}
	
	dot_addr = count_of_numbers;
	uint8_t count_of_fractal_part = (8 - count_of_numbers);
	if(count_of_fractal_part > 4) count_of_fractal_part = 4;
	count_of_numbers += count_of_fractal_part;
	
	//удаляем лишние знаки и 0 в конце дробной части
	uint8_t temp_count_of_fractal_part = count_of_fractal_part;
	while(temp_count_of_fractal_part > 4)
	{
		temp_fract_part /= 10;
		temp_count_of_fractal_part--;
	}
	
	//избавляемся от лишних нулей
	uint8_t flag = 0;
	do
	{
		flag = temp_fract_part % 10;
		if(flag) break;
		temp_fract_part /= 10;
		count_of_fractal_part--;
		count_of_numbers--;
	}while(flag == 0 && count_of_fractal_part > 0);
	
	dot_addr = 5 - count_of_fractal_part;
	
	if(dot_addr == 5)
	{
		dot_addr = 0;
	}
	fract_part = temp_fract_part;
	
	for(uint8_t i = 0; i < count_of_fractal_part; i++)
	{
		integer_part *= 10;
	}
	integer_part += fract_part;
	
	transform_number = integer_part;
	
	display_main_numbers(transform_number, count_of_numbers, dot_addr);
}

/**
!Вывод заданной цифры в заданный сегмент
*/
void display_write_one_number( uint8_t number, uint8_t address )
{	
	uint8_t data_byte_msb, data_byte_lsb;
	uint8_t number_mask_msb, number_mask_lsb;
	
	switch(number)
	{
		case 0x00:
			number_mask_msb = MAIN_NUMBER_0_MSB;
			number_mask_lsb = MAIN_NUMBER_0_LSB;
			break;
		case 0x01:
			number_mask_msb = MAIN_NUMBER_1_MSB;
			number_mask_lsb = MAIN_NUMBER_1_LSB;
			break;		
		case 0x02:
			number_mask_msb = MAIN_NUMBER_2_MSB;
			number_mask_lsb = MAIN_NUMBER_2_LSB;
			break;
		case 0x03:
			number_mask_msb = MAIN_NUMBER_3_MSB;
			number_mask_lsb = MAIN_NUMBER_3_LSB;
			break;
		case 0x04:
			number_mask_msb = MAIN_NUMBER_4_MSB;
			number_mask_lsb = MAIN_NUMBER_4_LSB;
			break;
		case 0x05:
			number_mask_msb = MAIN_NUMBER_5_MSB;
			number_mask_lsb = MAIN_NUMBER_5_LSB;
			break;
		case 0x06:
			number_mask_msb = MAIN_NUMBER_6_MSB;
			number_mask_lsb = MAIN_NUMBER_6_LSB;
			break;
		case 0x07:
			number_mask_msb = MAIN_NUMBER_7_MSB;
			number_mask_lsb = MAIN_NUMBER_7_LSB;
			break;
		case 0x08:
			number_mask_msb = MAIN_NUMBER_8_MSB;
			number_mask_lsb = MAIN_NUMBER_8_LSB;
			break;
		case 0x09:
			number_mask_msb = MAIN_NUMBER_9_MSB;
			number_mask_lsb = MAIN_NUMBER_9_LSB;
			break;
		case 0x10:
			number_mask_msb = MAIN_DOT_MSB;
			number_mask_lsb = MAIN_DOT_LSB;
		
		default:
			break;
	}
	
	if (address <= 0x05 && address >= 0x02)
	{
		HAL_I2C_Mem_Read(&hi2c2, LCD_ADDRESS, address, 1, &data_byte_msb, 1, 255);
		HAL_I2C_Mem_Read(&hi2c2, LCD_ADDRESS, address - 1, 1, &data_byte_lsb, 1, 255);

		data_byte_msb |= number_mask_msb;
		data_byte_lsb |= number_mask_lsb;

		HAL_I2C_Mem_Write(&hi2c2, LCD_ADDRESS, address, 1, &data_byte_msb, 1, 255);
		HAL_I2C_Mem_Write(&hi2c2, LCD_ADDRESS, address - 1, 1, &data_byte_lsb, 1, 255);
		
	} else if (address >= 0x10 && address <= 0x13) {
		
		uint8_t msb_numbers_mask = number_mask_msb | number_mask_lsb;
		uint8_t data_byte;
		
		HAL_I2C_Mem_Read(&hi2c2, LCD_ADDRESS, address, 1, &data_byte, 1, 255);

		data_byte |= msb_numbers_mask;

		HAL_I2C_Mem_Write(&hi2c2, LCD_ADDRESS, address, 1, &data_byte, 1, 255);
	}
}

/**
!Парсинг целых чисел. Из числа получаем массив (причем запись в массив ведется с конца
123 -> [3, 2, 1])
*/
void display_parse_number( uint32_t input, uint8_t *numbers, uint8_t count )
{
	for (uint8_t i = 0; i < count; i++)
	{	
		numbers[i] = input % 10;
		input /= 10;
	}
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

void display_WIFI_on( void )
{
	uint8_t value = 128;
		
	uint8_t current_value = display_byte_read( 6 );
	
	value |= current_value;
	
	HAL_I2C_Mem_Write(&hi2c2, LCD_ADDRESS, 6, 1, &value, 1, 255);
}

void display_WIFI_off( void )
{
	uint8_t value = 0x7F;
		
	uint8_t current_value = display_byte_read( 6 );
	
	value &= current_value;
	
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

//===================================================================================

/**
!Функции для включения единиц измерения
*/
void display_clear_units( void )
{
	uint8_t value = 0x07;
	
	uint8_t current_value = display_byte_read( 5 );
	value &= current_value;
	HAL_I2C_Mem_Write(&hi2c2, LCD_ADDRESS, 5, 1, &value, 1, 255);
	
	value = 0xF0;
    current_value = display_byte_read( 6 );
	value &= current_value;
	HAL_I2C_Mem_Write(&hi2c2, LCD_ADDRESS, 6, 1, &value, 1, 255);
}

void display_V( void )
{
	display_clear_units();
	
	uint8_t value = 0x20;
	uint8_t current_value = display_byte_read( 5 );
	value |= current_value;
	
	HAL_I2C_Mem_Write(&hi2c2, LCD_ADDRESS, 5, 1, &value, 1, 255);
}

void display_W( void )
{
	display_clear_units();
	
	uint8_t value = 0x60;

	uint8_t current_value = display_byte_read( 5 );
	value |= current_value;
	
	HAL_I2C_Mem_Write(&hi2c2, LCD_ADDRESS, 5, 1, &value, 1, 255);
}

void display_W_h( uint8_t pos )
{
	display_clear_units();
	
	uint8_t value = 0x60;
	
	uint8_t current_value = display_byte_read( 5 );
	value |= current_value;
	
	HAL_I2C_Mem_Write(&hi2c2, LCD_ADDRESS, 5, 1, &value, 1, 255);
	
	value = 0x40;
	
	current_value = display_byte_read( 6 );
	value |= current_value;
	
	HAL_I2C_Mem_Write(&hi2c2, LCD_ADDRESS, 6, 1, &value, 1, 255);
}

void display_VAr( void )
{
	display_clear_units();
	
	uint8_t value = 0x20;
	
	uint8_t current_value = display_byte_read( 5 );
	value |= current_value;
	
	HAL_I2C_Mem_Write(&hi2c2, LCD_ADDRESS, 5, 1, &value, 1, 255);
	
	value = 0x03;
	
	current_value = display_byte_read( 6 );
	value |= current_value;
	
	HAL_I2C_Mem_Write(&hi2c2, LCD_ADDRESS, 6, 1, &value, 1, 255);
}

void display_A( void )
{
	display_clear_units();
	
	uint8_t value = 0x01;
	
	uint8_t current_value = display_byte_read( 6 );
	value |= current_value;
	
	HAL_I2C_Mem_Write(&hi2c2, LCD_ADDRESS, 6, 1, &value, 1, 255);
}

void display_VAr_h( uint8_t pos )
{
	display_clear_units();
	
	uint8_t value = 0x20;
	
	uint8_t current_value = display_byte_read( 5 );
	value |= current_value;
	
	HAL_I2C_Mem_Write(&hi2c2, LCD_ADDRESS, 5, 1, &value, 1, 255);
	
	value = 0x07;
	
	current_value = display_byte_read( 6 );
	value |= current_value;
	
	HAL_I2C_Mem_Write(&hi2c2, LCD_ADDRESS, 6, 1, &value, 1, 255);
}

void display_Hz( void )
{
	display_clear_units();
	uint8_t value = 0x08;
	
	uint8_t current_value = display_byte_read( 6 );
	value |= current_value;
	
	HAL_I2C_Mem_Write(&hi2c2, LCD_ADDRESS, 6, 1, &value, 1, 255);
}


//===================================================================================

/**
!Набор функций для отображения направления энергии
*/
void display_active_consumed_energy( void )
{
	uint8_t value = 0x20;
	
	uint8_t current_value = display_byte_read( 6 );
	value |= current_value;
	
	HAL_I2C_Mem_Write(&hi2c2, LCD_ADDRESS, 6, 1, &value, 1, 255);
}

void display_reactive_consumed_energy( void )
{
	display_clear_units();
	uint8_t value = 0x40;
	
	uint8_t current_value = display_byte_read( 6 );
	value |= current_value;
	
	HAL_I2C_Mem_Write(&hi2c2, LCD_ADDRESS, 6, 1, &value, 1, 255);
}

void display_active_released_energy( void )
{
	display_clear_units();
	uint8_t value = 0x01;
	
	uint8_t current_value = display_byte_read( 7 );
	value |= current_value;
	
	HAL_I2C_Mem_Write(&hi2c2, LCD_ADDRESS, 7, 1, &value, 1, 255);
}

void display_reactive_released_energy( void )
{
	display_clear_units();
	uint8_t value = 0x10;
	
	uint8_t current_value = display_byte_read( 6 );
	value |= current_value;
	
	HAL_I2C_Mem_Write(&hi2c2, LCD_ADDRESS, 6, 1, &value, 1, 255);
}

void display_energy_clear( void )
{
	uint8_t value = 0x8F;
	
	uint8_t current_value = display_byte_read( 6 );
	value &= current_value;
	
	HAL_I2C_Mem_Write(&hi2c2, LCD_ADDRESS, 6, 1, &value, 1, 255);
	
	value = 0xFE;
	
	current_value = display_byte_read( 7 );
	value &= current_value;
	
	HAL_I2C_Mem_Write(&hi2c2, LCD_ADDRESS, 7, 1, &value, 1, 255);
}
