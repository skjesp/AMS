#ifndef COMMUNICATION_H
#define COMMUNICATION_H
#include "project_definitions.h"

void EnableUART(int uartNum);
char ReadChar(int uartNum);
unsigned char CharReady(int uartNum);
void SendChar(int uartNum, char c);
void SendString(int uartNum, char* str);
void sendATcommand(char* ATcommand, char* response, int selected_UART, char* expectedResult);
#endif