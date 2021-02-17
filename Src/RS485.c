#include "RS485.h"

extern __IO uint32_t uwTick;
uint8_t rxBuffer[10];
uint8_t txBuffer[10];
uint8_t first_byte_wait = RESET;
uint8_t timeout = 0;
uint8_t isRx = RESET;


/**
! Отправка одного байта по  RS485. После передачи пин RX/TX_485 
  устанавливается в 0. Используется как senchar(char ch)
*/
int rs485_send_byte( uint8_t byte )
{
	int output;
	
	//Задержка для предотвращения потерь 
	HAL_Delay(10);
	
	HAL_GPIO_WritePin(RX_TX_485_GPIO_Port, RX_TX_485_Pin, GPIO_PIN_SET);
	output = HAL_UART_Transmit_IT(&huart5, &byte, sizeof(byte));
	HAL_GPIO_WritePin(RX_TX_485_GPIO_Port, RX_TX_485_Pin, GPIO_PIN_RESET);

	return output;
}

/**
! Отправка байтов в количестве size по  RS485. После передачи пин RX/TX_485 
  устанавливается в 0
*/
uint8_t rs485_send_message( uint8_t *message, uint8_t size )
{
	uint8_t status = 0;
	
	HAL_GPIO_WritePin(RX_TX_485_GPIO_Port, RX_TX_485_Pin, GPIO_PIN_SET);
	status = HAL_UART_Transmit_IT(&huart5, message, size);
	HAL_GPIO_WritePin(RX_TX_485_GPIO_Port, RX_TX_485_Pin, GPIO_PIN_RESET);
	
	return status;
}

/**
! Получение нескольких байт размеров size по  RS485. После передачи пин RX/TX_485 
  устанавливается в 0
*/
uint8_t rs485_get_message( uint8_t *message )
{
	uint8_t status;
	HAL_GPIO_WritePin(RX_TX_485_GPIO_Port, RX_TX_485_Pin, GPIO_PIN_RESET);
	first_byte_wait = SET;
	
	HAL_UART_Receive_IT(&huart5, rxBuffer, 10);

	for(uint8_t i = 0; i < 10; i++)
	{
		message[i] = rxBuffer[i];
	}
	
	return status;
}

/**
!Если бит RXNE в UART_ISR присутствует, значит есть данные на входе
*/
uint8_t rs485_is_received( void )
{
	if(isRx == SET)
	{
		isRx = RESET;
		return 1;
	}
	
	return 0;
}


uint8_t rs485_proceed( void )
{
	
	return 0x00;
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *UartHandle)
{
	uint8_t this_data = UartHandle->Instance->TDR;
	
	HAL_GPIO_WritePin(RX_TX_485_GPIO_Port, RX_TX_485_Pin, GPIO_PIN_SET);
	HAL_UART_Transmit(UartHandle, &this_data, 1, 100);
	HAL_GPIO_WritePin(RX_TX_485_GPIO_Port, RX_TX_485_Pin, GPIO_PIN_RESET);
	
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle)
{
	HAL_GPIO_WritePin(RX_TX_485_GPIO_Port, RX_TX_485_Pin, GPIO_PIN_SET);
	HAL_UART_Transmit(UartHandle, rxBuffer, 10, 100);
	HAL_GPIO_WritePin(RX_TX_485_GPIO_Port, RX_TX_485_Pin, GPIO_PIN_RESET);
	isRx = SET;
	
	HAL_UART_Receive_IT(UartHandle, rxBuffer, 10);
		
}

 void HAL_UART_ErrorCallback(UART_HandleTypeDef *UartHandle)
{
	HAL_GPIO_WritePin(LED_REACT_GPIO_Port, LED_REACT_Pin, GPIO_PIN_SET);
	for(uint8_t i = 0; i < 10; i++)
	{
		rxBuffer[i] = 0;
	}
	HAL_GPIO_WritePin(LED_REACT_GPIO_Port, LED_REACT_Pin, GPIO_PIN_RESET);
}
