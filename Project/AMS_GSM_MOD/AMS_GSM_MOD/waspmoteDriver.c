/*
 * waspmoteDriver.c
 *
 * Created: 13-04-2020 15:14:15
 *  Author: JS
 */ 

#include "waspmoteDriver.h"

void Setup()
{
	// Set pin as output
	DDRE = 0b11111111;	
	PORTE &= ~(1 << 4); 
	//PORTE |= (1 << 4);
	EnableUART(UART_PC);
	EnableUART(UART_GSM);	
	
	// Setup timer to measure 
	TCNT1 = 65536-(3*15625); // 3 overflow each three seconds.
	
	TCCR1A = 0b00000000;
	TCCR1B = 0b00000101; // Prescaler 1024
	
}


void PowerOn()
{
	// Ensure that PowerOn pin is high.
	PORTE |= (1 << 4);
	_delay_ms(10);
	PORTE &= ~(1 << 4); 
	_delay_ms(180);
	PORTE |= (1 << 4);
}


// Note: Alternative power off by AT command
void PowerOff()
{
	
	
	
	
	
	// Ensure that PowerOn pin is high.
	PORTE |= (1 << 4);
	_delay_ms(1000);
	PORTE &= ~(1 << 4);	
}


void Reboot()
{
	PowerOff();
	PowerOn();
}