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

#ifndef __CUSTOM_MENU_H__
#define __CUSTOM_MENU_H__

#include "controller.h"

typedef enum {
	SANGRIA_MENU_TOP		= 0,
	SANGRIA_MENU_OLED_ON_LEVEL,
	SANGRIA_MENU_OLED_OFF_LEVEL,
	SANGRIA_MENU_KEY_CUSTOM,
	SANGRIA_MENU_FLASH_WRITE,
} CSANGRIA_CUSTOM_MENU_STATE;

class CSANGRIA_CUSTOM_MENU {
private:
	CSANGRIA_CUSTOM_MENU_STATE	menu_state = SANGRIA_MENU_TOP;
	int menu_pos = 0;
	int cursor_pos = 0;
	uint8_t key_code[6];
	void wait_release_enter_button( CSANGRIA_CONTROLLER *p_controller );
	bool check_enter_button( CSANGRIA_CONTROLLER *p_controller );
	void update_modifier_state( CSANGRIA_CONTROLLER *p_controller );
public:
	int oled_on_level = 0;
	int oled_off_level = 2;
	int sangria_key_position = 0;
	int us_key_position = 0;
	int us_key_modifier = 0;
	int is_us_key_select = 0;
	int animation = 0;
	int sangria_modifier = 0;
	int last_key_state[2] = {};

	// --------------------------------------------------------------------
	//	Constructor
	CSANGRIA_CUSTOM_MENU();

	// --------------------------------------------------------------------
	//	Draw task
	bool draw( CSANGRIA_CONTROLLER *p_controller );
	bool draw_top_menu( CSANGRIA_CONTROLLER *p_controller );
	bool draw_oled_level( CSANGRIA_CONTROLLER *p_controller, const char *p_name, int &level );
	bool draw_key_custom( CSANGRIA_CONTROLLER *p_controller );
	bool draw_flash_write( CSANGRIA_CONTROLLER *p_controller );
};

#endif
