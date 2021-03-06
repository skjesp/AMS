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
	EnableUART(UART_PC);
	EnableUART(UART_GSM);
	
	// Init the
	SGInit();	
	TM1638_init(1,1);
	StartGSM();
}



void TogglerPower()
{	
	SendString(UART_PC, "Toggling power.\r\n");
	
	// Set high
	PORTE |= (1 << 4);
	TimerStart(2000);
	while(CheckTimeout() == 0){	};
	TimerEnd();	
	PORTE &= ~(1 << 4);
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
	
	if(unlockSim("6913") != 0)
	{
		SendString(UART_PC, "Pin was wrong. Closing program\r\n");
		while (1)
		{
		}
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



int ListenForSMS(char* rec_buf)
{
	//while(!CharReady(UART_GSM)){};
	if(!CharReady(UART_GSM))
	{
		return 1;
	};

	int i = 0;
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
	return 0;
}



void ParseCommand(char* receiveBuffer, char* phonenumber_buf, char* payload_buf)
{
	// Tokenize with vertical tab and backspace.
	char *header;
	header = strtok(receiveBuffer, "\r\n");
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
	int res = -1;
	SendString(UART_PC, "Payload is: \r\n");
	SendString(UART_PC, payload);
	SendString(UART_PC, "\r\n");
	if(strcmp(payload, "Hello world") == 0)
	{
		SendString(UART_PC, "Hello world function!\r\n");
		res = 0;
	}
	else if(strcmp(payload, "UNLOCK") == 0)
	{
		SendString(UART_PC, "UNLOCK function called.\r\n");
		UnlockState();
		SendSMS("LOCK DISABLED", UserPhonenumber);		
		res = 0;
	}
	else if(strcmp(payload, "LOCK")  == 0)
	{
		SendString(UART_PC, "LOCK function called.\r\n");		
		LockedState();
		SendSMS("LOCK ENABLED", UserPhonenumber);
		res = 0;
	}	
	else
	{
		SendString(UART_PC, "Unknown command received.\r\n");
		res = -1;
	}
	return res;	
}



 //Gets MC to run in a listener state.
void run()
{				
	char buffer_header[BUFFER_SIZE] = "";
	char buffer_payload[BUFFER_SIZE] = "";
	char phonenumber[15] = "";
	int res;
	while(1)
	{			
		// Listen for SMS
		res = ListenForSMS(buffer_header); // OK
		if(res == 0)
		{
			ParseCommand(buffer_header, phonenumber, buffer_payload);
			if(Authenticate(phonenumber) != 0)
			{
				// Authentication failed
				SendString(UART_PC,"Notifying user\r\n");
			}
			else
			{
				HandleCommand(buffer_payload);
			}
		}
		
		// Listen for key
		TM1638_Handler();
	}
}