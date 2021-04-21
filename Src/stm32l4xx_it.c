#include "stm32l4xx_hal.h"
#include "stm32l4xx.h"
#include "stm32l4xx_it.h"
#include "lcd.h"

extern TIM_HandleTypeDef htim5;
extern UART_HandleTypeDef huart5;
extern ADC_HandleTypeDef hadc1;

uint8_t vbat_source();


/**
* @brief This function handles Non maskable interrupt.
*/
void NMI_Handler(void)
{
  
}

/**
* @brief This function handles Hard fault interrupt.
*/
void HardFault_Handler(void)
{
  
  while (1)
  {
    
  }
  
}

/**
* @brief This function handles Memory management fault.
*/
void MemManage_Handler(void)
{
  
  while (1)
  {
    
  }
  
}

/**
* @brief This function handles Prefetch fault, memory access fault.
*/
void BusFault_Handler(void)
{
  
  while (1)
  {
    
  }
  
}

/**
* @brief This function handles Undefined instruction or illegal state.
*/
void UsageFault_Handler(void)
{
  
  while (1)
  {
    
  }
  
}

/**
* @brief This function handles System service call via SWI instruction.
*/
void SVC_Handler(void)
{
  
}

/**
* @brief This function handles Debug monitor.
*/
void DebugMon_Handler(void)
{
  
}

/**
* @brief This function handles Pendable request for system service.
*/
void PendSV_Handler(void)
{
  
}

/**
* @brief This function handles System tick timer.
*/
void SysTick_Handler(void)
{
	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();

	if(!vbat_source())
	{
	  display_battery_on();
	} else {
	  display_battery_off();
	}
	
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
	
	GPIO_PinState tamper_1, tamper_2;
	
	tamper_1 = HAL_GPIO_ReadPin(TAMPER1_GPIO_Port, TAMPER1_Pin);
	if(tamper_1 == GPIO_PIN_SET)
	{
		HAL_GPIO_WritePin(LED_ACT_GPIO_Port, LED_ACT_Pin, GPIO_PIN_SET);
	} else {
		HAL_GPIO_WritePin(LED_ACT_GPIO_Port, LED_ACT_Pin, GPIO_PIN_RESET);
	}
	
}

void TIM5_IRQHandler(void)
{
  
  HAL_TIM_IRQHandler(&htim5);
  
}

uint8_t vbat_source()
{
	HAL_ADC_Start(&hadc1);
	HAL_ADC_PollForConversion(&hadc1, 100);
	
	float Vbat_voltage = (float) HAL_ADC_GetValue(&hadc1)*16/4096;
    
	HAL_ADC_Stop(&hadc1);
	
	return Vbat_voltage >= 3.0;
}
