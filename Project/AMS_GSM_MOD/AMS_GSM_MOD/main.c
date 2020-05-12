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
	
	// This function is OK.
	// johannes tlf: +4523923866
	// lasse tlf: +4522925623
	// lebara tlf: +4550238653
	//SendSMS("SMS test is the best.", "+4522925623");
	
	// This function is OK.
	//ReadSMS();
	
	// **************************************************
	// Print choices:
	// PrintMenu();
	SendString(UART_PC, "These are the choices:\r\nChoice 1\r\nChoice 2\r\n");
	
	char ResponseBuffer[255];
	int i = 0;
	char c;
	while(1)
	{
		while(1)
		{
			c = ReadChar(UART_PC);
			if(c != 12) // if not newline
			{
				ResponseBuffer[i] = c;
				i++;
				// Echo the char
				SendChar(UART_PC, c);
			}
			else{
				break;
			}
		}
		SendString(UART_PC, "Received input: \r\n");
		SendString(UART_PC, ResponseBuffer);
	}
	
	// **************************************************
	
	
	
	
	SendString(UART_PC, "Going to while loop\r\n");
	while(1)
	{	
		char c = ReadChar(UART_GSM);	
		SendChar(UART_PC, c);			
	}

	
	return 0;
}