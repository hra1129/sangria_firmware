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

#include <cstring>
#include "sangria_flash.h"
#include "pico/stdlib.h"
#include "hardware/flash.h"

// --------------------------------------------------------------------
//  16Mbit (2MByte)
//
//  +000000 +--------------+
//          | Program area |
//          |              |  1.5MB
//          |              |
//  +180000 +--------------+  FLASH_WINDOW_ADDRESS, FLASH_TARGET_OFFSET
//          | Data area    |  512KB
//  +1FFFFF +--------------+
//
// --------------------------------------------------------------------
#define FLASH_TARGET_OFFSET		(1536 * 1024)

#define FLASH_WINDOW_ADDRESS	((void*)(0x10000000 + FLASH_TARGET_OFFSET))

// --------------------------------------------------------------------
CSANGRIA_FLASH::CSANGRIA_FLASH() {

	this->read();
}

// --------------------------------------------------------------------
void CSANGRIA_FLASH::write( void ) {
	uint offset, data_size;
	int i, pages;
	uint8_t buffer[ FLASH_PAGE_SIZE ], *p_data;

	// Erase current data
	data_size = (sizeof(this->data) + (FLASH_SECTOR_SIZE - 1)) % FLASH_SECTOR_SIZE;
	flash_range_erase( FLASH_TARGET_OFFSET, data_size );

	// Write new data
	offset = 0;
	pages = data_size / FLASH_PAGE_SIZE;
	data_size = sizeof(this->data);
	p_data = (uint8_t*) &(this->data);
	for( i = 0; i < pages; i++ ) {
		memcpy( buffer, p_data + offset, data_size > FLASH_PAGE_SIZE ? FLASH_PAGE_SIZE : data_size );
		flash_range_program( FLASH_TARGET_OFFSET + offset, buffer, FLASH_PAGE_SIZE );
		data_size -= FLASH_PAGE_SIZE;
		offset += FLASH_PAGE_SIZE;
	}
}

// --------------------------------------------------------------------
void CSANGRIA_FLASH::read( void ) {

	memcpy( &(this->data), FLASH_WINDOW_ADDRESS, sizeof(this->data) );
}
