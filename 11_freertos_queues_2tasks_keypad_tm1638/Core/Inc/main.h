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
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "tm1638_led.h"
#include "keypad_4x4.h"
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
#define BTN_Pin GPIO_PIN_0
#define BTN_GPIO_Port GPIOA
#define BTN_EXTI_IRQn EXTI0_IRQn
#define SPI1_NSS_Pin GPIO_PIN_4
#define SPI1_NSS_GPIO_Port GPIOA
#define LD3_Pin GPIO_PIN_12
#define LD3_GPIO_Port GPIOD
#define LD4_Pin GPIO_PIN_13
#define LD4_GPIO_Port GPIOD
#define LD5_Pin GPIO_PIN_14
#define LD5_GPIO_Port GPIOD
#define LD6_Pin GPIO_PIN_15
#define LD6_GPIO_Port GPIOD
#define SWDIO_Pin GPIO_PIN_13
#define SWDIO_GPIO_Port GPIOA
#define SWCLK_Pin GPIO_PIN_14
#define SWCLK_GPIO_Port GPIOA
#define COL_0_Pin GPIO_PIN_0
#define COL_0_GPIO_Port GPIOD
#define COL_1_Pin GPIO_PIN_1
#define COL_1_GPIO_Port GPIOD
#define COL_2_Pin GPIO_PIN_2
#define COL_2_GPIO_Port GPIOD
#define COL_3_Pin GPIO_PIN_3
#define COL_3_GPIO_Port GPIOD
#define ROW_0_Pin GPIO_PIN_4
#define ROW_0_GPIO_Port GPIOD
#define ROW_1_Pin GPIO_PIN_5
#define ROW_1_GPIO_Port GPIOD
#define ROW_2_Pin GPIO_PIN_6
#define ROW_2_GPIO_Port GPIOD
#define ROW_3_Pin GPIO_PIN_7
#define ROW_3_GPIO_Port GPIOD
#define SWO_Pin GPIO_PIN_3
#define SWO_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */
struct queue_user {
	uint8_t pTime;
	uint8_t pAddr;
	uint8_t time[8];
	uint8_t addr1[8];
	uint8_t addr2[8];

};
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
