
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














enum Buttons_States { B_start, Release_state, Press_PINA0, Press_PINA1, Reset } buttons_state;
unsigned char output=0;
unsigned char pressing_counter=0;

void Tick()
{
	unsigned char tmpPINA = ~PINA;
	switch(buttons_state)
	{
		case B_start:
		
		PORTC=0x02;
		buttons_state=Release_state;
		
		LCD_Cursor(1);
		LCD_WriteData(output+'0');
		
		break;
		
		case Release_state:
							pressing_counter=0;
							
							if( tmpPINA==0x00 ){
								buttons_state=Release_state;
			
								}else if ( tmpPINA==0x01 ) {
			
								buttons_state=Press_PINA0;

			
								}else if ( tmpPINA==0x02 ) {
								buttons_state=Press_PINA1;
			

			
								}else if (tmpPINA==0x03 ) {
								output=0;
								buttons_state=Reset;
								}else {
								// do nothing to stay in this state
								// In case if PORTA has unexpected value such as 0x70
							}
							break;
		
		case Press_PINA0:
						if( pressing_counter==0){
							if( output<9 ){
								++output;
								LCD_Cursor(1);
								LCD_WriteData(output+'0');
							}
						} 
						
						if( tmpPINA==0x01 ){
			
							buttons_state=Press_PINA0;
							pressing_counter++;
							if(pressing_counter>=9) pressing_counter=0; //pressing_counter=(pressing_counter<10) ? pressing_counter++ : 0;
			
						} else if ( tmpPINA==0x00 ) {
							buttons_state=Release_state;
						} else if( tmpPINA==0x03 ) {
							buttons_state=Reset;
							output=0;
						} else {
							// do nothing to stay in this state
							// In case if PORTA has unexpected value such as 0x70
						}
						

						break;
		
		case Press_PINA1:
						if( pressing_counter==0){
							if(output!=0){
								--output;
								LCD_Cursor(1);
								LCD_WriteData(output+'0');
							}
						} 
						
						if( tmpPINA==0x02 ){
			
							buttons_state=Press_PINA1;
							pressing_counter++;
							if(pressing_counter>=9)   pressing_counter=0;    //pressing_counter=(pressing_counter<10) ? pressing_counter++ : 0;   "pressing_counter++ causes the problem, maybe related to copiler
							
						} else if ( tmpPINA==0x00 ) {
							buttons_state=Release_state;
						} else if( tmpPINA==0x03 ) {
							buttons_state=Reset;
							output=0;
						} else {
							// do nothing to stay in this state
							// In case if PORTA has unexpected value such as 0x70
						}
						

						
						break;
		
		case Reset:
					if( tmpPINA==0x00 ){
						buttons_state=Release_state;
						output=0x00;
						LCD_Cursor(1);
						LCD_WriteData(output+'0');
						}else{
						//stay here do nothing, because human CANNOT release button simultaneously
					}
					
					break;
		
		default:			output=0x0F;  // turn on both LED lights to show unexpected situation
		break;
	}
}




int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
    DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
	
	buttons_state=B_start;
	
	// Initialize LCD Display
	LCD_init();

	TimerSet(100);
	TimerOn();
	buttons_state=B_start;
	
	// Starting at position 1 on the LCD screen, writes Hello World
	//LCD_DisplayString(1, "Hello World.");
	
    while (1) 
    {
		Tick();
		while(!TimerFlag);
		TimerFlag=0;
    }
}

