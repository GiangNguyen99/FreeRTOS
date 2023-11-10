/*
 * tm1638_led.c
 *
 *  Created on: Sep 6, 2023
 *      Author: hoang
 */
#include "tm1638_led.h"
#include "FreeRTOS.h"
#include "task.h"
#include <stdio.h>
#include "queue.h"

extern SPI_HandleTypeDef hspi1;
extern struct queue_user qtime;
extern uint8_t address_number[];
extern QueueHandle_t qdata_key;

extern void MODE_A(void);
extern void MODE_B(void);
extern void MODE_C(void);

extern  uint8_t state0;
uint8_t length_time_first = 0;
uint8_t address_first = 0;


extern struct queue_user qtime;

//support function

static uint8_t count_length(uint32_t n);

static uint8_t position_address_number(uint8_t n);

static void tm1638_write_time_down(uint32_t time);

static void tm1638_write_time_up(uint32_t time);

char tm1638_check_btn() {
	uint32_t status_btn = tm1638_read_button();

	char btn_1 = '1', btn_2 = '2', btn_3 = '3', btn_4 = '4', btn_5 = '5',
			btn_6 = '6', btn_7 = '7', btn_8 = '8';
	if (status_btn & 0x01)
		return btn_1;
	else if (status_btn & 0x10)
		return btn_5;
	else if (status_btn & 0x100)
		return btn_2;
	else if (status_btn & 0x1000)
		return btn_6;
	else if (status_btn & 0x10000)
		return btn_3;
	else if (status_btn & 0x100000)
		return btn_7;
	else if (status_btn & 0x1000000)
		return btn_4;
	else if (status_btn & 0x10000000)
		return btn_8;
	else
		return 0;
}
uint8_t check_number(uint8_t data) {
	switch (data) {
	case 0:
		return NUM_0;
		break;
	case 1:
		return NUM_1;
		break;
	case 2:
		return NUM_2;
		break;
	case 3:
		return NUM_3;
		break;
	case 4:
		return NUM_4;
		break;
	case 5:
		return NUM_5;
		break;
	case 6:
		return NUM_6;
		break;
	case 7:
		return NUM_7;
		break;
	case 8:
		return NUM_8;
		break;
	case 9:
		return NUM_9;
		break;
	}
	return -1;
}

// support function

uint8_t count_length(uint32_t n) {
	uint8_t length = 0;
	uint32_t temp;
	temp = n;
	while (n != 0) {
		length++;
		n /= 10;
	}
	if (temp == 0)
		return 1;
	return length++;
}

uint8_t position_address_number(uint8_t n) {
	switch (n) {
	case 1:
		return ADDR_14;
		break;
	case 2:
		return ADDR_12;
		break;
	case 3:
		return ADDR_10;
		break;
	case 4:
		return ADDR_8;
		break;
	case 5:
		return ADDR_6;
		break;
	case 6:
		return ADDR_4;
		break;
	case 7:
		return ADDR_2;
		break;
	case 8:
		return ADDR_0;
		break;
	}
	return ADDR_14;
}

void tm1638_send_cmd(uint8_t *pData) {

	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, 0);
	HAL_SPI_Transmit(&hspi1, pData, 1, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, 1);
}

void tm1638_init(void) {
	uint8_t temp = AUTO_INC_ADDR_MODE;
	uint8_t *pData = &temp;

	/*
	 * Set command for writing data into display memory, in the mode of
	 * auto address increment by 1(40H)
	 */
	tm1638_send_cmd(pData);

	/* Set starting address (0C0H) */
	temp = ADDR_0;
	tm1638_send_cmd(pData);

	//Transmit data = 0 to clear all display
	temp = 0;

	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, 0);
	for (int var = 0; var < 16; ++var) {
		HAL_SPI_Transmit(&hspi1, pData, 1, HAL_MAX_DELAY);
	}
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, 1);

	// set maximum brightness (8FH)
	temp = SET_BRIGHTNESS_MAX;
	tm1638_send_cmd(pData);

}

void tm1638_reset(void) {
	uint8_t temp = AUTO_INC_ADDR_MODE;
	uint8_t *pData = &temp;

	tm1638_send_cmd(pData);

	/* Set starting address (0C0H) */
	temp = ADDR_0;
	tm1638_send_cmd(pData);

	//Transmit data = 0 to clear all display
	temp = 00;

	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, 0);
	for (int var = 0; var < 16; ++var) {
		HAL_SPI_Transmit(&hspi1, pData, 1, HAL_MAX_DELAY);
	}
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, 1);

}

void tm1638_write_number(uint8_t data, uint8_t address) {
	uint8_t *pTemp;
	uint8_t temp = 0;

	/*
	 * Set command for writing data into display memory, in the mode of
	 * auto address increment by 1(40H)
	 */
	temp = FIXED_ADDR_MODE;
	pTemp = &temp;
	tm1638_send_cmd(pTemp);

	/* Set address */
	temp = address;
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, 0);
	HAL_SPI_Transmit(&hspi1, pTemp, 1, HAL_MAX_DELAY);
	//Transmit data number
	HAL_SPI_Transmit(&hspi1, &data, 1, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, 1);
}

void tm1638_write_multi_number(uint32_t number) {
	uint8_t *pTemp;
	uint8_t temp = 0;
	pTemp = &temp;
	uint8_t data[8];

	uint8_t length_time = count_length(number);
	uint8_t address_time = position_address_number(length_time);

	for (int var = 0; var < length_time; ++var) {
		data[var] = number % 10;
		data[var] = check_number(data[var]);
		number /= 10;
	}

	/*
	 * Set command for writing data into display memory, in the mode of
	 * auto address increment by 1(40H)
	 */
	temp = FIXED_ADDR_MODE;
	tm1638_send_cmd(pTemp);

	temp = address_time;

	for (int var = length_time - 1; var >= 0; --var) {
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, 0);
		HAL_SPI_Transmit(&hspi1, &address_number[7 - var], 1, HAL_MAX_DELAY); //send address
		HAL_SPI_Transmit(&hspi1, &data[var], 1, HAL_MAX_DELAY);		//send data
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, 1);
	}

	/* Set address */
	//Transmit data number
}

void tm1638_ctr_led(uint8_t pData, uint8_t address) {
	uint8_t *pTemp;
	uint8_t temp = 0;
	/*
	 * Set command for writing data into display memory, in the mode of
	 * auto address increment by 1(40H)
	 */
	temp = FIXED_ADDR_MODE;
	pTemp = &temp;
	tm1638_send_cmd(pTemp);

	/* Set address */
	temp = address;
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, 0);
	HAL_SPI_Transmit(&hspi1, pTemp, 1, HAL_MAX_DELAY);
	//Transmit data number
	HAL_SPI_Transmit(&hspi1, &pData, 1, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, 1);
}
uint8_t state = 0;

void tm1638_toggle_led(uint8_t address) {
	if (state == 0) {
		tm1638_ctr_led(LED_ON, address);
		state = 1;
	} else {
		tm1638_ctr_led(LED_OFF, address);
		state = 0;
	}
}

uint32_t tm1638_read_button() {
	uint8_t *pTemp;
	uint8_t temp = READ_BUTTON_MODE;
	pTemp = &temp;
	uint8_t receive_data[4] = { 0 };
	uint32_t rec_data = 0;
	/*
	 * Set command for writing data into display memory, in the mode of
	 * auto address increment by 1(40H)
	 */

	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, 0);
	HAL_SPI_Transmit(&hspi1, pTemp, 1, HAL_MAX_DELAY);
	HAL_SPI_Receive(&hspi1, receive_data, 4, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, 1);

	rec_data = receive_data[0] | ((uint32_t) receive_data[1]) << 8
			| ((uint32_t) receive_data[2]) << 16
			| ((uint32_t) receive_data[3] << 24);

	return rec_data;
}

// blocking function to count down using recursion
void tm1638_cnt_down(uint32_t time) {

	if (time > 0) {
		tm1638_write_time_down(time);
//		HAL_Delay(100);
		vTaskDelay(pdMS_TO_TICKS(100));
		tm1638_cnt_down(time - 1);
	}

	else if (time == 0) {
		tm1638_write_time_down(0);
		state0 = 0;
		length_time_first = 0;
		address_first = 0;
	}
}

// non-blocking function to count down using freeRTOS
void tm1638_cnt_down_non_bl(uint32_t time) {
	TickType_t xLastWakeTime = xTaskGetTickCount();
	;
	const TickType_t xFrequency = 100;

	if (time > 0) {
		tm1638_write_time_down(time);
		vTaskDelayUntil(&xLastWakeTime, xFrequency);
	}

	else if (time == 0) {
		tm1638_write_time_down(0);
		state0 = 0;
		length_time_first = 0;
		address_first = 0;
	}
}

void tm1638_write_time_down(uint32_t time) {
	uint8_t temp = 0;
	uint8_t *pTemp = &temp;
	uint8_t temp1[8] = { 0 };
	uint32_t temp2 = 0;
	uint8_t address = 0;

// to clear number when count down pass different unit
	if (state0 == 0) {
		length_time_first = count_length(time);
		address_first = position_address_number(length_time_first);
		state0 = 1;
	}
	address = address_first;
//	uint8_t length_time = count_length(time);
//	address = position_address_number(length_time);

	/*
	 * Set command for writing data into display memory, in the mode of
	 * auto address increment by 1(40H)
	 */
	temp = FIXED_ADDR_MODE;
	tm1638_send_cmd(pTemp);

//Transmit data

	if (time > 0) {
		temp2 = time;
		for (int var = length_time_first - 1; var >= 0; --var) {
			temp1[var] = temp2 % 10;
			temp2 /= 10;
		}

		/* Set address */
		temp = address;
		for (int var = 0; var < length_time_first; ++var) {

			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, 0);
			HAL_SPI_Transmit(&hspi1, &address, 1, HAL_MAX_DELAY);
			//Transmit data number
			temp1[var] = check_number(temp1[var]);
			HAL_SPI_Transmit(&hspi1, &temp1[var], 1, HAL_MAX_DELAY);
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, 1);
			address += 2;
		}
	} else if (time == 0) {
		address = ADDR_14;
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, 0);
		HAL_SPI_Transmit(&hspi1, &address, 1, HAL_MAX_DELAY);
		//Transmit data number
		temp1[0] = NUM_0;
		HAL_SPI_Transmit(&hspi1, &temp1[0], 1, HAL_MAX_DELAY);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, 1);
	}
}
void tm1638_cnt_up(uint32_t time) {
	if (time < 99999999) {
		tm1638_write_time_up(time);
//		HAL_Delay(100);
		vTaskDelay(pdMS_TO_TICKS(100));
		tm1638_cnt_up(time + 1);
	}

	else if (time == 99999999) {
		tm1638_write_time_up(99999999);
	}
}

void tm1638_cnt_up_non_bl(uint32_t time) {
	TickType_t xLastWakeTime = xTaskGetTickCount();
	;
	const TickType_t xFrequency = 100;
	if (time < 99999999) {
		tm1638_write_time_up(time);
		vTaskDelayUntil(&xLastWakeTime, xFrequency);
	}

	else if (time == 99999999) {
		tm1638_write_time_up(99999999);
	}
}
void tm1638_write_time_up(uint32_t time) {
	uint8_t temp = 0;
	uint8_t *pTemp = &temp;
	uint8_t temp1[8] = { 0 };
	uint32_t temp2 = 0;
	uint8_t address = 0;

	uint8_t length_time = count_length(time);
	address = position_address_number(length_time);

	/*
	 * Set command for writing data into display memory, in the mode of
	 * auto address increment by 1(40H)
	 */
	temp = FIXED_ADDR_MODE;
	tm1638_send_cmd(pTemp);

//Transmit data

	if (time < 99999999) {
		temp2 = time;
		for (int var = length_time - 1; var >= 0; --var) {
			temp1[var] = temp2 % 10;
			temp2 /= 10;
		}

		/* Set address */
		temp = address;
		for (int var = 0; var < length_time; ++var) {

			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, 0);
			HAL_SPI_Transmit(&hspi1, &address, 1, HAL_MAX_DELAY);
			//Transmit data number
			temp1[var] = check_number(temp1[var]);
			HAL_SPI_Transmit(&hspi1, &temp1[var], 1, HAL_MAX_DELAY);
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, 1);
			address += 2;
		}
	} else if (time == 99999999) {
		address = ADDR_14;
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, 0);
		HAL_SPI_Transmit(&hspi1, &address, 1, HAL_MAX_DELAY);
		//Transmit data number
		temp1[0] = NUM_9;
		HAL_SPI_Transmit(&hspi1, &temp1[0], 1, HAL_MAX_DELAY);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, 1);
	}
}

void tm1638_cnt_up_right(uint32_t time) {
	if (time < 99999999) {
		tm1638_write_time_r2l(time);
		vTaskDelay(pdMS_TO_TICKS(500));
		tm1638_cnt_up_right(time + 1);
	}

	else if (time == 99999999) {
		tm1638_write_time_r2l(99999999);
	}
}

void tm1638_cnt_up_right_non_bl(uint32_t time) {
	TickType_t xLastWakeTime = xTaskGetTickCount();
	;
	const TickType_t xFrequency = 1000;
	if (time < 99999999) {
		tm1638_write_time_r2l(time);
		vTaskDelayUntil(&xLastWakeTime, xFrequency);
	} else if (time == 99999999) {
		tm1638_write_time_r2l(99999999);
	}
}
void tm1638_write_time_r2l(uint32_t time) {
	uint8_t temp = 0;
	uint8_t *pTemp = &temp;
	uint32_t temp2 = 0;
	uint8_t length_time = count_length(time);

	if (time > 9) {
		temp2 = time;
		for (int var = 0; var < length_time; ++var) {
			qtime.time[qtime.pTime - var] = temp2 % 10;
			temp2 /= 10;
			qtime.time[qtime.pTime - var] = check_number(
					qtime.time[qtime.pTime - var]);
		}
	} else {
		qtime.time[qtime.pAddr] = time;
		qtime.time[qtime.pAddr] = check_number(qtime.time[qtime.pAddr]);

	}
	/*
	 * Set command for writing data into display memory, in the mode of
	 * auto address increment by 1(40H)
	 */
	temp = FIXED_ADDR_MODE;
	tm1638_send_cmd(pTemp);

//Transmit data

	if (time <= 8) {

		for (int var = 0; var < time; ++var) {
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, 0);
			HAL_SPI_Transmit(&hspi1, &qtime.addr1[qtime.pAddr - var], 1,
			HAL_MAX_DELAY);

			HAL_SPI_Transmit(&hspi1, &qtime.time[var], 1, HAL_MAX_DELAY);
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, 1);
		}
		qtime.pAddr = (qtime.pAddr + 1) % 8;
	} else {
		for (int var = 0; var < 8; ++var) {
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, 0);
			HAL_SPI_Transmit(&hspi1, &qtime.addr2[var], 1,
			HAL_MAX_DELAY);

			HAL_SPI_Transmit(&hspi1,
					&qtime.time[(qtime.pTime + var + length_time - 1) % 8], 1,
					HAL_MAX_DELAY);
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, 1);
		}
		qtime.pTime = (qtime.pTime + 1) % 8 + length_time - 1;
		qtime.pAddr = (qtime.pAddr + 1) % 8;
	}

}

void create_addr(void) {
	for (int var = 0; var < 8; ++var) {
		qtime.addr2[var] = 0xc0 | var * 2;
	}
	int i = 0;
	for (int var = 7; var >= 0; --var) {
		qtime.addr1[var] = 0xc0 | i * 2;
		i++;
	}
}

void tm1638_clear_display() {
	uint8_t *pTemp;
	uint8_t temp = 0;
	pTemp = &temp;
	uint8_t data = 0;
	/*
	 * Set command for writing data into display memory, in the mode of
	 * auto address increment by 1(40H)
	 */
	temp = FIXED_ADDR_MODE;
	tm1638_send_cmd(pTemp);

	/* Set address */
	temp = ADDR_0;
	for (int var = 0; var < 8; ++var) {
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, 0);
		HAL_SPI_Transmit(&hspi1, pTemp, 1, HAL_MAX_DELAY);
		//Transmit data number
		HAL_SPI_Transmit(&hspi1, &data, 1, HAL_MAX_DELAY);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, 1);
		temp += 2;
	}

}

