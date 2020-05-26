/*
 * main.c
 *
 *  Author: Berat Kaya
 *	Inspiration: https://github.com/lpodkalicki/attiny-tm1638-library/blob/master/tm1638.h
 * Har fået søgt inspiration fra ovenstående git repos.
 */ 

#define F_CPU 16000000

#include <avr/io.h>
#include <util/delay.h>
#include "TM1638.h"

uint8_t keys = 0;
int j = 0;
uint8_t posArr[4] = {0, 1, 2, 3};
uint8_t dataArr[4];

int main(void)
{
	TM1638_init(1,1);
	Display_Digit_And_Dot(7, 0, 0);
	

	
	/* Replace with your application code */
	while(1) {
		TM1638_Handler();
	}
}

