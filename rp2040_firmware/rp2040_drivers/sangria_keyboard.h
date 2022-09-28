// --------------------------------------------------------------------
//	The MIT License (MIT)
//	
//	Sangria firmware keyboard
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

#ifndef __SANGRIA_KEYBOARD_H__
#define __SANGRIA_KEYBOARD_H__

#include <cstdint>
#include "sangria_firmware_config.h"
#include "sangria_jogdial.h"

class CSANGRIA_KEYBOARD {
private:
	CSANGRIA_JOGDIAL *p_jogdial;
	
	uint8_t last_key_matrix[5];
	uint8_t current_key_matrix[5];

	uint8_t last_key_code[6];
	uint16_t key_matrix_table[4][ 5 * 8 ];

	void _check_toggle_modifier( bool &current_key, uint8_t last_key_press, uint8_t current_key_press, int bit_num );

	bool alt_key;
	bool shift_key;
	bool caps_key;
	bool sym_key;
	bool ctrl_key;

	bool menu_mode;
public:
	// --------------------------------------------------------------------
	//	Constructor
	CSANGRIA_KEYBOARD();

	// --------------------------------------------------------------------
	//	Function to return true in menu mode
	bool is_menu_mode( void ) const {
		return this->menu_mode;
	}

	// --------------------------------------------------------------------
	//	Exit function from menu mode
	void exit_menu_mode( void ) {
		this->menu_mode = false;
	}

	// --------------------------------------------------------------------
	//	Set jogdial
	void set_jogdial( CSANGRIA_JOGDIAL *p_jogdial );

	// --------------------------------------------------------------------
	//	Update key state
	int update( uint8_t key_code[] );

	// --------------------------------------------------------------------
	//	modifier
	bool get_alt_key( void ) const {
		return this->alt_key;
	}

	bool get_shift_key( void ) const {
		return this->shift_key;
	}

	bool get_caps_key( void ) const {
		return this->caps_key;
	}

	bool get_sym_key( void ) const {
		return this->sym_key;
	}

	bool get_ctrl_key( void ) const {
		return this->ctrl_key;
	}

	bool check_host_connected( void );

	// --------------------------------------------------------------------
	//	backlight control
	void backlight( bool is_on );
};

#endif
