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
#define VHID_SYM_KEY		0x100
#define VHID_ALT_KEY		0x101
#define VHID_CAPS_KEY		0x102
#define VHID_SHIFT_KEY		0x103
#define VHID_CTRL_KEY		0x104

#define MODIFIER_SHIFT_BIT	0x1000
#define MODIFIER_ALT_BIT	0x2000
#define MODIFIER_BIT_MASK	0xF000

#define _S( a )				((a) | MODIFIER_SHIFT_BIT)		//	with SHIFT
#define _A( a )				((a) | MODIFIER_ALT_BIT)		//	with ALT

bool tud_check_host_connected( void );

// --------------------------------------------------------------------
//         ROW1 ROW2 ROW3 ROW4 ROW5 ROW6 ROW7
//    COL1  Q    W   sym   A   alt  SPC  MIC
//    COL2  E    S    D    P    Z    X   SH1
//    COL3  R    G    T   SH2   V    C    F
//    COL4  U    H    Y   RET   B    N    J
//    COL5  O    L    I   BK    $    M    K

static const uint16_t default_key_matrix_table[ 4 ][ 5 * 8 ] = {
	{	// Normal
		// ROW1                ROW2                    ROW3           ROW4                       ROW5               ROW6                ROW7                DUMMY
		HID_KEY_Q            , HID_KEY_W             , VHID_SYM_KEY , HID_KEY_A                , VHID_ALT_KEY     , HID_KEY_SPACE     , HID_KEY_TAB       , 0, // COL1
		HID_KEY_E            , HID_KEY_S             , HID_KEY_D    , HID_KEY_P                , HID_KEY_X        , HID_KEY_Z         , VHID_SHIFT_KEY    , 0, // COL2
		HID_KEY_R            , HID_KEY_G             , HID_KEY_T    , VHID_CTRL_KEY            , HID_KEY_V        , HID_KEY_C         , HID_KEY_F         , 0, // COL3
		HID_KEY_U            , HID_KEY_H             , HID_KEY_Y    , HID_KEY_ENTER            , HID_KEY_B        , HID_KEY_N         , HID_KEY_J         , 0, // COL4
		HID_KEY_O            , HID_KEY_L             , HID_KEY_I    , HID_KEY_BACKSPACE        , _S(HID_KEY_4)    , HID_KEY_M         , HID_KEY_K         , 0, // COL5
	},
	{	// Alt
		// ROW1                ROW2                    ROW3           ROW4                       ROW5               ROW6                ROW7                DUMMY
		_A(HID_KEY_Q)        , _A(HID_KEY_W)         , VHID_SYM_KEY , _A(HID_KEY_A)            , VHID_ALT_KEY     , HID_KEY_SPACE     , HID_KEY_BACKSLASH , 0, // COL1
		_A(HID_KEY_E)        , _A(HID_KEY_S)         , _A(HID_KEY_D), _A(HID_KEY_P)            , _A(HID_KEY_X)    , _A(HID_KEY_Z)     , VHID_SHIFT_KEY    , 0, // COL2
		_A(HID_KEY_R)        , _A(HID_KEY_G)         , _A(HID_KEY_T), VHID_CTRL_KEY            , _A(HID_KEY_V)    , _A(HID_KEY_C)     , _A(HID_KEY_F)     , 0, // COL3
		_A(HID_KEY_U)        , _A(HID_KEY_H)         , _A(HID_KEY_Y), HID_KEY_BRACKET_RIGHT    , _A(HID_KEY_B)    , _A(HID_KEY_N)     , _A(HID_KEY_J)     , 0, // COL4
		_A(HID_KEY_O)        , _A(HID_KEY_L)         , _A(HID_KEY_I), HID_KEY_BRACKET_LEFT     , HID_KEY_GRAVE    , _A(HID_KEY_M)     , _A(HID_KEY_K)     , 0, // COL5
	},
	{	// Sym
		// ROW1                ROW2                    ROW3           ROW4                       ROW5               ROW6                ROW7                DUMMY
		_S(HID_KEY_3)        , HID_KEY_1             , VHID_SYM_KEY , _S(HID_KEY_8)            , VHID_ALT_KEY     , HID_KEY_SPACE     , HID_KEY_0         , 0, // COL1
		HID_KEY_2            , HID_KEY_4             , HID_KEY_5    , _S(HID_KEY_2)            , HID_KEY_8        , HID_KEY_7         , VHID_SHIFT_KEY    , 0, // COL2
		HID_KEY_3            , HID_KEY_SLASH         , _S(HID_KEY_9), VHID_CTRL_KEY            , _S(HID_KEY_SLASH), HID_KEY_9         , HID_KEY_6         , 0, // COL3
		HID_KEY_EQUAL        , _S(HID_KEY_SEMICOLON) , _S(HID_KEY_0), HID_KEY_ENTER            , _S(HID_KEY_1)    , HID_KEY_COMMA     , HID_KEY_SEMICOLON , 0, // COL4
		_S(HID_KEY_EQUAL)    , _S(HID_KEY_APOSTROPHE), HID_KEY_MINUS, HID_KEY_BACKSPACE        , HID_KEY_EQUAL    , HID_KEY_PERIOD    , HID_KEY_APOSTROPHE, 0, // COL5
	},
	{	// Alt and Sym
		// ROW1                ROW2                    ROW3           ROW4                       ROW5               ROW6                ROW7                DUMMY
		_S(HID_KEY_3)        , HID_KEY_F1            , VHID_SYM_KEY , _S(HID_KEY_8)            , VHID_ALT_KEY     , HID_KEY_SPACE     , HID_KEY_F10       , 0, // COL1
		HID_KEY_F2           , HID_KEY_F4            , HID_KEY_F5   , _S(HID_KEY_2)            , HID_KEY_F8       , HID_KEY_F7        , VHID_SHIFT_KEY    , 0, // COL2
		HID_KEY_F3           , HID_KEY_SLASH         , _S(HID_KEY_9), VHID_CTRL_KEY            , _S(HID_KEY_SLASH), HID_KEY_F9        , HID_KEY_F6        , 0, // COL3
		HID_KEY_GRAVE        , _S(HID_KEY_SEMICOLON) , _S(HID_KEY_0), _S(HID_KEY_BRACKET_RIGHT), _S(HID_KEY_1)    , _S(HID_KEY_COMMA) , HID_KEY_SEMICOLON , 0, // COL4
		_S(HID_KEY_BACKSLASH), _S(HID_KEY_APOSTROPHE), _S(HID_KEY_7), _S(HID_KEY_BRACKET_LEFT) , HID_KEY_EQUAL    , _S(HID_KEY_PERIOD), HID_KEY_APOSTROPHE, 0, // COL5
	},
};

#define JOGDIAL_BACK_KEY    HID_KEY_ESCAPE
#define JOGDIAL_ENTER_KEY   HID_KEY_TAB
#define JOGDIAL_UP_KEY      HID_KEY_ARROW_UP
#define JOGDIAL_DOWN_KEY    HID_KEY_ARROW_DOWN
#define JOGDIAL_LEFT_KEY    HID_KEY_ARROW_LEFT
#define JOGDIAL_RIGHT_KEY   HID_KEY_ARROW_RIGHT

// --------------------------------------------------------------------
#define MODIFIER_ALT_KEY	(1 << 0)
#define MODIFIER_SYM_KEY	(1 << 1)

// --------------------------------------------------------------------
const uint16_t (*CSANGRIA_KEYBOARD::get_default_keymap( void ))[40] {

	return default_key_matrix_table;
}

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
	this->menu_mode = false;
}

// --------------------------------------------------------------------
void CSANGRIA_KEYBOARD::set_jogdial( CSANGRIA_JOGDIAL *p_jogdial ) {

	this->p_jogdial = p_jogdial;
}

// --------------------------------------------------------------------
void CSANGRIA_KEYBOARD::_check_toggle_modifier( bool &current_key, uint8_t last_key_press, uint8_t current_key_press, int bit_num ) {

	if( ((last_key_press & (1 << bit_num)) != 0) && ((current_key_press & (1 << bit_num)) == 0) ) {
		//	Timing of the push
		current_key = !current_key;
	}
}

// --------------------------------------------------------------------
int CSANGRIA_KEYBOARD::update( uint8_t key_code[] ) {
	int i, j, index, port, modifier_index, virtual_modifier_index;
	uint32_t key_data;
	uint16_t hid_key_code;

	modifier_index = (this->alt_key ? MODIFIER_ALT_KEY : 0) + (this->sym_key ? MODIFIER_SYM_KEY : 0);
	index = 0;
	virtual_modifier_index = -1;	//	invalid

	//	Update key press informations
	for( i = 0; i < 5; i++ ) {
		port = i + SANGRIA_COL1;
		gpio_set_dir( port, GPIO_OUT );
		gpio_put( port, 0 );
		sleep_us( 10 );
		key_data = (gpio_get_all() >> SANGRIA_ROW1) & 0x7F;	// 7bit
		gpio_set_dir( port, GPIO_IN );
		this->last_key_matrix[i] = this->current_key_matrix[i];
		this->current_key_matrix[i] = (uint8_t) key_data;
	}

	for( i = 0; i < 5; i++ ) {
		key_data = this->current_key_matrix[i];

		for( j = 0; j < 7 && index < 6; j++ ) {
			hid_key_code = key_matrix_table[ modifier_index ][ (i << 3) + j ];
			if( (hid_key_code & 0x100) != 0 ) {
				//	Modifier
				switch( hid_key_code ) {
				default:
				case VHID_SYM_KEY:
					this->sym_key = ( (key_data & (1 << j)) == 0 );
					break;
				case VHID_ALT_KEY:
					this->alt_key = ( (key_data & (1 << j)) == 0 );
					break;
				case VHID_CAPS_KEY:
					this->_check_toggle_modifier( this->caps_key, this->last_key_matrix[i], key_data, j );
					if( this->ctrl_key ) {
						key_code[ index ] = HID_KEY_CAPS_LOCK;
						index++;
					}
					break;
				case VHID_SHIFT_KEY:
					this->_check_toggle_modifier( this->shift_key, this->last_key_matrix[i], key_data, j );
					break;
				case VHID_CTRL_KEY:
					this->ctrl_key = ( (key_data & (1 << j)) == 0 );
					if( this->ctrl_key ) {
						key_code[ index ] = HID_KEY_CONTROL_LEFT;
						index++;
					}
					break;
				}
			}
			else {
				if( (key_data & (1 << j)) == 0 ) {
					if( virtual_modifier_index == -1 ) {
						//	始めて見つけたキーの仮想モディファイアを採用する
						virtual_modifier_index = 0;
						if( (hid_key_code & MODIFIER_SHIFT_BIT) != 0 ) {
							virtual_modifier_index = _S( virtual_modifier_index );
							if( !this->shift_key ) {
								key_code[ index ] = HID_KEY_SHIFT_LEFT;
								index++;
							}
						}
						else {
							if( this->shift_key ) {
								key_code[ index ] = HID_KEY_SHIFT_LEFT;
								index++;
							}
						}
						if( (hid_key_code & MODIFIER_ALT_BIT) != 0 ) {
							virtual_modifier_index = _A( virtual_modifier_index );
							key_code[ index ] = HID_KEY_ALT_LEFT;
							index++;
						}
						key_code[ index ] = hid_key_code & 255;
						index++;
					}
					else if( (hid_key_code & MODIFIER_BIT_MASK) == (virtual_modifier_index & MODIFIER_BIT_MASK) ) {
						//	初めて見つけたキーのモディファイアと、モディファイアが同じ場合にのみ採用する
						key_code[ index ] = hid_key_code & 255;
						index++;
					}
				}
			}
		}
	}

	if( this->menu_mode ) {
		//	In menu mode, it returns "no keys pressed" as a USB keyboard.
		index = 0;
	}
	else if( p_jogdial != nullptr ) {
		//	Update jogdial press informations and send datas
		p_jogdial->update();

		if( p_jogdial->get_back_button() ) {
			if( (this->current_key_matrix[1] & (1 << 6)) == 0 && (this->current_key_matrix[2] & (1 << 3)) == 0 ) {
				//	If the combination of [Left-Shift]+[Right-Shift]+[Jog BACK] is pressed, the menu mode is entered.
				this->menu_mode = true;
				for( i = 0; i < 6; i++ ) {
					key_code[ i ] = HID_KEY_NONE;
				}
				return 0;
			}
			key_code[ index ] = JOGDIAL_BACK_KEY;
			index++;
		}
		else if( p_jogdial->get_enter_button() ) {
			key_code[ index ] = JOGDIAL_ENTER_KEY;
			index++;
		}
		else if( p_jogdial->get_up_button() ) {
			if( this->sym_key ) {
				key_code[ index ] = JOGDIAL_RIGHT_KEY;
			}
			else {
				key_code[ index ] = JOGDIAL_UP_KEY;
			}
			index++;
		}
		else if( p_jogdial->get_down_button() ) {
			if( this->sym_key ) {
				key_code[ index ] = JOGDIAL_LEFT_KEY;
			}
			else {
				key_code[ index ] = JOGDIAL_DOWN_KEY;
			}
			index++;
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

// --------------------------------------------------------------------
bool CSANGRIA_KEYBOARD::check_host_connected( void ) {
	return tud_check_host_connected();
}
