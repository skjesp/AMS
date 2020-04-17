#ifndef WASP_DRIVER_H
#define WASP_DRIVER_H
#include "project_definitions.h"
void PowerOn();
void PowerOff();
void Setup();

void SetupCountdown(unsigned int seconds);
void StartCountdown();
void CheckCountdown();
#endif