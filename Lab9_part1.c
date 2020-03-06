
#include <avr/io.h>



void set_PWM(double frequency){
	static double current_frequency;

	if(frequency != current_frequency){
		if(!frequency) {TCCR3B &= 0x08;}
		else{TCCR3B |= 0x03;}

		if(frequency < 0.954){OCR3A = 0xFFFF;}

		else if(frequency > 31250) {OCR3A = 0x0000;}

		else {OCR3A = (short)(8000000 / (128*frequency))-1;}

		TCNT3 = 0;
		current_frequency = frequency;
	}

}
		
			


void PWM_on(){
	
	TCCR3A = ( 1 << COM3A0 );
	
	TCCR3B = ( 1 << WGM32 ) | ( 1 << CS31 ) | ( 1 << CS30 );
	
	set_PWM(0);
}

void PWM_off(){
	TCCR3A=0x00;
	TCCR3B=0x00;
}



enum STATES{ start, init, button1, button2, button3  } state;
	
void Tick(){
	
	unsigned char button1_press=~PINA & 0x01;
	unsigned char button2_press=~PINA & 0x02;
	unsigned char button3_press=~PINA & 0x04;
	
	switch (state){
		
		case start:	
					state = init;
					break;
					
		case init: 
					set_PWM( 0 );
					
					if ( !button1_press && !button2_press && !button3_press ){
						state=init;
					} else if ( button1_press && !button2_press && !button3_press ){
						state=button1;
					} else if ( button2_press && !button1_press && !button3_press ){
						state=button2;
					} else if ( button3_press && !button1_press && !button2_press ){
						state=button3;
					} else {
						state=init;
					}
					
					break;
					
		case button1:	
					set_PWM( 261.63 );
					
					if ( button1_press && !button2_press && !button3_press ){
						state=button1;
					} else {
						state=init;
					}
					
					break;
					
		case button2:
					set_PWM( 293.66 );
							
					if ( button2_press && !button1_press && !button3_press ){
						state=button2;
						} else {
						state=init;
					}
				
					break;			
					
		case button3:
					set_PWM( 329.63 );
							
					if ( button3_press && !button1_press && !button2_press ){
						state=button3;
						} else {
						state=init;
					}
		
					break;
					
		default: 
					state=init;
					break;
		}

}

int main(void)
{
    /* Replace with your application code */
	DDRA = 0x00; PORTA = 0xFF;
	DDRB=0XFF;
	PORTB=0x00;

	PWM_on();
    while (1) 
    {
		Tick();
    }
}

