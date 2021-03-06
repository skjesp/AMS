/*-------------------------------------------------------------------
  File name: "lcd162.c"

  Driver for "LCD Keypad Shield" alphanumeric display.
  Display controller = HD44780U (LCD-II).
  
  Max. uC clock frequency = 16 MHz (Tclk = 62,5 ns)

  Connection : PORTx (4 bit mode) :
  [LCD]        [Portx]
  RS   ------  PH 5
  RW   ------  GND
  E    ------  PH 6
  DB4  ------  PG 5
  DB5  ------  PE 3
  DB6  ------  PH 3
  DB7  ------  PH 4

  Henning Hargaard, February 5, 2018
---------------------------------------------------------------------*/
#include <avr/io.h>
#define F_CPU 16000000
#include <util/delay.h>
// Enabling us to use macro _NOP() to insert the NOP instruction
#include <avr/cpufunc.h>
#include "lcd162.h"

// library function itoa() is needed
#include <stdlib.h>
#include <string.h>

//*********************** PRIVATE (static) operations *********************
static void waitBusy()
{
  _delay_ms(5);
}  

static void pulse_E()
{ 
  // Enable E
  PORTH |= 0b01000000; // bit 6 set. (According to Lab Exercise document page 3.)
  
  // 5x nop, (Waiting 312,5 ns).
  _NOP();
  _NOP();
  _NOP();
  _NOP();
  _NOP();
    
  // disable E
  PORTH &= 0b10111111; // bit 6 cleared
  
  // Enable cycle time. min 500 ns. NOP delay is 125 ns. Return and call also takes time.
  _NOP();
  _NOP();
}

// Sets the display data pins according to the 4 lower bits of data
static void set4DataPins(unsigned char data)
{
  PORTH = (PORTH & 0b11100111) | ((data<<1) & 0b00011000);
  PORTE = (PORTE & 0b11110111) | ((data<<2) & 0b00001000);
  PORTG = (PORTG & 0b11011111) | ((data<<5) & 0b00100000);  
}

static void sendInstruction(unsigned char data)
{      
  // set rs low.
  waitBusy();
  PORTH &= 0b10011111;
  
  // wait nop (t_as)
  _NOP();
  
  // set databit (first 7, 6, 5, 4)
  set4DataPins((data>>4));
  
  // pulse
  pulse_E();
  
  // set databit (3, 2, 1, 0)
  set4DataPins(data);
  
  // pulse
  pulse_E();
  
  // make delay 2 busy  wait
  waitBusy();
}

static void sendData(unsigned char data)
{      
    // set rs high.
	waitBusy();
	
    PORTH |= 0b00100000;
    PORTH &= ~(0b01000000);
    
	// wait nop (t_as)
    _NOP();
    
    // set databit (first 7, 6, 5, 4)
    set4DataPins((data>>4));
    
    // pulse
    pulse_E();
    
    // set databit (3, 2, 1, 0)
    set4DataPins(data);
    
    // pulse
    pulse_E();
    
    // make delay nop (min 40 micro sec)
    _delay_us(50);
	// call and return takes time.
}

static void Timer2_Init()
{
	// Fast PWM, OC1A (= PB4) non-inverted mode
	TCCR2A = 0b10000011;
	// Prescaler = 1024
	TCCR2B = 0b00000111;
	// Duty cyc�e = 25 %
	OCR2A = 255/4;
	// Pin OC1A (= PB4) is output
	DDRB |= (1<<4);	
}

// A/D converter is used for switch readings
static void ADC_Init()
{
	// ADC reference = 5 volt, Input = ADC0
	ADMUX = 0b01000000;
	// ADC enable, Free running, No interrupts, Prescaler = 128.
	ADCSRA = 0b11100111;
	ADCSRB = 0b00000000;	
}

//*********************** PUBLIC functions *****************************

// Initializes the display, blanks it and sets "current display position"
// at the upper line, leftmost character (cursor invisible)
// Reference: Page 46 in the HD44780 data sheet
void LCDInit()
{
  // Initializing the used port
  DDRH |= 0b01111000;  // Outputs
  DDRE |= 0b00001000;
  DDRG |= 0b00100000;
  
  // Wait 50 ms (min. 15 ms demanded according to the data sheet)
  _delay_ms(50);
  // Function set (still 8 bit interface)
  PORTG |= 0b00100000;
  PORTE |= 0b00001000;
  pulse_E();
  
  // Wait 10 ms (min. 4,1 ms demanded according to the data sheet)
  _delay_ms(10);
  // Function set (still 8 bit interface)
  pulse_E();

  // Wait 10 ms (min. 100 us demanded according to the data sheet)
  _delay_ms(10);
  // Function set (still 8 bit interface)
  pulse_E();

  // Wait 10 ms (min. 100 us demanded according to the data sheet)
  _delay_ms(10);
  // Function set (now selecting 4 bit interface !)
  PORTG &= 0b11011111;
  pulse_E();

  // Function Set : 4 bit interface, 2 line display, 5x8 dots
  sendInstruction( 0b00101000 );
  // Display, cursor and blinking OFF
  sendInstruction( 0b00001000 );
  // Clear display and set DDRAM adr = 0	
  sendInstruction( 0b00000001 );
  // By display writes : Increment cursor / no shift
  sendInstruction( 0b00000110 );
  // Display ON, cursor and blinking OFF
  sendInstruction( 0b00001100 );
  
  Timer2_Init();
  ADC_Init();
}

// Blanks the display and sets "current display position" to
// the upper line, leftmost character
void LCDClear()
{
  sendInstruction(0b00000001); 
}

// Sets DDRAM address to character position x and line number y
void LCDGotoXY( unsigned char x, unsigned char y )
{
  char data = 0b10000000;
  
  // Set DB7 1  
  if(y == 1)
  {
	  data |= 0b01000000;	// Adding 0x40 to DDRAM-address.
  }
  
  if(x < 16)
  {
	  data |= x;	  
  } // no need for else since data is initialized to zeros.
  
  sendInstruction(data);  
}

// Display "ch" at "current display position"
void LCDDispChar(char ch)
{	
	sendData(ch);
}

// Displays the string "str" starting at "current display position"
void LCDDispString(char* str)
{
	for(int i = 0; i< strlen(str); i++)
	{	
		sendData(str[i]);
	}
}

// Displays the value of integer "i" at "current display position"
void LCDDispInteger(int i)
{	
	
	char buffer[13]; 
	itoa(i, buffer, 10);
	
	LCDDispString(buffer);	
}

// Loads one of the 8 user definable characters (UDC) with a dot-pattern,
// pre-defined in an 8 byte array in FLASH memory
// The parameter *UDCTab is a pointer to the array containing the bit (dot) pattern
// The parameter UDCNo (0 to 7), defines which of the 8 CGRAM characters is to be loaded.
void LCDLoadUDC(unsigned char UDCNo, const unsigned char *UDCTab)
{
	unsigned char cgram_addr = 0b01000000;
	cgram_addr |= (UDCNo << 3);
	sendInstruction(cgram_addr);	
	
	for(int i = 0; i< 8; i++)
	{
		sendData(UDCTab[i]);		
	}
	sendInstruction(0b00000010);
}

// Selects, if the cursor has to be visible, and if the character at
// the cursor position has to blink.
// "cursor" not 0 => visible cursor.
// "blink" not 0 => the character at the cursor position blinks.
void LCDOnOffControl(unsigned char cursor, unsigned char blink)
{
	unsigned char LCD_command = 0b00001100; // Display (bit 3) always on.
	if(cursor) // If cursor == 1
	{
		LCD_command |= 1 << 1;
	}
	if(blink)
	{
		LCD_command |= 1 << 0; // "<< 0" is redundant, but is written for educational purposes.
	}	
	sendInstruction(LCD_command);  
}

// Moves the cursor to the left
void LCDCursorLeft()
{
	sendInstruction(0b00010000);
}

// Moves the cursor to the right
void LCDCursorRight()
{
	sendInstruction(0b00010100);
}

// Moves the display text one position to the left
void LCDShiftLeft()
{
	sendInstruction(0b00011000);
}

// Moves the display text one position to the right
void LCDShiftRight()
{
	sendInstruction(0b00011100);
}

// Sets the backlight intensity to "percent" (0-100)
void setBacklight(unsigned char percent)
{
	if(percent <= 100)
		OCR2A = (percent*255) / 100;
}

// Reads the status for the 5 on board keys
// Returns 0, if no key pressed
unsigned char readKeys()
{
	if(ADCW < 50)
		return 1;
	if(ADCW < 195)
		return 2;
	if(ADCW < 380)
		return 3;
	if(ADCW < 555)
		return 4;
	if(ADCW < 790)
		return 5;
	
	return 0;
}