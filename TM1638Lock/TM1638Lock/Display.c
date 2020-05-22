/*
 * Display.c
 *
 * Created: 5/20/2020 11:13:17 PM
 *  Author: Berat Kaya
 */ 

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

#include "Settings.h"
#include "Component.h"
#include "ComponentFont.h"

const uint8_t segmentNumbers[10] = { 0x3F, // 0
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

void Display_DigitAndDot(const uint8_t position, const uint8_t digit, const uint8_t dot)
{
	Display_Segment(position,
	(digit < 10 ? pgm_read_byte_near((uint8_t *)&segmentNumbers + digit) : 0x00) |
	(dot ? 0b10000000 : 0));
}

void Clear_Segment()
{
	for (int i = 0; i < 8; ++i) {
		Display_Segment(i, 0x00);
	}
}

void Display_Segment(const uint8_t position, const uint8_t segments)
{
	Send_Data(position << 1, segments);
}


