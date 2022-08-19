// --------------------------------------------------------------------
//	The MIT License (MIT)
//	
//	Sangria firmware Jogdial
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
#include "sangria_keyboard.h"

// --------------------------------------------------------------------
CSANGRIA_KEYBOARD::CSANGRIA_KEYBOARD() {
	gpio_init( SANGRIA_COL1 );
	gpio_init( SANGRIA_COL2 );
	gpio_init( SANGRIA_COL3 );
	gpio_init( SANGRIA_COL4 );
	gpio_init( SANGRIA_COL5 );
	gpio_init( SANGRIA_ROW1 );
	gpio_init( SANGRIA_ROW2 );
	gpio_init( SANGRIA_ROW3 );
	gpio_init( SANGRIA_ROW4 );
	gpio_init( SANGRIA_ROW5 );
	gpio_init( SANGRIA_ROW6 );
	gpio_init( SANGRIA_ROW7 );
	gpio_init( SANGRIA_BACK_LIGHT );

	gpio_set_dir( SANGRIA_COL1, GPIO_IN );
	gpio_set_dir( SANGRIA_COL2, GPIO_IN );
	gpio_set_dir( SANGRIA_COL3, GPIO_IN );
	gpio_set_dir( SANGRIA_COL4, GPIO_IN );
	gpio_set_dir( SANGRIA_COL5, GPIO_IN );

	gpio_pull_up( SANGRIA_COL1 );
	gpio_pull_up( SANGRIA_COL2 );
	gpio_pull_up( SANGRIA_COL3 );
	gpio_pull_up( SANGRIA_COL4 );
	gpio_pull_up( SANGRIA_COL5 );

	gpio_set_dir( SANGRIA_ROW1, GPIO_IN );
	gpio_set_dir( SANGRIA_ROW2, GPIO_IN );
	gpio_set_dir( SANGRIA_ROW3, GPIO_IN );
	gpio_set_dir( SANGRIA_ROW4, GPIO_IN );
	gpio_set_dir( SANGRIA_ROW5, GPIO_IN );
	gpio_set_dir( SANGRIA_ROW6, GPIO_IN );
	gpio_set_dir( SANGRIA_ROW7, GPIO_IN );

	gpio_pull_up( SANGRIA_ROW1 );
	gpio_pull_up( SANGRIA_ROW2 );
	gpio_pull_up( SANGRIA_ROW3 );
	gpio_pull_up( SANGRIA_ROW4 );
	gpio_pull_up( SANGRIA_ROW5 );
	gpio_pull_up( SANGRIA_ROW6 );
	gpio_pull_up( SANGRIA_ROW7 );

	gpio_set_dir( SANGRIA_BACK_LIGHT, GPIO_OUT );
	gpio_put( SANGRIA_BACK_LIGHT, 0 );
}

// --------------------------------------------------------------------
void CSANGRIA_KEYBOARD::update( void ) {
	int i, port;
	uint32_t key_data;
	uint8_t current_key_matrix[5];

	//printf( "\033[0;0H--------\n" );
	for( i = 0; i < 5; i++ ) {
		port = i + SANGRIA_COL1;
		gpio_set_dir( port, GPIO_OUT );
		gpio_put( port, 0 );
		sleep_us( 10 );
		key_data = gpio_get_all();
		gpio_set_dir( port, GPIO_IN );
		key_data = (key_data >> SANGRIA_ROW1) & 0x7F;	// 7bit
		current_key_matrix[i] = (uint8_t) key_data;

		//for( int j = 0; j < 7; j++ ) {
		//	printf( "%d", (int)(((key_data << j) & 0x40) != 0) );
		//}
		//printf( "\n" );
	}
}

// --------------------------------------------------------------------
void CSANGRIA_KEYBOARD::backlight( bool is_on ) {
	gpio_put( SANGRIA_BACK_LIGHT, is_on );
}
