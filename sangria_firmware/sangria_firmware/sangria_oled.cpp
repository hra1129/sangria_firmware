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
#include "pico/binary_info.h"

#include "sangria_oled.h"

#define OLED_CONTROL_CMD_SINGLE		0x80
#define OLED_CONTROL_CMD_STREAM		0x00
#define OLED_CONTROL_DATA_STREAM	0x40

//// Fundamental commands (pg.28-33)
//#define OLED_SET_CONTRAST            0x81
//#define OLED_DISPLAY_ALL_ON_RESUME   0xA4
//#define OLED_DISPLAY_ALL_ON_IGNORE   0xA5
//#define OLED_NORMAL_DISPLAY          0xA6
//#define OLED_INVERT_DISPLAY          0xA7
//#define OLED_DISPLAY_OFF             0xAE
//#define OLED_DISPLAY_ON              0xAF
//
//// Scrolling #defines (pg.28-30)
//#define OLED_RIGHT_HORIZONTAL_SCROLL                 0x26
//#define OLED_LEFT_HORIZONTAL_SCROLL                  0x27
//#define OLED_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL    0x29
//#define OLED_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL     0x2A
//#define OLED_DEACTIVATE_SCROLL                       0x2E
//#define OLED_ACTIVATE_SCROLL                         0x2F
//#define OLED_SET_VERTICAL_SCROLL_AREA                0xA3
//
//// Addressing Command Table (pg.30)
//#define OLED_SET_LOW_COLUMN          0x00
//#define OLED_SET_HIGH_COLUMN         0x10
//#define OLED_SET_MEMORY_ADDR_MODE    0x20    // follow with 0x00 = HORZ mode = Behave like a KS108 graphic LCD
//#define OLED_SET_COLUMN_RANGE        0x21    // can be used only in HORZ/VERT mode - follow with 0x00 and 0x7F = COL127
//#define OLED_SET_PAGE_RANGE          0x22    // can be used only in HORZ/VERT mode - follow with 0x00 and 0x07 = PAGE7
//#define OLED_SET_PAGE_START_ADDRESS  0xB0
//
//// Hardware Config (pg.31)
//#define OLED_SET_DISPLAY_START_LINE  0x40
//#define OLED_SET_SEGMENT_REMAP_LOW   0xA0
//#define OLED_SET_SEGMENT_REMAP_HIGH  0xA1
//#define OLED_SET_MULTIPLEX_RATIO     0xA8    // follow with 0x3F = 64 MUX
//#define OLED_SET_COM_SCAN_INC        0xC0
//#define OLED_SET_COM_SCAN_DEC        0xC8
//#define OLED_SET_DISPLAY_OFFSET      0xD3    // follow with 0x00
//#define OLED_SET_COM_PIN_MAP         0xDA    // follow with 0x12
//
//// Timing and Driving Scheme (pg.32)
//#define OLED_SET_DISPLAY_CLK_DIV     0xD5    // follow with 0x80
//#define OLED_SET_PRECHARGE           0xD9    // follow with 0xF1
//#define OLED_SET_VCOMH_DESELCT       0xDB    // follow with 0x30
//#define OLED_NOP                     0xE3    // NOP
//
//// Charge Pump (pg.62)
//#define OLED_SET_CHARGE_PUMP         0x8D    // follow with 0x14


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

	int result = i2c_write_blocking( SANGRIA_OLED_I2C, SANGRIA_OLED_ADDR, send_buffer, sizeof(send_buffer), false );

	printf( "OLED_DAT <= 0x%02X : result = %d\n", data, result );
}

// --------------------------------------------------------------------
CSANGRIA_OLED::CSANGRIA_OLED() {
	uint8_t i, j;

	gpio_init( SANGRIA_BQ_INT );
	gpio_init( SANGRIA_BQ_OTG );
	gpio_init( SANGRIA_BQ_CE );

	gpio_init( SANGRIA_OLED_ON_N );
	gpio_init( SANGRIA_OLED_RST_N );

	gpio_set_dir( SANGRIA_OLED_ON_N, GPIO_OUT );
	gpio_set_dir( SANGRIA_OLED_RST_N, GPIO_OUT );


	//	I2C initialization, OLED connected SH1107 by I2C.
	i2c_init( SANGRIA_OLED_I2C, SANGRIA_OLED_CLOCK );
	gpio_set_function( SANGRIA_OLED_SDA, GPIO_FUNC_I2C );
	gpio_set_function( SANGRIA_OLED_SCL, GPIO_FUNC_I2C );
	gpio_pull_up( SANGRIA_OLED_SDA );
	gpio_pull_up( SANGRIA_OLED_SCL );
	bi_decl( bi_2pins_with_func( SANGRIA_OLED_SDA, SANGRIA_OLED_SCL, GPIO_FUNC_I2C ) );

	//	Built-in DC-DC pump power is being used immediately after turning on the power: (Data sheet p.43)
	//	- Turn on the VDD and AVDD power, keep the RES pin="L" (>10us)
	gpio_put( SANGRIA_OLED_RST_N, 0 );
	gpio_put( SANGRIA_OLED_ON_N, 1 );
	sleep_us( 30 );
	//	- Release the reset state (RES pin="H") Reset timing depends on SH1107 data sheet
	gpio_put( SANGRIA_OLED_RST_N, 1 );
	sleep_ms( 100 );
	//	- Initialized state (Default)
	//	- Set up initial code (user setup)
	this->send_command( 0xAE );		// set display off
	this->send_command( 0x20 );		// set memory address mode: Page addressing mode
	this->send_command( 0xA1 );		// set segment re-map: ADC=1

	this->send_command( 0xA8 );		// set multiplex ration
	this->send_command( 127 );		// - 127

	this->send_command( 0xD3 );		// set display offset
	this->send_command( 0x18 );		// - COM24

	this->send_command( 0x81 );		// set contrast control
	this->send_command( 255 );		// - 255

	this->send_command( 0xD5 );		// set display clock divide ratio (p.35, Section 14.)
	this->send_command( 0x50 );		// - div ratio of 1, standard freq

	this->send_command( 0xD9 );		// set pre-charge period (p.36: Section 15.)
	this->send_command( 0x22 );		// - 

	this->send_command( 0xDB );		// set VCOMH deselect level (p.37: Section 16.)
	this->send_command( 0x35 );		// - 0.770 x Vcc


	this->send_command( 0xA4 );		// set entire display on to follow RAM content
	this->send_command( 0xA5 );		// ‹­§“I‚É‘S“_“”

	this->send_command( 0xA6 );		// set normal (not inverted) display

	//	- Clear internal RAM to "00H"
	for( i = 0; i < OLED_NUM_PAGES; i++ ) {
		//	set page address = i
		this->send_command( 0xB0 + i );			//	set page address (p.42: No.12)
		//	set column address = 24 (0x18)
		this->send_command( 0x08 );				//	set lower column address (p.23: Section 1)
		this->send_command( 0x11 );				//	set higher column address(p.23: Section 2)
		for( j = 0; j < 80; j++ ) {
			this->send_data( 0 );				//	clear
		}
	}
	//	- Set display on: AFh
	this->send_command( 0xAF );					//	display on
	//	- Wait 100ms
	sleep_ms( 100 );

	for( i = 0; i < OLED_NUM_PAGES; i++ ) {
		//	set page address = i
		this->send_command( 0xB0 + i );			//	set page address (p.42: No.12)
		//	set column address = 24 (0x18)
		this->send_command( 0x08 );				//	set lower column address (p.23: Section 1)
		this->send_command( 0x11 );				//	set higher column address(p.23: Section 2)
		for( j = 0; j < 80; j++ ) {
			this->send_data( j );
		}
	}
}

// --------------------------------------------------------------------
void CSANGRIA_OLED::update( void ) {
}
