// --------------------------------------------------------------------
// Sharp Memory Display Driver
// ====================================================================
//	Copyright 2022 t.hara
//
//	Permission is hereby granted, free of charge, to any person obtaining 
//	a copy of this software and associated documentation files (the "Software"), 
//	to deal in the Software without restriction, including without limitation 
//	the rights to use, copy, modify, merge, publish, distribute, sublicense, 
//	and/or sell copies of the Software, and to permit persons to whom the 
//	Software is furnished to do so, subject to the following conditions:
//
//	The above copyright notice and this permission notice shall be included in 
//	all copies or substantial portions of the Software.
//
//	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, 
//	EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF 
//	MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. 
//	IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, 
//	DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, 
//	ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
//	DEALINGS IN THE SOFTWARE.
// ====================================================================
// 2022/Oct/18th	t.hara
// --------------------------------------------------------------------

#include <armbianio.h>
#include <stdio.h>
#include <string.h>
#include "sharp_memory_display_driver.h"
#include "fb_convert.h"

//	Parameter
static const int		ref_height	= 240;

static unsigned char	invert		= 0;
static int				threshold	= 128;
static int				hspi		= -1;

#define CS				28
#define DISPON			32
#define EXTCOMIN		27

#define SPEED			3500000

// --------------------------------------------s------------------------
//	Table for inverting the sequence of bits
//	{ b7, b6, ... , b0 } --> { b0, b1, ... , b7 }
//
static const unsigned char mirror[256] = {
	0, 128, 64, 192, 32, 160, 96, 224, 16, 144, 80, 208, 48, 176, 112, 240,
	8, 136, 72, 200, 40, 168, 104, 232, 24, 152, 88, 216, 56, 184, 120, 248,
	4, 132, 68, 196, 36, 164, 100, 228, 20, 148, 84, 212, 52, 180, 116, 244,
	12, 140, 76, 204, 44, 172, 108, 236, 28, 156, 92, 220, 60, 188, 124, 252,
	2, 130, 66, 194, 34, 162, 98, 226, 18, 146, 82, 210, 50, 178, 114, 242,
	10, 138, 74, 202, 42, 170, 106, 234, 26, 154, 90, 218, 58, 186, 122, 250,
	6, 134, 70, 198, 38, 166, 102, 230, 22, 150, 86, 214, 54, 182, 118, 246,
	14, 142, 78, 206, 46, 174, 110, 238, 30, 158, 94, 222, 62, 190, 126, 254,
	1, 129, 65, 193, 33, 161, 97, 225, 17, 145, 81, 209, 49, 177, 113, 241,
	9, 137, 73, 201, 41, 169, 105, 233, 25, 153, 89, 217, 57, 185, 121, 249,
	5, 133, 69, 197, 37, 165, 101, 229, 21, 149, 85, 213, 53, 181, 117, 245,
	13, 141, 77, 205, 45, 173, 109, 237, 29, 157, 93, 221, 61, 189, 125, 253,
	3, 131, 67, 195, 35, 163, 99, 227, 19, 147, 83, 211, 51, 179, 115, 243,
	11, 139, 75, 203, 43, 171, 107, 235, 27, 155, 91, 219, 59, 187, 123, 251,
	7, 135, 71, 199, 39, 167, 103, 231, 23, 151, 87, 215, 55, 183, 119, 247,
	15, 143, 79, 207, 47, 175, 111, 239, 31, 159, 95, 223, 63, 191, 127, 255
};

static void smdd_convert_image16( const uint16_t *p_src, unsigned char *p_dest );
static void smdd_convert_image32( const uint32_t *p_src, unsigned char *p_dest );

static void (*p_convert_image)( const void *p_src, unsigned char *p_dest ) = NULL;

// --------------------------------------------------------------------
int smdd_initialize( void ) {
	int bpp;
	unsigned char frame_buffer[ 50 * 240 ] = { 0 };

	AIOInitBoard( "Raspberry Pi" );
	AIOAddGPIO( CS			, GPIO_OUT );
	AIOAddGPIO( DISPON		, GPIO_OUT );
	AIOAddGPIO( EXTCOMIN	, GPIO_OUT );

	//	Get SPI handle
	hspi = AIOOpenSPI(SPI_BUS_NUMBER, SPEED);
	if( hspi == -1 ) {
		return 0;
	}

	AIOWriteGPIO( DISPON	, 0 );
	AIOWriteGPIO( EXTCOMIN	, 0 );
	AIOWriteGPIO( CS		, 0 );

	//	clear display memory
	smdd_transfer_bitmap( frame_buffer );
	AIOWriteGPIO( DISPON	, 1 );

	fbc_get_size( NULL, NULL, &bpp );
	if( bpp == 32 ) {
		p_convert_image = (void*) smdd_convert_image32;
	}
	else {
		p_convert_image = (void*) smdd_convert_image16;
	}
	return 1;
}

// --------------------------------------------------------------------
void smdd_terminate( void ) {
	AIOWriteGPIO( DISPON, 0 );
}

// --------------------------------------------------------------------
void smdd_display( int switch_on ) {
	AIOWriteGPIO( DISPON	, switch_on );
}

// --------------------------------------------------------------------
void smdd_transfer_bitmap( unsigned char *p_image ) {
	int y;
	unsigned char buffer = 0x80, zero = 0;
	unsigned char *p = p_image;

	AIOWriteGPIO( CS, 1 );
	AIOWriteSPI( hspi, &buffer, 1 );
	for( y = 0; y < ref_height; y++ ) {
		//	line number
		buffer = mirror[ y + 1 ];
		AIOWriteSPI( hspi, &buffer, 1 );
		AIOWriteSPI( hspi, p, 50 );
		AIOWriteSPI( hspi, &zero, 1 );
		p += 50;
	}
	AIOWriteGPIO( CS, 0 );
}

// --------------------------------------------------------------------
static void smdd_convert_image32( const uint32_t *p_src, unsigned char *p_dest ) {
	int x, y, width, height, bit_count;
	uint32_t p;
	unsigned char d;

	width		= 400;
	height		= 240;
	d			= 0;
	bit_count	= 0;

	for( y = 0; y < height; y++ ) {
		for( x = 0; x < width; x++ ) {
			p = *(p_src++);
			p = (p & 255) + ((p >> 8) & 255) + ((p >> 16) & 255);
			d <<= 1;
			d += ( p > threshold );
			bit_count++;
			if( bit_count >= 8 ) {
				*(p_dest++) = d ^ invert;
				bit_count = 0;
			}
		}
	}
}

// --------------------------------------------------------------------
static void smdd_convert_image16( const uint16_t *p_src, unsigned char *p_dest ) {
	int x, y, width, height, bit_count;
	uint32_t p;
	unsigned char d;

	width		= 400;
	height		= 240;
	d			= 0;
	bit_count	= 0;

	for( y = 0; y < height; y++ ) {
		for( x = 0; x < width; x++ ) {
			p = (uint32_t) *(p_src++);
			p = ((p & 31) + ((p >> 6) & 31) + ((p >> 11) & 31)) << 3;
			d <<= 1;
			d += ( p > threshold );
			bit_count++;
			if( bit_count >= 8 ) {
				*(p_dest++) = d ^ invert;
				bit_count = 0;
			}
		}
	}
}

// --------------------------------------------------------------------
void smdd_convert_image( const void *p_src, unsigned char *p_dest ) {

	p_convert_image( p_src, p_dest );
}

// --------------------------------------------------------------------
void smdd_set_invert( int inv ) {

	if( inv ) {
		invert = 0xFF;
	}
	else {
		invert = 0x00;
	}
}

// --------------------------------------------------------------------
void smdd_set_threshold( int thres ) {

	threshold = thres;
}
