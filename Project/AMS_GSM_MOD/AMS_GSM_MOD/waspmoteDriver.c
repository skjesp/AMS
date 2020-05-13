/*
 * waspmoteDriver.c
 *
 * Created: 13-04-2020 15:14:15
 *  Author: JS
 */ 

#include "waspmoteDriver.h"
static char UserPhonenumber[] = "+4593203866";

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
}

void TimerStart(unsigned int ms)
{
	// max allowed is 4096
	if(ms > 4096){
		ms = 1000; // default value will be 1 second.
	}
	
	// Setting the countdown.
	// 1 second: 15625L
	// 1 ms: 16L
	TCNT1 = 65536-(ms * 16L);
	
	// Start timer 1 with prescaler 1024
	TCCR1B = 0b00000101;
}


int CheckTimeout()
{
	return TIFR1 & 0b00000001; //(1 << 0);
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
	TimerStart(500);
	while(CheckTimeout() == 0)
	{}
	TimerEnd();
	SendString(UART_PC, "TestTimer end\r\n");
}



// Note: Alternative power off by AT command
void TogglerPower()
{	
	SendString(UART_PC, "Toggling power.\r\n");
	
	// Set high
	PORTE |= (1 << 4);
	TimerStart(2000);
	while(CheckTimeout() == 0){		
		};
	TimerEnd();
	
	PORTE &= ~(1 << 4);
	
	//TimerStart(3000);
	//while(CheckTimeout()){};
	//TimerEnd();
}



void StartGSM()
{
	SendString(UART_PC, "Checking if GSM module is powered.\r\n");	
	int res = sendATcommand("AT", UART_GSM, "OK", NULL);
	if(res == 0)
	{
		SendString(UART_PC, "GSM module is already on.\r\n");	
	}
	else{
		TogglerPower();
	}
	while(res != 0)
	{		
		res = sendATcommand("AT", UART_GSM, "OK", NULL);		
	}
	
	// Set preffered memory in simcard. 
	// TODO: not utilizing return to anything.
	SendString(UART_PC, "Setting prefered memory sections.\r\n");	
	sendATcommand("AT+CPMS=\"SM\",\"SM\",\"SM\"", UART_GSM, "OK", NULL);
	SendString(UART_PC, "Setting GSM to textmode.\r\n");	
	sendATcommand("AT+CMGF=1", UART_GSM, "OK", NULL);
	
	sendATcommand("AT+CNMI=1,2,0,0,0", UART_GSM, "OK", NULL);
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
	//sendATcommand("AT+CMGF=1", UART_GSM, "OK", NULL);	
	sendATcommand(cmgs_comm, UART_GSM, ">", NULL);
	SendString(UART_GSM, message);
	SendString(UART_GSM, "\x1A");	
	return 0;
}


// return the first SMS to 
int ReadSMS()
{
	// allocating 2KB to 
	char unreadMessageBuffer[1024] = "";
	
	//SendString(UART_GSM, "AT+CMGL=\"ALL\"\r\n");	
	int res = sendATcommand("AT+CMGL=\"ALL\"", UART_GSM, "OK", unreadMessageBuffer);
	if(res != 0)
	{
		SendString(UART_PC, "Something bad happened while reading SMS\r\n");	
	}
	SendString(UART_PC, "List of messages\r\n");
	SendString(UART_PC, unreadMessageBuffer);
	SendString(UART_PC, "\r\n");
	
	// Parse the received messages 	
	return 0;
}


void DisplayHelp()
{
	SendString(UART_PC, "1: Read SMS\r\n2: Send SMS\r\n");
}



// To be utilized in debugging.
void getUserInput()
{
	char ResponseBuffer[255];
	int i = 0;
	char c;
	while(1)
	{
		while(1)
		{
			c = ReadChar(UART_PC);
			if(c != 13) // if not carriage return
			{
				ResponseBuffer[i] = c;
				i++;
				// Echo the char
				SendChar(UART_PC, c);
			}
			else{
				SendString(UART_PC, "Breaking loop: \r\n");
				break;
			}
		}
		SendString(UART_PC, "Received input: \r\n");
		SendString(UART_PC, ResponseBuffer);
		SendString(UART_PC, "\r\n");
	}
}


int handleCommand(char *msg)
{
	
	
	
}


// Tokenize buffer. Get the phonenumber that send the command.
static void GetPhonenumber(char *buf, char *phoneNumber)
{	
	char *token;
	token = strtok(buf, "\""); // result type
	
	if(strstr(token, "+CMT") != NULL)
	{
		token = strtok(buf, "\""); // phone number
		strcpy(phoneNumber, token);		
	}
	else
	{
		phoneNumber = NULL;
	}
}



void listenState()
{
	
	char c;
	char buffer[255] = "";
	char phonenumber[15] = "";
	int i = 0;
	while(1)
	{
		buffer[i] = ReadChar(UART_GSM);
		// Check if buffer contains newline
		if (buffer[i] == 12)
		{
			// Get the phonenumber from command
			GetPhonenumber(buffer, phonenumber);			
			if (strcmp(phonenumber, UserPhonenumber) == 0)
			{
				// handle command
			}			
		}
		i++;				
	}	
}