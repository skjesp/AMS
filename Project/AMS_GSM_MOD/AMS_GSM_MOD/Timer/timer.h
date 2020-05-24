#ifndef UART_H
#define UART_H
#include "../project_definitions.h"

void SetupTimer();
void TimerStart(unsigned int ms);
void TimerEnd();
void TestTimer();
int CheckTimeout();

#endif