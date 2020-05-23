//3G + GPS shield
//
//main.c
//
//Created: 07-04-2020 13:06:00
//Author: Johannes

#include "project_definitions.h"

// TODO:
#include "sg92r.h"

int main()
{
	Setup();
	EnableUART(UART_PC);
	DDRE |= (1<<PE4)|(1<<PE5); 
	TIMSK3 |= 0b00000001;
	TCCR3A|=(1<<COM3A1)|(1<<COM3B1)|(1<<WGM31);        //NON Inverted PWM
	TCCR3B|=(1<<WGM33)|(1<<WGM32)|(1<<CS31)|(1<<CS30); //PRESCALER=64 MODE 14(FAST PWM)
	
	
	ICR3=4999;  //fPWM=50Hz	
	OCR3C = 250;
	
	
	while(1){
		//char buf[255];
	//itoa(PINE,buf, 10);
	SendString(UART_PC, "PINE is: ");
	SendString(UART_PC, PINE);
	SendString(UART_PC, "\r\n");
	}
	
	//DDRB = 0xFF;
	//PORTB = 0;
	//// Mode = 3 (PWM, Phase Correct, 10 bit)
	//// Set OC1A on match down counting / Clear OC1A on match up counting
	//// Clock prescaler = 1
	//TCCR1A = 0b10000011;
	//TCCR1B = 0b00000001;
	////ICR3 = 4999;
	//// Duty cycle = 50%
	//OCR3A = 1024/2;
	
	
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