#include <avr/io.h>
#include <avr/interrupt.h>
#include "sg92R.h"

void SGInit()
{
	DDRB = 0xFF;
	PORTB = 0;	
	TCCR1A = 0b10000010;
	TCCR1B = 0b00010100; // mode 10, prescaler 256
	ICR1 = 625;	
}

void UnlockState()
{
	OCR1A = 625*0.07;	
}

void LockedState()
{
	OCR1A = 625*0.11;		
}