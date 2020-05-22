/*
 * TM1638Lock.cpp
 *
 * Created: 5/20/2020 10:30:03 PM
 * Author : Berat
 */ 

#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>
#include "Component.h"

#include "Led.h"
#include "Button.h"
#include "Display.h"

uint8_t j = 0;
uint8_t x = 0;
uint8_t nej = 0;
uint8_t a = 0;
uint8_t mig = 0;
uint8_t keys = 0;
uint8_t posArr[4] = {0, 1, 2, 3};

int main(void)
{
	uint8_t a =1;
	
    Initialize(a, a);
}

/*
 
  Initialize(1enable, 1brighness); 
  Display_DigitAndDot(0, 0, 0);
  
  for (int i = 0; i < 8; ++i) {
	  SET_LED((i + 1) % 8, !!(keys & (1 << i)));
  }
  
  while(1) {
	  keys = TM1638_scan_keys();
	  switch(keys)
	  {
		  case 1:
		  if(x >= 0 && x < 10) {
			  if(j == 0 && j < 1)
			  Display_DigitAndDot(posArr[j], x,1);
			  _delay_ms(2000);
			  x = 0;
			  j++;
		  }
		  
		  if(nej >= 0 && nej < 10) {
			  if(j >= 1 && j < 2)
			  Display_DigitAndDot(posArr[j], nej,1);
			  _delay_ms(2000);
			  nej = 0;
		  }
		  
		  if(nej >= 0 && nej < 10) {
			  if(j >= 2 && j < 3)
			  Display_DigitAndDot(posArr[j], a,1);
			  _delay_ms(2000);
			  a = 0;
		  }
		  
		  if(nej >= 0 && nej < 10) {
			  if(j >= 3 && j < 4)
			  Display_DigitAndDot(posArr[j], mig,1);
			  _delay_ms(2000);
			  mig = 0;
		  }
		  break;
		  
		  case 2:
		  if(x >= 0 && x < 10) {
			  if(j == 0 && j < 1)
			  Display_DigitAndDot(0, x++, 0);
			  _delay_ms(2000);
			  
			  if(j >= 1 && j < 2)
			  Display_DigitAndDot(1, x++, 0);
			  _delay_ms(2000);
			  
			  if(j >= 2 && j < 3)
			  Display_DigitAndDot(2, x++, 0);
			  _delay_ms(2000);
			  
			  if(j >= 3 && j < 4)
			  Display_DigitAndDot(3, x++, 0);
			  _delay_ms(2000);
		  }
		  
		  
		  break;
		  
		  case 4:
		  if(x >= 0 && x < 10) {
			  if(j == 0 && j < 1)
			  Display_DigitAndDot(0, x--, 0);
			  _delay_ms(2000);
			  
			  if(j >= 1 && j < 2)
			  Display_DigitAndDot(1, x--, 0);
			  _delay_ms(2000);
			  
			  if(j >= 2 && j < 3)
			  Display_DigitAndDot(2, x--, 0);
			  _delay_ms(2000);
			  
			  if(j >= 3 && j < 4)
			  Display_DigitAndDot(3, x--, 0);
			  _delay_ms(2000);
		  }
		  break;
		  
		  case 8:
		  if(x >= 0 && x < 10) {
			  if(j == 0 && j < 1)
			  Display_DigitAndDot(posArr[j], x,1);
			  _delay_ms(2000);
			  x = 0;
			  j--;
		  }
		  
		  if(nej >= 0 && nej < 10) {
			  if(j >= 1 && j < 2)
			  Display_DigitAndDot(posArr[j], nej,1);
			  _delay_ms(2000);
			  nej = 0;
		  }
		  
		  if(nej >= 0 && nej < 10) {
			  if(j >= 2 && j < 3)
			  Display_DigitAndDot(posArr[j], a,1);
			  _delay_ms(2000);
			  a = 0;
		  }
		  
		  if(nej >= 0 && nej < 10) {
			  if(j >= 3 && j < 4)
			  Display_DigitAndDot(posArr[j], mig,1);
			  _delay_ms(2000);
			  mig = 0;
		  }
		  break;
		  
		  case 16:
		  Display_DigitAndDot(4, 4, 0);
		  break;
		  
		  case 32:
		  Display_DigitAndDot(5, 5, 0);
		  break;
		  
		  case 64:
		  Display_DigitAndDot(6, 6, 0);
		  break;
		  
		  case 128:
		  Display_DigitAndDot(7, 7, 0);
		  break;
		  
	  }
  }*/

