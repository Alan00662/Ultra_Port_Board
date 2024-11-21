/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#include "stm32h7xx_hal.h"

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
#define LCD_RST_Pin GPIO_PIN_3
#define LCD_RST_GPIO_Port GPIOE
#define LCD_CS_Pin GPIO_PIN_4
#define LCD_CS_GPIO_Port GPIOE
#define GPIO1_Pin GPIO_PIN_0
#define GPIO1_GPIO_Port GPIOA
#define BAT_VOLT_Pin GPIO_PIN_1
#define BAT_VOLT_GPIO_Port GPIOA
#define TF_DETECT_Pin GPIO_PIN_5
#define TF_DETECT_GPIO_Port GPIOC
#define LED_R_Pin GPIO_PIN_12
#define LED_R_GPIO_Port GPIOB
#define LED_G_Pin GPIO_PIN_13
#define LED_G_GPIO_Port GPIOB
#define TP_RST_Pin GPIO_PIN_14
#define TP_RST_GPIO_Port GPIOD
#define TP_INT_Pin GPIO_PIN_15
#define TP_INT_GPIO_Port GPIOD
#define GPIO2_Pin GPIO_PIN_3
#define GPIO2_GPIO_Port GPIOB
#define GPIO3_Pin GPIO_PIN_4
#define GPIO3_GPIO_Port GPIOB
#define GPIO4_Pin GPIO_PIN_5
#define GPIO4_GPIO_Port GPIOB
#define LCD_BL_Pin GPIO_PIN_6
#define LCD_BL_GPIO_Port GPIOB
#define PWR_Pin GPIO_PIN_0
#define PWR_GPIO_Port GPIOE
#define BTN_UP_Pin GPIO_PIN_1
#define BTN_UP_GPIO_Port GPIOE

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
