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
	
	SendSMS("testing SensSMS function.", "+4523920863");
	//sendATcommand("AT+CMGF=1", UART_GSM, "OK");
	//sendATcommand("AT+CMGS=\"+4523920863\"", UART_GSM, ">");	
	
	
	SendString(UART_PC, "Going to while loop\r\n");
	while(1)
	{	
		char c = ReadChar(UART_GSM);	
		SendChar(UART_PC, c);			
	}

	
	return 0;
}