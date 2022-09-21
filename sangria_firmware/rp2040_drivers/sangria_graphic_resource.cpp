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

#include <cstdint>
#include "sangria_graphic_resource.h"

// --------------------------------------------------------------------
static const uint8_t icon_ac_adapter[] = {
	0x00, 0x00, 0x0C, 0x60, 0x0C, 0x60, 0x0C, 0x60, 0x0C, 0x60, 0x1F, 0xF0, 0x1F, 0xF0, 0x1F, 0xF0, 
	0x1F, 0xF0, 0x1F, 0xF0, 0x1F, 0xF0, 0x0F, 0xE0, 0x01, 0x00, 0x01, 0x00, 0x00, 0xD0, 0x00, 0x00, 
};

// --------------------------------------------------------------------
static const uint8_t icon_dc_plug[] = {
	0x00, 0x00, 0x00, 0x20, 0x00, 0x40, 0x3E, 0xBE, 0x21, 0x82, 0x23, 0x02, 0x67, 0xC2, 0x6F, 0xFA, 
	0x61, 0xF2, 0x20, 0x62, 0x20, 0xC2, 0x3E, 0xBE, 0x01, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 
};

// --------------------------------------------------------------------
static const uint8_t icon_usb_power[] = {
	0x00, 0x00, 0x0F, 0xF0, 0x08, 0x10, 0x0A, 0x50, 0x08, 0x10, 0x08, 0x10, 0x1F, 0xF8, 0x1F, 0xF8, 
	0x1F, 0x78, 0x1E, 0xF8, 0x1C, 0x38, 0x1F, 0x78, 0x1E, 0xF8, 0x0F, 0xF0, 0x07, 0xE0, 0x01, 0x80, 
};

// --------------------------------------------------------------------
static const uint8_t icon_empty[] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F, 0xFE, 0x20, 0x02, 0x20, 0x82, 0x60, 0x82, 0x60, 0x82, 
	0x60, 0x02, 0x20, 0x82, 0x20, 0x02, 0x3F, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
};

// --------------------------------------------------------------------
static const uint8_t icon_half[] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F, 0xFE, 0x20, 0x02, 0x20, 0x7A, 0x60, 0x7A, 0x60, 0x7A, 
	0x60, 0x7A, 0x20, 0x7A, 0x20, 0x02, 0x3F, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
};

// --------------------------------------------------------------------
static const uint8_t icon_high[] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F, 0xFE, 0x20, 0x02, 0x27, 0xFA, 0x67, 0xFA, 0x67, 0xFA, 
	0x67, 0xFA, 0x27, 0xFA, 0x20, 0x02, 0x3F, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
};

// --------------------------------------------------------------------
static const uint8_t icon_full[] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F, 0xFE, 0x20, 0x02, 0x2F, 0xFA, 0x6F, 0xFA, 0x6F, 0xFA, 
	0x6F, 0xFA, 0x2F, 0xFA, 0x20, 0x02, 0x3F, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
};

// --------------------------------------------------------------------
static const uint8_t icon_no_battery[] = {
	0x00, 0x00, 0x07, 0xF0, 0x0E, 0x38, 0x2D, 0xDA, 0x2C, 0x1A, 0x24, 0x1A, 0x60, 0x3A, 0x60, 0x72, 
	0x60, 0xE2, 0x20, 0x82, 0x20, 0x02, 0x3C, 0x9E, 0x01, 0xC0, 0x01, 0xC0, 0x00, 0x80, 0x00, 0x00, 
};

// --------------------------------------------------------------------
static const uint8_t icon_alt[] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x01, 0x10, 0x31, 0x10, 0x09, 0x7C, 
	0x39, 0x10, 0x49, 0x10, 0x49, 0x10, 0x34, 0x88, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
};

// --------------------------------------------------------------------
static const uint8_t icon_shift[] = {
	0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x02, 0x80, 0x04, 0x40, 0x08, 0x20, 0x10, 0x10, 0x20, 0x08, 
	0x3C, 0x78, 0x04, 0x40, 0x04, 0x40, 0x04, 0x40, 0x07, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
};

// --------------------------------------------------------------------
static const uint8_t icon_sym[] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x00, 0x90, 0x00, 0x80, 0x00, 0x40, 0x00, 
	0x24, 0xB4, 0x14, 0xAA, 0x94, 0xAA, 0x63, 0xAA, 0x00, 0x80, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 
};

// --------------------------------------------------------------------
static const uint8_t icon_ctrl[] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x6E, 0xC8, 0x84, 0xA8, 0x84, 0xA8, 0x84, 0xC8, 
	0x84, 0xA8, 0x84, 0xA8, 0x64, 0xAE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
};

// --------------------------------------------------------------------
static const uint8_t big_no_power[] = {
	0x3F, 0xFF, 0xFF, 0xFC, 0x40, 0x00, 0x00, 0x02, 0x80, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x01, 
	0x80, 0x10, 0x00, 0x01, 0x88, 0x10, 0xE0, 0x01, 0x8C, 0x11, 0xB8, 0x01, 0x8E, 0x11, 0x08, 0x01, 
	0x8B, 0x33, 0x08, 0x01, 0x89, 0xA3, 0x18, 0x01, 0x88, 0xE1, 0xF0, 0x01, 0x88, 0x60, 0x00, 0x01, 
	0x88, 0x20, 0x00, 0x09, 0x88, 0x00, 0x00, 0x09, 0x80, 0x00, 0x00, 0x09, 0x80, 0x00, 0x00, 0x29, 
	0x80, 0x00, 0x00, 0x69, 0x80, 0x00, 0x00, 0x49, 0x80, 0x00, 0x02, 0x41, 0x81, 0xE0, 0x22, 0xC9, 
	0x81, 0x20, 0x23, 0x81, 0x81, 0x30, 0x17, 0x81, 0x81, 0x33, 0xDC, 0x01, 0x81, 0xE6, 0x40, 0x01, 
	0x81, 0x07, 0xC0, 0x01, 0x81, 0x00, 0x00, 0x01, 0x82, 0x00, 0x00, 0x01, 0x82, 0x00, 0x00, 0x01, 
	0x82, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x01, 0x40, 0x00, 0x00, 0x02, 0x3F, 0xFF, 0xFF, 0xFC, 
};

// --------------------------------------------------------------------
static const uint8_t big_ac_power[] = {
	0x3F, 0xFF, 0xFF, 0xFC, 0x40, 0x00, 0x00, 0x02, 0x80, 0x0C, 0x30, 0x01, 0x80, 0x0C, 0x30, 0x01, 
	0x80, 0x0C, 0x30, 0x01, 0x80, 0x0C, 0x30, 0x01, 0x80, 0x0C, 0x30, 0x01, 0x80, 0x0C, 0x30, 0x01, 
	0x80, 0x0C, 0x30, 0x01, 0x80, 0x3F, 0xFC, 0x01, 0x80, 0x20, 0x04, 0x01, 0x80, 0x20, 0x44, 0x01, 
	0x80, 0x20, 0x84, 0x01, 0x80, 0x21, 0x04, 0x01, 0x80, 0x23, 0x04, 0x01, 0x80, 0x27, 0xE4, 0x01, 
	0x80, 0x20, 0xC4, 0x01, 0x80, 0x20, 0x84, 0x01, 0x80, 0x21, 0x04, 0x01, 0x80, 0x22, 0x04, 0x01, 
	0x80, 0x20, 0x04, 0x01, 0x80, 0x3F, 0xFC, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 
	0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0xC0, 0x01, 0x80, 0x00, 0xC0, 0x01, 0x80, 0x00, 0xE0, 0x01, 
	0x80, 0x00, 0x7D, 0xB1, 0x80, 0x00, 0x3D, 0xB1, 0x40, 0x00, 0x00, 0x02, 0x3F, 0xFF, 0xFF, 0xFC, 
};

// --------------------------------------------------------------------
const uint8_t big_usb_power[] = {
	0x3F, 0xFF, 0xFF, 0xFC, 0x40, 0x00, 0x00, 0x02, 0x80, 0x1F, 0xFC, 0x01, 0x80, 0x10, 0x04, 0x01, 
	0x80, 0x10, 0x04, 0x01, 0x80, 0x10, 0x04, 0x01, 0x80, 0x16, 0x34, 0x01, 0x80, 0x16, 0x34, 0x01, 
	0x80, 0x10, 0x04, 0x01, 0x80, 0x10, 0x04, 0x01, 0x80, 0x11, 0xC4, 0x01, 0x80, 0x10, 0x04, 0x01, 
	0x80, 0x10, 0x04, 0x01, 0x80, 0x7F, 0xFF, 0x01, 0x80, 0x40, 0x01, 0x01, 0x80, 0x40, 0x11, 0x01, 
	0x80, 0x40, 0x21, 0x01, 0x80, 0x40, 0x41, 0x01, 0x80, 0x40, 0xC1, 0x01, 0x80, 0x41, 0x81, 0x01, 
	0x80, 0x43, 0xF1, 0x01, 0x80, 0x40, 0x61, 0x01, 0x80, 0x40, 0xC1, 0x01, 0x80, 0x40, 0x81, 0x01, 
	0x80, 0x41, 0x01, 0x01, 0x80, 0x22, 0x02, 0x01, 0x80, 0x10, 0x04, 0x01, 0x80, 0x08, 0x08, 0x01, 
	0x80, 0x08, 0x08, 0x01, 0x80, 0x08, 0x08, 0x01, 0x40, 0x08, 0x08, 0x02, 0x3F, 0xFF, 0xFF, 0xFC, 
};

// --------------------------------------------------------------------
const uint8_t big_otg_power[] = {
	0x3F, 0xFF, 0xFF, 0xFC, 0x40, 0x00, 0x00, 0x02, 0x80, 0x1F, 0xFC, 0x01, 0x80, 0x10, 0x04, 0x01, 
	0x80, 0x10, 0x04, 0x01, 0x80, 0x10, 0x04, 0x01, 0x80, 0x16, 0x34, 0x01, 0x80, 0x16, 0x34, 0x01, 
	0x80, 0x10, 0x04, 0x01, 0x80, 0x10, 0x04, 0x01, 0x80, 0x11, 0xC4, 0x01, 0x80, 0x10, 0x04, 0x01, 
	0x80, 0x10, 0x04, 0x01, 0x80, 0x7F, 0xFF, 0x01, 0x80, 0x40, 0x01, 0x01, 0x80, 0x5D, 0xCD, 0x01, 
	0x80, 0x54, 0x91, 0x01, 0x80, 0x54, 0x95, 0x01, 0x80, 0x54, 0x95, 0x01, 0x80, 0x5C, 0x8D, 0x01, 
	0x80, 0x40, 0x01, 0x01, 0x80, 0x40, 0x01, 0x01, 0x80, 0x40, 0x01, 0x01, 0x80, 0x40, 0x01, 0x01, 
	0x80, 0x40, 0x01, 0x01, 0x80, 0x20, 0x02, 0x01, 0x80, 0x10, 0x04, 0x01, 0x80, 0x08, 0x08, 0x01, 
	0x80, 0x08, 0x08, 0x01, 0x80, 0x08, 0x08, 0x01, 0x40, 0x08, 0x08, 0x02, 0x3F, 0xFF, 0xFF, 0xFC, 
};

// --------------------------------------------------------------------
static const uint8_t *p_icon[] = {
	icon_ac_adapter,
	icon_dc_plug,
	icon_usb_power,
	icon_empty,
	icon_half,
	icon_high,
	icon_full,
	icon_no_battery,
	icon_alt,
	icon_shift,
	icon_sym,
	icon_ctrl,
	big_no_power,
	big_ac_power,
	big_usb_power,
	big_otg_power,
};

// --------------------------------------------------------------------
static const uint8_t font[] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // ' '
	0x00, 0x30, 0x30, 0x30, 0x20, 0x00, 0x30, 0x30, // '!'
	0x00, 0x6C, 0x48, 0x00, 0x00, 0x00, 0x00, 0x00, // '"'
	0x00, 0x22, 0x7F, 0x22, 0x22, 0x22, 0x7F, 0x22, // '#'
	0x10, 0x38, 0x54, 0x30, 0x18, 0x54, 0x38, 0x10, // '$'
	0x00, 0x42, 0xA4, 0xA8, 0x54, 0x2A, 0x4A, 0x84, // '%'
	0x00, 0x38, 0x44, 0x28, 0x70, 0x8A, 0x84, 0x7A, // '&'
	0x00, 0x18, 0x18, 0x10, 0x00, 0x00, 0x00, 0x80, // '''
	0x00, 0x08, 0x10, 0x20, 0x20, 0x20, 0x10, 0x08, // '('
	0x00, 0x20, 0x10, 0x08, 0x08, 0x08, 0x10, 0x20, // ')'
	0x00, 0x10, 0x54, 0x38, 0x10, 0x38, 0x54, 0x10, // '*'
	0x00, 0x00, 0x10, 0x10, 0x7C, 0x10, 0x10, 0x00, // '+'
	0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x10, 0x20, // ','
	0x00, 0x00, 0x00, 0x00, 0x7C, 0x00, 0x00, 0x00, // '-'
	0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x30, 0x00, // '.'
	0x00, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, // '/'
	0x00, 0x7C, 0x82, 0x8A, 0x92, 0xA2, 0x82, 0x7C, // '0'
	0x00, 0x10, 0x30, 0x10, 0x10, 0x10, 0x10, 0x38, // '1'
	0x00, 0x7C, 0x82, 0x82, 0x1C, 0x60, 0x80, 0xFE, // '2'
	0x00, 0x7C, 0x82, 0x02, 0x3C, 0x02, 0x82, 0x7C, // '3'
	0x00, 0x18, 0x28, 0x48, 0x88, 0x88, 0xFE, 0x08, // '4'
	0x00, 0xFE, 0x80, 0xFC, 0x02, 0x02, 0x82, 0x7C, // '5'
	0x00, 0x7C, 0x82, 0x80, 0xFC, 0x82, 0x82, 0x7C, // '6'
	0x00, 0xFE, 0x82, 0x04, 0x08, 0x10, 0x10, 0x10, // '7'
	0x00, 0x7C, 0x82, 0x82, 0x7C, 0x82, 0x82, 0x7C, // '8'
	0x00, 0x7C, 0x82, 0x82, 0x7E, 0x02, 0x82, 0x7C, // '9'
	0x00, 0x00, 0x30, 0x30, 0x00, 0x30, 0x30, 0x00, // ':'
	0x00, 0x00, 0x30, 0x30, 0x00, 0x30, 0x10, 0x20, // ';'
	0x00, 0x06, 0x18, 0x60, 0x80, 0x60, 0x18, 0x06, // '<'
	0x00, 0x00, 0x00, 0x7C, 0x00, 0x7C, 0x00, 0x00, // '='
	0x00, 0xC0, 0x30, 0x0C, 0x02, 0x0C, 0x30, 0xC0, // '>'
	0x00, 0x7C, 0x82, 0x82, 0x1C, 0x10, 0x00, 0x10, // '?'
	0x00, 0x7C, 0x82, 0xBA, 0xAA, 0xBE, 0x80, 0x7C, // '@'
	0x00, 0x38, 0x44, 0x82, 0x82, 0x82, 0xFE, 0x82, // 'A'
	0x00, 0xFC, 0x82, 0x82, 0xFC, 0x82, 0x82, 0xFC, // 'B'
	0x00, 0x7C, 0x82, 0x80, 0x80, 0x80, 0x82, 0x7C, // 'C'
	0x00, 0xF8, 0x84, 0x82, 0x82, 0x82, 0x84, 0xF8, // 'D'
	0x00, 0xFE, 0x80, 0x80, 0xFC, 0x80, 0x80, 0xFE, // 'E'
	0x00, 0xFE, 0x80, 0x80, 0xFC, 0x80, 0x80, 0x80, // 'F'
	0x00, 0x7C, 0x82, 0x80, 0x9E, 0x82, 0x82, 0x7C, // 'G'
	0x00, 0x82, 0x82, 0x82, 0xFE, 0x82, 0x82, 0x82, // 'H'
	0x00, 0x38, 0x10, 0x10, 0x10, 0x10, 0x10, 0x38, // 'I'
	0x00, 0x02, 0x02, 0x02, 0x02, 0x82, 0x82, 0x7C, // 'J'
	0x00, 0x82, 0x8C, 0xB0, 0xC8, 0x84, 0x84, 0x82, // 'K'
	0x00, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0xFE, // 'L'
	0x00, 0x82, 0xC6, 0xAA, 0x92, 0x82, 0x82, 0x82, // 'M'
	0x00, 0x82, 0xC2, 0xA2, 0x92, 0x8A, 0x86, 0x82, // 'N'
	0x00, 0x7C, 0x82, 0x82, 0x82, 0x82, 0x82, 0x7C, // 'O'
	0x00, 0xFC, 0x82, 0x82, 0x82, 0xFC, 0x80, 0x80, // 'P'
	0x00, 0x7C, 0x82, 0x82, 0x82, 0x9A, 0xA4, 0x7A, // 'Q'
	0x00, 0xFC, 0x82, 0x82, 0x82, 0xFC, 0x88, 0x86, // 'R'
	0x00, 0x7C, 0x82, 0x80, 0x7C, 0x02, 0x82, 0x7C, // 'S'
	0x00, 0xFE, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, // 'T'
	0x00, 0x82, 0x82, 0x82, 0x82, 0x82, 0x82, 0x7C, // 'U'
	0x00, 0x82, 0x82, 0x82, 0x82, 0x44, 0x28, 0x10, // 'V'
	0x00, 0x82, 0x82, 0x82, 0x92, 0xAA, 0xC6, 0x82, // 'W'
	0x00, 0x82, 0x44, 0x28, 0x10, 0x28, 0x44, 0x82, // 'X'
	0x00, 0x82, 0x44, 0x28, 0x10, 0x10, 0x10, 0x10, // 'Y'
	0x00, 0xFE, 0x02, 0x0C, 0x10, 0x60, 0x80, 0xFE, // 'Z'
	0x00, 0x1C, 0x10, 0x10, 0x10, 0x10, 0x10, 0x1C, // '['
	0x00, 0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, // '\'
	0x00, 0x38, 0x08, 0x08, 0x08, 0x08, 0x08, 0x38, // ']'
	0x00, 0x10, 0x28, 0x44, 0x00, 0x00, 0x00, 0x00, // '^'
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFE, // '_'
	0x00, 0x10, 0x08, 0x04, 0x00, 0x00, 0x00, 0x00, // '`'
	0x00, 0x00, 0x78, 0x84, 0x7C, 0x84, 0x84, 0x7A, // 'a'
	0x00, 0x80, 0x80, 0x80, 0xFC, 0x82, 0x82, 0xFC, // 'b'
	0x00, 0x00, 0x00, 0x00, 0x7C, 0x80, 0x80, 0x7C, // 'c'
	0x00, 0x02, 0x02, 0x02, 0x7E, 0x82, 0x82, 0x7E, // 'd'
	0x00, 0x00, 0x00, 0x7C, 0x82, 0xFE, 0x80, 0x7C, // 'e'
	0x00, 0x0E, 0x10, 0x10, 0x7E, 0x10, 0x10, 0x10, // 'f'
	0x00, 0x00, 0x7E, 0x82, 0x82, 0x7E, 0x02, 0x7C, // 'g'
	0x00, 0x80, 0x80, 0x80, 0xBC, 0xC2, 0x82, 0x82, // 'h'
	0x00, 0x10, 0x10, 0x00, 0x30, 0x10, 0x10, 0x38, // 'i'
	0x00, 0x08, 0x08, 0x00, 0x08, 0x08, 0x88, 0x70, // 'j'
	0x00, 0x40, 0x40, 0x44, 0x58, 0x60, 0x50, 0x4C, // 'k'
	0x00, 0x20, 0x10, 0x10, 0x10, 0x10, 0x10, 0x08, // 'l'
	0x00, 0x00, 0x00, 0xEC, 0x92, 0x92, 0x92, 0x92, // 'm'
	0x00, 0x00, 0x00, 0xBC, 0xC2, 0x82, 0x82, 0x82, // 'n'
	0x00, 0x00, 0x00, 0x7C, 0x82, 0x82, 0x82, 0x7C, // 'o'
	0x00, 0x00, 0x00, 0xFC, 0x82, 0x82, 0xFC, 0x80, // 'p'
	0x00, 0x00, 0x00, 0x7E, 0x82, 0x82, 0x7E, 0x02, // 'q'
	0x00, 0x00, 0x00, 0x5C, 0x60, 0x40, 0x40, 0x40, // 'r'
	0x00, 0x00, 0x00, 0x7E, 0x80, 0x7C, 0x02, 0xFC, // 's'
	0x00, 0x00, 0x10, 0x7C, 0x10, 0x10, 0x10, 0x08, // 't'
	0x00, 0x00, 0x00, 0x82, 0x82, 0x82, 0x82, 0x7E, // 'u'
	0x00, 0x00, 0x00, 0x82, 0x82, 0x44, 0x28, 0x10, // 'v'
	0x00, 0x00, 0x00, 0x92, 0x92, 0x92, 0x92, 0x6C, // 'w'
	0x00, 0x00, 0x00, 0xC6, 0x28, 0x10, 0x28, 0xC6, // 'x'
	0x00, 0x00, 0x00, 0x84, 0x84, 0x7C, 0x04, 0x78, // 'y'
	0x00, 0x00, 0x00, 0xFE, 0x04, 0x38, 0x40, 0xFE, // 'z'
	0x00, 0x18, 0x20, 0x20, 0x40, 0x20, 0x20, 0x18, // '{'
	0x00, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, // '|'
	0x00, 0x30, 0x08, 0x08, 0x04, 0x08, 0x08, 0x30, // '}'
	0x00, 0x00, 0x00, 0x20, 0x52, 0x94, 0x08, 0x00, // '~'
	0x00, 0x7E, 0x7E, 0x7E, 0x7E, 0x7E, 0x7E, 0x00, // ''
};

// --------------------------------------------------------------------
static const uint8_t sangria_logo1[] = {
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xC0, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0x80, 0x00, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF9, 0xFF, 0xFF, 0xFF, 
	0xFF, 0x1F, 0xFE, 0x3F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF0, 0xFF, 0xFF, 0xFF, 
	0xFE, 0x3F, 0xFF, 0x1F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xE0, 0x7F, 0xFF, 0xFF, 
	0xFE, 0x78, 0x07, 0x9F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xE6, 0x7F, 0xFF, 0xFF, 
	0xFE, 0x70, 0x03, 0xCF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xE6, 0x7F, 0xFF, 0xFF, 
	0xFE, 0x67, 0xF9, 0x8C, 0x00, 0x1E, 0x00, 0x07, 0xE0, 0x00, 0xE0, 0x00, 0x60, 0x70, 0x00, 0x7F, 
	0xFE, 0x60, 0x00, 0x18, 0x00, 0x0C, 0x00, 0x03, 0xC0, 0x00, 0x40, 0x00, 0x00, 0x60, 0x00, 0x3F, 
	0xFE, 0x70, 0x00, 0x71, 0xFF, 0xC4, 0xFF, 0xF1, 0x8F, 0xFE, 0x1F, 0xFF, 0x86, 0x47, 0xFF, 0x1F, 
	0xFE, 0x3F, 0xFE, 0x33, 0xFF, 0xE0, 0xFF, 0xF8, 0x9F, 0xFF, 0x1F, 0xFF, 0xE6, 0x4F, 0xFF, 0x8F, 
	0xFF, 0x1F, 0xFF, 0x33, 0x00, 0x70, 0xF0, 0x3C, 0x38, 0x07, 0x1E, 0x00, 0xE6, 0x4C, 0x01, 0xCF, 
	0xFF, 0x80, 0x07, 0x90, 0x00, 0x30, 0xE0, 0x1E, 0x30, 0x03, 0x1C, 0x00, 0x46, 0x40, 0x00, 0xCF, 
	0xFF, 0x00, 0x03, 0x93, 0xFF, 0xF0, 0xE3, 0x8E, 0x33, 0xF3, 0x1C, 0xFE, 0x06, 0x4F, 0xFF, 0xCF, 
	0xFE, 0x07, 0xF9, 0x93, 0xFF, 0xF0, 0xE7, 0xCE, 0x33, 0xF3, 0x1C, 0xFF, 0x26, 0x4F, 0xFF, 0xCF, 
	0xFC, 0x60, 0x01, 0x93, 0x00, 0x30, 0xE7, 0xCE, 0x30, 0x03, 0x1C, 0xFF, 0xE6, 0x4C, 0x00, 0xCF, 
	0xFC, 0xF0, 0x03, 0x93, 0x80, 0x70, 0xE7, 0xCE, 0x38, 0x07, 0x1C, 0xFF, 0xE6, 0x4E, 0x01, 0xCF, 
	0xFE, 0x7F, 0xFF, 0x93, 0xFF, 0xF0, 0xE7, 0xCE, 0x3F, 0xFF, 0x1C, 0xFF, 0xE6, 0x4F, 0xFF, 0xCF, 
	0xFE, 0x3F, 0xFF, 0x11, 0xFF, 0xF0, 0xE7, 0xCE, 0x1F, 0xFF, 0x1C, 0xFF, 0xE6, 0x47, 0xFF, 0xCF, 
	0xFF, 0x0F, 0xFE, 0x38, 0xFF, 0xB0, 0x47, 0xC4, 0x07, 0xF3, 0x08, 0xFF, 0xE6, 0x63, 0xFE, 0xCF, 
	0xFF, 0x80, 0x00, 0x7C, 0x00, 0x00, 0x07, 0xE0, 0xC0, 0x03, 0x00, 0xFF, 0xF0, 0xF0, 0x00, 0x0F, 
	0xFF, 0xE0, 0x01, 0xFE, 0x00, 0x06, 0x0F, 0xF1, 0x88, 0x07, 0x21, 0xFF, 0xF9, 0xF8, 0x00, 0x1F, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x9F, 0xFF, 0x3F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x9F, 0xFE, 0x3F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x87, 0xF8, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xC0, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF0, 0x03, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
};

// --------------------------------------------------------------------
static const uint8_t sangria_logo2[] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x1F, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x3F, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x78, 0x07, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 
	0x00, 0x70, 0x03, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 
	0x00, 0x60, 0x01, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x70, 0x00, 0x01, 0xFF, 0xC0, 0xFF, 0xF0, 0x0F, 0xFE, 0x1F, 0xFF, 0x86, 0x07, 0xFF, 0x00, 
	0x00, 0x3F, 0xFE, 0x03, 0xFF, 0xE0, 0xFF, 0xF8, 0x1F, 0xFF, 0x1F, 0xFF, 0xE6, 0x0F, 0xFF, 0x80, 
	0x00, 0x1F, 0xFF, 0x03, 0x00, 0x70, 0xF0, 0x3C, 0x38, 0x07, 0x1E, 0x00, 0xE6, 0x0C, 0x01, 0xC0, 
	0x00, 0x00, 0x07, 0x80, 0x00, 0x30, 0xE0, 0x1E, 0x30, 0x03, 0x1C, 0x00, 0x46, 0x00, 0x00, 0xC0, 
	0x00, 0x00, 0x03, 0x83, 0xFF, 0xF0, 0xE0, 0x0E, 0x30, 0x03, 0x1C, 0x00, 0x06, 0x0F, 0xFF, 0xC0, 
	0x00, 0x00, 0x01, 0x83, 0xFF, 0xF0, 0xE0, 0x0E, 0x30, 0x03, 0x1C, 0x00, 0x06, 0x0F, 0xFF, 0xC0, 
	0x00, 0x60, 0x01, 0x83, 0x00, 0x30, 0xE0, 0x0E, 0x30, 0x03, 0x1C, 0x00, 0x06, 0x0C, 0x00, 0xC0, 
	0x00, 0xF0, 0x03, 0x83, 0x80, 0x70, 0xE0, 0x0E, 0x38, 0x07, 0x1C, 0x00, 0x06, 0x0E, 0x01, 0xC0, 
	0x00, 0x7F, 0xFF, 0x83, 0xFF, 0xF0, 0xE0, 0x0E, 0x3F, 0xFF, 0x1C, 0x00, 0x06, 0x0F, 0xFF, 0xC0, 
	0x00, 0x3F, 0xFF, 0x01, 0xFF, 0xF0, 0xE0, 0x0E, 0x1F, 0xFF, 0x1C, 0x00, 0x06, 0x07, 0xFF, 0xC0, 
	0x00, 0x0F, 0xFE, 0x00, 0xFF, 0xB0, 0x40, 0x04, 0x07, 0xF3, 0x08, 0x00, 0x06, 0x03, 0xFE, 0xC0, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
};

// --------------------------------------------------------------------
const uint8_t *get_sangria_logo1( void ) {
	return sangria_logo1;
}

// --------------------------------------------------------------------
const uint8_t *get_sangria_logo2( void ) {
	return sangria_logo2;
}

// --------------------------------------------------------------------
const uint8_t *get_icon( SANGRIA_ICON_ID_T id ) {
	return p_icon[ id ];
}

// --------------------------------------------------------------------
const uint8_t *get_font( void ) {
	return font;
}
