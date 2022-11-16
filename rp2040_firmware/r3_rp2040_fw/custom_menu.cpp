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
//	main.cpp
void do_write_flash( void );

// --------------------------------------------------------------------
static const char *p_menu_item[] = {
//	 0123456789012345
	"OLED LV.(ON)",
	"OLED LV.(OFF)",
	"KEY CUSTOM",
	"FLASH WR TEST",
	"ITEM4",
	"ITEM5",
	"ITEM6",
	"EXIT",
};

typedef enum {
	MENU_ITEM_ID_OLED_ON_LEVEL = 0,
	MENU_ITEM_ID_OLED_OFF_LEVEL,
	MENU_ITEM_ID_KEY_CUSTOM,
	MENU_ITEM_ID_FLASH_WRITE_TEST,
	MENU_ITEM_ID_RESERVED4,
	MENU_ITEM_ID_RESERVED5,
	MENU_ITEM_ID_RESERVED6,
	MENU_ITEM_EXIT,
} MENU_ITEM_T;

#define MENU_HEIGHT	4

// --------------------------------------------------------------------
//  key bit assign
//         ROW0 ROW1 ROW2 ROW3 ROW4 ROW5 ROW6
//    COL0  Q    W   sym   A   alt  SPC  MIC
//    COL1  E    S    D    P    Z    X   SH1
//    COL2  R    G    T   SH2   V    C    F
//    COL3  U    H    Y   RET   B    N    J
//    COL4  O    L    I   BK    $    M    K
//    COL5 JEN  JUP  JDN  BAK  N/A  N/A  N/A  ※Jogdialは、管理の都合でここにマッピング 
#define CR( col, row )	( (row) + (col) * 8 )
#define CR_GET_COL( cr )	( (cr) >> 3 )
#define CR_GET_ROW( cr )	( (cr) & 7 )

//  key index
//          0    1    2    3    4    5    6    7    8    9
//     0    Q    W    E    R    T    Y    U    I    O    P
//     1    A    S    D    F    G    H    J    K    L    BK
//     2   alt   Z    X    C    V    B    N    M    $   RET
//     3   SH1  MIC  SPC  sym  SH2  JEN  JUP  JDN  BAK  N/A
//
static const int keyindex_assign_table[] = {
	//  Q        W        E        R        T        Y        U        I        O        P
	CR(0,0), CR(0,1), CR(1,0), CR(2,0), CR(2,2), CR(3,2), CR(3,0), CR(4,2), CR(4,0), CR(1,3),
	//  A        S        D        F        G        H        J        K        L        BK
	CR(0,3), CR(1,1), CR(1,2), CR(2,6), CR(2,1), CR(3,1), CR(3,6), CR(4,6), CR(4,1), CR(4,3),
	// alt       Z        X        C        V        B        N        M        $       RET
	CR(0,4), CR(1,4), CR(1,5), CR(2,5), CR(2,4), CR(3,4), CR(3,5), CR(4,5), CR(4,4), CR(3,3),
	// SH1      MIC      SPC      sym      SH2      JEN      JUP      JDN      BAK      N/A
	CR(1,6), CR(0,6), CR(0,5), CR(0,2), CR(2,3), CR(5,0), CR(5,1), CR(5,2), CR(5,3), CR(5,4),
};

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
bool CSANGRIA_CUSTOM_MENU::draw_key_custom( CSANGRIA_CONTROLLER *p_controller ) {
	const uint8_t *p_icon;
	CSANGRIA_OLED *p_oled = p_controller->get_oled();
	uint8_t key_code[6];

	//	Check button
	p_controller->get_keyboard()->update( key_code );
	p_controller->get_jogdial()->update();
	if( p_controller->get_jogdial()->get_enter_button() ) {
		this->is_us_key_select = 1 - this->is_us_key_select;
		wait_release_jogdial_buttons( p_controller );
	}
	if( p_controller->get_jogdial()->get_back_button() ) {
		wait_release_jogdial_buttons( p_controller );
		this->is_us_key_select = 0;
		return false;
	}

	if( this->is_us_key_select ) {
		//	US Keymap select
		if( !p_controller->get_keyboard()->get_sym_key() ) {
			if( p_controller->get_jogdial()->get_up_button() ) {
				this->us_key_position = (this->us_key_position & 0xF0) | ((this->us_key_position +  1) & 0x0F);
			}
			else if( p_controller->get_jogdial()->get_down_button() ) {
				this->us_key_position = (this->us_key_position & 0xF0) | ((this->us_key_position + 15) & 0x0F);
			}
		}
		else {
			if( p_controller->get_jogdial()->get_up_button() ) {
				this->us_key_position = (this->us_key_position + 0x10) & 0xFF;
			}
			else if( p_controller->get_jogdial()->get_down_button() ) {
				this->us_key_position = (this->us_key_position + 0xF0) & 0xFF;
			}
		}
	}
	else {
		//	Sangria key select
		if( !p_controller->get_keyboard()->get_sym_key() ) {
			if( p_controller->get_jogdial()->get_up_button() ) {
				this->sangria_key_position = (this->sangria_key_position / 10) * 10 + (((this->sangria_key_position % 10) + 1) % 10);
			}
			else if( p_controller->get_jogdial()->get_down_button() ) {
				this->sangria_key_position = (this->sangria_key_position / 10) * 10 + (((this->sangria_key_position % 10) + 9) % 10);
			}
		}
		else {
			if( p_controller->get_jogdial()->get_up_button() ) {
				this->sangria_key_position = (this->sangria_key_position + 10) % 40;
			}
			else if( p_controller->get_jogdial()->get_down_button() ) {
				this->sangria_key_position = (this->sangria_key_position + 30) % 40;
			}
		}
	}

	if( !this->is_us_key_select ) {
		//	Sangriaキーを選択している最中は、連動して USキーの表示が変化する
		this->us_key_position = p_controller->get_flash()->get()->key_matrix_table[0][ keyindex_assign_table[ this->sangria_key_position ] ] & 255;
	}

	p_oled->clear();
	if( this->is_us_key_select || ((this->animation & 16) != 0) ) {
		p_icon = get_icon( SANGRIA_ICON_KEYMAP_SANGRIA );
		p_oled->copy_1bpp( p_icon, 36, 8, 40, 0 );
	}
	if( !this->is_us_key_select || ((this->animation & 16) != 0) ) {
		p_icon = get_icon( SANGRIA_ICON_KEYMAP_TARGET );
		p_oled->copy_1bpp( p_icon, 36, 8, 52, 24 );
	}
	p_icon = get_icon( SANGRIA_ICON_KEYBOARD );
	p_oled->copy_1bpp_part( p_icon, 320, 128, (this->sangria_key_position % 10) * 32, (this->sangria_key_position / 10) * 32, 32, 32, 8, 0 );
	p_icon = get_icon( SANGRIA_ICON_US_KEYMAP );
	p_oled->copy_1bpp_part( p_icon, 512, 512, (this->us_key_position & 0x0F) * 32, (this->us_key_position >> 4) * 32, 32, 32, 88, 0 );
	p_oled->update();
	this->animation = (this->animation + 1) & 63;
	return true;
}

// --------------------------------------------------------------------
bool CSANGRIA_CUSTOM_MENU::draw_flash_write_test( CSANGRIA_CONTROLLER *p_controller ) {

	do_write_flash();
	menu_state = SANGRIA_MENU_TOP;
	wait_release_jogdial_buttons( p_controller );
	return false;
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
		if( cursor_pos == MENU_ITEM_ID_KEY_CUSTOM ) {
			wait_release_jogdial_buttons( p_controller );
			menu_state = SANGRIA_MENU_KEY_CUSTOM;
			//	★暫定★ 
			memcpy( p_controller->get_flash()->get()->key_matrix_table, p_controller->get_keyboard()->get_default_keymap(), sizeof(uint16_t) * 4 * 5 * 8 );
			return true;
		}
		if( cursor_pos == MENU_ITEM_ID_FLASH_WRITE_TEST ) {
			wait_release_jogdial_buttons( p_controller );
			menu_state = SANGRIA_MENU_FLASH_WRITE_TEST;
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
	case SANGRIA_MENU_KEY_CUSTOM:
		if( !this->draw_key_custom( p_controller ) ) {
			menu_state = SANGRIA_MENU_TOP;
		}
		break;
	case SANGRIA_MENU_FLASH_WRITE_TEST:
		if( !this->draw_flash_write_test( p_controller ) ) {
			menu_state = SANGRIA_MENU_TOP;
		}
		break;
	}
	return result;
}
