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
#include <string.h>

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
I2C_HandleTypeDef hi2c1;
I2C_HandleTypeDef hi2c2;

/* USER CODE BEGIN PV */
#define DWT_CTRL    (*(volatile uint32_t*)0xE0001000)

TaskHandle_t keypad;
TaskHandle_t lcd;
TaskHandle_t rtc;

QueueHandle_t qdata_key;

uint8_t state0 = 0;

struct queue_user qtime;

RTC_time_t current_time;
RTC_date_t current_date;

RTC_time_t time_alarm1;
RTC_date_t date_alarm1;

struct state_machine state;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_I2C2_Init(void);
/* USER CODE BEGIN PFP */

extern void SEGGER_SYSVIEW_Conf(void);
extern void SEGGER_SYSVIEW_Start(void);

void btn_user(void);
static void handle_keypad(void *parameters);
static void handle_lcd(void *parameters);
static void handle_rtc(void *parameters);

char* time_to_string(RTC_time_t *rtc_time);
char* date_to_string(RTC_date_t *rtc_date);
void day_to_string(uint8_t day, char *buf);
void am_pm_to_string(uint8_t am_pm, char *buf);

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
	current_time.hours = 1;
	current_time.minutes = 1;
	current_time.seconds = 1;
	current_time.am_pm = 1;
	current_time.time_format = TIME_FORMAT_12HRS_PM;

	time_alarm1.hours = 1;
	time_alarm1.minutes = 1;
	time_alarm1.seconds = 1;
	time_alarm1.am_pm = 1;
	time_alarm1.time_format = TIME_FORMAT_12HRS_PM;
	date_alarm1.day = SUNDAY
	;

	current_date.day = SUNDAY
	;
	current_date.date = 1;
	current_date.month = 1;
	current_date.year = 1;

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
	MX_I2C1_Init();
	MX_I2C2_Init();
	/* USER CODE BEGIN 2 */

	//Enable the CYCCNT counter.
	DWT_CTRL |= (1 << 0);

//	SEGGER_SYSVIEW_Conf();
//	SEGGER_SYSVIEW_Start();
//
//	SEGGER_UART_init(500000);
	lcd_init_i2c();
	lcd_display_clear_i2c();

	if (ds3231_init()) {
		printf("RTC init has failed\n");
		while (1)
			;
	}

	status = xTaskCreate(handle_keypad, "keypad_task", 500,
			"this is task keypad", 3, &keypad);
	configASSERT(status == pdPASS);

	status = xTaskCreate(handle_lcd, "lcd_task", 500, "this is task lcd", 3,
			&lcd);
	configASSERT(status == pdPASS);

	status = xTaskCreate(handle_rtc, "rtc_task", 500, "this is task rtc", 2,
			&rtc);
	configASSERT(status == pdPASS);

	qdata_key = xQueueCreate(1, sizeof(struct state_machine));
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
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		Error_Handler();
	}
	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
			| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSE;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK) {
		Error_Handler();
	}
}

/**
 * @brief I2C1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_I2C1_Init(void) {

	/* USER CODE BEGIN I2C1_Init 0 */

	/* USER CODE END I2C1_Init 0 */

	/* USER CODE BEGIN I2C1_Init 1 */

	/* USER CODE END I2C1_Init 1 */
	hi2c1.Instance = I2C1;
	hi2c1.Init.ClockSpeed = 100000;
	hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
	hi2c1.Init.OwnAddress1 = 0;
	hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	hi2c1.Init.OwnAddress2 = 0;
	hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
	if (HAL_I2C_Init(&hi2c1) != HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN I2C1_Init 2 */

	/* USER CODE END I2C1_Init 2 */

}

/**
 * @brief I2C2 Initialization Function
 * @param None
 * @retval None
 */
static void MX_I2C2_Init(void) {

	/* USER CODE BEGIN I2C2_Init 0 */

	/* USER CODE END I2C2_Init 0 */

	/* USER CODE BEGIN I2C2_Init 1 */

	/* USER CODE END I2C2_Init 1 */
	hi2c2.Instance = I2C2;
	hi2c2.Init.ClockSpeed = 100000;
	hi2c2.Init.DutyCycle = I2C_DUTYCYCLE_2;
	hi2c2.Init.OwnAddress1 = 0;
	hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	hi2c2.Init.OwnAddress2 = 0;
	hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_ENABLE;
	if (HAL_I2C_Init(&hi2c2) != HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN I2C2_Init 2 */

	/* USER CODE END I2C2_Init 2 */

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
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(SPI1_NSS_GPIO_Port, SPI1_NSS_Pin, GPIO_PIN_SET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOD,
			LD3_Pin | LD4_Pin | LD5_Pin | LD6_Pin | ROW_0_Pin | ROW_1_Pin
					| ROW_2_Pin | ROW_3_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pin : BTN_Pin */
	GPIO_InitStruct.Pin = BTN_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
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

	/*Configure GPIO pin : ALARM_Pin */
	GPIO_InitStruct.Pin = ALARM_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(ALARM_GPIO_Port, &GPIO_InitStruct);

	/* EXTI interrupt init*/
	HAL_NVIC_SetPriority(EXTI0_IRQn, 8, 0);
	HAL_NVIC_EnableIRQ(EXTI0_IRQn);

	HAL_NVIC_SetPriority(EXTI9_5_IRQn, 9, 0);
	HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

}

/* USER CODE BEGIN 4 */

void handle_keypad(void *parameters) {
	state.level = 0;
	state.en_send_number = 0;
	state.scroll = 0;
	state.position_cursor = 0;
	char key_read;
	while (1) {
		key_read = keypad_read();
		if (key_read == 'A') {
			if (state.level < 3) {
				state.level = state.level + 1;
				xQueueSend(qdata_key, (void* )&state, portMAX_DELAY);
			}

		} else if (key_read == 'B') {
			if (state.level > 0) {
				state.level = state.level - 1;
				xQueueSend(qdata_key, (void* )&state, portMAX_DELAY);
			}
		} else if (key_read == 'C') {
			if (state.level == 2) {
				state.scroll = (state.scroll + 1) % 3;
				xQueueSend(qdata_key, (void* )&state, portMAX_DELAY);
			}
		} else if (key_read == 'D') {
			if (state.level == 2) {
				state.scroll = (state.scroll + 2) % 3;
				xQueueSend(qdata_key, (void* )&state, portMAX_DELAY);
			}
		} else if (key_read == '*') {
			if (state.level == 3) {
				if (state.position_cursor == 3 || state.position_cursor == 6) {
					state.position_cursor = (state.position_cursor - 2) % 13;

				} else if (state.position_cursor == 10
						|| state.position_cursor == 0) {
					state.position_cursor = (state.position_cursor + 10) % 13;

				} else {
					state.position_cursor = (state.position_cursor - 1) % 13;
				}
				xQueueSend(qdata_key, (void* )&state, portMAX_DELAY);
			}
		} else if (key_read == '#') {
			if (state.level == 3) {
				if (state.position_cursor == 1 || state.position_cursor == 4) {
					state.position_cursor = (state.position_cursor + 2) % 13;

				} else if (state.position_cursor == 7
						|| state.position_cursor == 10) {
					state.position_cursor = (state.position_cursor + 3) % 13;

				} else {
					state.position_cursor = (state.position_cursor + 1) % 13;
				}
				xQueueSend(qdata_key, (void* )&state, portMAX_DELAY);
			}
		} else if (key_read == 'x') {

		} else {
			if (state.level == 3) {
				state.number = check_key(key_read);
				state.en_send_number = 1;
				xQueueSend(qdata_key, (void* )&state, portMAX_DELAY);

				if (state.position_cursor == 1 || state.position_cursor == 4) {
					state.position_cursor = (state.position_cursor + 2) % 13;

				} else if (state.position_cursor == 7
						|| state.position_cursor == 10) {
					state.position_cursor = (state.position_cursor + 3) % 13;

				} else {
					state.position_cursor = (state.position_cursor + 1) % 13;
				}
				state.en_send_number = 0;
			}
		}
		vTaskDelay(pdMS_TO_TICKS(50));
	}
}

void handle_lcd(void *parameters) {
	struct state_machine data_receive;

	//current time variable
	uint8_t number_array[7] = { 0 };
	uint8_t temp = 0;

	//current day variable
	uint8_t number_array_day[7] = { 0 };
	number_array_day[6] = 1;
	uint8_t temp_day = 0;

	//alarm time variable
	uint8_t temp_al = 0;
	uint8_t number_array_al[7] = { 0 };

	data_receive.level = 0;
	data_receive.en_send_number = 0;
	data_receive.scroll = 0;

	char string_l1_r0[] = "A:set  B:return";
	char string_l1_r1[] = "C:left D:right";
	char string_l2_s0_r0[] = "set current time";
//	char string_l2_s0_r1[] = "format 12 hours";

	char string_l2_s1_r0[] = "set day-date";
	char string_l2_s2_r0[] = "set alarm time";
	char string_l3_s0_r0[] = "hh:mm:ss <AM/PM>";
	char string_l3_s1_r0[] = "dd:mm:yy <day>";
	char string_l3_s2_r0[] = "hh:mm:ss <AM/PM>";

	lcd_display_clear_i2c();
	lcd_display_return_home_i2c();

	ds3231_set_current_time(&current_time);
	ds3231_set_current_date(&current_date);

	ds3231_get_current_time(&current_time);
	ds3231_get_current_date(&current_date);

	lcd_send_string(time_to_string(&current_time));
	lcd_put_cur(2, 1);
	lcd_send_string(date_to_string(&current_date));
	vTaskSuspend(rtc);

	while (1) {
		if (qdata_key != NULL) {
			if (xQueueReceive(qdata_key, &data_receive,
					(TickType_t) 100) == pdPASS) {
				lcd_send_cmd(0x0C);

				if (data_receive.level == 0) {
					lcd_display_clear_i2c();
					lcd_display_return_home_i2c();
					vTaskResume(rtc);
				}
				if (data_receive.level == 1) {
					vTaskSuspend(rtc);
					lcd_display_clear_i2c();
					lcd_display_return_home_i2c();
					lcd_send_string(string_l1_r0);
					lcd_put_cur(2, 1);
					lcd_send_string(string_l1_r1);
				}
				if (data_receive.level == 2) {
					lcd_display_clear_i2c();
					lcd_display_return_home_i2c();
					if (data_receive.scroll == 0) {
						lcd_send_string(string_l2_s0_r0);
//						lcd_put_cur(2, 1);
//						lcd_send_string(string_l2_s0_r1);
					}
					if (data_receive.scroll == 1) {
						lcd_send_string(string_l2_s1_r0);
					}
					if (data_receive.scroll == 2) {
						lcd_send_string(string_l2_s2_r0);
					}
				}
				if (data_receive.level == 3) {
					lcd_display_clear_i2c();
					lcd_display_return_home_i2c();

					if (data_receive.scroll == 0) {
						lcd_send_string(string_l3_s0_r0);
						lcd_put_cur(2, 1);
						lcd_send_string(time_to_string(&current_time));
						temp = data_receive.position_cursor + 1;

						if (data_receive.en_send_number == 1) {
							if (data_receive.position_cursor < 2) {
								number_array[data_receive.position_cursor] =
										data_receive.number;

							} else if (data_receive.position_cursor < 5
									&& data_receive.position_cursor > 2) {
								number_array[data_receive.position_cursor - 1] =
										data_receive.number;

							} else if (data_receive.position_cursor < 8
									&& data_receive.position_cursor > 5) {
								number_array[data_receive.position_cursor - 2] =
										data_receive.number;
							} else {
								number_array[6] = data_receive.number;
							}
							current_time.hours = number_array[0] * 10
									+ number_array[1];
							current_time.minutes = number_array[2] * 10
									+ number_array[3];
							current_time.seconds = number_array[4] * 10
									+ number_array[5];
							current_time.time_format = number_array[6];

							ds3231_set_current_time(&current_time);

							lcd_put_cur(2, 1);
							lcd_send_string(time_to_string(&current_time));

							if (temp == 2 || temp == 5) {
								lcd_put_cur(2, temp + 2);
							} else if (temp == 8) {
								lcd_put_cur(2, temp + 3);
							} else if (temp == 11) {
								lcd_put_cur(2, 1);
							} else {
								lcd_put_cur(2, temp + 1);
							}
						} else {

							lcd_put_cur(2, 1);
							lcd_send_string(time_to_string(&current_time));
							lcd_put_cur(2, temp);

						}

						data_receive.en_send_number = 0;
						//blink cursor on lcd
						lcd_send_cmd(0x0F);
					}

					if (data_receive.scroll == 1) {
						lcd_send_string(string_l3_s1_r0);
						lcd_put_cur(2, 1);
						lcd_send_string(date_to_string(&current_date));
						temp_day = data_receive.position_cursor + 1;

						if (data_receive.en_send_number == 1) {
							if (data_receive.position_cursor < 2) {
								number_array_day[data_receive.position_cursor] =
										data_receive.number;
							} else if (data_receive.position_cursor < 5
									&& data_receive.position_cursor > 2) {
								number_array_day[data_receive.position_cursor
										- 1] = data_receive.number;
							} else if (data_receive.position_cursor < 8
									&& data_receive.position_cursor > 5) {
								number_array_day[data_receive.position_cursor
										- 2] = data_receive.number;
							} else {
								number_array_day[6] = data_receive.number;

							}

							current_date.date = number_array_day[0] * 10
									+ number_array_day[1];
							current_date.month = number_array_day[2] * 10
									+ number_array_day[3];
							current_date.year = number_array_day[4] * 10
									+ number_array_day[5];
							current_date.day = number_array_day[6];
							ds3231_set_current_date(&current_date);

							lcd_put_cur(2, 1);
							lcd_send_string(date_to_string(&current_date));
							if (temp_day == 2 || temp_day == 5) {
								lcd_put_cur(2, temp_day + 2);
							} else if (temp_day == 8) {
								lcd_put_cur(2, temp_day + 3);
							} else if (temp_day == 11) {
								lcd_put_cur(2, 1);
							} else {
								lcd_put_cur(2, temp_day + 1);
							}
						} else {
							lcd_put_cur(2, 1);
							lcd_send_string(date_to_string(&current_date));
							lcd_put_cur(2, temp_day);
						}

						data_receive.en_send_number = 0;
						//blink cursor on lcd
						lcd_send_cmd(0x0F);
					}

					if (data_receive.scroll == 2) {
						lcd_send_string(string_l3_s2_r0);
						lcd_put_cur(2, 1);
						lcd_send_string(time_to_string(&time_alarm1));
						temp_al = data_receive.position_cursor + 1;

						if (data_receive.en_send_number == 1) {
							if (data_receive.position_cursor < 2) {
								number_array_al[data_receive.position_cursor] =
										data_receive.number;

							} else if (data_receive.position_cursor < 5
									&& data_receive.position_cursor > 2) {
								number_array_al[data_receive.position_cursor - 1] =
										data_receive.number;

							} else if (data_receive.position_cursor < 8
									&& data_receive.position_cursor > 5) {
								number_array_al[data_receive.position_cursor - 2] =
										data_receive.number;

							} else {
								number_array_al[6] = data_receive.number;

							}

							time_alarm1.hours = number_array_al[0] * 10
									+ number_array_al[1];
							time_alarm1.minutes = number_array_al[2] * 10
									+ number_array_al[3];
							time_alarm1.seconds = number_array_al[4] * 10
									+ number_array_al[5];
							time_alarm1.am_pm = number_array_al[6];
							ds3231_set_time_alarm1(&time_alarm1, &date_alarm1,
									DS3231_MB_AL1_3);
							lcd_put_cur(2, 1);
							lcd_send_string(time_to_string(&time_alarm1));

							if (temp_al == 2 || temp_al == 5) {
								lcd_put_cur(2, temp_al + 2);
							} else if (temp_al == 8) {
								lcd_put_cur(2, temp_al + 3);
							} else if (temp_al == 11) {
								lcd_put_cur(2, 1);
							} else {
								lcd_put_cur(2, temp_al + 1);
							}
						} else {

							lcd_put_cur(2, 1);
							lcd_send_string(time_to_string(&time_alarm1));
							lcd_put_cur(2, temp_al);

						}

						data_receive.en_send_number = 0;
						//blink cursor on lcd
						lcd_send_cmd(0x0F);
					}
				}
			}
		}

	}
}
void handle_rtc(void *parameters) {
//	TickType_t xLastWakeTime;
//	const TickType_t xFrequency = 1000;
//	xLastWakeTime = xTaskGetTickCount();

	while (1) {
		ds3231_get_current_time(&current_time);
		ds3231_get_current_date(&current_date);
		lcd_display_return_home_i2c();
		lcd_send_string(time_to_string(&current_time));
		lcd_put_cur(2, 1);
		lcd_send_string(date_to_string(&current_date));
		vTaskDelay(pdMS_TO_TICKS(1000));
	}
}

void number_to_string(uint8_t num, char *buf) {

	if (num < 10) {
		buf[0] = '0';
		buf[1] = num + 48;
	} else if (num >= 10 && num <= 99) {
		buf[0] = (num / 10) + 48;
		buf[1] = (num % 10) + 48;
	}
}
void am_pm_to_string(uint8_t am_pm, char *buf) {

	switch (am_pm) {
	case 0:
		buf[0] = 'A';
		buf[1] = 'M';

		break;
	case 1:
		buf[0] = 'P';
		buf[1] = 'M';
		break;
	}
}
void day_to_string(uint8_t day, char *buf) {

	switch (day) {
	case 1:
		buf[0] = 'S';
		buf[1] = 'u';
		buf[2] = 'n';

		break;
	case 2:
		buf[0] = 'M';
		buf[1] = 'o';
		buf[2] = 'n';
		break;
	case 3:
		buf[0] = 'T';
		buf[1] = 'u';
		buf[2] = 'e';
		break;
	case 4:
		buf[0] = 'W';
		buf[1] = 'e';
		buf[2] = 'd';

		break;
	case 5:
		buf[0] = 'T';
		buf[1] = 'h';
		buf[2] = 'u';
		break;
	case 6:
		buf[0] = 'F';
		buf[1] = 'r';
		buf[2] = 'i';
		break;
	case 7:
		buf[0] = 'S';
		buf[1] = 'a';
		buf[2] = 't';
		break;
	}
}
//hh:mm:ss
char* time_to_string(RTC_time_t *rtc_time) {
	static char buf[14];

	buf[2] = ':';
	buf[5] = ':';
	buf[8] = ' ';
	buf[9] = '<';
	buf[12] = '>';
	number_to_string(rtc_time->hours, buf);
	number_to_string(rtc_time->minutes, &buf[3]);
	number_to_string(rtc_time->seconds, &buf[6]);
	if (rtc_time->time_format != TIME_FORMAT_24HRS) {
		am_pm_to_string(rtc_time->time_format, &buf[10]);
	}
	buf[13] = '\0';

	return buf;

}

//dd/mm/yy
char* date_to_string(RTC_date_t *rtc_date) {
	static char buf[15];

	buf[2] = '/';
	buf[5] = '/';
	buf[8] = ' ';
	buf[9] = '<';
	buf[13] = '>';
	number_to_string(rtc_date->date, buf);
	number_to_string(rtc_date->month, &buf[3]);
	number_to_string(rtc_date->year, &buf[6]);
	day_to_string(rtc_date->day, &buf[10]);
	buf[14] = '\0';

	return buf;

}

void alarm(void) {
	HAL_GPIO_WritePin(GPIOD, LD3_Pin, GPIO_PIN_SET);

	//RESET ALARM SIGNAL
	ds3231_clear_sr();
}

void btn_off_alarm(void)
{
	HAL_GPIO_WritePin(GPIOD, LD3_Pin, GPIO_PIN_RESET);
//	HAL_GPIO_TogglePin(LD3_GPIO_Port, LD3_Pin);

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

