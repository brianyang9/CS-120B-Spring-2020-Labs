
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs
	DDRB = 0xFF; PORTB = 0x00; // Configure port B's 8 pins as outputs, initialize to 0s
	unsigned char tmpPA0 = 0x00; // Temporary variable to hold the value of B
	unsigned char tmpPA1 = 0x00; // Temporary variable to hold the value of A

    /* Insert your solution below */
    while(1) {
		// 1) Read input
		tmpPA0 = PINA & 0x01;
		tmpPA1 = PINA & 0x02;
		// 2) Perform computation and Write output
		PORTB=(!tmpPA1&&tmpPA0) ? 1 : 0;
			


}

    return 1;
}