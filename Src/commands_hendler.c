#include "commands_hendler.h"

const char* led_on_0 = "led_on_0\r\n";
const char* led_on_1 = "led_on_1\r\n";
const char* led_off = "led_off\r\n";

command_defenition regitered_commands[MAX_CMD_COUNT];

void commands_init()
{
	command_defenition led_on_act;
	led_on_act.command = led_on_0;
	led_on_act.callback_func = led_on_act_1;
	regitered_commands[0] = led_on_act;
}

/**
!Обработчик команд. Принимает данные из кольцевого буфера, формирует
стоку, проверяет наличие команды и вызывает соответствующую функцию
*/
void execute_command( ring_buf *ring, uint8_t count )
{
	
	uint8_t cmd[count+1];
	uint8_t i = 0;
	while(ring->out_index < ring->in_index)
	{
		cmd[i] = ring_buff_pop(ring);
		i++;
	}
	
	get_command(cmd);
//	if(compare_strings(cmd, led_on_1) == 0)
//	{
//		HAL_GPIO_WritePin(LED_ACT_GPIO_Port, LED_ACT_Pin, GPIO_PIN_SET);
//	} else if(compare_strings(cmd, led_on_2) == 0) {
//		HAL_GPIO_WritePin(LED_REACT_GPIO_Port, LED_REACT_Pin, GPIO_PIN_SET);
//	} else if(compare_strings(cmd, led_off) == 0) {
//		HAL_GPIO_WritePin(LED_ACT_GPIO_Port, LED_ACT_Pin, GPIO_PIN_RESET);
//		HAL_GPIO_WritePin(LED_REACT_GPIO_Port, LED_REACT_Pin, GPIO_PIN_RESET);
//	}
	
}

uint8_t compare_strings(uint8_t str_1[], const char* str_2)
{
	uint16_t size = strlen(str_2);
	
	for( uint16_t i = 0; i < size; i++)
	{
		if(str_1[i] != str_2[i])
		{
			return 1;
		}
	}
	
	return 0;
}

uint8_t get_command(uint8_t cmd[])
{
	uint8_t flag = 0;
	uint8_t i = 0;
	while(flag == 0)
	{
		if(compare_strings(cmd, regitered_commands[i].command) == 0)
		{
			regitered_commands[i].callback_func((const char*) cmd);
			return 0;
		}
		i++;
		if( i == MAX_CMD_COUNT)
		{
			return 0x01;
		}
	}
	return 0x02;
}

void led_on_act_1( const char* cmd )
{
	HAL_GPIO_WritePin(LED_ACT_GPIO_Port, LED_ACT_Pin, GPIO_PIN_SET);
}
