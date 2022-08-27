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

class CSANGRIA_KEYBOARD {
private:
	uint8_t last_key_matrix[5];
	uint8_t current_key_matrix[5];

	uint8_t last_key_code[6];
	uint8_t key_matrix_table[ 5 * 8 ];
public:
	// --------------------------------------------------------------------
	//	Constructor
	CSANGRIA_KEYBOARD();

	// --------------------------------------------------------------------
	//	Update key state
	int update( uint8_t key_code[] );

	// --------------------------------------------------------------------
	//	backlight control
	void backlight( bool is_on );
};

#endif
