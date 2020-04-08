/*
 * main.c
 *
 * Created: 07-04-2020 13:06:00
 *  Author: Johannes and Lasse
 */ 

//#include <stdlib.h>
#include <stdio.h>
#include <avr/io.h>
// Uart
#include "uart.h"
#define myUART UART0
#define baud 9600
#define databits 8

int main(){
	//InitUART(myUART, baud, databits, 'N');
	InitUART(myUART, 9600, 8, 'N');
	SendString(myUART, "This is the first string\r\n");	
	while(1){
				
		SendString(myUART, "Enter input\r\n");
		char test = ReadChar(myUART);
		while (!CharReady(myUART)){

		}
		char buf[255];
		//char test = ReadChar(myUART);
		sprintf(buf, "res is %c", test);	
		SendString(myUART, buf);
		
	}	
	return 0;
}

//The entered char was: 