


#include <avr/io.h>


enum Buttons_States { B_start, Release_state, Press_PINA0, Press_PINA1, Reset } buttons_state;

void One_Button_System()
{
	unsigned char tmpPINA = ~PINA;
	switch(buttons_state)
	{
		case B_start: 
							
							PORTC=0x02; 
							buttons_state=Release_state;
							break;
						
		case Release_state:	
							if( tmpPINA==0x00 ){
								buttons_state=Release_state;
							}else if ( tmpPINA==0x01 ) {
								buttons_state=Press_PINA0;
								if( PORTC<9 ){
								++PORTC;
								}
							}else if ( tmpPINA==0x02 ) {
								buttons_state=Press_PINA1;
								
								if(PORTC!=0){
								--PORTC;
								}
								
							}else if (tmpPINA==0x03 ) {
								PORTC=0;
								buttons_state=Reset;
							}else {
								// do nothing to stay in this state
								// In case if PORTA has unexpected value such as 0x70
							}
							break;
							
		case Press_PINA0:	
							if( tmpPINA==0x01 ){
								
								//if (PORTB>=9){
									//PORTC=9;
								//}else{
									// do nothing
								//}
								
							} else if ( tmpPINA==0x00 ) {
								buttons_state=Release_state;
							} else if( tmpPINA==0x03 ) {
								buttons_state=Reset;
									PORTC=0;
							} else {
								// do nothing to stay in this state
								// In case if PORTA has unexpected value such as 0x70
							}
							break;
							
		case Press_PINA1:
							if( tmpPINA==0x02 ){

								// 
								//if ( PORTB <= 0){
								//	PORTC=0;
								//	}else{
								//	// do nothing
								//}
								// ?????????????????????????????????????????
								
								\74
							} else if ( tmpPINA==0x00 ) {
								buttons_state=Release_state;
							} else if( tmpPINA==0x03 ) {
								buttons_state=Reset;
								PORTC=0;
							} else {
								// do nothing to stay in this state
								// In case if PORTA has unexpected value such as 0x70
							}
							break;
		case Reset:		
							if( tmpPINA==0x00 ){
								buttons_state=Release_state;
								PORTC=0x00;
							}else{
								//stay here do nothing, because human CANNOT release button simultaneously
							}
							break;
							
		default:			PORTC=0x0F;  // turn on both LED lights to show unexpected situation
							break;
	}
}
int main(void)
{
    DDRA = 0X00; PORTA = 0xFF;
	DDRC = 0xFF; PORTC = 0x00;
	
	buttons_state=B_start;
	
	
    while (1) 
    {
		One_Button_System();
    }
}

