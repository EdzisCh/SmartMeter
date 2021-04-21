#include "CS5490.h"

/**
! Инициализация измерителей и изменение калибровочных регистров
Возвращает 0 если все хорошо или номер измерителя, который не удалось 
инициализировать (причем оставшиеся измерители также не инициализируются)
*/
uint8_t cs5490_init( CS5490 *chip_1, CS5490 *chip_2, CS5490 *chip_3 )
{
	cs5490_reset(chip_1);
	chip_1->cs5490_read_OK = READ_OPERATION_SUCCESS;
	cs5490_reset(chip_2);
	chip_2->cs5490_read_OK = READ_OPERATION_SUCCESS;
	cs5490_reset(chip_3);
	chip_3->cs5490_read_OK = READ_OPERATION_SUCCESS;

	//Забираем из ПЗУ данные
	double Igain_1;
	double Vgain_1;
	double Iac_off_1;
	double Poff_1;
	double PF_1;
	uint32_t regcheck_from_ROM_1;

	double Igain_2;
	double Vgain_2;
	double Iac_off_2;
	double Poff_2;
	double PF_2;
	uint32_t regcheck_from_ROM_2;

	double Igain_3;
	double Vgain_3;
	double Iac_off_3;
	double Poff_3;
	double PF_3;
	uint32_t regcheck_from_ROM_3;

	m24m01_get_from_mem(MEM_ADDRESS_I_GAIN_L_1, (uint8_t *) &Igain_1, sizeof(double));
	m24m01_get_from_mem(MEM_ADDRESS_V_GAIN_L_1, (uint8_t *) &Vgain_1, sizeof(double));
	m24m01_get_from_mem(MEM_ADDRESS_I_AC_OFF_L_1, (uint8_t *) &Iac_off_1, sizeof(double));
	m24m01_get_from_mem(MEM_ADDRESS_P_OFF_L_1, (uint8_t *) &Poff_1, sizeof(double));
	m24m01_get_from_mem(MEM_ADDRESS_PF_L_1, (uint8_t *) &PF_1, sizeof(double));
	m24m01_get_from_mem(MEM_ADDRESS_REGCHECK_L_1, (uint8_t *) &regcheck_from_ROM_1, 4);


	m24m01_get_from_mem(MEM_ADDRESS_I_GAIN_L_2, (uint8_t *) &Igain_2, sizeof(double));
	m24m01_get_from_mem(MEM_ADDRESS_V_GAIN_L_2, (uint8_t *) &Vgain_2, sizeof(double));
	m24m01_get_from_mem(MEM_ADDRESS_I_AC_OFF_L_2, (uint8_t *) &Iac_off_2, sizeof(double));
	m24m01_get_from_mem(MEM_ADDRESS_P_OFF_L_2, (uint8_t *) &Poff_2, sizeof(double));
	m24m01_get_from_mem(MEM_ADDRESS_PF_L_2, (uint8_t *) &PF_2, sizeof(double));
	m24m01_get_from_mem(MEM_ADDRESS_REGCHECK_L_2, (uint8_t *) &regcheck_from_ROM_2, 4);

	m24m01_get_from_mem(MEM_ADDRESS_I_GAIN_L_3, (uint8_t *) &Igain_3, sizeof(double));
	m24m01_get_from_mem(MEM_ADDRESS_V_GAIN_L_3, (uint8_t *) &Vgain_3, sizeof(double));
	m24m01_get_from_mem(MEM_ADDRESS_I_AC_OFF_L_3, (uint8_t *) &Iac_off_3, sizeof(double));
	m24m01_get_from_mem(MEM_ADDRESS_P_OFF_L_3, (uint8_t *) &Poff_3, sizeof(double));
	m24m01_get_from_mem(MEM_ADDRESS_PF_L_3, (uint8_t *) &PF_3, sizeof(double));
	m24m01_get_from_mem(MEM_ADDRESS_REGCHECK_L_3, (uint8_t *) &regcheck_from_ROM_3, 4);

	//вписываем их в измеритель
	cs5490_write(chip_1, 16, 33, Igain_1);
	cs5490_write(chip_1, 16, 35, Vgain_1);
	cs5490_write(chip_1, 16, 37, Iac_off_1);
	cs5490_write(chip_1, 16, 21, PF_1);

	cs5490_write(chip_2, 16, 33, Igain_2);
	cs5490_write(chip_2, 16, 35, Vgain_2);
	cs5490_write(chip_2, 16, 37, Iac_off_2);
	cs5490_write(chip_2, 16, 21, PF_2);

	cs5490_write(chip_3, 16, 33, Igain_3);
	cs5490_write(chip_3, 16, 35, Vgain_3);
	cs5490_write(chip_3, 16, 37, Iac_off_3);
	cs5490_write(chip_3, 16, 21, PF_3);

	//проверка 
	cs5490_single_conversation(chip_1);
	cs5490_single_conversation(chip_2);
	cs5490_single_conversation(chip_3);

	uint32_t regcheck_1 = cs5490_get_RegChk(chip_1);
	uint32_t regcheck_2 = cs5490_get_RegChk(chip_2);
	uint32_t regcheck_3 = cs5490_get_RegChk(chip_3);
	
	if(regcheck_1 != regcheck_from_ROM_1 && chip_1->cs5490_read_OK != READ_OPERATION_SUCCESS)
	{
		return 0x01;
	}
	cs5490_continious_conversation(chip_1);

	if(regcheck_2 != regcheck_from_ROM_2 && chip_2->cs5490_read_OK != READ_OPERATION_SUCCESS)
	{
		return 0x02;
	}
	cs5490_continious_conversation(chip_2);

	if(regcheck_3 != regcheck_from_ROM_3 && chip_3->cs5490_read_OK != READ_OPERATION_SUCCESS)
	{
		return 0x03;
	}
	cs5490_continious_conversation(chip_3);
	
	return 0x00;
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
	
	uint32_t hex_value = cs5490_concatData(chip);
	double factor = (double) hex_value;
	return factor;
}

double cs5490_get_gain_I( CS5490 *chip )
{
	cs5490_read(chip, 16, 33);
	
	uint32_t hex_value = cs5490_concatData(chip);
	double factor = (double) hex_value;
	
	return factor;
}

void cs5490_set_gain_V( CS5490 *chip, double value )
{
	double Vgain = (VOLTAGE_FULLSCALE_MAX / value) * pow(2,22);
	cs5490_write( chip, 16, 35, Vgain);
}

void cs5490_set_gain_I( CS5490 *chip, double value ){
	
	double Igain = (CURRENT_FULLSCALE_MAX / value) * pow(2,22);
	cs5490_write( chip, 16, 33, Igain);
}

//===================================================================================

double cs5490_get_Offset_P(CS5490 * chip)
{
    cs5490_read(chip, 16, 36);
	
    uint32_t hex_value = cs5490_concatData(chip);
    double factor = (double) hex_value;
	
    return factor;
}

double cs5490_get_AC_Offset_I(CS5490 * chip)
{
    cs5490_read(chip, 16, 37);
	
    uint32_t hex_value = cs5490_concatData(chip);
    double factor = (double) hex_value;
	
    return factor;
}

//===================================================================================

/**
!Функции получения измерений и преобразование в нормальный вид 

*/
double cs5490_get_I( CS5490 *chip )
{
	//Page 16, Address 2
	cs5490_read(chip, 0x10, 0x02);
	
	uint32_t hex_value = cs5490_concatData(chip);
	double factor = 1/(pow(2, 24) - 1);
	factor *= (double) hex_value;
	
	factor = (factor*CURRENT_FULLSCALE)/REGISTER_FULLSCALE;
	
	return factor;
}

double cs5490_get_V( CS5490 *chip )
{
	//Page 16, Address 3
	cs5490_read(chip, 0x10, 0x03);
	uint32_t hex_value = cs5490_concatData(chip);
	double factor = 1/(pow(2, 24) - 1);
	factor *= (double) hex_value;
	
	factor = (factor*VOLTAGE_FULLSCALE)/REGISTER_FULLSCALE;
	
	return factor;
}

double cs5490_get_P( CS5490 *chip )
{
	//Page 16, Address 4
	cs5490_read(chip, 0x10, 0x04);
	uint32_t hex_value = cs5490_concatData(chip);
	double factor = 1/(pow(2, 24) - 1);
	factor *= (double) hex_value;
	
	factor = (factor*VOLTAGE_FULLSCALE)/REGISTER_FULLSCALE;
	
	return factor;
}

double cs5490_get_Irms( CS5490 *chip ){
	//Page 16, Address 6
	cs5490_read(chip, 0x10, 0x06);
	
	uint32_t hex_value = cs5490_concatData(chip);
	double factor = 1/(pow(2, 24) - 1);
	
	factor *= (double) hex_value;
	
	factor *= CURRENT_FULLSCALE / REGISTER_FULLSCALE;
	
	return factor;
}

double cs5490_get_Vrms( CS5490 *chip ){
	//Page 16, Address 7
	cs5490_read(chip, 0x10, 0x07);
	
	uint32_t hex_value = cs5490_concatData(chip);
	double factor = 1/(pow(2, 24) - 1);
	
	factor *= (double) hex_value;
	
	factor *= VOLTAGE_FULLSCALE / REGISTER_FULLSCALE;
	
	return factor;
}

double cs5490_get_PF( CS5490 *chip )
{
	//Page 16, Address 21
	cs5490_read(chip, 16, 21);
	//Знак определяется Pavg
	uint32_t hex_value = cs5490_concatData(chip);
	double factor = 1/(pow(2, 23) - 1);
	factor *= (double) hex_value;
	
	return factor;
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
	
	factor *= (double) hex_value;
	
	factor = factor * POWER_FULLSCALE;
	
	return factor;
}

double cs5490_get_Qavg( CS5490 *chip )
{
	//Page 16, Address 14
	cs5490_read(chip, 0x10, 0x0E);
	int8_t MSB = (chip->cs5490_data[0] & 0x80) >> 7;
	if(MSB == 0x00)
	{
		MSB = -1;
	}
	uint32_t hex_value = cs5490_concatData(chip);
	double factor = 1/(pow(2, 23) - 1) * (-1) * MSB;
	
	factor *= (double) hex_value;
	
	factor = factor * POWER_FULLSCALE;
	
	return factor;
}

double cs5490_get_Savg( CS5490 *chip )
{
	//Page 16, Address 20
	cs5490_read(chip, 0x10, 0x14);
	int8_t MSB = (chip->cs5490_data[0] & 0x80) >> 7;
	if(MSB == 0x00)
	{
		MSB = -1;
	}
	uint32_t hex_value = cs5490_concatData(chip);
	double factor = 1/(pow(2, 23) - 1) * (-1) * MSB;
	
	factor *= (double) hex_value;
	
	factor = factor * POWER_FULLSCALE;
	
	return factor;
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
	uint32_t hex_value = cs5490_concatData(chip);
	
	double factor = 1/(pow(2, 23)-1);
	factor *= (double) hex_value;
	
	factor *= SAMPLE_COUNT_DEFAULT;
	
	return factor;
}

uint32_t cs5490_get_time( CS5490 *chip )
{
	//Page 16, Address 61
	cs5490_read(chip, 0x10, 0x3D);
	uint32_t value = cs5490_concatData(chip);
	return cs5490_concatData(chip);
}

uint32_t cs5490_get_RegChk( CS5490 *chip )
{
	return cs5490_readReg(chip, 0x10, 0x01);
}

/**
!Калибровка измерителя по инструкции AN366REV2 стр 15

*/
uint8_t cs5490_full_callibration( CS5490 *chip_1, CS5490 *chip_2, CS5490 *chip_3 )
{
	//1 reset
	cs5490_reset(chip_1);
	chip_1->cs5490_read_OK = READ_OPERATION_SUCCESS;
        cs5490_reset(chip_2);
	chip_2->cs5490_read_OK = READ_OPERATION_SUCCESS;
        cs5490_reset(chip_3);
	chip_3->cs5490_read_OK = READ_OPERATION_SUCCESS;
	
	//2 single conv.
	cs5490_single_conversation(chip_1);
	cs5490_single_conversation(chip_2);
	cs5490_single_conversation(chip_3);
	
	//3 verify regcheck
	uint32_t regcheck = cs5490_get_RegChk(chip_1);
	if (regcheck != 0x00dd8bcc || !chip_1->cs5490_read_OK)
	{
		return 0x03;
	}
        regcheck = cs5490_get_RegChk(chip_2);
	if (regcheck != 0x00dd8bcc || !chip_2->cs5490_read_OK)
	{
		return 0x03;
	}
        regcheck = cs5490_get_RegChk(chip_3);
	if (regcheck != 0x00dd8bcc || !chip_3->cs5490_read_OK)
	{
		return 0x03;
	}
	
	//4 enable HPF
	uint32_t reset = cs5490_readReg(chip_1, 0x10, 0x00);
	if(!chip_1->cs5490_read_OK)
	{
		return 0x04;
	}
	reset |= 0x0000000A;
	cs5490_write(chip_1, 0x10, 0x00, reset);
    
	reset = cs5490_readReg(chip_2, 0x10, 0x00);
	if(!chip_2->cs5490_read_OK)
	{
		return 0x04;
	}
	reset |= 0x0000000A;
	cs5490_write(chip_2, 0x10, 0x00, reset);
    
	reset = cs5490_readReg(chip_3, 0x10, 0x00);
	if(!chip_3->cs5490_read_OK)
	{
		return 0x04;
	}
	reset |= 0x0000000A;
	cs5490_write(chip_3, 0x10, 0x00, reset);
	
	//5 set scale reg (no full load available)
	cs5490_set_gain_V(chip_1, 220);
	// cs5490_set_gain_I(chip_1, 0.18);
	cs5490_calibrate(chip_1, CALIBRATION_TYPE_GAIN, CALIBRATION_CHANNEL_VOLTAGE);
    cs5490_write(chip_1, 0x12, 0x3F, SCALE_REGISTER_VALUE);
	
	cs5490_set_gain_V(chip_2, 220);
	// cs5490_set_gain_I(chip_2, 0.18);
	cs5490_calibrate(chip_2, CALIBRATION_TYPE_GAIN, CALIBRATION_CHANNEL_VOLTAGE);
    cs5490_write(chip_2, 0x12, 0x3F, SCALE_REGISTER_VALUE);
	
	cs5490_set_gain_V(chip_3, 220);
	// cs5490_set_gain_I(chip_3, 0.18);
	cs5490_calibrate(chip_3, CALIBRATION_TYPE_GAIN, CALIBRATION_CHANNEL_VOLTAGE);
	cs5490_write(chip_3, 0x12, 0x3F, SCALE_REGISTER_VALUE);
	
	//6 apply ref. voltage and current
	
	//7 start continious conv
	cs5490_continious_conversation(chip_1);
    cs5490_continious_conversation(chip_2);
    cs5490_continious_conversation(chip_3);
	
	HAL_Delay(500);
	
	//8 read values
	double rmsV_1 = cs5490_get_Vrms(chip_1);
    double rmsV_2 = cs5490_get_Vrms(chip_2);
    double rmsV_3 = cs5490_get_Vrms(chip_3);
	double freq_1 = cs5490_get_freq(chip_1);
    double freq_2 = cs5490_get_freq(chip_2);
    double freq_3 = cs5490_get_freq(chip_3);
	double rmsI_1 = cs5490_get_Irms(chip_1);
    double rmsI_2 = cs5490_get_Irms(chip_2);
    double rmsI_3 = cs5490_get_Irms(chip_3);
	double PF_1 = cs5490_get_PF(chip_1);
	double PF_2 = cs5490_get_PF(chip_2);
    double PF_3 = cs5490_get_PF(chip_3);
	double P_1 = cs5490_get_Pavg(chip_1);
    double P_2 = cs5490_get_Pavg(chip_2);
    double P_3 = cs5490_get_Pavg(chip_3);
   
	if(!chip_1->cs5490_read_OK || !chip_2->cs5490_read_OK || !chip_3->cs5490_read_OK)
	{
		return 0x09;
	}
	
	//9 stop conv
	cs5490_halt_conversation(chip_1);
    cs5490_halt_conversation(chip_2);
    cs5490_halt_conversation(chip_3);
	
	//10 set Tsettle 2000ms
	cs5490_write(chip_1, 16, 57, 0x0FD0); 
	uint32_t reg = cs5490_readReg(chip_1, 16, 57);
	if (reg != 0x0FD0 || !chip_1->cs5490_read_OK)
	{
		return 0x0A;
	}
	
    cs5490_write(chip_2, 16, 57, 0x0FD0); 
	reg = cs5490_readReg(chip_2, 16, 57);
	if (reg != 0x0FD0 || !chip_2->cs5490_read_OK)
	{
		return 0x0A;
	}
    
	cs5490_write(chip_3, 16, 57, 0x0FD0); 
	reg = cs5490_readReg(chip_3, 16, 57);
	if (reg != 0x0FD0 || !chip_3->cs5490_read_OK)
	{
		return 0x0A;
	}
	
	//11 set SampleCount 16000
	cs5490_write(chip_1, 16, 51, 0x3E80); 
	reg = cs5490_readReg(chip_1, 16, 51);
	if (reg != 0x3E80 || !chip_1->cs5490_read_OK)	
	{
		return 0x0B;
	}
    
	cs5490_write(chip_2, 16, 51, 0x3E80); 
	reg = cs5490_readReg(chip_2, 16, 51);
	if (reg != 0x3E80 || !chip_2->cs5490_read_OK)	
	{
		return 0x0B;
	}
    
	cs5490_write(chip_3, 16, 51, 0x3E80); 
	reg = cs5490_readReg(chip_3, 16, 51);
	if (reg != 0x3E80 || !chip_3->cs5490_read_OK)	
	{
		return 0x0B;
	}

	//12 clear DRDY bit
	reg = cs5490_readReg(chip_1, 0, 23);
	cs5490_write(chip_1, 0, 23, 0x800000); 
	uint32_t status0_reg_verify = cs5490_readReg(chip_1, 0, 23);
	if( (reg & 0x7FFFFF ) != status0_reg_verify || !chip_1->cs5490_read_OK )
	{
		return 0x0C;
	}
        
	reg = cs5490_readReg(chip_2, 0, 23);
	cs5490_write(chip_2, 0, 23, 0x800000); 
	status0_reg_verify = cs5490_readReg(chip_2, 0, 23);
	if( (reg & 0x7FFFFF ) != status0_reg_verify || !chip_2->cs5490_read_OK )
	{
		return 0x0C;
	}
    
	reg = cs5490_readReg(chip_3, 0, 23);
	cs5490_write(chip_3, 0, 23, 0x800000); 
	status0_reg_verify = cs5490_readReg(chip_3, 0, 23);
	if( (reg & 0x7FFFFF ) != status0_reg_verify || !chip_3->cs5490_read_OK )
	{
		return 0x0C;
	}
	
	//13 send AC Gain calibration command
	cs5490_calibrate(chip_1, CALIBRATION_TYPE_GAIN, CALIBRATION_CHANNEL_CURRENT);
    cs5490_calibrate(chip_2, CALIBRATION_TYPE_GAIN, CALIBRATION_CHANNEL_CURRENT);
    cs5490_calibrate(chip_3, CALIBRATION_TYPE_GAIN, CALIBRATION_CHANNEL_CURRENT);
	
	//14 whait DRDY for 3 sec
	uint8_t DRDY_1, DRDY_2, DRDY_3;
	uint32_t start = 0;
	uint32_t reg_2 = 0;
    uint32_t reg_3 = 0;
	do
	{
		reg = cs5490_readReg(chip_1, 0, 23); 
        reg_2 = cs5490_readReg(chip_2, 0, 23); 
        reg_3 = cs5490_readReg(chip_3, 0, 23); 
		
		if(chip_1->cs5490_read_OK && (reg & 0x800000))
		{
			DRDY_1 = 0x01; 
		} else {
			DRDY_1 = 0x00;
		}
		
		if(chip_2->cs5490_read_OK && (reg_2 & 0x800000))
		{
			DRDY_2 = 0x01; 
		} else {
			DRDY_2 = 0x00;
		}
		if(chip_3->cs5490_read_OK && (reg_3 & 0x800000))
		{
			DRDY_3 = 0x01; 
		} else {
			DRDY_3 = 0x00;
		}
                start++;
	} while( DRDY_1 == 0 && DRDY_2 == 0 && DRDY_3 == 0 && (uwTick - start) < 3000);
	
	//15 start cont. conv.
	cs5490_continious_conversation(chip_1);
    cs5490_continious_conversation(chip_2);
    cs5490_continious_conversation(chip_3);
	
	HAL_Delay(500);	
		
	//16 verify accuracy
    rmsV_1 = cs5490_get_Vrms(chip_1);
	rmsV_2 = cs5490_get_Vrms(chip_2);
	rmsV_3 = cs5490_get_Vrms(chip_3);
	freq_1 = cs5490_get_freq(chip_1);
	freq_2 = cs5490_get_freq(chip_2);
	freq_3 = cs5490_get_freq(chip_3);
	rmsI_1 = cs5490_get_Irms(chip_1);
	rmsI_2 = cs5490_get_Irms(chip_2);
	rmsI_3 = cs5490_get_Irms(chip_3);
	PF_1 = cs5490_get_PF(chip_1);
	PF_2 = cs5490_get_PF(chip_2);
	PF_3 = cs5490_get_PF(chip_3);
	P_1 = cs5490_get_Pavg(chip_1);
	P_2 = cs5490_get_Pavg(chip_2);
	P_3 = cs5490_get_Pavg(chip_3);
   
	if(!chip_1->cs5490_read_OK || !chip_2->cs5490_read_OK || !chip_3->cs5490_read_OK)
	{
		return 0x09;
	}

	double Igain_1 = cs5490_get_gain_I(chip_1);
	double Igain_2 = cs5490_get_gain_I(chip_2);
	double Igain_3 = cs5490_get_gain_I(chip_3);
	double Vgain_1 = cs5490_get_gain_V(chip_1);
	double Vgain_2 = cs5490_get_gain_V(chip_2);
	double Vgain_3 = cs5490_get_gain_V(chip_3);
	double Iac_off_1 = cs5490_get_AC_Offset_I(chip_1);
	double Iac_off_2 = cs5490_get_AC_Offset_I(chip_2);
	double Iac_off_3 = cs5490_get_AC_Offset_I(chip_3);
	double Poff_1 = cs5490_get_Offset_P(chip_1);
	double Poff_2 = cs5490_get_Offset_P(chip_2);
	double Poff_3 = cs5490_get_Offset_P(chip_3);
	uint32_t regcheck_1 = cs5490_readReg(chip_1, 16, 1);
	uint32_t regcheck_2 = cs5490_readReg(chip_2, 16, 1);
	uint32_t regcheck_3 = cs5490_readReg(chip_3, 16, 1);

	//сохраняем данные в EEPROM
	m24m01_save_to_mem(MEM_ADDRESS_I_GAIN_L_1, (uint8_t *) &Igain_1, sizeof(double));
	m24m01_save_to_mem(MEM_ADDRESS_V_GAIN_L_1, (uint8_t *) &Vgain_1, sizeof(double));
	m24m01_save_to_mem(MEM_ADDRESS_I_AC_OFF_L_1, (uint8_t *) &Iac_off_1, sizeof(double));
	m24m01_save_to_mem(MEM_ADDRESS_P_OFF_L_1, (uint8_t *) &Poff_1, sizeof(double));
	m24m01_save_to_mem(MEM_ADDRESS_PF_L_1, (uint8_t *) &PF_1, sizeof(double));
	m24m01_save_to_mem(MEM_ADDRESS_REGCHECK_L_1, (uint8_t *) &regcheck_1, 4);

	m24m01_save_to_mem(MEM_ADDRESS_I_GAIN_L_2, (uint8_t *) &Igain_2, sizeof(double));
	m24m01_save_to_mem(MEM_ADDRESS_V_GAIN_L_2, (uint8_t *) &Vgain_2, sizeof(double));
	m24m01_save_to_mem(MEM_ADDRESS_I_AC_OFF_L_2, (uint8_t *) &Iac_off_2, sizeof(double));
	m24m01_save_to_mem(MEM_ADDRESS_P_OFF_L_2, (uint8_t *) &Poff_2, sizeof(double));
	m24m01_save_to_mem(MEM_ADDRESS_PF_L_2, (uint8_t *) &PF_2, sizeof(double));
	m24m01_save_to_mem(MEM_ADDRESS_REGCHECK_L_2, (uint8_t *) &regcheck_2, 4);

	m24m01_save_to_mem(MEM_ADDRESS_I_GAIN_L_3, (uint8_t *) &Igain_3, sizeof(double));
	m24m01_save_to_mem(MEM_ADDRESS_V_GAIN_L_3, (uint8_t *) &Vgain_3, sizeof(double));
	m24m01_save_to_mem(MEM_ADDRESS_I_AC_OFF_L_3, (uint8_t *) &Iac_off_3, sizeof(double));
	m24m01_save_to_mem(MEM_ADDRESS_P_OFF_L_3, (uint8_t *) &Poff_3, sizeof(double));
	m24m01_save_to_mem(MEM_ADDRESS_PF_L_3, (uint8_t *) &PF_3, sizeof(double));
	m24m01_save_to_mem(MEM_ADDRESS_REGCHECK_L_3, (uint8_t *) &regcheck_3, 4);

	return 0x00;
}
