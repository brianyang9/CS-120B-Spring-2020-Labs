
#include <avr/io.h>

unsigned char clickButton=0;

enum Button_States { B_start, Release_state, Press_state } button_state;

void One_Button_System()
{
	switch( button_state)
	{
		case B_start: 
							//clickButton=0;  // doesn't affect the logic at all, so block it
							PORTB=0x01; 
							button_state=Release_state;
							break;
						
		case Release_state:	
							if( PINA==0x00 ){
								// do nothing to stay in this state
							}else if ( PINA==0x01 ) {
								button_state=Press_state;
								//clickButton=(clickButton==0) ? 1 : 0;   // doesn't affect the logic at all, so block it
								PORTB=(PORTB==1) ? 2 : 1; 
							}else {
								// do nothing to stay in this state
								// In case if PORTA has unexpected value such as 0x70
							}
							break;
							
		case Press_state:	
							if( PINA==0x01 ){
								// do nothing to stay in this state
								}else if ( PINA==0x00 ) {
								button_state=Release_state;
								}else {
								// do nothing to stay in this state
								// In case if PORTA has unexpected value such as 0x70
							}
							break;
							
		default:			PORTB=0x03;  // turn on both LED lights to show unexpected situation
							break;
	}
}
int main(void)
{
    DDRA = 0X00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	
	button_state=B_start;
	
    while (1) 
    {
		One_Button_System();
    }
}

