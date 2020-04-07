/*
 * Lab6App.c
 *
 * Created: 12-03-2020 21:15:02
 * Author : Sebas
 */ 

#include <avr/io.h>
#include "uart.h"
#include "lcd162.h"

int main(void)
{
	
	LCDInit();
	
	
	// Display clear
	LCDClear();
	// Goto line 1, 3.character
	LCDGotoXY(2,0);
	// Testing LCD_DispString()
	LCDDispString("It works?");
	
	
	InitUART(9600,8,'N');
    /* Replace with your application code */
    while (1) 
    {
		for(int i=0; i<10 ; i++){
			SendInteger(i);
			SendString("\r\n");	
		}
    }
}

