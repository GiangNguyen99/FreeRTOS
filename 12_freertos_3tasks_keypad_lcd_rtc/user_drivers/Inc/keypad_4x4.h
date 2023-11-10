/*
 * keypad_4x4.h
 *
 *  Created on: Sep 9, 2023
 *      Author: hoang
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
 *
 * 		SPI1 GPIO Configuration
 *
 *  PA4     ------> SPI1_NSS
 *  PA5     ------> SPI1_SCK
 *  PA7     ------> SPI1_MOSI
 *
 */
#ifndef INC_KEYPAD_4X4_H_
#define INC_KEYPAD_4X4_H_
#include "main.h"

char keypad_read(void);

uint32_t keypad_enter(void);

uint8_t check_key(char key);

#endif /* INC_KEYPAD_4X4_H_ */
