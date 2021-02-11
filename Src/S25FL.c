#include "S25FL.h"

/**
!Отправка сообщения в память
TODO: return и size
*/
uint8_t s25fl_send_to_mem( uint32_t address, uint8_t *data, uint32_t size )
{
	uint8_t result = 0x00;
	QSPI_CommandTypeDef instruction;
	
	result = s25fl_send_write_enable();
	
	if (result != 0x00)
	{
		return result;
	}
	
	instruction.InstructionMode = QSPI_INSTRUCTION_1_LINE;
	instruction.Instruction = S25FL_PAGE_PROGRAM;
	
	instruction.AddressSize = QSPI_ADDRESS_24_BITS;
	instruction.AddressMode = QSPI_ADDRESS_1_LINE;
	instruction.Address = address;

	instruction.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	instruction.AlternateBytesSize = 0x00;
	instruction.AlternateBytes = 0x00;
	
	instruction.DummyCycles = 0x00;
	
	instruction.DataMode    = QSPI_DATA_1_LINE;
	instruction.NbData = size;

	instruction.DdrMode = QSPI_DDR_MODE_DISABLE;
	instruction.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
	instruction.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;

	result = HAL_QSPI_Command(&hqspi, &instruction, HAL_QSPI_TIMEOUT_DEFAULT_VALUE);
	
	if(result != 0x00)
	{
		return result;
	}
	
	result = HAL_QSPI_Transmit(&hqspi, data, HAL_QSPI_TIMEOUT_DEFAULT_VALUE);
	
	return result;
}

/**
!Получение данных из памяти
TODO 
*/
uint8_t s25fl_get_from_mem( uint32_t address, uint8_t *data, uint32_t size)
{
	uint8_t result = HAL_OK;
	
	QSPI_CommandTypeDef instruction;
	
	instruction.InstructionMode = QSPI_INSTRUCTION_1_LINE;
	instruction.Instruction = S25FL_READ_DATA;
	
	instruction.AddressSize = QSPI_ADDRESS_24_BITS;
	instruction.AddressMode = QSPI_ADDRESS_1_LINE;
	instruction.Address = address;

	instruction.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	
	instruction.DummyCycles = 0x00;
	
	instruction.DataMode = QSPI_DATA_1_LINE;
	instruction.NbData = size;

	instruction.DdrMode = QSPI_DDR_MODE_DISABLE;
	instruction.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
	instruction.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;

	result = HAL_QSPI_Command(&hqspi, &instruction, HAL_QSPI_TIMEOUT_DEFAULT_VALUE);
	
	if(result != HAL_OK)
	{
		return result;
	}
	
	result = HAL_QSPI_Receive(&hqspi, data, HAL_QSPI_TIMEOUT_DEFAULT_VALUE);

	return result;
}

uint8_t s25fl_send_write_enable( void )
{
	uint8_t result = HAL_OK;
	QSPI_CommandTypeDef instruction;

	instruction.InstructionMode = QSPI_INSTRUCTION_1_LINE;
	instruction.Instruction = S25FL_WRITE_ENABLE;
	
	instruction.AddressMode = QSPI_ADDRESS_1_LINE;

	instruction.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	instruction.AlternateBytesSize = 0x00;
	instruction.AlternateBytes = 0x00;
	
	instruction.DummyCycles = 0x00;
	
	instruction.DataMode    = QSPI_DATA_1_LINE;

	instruction.DdrMode = QSPI_DDR_MODE_DISABLE;
	instruction.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
	instruction.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;
	
	result = HAL_QSPI_Command(&hqspi, &instruction, HAL_QSPI_TIMEOUT_DEFAULT_VALUE);
	
	return result;
}

uint8_t s25fl_send_write_disable( void )
{
	uint8_t result = HAL_OK;
	QSPI_CommandTypeDef instruction;

	instruction.InstructionMode = QSPI_INSTRUCTION_1_LINE;
	instruction.Instruction = S25FL_WRITE_DISABLE;
	
	instruction.AddressMode = QSPI_ADDRESS_1_LINE;

	instruction.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	instruction.AlternateBytesSize = 0x00;
	instruction.AlternateBytes = 0x00;
	
	instruction.DummyCycles = 0x00;
	
	instruction.DataMode    = QSPI_DATA_1_LINE;

	instruction.DdrMode = QSPI_DDR_MODE_DISABLE;
	instruction.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
	instruction.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;

	result = HAL_QSPI_Command(&hqspi, &instruction, HAL_QSPI_TIMEOUT_DEFAULT_VALUE);
	
	return result;
}

/**
!

*/
uint8_t s25fl_sector_erase( uint8_t address )
{
	uint8_t result = HAL_OK;
	result = s25fl_send_write_enable();
	
	if (result != HAL_OK)
	{
		return result;
	}
	QSPI_CommandTypeDef instruction;

	instruction.InstructionMode = QSPI_INSTRUCTION_1_LINE;
	instruction.Instruction = S25FL_SECTOR_ERASE;
	
	instruction.AddressSize = QSPI_ADDRESS_24_BITS;
	instruction.AddressMode = QSPI_ADDRESS_1_LINE;
	instruction.Address = address;

	instruction.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	instruction.AlternateBytesSize = 0x00;
	instruction.AlternateBytes = 0x00;
	
	instruction.DummyCycles = 0x00;
	
	instruction.DataMode = QSPI_DATA_NONE;

	instruction.DdrMode = QSPI_DDR_MODE_DISABLE;
	instruction.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
	instruction.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;
	
	result = HAL_QSPI_Command(&hqspi, &instruction, HAL_QSPI_TIMEOUT_DEFAULT_VALUE);
	
	return result;
}

uint8_t s25fl_block_erase( uint8_t address )
{
	uint8_t result = HAL_OK;
	result = s25fl_send_write_enable();
	
	if (result != HAL_OK)
	{
		return result;
	}
	
	QSPI_CommandTypeDef instruction;

	instruction.InstructionMode = QSPI_INSTRUCTION_1_LINE;
	instruction.Instruction = S25FL_BLOCK_ERASE;
	
	instruction.AddressSize = QSPI_ADDRESS_24_BITS;
	instruction.AddressMode = QSPI_ADDRESS_1_LINE;
	instruction.Address = address;

	instruction.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	instruction.AlternateBytesSize = 0x00;
	instruction.AlternateBytes = 0x00;
	
	instruction.DummyCycles = 0x00;
	
	instruction.DataMode = QSPI_DATA_NONE;

	instruction.DdrMode = QSPI_DDR_MODE_DISABLE;
	instruction.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
	instruction.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;
	
	result = HAL_QSPI_Command(&hqspi, &instruction, HAL_QSPI_TIMEOUT_DEFAULT_VALUE);
	
	return result;
}

uint8_t s25fl_chip_erase( void )
{
	uint8_t result = HAL_OK;
	result = s25fl_send_write_enable();
	
	if (result != HAL_OK)
	{
		return result;
	}
	QSPI_CommandTypeDef instruction;

	instruction.InstructionMode = QSPI_INSTRUCTION_1_LINE;
	instruction.Instruction = S25FL_CHIP_ERASE;
	
	instruction.AddressMode = QSPI_ADDRESS_NONE;

	instruction.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	instruction.AlternateBytesSize = 0x00;
	instruction.AlternateBytes = 0x00;
	
	instruction.DummyCycles = 0x00;
	
	instruction.DataMode = QSPI_DATA_NONE;

	instruction.DdrMode = QSPI_DDR_MODE_DISABLE;
	instruction.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
	instruction.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;
	
	result = HAL_QSPI_Command(&hqspi, &instruction, HAL_QSPI_TIMEOUT_DEFAULT_VALUE);
	
	return result;
}

/**
!

*/
uint8_t s25fl_read_status_register( uint8_t *data )
{
	uint8_t result = HAL_OK;
	QSPI_CommandTypeDef instruction;
	
	instruction.InstructionMode = QSPI_INSTRUCTION_1_LINE;
	instruction.Instruction = S25FL_READ_STATUS_REG_2;
	
	instruction.AddressMode = QSPI_ADDRESS_NONE;

	instruction.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	instruction.AlternateBytesSize = 0x00;
	instruction.AlternateBytes = 0x00;
	
	instruction.DummyCycles = 0x00;
	
	instruction.DataMode = QSPI_DATA_1_LINE;
	instruction.NbData = 1;

	instruction.DdrMode = QSPI_DDR_MODE_DISABLE;
	instruction.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
	instruction.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;
	
	result = HAL_QSPI_Command(&hqspi, &instruction, HAL_QSPI_TIMEOUT_DEFAULT_VALUE);
	
	if (result != HAL_OK)
	{
		return result;
	}
	
	result = HAL_QSPI_Receive(&hqspi, data, HAL_QSPI_TIMEOUT_DEFAULT_VALUE);
	
	return result;
}

uint8_t s25fl_read_device_ID( uint8_t *data )
{
	uint8_t result = HAL_OK;
	QSPI_CommandTypeDef instruction;

	instruction.InstructionMode = QSPI_INSTRUCTION_1_LINE;
	instruction.Instruction = S25FL_DEV_ID;
	
	instruction.AddressMode = QSPI_ADDRESS_1_LINE;
	instruction.Address = 0x00;
	instruction.AddressSize = QSPI_ADDRESS_24_BITS;

	instruction.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	instruction.AlternateBytesSize = 0x00;
	instruction.AlternateBytes = 0x00;
	
	instruction.DummyCycles = 0x03;
	
	instruction.DataMode = QSPI_DATA_1_LINE;
	instruction.NbData = 6;

	instruction.DdrMode = QSPI_DDR_MODE_DISABLE;
	instruction.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
	instruction.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;
	
	result = HAL_QSPI_Command(&hqspi, &instruction, HAL_QSPI_TIMEOUT_DEFAULT_VALUE);
	
	if (result != HAL_OK)
	{
		return result;
	}
	
	result = HAL_QSPI_Receive(&hqspi, data, HAL_QSPI_TIMEOUT_DEFAULT_VALUE);
	
	return result;
}

uint8_t s25fl_release_from_DPD( uint8_t *data )
{
	uint8_t result = HAL_OK;
	QSPI_CommandTypeDef instruction;

	instruction.InstructionMode = QSPI_INSTRUCTION_1_LINE;
	instruction.Instruction = S25FL_RELEASE_POWER_DOWN;
	
	instruction.AddressMode = QSPI_ADDRESS_NONE;

	instruction.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	instruction.AlternateBytesSize = 0x00;
	instruction.AlternateBytes = 0x00;
	
	instruction.DummyCycles = 0x03;
	
	instruction.DataMode = QSPI_DATA_1_LINE;
	instruction.NbData = 1;

	instruction.DdrMode = QSPI_DDR_MODE_DISABLE;
	instruction.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
	instruction.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;
	
	result = HAL_QSPI_Command(&hqspi, &instruction, HAL_QSPI_TIMEOUT_DEFAULT_VALUE);
	
	if (result != HAL_OK)
	{
		return result;
	}
	
	result = HAL_QSPI_Receive(&hqspi, data, HAL_QSPI_TIMEOUT_DEFAULT_VALUE);
	
	return result;
}
