// --------------------------------------------------------------------
//	The MIT License (MIT)
//	
//	Sangria firmware OLED display
//	Copyright (c) 2022 Takayuki Hara
//	
//	Permission is hereby granted, free of charge, to any person obtaining a copy
//	of this software and associated documentation files (the "Software"), to deal
//	in the Software without restriction, including without limitation the rights
//	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//	copies of the Software, and to permit persons to whom the Software is
//	furnished to do so, subject to the following conditions:
//	
//	The above copyright notice and this permission notice shall be included in
//	all copies or substantial portions of the Software.
//	
//	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//	THE SOFTWARE.
// --------------------------------------------------------------------

#include <cstdio>
#include <cstdlib>

#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/i2c.h"
#include "pico/binary_info.h"

#include "sangria_oled.h"

#define OLED_CONTROL_CMD_SINGLE		0x80
#define OLED_CONTROL_CMD_STREAM		0x00
#define OLED_CONTROL_DATA_STREAM	0x40

#define OLED_SET_CONTRAST			0x81
#define OLED_SET_ENTIRE_ON			0xA4
#define OLED_SET_NORM_INV			0xA6
#define OLED_SET_DISP				0xAE
#define OLED_SET_MEM_ADDR			0x20
#define OLED_SET_COL_ADDR			0x21
#define OLED_SET_PAGE_ADDR			0x22
#define OLED_SET_DISP_START_LINE	0x40
#define OLED_SET_SEG_REMAP			0xA0
#define OLED_SET_MUX_RATIO			0xA8
#define OLED_SET_COM_OUT_DIR		0xC0
#define OLED_SET_DISP_OFFSET		0xD3
#define OLED_SET_COM_PIN_CFG		0xDA
#define OLED_SET_DISP_CLK_DIV		0xD5
#define OLED_SET_PRECHARGE			0xD9
#define OLED_SET_VCOM_DESEL			0xDB
#define OLED_SET_CHARGE_PUMP		0x8D
#define OLED_SET_HORIZ_SCROLL		0x26
#define OLED_SET_SCROLL				0x2E

#define OLED_HEIGHT					128
#define OLED_WIDTH					80
#define OLED_PAGE_HEIGHT			8
#define OLED_NUM_PAGES				(OLED_HEIGHT / OLED_PAGE_HEIGHT)
#define OLED_BUF_LEN				(OLED_NUM_PAGES * OLED_WIDTH)

// --------------------------------------------------------------------
void CSANGRIA_OLED::send_command( uint8_t command ) {
	uint8_t send_buffer[2] = { SANGRIA_OLED_CMD, command };

	int result = i2c_write_blocking( SANGRIA_OLED_I2C, SANGRIA_OLED_ADDR, send_buffer, sizeof(send_buffer), false );
	printf( "OLED_COM <= 0x%02X : result = %d\n", command, result );
}

// --------------------------------------------------------------------
void CSANGRIA_OLED::send_data( uint8_t data ) {
	uint8_t send_buffer[2] = { SANGRIA_OLED_DATA, data };

	i2c_write_blocking( SANGRIA_OLED_I2C, SANGRIA_OLED_ADDR, send_buffer, sizeof(send_buffer), false );
}

// --------------------------------------------------------------------
CSANGRIA_OLED::CSANGRIA_OLED() {
	uint8_t i, j;
	uint8_t buf[200];
	int cnt;

	gpio_init( SANGRIA_OLED_ON_N );
	gpio_init( SANGRIA_OLED_RST_N );

	gpio_set_dir( SANGRIA_OLED_ON_N, GPIO_OUT );
	gpio_set_dir( SANGRIA_OLED_RST_N, GPIO_OUT );

	//	OLED Power OFF
	gpio_put( SANGRIA_OLED_ON_N, 0 );
	sleep_ms( 100 );

	//	I2C initialization, OLED connected SH1107 by I2C.
	i2c_init( SANGRIA_OLED_I2C, SANGRIA_OLED_CLOCK );
	gpio_set_function( SANGRIA_OLED_SDA, GPIO_FUNC_I2C );
	gpio_set_function( SANGRIA_OLED_SCL, GPIO_FUNC_I2C );
	gpio_pull_up( SANGRIA_OLED_SDA );
	gpio_pull_up( SANGRIA_OLED_SCL );
	bi_decl( bi_2pins_with_func( SANGRIA_OLED_SDA, SANGRIA_OLED_SCL, GPIO_FUNC_I2C ) );

	//	Built-in DC-DC pump power is being used immediately after turning on the power: (Data sheet p.43)
	//	- Turn on the VDD and AVDD power, keep the RES pin="L" (>10us)
	gpio_put( SANGRIA_OLED_RST_N, 0 );		//	OLED Reset ON
	gpio_put( SANGRIA_OLED_ON_N, 1 );		//	OLED Power ON
	sleep_us( 30 );
	//	- Release the reset state (RES pin="H") Reset timing depends on SH1107 data sheet
	gpio_put( SANGRIA_OLED_RST_N, 1 );		//	OLED Reset OFF
	sleep_ms( 100 );
	//	- Initialized state (Default)
	//	- Set up initial code (user setup)
	cnt = 0;
	buf[cnt++] = 0x00;				// Control byte

	buf[cnt++] = 0xAE;				// set display off

	buf[cnt++] = 0xD5;				// set display clock divide ratio (p.35, Section 14.)
	buf[cnt++] = 0x80;				// - 2 suggested ratio

	buf[cnt++] = 0xA8;				// set multiplex ration
	buf[cnt++] = 127;				// - 

	buf[cnt++] = 0xD3;				// set display offset
	buf[cnt++] = 0x62;				// - 

	buf[cnt++] = 0x40;				// set start line

	buf[cnt++] = 0xAD;				// set DC-DC setting
	buf[cnt++] = 0x8B;				// DC-DC ON

	buf[cnt++] = 0xA1;				// set segment re-map: ADC=1

	buf[cnt++] = 0xC8;				// set common output scan direction

	buf[cnt++] = 0xDA;				// 
	buf[cnt++] = 0x02;				// 

	buf[cnt++] = 0x81;				// set contrast control
	buf[cnt++] = 0x80;				// - 128

	buf[cnt++] = 0xD9;				// set pre-charge period (p.36: Section 15.)
	buf[cnt++] = 0x22;				// -

	buf[cnt++] = 0xDB;				// set VCOMH deselect level (p.37: Section 16.)
	buf[cnt++] = 0x40;				// - ???? x Vcc

	buf[cnt++] = 0xA6;				// set normal (not inverted) display

	buf[cnt++] = 0xAF;				// set display on

	i2c_write_blocking( SANGRIA_OLED_I2C, SANGRIA_OLED_ADDR, buf, cnt, false );

	//	- Wait 100ms
	sleep_ms( 100 );

	//	- Clear internal RAM to "00H"
	for( i = 0; i < OLED_NUM_PAGES; i++ ) {
		cnt = 0;
		//	set page address = i
		buf[cnt++] = 0x00;					// Control byte
		buf[cnt++] = 0xB0 + i;				//	set page address (p.42: No.12)
		//	set column address = 24 (0x18)
		buf[cnt++] = 0x02;					//	set lower column address (p.23: Section 1)
		buf[cnt++] = 0x10;					//	set higher column address(p.23: Section 2)
		i2c_write_blocking( SANGRIA_OLED_I2C, SANGRIA_OLED_ADDR, buf, cnt, false );

		cnt = 0;
		buf[cnt++] = 0x40;
		for( j = 0; j < 128; j++ ) {
			buf[cnt++] = 0;
		}
		i2c_write_blocking( SANGRIA_OLED_I2C, SANGRIA_OLED_ADDR, buf, cnt, false );
	}
}

// --------------------------------------------------------------------
void CSANGRIA_OLED::update( void ) {
	int i, j, cnt;
	uint8_t buf[200];

	for( i = 0; i < OLED_NUM_PAGES; i++ ) {
		cnt = 0;
		//	set page address = i
		buf[cnt++] = 0x00;					// Control byte
		buf[cnt++] = 0xB0 + i;				//	set page address (p.42: No.12)
		//	set column address = 24 (0x18)
		buf[cnt++] = 0x02;					//	set lower column address (p.23: Section 1)
		buf[cnt++] = 0x10;					//	set higher column address(p.23: Section 2)
		i2c_write_blocking( SANGRIA_OLED_I2C, SANGRIA_OLED_ADDR, buf, cnt, false );

		cnt = 0;
		buf[cnt++] = 0x40;
		for( j = 0; j < 128; j++ ) {
			buf[cnt++] = (uint8_t) rand();
		}
		i2c_write_blocking( SANGRIA_OLED_I2C, SANGRIA_OLED_ADDR, buf, cnt, false );
	}
}
