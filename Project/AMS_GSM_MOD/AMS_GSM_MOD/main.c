//3G + GPS shield
//
//main.c
//
//Created: 07-04-2020 13:06:00
//Author: Johannes

#include "project_definitions.h"

// TODO:
#include "sg92r.h"
//#define F_CPU 6600000
int main()
{		
	DDRB = 0xFF;
	PORTB = 0;	
	TCCR1A = 0b10000010;
	TCCR1B = 0b00010100; // mode 10, prescaler 256
	ICR1 = 625;
	// Control dutycycle. Keep it between ~0.05 and ~0.1
	OCR1A = 625*0.05;
	while(1){};
	
	DDRE = 0xFF;
	PORTE = 0;
	// Mode = 3 (PWM, Phase Correct, 10 bit)
	// Set OC1A on match down counting / Clear OC1A on match up counting
	// Clock prescaler = 1
	
	
	TCCR3A = 0b10000011;
	TCCR3B = 0b00000001;
	
	//TCCR1A = 0b00000011;
	//TCCR1B = 0b00010101;
	
	
	//ICR3 = 4999;
	// Duty cycle = 50%
	OCR3C = 1024/2;
	
	
	while(1){};
	
	SGInit();
	
	
	while(1){};
	Setup();	
	StartGSM();
	
	if(unlockSim("6913") != 0)
	{
		SendString(UART_PC, "Pin was wrong. Closing program\r\n");
		return 1;
	}
	
	// This function is OK.
	// johannes tlf: +4523923866
	// lasse tlf: +4522925623
	// lebara tlf: +4550238653
	//SendSMS("SMS test is the best.", "+4522925623");
	
	// This function is OK.
	//ReadSMS();
	
	DisplayHelp();
	SendString(UART_PC, "Going to while loop\r\n");
	run();
	return 0;
}