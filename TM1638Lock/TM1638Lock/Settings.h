/*
 * Settings.h
 *
 * Created: 5/20/2020 11:20:49 PM
 *  Author: Berat Kaya
 */ 

#ifndef SETTINGS_H_
#define SETTINGS_H_

#include <stdint.h>
//TODO: Settings

// Data, Clock and Strobe settings
#define DIO  PB0 //Data I/O
#define CLK  PB1 //Clock
#define STB  PB2 //Strobe

// Command settings
#define SET_DATA  0x40 //Command 1
#define SET_ADDRES  0xC0 //Command 2
#define CONTROL_DISPLAY  0x80 //Command 3

// Data command settings
#define DATA_WRITE  0x00 //Write to Display Reg
#define DATA_READ  0x02 //Read key scan data
#define AUTOMATIC_ADDRESS  0x00
#define FIX_ADDRESS  0x04
#define NORMAL_MODE  0x00
#define TEST_MODE  0x10

// Display Control Settings
#define DISPLAY_ON  0x00 //
#define DISPLAY_OFF  0x08

// Delay
#define T_WAIT (5) // 500us

// Display Brightness
#define MAX_LIGHT  0x07

#define	TM1638_DIO_HIGH()	(PORTB |= _BV(DIO))
#define	TM1638_DIO_LOW()	(PORTB &= ~_BV(DIO))
#define	TM1638_DIO_OUTPUT()	(DDRB |= _BV(DIO))
#define	TM1638_DIO_INPUT()	(DDRB &= ~_BV(DIO))
#define	TM1638_DIO_READ()	(((PINB & _BV(DIO)) > 0) ? 1 : 0)
#define	TM1638_CLK_HIGH()	(PORTB |= _BV(CLK))
#define	TM1638_CLK_LOW()	(PORTB &= ~_BV(CLK))
#define	TM1638_STB_HIGH()	(PORTB |= _BV(STB))
#define	TM1638_STB_LOW()	(PORTB &= ~_BV(STB))

#endif /* SETTINGS_H_ */