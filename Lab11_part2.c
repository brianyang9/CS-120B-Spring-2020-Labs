

#include <avr/io.h>
//#include "keypad.h"
#include "timer.h"
#include "scheduler.h"
#include "io.h"




enum LCD_STATES{ init, print,print2}LCD_state;
unsigned char column=0;
unsigned char temp[38]="CS120B is Legend... wait for it DARY!";
unsigned char string_i=0;
unsigned char temp2[37];
void LCD_tick(){
	
	switch(LCD_state){
		case init:  column=32; LCD_state=print; LCD_ClearScreen(); LCD_DisplayString(column, "                              ");break;
		case print: 
					PORTB=0x00;
					LCD_ClearScreen();
					//delay_ms(50);                            

					for ( int i=32-column,j=0; j<i;j++)
					{
						temp2[j]=temp[j];
					}
					
					//LCD_DisplayString(column, temp);
					LCD_DisplayString(column, temp2);

					
					--column;
					
					
					
					if( column==0){
						 LCD_state=print2;
						 
					}
					break;
					
		case print2:
					PORTB=0xff;

					for ( int i=string_i, j=0; i<38; i++,j++)
					{
						temp2[j]=temp[i];
					}
					
					LCD_DisplayString(column, temp2);
					
					if( string_i==37 ){
						string_i=0;
						LCD_state=init;
					}
					
					++string_i;
					
					break;
					
		default: LCD_state=init; break;
					
	}
};




int main(void)
{
	DDRB=0xFF; PORTB=0x00;
	DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
	

	// Initialize LCD Display
	LCD_init();


	// Starting at position 1 on the LCD screen, writes Hello World
	//LCD_DisplayString(1, "Hello World.");
	
	
	
	
// 	static task task1;
// 	task *tasks[] = {&task1};
// 	const unsigned short numTasks = sizeof(tasks)/sizeof(task*);
	
	//Task 1
// 	task1.state = LI_Init1;
// 	task1.period = 10;
// 	task1.elapsedTime = task1.period;
// 	task1.TickFct = &LCDI_SMTick;
	
	TimerSet(350);
	TimerOn();
	
	unsigned short i;
    /* Replace with your application code */
    while (1) 
    {
// 		for(i = 0; i < numTasks; i++){
// 			if(tasks[i]->elapsedTime == tasks[i]->period){
// 				tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
// 				tasks[i]->elapsedTime = 0;
// 			}
// 			tasks[i]->elapsedTime += 50;
// 		}
		LCD_tick();
		while(!TimerFlag);
		TimerFlag = 0;
	}
	return 0;
}

