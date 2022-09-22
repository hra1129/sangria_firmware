// --------------------------------------------------------------------
//	The MIT License (MIT)
//	
//	Sangria firmware configuration file
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

#ifndef __SANGRIA_GRAPHIC_RESOURCE_H__
#define __SANGRIA_GRAPHIC_RESOURCE_H__

typedef enum {
	SANGRIA_ICON_AC_ADAPTER = 0,
	SANGRIA_ICON_DC_PLUG,
	SANGRIA_ICON_USB_POWER,
	SANGRIA_ICON_EMPTY,
	SANGRIA_ICON_HALF,
	SANGRIA_ICON_HIGH,
	SANGRIA_ICON_FULL,
	SANGRIA_ICON_NO_BATTERY,
	SANGRIA_ICON_ALT,
	SANGRIA_ICON_SHIFT,
	SANGRIA_ICON_SYM,
	SANGRIA_ICON_CTRL,
	SANGRIA_ICON_BIG_NO_POWER,
	SANGRIA_ICON_BIG_AC_POWER,
	SANGRIA_ICON_BIG_USB_POWER,
	SANGRIA_ICON_BIG_OTG_POWER,
	SANGRIA_ICON_BIG_BATTERY_EMPTY_A,
	SANGRIA_ICON_BIG_BATTERY_EMPTY_B,
	SANGRIA_ICON_BIG_BATTERY_LOW_A,
	SANGRIA_ICON_BIG_BATTERY_LOW_B,
	SANGRIA_ICON_BIG_BATTERY_HIGH_A,
	SANGRIA_ICON_BIG_BATTERY_HIGH_B,
	SANGRIA_ICON_BIG_BATTERY_FULL_A,
	SANGRIA_ICON_BIG_BATTERY_FULL_B,
	SANGRIA_ICON_BIG_BQ_IS_NOT_DETECT,
} SANGRIA_ICON_ID_T;

const uint8_t *get_sangria_logo1( void );
const uint8_t *get_sangria_logo2( void );
const uint8_t *get_icon( int id );
const uint8_t *get_font( void );

#endif
