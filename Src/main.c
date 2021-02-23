#include "main.h"
#include "stm32l4xx_hal.h"

/* USER CODE BEGIN Includes */
#include "lcd.h"
#include "RTC.h"
#include "CS5490.h"
#include "event_handler.h"
#include "mem_handler.h"
#include "RS485.h"
#include "stdio.h"
#include "M24M01.h"
#include "S25FL.h"
#include "Tests.h"
#include "string.h"
/* USER CODE END Includes */

I2C_HandleTypeDef hi2c2;
I2C_HandleTypeDef hi2c3;

QSPI_HandleTypeDef hqspi;

RTC_HandleTypeDef hrtc;

TIM_HandleTypeDef htim5;

UART_HandleTypeDef huart5;
UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;
UART_HandleTypeDef huart3;

/* USER CODE BEGIN PV */
extern __IO uint32_t uwTick;
uint8_t cycle;
/* USER CODE END PV */

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C2_Init(void);
static void MX_UART5_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_I2C3_Init(void);
static void MX_QUADSPI_Init(void);
static void MX_RTC_Init(void);
static void MX_TIM5_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_USART3_UART_Init(void);

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);
  
int main(void)
{
	HAL_Init();
	SystemClock_Config();
	
	MX_GPIO_Init();
	MX_I2C2_Init();
	MX_UART5_Init();
	MX_USART1_UART_Init();
	MX_I2C3_Init();
	MX_QUADSPI_Init();
	MX_RTC_Init();
	MX_TIM5_Init();
	MX_USART2_UART_Init();
	MX_USART3_UART_Init();
	
  /* USER CODE BEGIN 2 */

	printf("--START--\r\n");
	HAL_GPIO_WritePin(LED_BL_GPIO_Port, LED_BL_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(LED_ACT_GPIO_Port, LED_ACT_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LED_REACT_GPIO_Port, LED_REACT_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(RX_TX_485_GPIO_Port, RX_TX_485_Pin, GPIO_PIN_RESET);
	HAL_TIM_PWM_Start(&htim5, TIM_CHANNEL_4);
	cycle = 1;

	display_init();

	display_clear();
	
	rtc_set_init_dateTime();

	CS5490 chip_L1;
	chip_L1.cs5490_huart = &huart1;
	CS5490 chip_L2;
	chip_L2.cs5490_huart = &huart2;
	CS5490 chip_L3;
	chip_L3.cs5490_huart = &huart3;
	
	cs5490_full_callibration(&chip_L3);
	
	if(!cs5490_init(&chip_L1))
	{
		display_L1();
	}
	if(!cs5490_init(&chip_L2))
	{
		display_L2();
	}
	if(cs5490_init(&chip_L3) == 0)
	{
		display_L3();
	}
	
	//Включение необходимых элементов дисплея
	display_battery();
	display_N();
	display_level(5);
	
	double Vrms;
	double Irms;
	double freq;
	double Pavg;
	
	data data;
	total_energy_register TER;
	
	uint32_t timestamp[2];
	rtc_get_timestamp(timestamp);
	
	uint8_t test_result = tests_run();
	if(test_result)
	{
		display_ExMark();
		display_main_numbers_double(test_result);
		HAL_Delay(500);
	}
	
	rs485_start();
	display_clear_units();
  /* USER CODE END 2 */

  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  uint32_t time_start = uwTick;
	  
	  //измерения
	  Vrms = cs5490_get_Vrms(&chip_L3);
	  Irms = cs5490_get_Irms(&chip_L3);
	  freq = cs5490_get_freq(&chip_L3);
	  freq *= 4000;
	  Pavg = cs5490_get_Pavg(&chip_L3);
	  
	  //накопление
	  mem_handler_set_data(&data, Pavg, 0, 0, 0, 0, Vrms, freq);
	  mem_handler_set_total_energy_register(&TER, &data);
	  
	  //индикация
	  //выводится только одно значение в 5 циклов
	  //воеменная реализация 
	  display_energy_clear();
	  if ( cycle <= 5 )
	  {
		  display_main_numbers_double(Vrms);
		  display_V();
	  } else if ( cycle > 5 && cycle <= 10 ) 
	  {
		  display_main_numbers_double(freq);
		  display_Hz();
	  } else if ( cycle > 10 && cycle <= 15 ) 
	  {
		  display_main_numbers_double(Pavg);
		  display_W();
		  display_active_consumed_energy();
	  } else {
		  display_main_numbers_double(Irms);
		  display_A();
	  }
	  
	  cycle++;
	  if(cycle == 21)
	  {
		  cycle = 1;
	  }
	  
	  //блок формирования ретроспективы РОН
	  uint8_t new_date = rtc_date_update(timestamp);
	  
	  if(new_date != 0)
	  {
		  mem_handler_send_retrospective_to_eeprom(new_date, timestamp, &TER);
	  }
	  
	  //---тарифы
		//блок формирования ретроспективы аналогично регистрам общего накопления
	  
	  //---rs485
	  
	  
	  //события
	  
	  uint32_t time_stop = uwTick;
	  //printf("\r\nSrart:%d Stop:%d Diff:%d\r\n", time_start, time_stop, time_stop - time_start);
  /* USER CODE END WHILE */

  }

}

void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_PeriphCLKInitTypeDef PeriphClkInit;

    /**Configure LSE Drive Capability 
    */
  HAL_PWR_EnableBkUpAccess();

  __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE|RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 16;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC|RCC_PERIPHCLK_USART1
                              |RCC_PERIPHCLK_USART2|RCC_PERIPHCLK_USART3
                              |RCC_PERIPHCLK_UART5|RCC_PERIPHCLK_I2C2
                              |RCC_PERIPHCLK_I2C3;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
  PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
  PeriphClkInit.Usart3ClockSelection = RCC_USART3CLKSOURCE_PCLK1;
  PeriphClkInit.Uart5ClockSelection = RCC_UART5CLKSOURCE_PCLK1;
  PeriphClkInit.I2c2ClockSelection = RCC_I2C2CLKSOURCE_PCLK1;
  PeriphClkInit.I2c3ClockSelection = RCC_I2C3CLKSOURCE_PCLK1;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the main internal regulator output voltage 
    */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* I2C2 init function */
static void MX_I2C2_Init(void)
{

  hi2c2.Instance = I2C2;
  hi2c2.Init.Timing = 0x10909CEC;
  hi2c2.Init.OwnAddress1 = 0;
  hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c2.Init.OwnAddress2 = 0;
  hi2c2.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure Analogue filter 
    */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c2, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure Digital filter 
    */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c2, 0) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* I2C3 init function */
static void MX_I2C3_Init(void)
{

  hi2c3.Instance = I2C3;
  hi2c3.Init.Timing = 0x10909CEC;
  hi2c3.Init.OwnAddress1 = 0;
  hi2c3.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c3.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c3.Init.OwnAddress2 = 0;
  hi2c3.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c3.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c3.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c3) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure Analogue filter 
    */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c3, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure Digital filter 
    */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c3, 0) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* QUADSPI init function */
static void MX_QUADSPI_Init(void)
{

  /* QUADSPI parameter configuration*/
  hqspi.Instance = QUADSPI;
  hqspi.Init.ClockPrescaler = 255;
  hqspi.Init.FifoThreshold = 1;
  hqspi.Init.SampleShifting = QSPI_SAMPLE_SHIFTING_NONE;
  hqspi.Init.FlashSize = 1;
  hqspi.Init.ChipSelectHighTime = QSPI_CS_HIGH_TIME_1_CYCLE;
  hqspi.Init.ClockMode = QSPI_CLOCK_MODE_0;
  if (HAL_QSPI_Init(&hqspi) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* RTC init function */
static void MX_RTC_Init(void)
{
	
  RTC_TimeTypeDef sTime;
  RTC_DateTypeDef sDate;
	

    /**Initialize RTC Only 
    */
  hrtc.Instance = RTC;
  hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
  hrtc.Init.AsynchPrediv = 127;
  hrtc.Init.SynchPrediv = 255;
  hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
  hrtc.Init.OutPutRemap = RTC_OUTPUT_REMAP_NONE;
  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initialize RTC and set the Time and Date 
    */
  sTime.Hours = 0x0;
  sTime.Minutes = 0x0;
  sTime.Seconds = 0x0;
  sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sTime.StoreOperation = RTC_STOREOPERATION_RESET;
  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sDate.WeekDay = RTC_WEEKDAY_MONDAY;
  sDate.Month = RTC_MONTH_JANUARY;
  sDate.Date = 0x1;
  sDate.Year = 0x0;

  if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BCD) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* TIM5 init function */
static void MX_TIM5_Init(void)
{

  TIM_ClockConfigTypeDef sClockSourceConfig;
  TIM_MasterConfigTypeDef sMasterConfig;
  TIM_OC_InitTypeDef sConfigOC;

  htim5.Instance = TIM5;
  htim5.Init.Prescaler = 2000;
  htim5.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim5.Init.Period = 2;
  htim5.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim5.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim5) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim5, &sClockSourceConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  if (HAL_TIM_PWM_Init(&htim5) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim5, &sMasterConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim5, &sConfigOC, TIM_CHANNEL_4) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  HAL_TIM_MspPostInit(&htim5);

}

/* UART5 init function */
static void MX_UART5_Init(void)
{

  huart5.Instance = UART5;
  huart5.Init.BaudRate = 2400;
  huart5.Init.WordLength = UART_WORDLENGTH_8B;
  huart5.Init.StopBits = UART_STOPBITS_1;
  huart5.Init.Parity = UART_PARITY_NONE;
  huart5.Init.Mode = UART_MODE_TX_RX;
  huart5.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart5.Init.OverSampling = UART_OVERSAMPLING_16;
  huart5.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart5.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart5) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* USART1 init function */
static void MX_USART1_UART_Init(void)
{

  huart1.Instance = USART1;
  huart1.Init.BaudRate = 2400;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* USART2 init function */
static void MX_USART2_UART_Init(void)
{

  huart2.Instance = USART2;
  huart2.Init.BaudRate = 2400;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* USART3 init function */
static void MX_USART3_UART_Init(void)
{

  huart3.Instance = USART3;
  huart3.Init.BaudRate = 2400;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  huart3.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart3.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

static void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  HAL_PWREx_EnableVddIO2();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, RX_TX_485_Pin|LED_REACT_Pin|LED_ACT_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LED_BL_GPIO_Port, LED_BL_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : KEY_3_Pin KEY_1_Pin */
  GPIO_InitStruct.Pin = KEY_3_Pin|KEY_1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pin : KEY_2_Pin */
  GPIO_InitStruct.Pin = KEY_2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(KEY_2_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : TAMPER2_Pin TAMPER1_Pin */
  GPIO_InitStruct.Pin = TAMPER2_Pin|TAMPER1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : RX_TX_485_Pin LED_REACT_Pin LED_ACT_Pin */
  GPIO_InitStruct.Pin = RX_TX_485_Pin|LED_REACT_Pin|LED_ACT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pin : LED_BL_Pin */
  GPIO_InitStruct.Pin = LED_BL_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LED_BL_GPIO_Port, &GPIO_InitStruct);

}

void _Error_Handler(char *file, int line)
{
  /* User can add his own implementation to report the HAL error return state */
  while(1)
  {
  }
  
}

#ifdef  USE_FULL_ASSERT

void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif 
