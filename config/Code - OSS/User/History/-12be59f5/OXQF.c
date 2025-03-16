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


char* UART_Read(UART_Object Obj, char start_sign, char end_sign){
    /**
     * @brief this functions reads a uart message and return the message
     * to the next relevant parser
     * @param UART_Object -> struct of UART that consists on the RX & TX of GPIO_Objects
     * and another parameters of the UART default struct. (CMSIS)
     */
    
    static char message[MAX_MESSAGE_SIZE];
    memset(message, 0, MAX_MESSAGE_SIZE);
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
            return message;
        }
        
        else if (start) {
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

void UART_printf(const char *format, ...){
    char buffer[128];
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    UART_Write(UART_Debug, buffer);

}
