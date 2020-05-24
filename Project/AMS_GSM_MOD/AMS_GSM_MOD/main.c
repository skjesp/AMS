#include "project_definitions.h"
#include "./SG92/sg92r.h"
int main()
{			
	//Setup();
	////LockedState();
	//UnlockState();	
	//
	//
	////TestTimer();
	//
	//while(1){};	
	
	Setup();	
	StartGSM();
	
	if(unlockSim("6913") != 0)
	{
		SendString(UART_PC, "Pin was wrong. Closing program\r\n");
		return 1;
	}
	
	// This function is OK.
	// johannes tlf: +4523923866
	// lasse tlf: +4522925623
	// lebara tlf: +4550238653
	//SendSMS("SMS test is the best.", "+4522925623");
	
	// This function is OK.
	//ReadSMS();
	
	DisplayHelp();
	SendString(UART_PC, "Going to while loop\r\n");
	run();
	return 0;
}