// --------------------------------------------------------------------
// Sangria game library
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
// --------------------------------------------------------------------

#ifndef __SANGRIA_SANGRIA_H__
#define __SANGRIA_SANGRIA_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// --------------------------------------------------------------------
//	SANGRIA_BACKBUFFER_T
// --------------------------------------------------------------------
typedef struct {
	int32_t		width;
	int32_t		height;
	uint8_t		image[1];
} SANGRIA_BACKBUFFER_T;

// --------------------------------------------------------------------
//	sangria_initialize()
//	input)
//		none
//	output)
//		0 ..... Failed.
//		!0 .... Success.
// --------------------------------------------------------------------
int sangria_initialize( void );

// --------------------------------------------------------------------
//	sangria_terminate()
//	input)
//		none
//	output)
//		none
// --------------------------------------------------------------------
void sangria_terminate( void );

// --------------------------------------------------------------------
//	sangria_display()
//	input)
//		p_image .... 8bit image data (0...127: black, 128...255: white)
//	output)
//		none
//	comment)
//		Size must be 400x240.
// --------------------------------------------------------------------
void sangria_display( const SANGRIA_BACKBUFFER_T *p_image );

// --------------------------------------------------------------------
//	sangria_get_backbuffer()
//	input)
//		width ...... width of target backbuffer
//		height ..... height of target backbuffer
//	output)
//		NULL ....... failed
// --------------------------------------------------------------------
SANGRIA_BACKBUFFER_T *sangria_get_backbuffer( uint32_t width, uint32_t height );

// --------------------------------------------------------------------
//	sangria_get_display()
//	input)
//		none
//	output)
//		NULL ....... failed
// --------------------------------------------------------------------
SANGRIA_BACKBUFFER_T *sangria_get_display( void );

// --------------------------------------------------------------------
//	sangria_release_backbuffer()
//	input)
//		p_image .... target backbuffer pointer
//	output)
//		none
// --------------------------------------------------------------------
void sangria_release_backbuffer( SANGRIA_BACKBUFFER_T *p_image );

// --------------------------------------------------------------------
//	sangria_clear_buffer()
//	input)
//		p_image .... target backbuffer pointer
//		c .......... pixel value
//	output)
//		none
// --------------------------------------------------------------------
void sangria_clear_buffer( SANGRIA_BACKBUFFER_T *p_image, uint8_t c );

// --------------------------------------------------------------------
//	sangria_set_pixel()
//	input)
//		p_image .... target backbuffer pointer
//		x .......... X position
//		y .......... Y position
//		c .......... pixel value
//	output)
//		none
//	comment)
//		If outside the back buffer is specified, it returns without doing anything.
// --------------------------------------------------------------------
void sangria_set_pixel( SANGRIA_BACKBUFFER_T *p_image, int x, int y, uint8_t c );

// --------------------------------------------------------------------
//	sangria_get_pixel()
//	input)
//		p_image .... target backbuffer pointer
//		x .......... X position
//		y .......... Y position
//	output)
//		pixel value
//	comment)
//		If outside the back buffer is specified, 0 is returned.
// --------------------------------------------------------------------
uint8_t sangria_get_pixel( SANGRIA_BACKBUFFER_T *p_image, int x, int y );

// --------------------------------------------------------------------
//	sangria_line()
//	input)
//		p_image .... target backbuffer pointer
//		x1 ......... start X position
//		y1 ......... start Y position
//		x2 ......... end X position
//		y2 ......... end Y position
//		c .......... pixel value
//	output)
//		none
// --------------------------------------------------------------------
void sangria_line( SANGRIA_BACKBUFFER_T *p_image, int x1, int y1, int x2, int y2, uint8_t c );

// --------------------------------------------------------------------
//	sangria_fill_rect()
//	input)
//		p_image .... target backbuffer pointer
//		x1 ......... start X position
//		y1 ......... start Y position
//		x2 ......... end X position
//		y2 ......... end Y position
//		c .......... pixel value
//	output)
//		none
// --------------------------------------------------------------------
void sangria_fill_rect( SANGRIA_BACKBUFFER_T *p_image, int x1, int y1, int x2, int y2, uint8_t c );

// --------------------------------------------------------------------
//	sangria_copy()
//	input)
//		p_src_image .... source backbuffer pointer
//		sx1 ............ start X position on source
//		sy1 ............ start Y position on source
//		sx2 ............ end X position on source
//		sy2 ............ end Y position on source
//		p_dest_image ... destination backbuffer pointer
//		dx1 ............ start X position on destination
//		dy1 ............ start Y position on destination
//	output)
//		none
// --------------------------------------------------------------------
void sangria_copy( SANGRIA_BACKBUFFER_T *p_src_image, int sx1, int sy1, int sx2, int sy2, SANGRIA_BACKBUFFER_T *p_dest_image, int dx1, int dy1 );

// --------------------------------------------------------------------
//	sangria_stretch_copy()
//	input)
//		p_src_image .... source backbuffer pointer
//		sx1 ............ start X position on source
//		sy1 ............ start Y position on source
//		sx2 ............ end X position on source
//		sy2 ............ end Y position on source
//		p_dest_image ... destination backbuffer pointer
//		dx1 ............ start X position on destination
//		dy1 ............ start Y position on destination
//		dx2 ............ end X position on destination
//		dy2 ............ end Y position on destination
//	output)
//		none
// --------------------------------------------------------------------
void sangria_stretch_copy( SANGRIA_BACKBUFFER_T *p_src_image, int sx1, int sy1, int sx2, int sy2, SANGRIA_BACKBUFFER_T *p_dest_image, int dx1, int dy1, int dx2, int dy2 );

// --------------------------------------------------------------------
//	sangria_rotate_copy()
//	input)
//		p_src_image .... source backbuffer pointer
//		sx1 ............ X1 position on source
//		sy1 ............ Y1 position on source
//		sx2 ............ X2 position on source
//		sy2 ............ Y2 position on source
//		sx3 ............ X3 position on source
//		sy3 ............ Y3 position on source
//		p_dest_image ... destination backbuffer pointer
//		dx1 ............ start X position on destination
//		dy1 ............ start Y position on destination
//		dx2 ............ end X position on destination
//		dy2 ............ end Y position on destination
//	output)
//		none
// --------------------------------------------------------------------
void sangria_rotate_copy( SANGRIA_BACKBUFFER_T *p_src_image, int sx1, int sy1, int sx2, int sy2, int sx3, int sy3, SANGRIA_BACKBUFFER_T *p_dest_image, int dx1, int dy1, int dx2, int dy2 );

// --------------------------------------------------------------------
//	sangria_led
//	input)
//		led ...... 0: Left-up, 1: Right-up, 2: Left-down, 3: Right-down
//		led_on ... 0: off, 1: on
//	output)
//		none
//	comment)
//		LED position
//			0 1
//			2 3
// --------------------------------------------------------------------
typedef enum {
	SANGRIA_LED_LU = 0,
	SANGRIA_LED_RU = 1,
	SANGRIA_LED_LD = 2,
	SANGRIA_LED_RD = 3,
} SANGRIA_LED_T;

typedef enum {
	SANGRIA_LED_OFF = 0,
	SANGRIA_LED_ON = 1,
} SANGRIA_LED_ON_T;

void sangria_led( SANGRIA_LED_T led, SANGRIA_LED_ON_T led_on );

// --------------------------------------------------------------------
//	sangria_get_key_state
//	input)
//		none
//	output)
//		bit7 ... 0: up off    , 1: up on
//		bit6 ... 0: left off  , 1: left on
//		bit5 ... 0: right off , 1: right on
//		bit4 ... 0: down off  , 1: down on
//		bit0 ... 0: button off, 1: button on
// --------------------------------------------------------------------
typedef enum {
	SANGRIA_KEY_UP		= 1 << 7,
	SANGRIA_KEY_LEFT	= 1 << 6,
	SANGRIA_KEY_RIGHT	= 1 << 5,
	SANGRIA_KEY_DOWN	= 1 << 4,
	SANGRIA_KEY_BUTTON	= 1 << 0,
} SANGRIA_KEY_STATE_T;

SANGRIA_KEY_STATE_T sangria_get_key_state( void );

#ifdef __cplusplus
}
#endif

#endif
