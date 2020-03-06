

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



enum STATES { start, sound, on_press, button_press } state;

unsigned char button_on=0;
double sound_arr[9]={0, 261.63, 293.66, 329.63, 349.23, 392.00, 440.00, 493.88, 523.25 };
unsigned char index=0;

void Tick(){
	
	unsigned char button1=~PINA & 0x01;
	unsigned char button2=~PINA & 0x02;
	unsigned char button3=~PINA & 0x04;
	
	
	switch (state){
		
		case start:	
							state = sound;
							break;
					
		case sound:			
							set_PWM( sound_arr[index] );
							
							if( button1 ){
								state=on_press;
								button_on=(button_on==0) ? 1 : 0;
								
								if ( button_on==0 ){
									index=0;
								}
								
								
							}else if ( button_on==1 && ( button2 || button3 ) ) {
								state=button_press;
								
								if( button2 && ( index >-1 && index < 9 )) {
									++index;
									
									if ( index >=9 ){
										index=8;
									}
								}
								
								if ( button3 && ( index >-1 && index < 9 )) {
									--index;
									
									if ( index <= 0 ){
										index=0;
									}
								}
								
								
							}else{
								state=sound;
							}
							
							break;
					
		case on_press:		
							if( button1 ){
								state=on_press;
							} else {
								state=sound;
							}
		
							break;			
					
		case button_press:	
							if( button_on==1 && ( button2 || button3 ) ){
								state=button_press;
							}else if ( button_on==1 && ( !button2 || !button3 ) ){
								state=sound;
							}
							break;
					
		default: 
					state=sound;
					break;
		}

}

int main(void)
{
    /* Replace with your application code */
	DDRA=0x00; PORTA=0xFF;
	DDRB=0XFF; PORTB=0x00;

	PWM_on();
	
    while (1) 
    {
		Tick();
    }
}

