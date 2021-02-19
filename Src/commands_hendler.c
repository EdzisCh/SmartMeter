#include "commands_hendler.h"

uint8_t led_on_1[] = "led_on_0\r\n";
uint8_t led_on_2[] = "led_on_1\r\n";
uint8_t led_off[] = "led_off\r\n";

/**
!Обработчик команд. Принимает строку, проверяет на ошибки/наличие
команды и вызывает соответствующую функцию
*/
void execute_command( ring_buf *ring, uint8_t count )
{
	
	uint8_t cmd[count];
	uint8_t i = 0;
	while(ring->out_index <= ring->in_index)
	{
		cmd[i] = ring_buff_pop(ring);
		i++;
	}
	
	HAL_GPIO_WritePin(RX_TX_485_GPIO_Port, RX_TX_485_Pin, GPIO_PIN_SET);
	HAL_UART_Transmit(&huart5, cmd, strlen((char*)cmd), 100);
	HAL_GPIO_WritePin(RX_TX_485_GPIO_Port, RX_TX_485_Pin, GPIO_PIN_RESET);
	
	if(compare_strings(cmd, led_on_1) == 0)
	{
		HAL_GPIO_WritePin(LED_ACT_GPIO_Port, LED_ACT_Pin, GPIO_PIN_SET);
	} else if(compare_strings(cmd, led_on_2) == 0) {
		HAL_GPIO_WritePin(LED_REACT_GPIO_Port, LED_REACT_Pin, GPIO_PIN_SET);
	} else if(compare_strings(cmd, led_off) == 0) {
		HAL_GPIO_WritePin(LED_ACT_GPIO_Port, LED_ACT_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(LED_REACT_GPIO_Port, LED_REACT_Pin, GPIO_PIN_RESET);
	}
	
}

uint8_t compare_strings(uint8_t str_1[], uint8_t str_2[])
{
	uint16_t size = strlen((const char *)str_2);
	
	for( uint16_t i = 0; i < size; i++)
	{
		if(str_1[i] != str_2[i])
		{
			return 1;
		}
	}
	
	return 0;
}