#ifndef GSM_SETUP
#define GSM_SETUP
#define UART_PC 0
#define UART_GSM 1
#define UART_TEST 2

#include <avr/io.h>
#include <stdio.h>
#include <string.h>

#include "./ATmega2560/Timer/timer.h"
#include "./ATmega2560/UART/uart.h"
#include "./Drivers/Waspmote/waspmoteDriver.h"
#include "./Drivers/SG92/sg92r.h"
#include "./Drivers/TM1638/TM1638.h"
#endif