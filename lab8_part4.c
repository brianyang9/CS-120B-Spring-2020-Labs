
#include <avr/io.h>

void ADC_init(){
	ADCSRA |= ( 1 << ADEN ) | ( 1 << ADSC ) | ( 1 << ADATE );
}

int main(void)
{
    /* Replace with your application code */
	
	DDRA=0x00; PORTA=0xFF;
	DDRB=0xFF; PORTB=0x00;
	DDRD=0xFF; PORTD=0x00;
	
	ADC_init();
	
	unsigned char tempB=0;
	unsigned char tempD=0;
	
    while (1) 
    {
		unsigned short x=ADC;

	

	if(x < 67) {
		PORTB = 0;
	}else if(x >= 67 && x < 134) {
		PORTB = 1;
	}else if(x >= 134 && x < 201) {
		PORTB = 3;
	}else if(x >= 201 && x < 268) {
		PORTB = 7;
	}else if(x >= 268 && x < 335) {
		PORTB = 15;
	}else if(x >= 335 && x < 402) {
		PORTB = 31;
	}else if(x >= 402 && x < 469) {
		PORTB = 63;
	}else if(x >= 469 && x < 536) {
		PORTB = 127;
	}else if(x >=536) {
		PORTB = 255;
	}else {
		PORTB = 0;
	}
		
		
    }
}

