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

extern uint8_t sangria_logo[];

// --------------------------------------------------------------------
class CANIME {
public:
	int x;
	int y;
	int wait;
	int state;
	int count;
	CSANGRIA_OLED *p_oled;
	CSANGRIA_JOGDIAL *p_jogdial;
	CSANGRIA_KEYBOARD *p_keyboard;

	CANIME() {
		x = 128;
		y = 32;
		wait = 0;
		state = 0;
	}

	void set( CSANGRIA_OLED *p_oled, CSANGRIA_JOGDIAL *p_jogdial, CSANGRIA_KEYBOARD *p_keyboard ) {
		this->p_oled = p_oled;
		this->p_jogdial = p_jogdial;
		this->p_keyboard = p_keyboard;
	}

	void draw( void ) {
		if( state == 0 ) {
			if( x ) {
				x = x - 1;
			}
			if( y && (x & 3) == 0 ) {
				y = y - 1;
			}
			p_oled->clear();
			p_oled->copy( sangria_logo, 128, 32, x, y );
			if( x == 0 && y == 0 ) {
				state = 1;
				wait = 200;
			}
		}
		else if( state == 1 ) {
			wait--;
			if( wait == 0 ) {
				p_oled->clear();
				p_oled->puts( "SANGRIA v0.1\n" );
				state = 2;
				wait = 20;
				count = 10;
			}
		}
		else if( state == 2 ) {
			wait--;
			if( wait == 0 ) {
				p_oled->putc( '.' );
				count--;
				wait = 20;
				if( count == 0 ) {
					state = 3;
					p_oled->puts( "\nOK." );
				}
			}
		}
		else {
			p_jogdial->update();
			if( p_jogdial->get_back_button() ) {
				p_oled->puts( "\nBACK pressed." );
			}
			if( p_jogdial->get_enter_button() ) {
				p_oled->puts( "\nENTER pressed." );
			}
			if( p_jogdial->get_up_button() ) {
				p_oled->puts( "\nUP pressed." );
			}
			if( p_jogdial->get_down_button() ) {
				p_oled->puts( "\nDOWN pressed." );
			}
		}
		p_oled->update();
	}
};

// --------------------------------------------------------------------
void usb_core( void ) {

	for(;;) {
		//	tinyusb device task
		tud_task();
		//	sangria_usb_keyboard HID task
		//hid_task( keyboard, jogdial );
		hid_task();
	}
}

// --------------------------------------------------------------------
void other_core( void ) {
	CANIME anime;
	uint32_t start_ms, end_ms;
	int i;

	CSANGRIA_OLED oled;
	CSANGRIA_JOGDIAL jogdial;
	CSANGRIA_KEYBOARD keyboard;
	oled.power_on();

	anime.set( &oled, &jogdial, &keyboard );

	while( 1 ) {
		sleep_ms( 10 );
		keyboard.backlight( 1 );
		for( i = 0; i < 50; i++ ) {
			start_ms = board_millis();
			keyboard.update();
			anime.draw();
			end_ms = board_millis();
			if( (end_ms - start_ms) < 10 ) {
				sleep_ms( 10 - (end_ms - start_ms) );
			}
		}
		keyboard.backlight( 0 );
		for( i = 0; i < 50; i++ ) {
			start_ms = board_millis();
			keyboard.update();
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

	//while( 1 ) {
	//	p_jogdial->update();
	//	if( p_jogdial->get_back_button() ) {
	//		break;
	//	}
	//}

	multicore_launch_core1( other_core );
	usb_core();
	return 0;
}
