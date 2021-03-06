#ifndef GSM_PROJ_UART_H
#define GSM_PROJ_UART_H
#include "../../project_definitions.h"

void EnableUART(int uartNum);
char ReadChar(int uartNum);
unsigned char CharReady(int uartNum);
void SendChar(int uartNum, char c);
void SendString(int uartNum, char* str);
int sendATcommand(char* ATcommand, int selected_UART, char* expectedResult, char* resultBuffer);
void DisplayHelp(); // PC exclusively
#endif