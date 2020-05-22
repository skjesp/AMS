/*
 * Component.h
 *
 * Created: 5/20/2020 10:31:07 PM
 *  Author: Berat Kaya
 */ 

#ifndef COMPONENT_H_
#define COMPONENT_H_

#include <stdint.h>
/*TODO: Definitions

/*Initialize TM1638*/
void Initialize(const uint8_t start, const uint8_t brigtness);

static void Write_Data(uint8_t data);

static uint8_t Read_Data(void);

static void Send_Data(const uint8_t address, const uint8_t data);

static void Send_Config(const uint8_t start, const uint8_t brightness);

static void Send_Command(const uint8_t data);

/*Turns Display ON or OFF*/
void Start_TM1638(const uint8_t data);








#endif /* COMPONENT_H_ */