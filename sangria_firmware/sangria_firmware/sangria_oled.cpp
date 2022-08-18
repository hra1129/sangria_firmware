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
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/i2c.h"
#include "sangria_oled.h"

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

#define OLED_HEIGHT					32
#define OLED_WIDTH					128
#define OLED_PAGE_HEIGHT			8
#define OLED_NUM_PAGES				(OLED_HEIGHT / OLED_PAGE_HEIGHT)
#define OLED_BUF_LEN				(OLED_NUM_PAGES * OLED_WIDTH)

// --------------------------------------------------------------------
void CSANGRIA_OLED::send_bq_command( uint8_t command ) {
	uint8_t send_buffer[2] = { 0x80, command };

	i2c_write_blocking( SANGRIA_OLED_I2C, SANGRIA_BQ_ADDR, send_buffer, sizeof(send_buffer), false );
}

// --------------------------------------------------------------------
void CSANGRIA_OLED::send_command( uint8_t command ) {
	uint8_t send_buffer[2] = { 0x80, command };

	i2c_write_blocking( SANGRIA_OLED_I2C, SANGRIA_OLED_ADDR, send_buffer, sizeof(send_buffer), false );
}

// --------------------------------------------------------------------
CSANGRIA_OLED::CSANGRIA_OLED() {
	gpio_init( SANGRIA_BQ_INT );
	gpio_init( SANGRIA_BQ_OTG );
	gpio_init( SANGRIA_BQ_CE );

	gpio_init( SANGRIA_OLED_ON_N );
	gpio_init( SANGRIA_OLED_RST_N );

	gpio_set_dir( SANGRIA_OLED_ON_N, GPIO_OUT );
	gpio_set_dir( SANGRIA_OLED_RST_N, GPIO_OUT );

	//	OLED Power OFF
	gpio_put( SANGRIA_OLED_ON_N, 1 );

	//	I2C initialization, OLED connected SH1107 by I2C.
	i2c_init( SANGRIA_OLED_I2C, SANGRIA_OLED_CLOCK );
	gpio_set_function( SANGRIA_OLED_SDA, GPIO_FUNC_I2C );
	gpio_set_function( SANGRIA_OLED_SCL, GPIO_FUNC_I2C );
	gpio_pull_up( SANGRIA_OLED_SDA );
	gpio_pull_up( SANGRIA_OLED_SCL );

	//	Put the BQ device in boost mode before putting 0 in SANGRIA_OLED_ON_N
	//	T.B.D.

	//	OLED Power ON and RESET
	gpio_put( SANGRIA_OLED_ON_N, 0 );
	gpio_put( SANGRIA_OLED_RST_N, 0 );
	//	Reset pin must maintain L for at least 10us.
	sleep_us( 20 );
	gpio_put( SANGRIA_OLED_RST_N, 1 );
	sleep_ms( 120 );

	this->send_command( OLED_SET_DISP | 0x00 );			// set display off

	this->send_command( OLED_SET_MEM_ADDR );			// set memory address mode
	this->send_command( 0x00 );							// horizontal addressing mode

	this->send_command( OLED_SET_DISP_START_LINE );		// set display start line to 0

	this->send_command( OLED_SET_SEG_REMAP | 0x01 );	// set segment re-map

	this->send_command( OLED_SET_MUX_RATIO );			// set multiplex ratio
	this->send_command( OLED_HEIGHT - 1 );				// our display is only 32 pixels high

	this->send_command( OLED_SET_COM_OUT_DIR | 0x08 );	// set COM (common) output scan direction

	this->send_command( OLED_SET_DISP_OFFSET );			// set display offset
	this->send_command( 0x00 );							// no offset

	this->send_command( OLED_SET_COM_PIN_CFG );			// set COM (common) pins hardware configuration
	this->send_command( 0x02 );							// manufacturer magic number

	/* timing and driving scheme */
	this->send_command( OLED_SET_DISP_CLK_DIV );		// set display clock divide ratio
	this->send_command( 0x80 );							// div ratio of 1, standard freq

	this->send_command( OLED_SET_PRECHARGE );			// set pre-charge period
	this->send_command( 0xF1 );							// Vcc internally generated on our board

	this->send_command( OLED_SET_VCOM_DESEL );			// set VCOMH deselect level
	this->send_command( 0x30 );							// 0.83xVcc

	/* display */
	this->send_command( OLED_SET_CONTRAST );			// set contrast control
	this->send_command( 0xFF );

	this->send_command( OLED_SET_ENTIRE_ON );			// set entire display on to follow RAM content

	//this->send_command( OLED_SET_NORM_INV | 0x00 );		// set normal (not inverted) display
	this->send_command( OLED_SET_NORM_INV | 0x01 );		// set inverted display

	this->send_command( OLED_SET_CHARGE_PUMP );			// set charge pump
	this->send_command( 0x14 );							// Vcc internally generated on our board

	this->send_command( OLED_SET_SCROLL | 0x00 );		// deactivate horizontal scrolling if set

	this->send_command( OLED_SET_DISP | 0x01 );			// turn display on
	sleep_ms( 120 );

	uint8_t buffer[] = { 0x40, 
		0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 
		0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 
		0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 
		0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 
		0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 
		0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 
		0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 
		0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 
		0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 
		0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 
		0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 
		0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 
		0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 
		0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 
		0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 
		0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 
	};
    i2c_write_blocking( SANGRIA_OLED_I2C, SANGRIA_OLED_ADDR, buffer, sizeof(buffer), false );
}

// --------------------------------------------------------------------
void CSANGRIA_OLED::update( void ) {
}
