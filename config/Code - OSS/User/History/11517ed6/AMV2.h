#ifndef __UART_H
#define __UART_H

#include "stm32l072xx.h"
#define NMEA_MESSAGE_SIZE 100
#define Starting_Packet 0x24
#define Ending_packet 0x3C



extern char nmea_message[NMEA_MESSAGE_SIZE]; 

typedef struct UART_Configuration {
    int baud_rate; // bps
    int clock_rate; // system clock rate

} UART_Configuration;



// Functions for UART logs and debugging. 
void UART_debug_set_registers(int clock_rate, int baud_rate);
void UART_debug_sendMessage(char* message);
// Function for UART for Reading NMEA GPS messages.

void UART_l80_m39_set_registers(int clock_rate, int baud_rate);
void UART_l80_m39_ReadMessages();

#endif