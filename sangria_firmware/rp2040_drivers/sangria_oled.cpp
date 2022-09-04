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
#include <cstring>

#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/i2c.h"
#include "pico/binary_info.h"

#include "sangria_oled.h"
#include "sangria_graphic_resource.h"

// I2C address
#define OLED_ADDR										0x3C

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

#define OLED_PAGE_HEIGHT			8
#define OLED_NUM_PAGES				(OLED_HEIGHT / OLED_PAGE_HEIGHT)
#define OLED_BUF_LEN				(OLED_NUM_PAGES * OLED_WIDTH)

#define OLED_FONT_WIDTH				8
#define OLED_FONT_HEIGHT			8
#define OLED_CHAR_WIDTH				(OLED_WIDTH / OLED_FONT_WIDTH)
#define OLED_CHAR_HEIGHT			(OLED_HEIGHT / OLED_FONT_HEIGHT)

// --------------------------------------------------------------------
CSANGRIA_OLED::CSANGRIA_OLED() {

	this->p_i2c = nullptr;
	gpio_init( SANGRIA_OLED_ON_N );
	gpio_init( SANGRIA_OLED_RST_N );

	gpio_set_dir( SANGRIA_OLED_ON_N, GPIO_OUT );
	gpio_set_dir( SANGRIA_OLED_RST_N, GPIO_OUT );

	this->power_off();
	sleep_ms( 10 );
}

// --------------------------------------------------------------------
void CSANGRIA_OLED::set_i2c( CSANGRIA_I2C *p_i2c ) {

	this->p_i2c = p_i2c;
}

// --------------------------------------------------------------------
void CSANGRIA_OLED::power_on( void ) {
	int count;

	//	Built-in DC-DC pump power is being used immediately after turning on the power: (Data sheet p.43)
	//	- Turn on the VDD and AVDD power, keep the RES pin="L" (>10us)
	gpio_put( SANGRIA_OLED_ON_N, OLED_POWER_ON );
	sleep_ms( 1 );

	//	- Set up initial code (user setup)
	count = 0;
	this->send_buffer[count++] = SSD1306_CONTROL_CMD_STREAM;		// Control byte

	this->send_buffer[count++] = SSD1306_DISPLAY_OFF;				// set display off

	this->send_buffer[count++] = SSD1306_SET_MULTIPLEX_RATIO;		// set multiplex ration
	this->send_buffer[count++] = 0x3F;								// - 63

	this->send_buffer[count++] = SSD1306_SET_DISPLAY_OFFSET;		// set display offset
	#if SANGRIA_OLED_REV == 1
		this->send_buffer[count++] = 0x20;								// - Vertical reverse
		this->send_buffer[count++] = SSD1306_SET_DISPLAY_START_LINE;	// set start line
		this->send_buffer[count++] = SSD1306_SET_SEGMENT_REMAP_HIGH;	// set segment re-map: ADC=1  (left-right reverse)
		this->send_buffer[count++] = SSD1306_SET_COM_SCAN_DEC;			// set common output scan direction
	#else
		this->send_buffer[count++] = 0x00;								// - Normal
		this->send_buffer[count++] = SSD1306_SET_DISPLAY_START_LINE;	// set start line
		this->send_buffer[count++] = SSD1306_SET_SEGMENT_REMAP_LOW;		// set segment re-map: ADC=1
		this->send_buffer[count++] = SSD1306_SET_COM_SCAN_INC;			// set common output scan direction
	#endif

	this->send_buffer[count++] = SSD1306_SET_COM_PIN_MAP;			// set COM pins hardware configuration DAh, 02h
	this->send_buffer[count++] = 0x02;								// 

	this->send_buffer[count++] = SSD1306_SET_CONTRAST;				// set contrast control
	this->send_buffer[count++] = 0x7F;								// - 127

	this->send_buffer[count++] = SSD1306_DISPLAY_ALL_ON_RESUME;		// disable entire display on A4h

	this->send_buffer[count++] = SSD1306_NORMAL_DISPLAY;			// set normal (not inverted) display

	this->send_buffer[count++] = SSD1306_SET_DISPLAY_CLK_DIV;		// set OSC frequency D5h, 80h
	this->send_buffer[count++] = 0x80;

	this->send_buffer[count++] = SSD1306_SET_CHARGE_PUMP;			// enable charge pump regulator 8Dh, 14h
	this->send_buffer[count++] = 0x14;

	this->send_buffer[count++] = SSD1306_DISPLAY_ON;				// set display on AFh

	p_i2c->write( OLED_ADDR, this->send_buffer, count );

	//	- Clear internal RAM to "00H"
	this->clear();
	this->update();
}

// --------------------------------------------------------------------
void CSANGRIA_OLED::power_off( void ) {
	//	OLED Power OFF
	gpio_put( SANGRIA_OLED_ON_N, OLED_POWER_OFF );
}

// --------------------------------------------------------------------
void CSANGRIA_OLED::update( void ) {
	int i, j, y, bit, pattern, count;

	for( i = 0, y = 0; i < OLED_NUM_PAGES; i++, y += OLED_PAGE_HEIGHT ) {
		count = 0;
		//	set page address = i
		this->send_buffer[count++] = SSD1306_CONTROL_CMD_STREAM;			// Control byte
		this->send_buffer[count++] = SSD1306_SET_PAGE_START_ADDRESS + i;	// set page address
		//	set column address = 24 (0x18)
		this->send_buffer[count++] = SSD1306_SET_COLUMN_RANGE;				// set column address
		this->send_buffer[count++] = 0x00;									//	column start address
		this->send_buffer[count++] = 0x7F;									//	column end address
		p_i2c->write( OLED_ADDR, this->send_buffer, count );

		count = 0;
		this->send_buffer[count++] = SSD1306_CONTROL_DATA_STREAM;			// Control byte
		for( j = 0; j < 128; j++ ) {
			pattern = 0;
			for( bit = 0; bit < OLED_PAGE_HEIGHT; bit++ ) {
				pattern = (pattern >> 1) | ((uint8_t)(this->frame_buffer[ (y + bit) * OLED_WIDTH + j ] != 0) << (OLED_PAGE_HEIGHT - 1));
			}
			this->send_buffer[count++] = (uint8_t) pattern;
		}
		p_i2c->write( OLED_ADDR, this->send_buffer, count );
	}
}

// --------------------------------------------------------------------
void CSANGRIA_OLED::clear( void ) {
	int y, x;

	for( y = 0; y < OLED_HEIGHT; y++ ) {
		for( x = 0; x < OLED_WIDTH; x++ ) {
			this->frame_buffer[ OLED_WIDTH * y + x ] = 0;
		}
	}
	this->x = 0;
	this->y = 0;
}

// --------------------------------------------------------------------
void CSANGRIA_OLED::pset( int x, int y, int c ) {

	if( x < 0 || y < 0 || x >= OLED_WIDTH || y >= OLED_HEIGHT ) {
		return;
	}
	this->frame_buffer[ OLED_WIDTH * y + x ] = ( c ? 1: 0 );
}

// --------------------------------------------------------------------
void CSANGRIA_OLED::line( int x1, int y1, int x2, int y2, int c ) {
	int x, y, dx, dy, vx, vy, count;

	dx = x2 - x1;
	dy = y2 - y1;

	vx = (dx == 0) ? 0: (dx < 0) ? -1: 1;
	vy = (dy == 0) ? 0: (dy < 0) ? -1: 1;
	dx = (dx < 0) ? -dx: dx;
	dy = (dy < 0) ? -dy: dy;

	if( dx > dy ) {
		for( x = x1, y = y1, count = 0; x != x2; x += vx ) {
			this->pset( x, y, c );
			count += dy;
			if( count >= dx ) {
				y += vy;
				count -= dx;
			}
		}
	}
	else {
		for( y = y1, x = x1, count = 0; y != y2; y += vy ) {
			this->pset( x, y, c );
			count += dx;
			if( count >= dy ) {
				x += vx;
				count -= dy;
			}
		}
	}
}

// --------------------------------------------------------------------
void CSANGRIA_OLED::copy( const uint8_t *p_image, int width, int height, int x, int y ) {
	int px, py;

	for( py = 0; py < height; py++ ) {
		for( px = 0; px < width; px++ ) {
			this->pset( x + px, y + py, *p_image );
			p_image++;
		}
	}
}

// --------------------------------------------------------------------
void CSANGRIA_OLED::copy_1bpp( const uint8_t *p_image, int width, int height, int x, int y ) {
	int px, py, bit_mask;

	for( py = 0; py < height; py++ ) {
		bit_mask = 0x80;
		for( px = 0; px < width; px++ ) {
			this->pset( x + px, y + py, *p_image & bit_mask );
			bit_mask >>= 1;
			if( bit_mask == 0 ) {
				p_image++;
				bit_mask = 0x80;
			}
		}
		if( bit_mask != 0x80 ) {
			p_image++;
		}
	}
}

// --------------------------------------------------------------------
void CSANGRIA_OLED::scroll_up( void ) {
	int y, x;

	for( y = 0; y < ((OLED_CHAR_HEIGHT - 1) * OLED_FONT_HEIGHT); y++ ) {
		for( x = 0; x < OLED_WIDTH; x++ ) {
			this->frame_buffer[ y * OLED_WIDTH + x ] = this->frame_buffer[ (y + OLED_FONT_HEIGHT) * OLED_WIDTH + x ];
		}
	}
	for( ; y < (OLED_CHAR_HEIGHT * OLED_FONT_HEIGHT); y++ ) {
		for( x = 0; x < OLED_WIDTH; x++ ) {
			this->frame_buffer[ y * OLED_WIDTH + x ] = 0;
		}
	}
}

// --------------------------------------------------------------------
void CSANGRIA_OLED::putc( char c ) {
	int xx, yy, pattern;

	//	右にはみ出していたら次の行の先頭へ移動
	//	If it extends to the right, move to the beginning of the next line
	if( this->x >= OLED_CHAR_WIDTH || c == '\n' ) {
		this->x = 0;
		this->y++;
		//	さらに下にはみ出したら1行上へスクロール
		//	Scroll up one line if it extends further down.
		if( this->y >= OLED_CHAR_HEIGHT ) {
			this->scroll_up();
			this->y = OLED_CHAR_HEIGHT - 1;
		}
	}
	if( (uint8_t)c < 32 || (uint8_t)c > 127 ) {
		return;
	}
	//	文字を描画する
	//	Drawing Characters
	const uint8_t *p_font = &(get_font()[ (((uint8_t)c) - 32) * OLED_FONT_HEIGHT ]);
	for( yy = 0; yy < OLED_FONT_HEIGHT; yy++ ) {
		pattern = *p_font;
		for( xx = 0; xx < OLED_FONT_WIDTH; xx++ ) {
			this->pset( this->x * OLED_FONT_WIDTH + xx, this->y * OLED_FONT_HEIGHT + yy, pattern & 0x80 );
			pattern <<= 1;
		}
		p_font++;
	}
	this->x++;
}

// --------------------------------------------------------------------
void CSANGRIA_OLED::puts( const char *p_str ) {

	while( *p_str ) {
		this->putc( *p_str );
		p_str++;
	}
}

// --------------------------------------------------------------------
void CSANGRIA_OLED::set_position( int xx, int yy ) {

	if( xx < 0 ) {
		xx = 0;
	}
	if( yy < 0 ) {
		yy = 0;
	}
	if( xx >= OLED_CHAR_WIDTH ) {
		xx = OLED_CHAR_WIDTH;
	}
	if( yy >= OLED_CHAR_HEIGHT ) {
		yy = OLED_CHAR_HEIGHT;
	}
	this->x = xx;
	this->y = yy;
}
