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

#ifndef __SANGRIA_FIRMWARE_CONFIG_H__
#define __SANGRIA_FIRMWARE_CONFIG_H__

#define SANGRIA_VERSION			1		//	0:RC1, 1:RC3

// --------------------------------------------------------------------
//	GPIO PIN defines: keyboard/buttons
//
#define SANGRIA_BACK			23
#define SANGRIA_JOG_A			3
#define SANGRIA_JOG_B			4
#define SANGRIA_JOG_PUSH		5
#define SANGRIA_COL1			18
#define SANGRIA_ROW1			6

#define SANGRIA_JOG_MIDIFY( a )		a

// --------------------------------------------------------------------
//	GPIO PIN defines: keyboard back light device
//
#define SANGRIA_BACK_LIGHT		13

// --------------------------------------------------------------------
//	GPIO PIN defines: I2C connection ports
//
#define SANGRIA_I2C0			i2c0
#define SANGRIA_I2C0_CLOCK		(100 * 1000)
#define SANGRIA_I2C0_SCL		1
#define SANGRIA_I2C0_SDA		0

#define SANGRIA_I2C1			i2c1
#define SANGRIA_I2C1_CLOCK		(400 * 1000)
#define SANGRIA_I2C1_SCL		27
#define SANGRIA_I2C1_SDA		26

// --------------------------------------------------------------------
//	GPIO PIN defines: OLED display device
//
#define SANGRIA_SSD1306			0
#define SANGRIA_SSD1316			1

#define SANGRIA_OLED_I2C		SANGRIA_I2C1

#define SANGRIA_OLED_ON_N		14
#define SANGRIA_OLED_RST_N		24
#define SANGRIA_OLED_DRIVER		SANGRIA_SSD1316

#define OLED_WIDTH				128
#define OLED_HEIGHT				32

#define OLED_POWER_ON			0
#define OLED_POWER_OFF			1

// --------------------------------------------------------------------
//	GPIO PIN defines: Battery controller device
#define SANGRIA_BQ_I2C			SANGRIA_I2C0

#define SANGRIA_BQ_INT			17
#define SANGRIA_BQ_OTG			16
#define SANGRIA_BQ_CE_N			2
#define SANGRIA_BATTERY_ADC		29

// --------------------------------------------------------------------
//	GPIO PIN defines: keyboard/buttons
//	[!] NOT CHANGE!
//
#define SANGRIA_COL2			(SANGRIA_COL1 + 1)
#define SANGRIA_COL3			(SANGRIA_COL2 + 1)
#define SANGRIA_COL4			(SANGRIA_COL3 + 1)
#define SANGRIA_COL5			(SANGRIA_COL4 + 1)
#define SANGRIA_ROW2			(SANGRIA_ROW1 + 1)
#define SANGRIA_ROW3			(SANGRIA_ROW2 + 1)
#define SANGRIA_ROW4			(SANGRIA_ROW3 + 1)
#define SANGRIA_ROW5			(SANGRIA_ROW4 + 1)
#define SANGRIA_ROW6			(SANGRIA_ROW5 + 1)
#define SANGRIA_ROW7			(SANGRIA_ROW6 + 1)

#endif
