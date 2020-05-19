/*
 * waspmoteDriver.c
 *
 * Created: 13-04-2020 15:14:15
 *  Author: JS
 */ 

#include "waspmoteDriver.h"
// Todo: Use EEPROM to store userphonenumber
//#include <EEPROM.h>
#define BUFFER_SIZE 255
static const char UserPhonenumber[] = "+4593203866";

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
	
	res = sendATcommand("AT+CNMI=1,2,0,0,0", UART_GSM, "OK", NULL);
	while(res != 0)
	{
		res = sendATcommand("AT+CNMI=1,2,0,0,0", UART_GSM, "OK", NULL);
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


//run()
//{
	//char receiveBuffer[BUFFER_SIZE] = "";	
	//while(1)
	//{
		//while(!CharReady(UART_GSM))
		//{}
			//
		//
	//}	
//}


void ListenForSMS(char* rec_buf)
{
	int i = 0;
	while(!CharReady(UART_GSM)){};
	//SendString(UART_GSM, "AT+CMGL=\"ALL\"\r\n\0");
	TimerStart(3000);
	while(!CheckTimeout())
	{
		if(CharReady(UART_GSM))
		{
			rec_buf[i] = ReadChar(UART_GSM);
			//SendChar(UART_PC, buffer_header[i]);
			i++;
		}
	}
	TimerEnd();
	SendString(UART_PC, "The received message was:\r\n");
	SendString(UART_PC, rec_buf);	
}



void ParseCommand(char* receiveBuffer, char* phonenumber_buf, char* payload_buf)
{
	/// Testing purpose:
	char testbuffer[] = {13, 10, 43, 67, 77, 84, 58, 32, 34, 43, 52, 53, 57, 51, 50, 48, 51, 56, 54, 54, 34, 44, 34, 34, 44, 34, 50, 48, 47, 48, 53, 47, 49, 54, 44, 49, 50, 58, 51, 48, 58, 52, 54, 43, 48, 56, 34, 13, 10, 72, 101, 108, 108, 111, 32, 119, 111, 114, 108, 100, 13, 10, 0};	
	
	// Tokenize with vertical tab and backspace.
	char *header;
	header = strtok(testbuffer, "\r\n");
	SendString(UART_PC, "Message header is: ");
	SendString(UART_PC, header);
	SendString(UART_PC, "\r\n");
		
	//payload_buf = strtok(NULL, "\r\n");	
	strcpy(payload_buf, strtok(NULL, "\r\n"));
	
	SendString(UART_PC, "Message payload is: ");
	SendString(UART_PC, payload_buf);
	SendString(UART_PC, "\r\n");
	
	// Get phonenumber from header string.
	strtok(header, "\"");	
	strcpy(phonenumber_buf, strtok(NULL, "\""));
	
	SendString(UART_PC, "Phonenumber is: ");
	SendString(UART_PC, phonenumber_buf);
	SendString(UART_PC, "\r\n");
}


int Authenticate(char *received_number)
{
	if(strcmp(received_number, UserPhonenumber) == 0)
	{
		SendString(UART_PC, "Phonenumber authentication: OK\r\n");
		return 0;
	}
	else
	{
		SendString(UART_PC, "Phonenumber authentication: FAIL\r\n");
		return -1;
	}
}


int HandleCommand(char* payload)
{
	SendString(UART_PC, "Payload is: \r\n");
	SendString(UART_PC, payload);
	SendString(UART_PC, "\r\n");
	if(strcmp(payload, "Hello world") == 0)
	{
		SendString(UART_PC, "Hello world function!\r\n");
	}
	else if(strcmp(payload, "UNLOCK") == 0)
	{
		SendString(UART_PC, "UNLOCK function called.\r\n");
	}
	else if(strcmp(payload, "LOCK")  == 0)
	{
		SendString(UART_PC, "LOCK function called.\r\n");
	}
	else
	{
		SendString(UART_PC, "Unknown command received.\r\n");
	}
}


 //Gets MC to run in a listener state.
void run()
{	
	//char c;
	char buffer_header[BUFFER_SIZE] = "";
	char buffer_payload[BUFFER_SIZE] = "";
	char phonenumber[15] = "";	
	while(1)
	{
		//ListenForSMS(buffer_header); // OK
		ParseCommand(buffer_header, phonenumber, buffer_payload);
		if(Authenticate(phonenumber) != 0)
		{
			SendString(UART_PC,"Notifying user\r\n");
			while(1){};
		}
		HandleCommand(buffer_payload);
	}
}