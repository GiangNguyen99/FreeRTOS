/*
 * keypad_4x4.c
 *
 *  Created on: Sep 9, 2023
 *      Author: hoang
 */
#include "stm32f4xx_hal.h"
#include "main.h"

static uint8_t check_key(char key);

extern uint8_t address_number[];


char keypad_map[] = { '1', '2', '3', 'A', '4', '5', '6', 'B', '7', '8', '9',
		'C', '*', '0', '#', 'D' };
uint16_t row_pin[] = { ROW_0_Pin, ROW_1_Pin, ROW_2_Pin, ROW_3_Pin };
uint16_t col_pin[] = { COL_0_Pin, COL_1_Pin, COL_2_Pin, COL_3_Pin };
char keypad_read(void) {
	HAL_Delay(150);
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			HAL_GPIO_WritePin(GPIOD, row_pin[i], GPIO_PIN_SET);
			if (HAL_GPIO_ReadPin(GPIOD, col_pin[j]) == 1) {
				HAL_GPIO_WritePin(GPIOD, row_pin[i], GPIO_PIN_RESET);
				return keypad_map[4 * i + j];
			}
			HAL_GPIO_WritePin(GPIOD, row_pin[i], GPIO_PIN_RESET);
		}
	}
	return 'x';
}


//blocking function
uint32_t keypad_enter(void) {
	uint32_t number = 0;
	uint32_t number_temp = 0;

	volatile char key;
	tm1638_ctr_led(LED_ON, ADDR_15);
	for (int var = 0; var < 9; ++var) {

		do {
			key = keypad_read();

		} while (key == 'x' || key == 'A' || key == 'B' || key == 'C'
				|| key == 'D');

		if (var == 8) {
			if (key == '#') {
				return number;

			} else {
				var--;
			}
		} else {
			if (key == '#') {
				return number;
			}

			if (key == '*') {
				var -= 2;
				number /= 10;
				tm1638_clear_display();

			} else {
				number_temp = check_key(key);
				number = number * 10 + number_temp;
			}

			tm1638_write_multi_number(number);
		}
	}
	tm1638_ctr_led(LED_OFF, ADDR_15);
	return number;
}

uint8_t check_key(char key) {
	switch (key) {
	case '0':
		return 0;
		break;
	case '1':
		return 1;
		break;
	case '2':
		return 2;
		break;
	case '3':
		return 3;
		break;
	case '4':
		return 4;
		break;
	case '5':
		return 5;
		break;
	case '6':
		return 6;
		break;
	case '7':
		return 7;
		break;
	case '8':
		return 8;
		break;
	case '9':
		return 9;
		break;
	}
	return -1;
}

