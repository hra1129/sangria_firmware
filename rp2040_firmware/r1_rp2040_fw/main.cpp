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
#include "battery_level.h"
#include "custom_menu.h"
#include "sangria_graphic_resource.h"

static CSANGRIA_CONTROLLER controller;

// --------------------------------------------------------------------
class CBOOT_ANIME {
public:
	int char_count;
	int wait;
	int state;
	int count;
	CSANGRIA_CONTROLLER *p_controller;
	const char *s_title;

	const int c_string_putc_wait = 6;
	const int c_title_display_wait = 240;
	
	CBOOT_ANIME(): s_title( "SANGRIA System\nVersion R1\n" ) {
		char_count = 1;
		wait = 0;
		state = 0;
	}

	void set( CSANGRIA_CONTROLLER *_p_controller ) {
		this->p_controller = _p_controller;
	}

	void draw( void ) {
		int i;

		if( state == 0 ) {
			//	文字を順次表示
			if( wait == 0 ) {
				p_controller->get_oled()->clear();
				for( i = 0; i < char_count; i++ ) {
					p_controller->get_oled()->putc( s_title[i] );
				}
				p_controller->get_oled()->putc( 127 );
				if( s_title[ char_count ] == '\0' ) {
					state++;
					wait = c_title_display_wait;
				}
				else {
					char_count++;
					wait = c_string_putc_wait;
				}
			}
			else {
				wait--;
			}
		}
		else if( state == 1 ) {
			//	カーソルを点滅
			if( wait == 0 ) {
				state++;
			}
			else {
				p_controller->get_oled()->clear();
				p_controller->get_oled()->puts( s_title );
				p_controller->get_oled()->set_position( 0, 2 );
				if( (wait & 16) == 0 ) {
					p_controller->get_oled()->putc( ' ' );
				}
				else {
					p_controller->get_oled()->putc( 127 );
				}
				wait--;
			}
		}
		else {
			p_controller->get_oled()->clear();
			//	Key status
			if( p_controller->get_keyboard()->get_shift_key() ) {
				p_controller->get_oled()->copy_1bpp( get_icon( SANGRIA_ICON_SHIFT ), 16, 16, 32, 0 );
			}
			if( p_controller->get_keyboard()->get_alt_key() ) {
				p_controller->get_oled()->copy_1bpp( get_icon( SANGRIA_ICON_ALT ), 16, 16, 48, 0 );
			}
			if( p_controller->get_keyboard()->get_sym_key() ) {
				p_controller->get_oled()->copy_1bpp( get_icon( SANGRIA_ICON_SYM ), 16, 16, 64, 0 );
			}
			if( p_controller->get_keyboard()->get_ctrl_key() ) {
				p_controller->get_oled()->copy_1bpp( get_icon( SANGRIA_ICON_CTRL ), 16, 16, 80, 0 );
			}
			//	Battery status
			count = (count + 1) & 63;
			display_battery_status( p_controller, count >> 5 );
		}
		p_controller->get_oled()->update();
	}
};

// --------------------------------------------------------------------
class CSHUTDOWN_ANIME {
public:
	int state;
	int count;
	const char *p_string;
	int index;
	CSANGRIA_CONTROLLER *p_controller;

	CSHUTDOWN_ANIME() {
		state = 0;
		p_string = "RasPiZero HAS\nSHUTDOWN...";
		index = 0;
		count = 0;
	}

	void set( CSANGRIA_CONTROLLER *_p_controller ) {
		this->p_controller = _p_controller;
	}

	int draw( void ) {
		int i;

		if( state == 0 ) {
			//	カーソル点滅
			p_controller->get_oled()->clear();
			if( (count & 16) != 0 ) {
				p_controller->get_oled()->set_position( 0, 3 );
				p_controller->get_oled()->putc( 127 );
			}
			count++;
			if( count >= 120 ) {
				state = 1;
				count = 0;
			}
		}
		else if( state == 1 ) {
			if( count == 0 ) {
				if( p_string[ index ] == '\0' ) {
					state = 2;
					count = 0;
				}
				else {
					p_controller->get_oled()->clear();
					p_controller->get_oled()->set_position( 0, 3 );
					for( i = 0; i < index; i++ ) {
						p_controller->get_oled()->putc( p_string[i] );
					}
					p_controller->get_oled()->putc( 127 );
					count = 10;
					index++;
				}
			}
			else {
				count--;
			}
		}
		else if( state == 2 ) {
			p_controller->get_oled()->clear();
			p_controller->get_oled()->set_position( 0, 3 );
			p_controller->get_oled()->puts( p_string );
			if( (count & 32) != 0 ) {
				p_controller->get_oled()->putc( 127 );
			}
			count++;
			if( count >= 360 ) {
				p_controller->get_oled()->clear();
				state = 3;
			}
		}
		else {
			//	Finish
			return 0;
		}
		p_controller->get_oled()->update();
		return 1;
	}
};

// --------------------------------------------------------------------
//	Custom menu mode
//
static void custom_menu_mode( CSANGRIA_CONTROLLER *p_controller ) {
	uint32_t start_ms, end_ms;
	CSANGRIA_CUSTOM_MENU menu;

	p_controller->get_oled()->clear();
	p_controller->get_oled()->update();
	while( p_controller->get_jogdial()->get_back_button() ) {
		p_controller->get_jogdial()->update();
		sleep_ms( 16 );
	}

	for(;;) {
		start_ms = board_millis();
		p_controller->get_jogdial()->update();
		if( !menu.draw( p_controller ) ) {
			break;
		}

		if( !p_controller->get_keyboard()->check_host_connected() ) {
			break;
		}

		end_ms = board_millis();
		if( (end_ms - start_ms) < 16 ) {
			sleep_ms( 16 - (end_ms - start_ms) );
		}
	}

	p_controller->get_keyboard()->exit_menu_mode();
}

// --------------------------------------------------------------------
//	Suspend mode
//	input:
//		none
//	output:
//		0: Go to Battery Status Mode
//		1: Go to Run Mode
//
static int suspend_mode( CSANGRIA_CONTROLLER *p_controller ) {
	int status, count;
	bool is_oled_power = false;

	count = 0;
	p_controller->get_keyboard()->backlight( 0 );
	for(;;) {
		//	Check enter the custom menu mode
		if( p_controller->get_keyboard()->is_menu_mode() ) {
			//	Go to custom menu mode
			custom_menu_mode( p_controller );
			continue;
		}
		//	Check power plug
		if( p_controller->get_battery()->check_battery_management_device() ) {
			status = p_controller->get_battery()->get_system_status();
			if( ((status >> 6) & 3) != 0 ) {
				if( !is_oled_power ) {
					is_oled_power = true;
					p_controller->get_oled()->power_on();
				}
				count = (count + 1) & 15;
				display_battery_status( p_controller, count >> 3 );
				p_controller->get_oled()->update();
			}
			else {
				if( is_oled_power ) {
					is_oled_power = false;
					p_controller->get_oled()->power_off();
				}
			}
		}
		else {
			if( is_oled_power ) {
				is_oled_power = false;
				p_controller->get_oled()->power_off();
			}
		}
		p_controller->get_jogdial()->update();
		if( !is_oled_power && p_controller->get_jogdial()->get_enter_button() ) {
			//	Go to Battery Status Mode
			return 0;
		}
		if( p_controller->get_keyboard()->check_host_connected() ) {
			//	Go to Run Mode
			return 1;
		}
		sleep_ms( 100 );
	}
}

// --------------------------------------------------------------------
//	Battery Status Mode
//	input:
//		none
//	output:
//		0: Go to Suspend Mode
//		1: Go to Run Mode
//
static int battery_status_mode( void ) {
	int time_out = 50;	//	5.0sec

	p_oled->power_on();
	while( time_out ) {
		if( board_button_read() ) {
			//	Reset time out counter
			time_out = 50;
		}
		if( p_keyboard->check_host_connected() ) {
			//	Go to Run Mode
			return 1;
		}
		display_battery_status( p_oled, p_battery );
		sleep_ms( 100 );
		time_out--;
	}
	//	Go to Suspend Mode
	p_oled->power_off();
	return 0;
}

// --------------------------------------------------------------------
static void run_mode( void ) {
	CBOOT_ANIME anime;
	uint32_t start_ms, end_ms;

	anime.set( p_oled, p_battery );

	p_keyboard->backlight( 1 );
	p_oled->power_on();

	while( 1 ) {
		start_ms = board_millis();
		anime.draw();

		if( !p_keyboard->check_host_connected() ) {
			break;
		}

		end_ms = board_millis();
		if( (end_ms - start_ms) < 16 ) {
			sleep_ms( 16 - (end_ms - start_ms) );
		}
	}
}

// --------------------------------------------------------------------
static void shutdown_mode( void ) {
	CSHUTDOWN_ANIME anime;
	uint32_t start_ms, end_ms;

	anime.set( p_oled, p_battery );

	while( 1 ) {
		start_ms = board_millis();
		anime.draw();

		if( p_keyboard->check_host_connected() ) {
			break;
		}

		end_ms = board_millis();
		if( (end_ms - start_ms) < 16 ) {
			sleep_ms( 16 - (end_ms - start_ms) );
		}
	}

	p_oled->power_off();
	p_keyboard->backlight( 0 );
}

// --------------------------------------------------------------------
void other_core( void ) {

	while( 1 ) {
		if( suspend_mode() == 0 ) {
			if( battery_status_mode() == 0 ) {
				continue;
			}
		}
		run_mode();
		shutdown_mode();
	}
}

// --------------------------------------------------------------------
int main( void ) {

	board_init();

	CSANGRIA_JOGDIAL jogdial;
	CSANGRIA_KEYBOARD keyboard;
	CSANGRIA_I2C i2c_all( SANGRIA_I2C, SANGRIA_I2C_CLOCK, SANGRIA_I2C_SCL, SANGRIA_I2C_SDA );
	CSANGRIA_OLED oled;
	CSANGRIA_BATTERY battery;

	p_jogdial = &jogdial;
	p_keyboard = &keyboard;
	p_i2c = &i2c_all;
	p_oled = &oled;
	p_battery = &battery;

	p_oled->set_i2c( p_i2c );
	p_battery->set_i2c( p_i2c );
	keyboard.set_jogdial( p_jogdial );

	p_battery->power_on();

	tusb_init();
	multicore_launch_core1( other_core );
	usb_core( keyboard );
	return 0;
}
