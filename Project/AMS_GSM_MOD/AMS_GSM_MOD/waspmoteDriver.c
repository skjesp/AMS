/*
 * waspmoteDriver.c
 *
 * Created: 13-04-2020 15:14:15
 *  Author: JS
 */ 

#include "waspmoteDriver.h"

void Setup()
{
	// Set pin as output
	DDRE = 0b11111111;	
	PORTE &= ~(1 << 4); 
	//PORTE |= (1 << 4);
	EnableUART(UART_PC);
	EnableUART(UART_GSM);	
	
	// Setup timer to measure
	// 16000000 Hz /1024 = 15625 Hz (15625 steps pr seconds). Maximum wait time is  approx 4.19 seconds.	
	TCNT1 = 65536-(3*15625L); // 3 overflow each three seconds.
	TCCR1A = 0b00000000;
	//TCCR1B = 0b00000101; // Prescaler 1024	
	
	// Set preffered memory in simcard. 
	// TODO: not utilizing return to anything.
	sendATcommand("AT+CPMS=\"SM\",\"SM\",\"SM\"", UART_GSM, "OK", NULL);	
}

void TimerStart()
{
	// Start timer 1 with prescaler 1024
	TCCR1B = 0b00000101;
}


int CheckTimeout()
{
	return TIFR1 & (1 << 0);	
}


void TimerEnd()
{
	// Stop timer 1
	TCCR1B = 0b00000000;
	// Reset timer 1
	TIFR1 = 1<<0;		
}



void TestTimer()
{	
	SendString(UART_PC, "TestTimer start\r\n");
	TimerStart();
	while(CheckTimeout() == 0)
	{}
	TimerEnd();
	SendString(UART_PC, "TestTimer end\r\n");
}



// Note: Alternative power off by AT command
void TogglerPower()
{	
	// Set high
	PORTE |= (1 << 4);
	
	TimerStart();
	while(CheckTimeout()){};
	TimerEnd();
	
	PORTE &= ~(1 << 4);
	
	TimerStart();
	while(CheckTimeout()){};
	TimerEnd();
}



void StartGSM()
{
	SendString(UART_PC, "Checking if GSM module is powered.\r\n");	
	int res = sendATcommand("AT", UART_GSM, "OK", NULL);
	if(res == 0)
	{
		SendString(UART_PC, "GSM module is already on.\r\n");	
	}
	while(res != 0)
	{
		TogglerPower(); // TODO: not waiting long enough to let gsm start
		res = sendATcommand("AT", UART_GSM, "OK", NULL);		
	}	
}

int unlockSim(char* simCode)
{	
	int res = sendATcommand("AT+CPIN?", UART_GSM, "SIM PIN", NULL);
	if(res == 0)
	{
		SendString(UART_PC, "Unlocking SIM\r\n");
		
		char command[50] = "";
		strcpy(command, "AT+CPIN=");
		strcat(command, simCode);				
		int pinRes = sendATcommand(command, UART_GSM, "READY", NULL);		
		if(pinRes != 0)
		{
			SendString(UART_PC, "SIM not unlocked!\r\n");
			return -1;
		}
	}
	else
	{
		SendString(UART_PC, "SIM was already unlocked.\r\n");
	}	
	return 0;
}



int SendSMS(char* message, char* phoneNumber)
{
	char cmgs_comm [50] = "AT+CMGS=\"";
	strcat(cmgs_comm, phoneNumber);
	strcat(cmgs_comm, "\"");		
	sendATcommand("AT+CMGF=1", UART_GSM, "OK", NULL);	
	sendATcommand(cmgs_comm, UART_GSM, ">", NULL);
	SendString(UART_GSM, message);
	SendString(UART_GSM, "\x1A");	
	return 0;
}


int ReadSMS()
{
	char unreadMessageBuffer[255] = "";	
	SendString(UART_GSM, "AT+CMGL=\"REC UNREAD\"");
	int res = sendATcommand("AT+CMGL=\"REC UNREAD\"", UART_GSM, "OK", unreadMessageBuffer);
	if(res != 0)
	{
		SendString(UART_PC, "Something bad happened while reading SMS\r\n").		
	}
	
	// Parse the received messages 
	
	
	return 0;
}