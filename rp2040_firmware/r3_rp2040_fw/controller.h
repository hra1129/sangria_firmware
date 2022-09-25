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

#ifndef __SANGRIA_CONTROLLER_H___
#define __SANGRIA_CONTROLLER_H___

#include "sangria_firmware_config.h"
#include "sangria_jogdial.h"
#include "sangria_keyboard.h"
#include "sangria_oled.h"
#include "sangria_usb_keyboard.h"
#include "sangria_battery.h"
#include "sangria_gps.h"
#include "sangria_flash.h"
#include "sangria_graphic_resource.h"

class CSANGRIA_CONTROLLER {
private:
	CSANGRIA_I2C *p_i2c_oled = nullptr;
	CSANGRIA_I2C *p_i2c_bq = nullptr;

	CSANGRIA_KEYBOARD *p_keyboard = nullptr;
	CSANGRIA_JOGDIAL *p_jogdial = nullptr;
	CSANGRIA_OLED *p_oled = nullptr;
	CSANGRIA_BATTERY *p_battery = nullptr;
	CSANGRIA_GPS *p_gps = nullptr;
	CSANGRIA_FLASH *p_flash = nullptr;

public:
	CSANGRIA_CONTROLLER();
	void initialize( void );

	CSANGRIA_KEYBOARD *get_keyboard( void ) {
		return this->p_keyboard;
	}

	CSANGRIA_JOGDIAL *get_jogdial( void ) {
		return this->p_jogdial;
	}

	CSANGRIA_OLED *get_oled( void ) {
		return this->p_oled;
	}

	CSANGRIA_BATTERY *get_battery( void ) {
		return this->p_battery;
	}

	CSANGRIA_GPS *get_gps( void ) {
		return this->p_gps;
	}

	CSANGRIA_FLASH *get_flash( void ) {
		return this->p_flash;
	}
};

#endif
