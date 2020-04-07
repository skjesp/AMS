/******************************************
  AMS, LAB2
  Sample code:
  Using simulator and Atmel-ICE
  
  Henning Hargaard, February 1, 2019
*******************************************/
#include <avr/io.h>
#define F_CPU 16000000
#include <util/delay.h>

int main()
{
unsigned char i = 0;
  
  DDRA = 0;    //PORTA pins are inputs (SWITCHES)
  DDRB = 0xFF; //PORTB pins are outputs (LEDs)
  while (1)
  {
    PORTB = i; //Display "i" at the LEDs
    i++;
    _delay_ms(500);
    if ((PINA & 0b10000000)==0)
      i = 0;
  }
}