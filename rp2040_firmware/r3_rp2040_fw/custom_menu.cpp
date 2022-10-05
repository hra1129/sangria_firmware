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

#include "custom_menu.h"
#include "sangria_graphic_resource.h"

// --------------------------------------------------------------------
static const char *p_menu_item[] = {
//	 0123456789012345
	"OLED LV.(ON)",
	"OLED LV.(OFF)",
	"KEY CUSTOM",
	"ITEM3",
	"ITEM4",
	"ITEM5",
	"ITEM6",
	"EXIT",
};

typedef enum {
	MENU_ITEM_ID_OLED_ON_LEVEL = 0,
	MENU_ITEM_ID_OLED_OFF_LEVEL,
	MENU_ITEM_ID_KEY_CUSTOM,
	MENU_ITEM_ID_RESERVED3,
	MENU_ITEM_ID_RESERVED4,
	MENU_ITEM_ID_RESERVED5,
	MENU_ITEM_ID_RESERVED6,
	MENU_ITEM_EXIT,
} MENU_ITEM_T;

#define MENU_HEIGHT	4

// --------------------------------------------------------------------
CSANGRIA_CUSTOM_MENU::CSANGRIA_CUSTOM_MENU() {
}

// --------------------------------------------------------------------
void CSANGRIA_CUSTOM_MENU::wait_release_jogdial_buttons( CSANGRIA_CONTROLLER *p_controller ) {

	while( p_controller->get_jogdial()->get_enter_button() || p_controller->get_jogdial()->get_back_button() ) {
		p_controller->get_jogdial()->update();
		sleep_ms( 1 );
	}
}

// --------------------------------------------------------------------
bool CSANGRIA_CUSTOM_MENU::draw_oled_level( CSANGRIA_CONTROLLER *p_controller, const char *p_name, int &level ) {
	int i;
	static const int oled_level[] = { 1, 2, 4, 8, 16, 32, 64, 127 };
	char s_buffer[16];

	p_controller->get_oled()->clear();
	//	Move cursor position
	if( level > 0 && p_controller->get_jogdial()->get_down_button() ) {
		level--;
	}
	if( level < 7 && p_controller->get_jogdial()->get_up_button() ) {
		level++;
	}

	//	Check button
	if( p_controller->get_jogdial()->get_enter_button() ) {
		if( cursor_pos == MENU_ITEM_EXIT ) {
			wait_release_jogdial_buttons( p_controller );
			menu_state = SANGRIA_MENU_TOP;
			return false;
		}
	}
	if( p_controller->get_jogdial()->get_back_button() ) {
		wait_release_jogdial_buttons( p_controller );
		return false;
	}

	p_controller->get_oled()->clear();
	p_controller->get_oled()->set_position( 0, 0 );
	p_controller->get_oled()->puts( p_name );
	p_controller->get_oled()->set_position( 0, 1 );
	p_controller->get_oled()->puts( "----------------" );
	p_controller->get_oled()->set_position( 0, 2 );
	p_controller->get_oled()->puts( "LV[" );
	for( i = 0; i < 8; i++ ) {
		p_controller->get_oled()->putc( i <= level ? '#' : '-' );
	}
	sprintf( s_buffer, "] %3d", level );
	p_controller->get_oled()->puts( s_buffer );
	p_controller->get_oled()->update();
	p_controller->get_oled()->set_contrast_level( oled_level[ level ] );
	return true;
}

// --------------------------------------------------------------------
bool CSANGRIA_CUSTOM_MENU::draw_top_menu( CSANGRIA_CONTROLLER *p_controller ) {
	int i;

	p_controller->get_oled()->clear();
	//	Move cursor position
	if( p_controller->get_jogdial()->get_up_button() ) {
		cursor_pos--;
	}
	if( p_controller->get_jogdial()->get_down_button() ) {
		cursor_pos++;
	}
	if( cursor_pos < 0 ) {
		cursor_pos = (int) MENU_ITEM_EXIT;
	}
	if( cursor_pos > MENU_ITEM_EXIT ) {
		cursor_pos = 0;
	}
	//	Move menu position
	if( cursor_pos < menu_pos ) {
		menu_pos = cursor_pos;
	}
	if( cursor_pos >= (menu_pos + MENU_HEIGHT) ) {
		menu_pos = cursor_pos - MENU_HEIGHT + 1;
	}
	//	Draw menu
	for( i = 0; i < MENU_HEIGHT; i++ ) {
		if( (menu_pos + i) == cursor_pos ) {
			p_controller->get_oled()->set_position( 0, i );
			p_controller->get_oled()->putc( '[' );
			p_controller->get_oled()->puts( p_menu_item[ menu_pos + i ] );
			p_controller->get_oled()->putc( ']' );
		}
		else {
			p_controller->get_oled()->set_position( 1, i );
			p_controller->get_oled()->puts( p_menu_item[ menu_pos + i ] );
		}
	}
	p_controller->get_oled()->update();
	//	Check button
	if( p_controller->get_jogdial()->get_enter_button() ) {
		if( cursor_pos == MENU_ITEM_EXIT ) {
			wait_release_jogdial_buttons( p_controller );
			return false;
		}
		if( cursor_pos == MENU_ITEM_ID_OLED_ON_LEVEL ) {
			wait_release_jogdial_buttons( p_controller );
			menu_state = SANGRIA_MENU_OLED_ON_LEVEL;
			return true;
		}
		if( cursor_pos == MENU_ITEM_ID_OLED_OFF_LEVEL ) {
			wait_release_jogdial_buttons( p_controller );
			menu_state = SANGRIA_MENU_OLED_OFF_LEVEL;
			return true;
		}
	}
	if( p_controller->get_jogdial()->get_back_button() ) {
		wait_release_jogdial_buttons( p_controller );
		return false;
	}
	return true;
}

// --------------------------------------------------------------------
bool CSANGRIA_CUSTOM_MENU::draw( CSANGRIA_CONTROLLER *p_controller ) {
	bool result = true;

	switch( menu_state ) {
	default:
	case SANGRIA_MENU_TOP:
		result = this->draw_top_menu( p_controller );
		break;
	case SANGRIA_MENU_OLED_ON_LEVEL:
		if( !this->draw_oled_level( p_controller, "OLED ON LEVEL", oled_on_level ) ) {
			menu_state = SANGRIA_MENU_TOP;
		}
		break;
	case SANGRIA_MENU_OLED_OFF_LEVEL:
		if( !this->draw_oled_level( p_controller, "OLED OFF LEVEL", oled_off_level ) ) {
			menu_state = SANGRIA_MENU_TOP;
		}
		break;
	}
	return result;
}
