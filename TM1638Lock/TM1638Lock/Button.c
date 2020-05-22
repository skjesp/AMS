/*
 * Button.c
 *
 * Created: 5/20/2020 10:32:23 PM
 *  Author: Admin
 */ 
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>

#include "Settings.h"
#include "Component.h"
#include "ComponentFont.h"

uint8_t TM1638_scan_keys(void)
{
	uint8_t i, keys = 0;

	TM1638_STB_LOW();
	Write_Data(SET_DATA|DATA_READ);
	for (i = 0; i < 4; i++) {
		keys |= Read_Data() << i;
	}
	TM1638_STB_HIGH();

	return keys;
}
