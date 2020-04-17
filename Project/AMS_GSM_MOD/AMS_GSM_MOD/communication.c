/*
 * communication.c
 *
 * Created: 09-04-2020 17:47:54
 *  Author: Sebas
 */ 
//#define XTAL 16000000
//#define BAUD 115200
#include "communication.h"
#include <avr/io.h>
#include <string.h>
#include <util/delay.h>

void EnableUART(int uartNum)
{
	switch(uartNum){
		case 0:			
			UCSR0A = 0b00100000;
			UCSR0B = 0b00011000; // enable transmitter and receiver. No interrupts.
			UCSR0C = 0b00000110; // 8 databit
			UBRR0 = 8; // Baud set to 115200. See slide 36 lecture 17 MSYS. 9600 baud
			break;
		
		case 1:
			UCSR1A = 0b00100000;
			UCSR1B = 0b00011000;
			UCSR1C = 0b00000110;
			UBRR1 = 8;// 103; 115200 baud
			break;
			
		case 2:
			UCSR2A = 0b00100000;
			UCSR2B = 0b00011000;
			UCSR2C = 0b00000110;
			UBRR2 = 103;			
			break;
			
		case 3:
			break;
			
		default:
			break;
	}
}


unsigned char CharReady(int uartNum){
	switch(uartNum)
	{
		case 0:
		return UCSR0A & (1<<7);		
		break;
		
		case 1:
		return UCSR1A & (1<<7);
		break;
		
		default:
		break;
	}
	return 0;
}


char ReadChar(int uartNum)
{	
	switch(uartNum)
	{
		case 0:
			while ( (UCSR0A & (1<<7)) == 0 )
			{}
			return UDR0;
			break;
			
		case 1:
			while ( (UCSR1A & (1<<7)) == 0 )
			{}
			return UDR1;
			break;
			
		case 2:
			while ( (UCSR2A & (1<<7)) == 0 )
			{}
			return UDR2;
			break;
			
		default:
			break;		
	}
	return 0;	
}

//// Modified from uart.c from BB.
void SendChar(int uartNum, char c)
{
	switch(uartNum)
	{
		case 0:
			// Wait for transmitter register empty (ready for new character)
			while ( (UCSR0A & (1<<5)) == 0 )
			{}
			// Then send the character
			UDR0 = c;
			break;
			
		case 1:
			// Wait for transmitter register empty (ready for new character)
			while ( (UCSR1A & (1<<5)) == 0 )
			{}
			// Then send the character
			UDR1 = c;
			break;
			
		case 2:
			// Wait for transmitter register empty (ready for new character)
			while ( (UCSR2A & (1<<5)) == 0 )
			{}
			// Then send the character
			UDR2 = c;
			break;
			
		default:
			break;		
	}
}



// Modified code from uart.c from BB.
void SendString(int uartNum, char* str)
{
	while(*str != 0)
	{
		// Send the character pointed to by "Streng"
		SendChar(uartNum, *str);
		// Advance the pointer one step
		str++;			
	}	
}

void sendATcommand(char* ATcommand, char* response, int selected_UART, char* expectedResult)//, char* expected_answer, unsigned int timeout)
{				
	// Clean inputbuffer
	while(CharReady(selected_UART))
	{
		ReadChar(selected_UART);
	}
	
	// Concatenate command with newline.
	char AT_command_buf[255] = "";
	strcpy(AT_command_buf,ATcommand);
	strcat(AT_command_buf, "\r\n\0");
	SendString(selected_UART, AT_command_buf);
	
			
	int i = 0;
	int elapsedms = 0;
	char c;
	//// this loop waits for the answer
	do{
		// if there are data in the UART input buffer, reads it and checks for the answer
		if(CharReady(selected_UART))
		{						
			c = ReadChar(selected_UART);
			response[i] = c;
			i++;

			// Check if expected result is somewhere in the response.			
			if(strstr(response, expectedResult) != NULL){				
				return;
			}
		}
		else
		{
			//_delay_ms(1);
			//elapsedms += 1;
			//if(elapsedms > 1500)
			//{
				//SendString(UART_PC, "AT command has timed out.\r\n");				
				//return;
			//}
		}
		// Waits for the answer with time out
	}
	while(1);	
}