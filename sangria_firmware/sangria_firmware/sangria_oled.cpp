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

// Fundamental commands (pg.28-33)
#define OLED_SET_CONTRAST            0x81
#define OLED_DISPLAY_ALL_ON_RESUME   0xA4
#define OLED_DISPLAY_ALL_ON_IGNORE   0xA5
#define OLED_NORMAL_DISPLAY          0xA6
#define OLED_INVERT_DISPLAY          0xA7
#define OLED_DISPLAY_OFF             0xAE
#define OLED_DISPLAY_ON              0xAF

// Scrolling #defines (pg.28-30)
#define OLED_RIGHT_HORIZONTAL_SCROLL                 0x26
#define OLED_LEFT_HORIZONTAL_SCROLL                  0x27
#define OLED_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL    0x29
#define OLED_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL     0x2A
#define OLED_DEACTIVATE_SCROLL                       0x2E
#define OLED_ACTIVATE_SCROLL                         0x2F
#define OLED_SET_VERTICAL_SCROLL_AREA                0xA3

// Addressing Command Table (pg.30)
#define OLED_SET_LOW_COLUMN          0x00
#define OLED_SET_HIGH_COLUMN         0x10
#define OLED_SET_MEMORY_ADDR_MODE    0x20    // follow with 0x00 = HORZ mode = Behave like a KS108 graphic LCD
#define OLED_SET_COLUMN_RANGE        0x21    // can be used only in HORZ/VERT mode - follow with 0x00 and 0x7F = COL127
#define OLED_SET_PAGE_RANGE          0x22    // can be used only in HORZ/VERT mode - follow with 0x00 and 0x07 = PAGE7
#define OLED_SET_PAGE_START_ADDRESS  0xB0

// Hardware Config (pg.31)
#define OLED_SET_DISPLAY_START_LINE  0x40
#define OLED_SET_SEGMENT_REMAP_LOW   0xA0
#define OLED_SET_SEGMENT_REMAP_HIGH  0xA1
#define OLED_SET_MULTIPLEX_RATIO     0xA8    // follow with 0x3F = 64 MUX
#define OLED_SET_COM_SCAN_INC        0xC0
#define OLED_SET_COM_SCAN_DEC        0xC8
#define OLED_SET_DISPLAY_OFFSET      0xD3    // follow with 0x00
#define OLED_SET_COM_PIN_MAP         0xDA    // follow with 0x12

// Timing and Driving Scheme (pg.32)
#define OLED_SET_DISPLAY_CLK_DIV     0xD5    // follow with 0x80
#define OLED_SET_PRECHARGE           0xD9    // follow with 0xF1
#define OLED_SET_VCOMH_DESELCT       0xDB    // follow with 0x30
#define OLED_NOP                     0xE3    // NOP

// Charge Pump (pg.62)
#define OLED_SET_CHARGE_PUMP         0x8D    // follow with 0x14


//#define OLED_SET_CONTRAST			0x81
//#define OLED_SET_ENTIRE_ON			0xA4
//#define OLED_SET_NORM_INV			0xA6
//#define OLED_SET_DISP				0xAE
//#define OLED_SET_MEM_ADDR			0x20
//#define OLED_SET_COL_ADDR			0x21
//#define OLED_SET_PAGE_ADDR			0x22
//#define OLED_SET_DISP_START_LINE	0x40
//#define OLED_SET_SEG_REMAP			0xA0
//#define OLED_SET_MUX_RATIO			0xA8
//#define OLED_SET_COM_OUT_DIR		0xC0
//#define OLED_SET_DISP_OFFSET		0xD3
//#define OLED_SET_COM_PIN_CFG		0xDA
//#define OLED_SET_DISP_CLK_DIV		0xD5
//#define OLED_SET_PRECHARGE			0xD9
//#define OLED_SET_VCOM_DESEL			0xDB
//#define OLED_SET_CHARGE_PUMP		0x8D
//#define OLED_SET_HORIZ_SCROLL		0x26
//#define OLED_SET_SCROLL				0x2E

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
	gpio_put( SANGRIA_OLED_ON_N, 0 );

	//	I2C initialization, OLED connected SH1107 by I2C.
	i2c_init( SANGRIA_OLED_I2C, SANGRIA_OLED_CLOCK );
	gpio_set_function( SANGRIA_OLED_SDA, GPIO_FUNC_I2C );
	gpio_set_function( SANGRIA_OLED_SCL, GPIO_FUNC_I2C );
	gpio_pull_up( SANGRIA_OLED_SDA );
	gpio_pull_up( SANGRIA_OLED_SCL );
	bi_decl( bi_2pins_with_func( SANGRIA_OLED_SDA, SANGRIA_OLED_SCL, GPIO_FUNC_I2C ) );

	//	Put the BQ device in boost mode before putting 0 in SANGRIA_OLED_ON_N
	//	T.B.D.

	//	OLED Power ON and RESET
	//gpio_put( SANGRIA_OLED_ON_N, 0 );
	gpio_put( SANGRIA_OLED_RST_N, 0 );
	//	Reset pin must maintain L for at least 10us.
	sleep_us( 20 );
	gpio_put( SANGRIA_OLED_RST_N, 1 );
	sleep_ms( 120 );

//	this->send_command( OLED_SET_DISP | 0x00 );			// set display off
//
//	this->send_command( OLED_SET_MEM_ADDR );			// set memory address mode
//	this->send_command( 0x00 );							// horizontal addressing mode
//
//	this->send_command( OLED_SET_DISP_START_LINE );		// set display start line to 0
//
//	this->send_command( OLED_SET_SEG_REMAP | 0x01 );	// set segment re-map
//
//	this->send_command( OLED_SET_MUX_RATIO );			// set multiplex ratio
//	this->send_command( OLED_HEIGHT - 1 );				// our display is only 32 pixels high
//
//	this->send_command( OLED_SET_COM_OUT_DIR | 0x08 );	// set COM (common) output scan direction
//
//	this->send_command( OLED_SET_DISP_OFFSET );			// set display offset
//	this->send_command( 0x00 );							// no offset
//
//	this->send_command( OLED_SET_COM_PIN_CFG );			// set COM (common) pins hardware configuration
//	this->send_command( 0x02 );							// manufacturer magic number
//
//	/* timing and driving scheme */
//	this->send_command( OLED_SET_DISP_CLK_DIV );		// set display clock divide ratio
//	this->send_command( 0x80 );							// div ratio of 1, standard freq
//
//	this->send_command( OLED_SET_PRECHARGE );			// set pre-charge period
//	this->send_command( 0xF1 );							// Vcc internally generated on our board
//
//	this->send_command( OLED_SET_VCOM_DESEL );			// set VCOMH deselect level
//	this->send_command( 0x30 );							// 0.83xVcc
//
//	/* display */
//	this->send_command( OLED_SET_CONTRAST );			// set contrast control
//	this->send_command( 0xFF );
//
//	this->send_command( OLED_SET_ENTIRE_ON );			// set entire display on to follow RAM content
//
//	//this->send_command( OLED_SET_NORM_INV | 0x00 );		// set normal (not inverted) display
//	this->send_command( OLED_SET_NORM_INV | 0x01 );		// set inverted display
//
//	this->send_command( OLED_SET_CHARGE_PUMP );			// set charge pump
//	this->send_command( 0x14 );							// Vcc internally generated on our board
//
//	this->send_command( OLED_SET_SCROLL | 0x00 );		// deactivate horizontal scrolling if set
//
//	this->send_command( OLED_SET_DISP | 0x01 );			// turn display on

	uint8_t buf[256];
//	
//	buf[0] = OLED_CONTROL_CMD_STREAM;
//	
//	/* Display off */
//	buf[1] = OLED_DISPLAY_OFF;
//	
//	/* Set MUX Ratio A8h, 3Fh*/
//	buf[2] = OLED_SET_MULTIPLEX_RATIO;
//	buf[3] =0x3F;
//	
//	/* Set Display Offset D3h, 00h */
//	buf[4] = OLED_SET_DISPLAY_OFFSET;
//	buf[5] = 0x00;
//	
//	/* Set Display Start Line 40h */
//	buf[6] = OLED_SET_DISPLAY_START_LINE;
//	
//	/* Set Segment re-map A0h/A1h */
//	buf[7] = OLED_SET_SEGMENT_REMAP_LOW;
//	
//	/* Set COM Output Scan Direction C0h/C8h */
//	buf[8] = OLED_SET_COM_SCAN_INC;
//	
//	/* Set COM Pins hardware configuration DAh, 02*/
//	buf[9] = OLED_SET_COM_PIN_MAP;
//	buf[10] = 0x12;
//	
//	/* Set Contrast Control 81h, 7Fh */
//	buf[11] = OLED_SET_CONTRAST;
//	buf[12] = 0x7F;
//	
//	/* Disable Entire Display On A4h */
//	buf[13] = OLED_DISPLAY_ALL_ON_RESUME;
//	
//	/* Set Normal Display A6h */
//	buf[14] = OLED_NORMAL_DISPLAY;
//	
//	/* Set Osc Frequency D5h, 80h */
//	buf[15] = OLED_SET_DISPLAY_CLK_DIV;
//	buf[16] = 0x80;	
//	
//	buf[17] = OLED_SET_MEMORY_ADDR_MODE;
//	buf[18] = 0x10;
//	
//	buf[19] = OLED_SET_COLUMN_RANGE;
//	buf[20] = 0x00;
//	buf[21] = 0x7F;
//	
//	buf[22] = OLED_SET_PAGE_RANGE;
//	buf[23] = 0x00;
//	buf[24] = 0x07;
//	
//	buf[25] = OLED_DEACTIVATE_SCROLL;
//	
//	/* Enable charge pump regulator 8Dh, 14h */
//	buf[26] = OLED_SET_CHARGE_PUMP;
//	buf[27] = 0x14;
//	
//	/* Display On AFh */
//	buf[28] = OLED_DISPLAY_ON;
//	
//	i2c_write_blocking ( SANGRIA_OLED_I2C, SANGRIA_OLED_ADDR, buf, sizeof(buf), false );

	this->send_command( 0xAE );		// display off
	this->send_command( 0x00 );		// set lower column address
	this->send_command( 0x10 );		// set higher column address
	this->send_command( 0xB0 );		// set page address
	this->send_command( 0xDC );		// set display start line
	this->send_command( 0x00 );
	this->send_command( 0x81 );		// cotract control
	this->send_command( 0x6E );		// 128
	this->send_command( 0x20 );		// set memory addressing mode
	this->send_command( 0xA0 );		// set segment remap
	this->send_command( 0xC0 );		// com scan direction
	this->send_command( 0xA4 );		// disable entire display on
	this->send_command( 0xA6 );		// normal/reverse
	this->send_command( 0xA8 );		// multiplex ratio
	this->send_command( 0x3F );
	this->send_command( 0xD3 );		// set display offset
	this->send_command( 0x60 );
	this->send_command( 0xD5 );		// set osc division
	this->send_command( 0x41 );
	this->send_command( 0xD9 );		// set pre-charge period
	this->send_command( 0x22 );
	this->send_command( 0xDB );		// set vcomh
	this->send_command( 0x35 );
	this->send_command( 0xAD );		// set charge pump enable
	this->send_command( 0x8A );		// set DC-DC enable
	this->send_command( 0xAF );		// display on

	sleep_ms( 120 );

	uint8_t i,j;
	
	for(i = 0; i < 8; i++) {
		buf[0] = OLED_CONTROL_CMD_STREAM;
		buf[1] = OLED_SET_PAGE_START_ADDRESS | i;		// set page start address(B0`B7)
		buf[2] = OLED_SET_COLUMN_RANGE;					// set Column Address
		buf[3] = 0x00;									// Column Start Address(0-127)
		buf[4] = 0x7F;									// Column Stop Address(0-127)
		
		(void)i2c_write_blocking( SANGRIA_OLED_I2C, SANGRIA_OLED_ADDR, buf, 5, false );
		
		buf[0] = OLED_CONTROL_DATA_STREAM;
		for(j = 0; j < 8; j++) {
			buf[1 + j] = (i << 3) | j;
		}
		for(j = 0; j < 16; j++) {
			i2c_write_blocking( SANGRIA_OLED_I2C, SANGRIA_OLED_ADDR, buf, 9, false );
		}
	}
}

// --------------------------------------------------------------------
void CSANGRIA_OLED::update( void ) {
}
