// --------------------------------------------------------------------
//	PIO test
// ====================================================================
//	2022/9/15	t.hara
// --------------------------------------------------------------------

#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "pio_test.pio.h"

int main() {
	PIO pio = pio0;

	uint offset = pio_add_program( pio, &pio_test_program );
	uint sm = pio_claim_unused_sm( pio, true );
	pio_test_program_init( pio, sm, offset, 0 );

	while (true) {
		sleep_ms(500);
	}
}
