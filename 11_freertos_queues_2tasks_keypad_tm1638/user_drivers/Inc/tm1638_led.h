
#include "main.h"



/*
 * ----------connect diagram----------------
 *
 * 		SPI1 GPIO Configuration
 *
 *  PA4     ------> SPI1_NSS
 *  PA5     ------> SPI1_SCK
 *  PA7     ------> SPI1_MOSI
 *
 */







/*
 * 	display brightness
 * */
#define SET_BRIGHTNESS_MIN			0x88
#define SET_BRIGHTNESS_MID			0x8B
#define SET_BRIGHTNESS_MAX			0x8F

/*
 * 		DATA COMMAND
 * */
#define AUTO_INC_ADDR_MODE			0x40
#define READ_BUTTON_MODE			0x42
#define FIXED_ADDR_MODE				0x44
#define TEST_MODE					0x48

/*
 * 		ADDRESS COMMAND
 * */
#define ADDR_0						0xc0
#define ADDR_1						0xc1
#define ADDR_2						0xc2
#define ADDR_3						0xc3
#define ADDR_4						0xc4
#define ADDR_5						0xc5
#define ADDR_6						0xc6
#define ADDR_7						0xc7
#define ADDR_8						0xc8
#define ADDR_9						0xc9
#define ADDR_10						0xcA
#define ADDR_11						0xcB
#define ADDR_12						0xcC
#define ADDR_13						0xcD
#define ADDR_14						0xcE
#define ADDR_15						0xcF

/*
 * 		ADDRESS OF DISPLAY
 * */
#define NUM_0			0x3F
#define NUM_1			0x06
#define NUM_2			0x5B
#define NUM_3			0x4F
#define NUM_4			0x66
#define NUM_5			0x6D
#define NUM_6			0x7D
#define NUM_7			0x07
#define NUM_8			0x7F
#define NUM_9			0x6F

#define LED_ON			0x01
#define LED_OFF			0x00





void tm1638_init(void);

void tm1638_send_cmd(uint8_t *pData);

void tm1638_reset(void);

void tm1638_write_number(uint8_t pData, uint8_t address);

void tm1638_write_multi_number(uint32_t number);

void tm1638_ctr_led(uint8_t pData, uint8_t address);

void tm1638_toggle_led(uint8_t address);

uint32_t tm1638_read_button();

char tm1638_check_btn();

void tm1638_cnt_up(uint32_t time);

void tm1638_cnt_up_non_bl(uint32_t time);

void tm1638_cnt_down(uint32_t time);

void tm1638_cnt_down_non_bl(uint32_t time);

void tm1638_cnt_up_right(uint32_t time);

void tm1638_cnt_up_right_non_bl(uint32_t time);

void tm1638_write_time_r2l(uint32_t time);

void create_addr(void);

uint8_t check_number(uint8_t data);

void tm1638_clear_display();
