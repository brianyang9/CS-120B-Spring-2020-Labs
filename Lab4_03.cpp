
#include <avr/io.h>

enum Door_states {Start, Init, Press_pound, Release_pound, Press_Y, Open_state} Door_State;
	
void state_machine(){

	
	switch(Door_State)
	{

		case Start:
					Door_State  = Init;
					PORTB = 0;
					break;

		case Init:
					if( PINA ==4 ){
						Door_State = Press_pound;
					}else{
						Door_State  = Init;
					}
					break;
		
		case Press_pound:
							if(PINA == 0x04){
								Door_State = Press_pound;			
							}else if(PINA == 0x00){
								Door_State = Release_pound;
							}else{
								Door_State = Init;
							}
							break;

		case Release_pound:
							if(PINA == 0x00){
								Door_State = Release_pound;
							}else if(PINA == 0x02){
								Door_State = Press_Y;
							}else{
								Door_State = Init;
							}
							break;
		
		case Press_Y:
						if(PINA == 0x02){
							Door_State = Press_Y;
						}else if(PINA == 0x00){
							Door_State = Open_state;
							PORTB=1;
						}else{
							Door_State = Init;
						}
						break;

		case Open_state:
						if(PINA == 0x00){
							Door_State = Open_state;
						}else if(PINA = 0x80){
							Door_State = Init;
							PORTB=0;
						}else{
							Door_State = Open_state;
						}
						break;					
		default:	
					PORTB=0xFF;  // to show unexpected error
					break;
		}
}

int main(void) {
	DDRA = 0X00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	
	Door_State = Start;
	
	while(1) {
		state_machine();
	}
	
	return 0;

}