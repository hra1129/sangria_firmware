// --------------------------------------------------------------------
//	The MIT License (MIT)
//	
//	Sangria firmware flashROM
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

#ifndef __SANGRIA_FLASH_H__
#define __SANGRIA_FLASH_H__

#include <cstdint>
#include "sangria_firmware_config.h"

typedef struct {
	int		oled_contrast_level_for_stand_by;
	int		oled_contrast_level_for_power_on;
} SANGRIA_FLASH_DATA_T;

class CSANGRIA_FLASH {
private:
	SANGRIA_FLASH_DATA_T	data;

public:
	// --------------------------------------------------------------------
	//	Constructor
	CSANGRIA_FLASH();

	// --------------------------------------------------------------------
	SANGRIA_FLASH_DATA_T *get( void ) {
		return &data;
	}

	// --------------------------------------------------------------------
	void write( void );

	// --------------------------------------------------------------------
	void read( void );
};

#endif
