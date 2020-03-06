

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
		
		tempB=x;
		tempD=x>>8;
		
		PORTB=tempB;
		PORTD=tempD;
		

    }
}

