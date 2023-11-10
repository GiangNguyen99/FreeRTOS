/*
 * lcd.c
 *
 *  Created on: Aug 23, 2023
 *      Author: hoang
 */
#include <lcd_hal.h>
#include "stm32f4xx_hal.h"



extern I2C_HandleTypeDef hi2c2;

void lcd_print_string(char *message)
{
	do
	{
		lcd_print_char((uint8_t) *message++);
	} while (*message != '\0');

}



void lcd_display_clear_i2c(void)
{
	//Display clear
	lcd_send_cmd(LCD_CMD_DIS_CLEAR);

	/*
	 * check page number 24 of datasheet.
	 * display clear command execution wait time is around 2ms
	 */

	HAL_Delay(5);
}

/*Cursor returns to home position */
void lcd_display_return_home(void)
{

	lcd_send_cmd(LCD_CMD_DIS_RETURN_HOME);
	/*
	 * check page number 24 of datasheet.
	 * return home command execution wait time is around 2ms
	 */
	HAL_Delay(5);
}

/**
 *   Set Lcd to a specified location given by row and column information
 *   Row Number (1 to 2)
 *   Column Number (1 to 16) Assuming a 2 X 16 characters display
 */
void lcd_set_cursor(uint8_t row, uint8_t column)
{
	column--;
	switch (row)
	{
	case 1:
		/* Set cursor to 1st row address and add index*/
		lcd_send_command((column |= 0x80));
		break;
	case 2:
		/* Set cursor to 2nd row address and add index*/
		lcd_send_command((column |= 0xC0));
		break;
	default:
		break;
	}
}




/*
 * code i2c lcd
 *
 *
 *
 *
 *
 *
 * */

void lcd_send_cmd(char cmd)
{
	char data_u, data_l;
	uint8_t data_t[4];
	data_u = (cmd & 0xf0);
	data_l = ((cmd << 4) & 0xf0);
	data_t[0] = (uint8_t) data_u | 0x0C;  //en=1, rs=0
	data_t[1] = (uint8_t) data_u | 0x08;  //en=0, rs=0
	data_t[2] = (uint8_t) data_l | 0x0C;  //en=1, rs=0
	data_t[3] = (uint8_t) data_l | 0x08;  //en=0, rs=0

	if(HAL_I2C_Master_Transmit(&hi2c2, (uint16_t)SLAVE_ADDRESS_LCD, data_t, 4, 200)!=HAL_OK)
	{
		while(1);
	}
}

void lcd_send_data(char data)
{
	char data_u, data_l;
	uint8_t data_t[4];
	data_u = (data & 0xf0);
	data_l = ((data << 4) & 0xf0);
	data_t[0] = (uint8_t) data_u | 0x0D;  //en=1, rs=0
	data_t[1] = (uint8_t) data_u | 0x09;  //en=0, rs=0
	data_t[2] = (uint8_t) data_l | 0x0D;  //en=1, rs=0
	data_t[3] = (uint8_t) data_l | 0x09;  //en=0, rs=0
	HAL_I2C_Master_Transmit(&hi2c2, SLAVE_ADDRESS_LCD, data_t, 4, 500);

}

void lcd_clear(void)
{
	lcd_send_cmd(0x80);
	for (int i = 0; i < 70; i++)
	{
		lcd_send_data(' ');
	}
}

void lcd_put_cur(int row, int col)
{
	row--;
	col--;
	switch (row)
	{
	case 0:
		lcd_send_cmd((col |= 0x80));
		break;
	case 1:
		lcd_send_cmd((col |= 0xC0));
		break;
	}
	HAL_Delay(5);

}

void lcd_init_i2c(void)
{
	// 4 bit initialisation
	HAL_Delay(50);  // wait for >40ms
	lcd_send_cmd(0x30);
	HAL_Delay(7);  // wait for >4.1ms
	lcd_send_cmd(0x30);
	HAL_Delay(2);  // wait for >100us
	lcd_send_cmd(0x30);
	HAL_Delay(20);
	lcd_send_cmd(0x20);  // 4bit mode
	HAL_Delay(20);

	// dislay initialisation

	//function set command
	lcd_send_cmd(LCD_CMD_4DL_2N_5X8F);

	//disply ON and cursor ON
	lcd_send_cmd(LCD_CMD_DON_CURON);
//	lcd_send_cmd(0x08);

	lcd_send_cmd(LCD_CMD_DIS_CLEAR);

	//entry mode set--> I/D = 1 (increment cursor) & S = 0 (no shift)
	lcd_send_cmd(LCD_CMD_INCADD);

//	//Display on/off control --> D = 1, C and B = 0. (Cursor and blink, last two bits)
//	lcd_send_cmd(0x0C);

}

void lcd_send_string(char *str)
{
	while (*str)
		lcd_send_data(*str++);
}

void lcd_display_return_home_i2c(void)
{

	lcd_send_cmd(LCD_CMD_DIS_RETURN_HOME);
	/*
	 * check page number 24 of datasheet.
	 * return home command execution wait time is around 2ms
	 */
	HAL_Delay(5);
}
