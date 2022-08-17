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

#include "sangria_jogdial.h"
#include "hardware/gpio.h"

// --------------------------------------------------------------------
CSANGRIA_JOGDIAL::CSANGRIA_JOGDIAL() {
	gpio_init( SANGRIA_BACK );
	gpio_init( SANGRIA_JOG_A );
	gpio_init( SANGRIA_JOG_B );
	gpio_init( SANGRIA_JOG_PUSH );

	gpio_set_dir( SANGRIA_BACK, GPIO_IN );
	gpio_set_dir( SANGRIA_JOG_A, GPIO_IN );
	gpio_set_dir( SANGRIA_JOG_B, GPIO_IN );
	gpio_set_dir( SANGRIA_JOG_PUSH, GPIO_IN );

	gpio_pull_down( SANGRIA_BACK );
	gpio_pull_up( SANGRIA_JOG_A );
	gpio_pull_up( SANGRIA_JOG_B );
	gpio_pull_up( SANGRIA_JOG_PUSH );

	this->current_key_code	= (1 << SANGRIA_JOG_A) | (1 << SANGRIA_JOG_B) | (1 << SANGRIA_JOG_PUSH);
	this->last_key_code		= current_key_code;
}

// --------------------------------------------------------------------
void CSANGRIA_JOGDIAL::update( void ) {

	this->last_key_code		= this->current_key_code;
	this->current_key_code	= gpio_get_all() & this->key_code_mask;
}

// --------------------------------------------------------------------
bool CSANGRIA_JOGDIAL::get_back_button( void ) {

	return( (this->current_key_code & (1 << SANGRIA_BACK)) != 0 );
}

// --------------------------------------------------------------------
bool CSANGRIA_JOGDIAL::get_enter_button( void ) {

	return( (this->current_key_code & (1 << SANGRIA_JOG_PUSH)) == 0 );
}

// --------------------------------------------------------------------
uint32_t CSANGRIA_JOGDIAL::_get_jog_update_state( void ) {

	if( ((this->last_key_code >> SANGRIA_JOG_A) & 1) == 1 && ((this->current_key_code >> SANGRIA_JOG_A) & 1) == 0 ) {
		if( ((this->current_key_code >> SANGRIA_JOG_B) & 1) == 0 ) {
			return 2;
		}
		else {
			return 1;
		}
	}
	return 0;
}

// --------------------------------------------------------------------
bool CSANGRIA_JOGDIAL::get_up_button( void ) {

	return( this->_get_jog_update_state() == 1 );
}

// --------------------------------------------------------------------
bool CSANGRIA_JOGDIAL::get_down_button( void ) {

	return( this->_get_jog_update_state() == 2 );
}
