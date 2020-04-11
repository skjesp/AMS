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

void EnableUART(int uartNum)
{
	switch(uartNum){
		case 0:			
			UCSR0A = 0b00100000;
			UCSR0B = 0b00011000; // enable transmitter and receiver. No interrupts.
			UCSR0C = 0b00000110; // 8 databit
			UBRR0 = 103; // Baud set to 9600. See slide 36 lecture 17 MSYS.
			break;
		
		case 1:
			UCSR1A = 0b00100000;
			UCSR1B = 0b00011000;
			UCSR1C = 0b00000110;
			UBRR1 = 103;
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


void testSendString(char* str)
{
	while(*str != 0)
	{
		// Wait for transmitter register empty (ready for new character)
		while ( (UCSR1A & (1<<5)) == 0 )
		{}
		// Then send the character
		UDR1 = *str;
		str++;	
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