#include "RS485.h"

ring_buf ring;
uint8_t buffer[RX_BUFF_SIZE];
uint16_t size = 80;
uint8_t count = 0;

/**
! Отправка одного байта по  RS485. После передачи пин RX/TX_485 
  устанавливается в 0. Используется как senchar(char ch)
*/
int rs485_send_byte( int byte )
{
	display_WIFI_on();
	int output;
	
	//Задержка для предотвращения потерь 
	HAL_Delay(40);
	
	HAL_GPIO_WritePin(RX_TX_485_GPIO_Port, RX_TX_485_Pin, GPIO_PIN_SET);
	output = HAL_UART_Transmit(&huart5, (uint8_t*)&byte, 1, 100);
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
! Инициализация кольцевого буфера, команд, включение прерываний по приему
*/
void rs485_start( void )
{
	HAL_GPIO_WritePin(RX_TX_485_GPIO_Port, RX_TX_485_Pin, GPIO_PIN_RESET);
	count = 0;
	ring_buff_init(&ring, buffer, size);
	cmd_handler_commands_init();
	
	__HAL_UART_ENABLE_IT(&huart5, UART_IT_RXNE);
}

/**
! Обработчик прерываний
*/
void UART5_IRQHandler(void)
{
	rs485_rx_byte_handler(&huart5);
	return;
	//HAL_UART_IRQHandler(&huart5);
	
}

/**
! Помещение входящего байта в кольцевой буффер и вызов комманды,
если входящий байт == '\n'
*/
void rs485_rx_byte_handler(UART_HandleTypeDef* huart)
{
        display_WIFI_on();
	if(huart->Instance == UART5)
	{
		if((huart->Instance->ISR & USART_ISR_RXNE) != RESET)
		{
			uint8_t c = (uint8_t) (huart->Instance->RDR & (uint8_t)0x00FF);
			
			ring_buff_put(c, &ring);
			count++;
			
			if(c == '\n')
			{
				cmd_handler_execute_command(&ring, count);
				ring_buff_clear(&ring);
				count = 0;
			}
		}
	}
        display_WIFI_off();
	return;
}
