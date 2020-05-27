#ifndef GSM_SETUP
#define GSM_SETUP
//#define F_CPU 16000000UL
#define UART_PC 0
#define UART_GSM 1
#define UART_TEST 2

#include <avr/io.h>
#include <stdio.h>
#include <string.h>

#include "./Timer/timer.h"
#include "./UART/uart.h"
#include "./Waspmote/waspmoteDriver.h"
#include "./SG92/sg92r.h"
#include "./TM1638/TM1638.h"
// Testing
#include "./Test/Timer/timertest.h"

#endif