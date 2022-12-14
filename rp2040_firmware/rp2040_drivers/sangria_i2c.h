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

#ifndef __SANGRIA_I2C_H__
#define __SANGRIA_I2C_H__

#include <cstdint>
#include "hardware/i2c.h"
#include "sangria_firmware_config.h"

class CSANGRIA_I2C {
private:
	i2c_inst_t* p_i2c;

public:
	// --------------------------------------------------------------------
	//	Constructor
	CSANGRIA_I2C( i2c_inst_t* i2c, uint32_t rate, uint32_t scl, uint32_t sda );

	// --------------------------------------------------------------------
	//	write
	//	input:
	//		address ...... device address
	//		p_buffer ..... send data address
	//		count ........ size of p_buffer
	//	output:
	//		none
	//	comment:
	//
	int write( int address, const uint8_t *p_buffer, int count );

	// --------------------------------------------------------------------
	//	read
	//	input:
	//		address ...... device address
	//		p_buffer ..... send data address
	//		count ........ size of p_buffer
	//	output:
	//		none
	//	comment:
	//
	int read( int address, uint8_t *p_buffer, int count );
};

#endif
