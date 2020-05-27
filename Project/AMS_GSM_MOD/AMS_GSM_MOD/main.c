#include "project_definitions.h"

int main()
{			
	Setup();	
	StartGSM();
	
	// This function is OK.
	// johannes tlf: +4523923866
	// lasse tlf: +4522925623
	// lebara tlf: +4550238653
	//SendSMS("SMS test is the best.", "+4522925623");
	
	//DisplayHelp();
	//SendString(UART_PC, "Going to while loop\r\n");
	run();
	
	//TM1638_init(1,1);
	//while(1)
	//{
		//TM1638_Handler();
	//}
	
	return 0;
}