// --------------------------------------------------------------------
//	The MIT License (MIT)
//	
//	Sangria firmware
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

#include "controller.h"

// --------------------------------------------------------------------
CSANGRIA_CONTROLLER::CSANGRIA_CONTROLLER() {
}

// --------------------------------------------------------------------
void CSANGRIA_CONTROLLER::initialize( void ) {

	this->p_jogdial		= new CSANGRIA_JOGDIAL();
	this->p_keyboard	= new CSANGRIA_KEYBOARD();
	this->p_i2c_oled	= new CSANGRIA_I2C( SANGRIA_OLED_I2C, SANGRIA_I2C1_CLOCK, SANGRIA_I2C1_SCL, SANGRIA_I2C1_SDA );
	this->p_i2c_bq		= new CSANGRIA_I2C( SANGRIA_BQ_I2C, SANGRIA_I2C0_CLOCK, SANGRIA_I2C0_SCL, SANGRIA_I2C0_SDA );
	this->p_oled		= new CSANGRIA_OLED();
	this->p_battery		= new CSANGRIA_BATTERY();
	this->p_gps			= new CSANGRIA_GPS();
	this->p_flash		= new CSANGRIA_FLASH();

	this->p_oled->set_i2c( p_i2c_oled );
	this->p_battery->set_i2c( p_i2c_bq );
	this->p_keyboard->set_jogdial( p_jogdial );
}
