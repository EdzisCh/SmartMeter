#include "main.h"
#include "stm32l4xx_hal.h"
// /* USER CODE BEGIN Includes */
#include "lcd.h"
#include "RTC.h"
#include "CS5490.h"
#include "event_handler.h"
#include "mem_handler.h"
#include "RS485.h"
#include "stdio.h"
#include "M24M01.h"
//#include "S25FL.h"
#include "Tests.h"
#include "tariffs.h"
#include "pass_handler.h"
/* USER CODE END Includes */

ADC_HandleTypeDef hadc1;
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

uint8_t group_number;
uint8_t main_display_cycle;

data current_data_3;
data current_data_1;
data current_data_2;
total_energy_register TER;

CS5490 chip_L1;
CS5490 chip_L2;
CS5490 chip_L3;

uint8_t phase_1_enable = 0;
uint8_t phase_2_enable = 0;
uint8_t phase_3_enable = 0;
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
static void MX_ADC1_Init(void);

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);
void main_display_data_on_LCD(data *data, uint8_t group_number, uint8_t cycle);
  
int main(void)
{
	HAL_Init();
	SystemClock_Config();
		
    MX_RTC_Init();
	MX_GPIO_Init();
	MX_I2C2_Init();
	MX_UART5_Init();
	MX_USART1_UART_Init();
	MX_I2C3_Init();
	MX_QUADSPI_Init();
	MX_TIM5_Init();
	MX_USART2_UART_Init();
	MX_USART3_UART_Init();
	MX_ADC1_Init();
	
  /* USER CODE BEGIN 2 */

	printf("--START--\r\n");
                
	HAL_GPIO_WritePin(LED_BL_GPIO_Port, LED_BL_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(LED_ACT_GPIO_Port, LED_ACT_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LED_REACT_GPIO_Port, LED_REACT_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(RX_TX_485_GPIO_Port, RX_TX_485_Pin, GPIO_PIN_RESET);
	
	main_display_cycle = 0;
	group_number = 1;
	
	pass_handler_passwords_init();
	mem_handler_get_current_address_from_eeprom();
	display_init();
	display_clear();
	tariffs_init();
	event_handler_init_logs();
	rs485_start();
	HAL_TIM_PWM_Start(&htim5, TIM_CHANNEL_4);
	
	chip_L1.cs5490_huart = &huart1;
	chip_L2.cs5490_huart = &huart2;
	chip_L3.cs5490_huart = &huart3;
        
    uint8_t init = cs5490_init(&chip_L1, &chip_L2, &chip_L3);
	if(init == 0)
	{
		display_L1();
		display_L2();
		display_L3();
		phase_1_enable = 1;
		phase_2_enable = 1;
		phase_3_enable = 1;
	} else if(init == 2)
	{
		display_L1();
		phase_1_enable = 1;
	} else if(init == 3)
	{
		display_L1();
		display_L2();
		phase_1_enable = 1;
		phase_2_enable = 1;
	} 
	
	display_level(5);
	display_GR();
	display_PR();
	display_T();
	
	double Vrms_1 = 0;
	double Vrms_2 = 0;
	double Vrms_3 = 0;
	double Irms_1 = 0;
	double Irms_2 = 0;
	double Irms_3 = 0;
	double freq_1 = 0;
	double freq_2 = 0;
	double freq_3 = 0;
	double Pavg_1 = 0;
	double Pavg_2 = 0;
	double Pavg_3 = 0;
	double Qavg_1 = 0;
	double Qavg_2 = 0;
	double Qavg_3 = 0;
	double Savg_1 = 0;
	double Savg_2 = 0;
	double Savg_3 = 0;
	double PF_1 = 0;
	double PF_2 = 0;
	double PF_3 = 0;

	uint32_t timestamp[2];
	rtc_get_timestamp(timestamp);
	
//	uint8_t test_result = tests_run();
//	if(test_result)
//	{
//		display_ExMark();
//		display_main_numbers_double(test_result);
//		HAL_Delay(500);
//	}
	
  /* USER CODE END 2 */

  /* USER CODE BEGIN WHILE */
  while (1)
  {
		int32_t time_start = uwTick;
		
		Vrms_1 = cs5490_get_Vrms(&chip_L1);
		freq_1 = cs5490_get_freq(&chip_L1);
		Pavg_1 = cs5490_get_Pavg(&chip_L1);
		Irms_1 = cs5490_get_Irms(&chip_L1);
		Qavg_1 = cs5490_get_Qavg(&chip_L1);
		Savg_1 = cs5490_get_Savg(&chip_L1);
		PF_1 = cs5490_get_PF(&chip_L1);
			
		Vrms_2 = cs5490_get_Vrms(&chip_L2);
		freq_2 = cs5490_get_freq(&chip_L2);
		Pavg_2 = cs5490_get_Pavg(&chip_L2);
		Irms_2 = cs5490_get_Irms(&chip_L2);
		Qavg_2 = cs5490_get_Qavg(&chip_L2);
		Savg_2 = cs5490_get_Savg(&chip_L2);
		PF_2 = cs5490_get_PF(&chip_L2);
		
		Vrms_3 = cs5490_get_Vrms(&chip_L3);
		freq_3 = cs5490_get_freq(&chip_L3);
		Pavg_3 = cs5490_get_Pavg(&chip_L3);
		Irms_3 = cs5490_get_Irms(&chip_L3);
		Qavg_3 = cs5490_get_Qavg(&chip_L3);
		Savg_3 = cs5490_get_Savg(&chip_L3);
		PF_3 = cs5490_get_PF(&chip_L3);
		
		//накопление в РОНЭ и в тарифы
		mem_handler_set_I(&current_data_1, Irms_1);
		mem_handler_set_U(&current_data_1, Vrms_1);
		mem_handler_set_F(&current_data_1, freq_1);
		mem_handler_set_PF(&current_data_1, PF_1);
		mem_handler_set_P(&current_data_1, Pavg_1);
		mem_handler_set_Q(&current_data_1, Qavg_1);
		mem_handler_set_S(&current_data_1, Savg_1);
		
		mem_handler_set_I(&current_data_2, Irms_2);
		mem_handler_set_U(&current_data_2, Vrms_2);
		mem_handler_set_F(&current_data_2, freq_2);
		mem_handler_set_PF(&current_data_2, PF_2);
		mem_handler_set_P(&current_data_2, Pavg_2);
		mem_handler_set_Q(&current_data_2, Qavg_2);
		mem_handler_set_S(&current_data_2, Savg_2);
		
		mem_handler_set_I(&current_data_3, Irms_3);
		mem_handler_set_U(&current_data_3, Vrms_3);
		mem_handler_set_F(&current_data_3, freq_3);
		mem_handler_set_PF(&current_data_3, PF_3);
		mem_handler_set_P(&current_data_3, Pavg_3);
		mem_handler_set_Q(&current_data_3, Qavg_3);
		mem_handler_set_S(&current_data_3, Savg_3);
		
		mem_handler_set_total_energy_register(&TER, &current_data_1);
		mem_handler_set_total_energy_register(&TER, &current_data_2);
		mem_handler_set_total_energy_register(&TER, &current_data_3);
		
		tariffs_set_data(Pavg_1, Qavg_1);
		tariffs_set_data(Pavg_2, Qavg_2);
		tariffs_set_data(Pavg_3, Qavg_3);
		
		if(group_number == 1 && phase_1_enable)
		{
			main_display_data_on_LCD(&current_data_1, group_number, main_display_cycle);
		} else if (group_number == 2 && phase_2_enable)
		{
			main_display_data_on_LCD(&current_data_2, group_number, main_display_cycle);
		} else if (group_number == 3 && phase_3_enable)
		{
			main_display_data_on_LCD(&current_data_3, group_number, main_display_cycle);
		}
	  
		//блок формирования ретроспективы РОН и тарифов
		uint8_t new_date = rtc_date_update(timestamp);

		if(new_date != 0)
		{
			mem_handler_send_retrospective_to_eeprom(new_date, timestamp, &TER);
			tariffs_send_retrospective_to_eeprom(new_date, timestamp);
		}
		
		tariffs_update_plan(0, 0);
	  
		main_display_cycle++;
		if(main_display_cycle >= 7)
		{
			main_display_cycle = 0;
			group_number++;
		}
		if(group_number > 3)
		{
			group_number = 1;
		}
		
	    uint32_t delay = uwTick - time_start;
		uint32_t stop = uwTick;
		while(delay < 1000)
		{
			delay = uwTick - time_start;
		}
		//printf("Srart:%d Stop:%d Diff:%d Stop:%d\r\n", time_start, uwTick, delay, stop);
  /* USER CODE END WHILE */

  }

}

void main_display_data_on_LCD(data *data, uint8_t group_number, uint8_t cycle)
{
	display_write_group_number(group_number);
	
	if ( cycle == 0 )
	{
		display_main_numbers_double(data->U);
		display_V();
	} else if ( cycle == 1 ) 
	{
		display_main_numbers_double(data->F);
		display_Hz();
	} else if ( cycle == 2 ) 
	{
		display_main_numbers_double(data->P);
		display_W();
	} else if ( cycle == 3 )
	{
		display_main_numbers_double(data->I);
		display_A();
	} else if( cycle == 4 )
	{
		display_main_numbers_double(data->Q);
		display_VAr();
	} else if( cycle == 5)
	{
		display_main_numbers_double(data->S);
		display_VA();
	} else if( cycle == 6)
	{
		display_clear_units();
		display_main_numbers_double(data->PF);
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
                              |RCC_PERIPHCLK_I2C3|RCC_PERIPHCLK_ADC;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
  PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
  PeriphClkInit.Usart3ClockSelection = RCC_USART3CLKSOURCE_PCLK1;
  PeriphClkInit.Uart5ClockSelection = RCC_UART5CLKSOURCE_PCLK1;
  PeriphClkInit.I2c2ClockSelection = RCC_I2C2CLKSOURCE_PCLK1;
  PeriphClkInit.I2c3ClockSelection = RCC_I2C3CLKSOURCE_PCLK1;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
  PeriphClkInit.AdcClockSelection = RCC_ADCCLKSOURCE_PLLSAI1;
  PeriphClkInit.PLLSAI1.PLLSAI1Source = RCC_PLLSOURCE_HSE;
  PeriphClkInit.PLLSAI1.PLLSAI1M = 4;
  PeriphClkInit.PLLSAI1.PLLSAI1N = 8;
  PeriphClkInit.PLLSAI1.PLLSAI1P = RCC_PLLP_DIV7;
  PeriphClkInit.PLLSAI1.PLLSAI1Q = RCC_PLLQ_DIV2;
  PeriphClkInit.PLLSAI1.PLLSAI1R = RCC_PLLR_DIV2;
  PeriphClkInit.PLLSAI1.PLLSAI1ClockOut = RCC_PLLSAI1_ADC1CLK;
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

/* ADC1 init function */
static void MX_ADC1_Init(void)
{

  ADC_MultiModeTypeDef multimode;
  ADC_ChannelConfTypeDef sConfig;

    /**Common config 
    */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hadc1.Init.LowPowerAutoWait = DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.DMAContinuousRequests = DISABLE;
  hadc1.Init.Overrun = ADC_OVR_DATA_PRESERVED;
  hadc1.Init.OversamplingMode = DISABLE;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the ADC multi-mode 
    */
  multimode.Mode = ADC_MODE_INDEPENDENT;
  if (HAL_ADCEx_MultiModeConfigChannel(&hadc1, &multimode) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure Regular Channel 
    */
  sConfig.Channel = ADC_CHANNEL_VBAT;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_2CYCLES_5;
  sConfig.SingleDiff = ADC_SINGLE_ENDED;
  sConfig.OffsetNumber = ADC_OFFSET_NONE;
  sConfig.Offset = 0;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

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

  
  if (HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR1) != 0x32F2)
  {
   rtc_set_init_dateTime();
   HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR1, 0x32F2);
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
