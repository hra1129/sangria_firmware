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

#define OLED_CONTROL_CMD_SINGLE							0x80
#define OLED_CONTROL_CMD_STREAM							0x00
#define OLED_CONTROL_DATA_STREAM						0x40

// Control byte
#define SSD1306_CONTROL_CMD_SINGLE						0x80
#define SSD1306_CONTROL_CMD_STREAM						0x00
#define SSD1306_CONTROL_DATA_STREAM						0x40

// Fundamental commands (pg.28)
#define SSD1306_SET_CONTRAST							0x81
#define SSD1306_DISPLAY_ALL_ON_RESUME					0xA4
#define SSD1306_DISPLAY_ALL_ON_IGNORE					0xA5
#define SSD1306_NORMAL_DISPLAY							0xA6
#define SSD1306_INVERT_DISPLAY							0xA7
#define SSD1306_DISPLAY_OFF								0xAE
#define SSD1306_DISPLAY_ON								0xAF

// Scrolling #defines (pg.28-30)
#define SSD1306_RIGHT_HORIZONTAL_SCROLL					0x26
#define SSD1306_LEFT_HORIZONTAL_SCROLL					0x27
#define SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL	0x29
#define SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL		0x2A
#define SSD1306_DEACTIVATE_SCROLL						0x2E
#define SSD1306_ACTIVATE_SCROLL							0x2F
#define SSD1306_SET_VERTICAL_SCROLL_AREA				0xA3

// Addressing Command Table (pg.30)
#define SSD1306_SET_LOW_COLUMN							0x00
#define SSD1306_SET_HIGH_COLUMN							0x10
#define SSD1306_SET_MEMORY_ADDR_MODE					0x20	// follow with 0x00 = HORZ mode = Behave like a KS108 graphic LCD
#define SSD1306_SET_COLUMN_RANGE						0x21	// can be used only in HORZ/VERT mode - follow with 0x00 and 0x7F = COL127
#define SSD1306_SET_PAGE_RANGE							0x22	// can be used only in HORZ/VERT mode - follow with 0x00 and 0x07 = PAGE7
#define SSD1306_SET_PAGE_START_ADDRESS					0xB0

// Hardware Config (pg.31)
#define SSD1306_SET_DISPLAY_START_LINE					0x40
#define SSD1306_SET_SEGMENT_REMAP_LOW					0xA0
#define SSD1306_SET_SEGMENT_REMAP_HIGH					0xA1
#define SSD1306_SET_MULTIPLEX_RATIO						0xA8	// follow with 0x3F = 64 MUX
#define SSD1306_SET_COM_SCAN_INC						0xC0
#define SSD1306_SET_COM_SCAN_DEC						0xC8
#define SSD1306_SET_DISPLAY_OFFSET						0xD3	// follow with 0x00
#define SSD1306_SET_COM_PIN_MAP							0xDA	// follow with 0x12

// Timing and Driving Scheme (pg.32)
#define SSD1306_SET_DISPLAY_CLK_DIV						0xD5	// follow with 0x80
#define SSD1306_SET_PRECHARGE							0xD9	// follow with 0xF1
#define SSD1306_SET_VCOMH_DESELCT						0xDB	// follow with 0x30
#define SSD1306_NOP										0xE3	// NOP

// Charge Pump (pg.62)
#define SSD1306_SET_CHARGE_PUMP							0x8D	// follow with 0x14

#define OLED_HEIGHT					128
#define OLED_WIDTH					80
#define OLED_PAGE_HEIGHT			8
#define OLED_NUM_PAGES				(OLED_HEIGHT / OLED_PAGE_HEIGHT)
#define OLED_BUF_LEN				(OLED_NUM_PAGES * OLED_WIDTH)

// --------------------------------------------------------------------
CSANGRIA_OLED::CSANGRIA_OLED() {
	uint8_t i, j;
	uint8_t buf[160];
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
	buf[cnt++] = SSD1306_CONTROL_CMD_STREAM;		// Control byte

	buf[cnt++] = SSD1306_DISPLAY_OFF;				// set display off

	buf[cnt++] = SSD1306_SET_MULTIPLEX_RATIO;		// set multiplex ration
	buf[cnt++] = 0x3F;								// - 63

	buf[cnt++] = SSD1306_SET_DISPLAY_OFFSET;		// set display offset
	buf[cnt++] = 0x00;								// - 

	buf[cnt++] = SSD1306_SET_DISPLAY_START_LINE;	// set start line

	buf[cnt++] = SSD1306_SET_SEGMENT_REMAP_LOW;		// set segment re-map: ADC=1

	buf[cnt++] = SSD1306_SET_COM_SCAN_INC;			// set common output scan direction

	buf[cnt++] = SSD1306_SET_COM_PIN_MAP;			// set COM pins hardware configuration DAh, 12h
	buf[cnt++] = 0x12;								// 

	buf[cnt++] = SSD1306_SET_CONTRAST;				// set contrast control
	buf[cnt++] = 0x7F;								// - 127

	buf[cnt++] = SSD1306_DISPLAY_ALL_ON_RESUME;		// disable entire display on A4h

	buf[cnt++] = SSD1306_NORMAL_DISPLAY;			// set normal (not inverted) display

	buf[cnt++] = SSD1306_SET_DISPLAY_CLK_DIV;		// set OSC frequency D5h, 80h
	buf[cnt++] = 0x80;

	buf[cnt++] = SSD1306_SET_MEMORY_ADDR_MODE;
	buf[cnt++] = 0x10;
	
	buf[cnt++] = SSD1306_SET_COLUMN_RANGE;
	buf[cnt++] = 0x00;
	buf[cnt++] = 0x7F;
	
	buf[cnt++] = SSD1306_SET_PAGE_RANGE;
	buf[cnt++] = 0x00;
	buf[cnt++] = 0x07;
	
	buf[cnt++] = SSD1306_DEACTIVATE_SCROLL;

	buf[cnt++] = SSD1306_SET_CHARGE_PUMP;			// enable charge pump regulator 8Dh, 14h
	buf[cnt++] = 0x14;

	buf[cnt++] = SSD1306_DISPLAY_ON;				// set display on AFh

	i2c_write_blocking( SANGRIA_OLED_I2C, SANGRIA_OLED_ADDR, buf, cnt, false );

	//	- Clear internal RAM to "00H"
	for( i = 0; i < OLED_NUM_PAGES; i++ ) {
		cnt = 0;
		//	set page address = i
		buf[cnt++] = SSD1306_CONTROL_CMD_STREAM;			// Control byte
		buf[cnt++] = SSD1306_SET_PAGE_START_ADDRESS + i;	// set page address
		//	set column address = 24 (0x18)
		buf[cnt++] = SSD1306_SET_COLUMN_RANGE;				// set column address
		buf[cnt++] = 0x00;									//	column start address
		buf[cnt++] = 0x7F;									//	column end address
		i2c_write_blocking( SANGRIA_OLED_I2C, SANGRIA_OLED_ADDR, buf, cnt, false );

		cnt = 0;
		buf[cnt++] = SSD1306_CONTROL_DATA_STREAM;			// Control byte
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
		buf[cnt++] = SSD1306_CONTROL_CMD_STREAM;			// Control byte
		buf[cnt++] = SSD1306_SET_PAGE_START_ADDRESS + i;	// set page address
		//	set column address = 24 (0x18)
		buf[cnt++] = SSD1306_SET_COLUMN_RANGE;				// set column address
		buf[cnt++] = 0x00;									//	column start address
		buf[cnt++] = 0x7F;									//	column end address
		i2c_write_blocking( SANGRIA_OLED_I2C, SANGRIA_OLED_ADDR, buf, cnt, false );

		cnt = 0;
		buf[cnt++] = SSD1306_CONTROL_DATA_STREAM;			// Control byte
		for( j = 0; j < 128; j++ ) {
			buf[cnt++] = (uint8_t) rand();
		}
		i2c_write_blocking( SANGRIA_OLED_I2C, SANGRIA_OLED_ADDR, buf, cnt, false );
	}
}
