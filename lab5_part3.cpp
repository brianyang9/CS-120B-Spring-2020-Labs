
#include <avr/io.h>
enum STATES{ start, initial, press} state;
unsigned char tempB=0;
unsigned char tempA=0;
void Tick(){
	
	tempA=~PINA;

	switch(state){
		case start: 
					state=initial;

					break;
					
		case initial: 
						switch(tempB){
						case 0: PORTC=0x00; break;
						case 1: PORTC=0x2A; break;  
						case 2: PORTC=0x15; break;  
						case 3: PORTC=0x33; break;  
						default: PORTC=0xFF; break;
						}
						
						if( tempA==1 ) {
							state=press;
							tempB=(tempB<3) ? ++tempB : 0;
						}else{
							state=initial;
						}
						break;
						
		case press:		state=( tempA ) ?  press: initial; break;
		
		default: break;				
	}
	
	};

int main(void)
{
    /* Replace with your application code */
	DDRA=0x00; PORTA=0xFF;
	DDRC=0xFF; PORTC=0x00;
	state=start;
	
    while (1) 
    {
		Tick();
    }
}

