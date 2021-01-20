#include "RS485.h"

void rs485_send_byte( uint8_t byte )
{
	if (HAL_GPIO_ReadPin(RX_TX_485_GPIO_Port, RX_TX_485_Pin))
	{
		HAL_UART_Transmit(&huart5, &byte, 1, 1000);
	} else {
		HAL_GPIO_WritePin(RX_TX_485_GPIO_Port, RX_TX_485_Pin, GPIO_PIN_SET);
		HAL_UART_Transmit(&huart5, &byte, 1, 1000);
		HAL_GPIO_WritePin(RX_TX_485_GPIO_Port, RX_TX_485_Pin, GPIO_PIN_RESET);
	}
}


void rs485_send_message( uint8_t *message, uint8_t size )
{
	if (HAL_GPIO_ReadPin(RX_TX_485_GPIO_Port, RX_TX_485_Pin))
	{
		HAL_UART_Transmit(&huart5, message, size, 1000);
	} else {
		HAL_GPIO_WritePin(RX_TX_485_GPIO_Port, RX_TX_485_Pin, GPIO_PIN_SET);
		HAL_UART_Transmit(&huart5, message, size, 1000);
		HAL_GPIO_WritePin(RX_TX_485_GPIO_Port, RX_TX_485_Pin, GPIO_PIN_RESET);
	}
}