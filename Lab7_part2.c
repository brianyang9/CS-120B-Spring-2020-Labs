
#include <avr/io.h>
#include "io.h"
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
	
unsigned char tempB;   // LED lights
unsigned char currentState;		// stores currentState for regular cycle of LED-turning-On-and-Off process, which does not stores a state when the middle LED turns on
unsigned char tempState;	// stores current state's value within regular cycle of LED-turning-On-and-Off process, the main difference between this and currentState is this also stores a state when the middle LED turns on

unsigned char score=8;

void Tick(){
	
	
	unsigned char tempA=~PINA; // & 0x01;
	
	switch (state){
		
		case start: tempB=0x00; state=pb0; currentState= start; break;
		case pb0: 
					LCD_ClearScreen();
					
					tempB=0x01; 
		            
					if ( tempA==0x01 ){
						
						//LCD_DisplayString(1, "Next one.");
						
						--score;
						
						tempState=pb0;
						
						state= firstP;
						
						//currentState= start;
						
					}else {
						state=pb1;
					}
					
					currentState=pb0;
					
					break;
					
		case pb1: 
					LCD_ClearScreen();
					
					tempB=0x02;
		
					if ( tempA==0x01 ){
						
						++score;
						//LCD_DisplayString(1, "correct!");
						if ( score==9 ){

							LCD_DisplayString(1, "Winner!");
							
						}
						
						tempState=pb1;
						
						state= firstP;
						
						//currentState=pb1;
						
					}else {
						
						switch( currentState ){				// over here, currentState stores previous state value
							case pb0: state = pb2; break;
							case pb2: state = pb0; break;
							default: break;
						}
						
					}
		
					//currentState=pb1;
					
					break;
					
		case pb2: 
					LCD_ClearScreen();

					tempB=0x04;
					
					if ( tempA==0x01 ){
						
						//LCD_DisplayString(1, "Previous one.");
						--score;
						
						tempState=pb2;
						
						state= firstP;
						
						//currentState=pb2;
					}else {
						state = pb1;
					}
					
					currentState=pb2;
					
					break;
					
		
		
		
		case firstP:    state=( tempA==0x01 ) ? firstP : firstR ; 
						
						break;
		
		
		case firstR:    state=( tempA==0x00 ) ? firstR : secondP ; 

		
						break;

		case secondP:   
						
						if( tempA==1 ){
							state=secondP;
						}else{
							switch(tempState){				// over here, tempState stores previous state value
								case pb0: state=pb0; break;      
								case pb1: state=pb1; break;
								case pb2: state=pb2; break;
								default: break;
							}
						}
						break;
		
		
		default: break;
	}
	
	if ( score!=9 ){
		
		LCD_Cursor(1);
		LCD_WriteData( score + '0');
	}
	PORTB=tempB;
}

int main()
{
    /* Replace with your application code */
	DDRA = 0x00; PORTA = 0xFF;
	DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0xFF; PORTD = 0x00;

	DDRB=0xFF; PORTC=0x00;
	
	
	LCD_init();
	
	TimerSet(200);   
	TimerOn(); 
	
	
	state=start;
	//unsigned char tempC=0x00;
	
    while (1) 
    {
		
		
		Tick();
		while(!TimerFlag);
		TimerFlag=0;
    }
}

