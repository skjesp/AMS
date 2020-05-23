/*
 * sg92r.c
 *
 * Created: 24-04-2020 13:36:42
 *  Author: Lasse Kjærgaard
 */


#include <avr/io.h>
#include <avr/interrupt.h>
#include "sg92R.h"

void SGInit()
{
	DDRE |= (1<<PE4)|(1<<PE5); 
	
	
	
	TIMSK3 |= 0b00000001;
	
	TCCR3A|=(1<<COM3A1)|(1<<COM3B1)|(1<<WGM31);        //NON Inverted PWM
	TCCR3B|=(1<<WGM33)|(1<<WGM32)|(1<<CS31)|(1<<CS30); //PRESCALER=64 MODE 14(FAST PWM)
	ICR3=4999;  //fPWM=50Hz
	
	
	TCNT3 = 0;
	
	OCR3C = 250;
	
	sei();
	
}

ISR(TIMER3_OVF_vect)
{
	
}


void RotateLeft()
{
	OCR3A = 250; //1ms	
}

void RotateRight()
{
	OCR3A = 500; //2ms		
}

void RotateMiddle()
{
	OCR3A = 375; //1,5ms	
}
 

