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
// --------------------------------------------------------------------

#include <unistd.h>
#include <string.h>
#include <armbianio.h>
#include "sangria_effects.h"
#include "sharp_memory_display_driver.h"

static const unsigned char splash_logo[] = {
#include "startup_logo.txt"
};

// --------------------------------------------------------------------
void sangria_initialize( void ) {
}

// --------------------------------------------------------------------
void sangria_terminate( void ) {
}

// --------------------------------------------------------------------
static void animation_logo_appearance( unsigned char *p_image, int i ) {
	int y, src_y;

	for( y = 0; y < 240; y++ ) {
		src_y = (y - 120) * i / 32 + 120;
		memcpy( p_image, splash_logo + src_y * 50, 50 );
		p_image += 50;
	}
}

// --------------------------------------------------------------------
void sangria_splash( void ) {
	int i;
	unsigned char frame_buffer[ 50 * 240 ];

	for( i = 0; i <= 32; i++ ) {
		animation_logo_appearance( frame_buffer, i );
		smdd_transfer_bitmap( frame_buffer );
	}
	usleep( 2000000 );
	for( i = 31; i >= 0; i-- ) {
		animation_logo_appearance( frame_buffer, i );
		smdd_transfer_bitmap( frame_buffer );
	}
}
