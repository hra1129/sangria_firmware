// --------------------------------------------------------------------
//	The MIT License (MIT)
//	
//	Sangria firmware I2C connection
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
#include <cstdlib>
#include <cstring>

#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/i2c.h"
#include "pico/binary_info.h"

#include "sangria_i2c.h"

// --------------------------------------------------------------------
CSANGRIA_I2C::CSANGRIA_I2C( i2c_inst_t *i2c, uint32_t rate, uint32_t scl, uint32_t sda ) {

	p_i2c = i2c;
	i2c_init( this->p_i2c, rate );
	gpio_set_function( sda, GPIO_FUNC_I2C );
	gpio_set_function( scl, GPIO_FUNC_I2C );
	gpio_pull_up( sda );
	gpio_pull_up( scl );
	bi_decl( bi_2pins_with_func( sda, scl, GPIO_FUNC_I2C ) );
}

// --------------------------------------------------------------------
int CSANGRIA_I2C::write( int address, const uint8_t *p_buffer, int count ) {

	return i2c_write_blocking( this->p_i2c, address, p_buffer, count, false );
}

// --------------------------------------------------------------------
int CSANGRIA_I2C::read( int address, uint8_t *p_buffer, int count ) {

	return i2c_read_blocking( this->p_i2c, address, p_buffer, count, false );
}
