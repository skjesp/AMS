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
	
	Display_Digit_And_Dot(posArr[0], dataArr[0], 1);
	Display_Digit_And_Dot(posArr[1], dataArr[0], 0);
	Display_Digit_And_Dot(posArr[2], dataArr[0], 0);
	Display_Digit_And_Dot(posArr[3], dataArr[0], 0);
	
	
	/* Replace with your application code */
	while(1) {
		keys = TM1638_Read_Keys();
		switch(keys)
		{
			case 1:
			
			j++;
			
			if(j == 4)
			{
				j = 0;
			}
			
			if (j == 0)
			{
				Display_Digit_And_Dot(posArr[3], dataArr[3], 0);
			}
			else
			{
				Display_Digit_And_Dot(posArr[j-1], dataArr[j-1], 0);
			}
			
			Display_Digit_And_Dot(posArr[j], dataArr[j], 1);
			_delay_ms(500);
			
			
			break;
			
			
			case 2:
			if(dataArr[j] < 9) {
				Display_Digit_And_Dot(posArr[j], ++dataArr[j], 1);
			}
			else {
				dataArr[j] = 0;
				Display_Digit_And_Dot(posArr[j], dataArr[j], 1);
			}
			_delay_ms(500);
			
			break;
			
			case 4:
			if(dataArr[j] > 0) {
				Display_Digit_And_Dot(posArr[j], --dataArr[j], 1);
			}
			else {
				dataArr[j] = 9;
				Display_Digit_And_Dot(posArr[j], dataArr[j], 1);
			}
			_delay_ms(500);
			
			break;
			
			case 8:
			
			j--;
			if(j == -1)
			{
				j = 3;
			}
			
			if (j == 3)
			{
				Display_Digit_And_Dot(posArr[0], dataArr[0], 0);
			}
			else
			{
				Display_Digit_And_Dot(posArr[j+1], dataArr[j+1], 0);
			}
			
			Display_Digit_And_Dot(posArr[j], dataArr[j], 1);
			_delay_ms(500);
			
			break;
			
			case 16:
			Display_Digit_And_Dot(7, 1, 0);
			break;
		}
	}
}

