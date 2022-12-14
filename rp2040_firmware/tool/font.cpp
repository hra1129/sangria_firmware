#include <cstdint>

int font_width  = 768;
int font_height = 8;
const uint8_t font[] = {
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
