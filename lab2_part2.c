
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs
	DDRC = 0xFF; PORTC = 0x00; // Configure port B's 8 pins as outputs, initialize to 0s
	unsigned char tmpPA0 = 0x00; // Temporary variable to hold the value of B
	unsigned char tmpPA1 = 0x00; // Temporary variable to hold the value of A
	unsigned char tmpPA2 = 0x00;
	unsigned char tmpPA3 = 0x00;

    /* Insert your solution below */
    while(1) {
		
		// 1) According to the input to identify how many cars in the parking lot
		// *******************************************    WARNING    **********************************************************************************
		// tmpPA0 = PINA & 0x01;   WRONG! "&" does not do logical comapring, for example, if PINA is 0x08, then PINA&0x08 equals 0x08 not True or 0x01
		// ********************************************************************************************************************************************
		tmpPA0 = (PINA & 0x01) ? 1 : 0;
		tmpPA1 = (PINA & 0x02) ? 1 : 0;
		tmpPA2 = (PINA & 0x04) ? 1 : 0;
		tmpPA3 = (PINA & 0x08) ? 1 : 0;
		
		// 2) Perform computation and Write output

		PORTC= 4 - (tmpPA0 + tmpPA1 + tmpPA2 + tmpPA3);

}

    return 1;
}