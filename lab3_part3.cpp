

#include <avr/io.h>

// Bit-access function
unsigned char SetBit(unsigned char x, unsigned char k, unsigned char b) {
	return (b ? x | (0x01 << k) : x & ~(0x01 << k));
}
unsigned char GetBit(unsigned char x, unsigned char k) {
	return ((x & (0x01 << k)) != 0);
}

int main(void)
{
	// set PORTs
	DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs
	DDRC = 0xFF; PORTC = 0x00; // Configure port B's 8 pins as outputs
	
	// initialize to 0s
	unsigned char fuel_level =0x00;
	unsigned char safety_condition=0x00;

	while(1)
	{
		
		fuel_level=PINA&0x0F;    
		safety_condition=PINA;

		
		switch(fuel_level){
			case 0: PORTC=(PORTC&0x80)|0x40; break;
			case 1:
			case 2: PORTC=(PORTC&0x80)|0x60; break;
			case 3: 
			case 4: PORTC=(PORTC&0x80)|0x70; break;
			case 5:
			case 6: PORTC=(PORTC&0x80)|0x38; break;
			case 7:
			case 8:
			case 9: PORTC=(PORTC&0x80)|0x3C; break;
			case 10:
			case 11:
			case 12: PORTC=(PORTC&0x80)|0x3E; break;
			case 13:
			case 14:
			case 15: PORTC=(PORTC&0x80)|0x3F; break;
			default: break;
			//PORTC=(PORTC&0x00)|0xFF; break;
		}
		//tempPORTC=PORTC;
		PORTC=( GetBit( safety_condition, 4 ) && GetBit( safety_condition, 5 ) && !GetBit( safety_condition, 6 ) ) ? PORTC | 0x80 : PORTC & 0x7F;
		
		
		
	}
}

