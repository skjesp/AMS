// Author: Johannes Jespersen

#ifndef WASP_DRIVER_H
#define WASP_DRIVER_H
#include "../../project_definitions.h"

// Waspmote functions
void Setup();
void PowerOn();
void TogglerPower();
void StartGSM();
void SetupCountdown(unsigned int seconds);

// GSM functions
int SendSMS(char* message, char* phoneNumber);
int unlockSim(char* simCode);

// The while loop for MC
void run();
#endif