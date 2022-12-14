// --------------------------------------------------------------------
// Sangria game library DEMO
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "sangria_glib.h"
#include "startup_logo.h"

// --------------------------------------------------------------------
static void rotate_matrix( int *p_x, int *p_y, int x, int y, double c, double s ) {

	x -= 200;
	y -= 120;
	*p_x = (int)(x * c - y * s) + 200;
	*p_y = (int)(x * s + y * c) + 120;
}

// --------------------------------------------------------------------
static void rotate( SANGRIA_BACKBUFFER_T *p_screen, int deg, double zoom ) {
	int sx1, sx2, sx3, sy1, sy2, sy3;
	double rad, s, c;

	rad = deg * (2. * M_PI) / 256.;
	c = cos( rad ) * zoom;
	s = sin( rad ) * zoom;
	rotate_matrix( &sx1, &sy1,   0,   0, c, s );
	rotate_matrix( &sx2, &sy2, 399,   0, c, s );
	rotate_matrix( &sx3, &sy3,   0, 239, c, s );
	sangria_rotate_copy( p_startup_logo, sx1, sy1, sx2, sy2, sx3, sy3, p_screen, 0, 0, 399, 239 );
	sangria_display( p_screen );
}

// --------------------------------------------------------------------
static void demo( void ) {
	SANGRIA_BACKBUFFER_T *p_screen;
	int deg, zoom;

	p_screen = sangria_get_display();

	for(;;) {
		for( deg = 0; deg < 256; deg++ ) {
			rotate( p_screen, deg & 255, 1. );
		}
		for( zoom = 0; zoom < 256; zoom++ ) {
			deg = (deg + 1) & 255;
			rotate( p_screen, deg & 255, cos( zoom * M_PI / 64. ) * 2. );;
		}
	}
}

// --------------------------------------------------------------------
int main( int argc, char *argv[] ) {

	if( !sangria_initialize() ) {
		printf( "ERROR: Failed sangria_initialize()\n" );
		return 1;
	}

	demo();
	sangria_terminate();
	return 0;
}
