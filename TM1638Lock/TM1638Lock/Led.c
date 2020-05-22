/*
 * Led.c
 *
 * Created: 5/20/2020 11:15:40 PM
 *  Author: Berat Kaya
 */ 

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

#include "Settings.h"
#include "Component.h"
#include "ComponentFont.h"

void Clear_LED()
{
	uint8_t i;

	for (i = 0; i < 8; ++i) {
		SET_LED(i, 0x00);
	}
}

void SET_LED(const uint8_t position, const uint8_t data)
{
	Send_Data((position << 1) - 1, !!data);
}