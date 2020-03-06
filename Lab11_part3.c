
#include <avr/io.h>
#include "keypad.h"
#include "timer.h"
#include "scheduler.h"
#include "io.h"



unsigned char symbol=0x00; //our shared variable from keypad task to lcd print task
//unsigned char cursor_i=0;

enum keypad_STATES {start} keypad_state;	
int keypad_tick(int keypad_state){
	unsigned char x = GetKeypadKey();
	switch(keypad_state){
		case start: 
			keypad_state = start;
			switch(x) {
				case '\0': symbol = symbol; break;
				case '1': symbol = 0x01; break;
				case '2': symbol = 0x02; break;
				case '3': symbol = 0x03; break;
				case 'A': symbol = 0x0A; break;
				case '4': symbol = 0x04; break;
				case '5': symbol = 0x05; break;
				case '6': symbol = 0x06; break;
				case 'B': symbol = 0x0B; break;
				case '7': symbol = 0x07; break;
				case '8': symbol = 0x08; break;
				case '9': symbol = 0x09; break;
				case 'C': symbol = 0x0C; break;
				case '*': symbol = 0x0E; break;
				case '0': symbol = 0x00; break;
				case '#': symbol = 0x0F; break;
				case 'D': symbol = 0x0D; break;
				default: symbol = 0x1B; break;
			}
			break;
		
		default:
			keypad_state = start;
			break;
	}
	return keypad_state;
}
enum LCD_STATES {PRINT} LCD_STATE;
int Lcd_state_machine(LCD_STATE)
{
	switch(LCD_STATE)
	{
		case PRINT:
				LCD_Cursor(1);
				LCD_WriteData(symbol+'0');
// 				if (cursor_i>32){
// 					cursor_i=0;
// 				}else{
// 					++cursor_i;
// 				}
			break;
		default:break;

	}
	return LCD_STATE;
}

//setting up global task variables
task task1, task2;

int main(void)
{
	DDRA = 0xF0; PORTA = 0x0F; //our keypad port
	DDRB = 0xFF; PORTB = 0x00; //our test port to see which state we are currently at
	DDRC = 0xFF; PORTC = 0x00; //lcd port
	DDRD = 0xFF; PORTD = 0x00; 
	
	task *tasks[] = {&task1, &task2};
	const unsigned short numTasks = sizeof(tasks)/sizeof(task*);

	
	//Task 1
	task1.state = start;
	task1.period = 50;
	task1.elapsedTime = task1.period;
	task1.TickFct = &keypad_tick;

	//Task2
	task2.state = PRINT;
	task2.period = 50;
	task2.elapsedTime = task2.period;
	task2.TickFct = &Lcd_state_machine;
	
	TimerSet(50);
	TimerOn();
	LCD_init();

	
	unsigned short i;
    while (1) 
    {
		for(i = 0; i < numTasks; i++){
			if(tasks[i]->elapsedTime == tasks[i]->period){
				tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
				tasks[i]->elapsedTime = 0;
			}
			tasks[i]->elapsedTime += tasks[i]->period;
		}
		while(!TimerFlag);
		TimerFlag = 0;
	}
	return 0;
}