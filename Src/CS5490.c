#include "CS5490.h"

	/**
	! Инициализация измерителя 

	*/
void cs5490_init( CS5490 *chip, uint8_t conv_type )
{
	cs5490_reset(chip);
	
	switch ( conv_type )
	{
		case 0x00:
			cs5490_single_conversation(chip);
			break;
		case 0x01:
			cs5490_continious_conversation(chip);
			break;
		case 0x02:
			cs5490_halt_conversation(chip);
			break;
		default:
			break;
	}
}


	/**
	! Запись значения value в регистр по адресу address на странице page

	*/
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

	/**
	!Чтение 3-х байтов из регистра по адресу address на странице page

	*/
void cs5490_read( CS5490 *chip, int page, int address )
{
	
	HAL_UART_Transmit(chip->cs5490_huart, (uint8_t *) &page, 1, 100);
	HAL_UART_Transmit(chip->cs5490_huart, (uint8_t *) &address, 1, 100);
	
	for ( int i=2; i>=0; i-- )
	{
		HAL_UART_Receive(chip->cs5490_huart, &(chip->cs5490_data[i]), 1, 100);
	}
}

	/**
    ! Выполнение определенной инструкции 

	*/
void cs5490_instruct( CS5490 *chip, int value )
{
	uint8_t buffer = (INSTRUCTION_BYTE | (uint8_t)value);
	HAL_UART_Transmit(chip->cs5490_huart, &buffer, 1, 100);
}

	/**
	!Соединени 3-х байтов, полоченных от измерителя

	*/
uint32_t cs5490_concatData( CS5490 *chip )
{
	uint32_t output = 0x00;
	output = (output + chip->cs5490_data[2]) << 8;
	output = (output + chip->cs5490_data[1]) << 8;
	output = output + chip->cs5490_data[0];
	
	return output;
}

	/**
	!Чтение регистра по адресу address на странице page

	*/
uint32_t cs5490_readReg( CS5490 *chip, int page, int address )
{
	cs5490_read(chip, page, address);
	return cs5490_concatData(chip);
}

	/**
	!Функция калибровки. type - тип калибровки, channel - канал (V или I) 

	*/
void cs5490_calibrate( CS5490 *chip, uint8_t type, uint8_t channel )
{
	HAL_Delay(2000);
	
	uint8_t calibration_byte = CALIBRATION_BYTE;
	calibration_byte |= (type | channel);
	cs5490_instruct(chip, calibration_byte);
	
	HAL_Delay(2000);
}

	/**
	!Конвертация в удобочитаемый формат данных 

	*/
float cs5490_convert_to_double( CS5490 *chip, int LSBpow, int MSBoption )
{

	float output = 0.0;
	uint8_t MSB;

	uint32_t buffer = cs5490_concatData(chip);

	switch ( MSBoption )
	{
		case 0x00:
			buffer &= 0x7FFFFF;
			output = (float)buffer;
			output /= pow(2, LSBpow);
			break;

		case 0x01:
		    MSB = chip->cs5490_data[2] & 0x80;
			if ( MSB )
			{ 
				buffer = ~buffer;
				buffer = buffer & 0x00FFFFFF; 
				output = (float)buffer + 1.0;
				output /= -pow(2,LSBpow);
			} else {  
				output = (float)buffer;
				output /= (pow(2,LSBpow)-1.0);
			}
			break;

		case 0x02:
			output = (float)buffer;
			output /= pow(2,LSBpow);
			break;
		
		default:
			break;

	}

	return output;
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

void cs5490_wakeup( CS5490 *chip )
{
	cs5490_instruct(chip, 3);
}

void cs5490_single_conversation( CS5490 *chip )
{
	cs5490_instruct(chip, 20);
}

void cs5490_continious_conversation( CS5490 *chip )
{
	cs5490_instruct(chip, 21);
}

void cs5490_halt_conversation( CS5490 *chip )
{
	cs5490_instruct(chip, 24);
}

//===================================================================================

float cs5490_get_DС_Offset_V( CS5490 *chip )
{
	cs5490_read(chip, 16, 34);
	return cs5490_convert_to_double(chip, 23, 1);
}

void cs5490_set_DС_Offset_V( CS5490 *chip, float value){
	//TODO перевод в bin
	cs5490_write(chip, 16, 34, value);
}

//===================================================================================

uint32_t cs5490_get_I( CS5490 *chip )
{
	//Page 16, Address 2
	cs5490_read(chip, SOFTWARE_PAGE_BYTE, 0x02);
	return cs5490_concatData(chip);
}

uint32_t cs5490_get_V( CS5490 *chip )
{
	//Page 16, Address 3
	cs5490_read(chip, SOFTWARE_PAGE_BYTE, 0x03);
	return cs5490_concatData(chip);
}

uint32_t cs5490_get_P( CS5490 *chip )
{
	//Page 16, Address 4
	cs5490_read(chip, SOFTWARE_PAGE_BYTE, 0x04);
	return cs5490_concatData(chip);
}

uint32_t cs5490_get_Pavg( CS5490 *chip )
{
	//Page 16, Address 5
	cs5490_read(chip, SOFTWARE_PAGE_BYTE, 0x05);
	return cs5490_concatData(chip);
}

uint32_t cs5490_get_Qavg( CS5490 *chip )
{
	//Page 16, Address 14
	cs5490_read(chip, SOFTWARE_PAGE_BYTE, 0x0E);
	return cs5490_concatData(chip);
}

uint32_t cs5490_get_Savg( CS5490 *chip )
{
	//Page 16, Address 20
	cs5490_read(chip, SOFTWARE_PAGE_BYTE, 0x14);
	return cs5490_concatData(chip);
}

uint32_t cs5490_get_total_P( CS5490 *chip )
{
	//Page 16, Address 29
	cs5490_read(chip, SOFTWARE_PAGE_BYTE, 0x1D);
	return cs5490_concatData(chip);
}

uint32_t cs5490_get_total_S( CS5490 *chip )
{
	//Page 16, Address 30
	cs5490_read(chip, SOFTWARE_PAGE_BYTE, 0x1E);
	return cs5490_concatData(chip);
}

uint32_t cs5490_get_total_Q( CS5490 *chip )
{
	//Page 16, Address 31
	cs5490_read(chip, SOFTWARE_PAGE_BYTE, 0x1F);
	return cs5490_concatData(chip);
}

uint32_t cs5490_get_freq( CS5490 *chip )
{
	cs5490_read(chip, SOFTWARE_PAGE_BYTE, 0x31);
	return cs5490_concatData(chip);
}

uint32_t cs5490_get_time( CS5490 *chip )
{
	//Page 16, Address 61
	cs5490_read(chip, SOFTWARE_PAGE_BYTE, 0x3D);
	return cs5490_concatData(chip);
}
