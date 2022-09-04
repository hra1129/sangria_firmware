// --------------------------------------------------------------------
//	The MIT License (MIT)
//	
//	Sangria firmware Battery controller
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

#include "sangria_battery.h"

// I2C address
#define BQ_ADDR											0x6B

// --------------------------------------------------------------------
CSANGRIA_BATTERY::CSANGRIA_BATTERY() {

	this->p_i2c = nullptr;
	gpio_init( SANGRIA_BQ_INT );
	gpio_init( SANGRIA_BQ_OTG );
	gpio_init( SANGRIA_BQ_CE_N );

	gpio_set_dir( SANGRIA_BQ_INT, GPIO_IN );
	gpio_set_dir( SANGRIA_BQ_OTG, GPIO_OUT );
	gpio_set_dir( SANGRIA_BQ_CE_N, GPIO_OUT );

	gpio_pull_up( SANGRIA_BQ_INT );

	gpio_put( SANGRIA_BQ_OTG, 0 );		// OTG = L : boost mode  : OFF
	gpio_put( SANGRIA_BQ_CE_N, 0 );		// /CE = L : chip enable : ON
}
// --------------------------------------------------------------------
void CSANGRIA_BATTERY::set_i2c( CSANGRIA_I2C *p_i2c ) {

	this->p_i2c = p_i2c;
}

// --------------------------------------------------------------------
void CSANGRIA_BATTERY::power_on( void ) {

	gpio_put( SANGRIA_BQ_OTG, 1 );		// OTG = L : boost mode  : ON

	this->write_register( BQ_INPUT_SOURCE, 
		(1 << 7) |		//	enable
		(6 << 3) |		//	4.36V
		(7 << 0) );		//	3A
}

// --------------------------------------------------------------------
void CSANGRIA_BATTERY::power_off( void ) {

	gpio_put( SANGRIA_BQ_OTG, 0 );		// OTG = L : boost mode  : OFF
}

// --------------------------------------------------------------------
int CSANGRIA_BATTERY::write_register( uint8_t address, uint8_t data ) {
	uint8_t buffer[2];

	buffer[0] = address;
	buffer[1] = data;
	return this->p_i2c->write( BQ_ADDR, buffer, 2 );
}

// --------------------------------------------------------------------
uint8_t CSANGRIA_BATTERY::read_register( uint8_t address ) {
	uint8_t buffer;

	buffer = address;
	this->p_i2c->write( BQ_ADDR, &buffer, 1 );
	buffer = 0xFF;
	this->p_i2c->read( BQ_ADDR, &buffer, 1 );
	return buffer;
}
