/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
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


/*
 * ----------connect diagram----------------
 *
 *
 * KEYPAD 4X4	 			STM32F411
 * COL_0 (PIN_4)	--> 	PD0
 * COL_1 (PIN_3)	--> 	PD1
 * COL_2 (PIN_2)	--> 	PD2
 * COL_3 (PIN_1)	--> 	PD3
 * ROW_0 (PIN_8)	--> 	PD4
 * ROW_1 (PIN_7)	--> 	PD5
 * ROW_2 (PIN_6)	--> 	PD6
 * ROW_3 (PIN_5)	--> 	PD7
 *
 *
 * */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "FreeRTOS.h"
#include "task.h"
#include <stdio.h>
#include "queue.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
SPI_HandleTypeDef hspi1;

/* USER CODE BEGIN PV */
#define DWT_CTRL    (*(volatile uint32_t*)0xE0001000)

TaskHandle_t tm1638;
TaskHandle_t keypad;

QueueHandle_t qdata_key;

struct data {
	uint32_t id;
	uint32_t number;
};

uint8_t address_led[8] = { ADDR_1, ADDR_3, ADDR_5, ADDR_7, ADDR_9, ADDR_11,
ADDR_13, ADDR_15 };
uint8_t address_number[] = { ADDR_0, ADDR_2, ADDR_4, ADDR_6, ADDR_8, ADDR_10,
ADDR_12, ADDR_14 };

uint32_t var_time_down = 0;
uint32_t var_time_up = 0;
uint32_t var_time_right = 0;

uint8_t state0 = 0;


struct queue_user qtime;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);
/* USER CODE BEGIN PFP */

extern void SEGGER_SYSVIEW_Conf(void);
extern void SEGGER_SYSVIEW_Start(void);

static void handle_tm1638(void *parameters);
void btn_user(void);
static void handle_keypad(void *parameters);

void MODE_A(void);
void MODE_B(void);
void MODE_C(void);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
extern void SEGGER_UART_init(uint32_t);

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void) {
	/* USER CODE BEGIN 1 */
//	qtime.pTime = 1;
//	qtime.pAddr = 0;
//	create_addr();
	BaseType_t status;
	/* USER CODE END 1 */

	/* MCU Configuration--------------------------------------------------------*/

	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	/* USER CODE BEGIN Init */

	/* USER CODE END Init */

	/* Configure the system clock */
	SystemClock_Config();

	/* USER CODE BEGIN SysInit */

	/* USER CODE END SysInit */

	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	MX_SPI1_Init();
	/* USER CODE BEGIN 2 */
	tm1638_init();

	//Enable the CYCCNT counter.
	DWT_CTRL |= (1 << 0);

	SEGGER_SYSVIEW_Conf();
	SEGGER_SYSVIEW_Start();

	SEGGER_UART_init(500000);

	status = xTaskCreate(handle_tm1638, "tm1638", 5000, "this is task tm1638",
			2, &tm1638);
	configASSERT(status == pdPASS);

	status = xTaskCreate(handle_keypad, "keypad_task", 500,
			"this is task keypad", 3, &keypad);
	configASSERT(status == pdPASS);

	qdata_key = xQueueCreate(1, sizeof(char));
	if (qdata_key == NULL) {
		/* Queue was not created and must not be used. */
	}

	vTaskStartScheduler();
	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1) {
		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */
	}
	/* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void) {
	RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
	RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };

	/** Configure the main internal regulator output voltage
	 */
	__HAL_RCC_PWR_CLK_ENABLE();
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
	/** Initializes the RCC Oscillators according to the specified parameters
	 * in the RCC_OscInitTypeDef structure.
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLM = 4;
	RCC_OscInitStruct.PLL.PLLN = 200;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
	RCC_OscInitStruct.PLL.PLLQ = 8;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		Error_Handler();
	}
	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
			| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK) {
		Error_Handler();
	}
}

/**
 * @brief SPI1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_SPI1_Init(void) {

	/* USER CODE BEGIN SPI1_Init 0 */

	/* USER CODE END SPI1_Init 0 */

	/* USER CODE BEGIN SPI1_Init 1 */

	/* USER CODE END SPI1_Init 1 */
	/* SPI1 parameter configuration*/
	hspi1.Instance = SPI1;
	hspi1.Init.Mode = SPI_MODE_MASTER;
	hspi1.Init.Direction = SPI_DIRECTION_1LINE;
	hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
	hspi1.Init.CLKPolarity = SPI_POLARITY_HIGH;
	hspi1.Init.CLKPhase = SPI_PHASE_2EDGE;
	hspi1.Init.NSS = SPI_NSS_SOFT;
	hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_128;
	hspi1.Init.FirstBit = SPI_FIRSTBIT_LSB;
	hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
	hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
	hspi1.Init.CRCPolynomial = 10;
	if (HAL_SPI_Init(&hspi1) != HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN SPI1_Init 2 */

	/* USER CODE END SPI1_Init 2 */

}

/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_GPIO_Init(void) {
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOH_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(SPI1_NSS_GPIO_Port, SPI1_NSS_Pin, GPIO_PIN_SET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOD,
			LD3_Pin | LD4_Pin | LD5_Pin | LD6_Pin | ROW_0_Pin | ROW_1_Pin
					| ROW_2_Pin | ROW_3_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pin : BTN_Pin */
	GPIO_InitStruct.Pin = BTN_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	HAL_GPIO_Init(BTN_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pin : SPI1_NSS_Pin */
	GPIO_InitStruct.Pin = SPI1_NSS_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(SPI1_NSS_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pins : LD3_Pin LD4_Pin LD5_Pin LD6_Pin */
	GPIO_InitStruct.Pin = LD3_Pin | LD4_Pin | LD5_Pin | LD6_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

	/*Configure GPIO pins : COL_0_Pin COL_1_Pin COL_2_Pin COL_3_Pin */
	GPIO_InitStruct.Pin = COL_0_Pin | COL_1_Pin | COL_2_Pin | COL_3_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

	/*Configure GPIO pins : ROW_0_Pin ROW_1_Pin ROW_2_Pin ROW_3_Pin */
	GPIO_InitStruct.Pin = ROW_0_Pin | ROW_1_Pin | ROW_2_Pin | ROW_3_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

	/* EXTI interrupt init*/
	HAL_NVIC_SetPriority(EXTI0_IRQn, 7, 0);
	HAL_NVIC_EnableIRQ(EXTI0_IRQn);

}

/* USER CODE BEGIN 4 */
void handle_tm1638(void *parameters) {
	char data_receive;
	char data_temp;
	char msg[100];
	while (1) {
		snprintf(msg, 100, "%s\n", (char*) parameters);
		SEGGER_SYSVIEW_PrintfTarget(msg);
		if (qdata_key != NULL) {
			if (xQueueReceive(qdata_key, &data_receive,
					(TickType_t) 100) == pdPASS) {

				if (data_receive == 'A' || data_receive == 'B') {
					data_temp = data_receive;
					data_receive = 'x';
					tm1638_reset();
					if (data_temp == 'A') {
						state0 = 0;
						var_time_down = 0;
						tm1638_toggle_led(address_led[0]);
						var_time_down = keypad_enter();

					} else if (data_temp == 'B') {
						var_time_up = 0;
						tm1638_toggle_led(address_led[1]);
						var_time_up = keypad_enter();

					}

				}
			}
		}

		if (data_temp == 'A') {
			tm1638_cnt_down_non_bl(var_time_down);
			if (var_time_down > 0)
				var_time_down--;
			else {
				data_temp = 'x';
			}
		} else if (data_temp == 'B') {
			tm1638_cnt_up_non_bl(var_time_up);
			if (var_time_up < 100000000)
				var_time_up++;
			else
				data_temp = 'x';
		}
	}
}
void handle_keypad(void *parameters) {
	char data_trans;
	while (1) {
		data_trans = keypad_read();
		if (data_trans == 'C') {
			vTaskSuspend(tm1638);
		} else if (data_trans == 'D') {
			vTaskResume(tm1638);
		}
		if (data_trans != 'x') {
			xQueueSend(qdata_key, (void* )&data_trans, portMAX_DELAY);

		}
		vTaskDelay(pdMS_TO_TICKS(50));
	}
}

/* USER CODE END 4 */

/**
 * @brief  Period elapsed callback in non blocking mode
 * @note   This function is called  when TIM5 interrupt took place, inside
 * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
 * a global variable "uwTick" used as application time base.
 * @param  htim : TIM handle
 * @retval None
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	/* USER CODE BEGIN Callback 0 */

	/* USER CODE END Callback 0 */
	if (htim->Instance == TIM5) {
		HAL_IncTick();
	}
	/* USER CODE BEGIN Callback 1 */

	/* USER CODE END Callback 1 */
}

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void) {
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1) {
	}
	/* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

