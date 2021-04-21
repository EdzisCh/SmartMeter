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
