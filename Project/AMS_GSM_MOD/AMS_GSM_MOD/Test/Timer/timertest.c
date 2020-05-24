#include "timertest.h"

void TestTimer()
{
	SendString(UART_PC, "TestTimer start\r\n");
	TimerStart(2000);
	while(CheckTimeout() == 0)
	{}
	TimerEnd();
	SendString(UART_PC, "TestTimer end\r\n");
}