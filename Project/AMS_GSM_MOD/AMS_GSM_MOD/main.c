//3G + GPS shield
//
//main.c
//
//Created: 07-04-2020 13:06:00
//Author: Johannes

#include "project_definitions.h"

int main()
{
	Setup();	
	StartGSM();
	
	if(unlockSim("6913") != 0)
	{
		SendString(UART_PC, "Pin was wrong. Closing program\r\n");
		return 1;
	}
	
	// This function is OK
	// fars telefonnr: +4523920863
	// lasses tlf: +4522925623
	
	//SendSMS("SMS test is the best.", "+4522925623");
	
	
	ReadSMS();
	SendString(UART_PC, "Going to while loop\r\n");
	while(1)
	{	
		char c = ReadChar(UART_GSM);	
		SendChar(UART_PC, c);			
	}

	
	return 0;
}