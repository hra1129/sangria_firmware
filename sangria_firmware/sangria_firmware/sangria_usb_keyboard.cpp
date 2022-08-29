// --------------------------------------------------------------------
//	The MIT License (MIT)
//	
//	Sangria firmware USB keybard device driver
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

#include "bsp/board.h"
#include "tusb.h"
#include "usb_descriptors.h"
#include "sangria_usb_keyboard.h"

#define C_FUNC extern "C"

// --------------------------------------------------------------------
//	デバイスがマウントされたときに呼び出される
//	Invoked when device is mounted.
C_FUNC void tud_mount_cb( void ) {
}

// --------------------------------------------------------------------
//	デバイスがアンマウントされたときに呼び出される
//	Invoked when device is unmounted
C_FUNC void tud_umount_cb( void ) {
}

// --------------------------------------------------------------------
//	USB-BUS がサスペンドされたときに呼び出される
//	Invoked when usb bus is suspended
//	remote_wakeup_en : if host allow us  to perform remote wakeup
//	Within 7ms, device must draw an average of current less than 2.5 mA from bus
C_FUNC void tud_suspend_cb( bool remote_wakeup_en ) {
}

// --------------------------------------------------------------------
//	USB-BUS がレジュームされたときに呼び出される
//	Invoked when usb bus is resumed
C_FUNC void tud_resume_cb( void ) {
}

//--------------------------------------------------------------------+
// USB HID
//--------------------------------------------------------------------+
static void send_hid_report( uint8_t report_id, CSANGRIA_KEYBOARD &keyboard ) {
	// skip if hid is not ready yet
	if(  !tud_hid_ready() ) {
		return;
	}

	// use to avoid send multiple consecutive zero report for keyboard
	static bool has_keyboard_key = false;

	uint8_t keycode[6];
	int index = keyboard.update( keycode );

	if( index ) {
		if( tud_suspended() ) {
			//	サスペンドモードの時は、ホストをウェイクアップして
			//	REMOTE_WAKEUP を有効にする。
			tud_remote_wakeup();
		}
		else {
			tud_hid_keyboard_report( REPORT_ID_KEYBOARD, 0, keycode );
			has_keyboard_key = true;
		}
	}
	else {
		// send empty key report if previously has key pressed
		if( has_keyboard_key ) {
			tud_hid_keyboard_report( REPORT_ID_KEYBOARD, 0, NULL );
		}
		has_keyboard_key = false;
	}
}

// --------------------------------------------------------------------
// 10msごとに、各HIDプロファイル(キーボード、マウスなど)について1つのレポートを送信します。
// tud_hid_report_complete_cb() は、前のレポートが完了した後、次のレポートを送信するために使用されます。
// Every 10ms, we will sent 1 report for each HID profile (keyboard, mouse etc ..)
// tud_hid_report_complete_cb() is used to send the next report after previous one is complete

//void hid_task( CSANGRIA_KEYBOARD &keyboard, CSANGRIA_JOGDIAL &jogdial ) {
void hid_task( CSANGRIA_KEYBOARD &keyboard ) {
	// Poll every 10ms
	const uint32_t interval_ms = 10;
	static uint32_t start_ms = 0;

	if(  board_millis() - start_ms < interval_ms) {
		return; // not enough time
	}
	start_ms += interval_ms;
	send_hid_report( REPORT_ID_KEYBOARD, keyboard );
}

// --------------------------------------------------------------------
// REPORTが正常にホストに送信されたときに呼び出されます。
// アプリケーションは次のレポートを送信するためにこれを使用することができます
// 注：複合レポートの場合、report[0]はレポートIDです。
// Invoked when sent REPORT successfully to host
// Application can use this to send the next report
// Note: For composite reports, report[0] is report ID
C_FUNC void tud_hid_report_complete_cb( uint8_t instance, uint8_t const* report, uint8_t len ) {
	(void) instance;
	(void) len;

	uint8_t next_report_id = report[0] + 1;

	if( next_report_id < REPORT_ID_COUNT) {
		//send_hid_report(next_report_id, board_button_read(), *p_keyboard );
	}
}

// --------------------------------------------------------------------
// GET_REPORT 制御要求を受信したときに呼び出される。
// アプリケーションはレポートの内容をバッファに詰め、その長さを返さなければならない。
// 0 を返すとスタックが STALL リクエストになります。
// Invoked when received GET_REPORT control request
// Application must fill buffer report's content and return its length.
// Return zero will cause the stack to STALL request
C_FUNC uint16_t tud_hid_get_report_cb( uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t* buffer, uint16_t reqlen ) {
	// TODO not Implemented
	(void) instance;
	(void) report_id;
	(void) report_type;
	(void) buffer;
	(void) reqlen;

	return 0;
}

// --------------------------------------------------------------------
// SET_REPORT 制御要求を受信したとき、または OUT 側でデータを受信したときに呼び出される。
// OUTエンドポイントにデータを受信したとき ( レポートID = 0, タイプ = 0 )
// Invoked when received SET_REPORT control request or
// received data on OUT endpoint ( Report ID = 0, Type = 0 )
C_FUNC void tud_hid_set_report_cb( uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t const* buffer, uint16_t bufsize ) {
	(void) instance;

	if( report_type == HID_REPORT_TYPE_OUTPUT ) {
		// Set keyboard LED e.g Capslock, Numlock etc...
		if( report_id == REPORT_ID_KEYBOARD ) {
			// bufsize should be (at least) 1
			if(  bufsize < 1 ) {
				return;
			}

			uint8_t const kbd_leds = buffer[0];

			if( kbd_leds & KEYBOARD_LED_CAPSLOCK ) {
				// Capslock On: disable blink, turn led on
			}
			else {
				// Caplocks Off: back to normal blink
			}
		}
	}
}
