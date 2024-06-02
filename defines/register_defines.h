#if !defined REGISTER_DEFINES

#include "types.h"
//Modes and pin numbers and ports
#define OUTPUT 0
#define INPUT 1


#define PIN_ZERO (u32)(1<<0)
#define PIN_ONE (u32)(1<<1)
#define PIN_TWO (u32)(1<<2)

#define PA 0
#define PB 1


//registers
//TODO(Oscar): do I need volatile??????????????????
#define SYS_TICK (u32*)(0x0000003C)
#define STK (u32*)(0xE000E010)
#define STK_RVR (u32*)(0xE000E014)
#define STK_CVR (u32*)(0xE000E018)
#define STK_CALIB (u32*)(0xE000E01C)
#define PORTA (u32*)(0x48000000)
#define PORTA_BSRR (u32*)(0x48000018)
#define PORTA_OT (u32*)(0x48000004)
#define PORTA_IDR (u32*)(0x48000010)
#define PORTA_ODR (u32*)(0x48000014)
#define PORTA_PUPDR (u32*)(0x4800000C)
#define PORTB (u32*)(0x48000400)
#define PORTB_OT (u32*)(0x48000404)
#define PORTB_IDR (u32*)(0x48000410)
#define PORTB_ODR (u32*)(0x48000414)
#define PORTB_PUPDR (u32*)(0x4800040C)
#define RCC (u32*)(0x40021000)
#define RCC_AHBENR (u32*)(0x40021014)
#define RCC_APB2ENR (u32*)(0x40021018)
#define TIMER_14 (u32*)(0x40002000)
#define TIMER_14_IE (u32*)(0x4000200C)
#define TIMER_14_SR (u32*)(0x40002010)
#define TIMER_14_CNT (u32*)(0x40002024)
#define NVIC_IE (u32*)(0xE000E100)
#define SHPR3 (u32*)(0xE000ED20)
#define UIF (u32)(1<<0)

//bits

#define PORTA_RESET (0x28000000)
#define PORTn_RESET (0x00000000)
#define SYSTICK_I_MASK_NVIC (u32)(1<<15)
#define SYSTICK_CLOCK_PROCESSOR (u32)(1<<2)
#define SYSTICK_INTERUPT_EN (u32)(1<<1)
#define SYSTICK_COUNT_EN (u32)(1<<0)
#define RCC_PORT_A (u32)(1<<17)
#define RCC_PORT_B (u32)(1<<18)
#define PA1 (u32)(1<<1)
#define PA2 (u32)(1<<2)
#define PA1_M0 (u32)(1<<2)
#define PA1_M1 (u32)(1<<3)
#define PA1_PU0 (u32)(1<<2)
#define PA1_PU1 (u32)(1<<3)
#define PA1_OT (u32)(1<<1)
#define PA2_M0 (u32)(1<<4)
#define PA2_M1 (u32)(1<<5)
#define PA2_PU0 (u32)(1<<4)
#define PA2_PU1 (u32)(1<<5)
#define PA2_OT (u32)(1<<2)
#define PB1 (u32)(1<<1)
#define PB2 (u32)(1<<2)
#define PB1_M0 (u32)(1<<2)
#define PB1_M1 (u32)(1<<3)
#define PB1_PU0 (u32)(1<<2)
#define PB1_PU1 (u32)(1<<3)
#define PB1_OT (u32)(1<<1)
#define PB2_M0 (u32)(1<<4)
#define PB2_M1 (u32)(1<<5)
#define PB2_PU0 (u32)(1<<4)
#define PB2_PU1 (u32)(1<<5)
#define PB2_OT (u32)(1<<2)

//types
typedef struct{
	u32* data;
	u32 count;
}Arr;
typedef struct{
	u32* port;
	u32* pin;
	u32* pupr;
	u32* ot;
	u32* odr;
	u32* idr;
	u32  rcc;
}Port;

#include <stdarg.h>
//util functions
/*
#define PINS(...) PINS(...,(u8*)0); 
#define PINS_(...)      \
	u32 n = 0; \
	va_list p; \
	va_start(p, 10); \
	u32 array[10] \
	u32 a = 1;\
	while(a){ \
		a = va_arg(p, u32);\
	    	array[n] = a;	\
		n++; \
	} \
	va_end(p); \
	pin_array(n,array);
*/
Arr pin_array(s32 n, ...){
	va_list p;   
	Arr result; 
	u32 ps[10];  
	u32 cnt = 0; 
	va_start(p, n); 
	for(u32 i = 0; i<10 && ps[i]!=0; ++i){ 
		ps[i] = va_arg(p, u32);       
		cnt++;     
	}                 
	va_end(p);        
	result.data = ps; 
	result.count = cnt; 
	return result; 
}


void set_up_port(u32 port, u32 mode){
        u32* p = port == PA ? PORTA : PORTB;
	u32* pupr = port == PA ? PORTA_PUPDR : PORTB_PUPDR;
	u32* ot = port == PA ? PORTA_OT : PORTA_OT;

	u32 rcc = port == PA ? RCC_PORT_A : RCC_PORT_B;
	*RCC_AHBENR  |= rcc;
	switch(mode){
		case OUTPUT:
			*p |= PA1_M0|PA2_M0;//;
			*p &= ~(PA1_M1|PA2_M1);

			*pupr |=  PA1_PU0|PA2_PU0;
			*pupr &= ~(PA1_PU1|PA2_PU0);
			*ot &= ~(PA1_OT|PA2_OT);
		break;
		case INPUT:
			*p &= ~PB1_M0;
			*p &= ~PB1_M1;		
			*pupr &= ~PB1_PU1;
			*pupr &= ~PB1_PU0;
		break;
	}
}
#if 0
Port* set_up_port(u32 port, u32 mode, Arr pins){
	Port result;
        result.port = port == PA ? PORTA : PORTB;
	result.pupr = port == PA ? PORTA_PUPDR : PORTB_PUPDR;
	result.ot = port == PA ? PORTA_OT : PORTA_OT;

	result.rcc = port == PA ? RCC_PORT_A : RCC_PORT_B;
	*RCC_AHBENR  |= result.rcc;
	switch(mode){
		case OUTPUT:
			for(u32 i=0;i<pins.count; ++i){
				*result.port |= pins.data[i]*2;
				*result.port &= ~pins.data[i]+1*2;
			}
			//*result.port |= PA1_M0|PA2_M0;//;
			//*result.port &= ~(PA1_M1|PA2_M1);
			for(u32 i=0;i<pins.count; ++i){
				*result.pupr |= pins.data[i]*2;
				*result.pupr &= ~pins.data[i]+1*2;
			}

			//*result.pupr |=  PA1_PU0|PA2_PU0;
			//*result.pupr &= ~(PA1_PU1|PA2_PU0);
			for(u32 i=0;i<pins.count; ++i){
				*result.ot &= ~((pins.data[i]*2) |
						(pins.data[i]+1*2));
			}
			//*result.ot &= ~(PA1_OT|PA2_OT);
		break;
		case INPUT:
			for(u32 i=0;i<pins.count; ++i){
				*result.port &= ~pins.data[i]*2;
				*result.port &= ~pins.data[i]+1*2;
			}
			for(u32 i=0;i<pins.count; ++i){
				*result.pupr &= ~pins.data[i]*2;
				*result.pupr &= ~pins.data[i]+1*2;
			}
			//*result.port &= ~PB1_M0;
			//*result.port &= ~PB1_M1;		
			//*result.pupr &= ~PB1_PU1;
			//*result.pupr &= ~PB1_PU0;
		break;
	}
	return (Port*)(void*)result.port;
}
#endif

#define REGISTER_DEFINES
#endif
