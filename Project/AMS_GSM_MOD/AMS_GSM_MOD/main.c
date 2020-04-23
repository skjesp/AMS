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
	
	
	
	
	while(1)
	{
		
	}
	
	
	char buf[255] = "";	
	SendString(UART_PC, "TEST START.\r\n");	
	sendATcommand("AT", UART_GSM, "OK");
	
	sendATcommand("AT+CPIN=\"6913\"", UART_GSM, "OK");
	sendATcommand("AT+CMGF=1", UART_GSM, "OK");
	sendATcommand("AT+CMGS=\"+4523920863\"", UART_GSM, ">");	
	
	
	SendString(UART_PC, "Going to while loop\r\n");
	while(1)
	{	
		char c = ReadChar(UART_GSM);	
		SendChar(UART_PC, c);			
	}
	
	
	//while(1){
		////SendString(UART_GSM, "Hello from GSM.");
		//SendChar(UART_GSM, 'Q');
		//SendString(UART_PC, "Received Char was: ");
		//while(1){
			//char c = ReadChar(UART_TEST);
			//SendChar(UART_PC, c);
		//}
	//}
	
	
	
	while(1){
				
		//SendString(UART_PC, "Enter input\r\n");
		////char char = ReadChar(UART_PC);
		//char c;
		//char buf[INPUT_MAX] = "";
		//// while enter is not pressed
		//
		//for(int i = 0; i < INPUT_MAX; i++){
			//c = ReadChar(UART_PC);  //enter is pressed
			//if(c == 13)
			//{
				//break;
			//}
			//else
			//{
				//buf[i] = c;
				//SendChar(UART_PC, c);
			//}
		//}
		//SendString(UART_PC, "\r\n");
		//strcat(buf, "\r\n");	
		//SendString(UART_GSM,buf);
		
		//char ucsrna = CharReady(UART_GSM);
		//while (ucsrna == 0)
		//{
			//ucsrna = CharReady(UART_GSM);
		//}
		
		
		
		//char gsm = ReadChar(UART_GSM);
		//SendChar(UART_PC, gsm);
		
		//SendString(UART_PC, buf);	
	}	
	return 0;
}

//The entered char was: 


