#include "commands_hendler.h"

/**
!Обработчик команд. Принимает строку, проверяет на ошибки/наличие
команды и вызывает соответствующую функцию
*/
void execute_command( uint8_t *command )
{
	if(strcmp((const char *)command, "led_on_act") == 0)
	{
		HAL_GPIO_WritePin(LED_ACT_GPIO_Port, LED_ACT_Pin, GPIO_PIN_SET);
	}
	if(strcmp((const char *)command, "led_of_act") == 0)
	{
		HAL_GPIO_WritePin(LED_ACT_GPIO_Port, LED_ACT_Pin, GPIO_PIN_RESET);
	}
}
