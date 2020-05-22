/*
 * Led.h
 *
 * Created: 5/20/2020 11:15:24 PM
 *  Author: Berat Kaya
 */ 


#ifndef LED_H_
#define LED_H_

#include <stdint.h>

/*  Clear all led */
void Clear_LED(void);

/*
 * Position (0x00 -> 0x07)
 * Data: 1 = ON & 0 = OFF
 */
void SET_LED(const uint8_t position, const uint8_t data);

#endif /* LED_H_ */