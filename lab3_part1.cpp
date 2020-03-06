
#include <avr/io.h>

// Bit-access function
unsigned char SetBit(unsigned char x, unsigned char k, unsigned char b) {
	return (b ? x | (0x01 << k) : x & ~(0x01 << k));
}
unsigned char GetBit(unsigned char x, unsigned char k) {
	return ((x & (0x01 << k)) != 0);
}
unsigned char CountingOneBits( unsigned char input){
	unsigned char i;
	unsigned char onesAmount=0;
	for (i=0;i<=7;++i){
		onesAmount=onesAmount+GetBit(input, i);
	}
	return onesAmount;
}


int main(void)
{
	// set PORTs
	DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs
	DDRB = 0x00; PORTB = 0xFF; // Configure port A's 8 pins as inputs
	DDRC = 0xFF; PORTC = 0x00; // Configure port B's 8 pins as outputs
	
	// initialize to 0s
	unsigned char tempPINA=0x00;
	unsigned char tempPINB=0x00;
	unsigned char counter = 0x00; // intermediate variable used for amount of 1 bits

	while(1)
	{
		//1) copy inputs to variables
		tempPINA=PINA;
		tempPINB=PINB;
		
		//2) call function to calculate the total of 1-bits in each input
		counter = CountingOneBits( tempPINA )+CountingOneBits( tempPINB );
		
		//3) write into the output
		PORTC = counter;
	}
}
