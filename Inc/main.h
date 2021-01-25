/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2021 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H__
#define __MAIN_H__

/* Includes ------------------------------------------------------------------*/

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private define ------------------------------------------------------------*/

#define KEY_3_Pin GPIO_PIN_5
#define KEY_3_GPIO_Port GPIOE
#define KEY_1_Pin GPIO_PIN_6
#define KEY_1_GPIO_Port GPIOE
#define KEY_2_Pin GPIO_PIN_13
#define KEY_2_GPIO_Port GPIOC
#define EXOSC32_Pin GPIO_PIN_14
#define EXOSC32_GPIO_Port GPIOC
#define XOSC32_Pin GPIO_PIN_15
#define XOSC32_GPIO_Port GPIOC
#define SDA_LCD_Pin GPIO_PIN_0
#define SDA_LCD_GPIO_Port GPIOF
#define SCL_LCD_Pin GPIO_PIN_1
#define SCL_LCD_GPIO_Port GPIOF
#define BEEP_Pin GPIO_PIN_9
#define BEEP_GPIO_Port GPIOF
#define OSC2_Pin GPIO_PIN_0
#define OSC2_GPIO_Port GPIOH
#define OSC1_Pin GPIO_PIN_1
#define OSC1_GPIO_Port GPIOH
#define TAMPER2_Pin GPIO_PIN_2
#define TAMPER2_GPIO_Port GPIOA
#define TAMPER1_Pin GPIO_PIN_3
#define TAMPER1_GPIO_Port GPIOA
#define CLK_MEM_Pin GPIO_PIN_10
#define CLK_MEM_GPIO_Port GPIOE
#define CS_MEM_Pin GPIO_PIN_11
#define CS_MEM_GPIO_Port GPIOE
#define MOSI_MEM_Pin GPIO_PIN_12
#define MOSI_MEM_GPIO_Port GPIOE
#define MISO_MEM_Pin GPIO_PIN_13
#define MISO_MEM_GPIO_Port GPIOE
#define TXMCU_L2_Pin GPIO_PIN_8
#define TXMCU_L2_GPIO_Port GPIOD
#define RXMCU_L2_Pin GPIO_PIN_9
#define RXMCU_L2_GPIO_Port GPIOD
#define SCL_MEM_Pin GPIO_PIN_7
#define SCL_MEM_GPIO_Port GPIOG
#define SDA_MEM_Pin GPIO_PIN_8
#define SDA_MEM_GPIO_Port GPIOG
#define TXMCU_L1_Pin GPIO_PIN_9
#define TXMCU_L1_GPIO_Port GPIOA
#define RXMCU_L1_Pin GPIO_PIN_10
#define RXMCU_L1_GPIO_Port GPIOA
#define SWD_DIO_Pin GPIO_PIN_13
#define SWD_DIO_GPIO_Port GPIOA
#define SWD_CLK_Pin GPIO_PIN_14
#define SWD_CLK_GPIO_Port GPIOA
#define MCU_TX_485_Pin GPIO_PIN_12
#define MCU_TX_485_GPIO_Port GPIOC
#define RX_TX_485_Pin GPIO_PIN_0
#define RX_TX_485_GPIO_Port GPIOD
#define LED_REACT_Pin GPIO_PIN_1
#define LED_REACT_GPIO_Port GPIOD
#define MCU_RX_485_Pin GPIO_PIN_2
#define MCU_RX_485_GPIO_Port GPIOD
#define LED_ACT_Pin GPIO_PIN_3
#define LED_ACT_GPIO_Port GPIOD
#define TXMCU_L3_Pin GPIO_PIN_5
#define TXMCU_L3_GPIO_Port GPIOD
#define RXMCU_L3_Pin GPIO_PIN_6
#define RXMCU_L3_GPIO_Port GPIOD
#define LED_BL_Pin GPIO_PIN_1
#define LED_BL_GPIO_Port GPIOE

/* ########################## Assert Selection ############################## */
/**
  * @brief Uncomment the line below to expanse the "assert_param" macro in the 
  *        HAL drivers code
  */
/* #define USE_FULL_ASSERT    1U */

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
 extern "C" {
#endif
void _Error_Handler(char *, int);

#define Error_Handler() _Error_Handler(__FILE__, __LINE__)
#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
