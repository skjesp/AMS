///
 //* main.c
 //*
 //* Created: 07-04-2020 13:06:00
 //*  Author: Johannes and Lasse
 //*/ 

//#include <stdlib.h>
#include <stdio.h>
#include <avr/io.h>
// Uart
//#include "uart.h"
#include "communication.h"
#define UART_PC 0
#define UART_GSM 1
#define UART_TEST 2
#define F_CPU 16000000

#define databits 8
#define INPUT_MAX 255

int main(){
	//InitUART(myUART, baud, databits, 'N');
	//InitUART(UART_PC, 112500, 8, 'N'); // 9600
	//InitUART(UART_GSM, 112500, 8, 'N');
	//InitUART(UART_TEST, 112500, 8, 'N');
	
	EnableUART(UART_PC);
	EnableUART(UART_GSM);
	EnableUART(UART_TEST);
	
	
	// Comment this in for testing
	//while(1)
	//{	
		//char c = ReadChar(0);
		//SendChar(0, c);		
	//}
	
	
	SendString(UART_PC, "Starting GSM\r\n");
	
	while(1)
	{
		char c = ReadChar(UART_PC);
		SendChar(UART_PC, c);
	}
	
	
	while(1)
	{
		SendString(UART_GSM, "AT");
		SendChar(UART_GSM, 0x0D); // carriage return
		SendChar(UART_GSM, 0x0A); // line feed
		while(1){
			char c = ReadChar(UART_GSM);
			SendChar(UART_PC,c);
		}
		
		
		
		//char c = ReadChar(UART_PC);
		//SendChar(UART_GSM, c);
		//char c2 = ReadChar(UART_GSM);
		//SendChar(UART_PC,c2);
		
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