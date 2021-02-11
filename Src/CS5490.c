#include "CS5490.h"

	/**
	! Инициализация измерителя 

	*/
void cs5490_init( CS5490 *chip, uint8_t conv_type )
{
	cs5490_reset(chip);
	
	//cs5490_write(chip, );
	
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
	
	chip->cs5490_read_OK = 0x01;
}


	/**
	! Запись значения value в регистр по адресу address на странице page

	*/
void cs5490_write( CS5490 *chip, int page, int address, uint32_t value )
{
	page |= PAGE_BYTE;
	address |= WRITE_BYTE;
	
	HAL_UART_Transmit(chip->cs5490_huart, (uint8_t *) &page, 1, 255);
	HAL_UART_Transmit(chip->cs5490_huart, (uint8_t *) &address, 1, 255);
	
	for ( int i=2; i>=0 ; i-- )
	{
		chip->cs5490_data[i] = value & 0xFF;
		HAL_UART_Transmit(chip->cs5490_huart, &(chip->cs5490_data[i]), 1, 255);  
		value >>= 8;
	}
	
}

	/**
	!Чтение 3-х байтов из регистра по адресу address на странице page

	*/
void cs5490_read( CS5490 *chip, uint8_t page, uint8_t address )
{
	page |= PAGE_BYTE;
	address |= READ_BYTE;
	
	HAL_UART_Transmit(chip->cs5490_huart, (uint8_t *) &page, 1, 255);
	HAL_UART_Transmit(chip->cs5490_huart, (uint8_t *) &address, 1, 255);
	
	uint8_t output = 0x00;
	for ( int i=2; i>=0; i-- )
	{
		output |= HAL_UART_Receive(chip->cs5490_huart, &(chip->cs5490_data[i]), 1, 255);
	}
	
	if(output != 0x00)
	{
		chip->cs5490_read_OK = READ_OPERATION_FAILURE;
	}
}

	/**
    ! Выполнение определенной инструкции 

	*/
void cs5490_instruct( CS5490 *chip, int value )
{
	uint8_t buffer = (INSTRUCTION_BYTE | (uint8_t)value);
	HAL_UART_Transmit(chip->cs5490_huart, &buffer, 1, 255);
}

	/**
	!Соединени 3-х байтов, полоченных от измерителя

	*/
uint32_t cs5490_concatData( CS5490 *chip )
{
	uint32_t output = 0x00;
	output = (output + chip->cs5490_data[0]) << 8;
	output = (output + chip->cs5490_data[1]) << 8;
	output = output + chip->cs5490_data[2];
	
	return output;
}

	/**
	!Чтение регистра по адресу address на странице page

	*/
uint32_t cs5490_readReg( CS5490 *chip, uint8_t page, uint8_t address )
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
double cs5490_convert_to_double( CS5490 *chip, int LSB_pow, int MSB_option )
{
	double output = 0.0;
	uint8_t MSB;

	uint32_t buffer = cs5490_concatData(chip);

	switch ( MSB_option )
	{
		case 0x00:
			buffer &= 0x7FFFFF;
			output = (double)buffer;
			output /= pow(2, LSB_pow);
			break;

		case 0x01:
		    MSB = chip->cs5490_data[2] & 0x80;
			if ( MSB )
			{ 
				buffer = ~buffer;
				buffer = buffer & 0x00FFFFFF; 
				output = (double)buffer + 1.0;
				output /= -pow(2,LSB_pow);
			} else {  
				output = (double)buffer;
				output /= (pow(2,LSB_pow)-1.0);
			}
			break;
		default:
		case 0x02:
			output = (double)buffer;
			output /= pow(2,LSB_pow);
			break;
		
	}

	return output;
}

double cs5490_convert_to_double_2( uint32_t input, int LSB_pow, int MSB_option )
{
	double output = 0.0;
	uint8_t MSB;

	switch ( MSB_option )
	{
		case 0x00:
			input &= 0x7FFFFF;
			output = (double)input;
			output /= pow(2, LSB_pow);
			break;

		case 0x01:
		    MSB = ((input & 0x0000FF00) >> 8) & 0x80;
			if ( MSB )
			{ 
				input = ~input;
				input = input & 0x00FFFFFF; 
				output = (double)input + 1.0;
				output /= -pow(2,LSB_pow);
			} else {  
				output = (double)input;
				output /= (pow(2,LSB_pow)-1.0);
			}
			break;
		default:
		case 0x02:
			output = (double)input;
			output /= pow(2,LSB_pow);
			break;
		
	}

	return output;
}

/**
!Конвертация в двоичный формат для последующей отправки значения в измеритель 

*/
uint32_t cs5490_convert_to_binary(int LSB_pow, int MSB_option, double input)
{

	uint32_t output;

	switch(MSB_option)
	{
		case 0x00:
			input *= pow(2,LSB_pow);
			output = (uint32_t)input;
			output &= 0x7FFFFF; 
			break;
		case 0x01:
			if(input <= 0)
			{
				input *= -pow(2,LSB_pow);
				output = (uint32_t)input;
				output = ~output;
				output = (output+1) & 0xFFFFFF; 
			} else {       
				input *= (pow(2,LSB_pow)-1.0);
				output = (uint32_t)input;
			}
			break;
		
		default:
		case 0x02:
			input *= pow(2,LSB_pow);
			output = (uint32_t)input;
			break;
	}

	return output;
}

//===================================================================================

/**
!Функции, выполняющие различные инструкции

*/
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
/**
!Функции для калибровок: Gain

*/

double cs5490_get_gain_V( CS5490 *chip )
{
	cs5490_read(chip, 16, 35);
	return cs5490_convert_to_double(chip, 22, 0x02);
}

double cs5490_get_gain_I( CS5490 *chip )
{
	cs5490_read(chip, 16, 33);
	return cs5490_convert_to_double(chip, 22, 0x02);
}

void cs5490_set_gain_V( CS5490 *chip, double value )
{
	uint32_t val = cs5490_convert_to_binary(22, 0x02, value);
	cs5490_write( chip, 16, 35, val);
}

void cs5490_set_gain_I( CS5490 *chip, double value ){
	uint32_t val = cs5490_convert_to_binary(22, 0x02, value);
	cs5490_write( chip, 16, 33, val);
}

//===================================================================================

/**
!Функции для калибровок: AC offset, DC offset, Temperature

*/
float cs5490_get_DС_Offset_V( CS5490 *chip )
{
	cs5490_read(chip, 16, 34);
	return cs5490_convert_to_double(chip, 23, 1);
}

void cs5490_set_DС_Offset_V( CS5490 *chip, float value)
{
	uint32_t val = cs5490_convert_to_binary( 23, 0x01, value);
	cs5490_write(chip, 16, 34, val);
}

double cs5490_get_DC_Offset_I( CS5490 *chip )
{
	cs5490_read(chip, 16, 32);
	return cs5490_convert_to_double(chip, 23, 1);
}

double cs5490_get_AC_Offset_I( CS5490 *chip )
{
	cs5490_read(chip, 16, 37);
	return cs5490_convert_to_double(chip, 24, 0x02);
}

void cs5490_set_DC_Offset_I( CS5490 *chip, double value )
{
	uint32_t val = cs5490_convert_to_binary(23, 0x01, value);
	cs5490_write(chip, 16, 32, val);
}

void cs5490_set_AC_Offset_I( CS5490 *chip, double value )
{
	uint32_t val = cs5490_convert_to_binary(24, 0x02, value);
	cs5490_write(chip, 16, 37, val);
}

double cs5490_get_Offset_T( CS5490 *chip )
{
	cs5490_read(chip, 16, 55);
	return cs5490_convert_to_double(chip, 16, 0x01);
}

void cs5490_set_Offset_T( CS5490 *chip, double value ){
	uint32_t val = cs5490_convert_to_binary(16, 0x01, value);
	cs5490_write(chip, 16, 55, val);
}

//===================================================================================

/**
!Функции получения измерений и преобразование в нормальный вид 

*/
double cs5490_get_I( CS5490 *chip )
{
	//Page 16, Address 2
	cs5490_read(chip, 0x10, 0x02);
	return cs5490_convert_to_double(chip, 23, 0x01);
}

double cs5490_get_V( CS5490 *chip )
{
	//Page 16, Address 3
	cs5490_read(chip, 0x10, 0x03);
	return cs5490_convert_to_double(chip, 23, 0x01);
}

uint32_t cs5490_get_P( CS5490 *chip )
{
	//Page 16, Address 4
	cs5490_read(chip, 0x10, 0x04);
	return cs5490_concatData(chip);
}

double cs5490_get_Irms( CS5490 *chip ){
	//Page 16, Address 6
	cs5490_read(chip, 0x10, 0x06);
	
	uint32_t hex_value = cs5490_concatData(chip);
	double factor = 1/(pow(2, 24) - 1);
	return factor * (double) hex_value;
}

double cs5490_get_Vrms( CS5490 *chip ){
	//Page 16, Address 7
	cs5490_read(chip, 0x10, 0x07);
	
	uint32_t hex_value = cs5490_concatData(chip);
	double factor = 1/(pow(2, 24) - 1);
	return factor * (double) hex_value;
}

double cs5490_get_PF( CS5490 *chip )
{
	//Page 16, Address 21
	cs5490_read(chip, 16, 21);
	return cs5490_convert_to_double(chip, 23, 0x01);
}

double cs5490_get_Pavg( CS5490 *chip )
{
	//Page 16, Address 5
	cs5490_read(chip, 0x10, 0x05);
	
	int8_t MSB = (chip->cs5490_data[0] & 0x80) >> 7;
	if(MSB == 0x00)
	{
		MSB = -1;
	}
	uint32_t hex_value = cs5490_concatData(chip);
	double factor = 1/(pow(2, 23) - 1) * (-1) * MSB;
	return factor * (double) hex_value;
}

double cs5490_get_Qavg( CS5490 *chip )
{
	//Page 16, Address 14
	cs5490_read(chip, 0x10, 0x0E);
	return cs5490_convert_to_double(chip, 23, 0x01);
}

double cs5490_get_Savg( CS5490 *chip )
{
	//Page 16, Address 20
	cs5490_read(chip, 0x10, 0x14);
	return cs5490_convert_to_double(chip, 23, 0x01);
}

uint32_t cs5490_get_total_P( CS5490 *chip )
{
	//Page 16, Address 29
	cs5490_read(chip, 0x10, 0x1D);
	return cs5490_concatData(chip);
}

uint32_t cs5490_get_total_S( CS5490 *chip )
{
	//Page 16, Address 30
	cs5490_read(chip, 0x10, 0x1E);
	return cs5490_concatData(chip);
}

uint32_t cs5490_get_total_Q( CS5490 *chip )
{
	//Page 16, Address 31
	cs5490_read(chip, 0x10, 0x1F);
	return cs5490_concatData(chip);
}

double cs5490_get_freq( CS5490 *chip )
{
	//Page 16, Address 49
	cs5490_read(chip, 0x10, 0x31);
	return cs5490_convert_to_double(chip, 23, 0x02);
}

double cs5490_get_time( CS5490 *chip )
{
	//Page 16, Address 61
	cs5490_read(chip, 0x10, 0x3D);
	return cs5490_convert_to_double(chip, 0, 0x02);
}

double cs5490_get_T( CS5490 *chip )
{
	//Page 16, Address 27
	cs5490_read(chip, 0x10, 0x1B);
	return cs5490_convert_to_double(chip, 16, 0x01);
}

uint32_t cs5490_get_RegChk( CS5490 *chip )
{
	return cs5490_readReg(chip, 0x10, 0x01);
}

/**
!Калибровка измерителя по инструкции AN366REV2 стр 15

*/

uint8_t full_callibration( CS5490 *chip, uint8_t *calibrationData )
{
	//1 reset
	cs5490_reset(chip);
	
	//2 single conv.
	cs5490_single_conversation(chip);
	
	//3 verify regcheck
	uint32_t regcheck = cs5490_get_RegChk(chip);
	if (regcheck != 0x00dd8bcc || !chip->cs5490_read_OK)
	{
		printf("3 phase error: regcheck=%x\r\n", regcheck);
		return 0x03;
	}
	
	//4 enable HPF
	uint32_t reset = cs5490_readReg(chip, 0x10, 0x00);
	if(!chip->cs5490_read_OK)
	{
		printf("4 phase error\r\n");
		return 0x04;
	}
	reset |= 0x0000000A;
	cs5490_write(chip, 0x10, 0x00, reset);
	
	//5 set scale reg (no full load available)
	cs5490_write(chip, 0x12, 0x3F, SCALE_REGISTER_VALUE);
	
	//6 apply ref. voltage and current
	printf("apply\r\n");
	
	//7 start continious conv
	cs5490_continious_conversation(chip);
	HAL_Delay(1000);
	
	//8 read values
	double rmsV = cs5490_get_Vrms(chip);
	double freq = cs5490_get_freq(chip);
	double rmsI = cs5490_get_Irms(chip);
	double PF = cs5490_get_PF(chip);
	double P = cs5490_get_Pavg(chip);
   
	if(!chip->cs5490_read_OK)
	{
		printf("error reading data at 8 phase\r\n");
		return 0x09;
	}
	
	rmsV = (rmsV*VOLTAGE_FULLSCALE)/REGISTER_FULLSCALE;
	
	printf("Vrms: %f V\r\n", rmsV);
	printf("Freq: %f Hz\r\n", freq*SAMPLE_COUNT_DEFAULT);
	
	rmsI = (rmsI*CURRENT_FULLSCALE)/REGISTER_FULLSCALE;
	
	printf("Irms: %f A\r\n", rmsI);
	printf("PF %f\r\n", PF);
	printf("P: %f W\r\n", P * POWER_FULLSCALE);	
	
	//9 stop conv
	cs5490_halt_conversation(chip);
	
	//10 set Tsettle 2000ms
	cs5490_write(chip, 16, 57, 0x1F40); 
	uint32_t reg = cs5490_readReg(chip, 16, 57);
	if (reg != 0x1F40 || !chip->cs5490_read_OK)
	{
		printf("10 error: reg=%x\r\n", reg);
		return 0x0A;
	}
	
	//11 set SampleCount 16000
	cs5490_write(chip, 16, 51, 0x3E80); 
	reg = cs5490_readReg(chip, 16, 51);
	if (reg != 0x3E80 || !chip->cs5490_read_OK)	
	{
		printf("11 error: reg=%x\r\n", reg);
		return 0x0B;
	}

	//12 clear DRDY bit
	reg = cs5490_readReg(chip, 0, 23);
	cs5490_write(chip, 0, 23, 0x800000); 
	uint32_t status0_reg_verify = cs5490_readReg(chip, 0, 23);
	if( (reg & 0x7FFFFF ) != status0_reg_verify || !chip->cs5490_read_OK )
	{
		printf("12 error: reg=%x \r\n", status0_reg_verify);
		return 0x0C;
	}
	
	//13 send AC Gain calibration command
	cs5490_calibrate(chip, CALIBRATION_TYPE_GAIN, CALIBRATION_CHANNEL_CURRENT_AND_VOLTAGE);
	
	//14 whait DRDY for 3 sec
	uint8_t DRDY;
	uint16_t start = uwTick;
	
	do
	{
		reg = cs5490_readReg(chip, 0, 23); 
		if(chip->cs5490_read_OK && (reg & 0x800000))
		{
			DRDY = 0x01; 
		} else {
			DRDY = 0x00;
		}
	}
	while( DRDY == 0 && (uwTick - start) < 3000);
	
	//15 start cont. conv.
	cs5490_continious_conversation(chip);
	HAL_Delay(500);
	
	//16 verify accuracy
	rmsV = cs5490_get_Vrms(chip);
	freq = cs5490_get_freq(chip);
	rmsI = cs5490_get_Irms(chip);
	PF = cs5490_get_PF(chip);
	P = cs5490_get_Pavg(chip);
			
	if(!chip->cs5490_read_OK)
	{
		printf("16 error\r\n");
		return 0x10;
	}
	
	rmsV = (rmsV*VOLTAGE_FULLSCALE)/REGISTER_FULLSCALE;
	
	printf("Vrms: %f V\r\n", rmsV);
	printf("Freq: %f Hz\r\n", freq*SAMPLE_COUNT_DEFAULT);
	
	rmsI = (rmsI*CURRENT_FULLSCALE)/REGISTER_FULLSCALE;
	
	printf("Irms: %f A\r\n", rmsI);
	printf("PF %f\r\n", PF);
	printf("P: %f W\r\n", P * POWER_FULLSCALE);
	
	uint32_t Igain = cs5490_readReg(chip, 16, 33);
	uint32_t Vgain = cs5490_readReg(chip, 16, 35);
	uint32_t Iac_off = cs5490_readReg(chip, 16, 37);
	uint32_t Poff = cs5490_readReg(chip, 16, 36);
	uint32_t PF_2 = cs5490_readReg(chip, 16, 21);
	uint32_t regcheck_2 = cs5490_readReg(chip, 16, 1);
	
	if(!chip->cs5490_read_OK)
	{
		printf("16 last error\r\n");
		return 0x11;
	}
	
	printf("Igain %x\r\n", Igain);
	printf("Vgain %x\r\n", Vgain);
	printf("Iacoff %x\r\n", Iac_off);
	printf("Poff %x\r\n", Poff);
	printf("PF_2 %x\r\n", PF_2);
	printf("Regcheck %x\r\n", regcheck_2);
	
}