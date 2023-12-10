/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define Usart_TX_Pin GPIO_PIN_2
#define Usart_TX_GPIO_Port GPIOA
#define Usart_RX_Pin GPIO_PIN_3
#define Usart_RX_GPIO_Port GPIOA
#define feeding_1_Pin GPIO_PIN_4
#define feeding_1_GPIO_Port GPIOA
#define feeding_2_Pin GPIO_PIN_5
#define feeding_2_GPIO_Port GPIOA
#define feeding_3_Pin GPIO_PIN_6
#define feeding_3_GPIO_Port GPIOA
#define clock_pon_Pin GPIO_PIN_15
#define clock_pon_GPIO_Port GPIOB
#define clock_tccn_Pin GPIO_PIN_8
#define clock_tccn_GPIO_Port GPIOA
#define wifi_tx_Pin GPIO_PIN_9
#define wifi_tx_GPIO_Port GPIOA
#define wifi_rx_Pin GPIO_PIN_10
#define wifi_rx_GPIO_Port GPIOA
#define OLED_SCL_Pin GPIO_PIN_6
#define OLED_SCL_GPIO_Port GPIOB
#define OLED_SDA_Pin GPIO_PIN_7
#define OLED_SDA_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
