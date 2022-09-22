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
	//	check battery management device
	//	input:
	//		none
	//	output:
	//		true .... active
	//		false ... inactive
	//	comment:
	//
	bool check_battery_management_device( void );

	// --------------------------------------------------------------------
	//	get system status
	//	input:
	//		none
	//	output:
	//		status code
	//			bit7 VBUS_STAT[1]  : 00-Unkown, 01-USB host, 10-Adapter port, 11-OTG
	//			bit6 VBUS_STAT[0]  : 
	//			bit5 CHRG_STAT[1]  : 00-Not Charging, 01-Pre charge, 10-Fast charging, 11-Charge termination done
	//			bit4 CHRG_STAT[0]  : 
	//			bit3 DPM_STAT      : 0-Not DPM, 1-VINDPM
	//			bit2 PG_STAT       : 0-Not power good, 1-Power good
	//			bit1 THERM_STAT    : 0-Normal, 1-inthermal regulation
	//			bit0 VSYS_STAT     : 0-Not in VSYSMIN regulation, 1-In VSYSMIN regulation
	//	comment:
	//
	int get_system_status( void );

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

	// --------------------------------------------------------------------
	//	get battery level
	//	input:
	//		none
	//	output:
	//		battery level
	//	comment:
	//
	int get_battery_level( void );
};

#endif
