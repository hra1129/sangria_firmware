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

#ifndef __SANGRIA_JOGDIAL_H__
#define __SANGRIA_JOGDIAL_H__

#include <cstdint>
#include "sangria_firmware_config.h"

class CSANGRIA_JOGDIAL {
private:
	uint32_t		last_key_code;
	uint32_t		current_key_code;
	const uint32_t	key_code_mask = (1 << SANGRIA_BACK) | (1 << SANGRIA_JOG_A) | (1 << SANGRIA_JOG_B) | (1 << SANGRIA_JOG_PUSH);

	int last_jog;
	int current_jog;

public:
	// --------------------------------------------------------------------
	//	Constructor
	CSANGRIA_JOGDIAL();

	// --------------------------------------------------------------------
	//	Update key state
	void update( void );

	// --------------------------------------------------------------------
	//	Get key state : true = pressed, false = unpressed
	bool get_back_button( void );
	bool get_enter_button( void );
	bool get_up_button( void );
	bool get_down_button( void );
};

#endif
