/*
 * Component.c
 *
 * Created: 5/20/2020 10:31:18 PM
 *  Author: Admin
 */ 
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdio.h>

#include "Settings.h"
#include "Component.h"
#include "ComponentFont.h"

static uint8_t _config = DISPLAY_ON | MAX_LIGHT;

PROGMEM const uint8_t _digit2segments[10] = {
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

void Initialize(const uint8_t start, const uint8_t brigtness)
{
	uint8_t i;

	DDRB |= (_BV(DIO) | _BV(CLK) | _BV(STB));

	TM1638_STB_HIGH();
	TM1638_CLK_HIGH();

	TM1638_STB_LOW();
	Write_Data(SET_ADDRES);
	for (i = 0; i < 16; i++) {
		Write_Data(0x00);
	}
	TM1638_STB_HIGH();

	Send_Config(start, brigtness);
}

void Write_Data(uint8_t data)
{	
	for (int i = 0; i < 8; ++i, data >>= 1) {
		TM1638_CLK_LOW();
		_delay_us(T_WAIT);
		if (data & 0x01) {
			TM1638_DIO_HIGH();
			} else {
			TM1638_DIO_LOW();
		}
		TM1638_CLK_HIGH();
		_delay_us(T_WAIT);
	}
}

uint8_t Read_Data()
{
	int result = 0;

	TM1638_DIO_INPUT();
	TM1638_DIO_HIGH();

	for (int i = 0; i < 8; i++) {
		TM1638_CLK_LOW();
		if (TM1638_DIO_READ()) {
			result |= 0x80;
		}
		result >>= 1;
		TM1638_CLK_HIGH();
	}

	TM1638_DIO_OUTPUT();
	TM1638_DIO_LOW();

	return result;
}

static void Send_Data(const uint8_t address, const uint8_t data)
{
	Send_Command(SET_DATA | FIX_ADDRESS);	
	TM1638_STB_LOW();
	Write_Data(SET_ADDRES | address);
	Write_Data(data);
	TM1638_STB_HIGH();
}

static void Send_Config(const uint8_t start, const uint8_t brightness)
{
	_config = (start ? DISPLAY_ON : DISPLAY_OFF) |
	(brightness > MAX_LIGHT ? MAX_LIGHT : brightness);

	Send_Command(SET_DATA);
	Send_Command(CONTROL_DISPLAY | _config);
}

static void Send_Command(const uint8_t data)
{
	TM1638_STB_LOW();
	Write_Data(data);
	TM1638_STB_HIGH();
}

void Start_TM1638(const uint8_t data)
{
	Send_Config(data, _config & MAX_LIGHT);
}


