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

// Control byte
#define SSD1306_CONTROL_CMD_SINGLE      0x80
#define SSD1306_CONTROL_CMD_STREAM      0x00
#define SSD1306_CONTROL_DATA_STREAM     0x40

// Fundamental commands (pg.28)
#define SSD1306_SET_CONTRAST            0x81
#define SSD1306_DISPLAY_ALL_ON_RESUME   0xA4
#define SSD1306_DISPLAY_ALL_ON_IGNORE   0xA5
#define SSD1306_NORMAL_DISPLAY          0xA6
#define SSD1306_INVERT_DISPLAY          0xA7
#define SSD1306_DISPLAY_OFF             0xAE
#define SSD1306_DISPLAY_ON              0xAF

// Scrolling #defines (pg.28-30)
#define SSD1306_RIGHT_HORIZONTAL_SCROLL                 0x26
#define SSD1306_LEFT_HORIZONTAL_SCROLL                  0x27
#define SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL    0x29
#define SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL     0x2A
#define SSD1306_DEACTIVATE_SCROLL                       0x2E
#define SSD1306_ACTIVATE_SCROLL                         0x2F
#define SSD1306_SET_VERTICAL_SCROLL_AREA                0xA3

// Addressing Command Table (pg.30)
#define SSD1306_SET_LOW_COLUMN          0x00
#define SSD1306_SET_HIGH_COLUMN         0x10
#define SSD1306_SET_MEMORY_ADDR_MODE    0x20    // follow with 0x00 = HORZ mode = Behave like a KS108 graphic LCD
#define SSD1306_SET_COLUMN_RANGE        0x21    // can be used only in HORZ/VERT mode - follow with 0x00 and 0x7F = COL127
#define SSD1306_SET_PAGE_RANGE          0x22    // can be used only in HORZ/VERT mode - follow with 0x00 and 0x07 = PAGE7
#define SSD1306_SET_PAGE_START_ADDRESS  0xB0

// Hardware Config (pg.31)
#define SSD1306_SET_DISPLAY_START_LINE  0x40
#define SSD1306_SET_SEGMENT_REMAP_LOW   0xA0
#define SSD1306_SET_SEGMENT_REMAP_HIGH  0xA1
#define SSD1306_SET_MULTIPLEX_RATIO     0xA8    // follow with 0x3F = 64 MUX
#define SSD1306_SET_COM_SCAN_INC        0xC0
#define SSD1306_SET_COM_SCAN_DEC        0xC8
#define SSD1306_SET_DISPLAY_OFFSET      0xD3    // follow with 0x00
#define SSD1306_SET_COM_PIN_MAP         0xDA    // follow with 0x12

// Timing and Driving Scheme (pg.32)
#define SSD1306_SET_DISPLAY_CLK_DIV     0xD5    // follow with 0x80
#define SSD1306_SET_PRECHARGE           0xD9    // follow with 0xF1
#define SSD1306_SET_VCOMH_DESELCT       0xDB    // follow with 0x30
#define SSD1306_NOP                     0xE3    // NOP

// Charge Pump (pg.62)
#define SSD1306_SET_CHARGE_PUMP         0x8D    // follow with 0x14

// --------------------------------------------------------------------
void CSANGRIA_OLED::send_command( uint8_t command ) {
	uint8_t send_buffer[2] = { OLED_CONTROL_CMD_STREAM, command };

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
	uint8_t buf[160];

	//	I2C initialization, OLED connected SH1107 by I2C.
	printf( "i2c_init;\n" );
	i2c_init( SANGRIA_OLED_I2C, SANGRIA_OLED_CLOCK );
	gpio_set_function( SANGRIA_OLED_SDA, GPIO_FUNC_I2C );
	gpio_set_function( SANGRIA_OLED_SCL, GPIO_FUNC_I2C );
	gpio_pull_up( SANGRIA_OLED_SDA );
	gpio_pull_up( SANGRIA_OLED_SCL );
	bi_decl( bi_2pins_with_func( SANGRIA_OLED_SDA, SANGRIA_OLED_SCL, GPIO_FUNC_I2C ) );

	buf[0] = SSD1306_CONTROL_CMD_STREAM;				/* コントロールバイト */
	
	/* Display off */
	buf[1] = SSD1306_DISPLAY_OFF;
	
	/* Set MUX Ratio A8h, 3Fh*/
	buf[2] = SSD1306_SET_MULTIPLEX_RATIO;
	buf[3] =0x3F;
	
	/* Set Display Offset D3h, 00h */
	buf[4] = SSD1306_SET_DISPLAY_OFFSET;
	buf[5] = 0x00;
	
	/* Set Display Start Line 40h */
	buf[6] = SSD1306_SET_DISPLAY_START_LINE;
	
	/* Set Segment re-map A0h/A1h */
	buf[7] = SSD1306_SET_SEGMENT_REMAP_LOW;
	
	/* Set COM Output Scan Direction C0h/C8h */
	buf[8] = SSD1306_SET_COM_SCAN_INC;
	
	/* Set COM Pins hardware configuration DAh, 02*/
	buf[9] = SSD1306_SET_COM_PIN_MAP;
	buf[10] = 0x12;
	
	/* Set Contrast Control 81h, 7Fh */
	buf[11] = SSD1306_SET_CONTRAST;
	buf[12] = 0x7F;
	
	/* Disable Entire Display On A4h */
	buf[13] = SSD1306_DISPLAY_ALL_ON_RESUME;
	
	/* Set Normal Display A6h */
	buf[14] = SSD1306_NORMAL_DISPLAY;
	
	/* Set Osc Frequency D5h, 80h */
	buf[15] = SSD1306_SET_DISPLAY_CLK_DIV;
	buf[16] = 0x80;	
	
	buf[17] = SSD1306_SET_MEMORY_ADDR_MODE;
	buf[18] = 0x10;
	
	buf[19] = SSD1306_SET_COLUMN_RANGE;
	buf[20] = 0x00;
	buf[21] = 0x7F;
	
	buf[22] = SSD1306_SET_PAGE_RANGE;
	buf[23] = 0x00;
	buf[24] = 0x07;
	
	buf[25] = SSD1306_DEACTIVATE_SCROLL;
	
	/* Enable charge pump regulator 8Dh, 14h */
	buf[26] = SSD1306_SET_CHARGE_PUMP;
	buf[27] = 0x14;
	
	/* Display On AFh */
	buf[28] = SSD1306_DISPLAY_ON;
	
	i2c_write_blocking (SANGRIA_OLED_I2C, SANGRIA_OLED_ADDR, buf, 29, false);

	for(i = 0; i < 8; i++) {
		buf[0] = SSD1306_CONTROL_CMD_STREAM;			/* コントロールバイト */
		buf[1] = SSD1306_SET_PAGE_START_ADDRESS | i;	/* set page start address(B0～B7) */
		buf[2] = SSD1306_SET_COLUMN_RANGE;				/* set Column Address */
		buf[3] = 0x00;									/* Column Start Address(0-127) */
		buf[4] = 0x7F;									/* Column Stop Address(0-127) */
		
		(void)i2c_write_blocking (SANGRIA_OLED_I2C, SANGRIA_OLED_ADDR, buf, 5, false);
		
		buf[0] = SSD1306_CONTROL_DATA_STREAM;			/*	コントロールバイト */
		for(j = 0; j < 128; j++) {
			buf[1 + j] = 0;
		}
		i2c_write_blocking (SANGRIA_OLED_I2C, SANGRIA_OLED_ADDR, buf, 129, false);
	}
}

// --------------------------------------------------------------------
void CSANGRIA_OLED::update( void ) {
	uint8_t buf[160];
	int i, j;

	for(i = 0; i < 8; i++) {
		buf[0] = SSD1306_CONTROL_CMD_STREAM;			/* コントロールバイト */
		buf[1] = SSD1306_SET_PAGE_START_ADDRESS | i;	/* set page start address(B0～B7) */
		buf[2] = SSD1306_SET_COLUMN_RANGE;				/* set Column Address */
		buf[3] = 0x00;									/* Column Start Address(0-127) */
		buf[4] = 0x7F;									/* Column Stop Address(0-127) */
		
		(void)i2c_write_blocking (SANGRIA_OLED_I2C, SANGRIA_OLED_ADDR, buf, 5, false);
		
		buf[0] = SSD1306_CONTROL_DATA_STREAM;			/*	コントロールバイト */
		for(j = 0; j < 128; j++) {
			buf[1 + j] = (uint8_t) rand();
		}
		i2c_write_blocking (SANGRIA_OLED_I2C, SANGRIA_OLED_ADDR, buf, 129, false);
	}
}
