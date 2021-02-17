#include "stm32l4xx_hal.h"
#include "stm32l4xx.h"
#include "stm32l4xx_it.h"
#include "event_handler.h"
#include "RS485.h"

extern TIM_HandleTypeDef htim5;
extern UART_HandleTypeDef huart5;

void NMI_Handler(void)
{
	
}


void HardFault_Handler(void)
{
	
  while (1)
  {
	  
  }
  
}

void MemManage_Handler(void)
{
	
  while (1)
  {
	  
  }
}


void BusFault_Handler(void)
{
	
  while (1)
  {
	  
  }
  
}


void UsageFault_Handler(void)
{
	
  while (1)
  {
	  
  }
  
}


void SVC_Handler(void)
{
	
}


void DebugMon_Handler(void)
{
	
}


void PendSV_Handler(void)
{
	
}


void SysTick_Handler(void)
{
	
	GPIO_PinState key_1, key_2, key_3;

	key_1 = HAL_GPIO_ReadPin(KEY_1_GPIO_Port, KEY_1_Pin);
	key_2 = HAL_GPIO_ReadPin(KEY_2_GPIO_Port, KEY_2_Pin);
	key_3 = HAL_GPIO_ReadPin(KEY_3_GPIO_Port, KEY_3_Pin);

	if ( key_1 == GPIO_PIN_RESET || key_2 == GPIO_PIN_RESET || key_3 == GPIO_PIN_RESET )
	{
		event_handler_beep_on();
		
		while( key_1 == GPIO_PIN_RESET|| key_2 == GPIO_PIN_RESET || key_3 == GPIO_PIN_RESET )
		{
			key_1 = HAL_GPIO_ReadPin(KEY_1_GPIO_Port, KEY_1_Pin);
			key_2 = HAL_GPIO_ReadPin(KEY_2_GPIO_Port, KEY_2_Pin);
			key_3 = HAL_GPIO_ReadPin(KEY_3_GPIO_Port, KEY_3_Pin);
		}
	}
	event_handler_beep_off();
	
	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();
  /* USER CODE BEGIN SysTick_IRQn 1 */
	
  /* USER CODE END SysTick_IRQn 1 */
}


void TIM5_IRQHandler(void)
{
	
  HAL_TIM_IRQHandler(&htim5);
	
}


void UART5_IRQHandler(void)
{
	
	uint8_t message[10];
	rs485_get_message(message);
	
	if(rs485_is_received())
	{
		
		if(strcmp((const char *)message, "led_on_act") == 0)
		{
			HAL_GPIO_WritePin(LED_REACT_GPIO_Port, LED_ACT_Pin, GPIO_PIN_SET);
		}
		if(strcmp((const char *)message, "led_of_act") == 0)
		{
			HAL_GPIO_WritePin(LED_REACT_GPIO_Port, LED_ACT_Pin, GPIO_PIN_RESET);
		}
		
	}
	
	HAL_UART_IRQHandler(&huart5);
	
}

