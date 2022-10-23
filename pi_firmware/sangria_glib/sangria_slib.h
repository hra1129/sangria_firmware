// --------------------------------------------------------------------
// Sangria game library
// ====================================================================
//	Copyright 2022 t.hara
//
//	Permission is hereby granted, free of charge, to any person obtaining 
//	a copy of this software and associated documentation files (the "Software"), 
//	to deal in the Software without restriction, including without limitation 
//	the rights to use, copy, modify, merge, publish, distribute, sublicense, 
//	and/or sell copies of the Software, and to permit persons to whom the 
//	Software is furnished to do so, subject to the following conditions:
//
//	The above copyright notice and this permission notice shall be included in 
//	all copies or substantial portions of the Software.
//
//	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, 
//	EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF 
//	MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. 
//	IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, 
//	DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, 
//	ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
//	DEALINGS IN THE SOFTWARE.
// --------------------------------------------------------------------
//	Require:
//		sudo apt-get install libpulse-dev
//		compile options: -lpulse -lpulse-simple

#ifndef __SANGRIA_SLIB_H__
#define __SANGRIA_SLIB_H__

#include <psg_emulator.h>
#include <scc_emulator.h>

#ifdef __cplusplus
extern "C" {
#endif

// --------------------------------------------------------------------
//	sangria_sound_initialize
//	input)
//		none
//	output)
//		0 ...... Failed (Sound device is not found.)
//		!0 ..... Success
// --------------------------------------------------------------------
int sangria_sound_initialize( void );

// --------------------------------------------------------------------
//	sangria_sound_terminate
//	input)
//		none
//	output)
//		none
// --------------------------------------------------------------------
void sangria_sound_terminate( void );

// --------------------------------------------------------------------
//	sangria_get_psg_handle
//	input)
//		none
//	output)
//		PSG handle
// --------------------------------------------------------------------
H_PSG_T sangria_get_psg_handle( void );

// --------------------------------------------------------------------
//	sangria_get_psg_se_handle
//	input)
//		none
//	output)
//		PSG handle
// --------------------------------------------------------------------
H_PSG_T sangria_get_psg_se_handle( void );

// --------------------------------------------------------------------
//	sangria_get_scc_handle
//	input)
//		none
//	output)
//		SCC handle
// --------------------------------------------------------------------
H_SCC_T sangria_get_scc_handle( void );

#ifdef __cplusplus
}
#endif

#endif
