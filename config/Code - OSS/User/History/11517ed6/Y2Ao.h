#ifndef __UART_H
#define __UART_H

#include <stdio.h>
#include "stm32l072xx.h"
#include "gpio.h"
// Defining Buffer size
#define NMEA_MESSAGE_SIZE   (100)
#define Starting_Packet     (0x24)
#define Ending_packet       (0x0d)
#define MESSAGE_SIZE         ((__uint8_t)100)

extern int USART_DIV;
extern char nmea_message[NMEA_MESSAGE_SIZE]; 

typedef enum USART_BaudRateTag : int {
     BuadRate_1 = 2400,
     BuadRate_2 = 9600,
     BuadRate_3 = 19200, 
     BuadRate_4 = 38400,
     BuadRate_5 = 57600, 
     BuadRate_6 = 115200,
     BuadRate_7 = 230400,
     BuadRate_8 = 460800,
     BuadRate_9 = 921600

}USART_BaudRate;

typedef enum USART_Over8Tag : __uint8_t {
    OverSampling_16 = 0x0,
    OverSampling_8  = 0x1

}USART_Over8;

typedef enum USART_NumberTag : __uint8_t {
    USART_1     = 0x0, // Bit 14 APB2ENR
    USART_2     = 0x1, // Bit 17 APB1ENR
    USART_3     = 0x2, // None
    USART_4     = 0x3, // Bit 19 APB1ENR
    USART_5     = 0x4  // Bit 20 APB1ENR

}USART_Number;



typedef struct UART_Object {
    GPIO_Object     TX;
    GPIO_Object     RX;
    USART_TypeDef   *UART_Struct;
    USART_BaudRate  BaudRate;
    USART_Over8     OverSampling;
    USART_Number    USARTx;

} UART_Object;


static const UART_Object UART_Debug = {
    .TX                         = GPIOA_2, 
    .RX                         = GPIOA_3,
    .UART_Struct                = USART2,
    .BaudRate                   = BuadRate_2,
    .OverSampling               = OverSampling_16,
    .USARTx                     = USART_2
};

static const UART_Object UART_L80 = {
    .TX                         = GPIOA_9, 
    .RX                         = GPIOA_10,
    .UART_Struct                = USART1,
    .BaudRate                   = BuadRate_2,
    .OverSampling               = OverSampling_16,
    .USARTx                     = USART_1
};


void UART_SetRegisters(UART_Object Obj);
void UART_Write(UART_Object Obj);
char* UART_Read(UART_Object Obj);
// Functions for UART logs and debugging. 
void UART_debug_set_registers(int clock_rate, int baud_rate);
void UART_debug_sendMessage(char message[]);
void UART_printf(const char *format, ...);
// Function for UART for Reading NMEA GPS messages.

void UART_l80_m39_set_registers(int clock_rate, int baud_rate);
void UART_l80_m39_ReadMessages();

#endif