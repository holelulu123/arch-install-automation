#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "stm32l072xx.h"
#include "rcc.h"
#include "gpio.h"
#include "uart.h"
#include "l80_m39.h"

int USART_DIV = 0;

void UART_SetRegisters(UART_Object Obj){
    /**
     * @brief This function sets the registers of the uart object and 
     * sets regisetrs of TX & RX GPIOs.
     */
    
    // Turning on the Clock of USARTx
    
    Obj.UART_Struct->CR1 &= ~(USART_CR1_UE);
    switch(Obj.USARTx){
        case 0:
            RCC->APB2ENR |= (RCC_APB2ENR_USART1EN);
            RCC->CCIPR   |= (RCC_CCIPR_USART1SEL_0);
            break;
        case 1:
            RCC->APB1ENR |= (RCC_APB1ENR_USART2EN);
            RCC->CCIPR   |= (RCC_CCIPR_USART2SEL_0);
            break;
        case 2:
            break;
        case 3:
            RCC->APB1ENR |= (RCC_APB1ENR_USART4EN);
            break;
        case 4:
            RCC->APB1ENR |= (RCC_APB1ENR_USART5EN);
            break;
    }
    GPIO_Init(Obj.TX);
    GPIO_Init(Obj.RX);
    
    Obj.UART_Struct->CR1  &= ~(USART_CR1_OVER8_Msk);
    Obj.UART_Struct->CR1  |=  (Obj.OverSampling << USART_CR1_OVER8_Pos);

    // If OVER8 is equal 1 -> oversampling by 8
    int USART_DIV;
    if (Obj.OverSampling){
        USART_DIV = (2 * SystemClock) / Obj.BaudRate;
        Obj.UART_Struct->BRR &= ~(0xFFFF << 0); // Clear the USARTDIV
        Obj.UART_Struct->BRR |= (USART_DIV << 4); // Set the desired baud rate
        Obj.UART_Struct->BRR |= ((USART_DIV & 0xF) >> 1); // Set the desired baud rate
    } 
    // if OVER8 is equal 0 -> oversampling by 16 
    else {
        USART_DIV = SystemClock / Obj.BaudRate;
        Obj.UART_Struct->BRR &= ~(0xFFFF << 0); // Clear the USARTDIV
        Obj.UART_Struct->BRR |= (USART_DIV); // Set the desired baud rate

    }
    Obj.UART_Struct->CR3 |= (USART_CR3_OVRDIS); // Enable Overrun -> may cause losing data but in this situation data can be lost
    switch(Obj.Direction){
        case 0:
            Obj.UART_Struct->CR1 |= (USART_CR1_TE); // Enable the Transmission
            break;    
        case 1:
            Obj.UART_Struct->CR1 |= (USART_CR1_RE); // Enable the Reception
            
        case 2:
            Obj.UART_Struct->CR1 |= (USART_CR1_RE); // Enable the Transmission
            Obj.UART_Struct->CR1 |= (USART_CR1_TE); // Enable the Reception
            break;
    }
    Obj.UART_Struct->CR1 |= (USART_CR1_UE); // Enable the USART
}


void UART_Write(UART_Object Obj ,char message[]){
    uint16_t size = strlen(message);
    for (int i = 0; i < size; i++){
        while(!((Obj.UART_Struct->ISR >> 7) & 0x1));
        Obj.UART_Struct->TDR = message[i];
    }
    
    // Wait for TC to be 1, indicates that the transmittion has completed
    while(!((Obj.UART_Struct->ISR >> USART_ISR_TC_Pos) & 0x1));

    // USART1->CR1 &= ~(0x1 << 3); // Disable the Transmission
}


char* UART_Read(UART_Object Obj, char start_sign, char end_sign, int message_size){
    /**
     * @brief this functions reads a uart message and return the message
     * to the next relevant parser
     * @param UART_Object -> struct of UART that consists on the RX & TX of GPIO_Objects
     * and another parameters of the UART default struct. (CMSIS)
     */
    
    char message[message_size];
    memset(message, 0, message_size);
    char byte;
    __uint8_t start = 0; 
    __uint8_t end = 0; 
    __uint8_t index = 0; 
    while(!end){
        while(!((USART1->ISR >> 5) & 0x1));
        byte = Obj.UART_Struct->RDR;

        if (byte == end_sign && start){
            message[index]      = '\r';
            message[index + 1]  = '\n';
            message[index + 2]  = '\0';
            end                 =   1;
            return message*;
        }

        if (start) {
            message[index] = byte;
            index++;
        }

        else if (byte == start_sign){
            start = 1;
            message[index] = byte;
            index++;
        }
    }
    
}

// void UART_l80_m39_ReadMessages(){
// /*
// This function reads the messages of the NMEA GPS Sensor. 
// Reading occur only when the register RXNE is set, if its clear, wait is needed until the 
// buffer filled up again.
// There is a thing that called overrun error, which occured when a character is received when RXNE has not been reset.
// Data cannot be transferred from the shift register to the RDR register until the RXNE is clear.nmea_message
// When overrun error occurs 
// 1. ORE bit is set
// 2. ORE bit is reset by setting the ORECF bit in the ICR Register 

// */
//     static char nmea_message[NMEA_MESSAGE_SIZE];
//     memset(nmea_message, 0, NMEA_MESSAGE_SIZE);
//     uint8_t start = 0;
//     uint8_t end = 0;
//     uint8_t index = 0;
//     while(!end){
//         while(!((USART1->ISR >> 5) & 0x1));
//         uint32_t l80_log = USART1->RDR;
//         // Waiting for $ that represent the starting of the communication
//         if(!start){
//             // Check if there is dollar sign
//             if (l80_log == Starting_Packet){ 
//                 nmea_message[index] = (char)(l80_log & 0xFF);
//                 start = 1;
//                 index++;
//             }
//         }
//         else{

//             if(l80_log == Ending_packet){
//                 nmea_message[index] = '\r';
//                 nmea_message[index+1] = '\n';
//                 nmea_message[index+2] = '\0';
//                 end = 1;
//             }
//             else{
//                 nmea_message[index] = (char)(l80_log & 0xFF);
//                 index++;
//             }

//         }
//     }
//     GPS_NMEA_MessageNavigator(nmea_message);
// }

void UART_printf(const char *format, ...){
    char buffer[128];
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    UART_Write(UART_Debug, buffer);

}
