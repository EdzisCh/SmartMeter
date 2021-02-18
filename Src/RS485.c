#include "RS485.h"

uint8_t rxBuffer[RX_BUFF_SIZE];
uint8_t rx_buf[10];
uint8_t i = 0;

int rxindex = 0; // index for going though rxString


/**
! Отправка одного байта по  RS485. После передачи пин RX/TX_485 
  устанавливается в 0. Используется как senchar(char ch)
*/
int rs485_send_byte( uint8_t byte )
{
	display_WIFI_on();
	int output;
	
	//Задержка для предотвращения потерь 
	HAL_Delay(10);
	
	HAL_GPIO_WritePin(RX_TX_485_GPIO_Port, RX_TX_485_Pin, GPIO_PIN_SET);
	output = HAL_UART_Transmit(&huart5, &byte, sizeof(byte), 100);
	HAL_GPIO_WritePin(RX_TX_485_GPIO_Port, RX_TX_485_Pin, GPIO_PIN_RESET);

	display_WIFI_off();
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
void rs485_start( void )
{
	HAL_GPIO_WritePin(RX_TX_485_GPIO_Port, RX_TX_485_Pin, GPIO_PIN_RESET);
	i = 0;

	HAL_UART_Receive_IT(&huart5, rxBuffer, RX_BUFF_SIZE);
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
	display_WIFI_on();
	
	HAL_GPIO_WritePin(RX_TX_485_GPIO_Port, RX_TX_485_Pin, GPIO_PIN_SET);
	HAL_UART_Transmit(UartHandle, rx_buf, sizeof(rx_buf), 100);
	HAL_GPIO_WritePin(RX_TX_485_GPIO_Port, RX_TX_485_Pin, GPIO_PIN_RESET);
	
	HAL_UART_Receive_IT(UartHandle, rxBuffer, RX_BUFF_SIZE);
	
	display_WIFI_off();
}

void UART5_IRQHandler(void)
{
	HAL_UART_IRQHandler(&huart5);
	
	UART_byte_proceed(&huart5);
	
}

void UART_byte_proceed(UART_HandleTypeDef* huart)
{
	if(huart->Instance == UART5)
	{
		huart->pRxBuffPtr--;
		uint8_t c = (uint8_t) *huart->pRxBuffPtr;
		rx_buf[i] = c;
		i++;		
		huart->pRxBuffPtr++;
		if(i == 10)
		{
			execute_command(rx_buf);
			i = 0;
			for(uint8_t k = 0; k < 10; k++)
			{
				rx_buf[k] = 0;
			}
		} 
		
	}
}
