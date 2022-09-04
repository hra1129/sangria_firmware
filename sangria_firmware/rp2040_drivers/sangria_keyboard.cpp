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
#define MODIFYER_SYM	0x100
#define MODIFYER_ALT	0x101
#define MODIFYER_CAPS	0x102
#define MODIFYER_SHIFT	0x103
#define MODIFYER_CTRL	0x104

// --------------------------------------------------------------------
//         ROW1 ROW2 ROW3 ROW4 ROW5 ROW6 ROW7
//    COL1  Q    W   sym   A   alt  SPC  MIC
//    COL2  E    S    D    P    Z    X   SH1
//    COL3  R    G    T   SH2   V    C    F
//    COL4  U    H    Y   RET   B    N    J
//    COL5  O    L    I   BK    $    M    K

static const uint16_t default_key_matrix_table[ 4 ][ 5 * 8 ] = {
	{	// Normal
		// ROW1       ROW2          ROW3          ROW4                  ROW5               ROW6           ROW7                DUMMY
		HID_KEY_Q   , HID_KEY_W   , MODIFYER_SYM, HID_KEY_A           , MODIFYER_ALT     , HID_KEY_SPACE, HID_KEY_NONE      , HID_KEY_NONE, // COL1
		HID_KEY_E   , HID_KEY_S   , HID_KEY_D   , HID_KEY_P           , HID_KEY_X        , HID_KEY_Z    , MODIFYER_SHIFT    , HID_KEY_NONE, // COL2
		HID_KEY_R   , HID_KEY_G   , HID_KEY_T   , MODIFYER_CTRL       , HID_KEY_V        , HID_KEY_C    , HID_KEY_F         , HID_KEY_NONE, // COL3
		HID_KEY_U   , HID_KEY_H   , HID_KEY_Y   , HID_KEY_ENTER       , HID_KEY_B        , HID_KEY_N    , HID_KEY_J         , HID_KEY_NONE, // COL4
		HID_KEY_O   , HID_KEY_L   , HID_KEY_I   , HID_KEY_BACKSPACE   , HID_KEY_NONE     , HID_KEY_M    , HID_KEY_K         , HID_KEY_NONE, // COL5
	},
	{	// Shift
		// ROW1       ROW2          ROW3          ROW4                  ROW5               ROW6           ROW7                DUMMY
		HID_KEY_Q   , HID_KEY_W   , MODIFYER_SYM, HID_KEY_A           , MODIFYER_ALT     , HID_KEY_SPACE, HID_KEY_NONE      , HID_KEY_NONE, // COL1
		HID_KEY_E   , HID_KEY_S   , HID_KEY_D   , HID_KEY_P           , HID_KEY_X        , HID_KEY_Z    , MODIFYER_SHIFT    , HID_KEY_NONE, // COL2
		HID_KEY_R   , HID_KEY_G   , HID_KEY_T   , MODIFYER_CTRL       , HID_KEY_V        , HID_KEY_C    , HID_KEY_F         , HID_KEY_NONE, // COL3
		HID_KEY_U   , HID_KEY_H   , HID_KEY_Y   , HID_KEY_ENTER       , HID_KEY_B        , HID_KEY_N    , HID_KEY_J         , HID_KEY_NONE, // COL4
		HID_KEY_O   , HID_KEY_L   , HID_KEY_I   , HID_KEY_BACKSPACE   , HID_KEY_NONE     , HID_KEY_M    , HID_KEY_K         , HID_KEY_NONE, // COL5
	},
	{	// Sym
		// ROW1       ROW2          ROW3          ROW4                  ROW5               ROW6           ROW7                DUMMY
		HID_KEY_Q   , HID_KEY_1   , MODIFYER_SYM, HID_KEY_A           , MODIFYER_ALT     , HID_KEY_SPACE, HID_KEY_NONE      , HID_KEY_NONE, // COL1
		HID_KEY_2   , HID_KEY_4   , HID_KEY_D   , HID_KEY_P           , HID_KEY_8        , HID_KEY_7    , MODIFYER_SHIFT    , HID_KEY_NONE, // COL2
		HID_KEY_3   , HID_KEY_G   , HID_KEY_T   , MODIFYER_CTRL       , HID_KEY_V        , HID_KEY_C    , HID_KEY_6         , HID_KEY_NONE, // COL3
		HID_KEY_U   , HID_KEY_H   , HID_KEY_Y   , HID_KEY_ENTER       , HID_KEY_B        , HID_KEY_N    , HID_KEY_J         , HID_KEY_NONE, // COL4
		HID_KEY_O   , HID_KEY_L   , HID_KEY_I   , HID_KEY_BACKSPACE   , HID_KEY_NONE     , HID_KEY_M    , HID_KEY_K         , HID_KEY_NONE, // COL5
	},
	{	// Shift and Sym
		// ROW1       ROW2          ROW3          ROW4                  ROW5               ROW6           ROW7                DUMMY
		HID_KEY_Q   , HID_KEY_1   , MODIFYER_SYM, HID_KEY_A           , MODIFYER_ALT     , HID_KEY_SPACE, HID_KEY_NONE      , HID_KEY_NONE, // COL1
		HID_KEY_2   , HID_KEY_4   , HID_KEY_D   , HID_KEY_P           , HID_KEY_8        , HID_KEY_7    , MODIFYER_SHIFT    , HID_KEY_NONE, // COL2
		HID_KEY_3   , HID_KEY_G   , HID_KEY_T   , MODIFYER_CTRL       , HID_KEY_V        , HID_KEY_C    , HID_KEY_6         , HID_KEY_NONE, // COL3
		HID_KEY_U   , HID_KEY_H   , HID_KEY_Y   , HID_KEY_ENTER       , HID_KEY_B        , HID_KEY_N    , HID_KEY_J         , HID_KEY_NONE, // COL4
		HID_KEY_O   , HID_KEY_L   , HID_KEY_I   , HID_KEY_BACKSPACE   , HID_KEY_NONE     , HID_KEY_M    , HID_KEY_K         , HID_KEY_NONE, // COL5
	},
};

#define JOGDIAL_BACK_KEY    HID_KEY_ESCAPE
#define JOGDIAL_ENTER_KEY   HID_KEY_TAB
#define JOGDIAL_UP_KEY      HID_KEY_ARROW_UP
#define JOGDIAL_DOWN_KEY    HID_KEY_ARROW_DOWN
#define JOGDIAL_LEFT_KEY    HID_KEY_ARROW_LEFT
#define JOGDIAL_RIGHT_KEY   HID_KEY_ARROW_RIGHT

// --------------------------------------------------------------------
CSANGRIA_KEYBOARD::CSANGRIA_KEYBOARD() {
	size_t i, j;

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
	for( j = 0; j < 4; j++ ) {
		for( i = 0; i < 5 * 8; i++ ) {
			this->key_matrix_table[j][i] = default_key_matrix_table[j][i];
		}
	}
	this->p_jogdial = nullptr;
	this->alt_key = false;
	this->shift_key = false;
	this->sym_key = false;
	this->ctrl_key = false;
}

// --------------------------------------------------------------------
void CSANGRIA_KEYBOARD::set_jogdial( CSANGRIA_JOGDIAL *p_jogdial ) {

	this->p_jogdial = p_jogdial;
}

// --------------------------------------------------------------------
void CSANGRIA_KEYBOARD::_check_modifier_key( uint8_t key_code[], int &index, uint8_t hid_key_code, bool &current_key, uint8_t last_key_press, uint8_t current_key_press, int bit_num ) {

	if( ((last_key_press & (1 << bit_num)) != 0) && ((current_key_press & (1 << bit_num)) == 0) ) {
		//	押されたタイミング
		current_key = !current_key;
	}
	if( hid_key_code != HID_KEY_NONE && current_key ) {
		key_code[ index++ ] = hid_key_code;
	}
}

// --------------------------------------------------------------------
int CSANGRIA_KEYBOARD::update( uint8_t key_code[] ) {
	int i, j, index, port, modifier_index;
	uint32_t key_data;
	uint16_t hid_key_code;

	modifier_index = (this->shift_key ? 1 : 0) + (this->sym_key ? 2 : 0);
	index = 0;

	if( p_jogdial != nullptr ) {
		p_jogdial->update();

		if( p_jogdial->get_back_button() ) {
			key_code[ index ] = JOGDIAL_BACK_KEY;
			index++;
		}
		if( p_jogdial->get_enter_button() ) {
			key_code[ index ] = JOGDIAL_ENTER_KEY;
			index++;
		}
		if( p_jogdial->get_up_button() ) {
			if( (modifier_index & 2) == 0 ) {
				key_code[ index ] = JOGDIAL_UP_KEY;
			}
			else {
				key_code[ index ] = JOGDIAL_LEFT_KEY;
			}
			index++;
		}
		if( p_jogdial->get_down_button() ) {
			if( (modifier_index & 2) == 0 ) {
				key_code[ index ] = JOGDIAL_DOWN_KEY;
			}
			else {
				key_code[ index ] = JOGDIAL_RIGHT_KEY;
			}
			index++;
		}
	}

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
			hid_key_code = key_matrix_table[ modifier_index ][ (i << 3) + j ];
			if( (hid_key_code & 0x100) != 0 ) {
				//	Modifier
				switch( hid_key_code ) {
				default:
				case MODIFYER_SYM:
					this->_check_modifier_key( key_code, index, HID_KEY_NONE, this->sym_key, this->last_key_matrix[i], key_data, j );
					break;
				case MODIFYER_ALT:
					this->_check_modifier_key( key_code, index, HID_KEY_ALT_LEFT, this->alt_key, this->last_key_matrix[i], key_data, j );
					break;
				case MODIFYER_CAPS:
					this->_check_modifier_key( key_code, index, HID_KEY_CAPS_LOCK, this->caps_key, this->last_key_matrix[i], key_data, j );
					break;
				case MODIFYER_SHIFT:
					this->_check_modifier_key( key_code, index, HID_KEY_SHIFT_LEFT, this->shift_key, this->last_key_matrix[i], key_data, j );
					break;
				case MODIFYER_CTRL:
					this->_check_modifier_key( key_code, index, HID_KEY_CONTROL_LEFT, this->ctrl_key, this->last_key_matrix[i], key_data, j );
					break;
				}
			}
			else {
				if( (key_data & (1 << j)) == 0 ) {
					key_code[ index ] = hid_key_code;
					index++;
				}
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
