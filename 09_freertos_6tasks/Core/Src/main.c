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
#include <stdio.h>
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
volatile TaskHandle_t next_task_handle;

TaskHandle_t led_green;
TaskHandle_t led_orange;
TaskHandle_t led_red;
TaskHandle_t led_blue;
TaskHandle_t keypad;


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
/* USER CODE BEGIN PFP */

extern void SEGGER_SYSVIEW_Conf(void);
extern void SEGGER_SYSVIEW_Start(void);

static void handle_led_green(void *parameters);
static void handle_led_orange(void *parameters);
static void handle_led_red(void *parameters);
static void handle_led_blue(void *parameters);

void btn_user(void);
static void handle_keypad(void *parameters);

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

	status = xTaskCreate(handle_led_green, "led_green", 200,
			"this is task led green", 3, &led_green);
	configASSERT(status == pdPASS);
	status = xTaskCreate(handle_led_orange, "led_orange", 200,
			"this is task led orange", 3, &led_orange);
	configASSERT(status == pdPASS);
	status = xTaskCreate(handle_led_red, "led_red", 200, "this is task led red",
			3, &led_red);
	configASSERT(status == pdPASS);
	status = xTaskCreate(handle_led_blue, "led_blue", 200,
			"this is task led blue", 3, &led_blue);
	configASSERT(status == pdPASS);

	status = xTaskCreate(handle_keypad, "keypad_task", 200,
			"this is task keypad", 2, &keypad);
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
	__HAL_RCC_GPIOB_CLK_ENABLE();

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOD,
			LD3_Pin | LD4_Pin | LD5_Pin | LD6_Pin | ROW_0_Pin | ROW_1_Pin
					| ROW_2_Pin | ROW_3_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pin : BTN_Pin */
	GPIO_InitStruct.Pin = BTN_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	HAL_GPIO_Init(BTN_GPIO_Port, &GPIO_InitStruct);

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
void handle_led_green(void *parameters) {
	char msg[100];
	BaseType_t notify_status = 0;

	while (1) {
		snprintf(msg, 100, "%s\n", (char*) parameters);
		SEGGER_SYSVIEW_PrintfTarget(msg);

		notify_status = xTaskNotifyWait(0, 0, NULL, portMAX_DELAY);
		if (notify_status == pdTRUE) {
//			next_task_handle = led_orange;
			SEGGER_SYSVIEW_PrintfTarget("execute toggle led_green");
			HAL_GPIO_TogglePin(GPIOD, LD3_Pin);
		}
	}
}

void handle_led_orange(void *parameters) {
	char msg[100];
	BaseType_t notify_status = 0;

	while (1) {
		snprintf(msg, 100, "%s\n", (char*) parameters);
		SEGGER_SYSVIEW_PrintfTarget(msg);

		notify_status = xTaskNotifyWait(0, 0, NULL, portMAX_DELAY);
		if (notify_status == pdTRUE) {
//			next_task_handle = led_red;
			SEGGER_SYSVIEW_PrintfTarget("execute toggle led_orange");

			HAL_GPIO_TogglePin(GPIOD, LD4_Pin);
		}
	}
}

void handle_led_red(void *parameters) {
	char msg[100];
	BaseType_t notify_status = 0;

	while (1) {
		snprintf(msg, 100, "%s\n", (char*) parameters);
		SEGGER_SYSVIEW_PrintfTarget(msg);
		notify_status = xTaskNotifyWait(0, 0, NULL, portMAX_DELAY);
		if (notify_status == pdTRUE) {
//			next_task_handle = led_blue;
			SEGGER_SYSVIEW_PrintfTarget("execute toggle led_red");

			HAL_GPIO_TogglePin(GPIOD, LD5_Pin);
		}
	}
}
void handle_led_blue(void *parameters) {
	char msg[100];
	BaseType_t notify_status = 0;

	while (1) {
		snprintf(msg, 100, "%s\n", (char*) parameters);
		SEGGER_SYSVIEW_PrintfTarget(msg);

		notify_status = xTaskNotifyWait(0, 0, NULL, portMAX_DELAY);
		if (notify_status == pdTRUE) {
//			next_task_handle = led_green;
			SEGGER_SYSVIEW_PrintfTarget("execute toggle led_blue");

			HAL_GPIO_TogglePin(GPIOD, LD6_Pin);
		}
	}
}

void btn_user(void) {
	BaseType_t pxHigherPriorityTaskWoken = pdFALSE;

	traceISR_ENTER();
	xTaskNotifyFromISR(next_task_handle, 0, eNoAction,
			&pxHigherPriorityTaskWoken);

	/* once the ISR exits, the below macro makes higher priority task which got unblocked to resume on the CPU */
	portYIELD_FROM_ISR(pxHigherPriorityTaskWoken);

	traceISR_EXIT();
}

void handle_keypad(void *parameters) {
	char key;

	while (1) {
		key = keypad_read();
		switch (key) {
		case '0':

			break;
		case '1':
			SEGGER_SYSVIEW_PrintfTarget("toggle led green ");
			next_task_handle = led_green;
			xTaskNotify(next_task_handle, 0, eNoAction);
			taskYIELD()
			;

			break;
		case '2':
			SEGGER_SYSVIEW_PrintfTarget("toggle led_orange ");

			next_task_handle = led_orange;
			xTaskNotify(next_task_handle, 0, eNoAction);
			taskYIELD()
			;


			break;
		case '3':
			SEGGER_SYSVIEW_PrintfTarget("toggle led_red ");
			next_task_handle = led_red;
			xTaskNotify(next_task_handle, 0, eNoAction);
			taskYIELD()
			;
			break;
		case '4':
			SEGGER_SYSVIEW_PrintfTarget("toggle led_blue ");
			next_task_handle = led_blue;
			xTaskNotify(next_task_handle, 0, eNoAction);
			taskYIELD()
			;
			break;
		case '5':
			break;
		case '6':
			break;
		case '7':
			break;
		case '8':
			break;
		case '9':
			break;
		default:
			break;
		}
		vTaskDelay(pdMS_TO_TICKS(10));
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

