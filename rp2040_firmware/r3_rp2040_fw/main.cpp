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
static CSANGRIA_I2C *p_i2c_oled;
static CSANGRIA_I2C *p_i2c_bq;
static CSANGRIA_OLED *p_oled;
static CSANGRIA_BATTERY *p_battery;

// --------------------------------------------------------------------
static void display_battery_status( CSANGRIA_OLED *p_oled, CSANGRIA_BATTERY *p_battery, int anime ) {
	const uint8_t *p_icon;

	if( p_battery->check_battery_management_device() ) {
		int status = p_battery->get_system_status();
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
		p_oled->copy_1bpp( p_icon, 32, 32, 0, 0 );
		switch( (status >> 4) & 3 ) {
		case 0:		//	Not charging
			p_icon = get_icon( SANGRIA_ICON_BIG_BATTERY_EMPTY_A + anime );
			break;
		case 1:		//	Pre charging
			p_icon = get_icon( SANGRIA_ICON_BIG_BATTERY_LOW_A + anime );
			break;
		case 2:		//	Fast charging
			p_icon = get_icon( SANGRIA_ICON_BIG_BATTERY_HIGH_A + anime );
			break;
		default:	//	Charge termination done
			p_icon = get_icon( SANGRIA_ICON_BIG_BATTERY_FULL_A + anime );
			break;
		}
		p_oled->copy_1bpp( p_icon, 32, 32, 96, 0 );
	}
	else {
		//	Unkown
		p_icon = get_icon( SANGRIA_ICON_BIG_BQ_IS_NOT_DETECT );
		p_oled->copy_1bpp( p_icon, 32, 32, 0, 0 );
		//	Not charging
		p_icon = get_icon( SANGRIA_ICON_EMPTY );
		p_oled->copy_1bpp( p_icon, 32, 32, 96, 0 );
	}
}

// --------------------------------------------------------------------
class CBOOT_ANIME {
public:
	int x;
	int y;
	int wait;
	int state;
	int count;
	CSANGRIA_OLED *p_oled;
	CSANGRIA_BATTERY *p_battery;

	CBOOT_ANIME() {
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
				p_oled->copy_1bpp( get_sangria_logo1(), 128, 32, 0, 0 );
			}
		}
		else {
			p_oled->clear();
			//	Key status
			if( p_keyboard->get_shift_key() ) {
				p_oled->copy_1bpp( get_icon( SANGRIA_ICON_SHIFT ), 16, 16, 32, 0 );
			}
			if( p_keyboard->get_alt_key() ) {
				p_oled->copy_1bpp( get_icon( SANGRIA_ICON_ALT ), 16, 16, 48, 0 );
			}
			if( p_keyboard->get_sym_key() ) {
				p_oled->copy_1bpp( get_icon( SANGRIA_ICON_SYM ), 16, 16, 64, 0 );
			}
			if( p_keyboard->get_ctrl_key() ) {
				p_oled->copy_1bpp( get_icon( SANGRIA_ICON_CTRL ), 16, 16, 80, 0 );
			}
			//	Battery status
			count = (count + 1) & 63;
			display_battery_status( this->p_oled, this->p_battery, count >> 5 );
		}
		p_oled->update();
	}
};

// --------------------------------------------------------------------
class CSHUTDOWN_ANIME {
public:
	int state;
	int count;
	const char *p_string;
	int index;
	CSANGRIA_OLED *p_oled;
	CSANGRIA_BATTERY *p_battery;

	CSHUTDOWN_ANIME() {
		state = 0;
		p_string = "RasPiZero HAS\nSHUTDOWN...";
		index = 0;
		count = 0;
	}

	void set( CSANGRIA_OLED *p_oled, CSANGRIA_BATTERY *p_battery ) {
		this->p_oled = p_oled;
		this->p_battery = p_battery;
	}

	int draw( void ) {
		int i;

		if( state == 0 ) {
			//	カーソル点滅
			p_oled->clear();
			if( (count & 16) != 0 ) {
				p_oled->set_position( 0, 3 );
				p_oled->putc( 127 );
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
					p_oled->clear();
					p_oled->set_position( 0, 3 );
					for( i = 0; i < index; i++ ) {
						p_oled->putc( p_string[i] );
					}
					p_oled->putc( 127 );
					count = 10;
					index++;
				}
			}
			else {
				count--;
			}
		}
		else if( state == 2 ) {
			p_oled->clear();
			p_oled->set_position( 0, 3 );
			p_oled->puts( p_string );
			if( (count & 32) != 0 ) {
				p_oled->putc( 127 );
			}
			count++;
			if( count >= 360 ) {
				p_oled->clear();
				state = 3;
			}
		}
		else {
			//	Finish
			return 0;
		}
		p_oled->update();
		return 1;
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
//	Suspend mode
//	input:
//		none
//	output:
//		0: Go to Battery Status Mode
//		1: Go to Run Mode
//
static int suspend_mode( void ) {
	int status, count;
	bool is_oled_power = false;

	count = 0;
	p_keyboard->backlight( 0 );
	while( 1 ) {
		//	Check power plug
		if( p_battery->check_battery_management_device() ) {
			status = p_battery->get_system_status();
			if( ((status >> 6) & 3) != 0 ) {
				if( !is_oled_power ) {
					is_oled_power = true;
					p_oled->power_on();
				}
				count = (count + 1) & 15;
				display_battery_status( p_oled, p_battery, count >> 3 );
				p_oled->update();
			}
			else {
				if( is_oled_power ) {
					is_oled_power = false;
					p_oled->power_off();
				}
			}
		}
		else {
			if( is_oled_power ) {
				is_oled_power = false;
				p_oled->power_off();
			}
		}
		p_jogdial->update();
		if( !is_oled_power && p_jogdial->get_enter_button() ) {
			//	Go to Battery Status Mode
			return 0;
		}
		if( p_keyboard->check_host_connected() ) {
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
	static const int led_duty[] = { 1, 1, 1, 2, 2, 3, 4, 5, 6, 6, 7, 7, 7, 6, 6, 5, 4, 3, 2 };
	int index = 0;
	int i, count;

	count = 0;
	p_keyboard->backlight( 1 );
	p_oled->power_on();				// <== VERY SLOW!
	while( time_out ) {
		p_jogdial->update();
		if( p_jogdial->get_enter_button() ) {
			//	Reset time out counter
			time_out = 50;
		}
		if( p_keyboard->check_host_connected() ) {
			//	Go to Run Mode
			p_oled->power_off();
			p_keyboard->backlight( 0 );
			return 1;
		}
		count = (count + 1) & 15;
		display_battery_status( p_oled, p_battery, count >> 3 );
		p_oled->update();
		for( i = 0; i < 10; i++ ) {
			p_keyboard->backlight( 1 );
			sleep_ms( led_duty[index] );
			p_keyboard->backlight( 0 );
			sleep_ms( 10 - led_duty[index] );
			index = (index + 1) % (sizeof(led_duty) / sizeof(led_duty[0]));
		}
		time_out--;
	}
	//	Go to Suspend Mode
	p_oled->power_off();
	p_keyboard->backlight( 0 );
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
		if( !anime.draw() ) {
			break;
		}

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
	CSANGRIA_I2C i2c_oled( SANGRIA_OLED_I2C, SANGRIA_I2C1_CLOCK, SANGRIA_I2C1_SCL, SANGRIA_I2C1_SDA );
	CSANGRIA_I2C i2c_bq( SANGRIA_BQ_I2C, SANGRIA_I2C0_CLOCK, SANGRIA_I2C0_SCL, SANGRIA_I2C0_SDA );
	CSANGRIA_OLED oled;
	CSANGRIA_BATTERY battery;

	p_jogdial = &jogdial;
	p_keyboard = &keyboard;
	p_i2c_oled = &i2c_oled;
	p_i2c_bq = &i2c_bq;
	p_oled = &oled;
	p_battery = &battery;

	p_oled->set_i2c( p_i2c_oled );
	p_battery->set_i2c( p_i2c_bq );
	keyboard.set_jogdial( p_jogdial );

	p_battery->power_on();
	multicore_launch_core1( other_core );

	tusb_init();
	usb_core( keyboard );
	return 0;
}
