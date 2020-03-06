


#include <avr/io.h>
#include <avr/interrupt.h>
#include "timer.h"

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

unsigned char ThreeLEDs_value=0x00;

enum ThreeLEDs_STATES { ThreeLEDs_start, b0, b1, b2 } ThreeLEDs_state;
	
int Tick_ThreeLEDs(int ThreeLEDs_state) {
	switch(ThreeLEDs_state){
		case ThreeLEDs_start:	ThreeLEDs_state=b0; break;
		case b0:	ThreeLEDs_state=b1; ThreeLEDs_value=0x01; break;
		case b1:	ThreeLEDs_state=b2; ThreeLEDs_value=0x02; break;
		case b2:	ThreeLEDs_state=b0; ThreeLEDs_value=0x04; break;
		default:	ThreeLEDs_state=ThreeLEDs_start; break;
	}
	return ThreeLEDs_state;
}





unsigned char BlinkingLEDs_value=0x00;

enum BlinkingLEDs_STATES { BlinkingLEDs_start, b3_on, b3_off } BlinkingLEDs_state;

int Tick_BlinkingLEDs(int BlinkingLEDs_state) {
	switch(BlinkingLEDs_state){
		case BlinkingLEDs_start:	BlinkingLEDs_state=b3_on; break;
		case b3_on:	BlinkingLEDs_state=b3_off; BlinkingLEDs_value=0x08;	break;
		case b3_off: BlinkingLEDs_state=b3_on; BlinkingLEDs_value=0x00; break;
		default:	BlinkingLEDs_state=BlinkingLEDs_start; break;
	}
	return BlinkingLEDs_state;
}





enum CombineLEDs_STATES { CombineLEDs_start, light_up_LEDs } CombineLEDs_state;

int Tick_CombineLEDs(int CombineLEDs_state) {
	
	unsigned char output_B=0;
	
	switch(CombineLEDs_state){
		case CombineLEDs_start:	CombineLEDs_state=light_up_LEDs; break;
		case light_up_LEDs:	//BlinkingLEDs_state=light_up_LEDs;   why adding this will cause problem?
							output_B = ThreeLEDs_value | BlinkingLEDs_value;	break;
		default:	CombineLEDs_state=CombineLEDs_start; break;
	}
	
	PORTB=output_B;
	return CombineLEDs_state;
}

enum SPEAKER_STATES { start, sound, on_press, button_press } Speaker_State;

unsigned char button_on=0;
double sound_arr[9]={0, 261.63, 293.66, 329.63, 349.23, 392.00, 440.00, 493.88, 523.25 };
unsigned char index=0;

int Speaker_Tick(int Speaker_State){
	
	unsigned char button1=~PINA & 0x01;
	unsigned char button2=~PINA & 0x02;
	unsigned char button3=~PINA & 0x04;
	
	
	switch (Speaker_State){
		
		case start:
		Speaker_State = sound;
		break;
		
		case sound:
		set_PWM( sound_arr[index] );
		
		if( button1 ){
			Speaker_State=on_press;
			button_on=(button_on==0) ? 1 : 0;
			
			if ( button_on==0 ){
				index=0;
			}
			
			
			}else if ( button_on==1 && ( button2 || button3 ) ) {
			Speaker_State=button_press;
			
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
			Speaker_State=sound;
		}
		
		break;
		
		case on_press:
		if( button1 ){
			Speaker_State=on_press;
			} else {
			Speaker_State=sound;
		}
		
		break;
		
		case button_press:
		if( button_on==1 && ( button2 || button3 ) ){
			Speaker_State=button_press;
			}else if ( button_on==1 && ( !button2 || !button3 ) ){
			Speaker_State=sound;
		}
		break;
		
		default:
		Speaker_State=sound;
		break;
	}
	return Speaker_State;
}



// -----------------------------------------------------------------------------------------------------------------------------------




typedef struct task {
	int state;                  // Task's current state
	unsigned long period;       // Task period
	unsigned long elapsedTime;  // Time elapsed since last task tick
	int (*TickFct)(int);        // Task tick function
} task;

task tasks[4];
const unsigned short tasksNum = 4;


int main(void)
{
    /* Replace with your application code */
   /* Replace with your application code */
   DDRA=0x00; PORTA=0xFF;
   DDRB=0XFF; PORTB=0x00;

   
   TimerSet(2);
   TimerOn();
   
   

   
    unsigned char i = 0;
    tasks[i].state = ThreeLEDs_start;
    tasks[i].period = 300;
    tasks[i].elapsedTime = tasks[i].period;
    tasks[i].TickFct = &Tick_ThreeLEDs;
    i++;
	
    tasks[i].state = BlinkingLEDs_start;
    tasks[i].period = 1000;
    tasks[i].elapsedTime = tasks[i].period;
    tasks[i].TickFct = &Tick_BlinkingLEDs;
	i++;
	
	tasks[i].state = start;
    tasks[i].period = 2;
    tasks[i].elapsedTime = tasks[i].period;
    tasks[i].TickFct = &Speaker_Tick;
	i++;
    
	tasks[i].state = CombineLEDs_start;
	tasks[i].period = 2;
	tasks[i].elapsedTime = tasks[i].period;
	tasks[i].TickFct = &Tick_CombineLEDs;
	
	PWM_on();
	
	
   while (1)
   {
		
		for (i = 0; i < tasksNum; ++i) { // Heart of the scheduler code
			if ( tasks[i].elapsedTime >= tasks[i].period ) { // Ready
				tasks[i].state = tasks[i].TickFct(tasks[i].state);
				tasks[i].elapsedTime = 0;
			}
			tasks[i].elapsedTime += 2;
		}
		  
	   while(!TimerFlag);
	   TimerFlag=0;
   }
}
