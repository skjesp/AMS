/*
 * Display.h
 *
 * Created: 5/20/2020 11:12:58 PM
 *  Author: Berat Kaya
 */ 


#ifndef DISPLAY_H_
#define DISPLAY_H_

#include <stdint.h>

/* Display digit ('0'..'9') with optional dot at position (0x00..0x07) */
void Display_DigitAndDot(const uint8_t position, const uint8_t digit, const uint8_t dot);

/* Clear all display segments (including dots).*/
void Clear_Segment();

/* Display raw segments at position (0x00..0x07)
 * 
 *
 *      bits:
 *        -- 0 --
 *       |       |
 *       5       1
 *       |       |
 *        -- 6 --
 *       |       |
 *       4       2
 *       |       |
 *        -- 3 -- *7
 *
 * Example segment configurations:
 * - for character 'H', segments=0b01110110
 * - for character '-', segments=0b01000000
 * - etc.
 */
void Display_Segment(const uint8_t position, const uint8_t segments);


#endif /* DISPLAY_H_ */