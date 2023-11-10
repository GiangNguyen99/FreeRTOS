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
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "FreeRTOS.h"
#include "task.h"

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

/* USER CODE BEGIN PV */

#define DWT_CTRL    (*(volatile uint32_t*)0xE0001000)

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
/* USER CODE BEGIN PFP */
static void task_led_green(void *parameter);
static void task_led_orange(void *parameter);
static void task_led_red(void *parameter);
static void task_led_blue(void *parameter);

TaskHandle_t taskHandle_led_green;
TaskHandle_t taskHandle_led_orange;
TaskHandle_t taskHandle_led_red;
TaskHandle_t taskHandle_led_blue;
TaskHandle_t taskHandle_btn;

volatile TaskHandle_t next_task_handle = NULL;

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
	/* USER CODE BEGIN 2 */

	//Enable the CYCCNT counter.
	DWT_CTRL |= (1 << 0);

	SEGGER_SYSVIEW_Conf();
	SEGGER_SYSVIEW_Start();

	SEGGER_UART_init(500000);

	/*
	 * 		LED
	 *
	 *
	 *
	 * */
	status = xTaskCreate(task_led_green, "led_green", 200, NULL, 4,
			&taskHandle_led_green);

	configASSERT(status == pdPASS);
	next_task_handle = taskHandle_led_green;

	status = xTaskCreate(task_led_orange, "led_orange", 200, NULL, 3,
			&taskHandle_led_orange);

	configASSERT(status == pdPASS);

	status = xTaskCreate(task_led_red, "led_red", 200, NULL, 2,
			&taskHandle_led_red);

	configASSERT(status == pdPASS);
	next_task_handle = taskHandle_led_green;

	status = xTaskCreate(task_led_blue, "led_blue", 200, NULL, 1,
			&taskHandle_led_blue);

	configASSERT(status == pdPASS);

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
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
	RCC_OscInitStruct.PLL.PLLM = 8;
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
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_GPIO_Init(void) {
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOD,
	led_green_Pin | led_orange_Pin | led_red_Pin | led_blue_Pin,
			GPIO_PIN_RESET);

	/*Configure GPIO pin : button_Pin */
	GPIO_InitStruct.Pin = button_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(button_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pins : led_green_Pin led_orange_Pin led_red_Pin led_blue_Pin */
	GPIO_InitStruct.Pin = led_green_Pin | led_orange_Pin | led_red_Pin
			| led_blue_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

	/* EXTI interrupt init*/
	HAL_NVIC_SetPriority(EXTI0_IRQn, 12, 0);
	HAL_NVIC_EnableIRQ(EXTI0_IRQn);

}

/* USER CODE BEGIN 4 */

static void task_led_green(void *parameter) {
	BaseType_t status;
	while (1) {
		SEGGER_SYSVIEW_PrintfTarget("Toggling green LED");

		HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12);
		status = xTaskNotifyWait(0, 0, NULL, pdMS_TO_TICKS(1000));
		if (status == pdTRUE) {
			vTaskSuspendAll();
			next_task_handle = taskHandle_led_orange;
			xTaskResumeAll();

			HAL_GPIO_WritePin(GPIOD, led_green_Pin, GPIO_PIN_SET);

			SEGGER_SYSVIEW_PrintfTarget("delete green LED");

			vTaskDelete(NULL);
		}
	}
}

static void task_led_orange(void *parameter) {

	BaseType_t status;
	while (1) {
		SEGGER_SYSVIEW_PrintfTarget("Toggling orange LED");

		HAL_GPIO_TogglePin(GPIOD, led_orange_Pin);
		status = xTaskNotifyWait(0, 0, NULL, pdMS_TO_TICKS(500));
		if (status == pdTRUE) {
			vTaskSuspendAll();

			next_task_handle = taskHandle_led_red;

			xTaskResumeAll();
			HAL_GPIO_WritePin(GPIOD, led_orange_Pin, GPIO_PIN_SET);
			SEGGER_SYSVIEW_PrintfTarget("delete orange LED");

			vTaskDelete(NULL);
		}
	}
}

static void task_led_red(void *parameter) {

	BaseType_t status;
	while (1) {
		SEGGER_SYSVIEW_PrintfTarget("Toggling red LED");

		HAL_GPIO_TogglePin(GPIOD, led_red_Pin);
		status = xTaskNotifyWait(0, 0, NULL, pdMS_TO_TICKS(250));
		if (status == pdTRUE) {
			vTaskSuspendAll();
			next_task_handle = taskHandle_led_blue;
			xTaskResumeAll();

			HAL_GPIO_WritePin(GPIOD, led_red_Pin, GPIO_PIN_SET);
			SEGGER_SYSVIEW_PrintfTarget("delete red LED");
			vTaskDelete(NULL);
		}
	}
}

static void task_led_blue(void *parameter) {

	BaseType_t status;
	while (1) {
		SEGGER_SYSVIEW_PrintfTarget("Toggling blue LED");

		HAL_GPIO_TogglePin(GPIOD, led_blue_Pin);
		status = xTaskNotifyWait(0, 0, NULL, pdMS_TO_TICKS(125));
		if (status == pdTRUE) {
			vTaskSuspendAll();
			next_task_handle = NULL;
			xTaskResumeAll();

			HAL_GPIO_WritePin(GPIOD, led_blue_Pin, GPIO_PIN_SET);
			SEGGER_SYSVIEW_PrintfTarget("delete blue LED");
			vTaskDelete(NULL);
			SEGGER_SYSVIEW_PrintfTarget("delete button");
			vTaskDelete(taskHandle_btn);

		}
	}
}

void btn_isr_handle(void) {

	BaseType_t pxHigherPriorityTaskWoken;

	pxHigherPriorityTaskWoken = pdFALSE;

	traceISR_ENTER();
	xTaskNotifyFromISR(next_task_handle, 0, eNoAction,
			&pxHigherPriorityTaskWoken);

	/* once the ISR exits, the below macro makes higher priority task which got unblocked to resume on the CPU */
	portYIELD_FROM_ISR(pxHigherPriorityTaskWoken);

	traceISR_EXIT();
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

