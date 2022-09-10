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
#define BQ_ADDR					0x6B

//	BQ24296M/BQ24297 Registers
#define BQ_INPUT_SOURCE			0x00	// Read/Write
#define BQ_POWER_ON_CONFIG		0x01	// Read/Write
#define BQ_CHARGE_CURRENT		0x02	// Read/Write
#define BQ_PRECHARGE_CURRENT	0x03	// Read/Write
#define BQ_CHARGE_VOLTAGE		0x04	// Read/Write
#define BQ_CHARGE_TERMINATION	0x05	// Read/Write
#define BQ_BOOST_VOLTAGE		0x06	// Read/Write
#define BQ_MISC_OPERATION		0x07	// Read/Write
#define BQ_SYSTEM_STATUS		0x08	// Read only
#define BQ_NEW_FAULT			0x09	// Read only
#define BQ_VENDER_PART			0x0A	// Read only

//	BQ_VENDER_PART
#define ID_BQ24296				0x20
#define ID_BQ24297				0x60

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
bool CSANGRIA_BATTERY::check_battery_management_device( void ) {
	uint8_t result;

	result = this->read_register( BQ_VENDER_PART );
	if( result == ID_BQ24296 || result == ID_BQ24297 ) {
		return true;
	}
	return false;
}

// --------------------------------------------------------------------
int CSANGRIA_BATTERY::get_system_status( void ) {

	return this->read_register( BQ_SYSTEM_STATUS );
}

// --------------------------------------------------------------------
void CSANGRIA_BATTERY::power_on( void ) {

	gpio_put( SANGRIA_BQ_OTG, 1 );		// OTG = L : boost mode  : ON

	this->write_register( BQ_INPUT_SOURCE, 
		(1 << 7) |		//	enable
		(6 << 3) |		//	4.36V
		(7 << 0) );		//	3A

	//	Clear fault status
	this->read_register( BQ_NEW_FAULT );
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
