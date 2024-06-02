#include "defines/register_defines.h"
#include "defines/sys_tick.h"

volatile u32* idr = PORTB_IDR;
volatile u32* odr = PORTA_ODR;

int main(){
	initialize_sys_tick();
	*PORTA = PORTA_RESET;
	*PORTB = PORTn_RESET;
	set_up_port(PA, OUTPUT);
	set_up_port(PB, INPUT);	
	while(1){
		if(*idr & PB1){
			*odr &= ~PA2;
			*odr |= PA1;
			delay(1000);
		}
		else{
			*odr &= ~PA1;
			*odr |= PA2;
		}
	}
	return 0;
}
