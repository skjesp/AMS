/*
 * TM1638Berat.c
 *
 *  Author: Berat Kaya
 *	Inspiration: https://github.com/lpodkalicki/attiny-tm1638-library/blob/master/tm1638.h
 * Har søgt inspiration fra ovenstående git repos.
 */ 
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include "TM1638.h"

//TODO:: Define PIN MACRO behaviour
#define DIO_High() (PORTB |= _BV(DIO))
#define DIO_LOW() (PORTB &= ~_BV(DIO))
#define DIO_INPUT() (DDRB &= ~_BV(DIO))
#define DIO_OUTPUT() (DDRB |= _BV(DIO))
#define DIO_READ() (((PINB & _BV(DIO)) > 0) ? 1 : 0)
#define CLK_HIGH() (PORTB |= _BV(CLK))
#define CLK_Low() (PORTB &= ~_BV(CLK))
#define STB_HIGH() (PORTB |= _BV(STB))
#define STB_LOW() (PORTB &= ~_BV(STB))


static uint8_t konfig = DISPLAY_ON | MAX_LIGHT_DISP;

PROGMEM const uint8_t displayDigits[10] = {
	0x3F, // 0
	0x06, // 1
	0x5B, // 2
	0x4F, // 3
	0x66, // 4
	0x6D, // 5
	0x7D, // 6
	0x07, // 7
	0x7F, // 8
	0x6F  // 9
};

void TM1638_init(const uint8_t start, const uint8_t brightness)
{
	uint8_t i;
	
	DDRB |= (_BV(DIO) | _BV(CLK) | _BV(STB));
	STB_HIGH();
	CLK_HIGH();
	
	STB_LOW();
	TM1638_Write(SET_ADDRESS);
	for(i = 0; i < 16; i++)
	{
		TM1638_Write(0x00);
	}
	STB_HIGH();
	
	TM1638_Config(start, brightness);
	
	
}

void TM1638_Start(const uint8_t val)
{
	TM1638_Config(val, konfig & MAX_LIGHT_DISP);
}

void Display_Array(const uint8_t pos, const uint8_t rekke)
{
	TM1638_Send(pos << 1, rekke);
}

void Clear_Display_Array(void)
{
	uint8_t i;
	
	for(i = 0; i < 8; i++)
	{
		Display_Array(i, 0x00);
	}
}

void Display_Digit_And_Dot(const uint8_t pos, const uint8_t dig, const uint8_t dot)
{
	Display_Array(pos,
	(dig < 10 ? pgm_read_byte_near((uint8_t *)&displayDigits + dig) : 0x00) |
	(dot ? 0b10000000 : 0));
}

void Set_Led(const uint8_t pos, const uint8_t val)
{
	TM1638_Send((pos << 1) - 1, !!val);
}

void Clear_Led(void)
{
	uint8_t i;
	
	for(i = 0; i < 8; i++)
	{
		Set_Led(i, 0x00);
	}
}

uint8_t TM1638_Read_Keys(void)
{
	uint8_t i, keys = 0;
	
	STB_LOW();
	TM1638_Write(SET_DATA | READ_DATA);
	for(i = 0; i < 4; i++)
	{
		keys |= TM1638_Read() << i;
	}
	STB_HIGH();
	return keys;
}

void TM1638_Config(const uint8_t start, const uint8_t brightness)
{
	konfig =(start ? DISPLAY_ON : DISPLAY_OFF) |
	(brightness > MAX_LIGHT_DISP ? MAX_LIGHT_DISP : brightness);
	
	TM1638_Command(SET_DATA);
	TM1638_Command(SET_DISPLAY | konfig);
}

void TM1638_Command(const uint8_t val)
{
	STB_LOW();
	TM1638_Write(val);
	STB_HIGH();
}


void TM1638_Write(uint8_t val)
{
	uint8_t i;
	
	for(i = 0; i < 8; i++, val >>=1)
	{
		CLK_Low();
		_delay_us(Twait);
		
		if(val & 0x01)
		{
			DIO_High();
		}
		else
		{
			DIO_LOW();
		}
		CLK_HIGH();
		_delay_us(Twait);
	}
	
}

uint8_t TM1638_Read(void)
{
	uint8_t i, resultat = 0;
	DIO_INPUT();
	DIO_High();
	
	for(i = 0; i < 8; i++)
	{
		CLK_Low();
		if(DIO_READ())
		{
			resultat |= 0x80;
		}
		resultat >>= 1;
		CLK_HIGH();
	}
	DIO_OUTPUT();
	DIO_LOW();
	return resultat;
}

void TM1638_Send(const uint8_t addresss, const uint8_t data)
{
	TM1638_Command(SET_DATA | FIXED_ADDRESS);
	STB_LOW();
	TM1638_Write(SET_ADDRESS | addresss);
	TM1638_Write(data);
	STB_HIGH();
}
