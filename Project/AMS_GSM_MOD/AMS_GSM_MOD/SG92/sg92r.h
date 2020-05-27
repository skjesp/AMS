/*
 * sg92r.h
 *
 * Created: 24-04-2020 13:35:00
 * Author: Lasse Kjærgaard
 
 * Driver for "SG92R" micro servo motor.
 
 * pin 3 PWM control register OCR3C
 
 TCCR3A = 00001001
 TCCR3B = 00001100
 
 */ 

#ifndef SG92R_H_
#define SG92R_H_

void SGInit();
void UnlockState();
void LockedState();
void RotateLeft();
void RotateRight();
void RotateMiddle();

#endif /* SG92R_H_ */