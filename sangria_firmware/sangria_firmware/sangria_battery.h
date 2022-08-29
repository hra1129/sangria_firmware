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

#ifndef __SANGRIA_BATTERY_H__
#define __SANGRIA_BATTERY_H__

#include <cstdint>
#include "sangria_firmware_config.h"
#include "sangria_i2c.h"

// --------------------------------------------------------------------
//	BQ24296M Registers
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

class CSANGRIA_BATTERY {
private:
	CSANGRIA_I2C *p_i2c;

public:
	// --------------------------------------------------------------------
	//	Constructor
	CSANGRIA_BATTERY();

	// --------------------------------------------------------------------
	//	set_i2c
	//	input:
	//		p_i2c ..... Target connection
	//	output:
	//		none
	//	comment:
	//
	void set_i2c( CSANGRIA_I2C *p_i2c );

	// --------------------------------------------------------------------
	//	power on
	//	input:
	//		none
	//	output:
	//		none
	//	comment:
	//
	void power_on( void );

	// --------------------------------------------------------------------
	//	power off
	//	input:
	//		none
	//	output:
	//		none
	//	comment:
	//
	void power_off( void );

	// --------------------------------------------------------------------
	//	write register
	//	input:
	//		address .... register address BQ_XXXX
	//		data ....... write data
	//	output:
	//		none
	//	comment:
	//
	int write_register( uint8_t address, uint8_t data );

	// --------------------------------------------------------------------
	//	read register
	//	input:
	//		address .... register address BQ_XXXX
	//	output:
	//		data ....... read data
	//	comment:
	//
	uint8_t read_register( uint8_t address );
};

#endif
