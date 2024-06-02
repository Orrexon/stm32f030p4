#if !defined SYS_TICK_H

#include "types.h"
//TODO(Oscar): be smarter about the timing parameters
#define MICROSECONDS_PER_TIMER_OVERFLOW (8000UL)
#define MILLIS_INC (MICROSECONDS_PER_TIMER_OVERFLOW / 1000)

// NOTE(Oscar): check the maths on this boy
#define FRACT_INC ((MICROSECONDS_PER_TIMER_OVERFLOW % 1000) >> 3)
#define FRACT_MAX (1000 >> 3)


volatile u32 timer_overflow_count = 0;
volatile u32 timer_millis = 0;
static u8 timer_fract = 0;

void __attribute__((interrupt("sys_tick_handler")))
sys_tick_handler(){
		
	u32 m = timer_millis;
	u8 f = timer_fract;

	m += MILLIS_INC;
	f += FRACT_INC;
	if (f >= FRACT_MAX) {
		f -= FRACT_MAX;
		m += 1;
	}

	timer_fract = f;
	timer_millis = m;
	timer_overflow_count++;
}
static u8 initialized = 0;
void initialize_sys_tick(){
	*NVIC_IE = SYSTICK_I_MASK_NVIC;
	*STK_RVR = MICROSECONDS_PER_TIMER_OVERFLOW;
	*STK_CVR = 0;
	*STK |= SYSTICK_COUNT_EN|SYSTICK_INTERUPT_EN|SYSTICK_CLOCK_PROCESSOR;
	initialized = 1;
}

static void delay(u16 time_ms){
	if(!initialized) initialize_sys_tick();
	u16 ms = 0;
	u32 count = timer_overflow_count;
	while(ms<time_ms){

		if((count-timer_overflow_count) >0){
			count = timer_overflow_count;
			++ms;
		}
	}
}

#define SYS_TICK_H
#endif
