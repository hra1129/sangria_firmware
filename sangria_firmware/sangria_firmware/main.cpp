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

#include "pico/stdlib.h"
#include "hardware/gpio.h"

#include "sangria_firmware_config.h"
#include "sangria_jogdial.h"
#include "sangria_keyboard.h"
#include "sangria_oled.h"
#include "sangria_usb_keyboard.h"

CSANGRIA_JOGDIAL jogdial;
CSANGRIA_KEYBOARD keyboard;
CSANGRIA_OLED oled;

extern uint8_t sangria_logo[];

// --------------------------------------------------------------------
class CANIME {
public:
	int x;
	int y;
	int wait;
	int state;
	int count;

	CANIME() {
		x = 128;
		y = 32;
		wait = 0;
		state = 0;
	}

	void draw( CSANGRIA_OLED &oled ) {
		if( state == 0 ) {
			if( x ) {
				x = x - 1;
			}
			if( y && (x & 3) == 0 ) {
				y = y - 1;
			}
			oled.clear();
			oled.copy( sangria_logo, 128, 32, x, y );
			if( x == 0 && y == 0 ) {
				state = 1;
				wait = 200;
			}
		}
		else if( state == 1 ) {
			wait--;
			if( wait == 0 ) {
				oled.clear();
				oled.puts( "SANGRIA v0.1\n" );
				state = 2;
				wait = 20;
				count = 10;
			}
		}
		else if( state == 2 ) {
			wait--;
			if( wait == 0 ) {
				oled.putc( '.' );
				count--;
				wait = 20;
				if( count == 0 ) {
					state = 3;
					oled.puts( "\nOK." );
				}
			}
		}
		else {
			jogdial.update();
			if( jogdial.get_back_button() ) {
				oled.puts( "\nBACK pressed." );
			}
			if( jogdial.get_enter_button() ) {
				oled.puts( "\nENTER pressed." );
			}
			if( jogdial.get_up_button() ) {
				oled.puts( "\nUP pressed." );
			}
			if( jogdial.get_down_button() ) {
				oled.puts( "\nDOWN pressed." );
			}
		}
		oled.update();
	}
};

// --------------------------------------------------------------------
int main( void ) {
	int i;
	CANIME anime;

	stdio_init_all();

	//while( 1 ) {
	//	jogdial.update();
	//	if( jogdial.get_back_button() ) {
	//		break;
	//	}
	//}

	while( 1 ) {
		keyboard.backlight( 1 );
		for( i = 0; i < 50; i++ ) {
			sleep_ms( 10 );
			keyboard.update();
			anime.draw( oled );
		}
		keyboard.backlight( 0 );
		for( i = 0; i < 50; i++ ) {
			sleep_ms( 10 );
			keyboard.update();
			anime.draw( oled );
		}
	}
	return 0;
}
