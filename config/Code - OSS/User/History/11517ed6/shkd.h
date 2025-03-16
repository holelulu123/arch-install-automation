#ifndef __UART_H
#define __UART_H

#include "stm32l072xx.h"

typedef struct UART_Configuration {
    int baud_rate; // bps
    int clock_rate; // system clock rate

};
// Functions for UART logs and debugging. 
void UART_debug_set_registers(int clock_rate, int baud_rate);
void UART_debug_sendMessage(char* message);
void UART_l80_m39_set_registers(int clock_rate, int baud_rate);
void UART_l80_m39_ReadMessages();

#endif