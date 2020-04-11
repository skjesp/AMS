/*
 * communication.h
 *
 * Created: 09-04-2020 17:44:16
 *  Author: Sebas
 */ 

void EnableUART(int uartNum);
char ReadChar(int uartNum);
void SendChar(int uartNum, char c);
void SendString(int uartNum, char* str);