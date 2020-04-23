#ifndef WASP_DRIVER_H
#define WASP_DRIVER_H
#include "project_definitions.h"
void PowerOn();
void TogglerPower();
void Setup();
void StartGSM();
void SetupCountdown(unsigned int seconds);
void StartCountdown();
void CheckCountdown();
void TestTimer();
void TimerStart();
int CheckTimeout();
void TimerEnd();
int unlockSim(char* simCode);
int SendSMS(char* message, char* phoneNumber);
#endif