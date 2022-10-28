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

#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <cmath>

#include "bsp/board.h"
#include "tusb.h"

#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "hardware/gpio.h"

#include "controller.h"
#include "sangria_graphic_resource.h"

// --------------------------------------------------------------------
void display_battery_status( CSANGRIA_CONTROLLER *p_controller, int anime ) {
	const uint8_t *p_icon;
	int battery_level;

	if( p_controller->get_battery()->check_battery_management_device() ) {
		int status = p_controller->get_battery()->get_system_status();
		switch( (status >> 6) & 3 ) {
		default:
		case 0:		//	Unkown
			p_icon = get_icon( SANGRIA_ICON_BIG_NO_POWER );
			break;
		case 1:		//	USB host
			p_icon = get_icon( SANGRIA_ICON_BIG_USB_POWER );
			break;
		case 2:		//	Adapter port
			p_icon = get_icon( SANGRIA_ICON_BIG_AC_POWER );
			break;
		case 3:		//	OTG
			p_icon = get_icon( SANGRIA_ICON_BIG_OTG_POWER );
			break;
		}
		p_controller->get_oled()->copy_1bpp( p_icon, 32, 32, 0, 0 );
		switch( (status >> 4) & 3 ) {
		case 0:		//	Not charging
			p_icon = get_icon( SANGRIA_ICON_NO_CHARGE );
			break;
		case 1:		//	Pre charging
			p_icon = get_icon( SANGRIA_ICON_PRE_CHARGE0 + (anime & 1) );
			break;
		case 2:		//	Fast charging
			p_icon = get_icon( SANGRIA_ICON_CHARGE0 + (anime & 7) );
			break;
		default:	//	Charge termination done
			p_icon = get_icon( SANGRIA_ICON_CHARGE_TERMINATION_DONE );
			break;
		}
		p_controller->get_oled()->copy_1bpp( p_icon, 32, 16, 96, 16 );
	}
	else {
		//	Unkown
		p_icon = get_icon( SANGRIA_ICON_BIG_BQ_IS_NOT_DETECT );
		p_controller->get_oled()->copy_1bpp( p_icon, 32, 32, 0, 0 );
		//	Not charging
		p_icon = get_icon( SANGRIA_ICON_EMPTY );
		p_controller->get_oled()->copy_1bpp( p_icon, 32, 32, 96, 0 );
	}
	battery_level = p_controller->get_battery()->get_battery_level();
	//	Normalize 0%...100% ==> 0...5
	battery_level = ((battery_level - 1100) * 5) / (1750 - 1100);
	if( battery_level > 5 ) {
		battery_level = 5;
	}
	p_icon = get_icon( SANGRIA_ICON_BATTERY_000 + battery_level );
	p_controller->get_oled()->copy_1bpp( p_icon, 32, 16, 96, 0 );
}
