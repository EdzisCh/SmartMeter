#include "stm32l4xx_hal.h"
#include "stm32l4xx.h"
#include "stm32l4xx_it.h"

extern TIM_HandleTypeDef htim5;
extern ADC_HandleTypeDef hadc1;

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
	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();
}

void TIM5_IRQHandler(void)
{
  
  HAL_TIM_IRQHandler(&htim5);
  
}

void EXTI9_5_IRQHandler(void)
{
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_5);
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_6);
}

void EXTI15_10_IRQHandler(void)
{
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_13);
}
