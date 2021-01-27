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
}


	/**
	! Запись значения value в регистр по адресу address на странице page

	*/
void cs5490_write( CS5490 *chip, int page, int address, uint32_t value )
{
	page |= PAGE_BYTE;
	address |= WRITE_BYTE;
	
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
void cs5490_read( CS5490 *chip, uint8_t page, uint8_t address )
{
	page |= PAGE_BYTE;
	address |= READ_BYTE;
	
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
	output = (output + chip->cs5490_data[0]) << 8;
	output = (output + chip->cs5490_data[1]) << 8;
	output = output + chip->cs5490_data[2];
	
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
	return cs5490_convert_to_double(chip, 24, 0x02);
}

double cs5490_get_Vrms( CS5490 *chip ){
	//Page 16, Address 7
	cs5490_read(chip, 0x10, 0x07);
	return cs5490_convert_to_double(chip, 24, 0x02);
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
	return cs5490_convert_to_double(chip, 23, 0x01);
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
	return cs5490_readReg(chip, 16, 1);
}

/**
!Калибровка измерителя по инструкции AN366REV2 стр 15

*/
uint8_t cs5490_first_calibrate( CS5490 *chip, uint8_t *calibrationData )
{
	uint8_t outputStatus = 0x00;
	uint32_t temp;
	//1 reset
	cs5490_reset(chip);
	
	//2 single conv
	cs5490_single_conversation(chip);
	
	//3 check RegCheck
	uint32_t tempRegChk = cs5490_get_RegChk(chip);
//	if(tempRegChk != 0x00)
//	{
//		//outputStatus = 0x01;
//		//return outputStatus;
//	}
	
	//4 HPF enable
	cs5490_read(chip, 16, 0);
	temp = cs5490_concatData(chip);
	temp |= 0x00000A;
	cs5490_write(chip, 16, 0, temp);
	
	#define SCALE 50/2*4194304
	
	//5 set scale reg
	cs5490_write(chip, 18, 63, SCALE);
	
	//6 start cont. conv.
	cs5490_continious_conversation(chip);
	
	//7 read values
	double rmsV = cs5490_get_Vrms(chip);
	double freq = cs5490_get_freq(chip);
	double rmsI = cs5490_get_Irms(chip);
	double PF = cs5490_get_PF(chip);
	double P = cs5490_get_Pavg(chip);
	double Q = cs5490_get_Qavg(chip);
	double S = cs5490_get_Savg(chip);
   
	printf("Vac RMS: ");
	rmsV = (((rmsV/SYS_GAIN)/V_FS)*V_FS_RMS_V)/V_ALFA;
	printf("%f\r\n", rmsV);
	printf("%f Hz\r\n", freq*SAMPLE_COUNT_DEFAULT);
	printf("Iac RMS: ");
	rmsI  = (((rmsI/SYS_GAIN)/I_FS)*I_FS_RMS_V)/R_SHUNT_OHM;
	printf("%f\r\n", rmsI);
	printf("PF%f\r\n", PF);
	printf("Pact: ");
	printf("%f\r\n", P * P_COEFF); 
	printf("Preact: ");
	printf("%f\r\n", Q * P_COEFF);
	printf("S: ");
	printf("%f\r\n", S * P_COEFF);
	
	//8 stop the conv
	cs5490_instruct(chip, 0xD8);
	
	//9 set Tsettle 2000ms
	cs5490_write(chip, 16, 57, 0x1F40); 
	uint32_t reg = cs5490_readReg(chip, 16, 57);
	if (reg != 0x1F40) outputStatus = 0x02;
	
	//10 set SampleCount 16000
	cs5490_write(chip, 16, 51, 0x3E80); 
	reg = cs5490_readReg(chip, 16, 51);
	if (reg != 0x3E80) outputStatus = 0x03;

	//11 clear DRDY bit
	reg = cs5490_readReg(chip, 0, 23);
	cs5490_write(chip, 0, 23, 0x800000); 
	uint32_t status0_reg_verify = cs5490_readReg(chip, 0, 23);
	if( (reg & 0x7FFFFF) != status0_reg_verify) outputStatus = 0x04;

	
	if(outputStatus == 0x00)
	{
		//11 send calibrate command and wait for DRDY to be set
		cs5490_calibrate(chip, CALIBRATION_TYPE_GAIN, CALIBRATION_TYPE_GAIN);  
		
		//12 wait for DRDY
		uint8_t DRDY, time_flag = 0x00, i;
		HAL_GPIO_WritePin(LED_ACT_GPIO_Port, LED_ACT_Pin, GPIO_PIN_SET);
		do
		{
			reg = cs5490_readReg(chip, 0, 23); // Status0 register (in order to manage the DRDY bit)
			if(reg & 0x800000)
				DRDY = 0x01; 
			else 
				DRDY = 0x00;
			i++;
			if(i == 255) time_flag = 0x01;
		}
		while( DRDY == 0 && time_flag == 0x01);
	  	HAL_GPIO_WritePin(LED_ACT_GPIO_Port, LED_ACT_Pin, GPIO_PIN_RESET);

		if(DRDY)
		{
			//13 cont. conv. 
			cs5490_continious_conversation(chip);

			HAL_Delay(2000);
			
			//14 verify values
			double rmsV = cs5490_get_Vrms(chip);
			double freq = cs5490_get_freq(chip);
			double rmsI = cs5490_get_Irms(chip);
			double PF = cs5490_get_PF(chip);
			uint32_t Igain = cs5490_readReg(chip, 16, 33);
			
			printf("Vac RMS: ");
			rmsV = (((rmsV/SYS_GAIN)/V_FS)*V_FS_RMS_V)/V_ALFA;
			printf("%f\r\n", rmsV);
			printf("%f Hz\r\n", freq*SAMPLE_COUNT_DEFAULT);
			printf("Iac RMS: ");
			rmsI  = (((rmsI/SYS_GAIN)/I_FS)*I_FS_RMS_V)/R_SHUNT_OHM;
			printf("%f\r\n", rmsI);
			printf("PF%f\r\n", PF);

			//15 write values to mem
			cs5490_write(chip, 16, 33, Igain);
			uint32_t IgainReadBack = cs5490_readReg(chip, 16, 33);
			if (IgainReadBack == Igain)
			{
				cs5490_single_conversation(chip); // In order to compute the critical register checksum
				HAL_Delay(1000);

				uint32_t regChk = cs5490_get_RegChk(chip);
					
				printf("I gain: %x\r\n", Igain);
				printf("Critical registers checksum: "); // Must be compared with the one expected for the current configuration (stored in the eeprom). In case of mismatch --> reset CS5490
				printf("RegChk %x\r\n", regChk);
				printf( "RESET OK! Tuned parameters applied." );

				calibrationData[0] = (uint8_t)(Igain>>16); // MSB 
				calibrationData[1] = (uint8_t)(Igain>>8); 
				calibrationData[2] = (uint8_t)Igain;       // LSB
				calibrationData[3] = (uint8_t)(regChk>>16); // MSB 
				calibrationData[4] = (uint8_t)(regChk>>8); 
				calibrationData[5] = (uint8_t)regChk;       // LSB

				uint8_t CRC_v = 0;
				for(uint8_t i=0; i<6;i++)
				{
				  CRC_v ^= calibrationData[i];
				}  
				
			} else {
				printf("uart 1\r\n");
			}
		} else {
			printf("uart 2\r\n");
		}

	} else {
	  printf("uart 3\r\n"); 
	}

  return outputStatus;
}
	  
