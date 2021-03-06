#include "timer.h"

void SetupTimer()
{
	// Setup timer to measure
	// 16000000 Hz /1024 = 15625 Hz (15625 steps pr seconds). Maximum wait time is  approx 4.19 seconds.
	TCNT3 = 65536-(3*15625L); // 3 overflow each three seconds.
	TCCR3A = 0b00000000;
	//TCCR1B = 0b00000101; // Prescaler 1024
}

void TimerStart(unsigned int ms)
{
	// max allowed is 4096
	if(ms > 4096){
		ms = 1000; // default value will be 1 second.
	}
	
	// Setting the countdown.
	// 1 second: 15625L
	// 1 ms: 16L
	TCNT3 = 65536-(ms * 16L);
	
	// Start timer 1 with prescaler 1024
	TCCR3B = 0b00000101;
}


int CheckTimeout()
{
	return TIFR3 & 0b00000001; //(1 << 0);
}


void TimerEnd()
{
	// Stop timer 1
	TCCR3B = 0b00000000;
	// Reset timer 1
	TIFR3 = 1<<0;
}