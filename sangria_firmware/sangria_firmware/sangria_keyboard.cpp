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
#include "tusb.h"

// --------------------------------------------------------------------
static const uint8_t default_key_matrix_table[] = {
	// ROW1       ROW2          ROW3          ROW4                 ROW5               ROW6           ROW7                DUMMY
	HID_KEY_Q   , HID_KEY_W   , HID_KEY_NONE, HID_KEY_A          , HID_KEY_ALT_LEFT , HID_KEY_SPACE, HID_KEY_NONE      , HID_KEY_NONE, // COL1
	HID_KEY_E   , HID_KEY_S   , HID_KEY_D   , HID_KEY_P          , HID_KEY_X        , HID_KEY_Z    , HID_KEY_SHIFT_LEFT, HID_KEY_NONE, // COL2
	HID_KEY_R   , HID_KEY_G   , HID_KEY_T   , HID_KEY_SHIFT_RIGHT, HID_KEY_V        , HID_KEY_C    , HID_KEY_F         , HID_KEY_NONE, // COL3
	HID_KEY_U   , HID_KEY_H   , HID_KEY_Y   , HID_KEY_RETURN     , HID_KEY_B        , HID_KEY_N    , HID_KEY_J         , HID_KEY_NONE, // COL4
	HID_KEY_O   , HID_KEY_L   , HID_KEY_I   , HID_KEY_BACKSPACE  , HID_KEY_NONE     , HID_KEY_M    , HID_KEY_K         , HID_KEY_NONE, // COL5
};

// --------------------------------------------------------------------
CSANGRIA_KEYBOARD::CSANGRIA_KEYBOARD() {
	size_t i;

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

	for( i = 0; i < sizeof(this->last_key_matrix); i++ ) {
		this->last_key_matrix[i] = 0x7F;
		this->current_key_matrix[i] = 0x7F;
	}
	for( i = 0; i < sizeof(this->last_key_code); i++ ) {
		this->last_key_code[i] = HID_KEY_NONE;
	}
	for( i = 0; i < sizeof(this->key_matrix_table); i++ ) {
		this->key_matrix_table[i] = default_key_matrix_table[i];
	}
}

// --------------------------------------------------------------------
int CSANGRIA_KEYBOARD::update( uint8_t key_code[] ) {
	int i, j, index, port;
	uint32_t key_data;

	index = 0;
	for( i = 0; i < 5; i++ ) {
		port = i + SANGRIA_COL1;
		gpio_set_dir( port, GPIO_OUT );
		gpio_put( port, 0 );
		sleep_us( 10 );
		key_data = gpio_get_all();
		gpio_set_dir( port, GPIO_IN );
		key_data = (key_data >> SANGRIA_ROW1) & 0x7F;	// 7bit
		this->last_key_matrix[i] = this->current_key_matrix[i];
		this->current_key_matrix[i] = (uint8_t) key_data;

		for( j = 0; j < 7 && index < 6; j++ ) {
			if( (key_data & (1 << j)) == 0 ) {
				key_code[ index ] = key_matrix_table[ (i << 3) + j ];
				index++;
			}
		}
	}
	for( i = index; i < 6; i++ ) {
		key_code[ i ] = HID_KEY_NONE;
	}
	return index;
}

// --------------------------------------------------------------------
void CSANGRIA_KEYBOARD::backlight( bool is_on ) {
	gpio_put( SANGRIA_BACK_LIGHT, is_on );
}
