// --------------------------------------------------------------------
// sound demo
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
#include <unistd.h>
#include <math.h>
#include <sangria_slib.h>

#include <psg_emulator.h>

static const char *s_tone_name[] = { "C(L)", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B", "C(H)" };
static int tone_table[13];

// --------------------------------------------------------------------
static void makeup_tone_table( void ) {
	const double clock = 3579545.;			//	Hz
	double ratio, freq, tone_freq, cycles;
	int i;

	ratio	= pow( 2., 1./12. );
	freq	= 440 / pow( ratio, 9. );		//	frequency of C

	for( i = 0; i < 13; i++ ) {
		tone_freq		= freq * pow( ratio, (double) i );
		cycles			= clock / (tone_freq * 16);
		tone_table[i]	= (int) (cycles + 0.5);
	}
}

// --------------------------------------------------------------------
static void tone_test( H_PSG_T hpsg, int tone, int time ) {

	printf( "Tone %s.\n", s_tone_name[ tone ] );

	psg_write_register( hpsg, 0, tone_table[ tone ] & 255 );		//	tone period (L)
	psg_write_register( hpsg, 1, tone_table[ tone ] >> 8 );			//	tone period (H)
	psg_write_register( hpsg, 8, 15 );		//	volume
	sleep( time );
}

// --------------------------------------------------------------------
static void tone_test2( H_PSG_T hpsg, int tone1, int tone2, int tone3, int time ) {

	printf( "Tone %s-%s-%s.\n", s_tone_name[ tone1 ], s_tone_name[ tone2 ], s_tone_name[ tone3 ] );

	psg_write_register( hpsg, 0, tone_table[ tone1 ] & 255 );		//	tone period (L)
	psg_write_register( hpsg, 1, tone_table[ tone1 ] >> 8 );		//	tone period (H)
	psg_write_register( hpsg, 2, tone_table[ tone2 ] & 255 );		//	tone period (L)
	psg_write_register( hpsg, 3, tone_table[ tone2 ] >> 8 );		//	tone period (H)
	psg_write_register( hpsg, 4, tone_table[ tone3 ] & 255 );		//	tone period (L)
	psg_write_register( hpsg, 5, tone_table[ tone3 ] >> 8 );		//	tone period (H)
	psg_write_register( hpsg, 8, 15 );		//	volume
	psg_write_register( hpsg, 9, 15 );		//	volume
	psg_write_register( hpsg, 10, 15 );		//	volume
	sleep( time );
}

// --------------------------------------------------------------------
static void envelope_test( H_PSG_T hpsg, int tone, int time, int envelope, int envelope_freq ) {

	printf( "Tone %s with envelope %d.\n", s_tone_name[ tone ], envelope );
	psg_write_register( hpsg, 0, tone_table[ tone ] & 255 );		//	tone period (L)
	psg_write_register( hpsg, 1, tone_table[ tone ] >> 8 );			//	tone period (H)
	psg_write_register( hpsg, 11, envelope_freq & 255 );			//	envelope period (L)
	psg_write_register( hpsg, 12, envelope_freq >> 8 );				//	envelope period (H)
	psg_write_register( hpsg, 8, 16 );								//	use envelope
	psg_write_register( hpsg, 13, envelope );						//	envelope type
	sleep( time );
}

// --------------------------------------------------------------------
int main( int argc, char *argv[] ) {
	H_PSG_T hpsg;
	int i;
	
	sangria_sound_initialize();
	hpsg = sangria_get_psg_handle();

	makeup_tone_table();

	psg_write_register( hpsg, 7, 0b10111110 );
	tone_test( hpsg,  0, 1 );
	tone_test( hpsg,  2, 1 );
	tone_test( hpsg,  4, 1 );
	tone_test( hpsg,  5, 1 );
	tone_test( hpsg,  7, 1 );
	tone_test( hpsg,  9, 1 );
	tone_test( hpsg, 11, 1 );
	tone_test( hpsg, 12, 1 );
	printf( "Stop.\n" );
	psg_write_register( hpsg, 8, 0 );		//	stop tone
	sleep( 1 );

	psg_write_register( hpsg, 7, 0b10111000 );
	tone_test2( hpsg,  0, 4, 7, 1 );
	tone_test2( hpsg,  2, 6, 9, 1 );
	printf( "Stop.\n" );
	psg_write_register( hpsg, 8, 0 );		//	stop tone
	psg_write_register( hpsg, 9, 0 );		//	stop tone
	psg_write_register( hpsg, 10, 0 );		//	stop tone
	sleep( 1 );

	psg_write_register( hpsg, 8, 15 );		//	use envelope
	for( i = 0; i < 16; i++ ) {
		envelope_test( hpsg, 0, 4, i, 2000 );
	}
	printf( "Stop.\n" );
	psg_write_register( hpsg, 8, 0 );		//	stop tone
	sleep( 2 );

	psg_write_register( hpsg, 7, 0b10110111 );
	psg_write_register( hpsg, 0, 0 );		//	tone period (L)
	psg_write_register( hpsg, 1, 1 );		//	tone period (H)

	for( i = 0; i < 31; i++ ) {
		printf( "Noise freq. %d.\n", i );
		psg_write_register( hpsg, 6, i );		//	noise
		psg_write_register( hpsg, 8, 15 );		//	use envelope
		sleep( 1 );
	}
	printf( "Stop.\n" );

	sleep( 3 );
	sangria_sound_terminate();
	return 0;
}
