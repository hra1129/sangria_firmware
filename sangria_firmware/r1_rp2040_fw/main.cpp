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

#include "sangria_firmware_config.h"
#include "sangria_jogdial.h"
#include "sangria_keyboard.h"
#include "sangria_oled.h"
#include "sangria_usb_keyboard.h"
#include "sangria_battery.h"

#include "sangria_graphic_resource.h"

static CSANGRIA_KEYBOARD *p_keyboard;
static CSANGRIA_JOGDIAL *p_jogdial;

// --------------------------------------------------------------------
class CANIME {
public:
	int x;
	int y;
	int wait;
	int state;
	int count;
	CSANGRIA_OLED *p_oled;
	CSANGRIA_BATTERY *p_battery;

	CANIME() {
		x = 128;
		y = 0;
		wait = 0;
		state = 0;
	}

	void set( CSANGRIA_OLED *p_oled, CSANGRIA_BATTERY *p_battery ) {
		this->p_oled = p_oled;
		this->p_battery = p_battery;
	}

	void draw( void ) {
		if( state == 0 ) {
			//	左右からスワイプ
			p_oled->clear();
			if( y == 0 ) {
				p_oled->copy_1bpp( get_sangria_logo1(), 128, 32, x, 0 );
				y = 1;
			}
			else {
				p_oled->copy_1bpp( get_sangria_logo1(), 128, 32, -x, 0 );
				y = 0;
				x--;
			}
			if( x == 0 && y == 0 ) {
				state = 1;
				wait = 0;
			}
		}
		else if( state == 1 ) {
			//	点滅
			x = sqrt( wait * 8 );
			if( x != y ) {
				y = x;
				p_oled->copy_1bpp( get_sangria_logo1(), 128, 32, 0, 0 );
			}
			else {
				p_oled->copy_1bpp( get_sangria_logo2(), 128, 32, 0, 0 );
			}
			wait++;
			if( wait >= 300 ) {
				state = 2;
				wait = 100;
			}
		}
		else if( state == 2 ) {
			//	停止
			wait--;
			if( wait == 0 ) {
				state = 3;
				wait = 20;
				count = 10;
			}
			else {
				p_oled->copy_1bpp( get_sangria_logo2(), 128, 32, 0, 0 );
			}
		}
		else {
			p_oled->clear();
			//	Key status
			if( p_keyboard->get_shift_key() ) {
				p_oled->copy_1bpp( get_icon_shift(), 16, 16, 0, 0 );
			}
			if( p_keyboard->get_alt_key() ) {
				p_oled->copy_1bpp( get_icon_alt(), 16, 16, 16, 0 );
			}
			if( p_keyboard->get_sym_key() ) {
				p_oled->copy_1bpp( get_icon_sym(), 16, 16, 32, 0 );
			}
			if( p_keyboard->get_ctrl_key() ) {
				p_oled->copy_1bpp( get_icon_ctrl(), 16, 16, 48, 0 );
			}
			//	Battery status
			if( p_battery->check_battery_management_device() ) {
				int status = p_battery->get_system_status();
				const uint8_t *p_icon;
				switch( (status >> 6) & 3 ) {
				case 0:		//	Unkown
					p_icon = get_icon_no_battery();
					break;
				case 1:		//	USB host
					p_icon = get_icon_empty();
					break;
				case 2:		//	Adapter port
					p_icon = get_icon_dc_plug();
					break;
				default:	//	OTG
					p_icon = get_icon_full();
					break;
				}
				p_oled->copy_1bpp( p_icon, 16, 16, 80, 0 );
				switch( (status >> 4) & 3 ) {
				case 0:		//	Not charging
					p_icon = get_icon_no_battery();
					break;
				case 1:		//	Pre charging
					p_icon = get_icon_empty();
					break;
				case 2:		//	Fast charging
					p_icon = get_icon_half();
					break;
				default:	//	Charge termination done
					p_icon = get_icon_full();
					break;
				}
				p_oled->copy_1bpp( p_icon, 16, 16, 96, 0 );
			}
			else {
				p_oled->copy_1bpp( get_icon_no_battery(), 16, 16, 96, 0 );
			}
		}
		p_oled->update();
	}
};

// --------------------------------------------------------------------
void usb_core( CSANGRIA_KEYBOARD &keyboard ) {

	for(;;) {
		//	tinyusb device task
		tud_task();
		//	sangria_usb_keyboard HID task
		hid_task( keyboard );
	}
}

// --------------------------------------------------------------------
void other_core( void ) {
	CANIME anime;
	uint32_t start_ms, end_ms;
	int i;

	CSANGRIA_I2C i2c;
	CSANGRIA_OLED oled;
	CSANGRIA_BATTERY battery;

	oled.set_i2c( &i2c );
	battery.set_i2c( &i2c );

	oled.power_on();
	battery.power_on();
	anime.set( &oled, &battery );

	while( 1 ) {
		sleep_ms( 10 );
		//p_keyboard->backlight( 1 );
		for( i = 0; i < 50; i++ ) {
			start_ms = board_millis();
			anime.draw();
			end_ms = board_millis();
			if( (end_ms - start_ms) < 10 ) {
				sleep_ms( 10 - (end_ms - start_ms) );
			}
		}
		//p_keyboard->backlight( 0 );
		for( i = 0; i < 50; i++ ) {
			start_ms = board_millis();
			anime.draw();
			end_ms = board_millis();
			if( (end_ms - start_ms) < 10 ) {
				sleep_ms( 10 - (end_ms - start_ms) );
			}
		}
	}
}

// --------------------------------------------------------------------
int main( void ) {

	board_init();
	tusb_init();

	CSANGRIA_JOGDIAL jogdial;
	p_jogdial = &jogdial;

	//while( 1 ) {
	//	jogdial.update();
	//	if( jogdial.get_back_button() ) {
	//		break;
	//	}
	//}

	CSANGRIA_KEYBOARD keyboard;
	p_keyboard = &keyboard;
	keyboard.set_jogdial( &jogdial );
	p_keyboard->backlight( 1 );

	multicore_launch_core1( other_core );
	usb_core( keyboard );
	return 0;
}
