

#include <avr/io.h>
#include <avr/interrupt.h>
#include "timer.h"



unsigned char ThreeLEDs_value=0x00;

enum ThreeLEDs_STATES { ThreeLEDs_start, b0, b1, b2 } ThreeLEDs_state;
	
void Tick_ThreeLEDs() {
	switch(ThreeLEDs_state){
		case ThreeLEDs_start:	ThreeLEDs_state=b0; break;
		case b0:	ThreeLEDs_state=b1; ThreeLEDs_value=0x01; break;
		case b1:	ThreeLEDs_state=b2; ThreeLEDs_value=0x02; break;
		case b2:	ThreeLEDs_state=b0; ThreeLEDs_value=0x04; break;
		default:	ThreeLEDs_state=ThreeLEDs_start; break;
	}
}





unsigned char BlinkingLEDs_value=0x00;

enum BlinkingLEDs_STATES { BlinkingLEDs_start, b3_on, b3_off } BlinkingLEDs_state;

void Tick_BlinkingLEDs() {
	switch(BlinkingLEDs_state){
		case BlinkingLEDs_start:	BlinkingLEDs_state=b3_on; break;
		case b3_on:	BlinkingLEDs_state=b3_off; BlinkingLEDs_value=0x08;	break;
		case b3_off: BlinkingLEDs_state=b3_on; BlinkingLEDs_value=0x00; break;
		default:	ThreeLEDs_state=BlinkingLEDs_start; break;
	}
	
}




enum CombineLEDs_STATES { CombineLEDs_start, light_up_LEDs } CombineLEDs_state;

void Tick_CombineLEDs() {
	
	unsigned char output_B=0;
	
	switch(CombineLEDs_state){
		case CombineLEDs_start:	CombineLEDs_state=light_up_LEDs; break;
		case light_up_LEDs:	//BlinkingLEDs_state=light_up_LEDs;   why adding this will cause problem?
							output_B = ThreeLEDs_value | BlinkingLEDs_value;	break;
		default:	CombineLEDs_state=CombineLEDs_start; break;
	}
	
	PORTB=output_B;

}

int main(void)
{
    /* Replace with your application code */
   /* Replace with your application code */

   DDRB=0XFF; PORTB=0x00;

   
    ThreeLEDs_state = ThreeLEDs_start;
    BlinkingLEDs_state = BlinkingLEDs_start;
	CombineLEDs_state = CombineLEDs_start;

   TimerSet(1000);
   TimerOn();
   
   while (1)
   {
		Tick_ThreeLEDs();
		Tick_BlinkingLEDs();
		Tick_CombineLEDs();
		
	   while(!TimerFlag);
	   TimerFlag=0;
   }
}

