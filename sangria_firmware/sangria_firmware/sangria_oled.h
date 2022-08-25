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

#ifndef __SANGRIA_OLED_H__
#define __SANGRIA_OLED_H__

#include <cstdint>
#include "sangria_firmware_config.h"

class CSANGRIA_OLED {
private:
	int x;
	int y;
	uint8_t frame_buffer[32][128];
	uint8_t send_buffer[256];

public:
	// --------------------------------------------------------------------
	//	Constructor
	CSANGRIA_OLED();

	// --------------------------------------------------------------------
	//	Update
	//	input:
	//		none
	//	output:
	//		none
	//	comment:
	//		Transfers the contents of the frame buffer to OLED.
	//
	void update( void );

	// --------------------------------------------------------------------
	//	Clear
	//	input:
	//		none
	//	output:
	//		none
	//	comment:
	//		Clear the contents of the frame buffer.
	//
	void clear( void );

	// --------------------------------------------------------------------
	//	Pixel set
	//	input:
	//		x ..... X position
	//		y ..... Y position
	//		c ..... color (0 or 1)
	//	output:
	//		none
	//	comment:
	//		Draw a point at coordinates (X,Y)
	//
	void pset( int x, int y, int c );

	// --------------------------------------------------------------------
	//	Line
	//	input:
	//		x1 .... X1 position
	//		y1 .... Y1 position
	//		x2 .... X2 position
	//		y2 .... Y2 position
	//		c ..... color (0 or 1)
	//	output:
	//		none
	//	comment:
	//		Draw a line from coordinates (X1,Y1) to coordinates (X2,Y2)
	//
	void line( int x1, int y1, int x2, int y2, int c );

	// --------------------------------------------------------------------
	//	Line
	//	input:
	//		p_image ... image data address
	//		width ..... width of image data
	//		height .... height of image data
	//		x ......... left position
	//		y ......... top position
	//	output:
	//		none
	//	comment:
	//		Draw a image
	//
	void copy( const uint8_t *p_image, int width, int height, int x = 0, int y = 0 );

	// --------------------------------------------------------------------
	//	scroll_up
	//	input:
	//		none
	//	output:
	//		none
	//	comment:
	//		Scroll
	//
	void scroll_up( void );

	// --------------------------------------------------------------------
	//	putc
	//	input:
	//		c ......... target character ASCII code
	//	output:
	//		none
	//	comment:
	//		Draw a character
	//
	void putc( char c );

	// --------------------------------------------------------------------
	//	putc
	//	input:
	//		p_str ...... target string
	//	output:
	//		none
	//	comment:
	//		Draw string
	//
	void puts( const char *p_str );

	// --------------------------------------------------------------------
	//	set_position
	//	input:
	//		xx ...... character drawing position X
	//		yy ...... character drawing position Y
	//	output:
	//		none
	//	comment:
	//
	void set_position( int xx, int yy );
};

#endif
