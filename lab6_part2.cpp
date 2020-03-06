
#include <avr/io.h>
#include <avr/interrupt.h>

volatile unsigned char TimerFlag=0;

unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr=0;

void TimerOn(){
	TCCR1B=0x0B;
	OCR1A=125;
	TIMSK1=0x02;
	TCNT1=0;
	_avr_timer_cntcurr=_avr_timer_M;
	SREG |= 0x80;
}

void TimerOff(){
	TCCR1B=0x00;
}

void TimerISR(){
	TimerFlag=1;
}

ISR(TIMER1_COMPA_vect){
	_avr_timer_cntcurr --;
	
	if( _avr_timer_cntcurr==0){
		TimerISR();
		_avr_timer_cntcurr= _avr_timer_M;
	}
}

void TimerSet(unsigned long M ){
	_avr_timer_M=M;
	_avr_timer_cntcurr = _avr_timer_M;
}

enum STATES{ start, pb0, pb1, pb2, firstP, firstR, secondP, secondR  } state;
	
unsigned char tempC;
unsigned char currentState;
unsigned char tempState;

void Tick(){
	
	
	unsigned char tempA=~PINA & 0x01;
	
	switch (state){
		
		case start: tempC=0x00; state=pb0; currentState= start; break;
		case pb0: 
					tempC=0x01; 
		            
					if ( tempA==0x01 ){
						state= firstP;
						tempState=pb0;
						currentState= start;
					}else {
						state=pb1;
					}
					
					currentState=pb0;
					
					break;
					
		case pb1: 
					tempC=0x02;
		
					if ( tempA==0x01 ){
						state= firstP;
						tempState=pb1;
						//currentState=pb1;
						}else {
						switch( currentState ){
							case pb0: state = pb2; break;
							case pb2: state = pb0; break;
							default: break;
						}
					}
		
					//currentState=pb1;
					
					break;
					
		case pb2: 
					tempC=0x04;
					
					if ( tempA==0x01 ){
						state= firstP;
						tempState=pb2;
						currentState=pb2;
					}else {
						state = pb1;
					}
					
					currentState=pb2;
					
					break;
					
		
		
		
		case firstP:    state=( tempA==0x01 ) ? firstP : firstR ; break;
		case firstR:    state=( tempA==0x00 ) ? firstR : secondP ; break;
		//case secondP:   state=( tempA==0x01 ) ? secondP : secondR ; break;
		case secondP:   
						if( tempA==1 ){
							state=secondP;
						}else{
							switch(tempState){
								case pb0: state=pb0; break;
								case pb1: state=pb1; break;
								case pb2: state=pb2; break;
								default: break;
							}
						}
						break;
		
		
		default: break;
	}
	
	PORTC=tempC;
}

int main()
{
    /* Replace with your application code */

	DDRA=0x00; PORTA=0xFF;
	DDRC=0xFF; PORTC=0x00;
	
	TimerSet(300);   
	TimerOn(); 
	state=start;
	//unsigned char tempC=0x00;
	
    while (1) 
    {
		
		//tempC= ~tempC;
		//PORTC =tempC;
		
		Tick();
		while(!TimerFlag);
		TimerFlag=0;
    }
}

