#include "RS485.h"

/**
! Отправка одного байта по  RS485. После передачи пин RX/TX_485 
  устанавливается в 0. Используется как senchar(char ch)
*/
int rs485_send_byte( uint8_t byte )
{
	int output;
	
	//Задержка для предотвращения потерь 
	HAL_Delay(10);
	
	if (HAL_GPIO_ReadPin(RX_TX_485_GPIO_Port, RX_TX_485_Pin))
	{
		output = HAL_UART_Transmit(&huart5, &byte, 1, 1000);
	} else {
		HAL_GPIO_WritePin(RX_TX_485_GPIO_Port, RX_TX_485_Pin, GPIO_PIN_SET);
		output = HAL_UART_Transmit(&huart5, &byte, 1, 1000);
	}
	HAL_GPIO_WritePin(RX_TX_485_GPIO_Port, RX_TX_485_Pin, GPIO_PIN_RESET);
	
	return output;
}

/**
! Отправка байтов в количестве size по  RS485. После передачи пин RX/TX_485 
  устанавливается в 0
*/
void rs485_send_message( uint8_t *message, uint8_t size )
{
	if (HAL_GPIO_ReadPin(RX_TX_485_GPIO_Port, RX_TX_485_Pin))
	{
		HAL_UART_Transmit(&huart5, message, size, 1000);
	} else {
		HAL_GPIO_WritePin(RX_TX_485_GPIO_Port, RX_TX_485_Pin, GPIO_PIN_SET);
		HAL_UART_Transmit(&huart5, message, size, 1000);
	}
	HAL_GPIO_WritePin(RX_TX_485_GPIO_Port, RX_TX_485_Pin, GPIO_PIN_RESET);
}

/**
! Получение нескольких байт размеров size по  RS485. После передачи пин RX/TX_485 
  устанавливается в 0
*/
uint8_t rs485_get_message( uint8_t *message, uint8_t size )
{
	uint8_t status;
	
	if (HAL_GPIO_ReadPin(RX_TX_485_GPIO_Port, RX_TX_485_Pin))
	{
		HAL_GPIO_WritePin(RX_TX_485_GPIO_Port, RX_TX_485_Pin, GPIO_PIN_RESET);
		status = HAL_UART_Receive(&huart5, message, size, 100);
	} else {
		status = HAL_UART_Receive(&huart5, message, size, 100);	
	}
	HAL_GPIO_WritePin(RX_TX_485_GPIO_Port, RX_TX_485_Pin, GPIO_PIN_RESET);
	
	return status;
}

/**
!Если бит RXNE в UART_ISR присутствует, значит есть данные на входе
*/
uint8_t rs485_is_received( void )
{
	uint32_t isr_data = huart5.Instance->ISR;
	isr_data &= 0x00000020;
	if(isr_data != 0)
	{
		return 1;
	}
	
	return 0;
}
