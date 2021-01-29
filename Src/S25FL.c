#include "S25FL.h"

/**
!Инициализация модуля работы с flash памятью

*/
void s25fl_init( void )
{
	command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
	command.AddressSize       = QSPI_ADDRESS_24_BITS;
	command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	command.DdrMode           = QSPI_DDR_MODE_DISABLE;
	command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
	command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;
}	

uint8_t* s25fl_send_command( uint8_t command )
{
	uint8_t* result = NULL;
	
	return result;
}

/**
!Отправка сообщения в память
TODO: return и size
*/
uint8_t s25fl_send_to_mem( uint8_t address, uint8_t *data, uint8_t size )
{
	uint8_t result = 0x00;
	
	result = s25fl_send_write_enable();

	command.Instruction = PAGE_PROGRAM;
	command.AddressMode = QSPI_ADDRESS_1_LINE;
	command.Address = address;
	command.DataMode = QSPI_DATA_1_LINE;
	command.NbData = size;

	HAL_QSPI_Command(&hqspi, &command, HAL_QSPI_TIMEOUT_DEFAULT_VALUE);
	HAL_QSPI_Transmit(&hqspi, data, HAL_QSPI_TIMEOUT_DEFAULT_VALUE);
}

/**
!Получение данных из памяти
TODO 
*/
uint8_t s25fl_get_from_mem( uint8_t address, uint8_t *data )
{
	uint8_t result = 0x00;
	
	command.Instruction = READ_DATA;
	command.AddressMode = QSPI_ADDRESS_1_LINE;
	command.Address     = address;
	command.DataMode    = QSPI_DATA_1_LINE;
	command.DummyCycles = 0x00;

	HAL_QSPI_Command(&hqspi, &command, HAL_QSPI_TIMEOUT_DEFAULT_VALUE);
	HAL_QSPI_Receive(&hqspi, data, HAL_QSPI_TIMEOUT_DEFAULT_VALUE);

}

uint8_t s25fl_send_write_enable( void )
{
	uint8_t result = 0x00;
	
	command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
	command.Instruction       = WRITE_ENABLE;
	command.AddressMode       = QSPI_ADDRESS_NONE;
	command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	command.DataMode          = QSPI_DATA_NONE;
	command.DummyCycles       = 0;
	command.DdrMode           = QSPI_DDR_MODE_DISABLE;
	command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
	command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

	result = HAL_QSPI_Command(&hqspi, &command, HAL_QSPI_TIMEOUT_DEFAULT_VALUE);
	return result;
}
