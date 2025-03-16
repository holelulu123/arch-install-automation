#include <stdio.h>
#include "stm32l072xx.h"
#include "spi.h"
#include "uart.h"

void SetRegisters_SPI1(){
    /*
    This function sets the registers for SPI1 communication
    for half-duplex.
    */
    
    // Enable Clock for SPI1 and GPIOA
    RCC->APB2ENR |= (RCC_APB2ENR_SPI1EN); // Enable the USART2 Clock
    RCC->IOPENR |= (RCC_IOPENR_GPIOAEN); // Enable the Clock of GPIO A
    
    
    // GPIO CONFIGURATION
    GPIOA->MODER &= ~(0x3 << GPIOA_MOSI_PIN * 2); // Clear PA4 mode
    GPIOA->MODER &= ~(0x3 << GPIOA_MISO_PIN * 2); // Clear PA5 mode
    GPIOA->MODER &= ~(0x3 << GPIOA_NSS_PIN * 2); // Clear PA6 mode
    GPIOA->MODER &= ~(0x3 << GPIOA_SCLK_PIN * 2); // Clear PA7 mode
    
    GPIOA->MODER |= (0x2 << GPIOA_MOSI_PIN); // Select PA4 for Altenrate function
    GPIOA->MODER |= (0x2 << GPIOA_MISO_PIN); // Select PA5 for Altenrate function
    GPIOA->MODER |= (0x1 << GPIOA_NSS_PIN); // Select PA6 for Altenrate function
    GPIOA->MODER |= (0x1 << GPIOA_SCLK_PIN); // Select PA7 for Altenrate function
    
    GPIOA->AFR[0] |= (0x0 << GPIOA_MOSI_PIN * 4); // Select PA4 for MOSI (AFSEL 0) 
    GPIOA->AFR[0] |= (0x0 << GPIOA_MISO_PIN * 4); // Select PA5 for MISO (AFSEL 0)
    GPIOA->AFR[0] |= (0x0 << GPIOA_NSS_PIN * 4); // Select PA6 for NSS (AFSEL 0)
    GPIOA->AFR[0] |= (0x0 << GPIOA_SCLK_PIN * 4); // Select PA7 for PSCLK (AFSEL 0)

    GPIOA->PUPDR |= (0x0 << GPIOA_MOSI_PIN * 2); // Select Pull-down for PA4
    GPIOA->PUPDR |= (0x0 << GPIOA_MISO_PIN * 2); // Select Pull-down for PA5
    GPIOA->PUPDR |= (0x1 << GPIOA_NSS_PIN * 2); // Select Pull-up for PA6
    GPIOA->PUPDR |= (0x0 << GPIOA_SCLK_PIN * 2); // Select pull-down for PA7

    GPIOA->OSPEEDR |= (0x2 << GPIOA_MOSI_PIN * 2); // Select Output speed of High speed for PA4
    GPIOA->OSPEEDR |= (0x2 << GPIOA_MISO_PIN * 2); // Select Output speed of High speed for PA5
    GPIOA->OSPEEDR |= (0x2 << GPIOA_NSS_PIN * 2); // Select Output speed of High speed for PA6
    GPIOA->OSPEEDR |= (0x2 << GPIOA_SCLK_PIN * 2); // Select Output speed of High speed for PA7

    // SPI CONFIGURATION
    SPI1->CR1 &= ~(SPI_CR1_SPE_Msk); // Disables SPI
    SPI1->CR1 |= (SPI_CR1_BIDIMODE); // Enables Half-duplex mode 
    SPI1->CR1 &= ~(SPI_CR1_DFF_Msk); // Clears the data frame format to 0, make it 8-bit frame.
    SPI1->CR1 |= (SPI_CR1_MSTR_Msk); // Sets the MCU to the master
    SPI1->I2SCFGR &= ~(SPI_I2SCFGR_I2SMOD_Msk); // SPI Mode is selected
    SPI1->CR1 &= ~(0x7 << SPI_CR1_BR_Pos); // Sets the SPI Baud rate to Fpclk / 2
    SPI1->CR1 |= (SPI_CR1_SPE_Msk); // Enables SPI
}

void SPI_Write(__uint8_t address, __uint8_t *data){
    /*
    This function Writes a message to the SPI Buffer
    the rw_flag arguments represent if the message is to read data from slave or write to him.
    rw_flag = 1 means write and 0 means to read.
    There are a few steps before writing message, first we need to check
    if the "busy flag" is set, and wait if so.
    */
    // Needs to make NSS low when starting a write sequence
      
    while((SPI1->SR >> SPI_SR_BSY_Pos) & 0x1 == 0x1); // wait in a while loop until the busy flag is cleared by HW. 
    GPIOA->ODR &= ~(0x1 << GPIOA_NSS_PIN); // Driving NSS PIN LOW 
    while (*address){
        while((SPI1->SR >> SPI_SR_TXE_Pos) & 0x1 != 0x1); // wait in a while loop until transmit buffer is empty.
        SPI1->DR |= *address; // Write char (8 bit) to the TX Buffer (8b-dataframe).
        address++; // Itirate the pointer to the next address for the next word.
    }
    while(*data){
        while((SPI1->SR >> SPI_SR_TXE_Pos) & 0x1 != 0x1); // wait in a while loop until transmit buffer is empty.
        SPI1->DR |= *data; // Write char (8 bit) to the TX Buffer (8b-dataframe).
        data++; // Itirate the pointer to the next address for the next word.
    }
    if (rw_flag){
        GPIOA->ODR |= (0x1 << GPIOA_NSS_PIN); // Driving NSS PIN HIGH
    }
}

void SPI_Read(){
    /*
    This function Reads from SPI one byte,saves the data and print it for testing.
    we need to wait up until RXNE register is set and then we can read from the
    SR RX buffer register.

    */
    while((SPI1->SR >> SPI_SR_RXNE) & 0x1 != 0x1);
    char temp = (SPI1->DR & 0xFF);
    UART_printf("The Read value from SPI is: %x\r\n", temp);
    GPIOA->ODR |= (0x1 << GPIOA_NSS_PIN); // Setting NSS PIN HIGH

}
