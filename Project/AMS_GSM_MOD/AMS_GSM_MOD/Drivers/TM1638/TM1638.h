/*
 * TM1638Berat.h
 *
 *  Author: Berat Kaya
 *	Inspiration: https://github.com/lpodkalicki/attiny-tm1638-library/blob/master/tm1638.h
 * Har søgt inspiration fra ovenstående git repos.
 */ 


#ifndef TM1638_H_
#define TM1638_H_

#include <stdint.h>
#include "../../project_definitions.h"
//TODO: Settings

// Data, Clock and Strobe settings
#define DIO PB0 //Data I/O
#define CLK PB1 //Clock
#define STB PB2 //Strobe
#define Twait (5) //Vores delay 'tolerance'
#define MAX_LIGHT_DISP 0x07

// Command settings
#define SET_DATA 0X40 //Command 1
#define SET_ADDRESS 0xc0 //Command 2
#define SET_DISPLAY 0x80 //Command 3

// Data command settings
#define WRITE_DATA 0X00 //Write to Display Reg
#define READ_DATA 0x02 //Read key scan data
#define AUTO_ADDRESS 0X00
#define FIXED_ADDRESS 0x04
#define NORMAL_MODE 0X00
#define TEST_MODE 0x10

// Display Control Settings
#define DISPLAY_ON 0x08
#define DISPLAY_OFF 0x00

/*TODO:: Define prototypes for TM1638*/
void TM1638_init(const uint8_t start, const uint8_t brightness);

void TM1638_Start(const uint8_t val);

void Display_Array(const uint8_t pos, const uint8_t rekke);

void Clear_Display_Array(void);

void Display_Digit_And_Dot(const uint8_t pos, const uint8_t dig, const uint8_t dot);

void Set_Led(const uint8_t pos, const uint8_t val);

void Clear_Led(void);

uint8_t Read_Keys(void);

//TODO:: Main method implementation
void TM1638_Handler();

//Protocol for TM1638
static void TM1638_Config(const uint8_t start, const uint8_t brightness);
static void TM1638_Command(const uint8_t val);
static void TM1638_Write(const uint8_t val);
static uint8_t TM1638_Read(void);
static void TM1638_Send(const uint8_t addresss, const uint8_t data);

#endif /* TM1638_H_ */