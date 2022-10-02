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
} CSANGRIA_CUSTOM_MENU_STATE;

class CSANGRIA_CUSTOM_MENU {
private:
	CSANGRIA_CUSTOM_MENU_STATE	menu_state = SANGRIA_MENU_TOP;
	int menu_pos = 0;
	int cursor_pos = 0;
public:
	// --------------------------------------------------------------------
	//	Constructor
	CSANGRIA_CUSTOM_MENU();

	// --------------------------------------------------------------------
	//	Draw task
	bool draw( CSANGRIA_CONTROLLER *p_controller );
	bool draw_top_menu( CSANGRIA_CONTROLLER *p_controller );
};

#endif
