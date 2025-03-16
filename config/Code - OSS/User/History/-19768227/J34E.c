#include <stdio.h>
#include "stm32l072xx.h"
#include "spi.h"
#include "uart.h"
#include "sx_1276.h"

void Configurates_SPI1(){
    /*
    This function sets the registers for SPI1 communication
    for half-duplex.
    */
    
    // Enable Clock for SPI1, GPIOA and GPIOB
    RCC->APB2ENR |= (RCC_APB2ENR_SPI1EN); // Enable the SPI1 Clock
    RCC->IOPENR |= (RCC_IOPENR_GPIOAEN); // Enable the Clock of GPIO A
    RCC->IOPENR |= (RCC_IOPENR_GPIOBEN); // Enable the Clock of GPIO B
    
    // GPIO CONFIGURATION 
    GPIOA->MODER &= ~(0x3 << GPIOA_MISO_PIN * 2); // Clear MODER Register 
    GPIOA->MODER &= ~(0x3 << GPIOA_MOSI_PIN * 2); // Clear MODER Register 
    GPIOA->MODER &= ~(0x3 << GPIOA_NSS_PIN * 2); // Clear MODER Register 
    GPIOB->MODER &= ~(0x3 << GPIOB_SCLK_PIN * 2); // Clear MODER Register 
    
    GPIOA->MODER |= (0x2 << GPIOA_MISO_PIN * 2); // Select MISO for Altenrate function
    GPIOA->MODER |= (0x2 << GPIOA_MOSI_PIN * 2); // Select MOSI for Altenrate function
    GPIOA->MODER |= (0x1 << GPIOA_NSS_PIN * 2); // Select NSS for output mode
    GPIOB->MODER |= (0x2 << GPIOB_SCLK_PIN * 2); // Select SCLK for Altenrate function
    
    // GPIOA->AFR[1] |= (0x0 << (GPIOA_TCXO_POWER - 8) * 4); // Select PA12 for normal GPIO -> Needs to change Here!
    GPIOA->AFR[0] &= ~(0xF << GPIOA_MISO_PIN * 4); // Select PA6 for MISO (AFSEL 0)
    GPIOA->AFR[0] &= ~(0xF << GPIOA_MOSI_PIN * 4); // Select PA7 for MOSI (AFSEL 0) 
    GPIOA->AFR[1] &= ~(0xF << (GPIOA_NSS_PIN - 8) * 4); // Select PA15 for NSS (AFSEL 0)
    GPIOB->AFR[0] &= ~(0xF << GPIOB_SCLK_PIN * 4); // Select PB3 for PSCLK (AFSEL 0)

    // GPIOA->PUPDR &= ~(0x3 << GPIOA_TCXO_POWER); // Resets value for PA12 
    GPIOA->PUPDR |= (0x1 << GPIOA_TCXO_POWER); // Select pull-up for PA12
    GPIOA->PUPDR |= (0x0 << GPIOA_MISO_PIN * 2); // Select Pull-down for PA6
    GPIOA->PUPDR |= (0x0 << GPIOA_MOSI_PIN * 2); // Select Pull-down for PA7
    GPIOA->PUPDR &= ~(0x3 << GPIOA_NSS_PIN * 2); // Select no-pull for PA15
    GPIOB->PUPDR |= (0x0 << GPIOB_SCLK_PIN * 2); // Select pull-down for PB3

    // GPIOA->OTYPER &= ~(GPIO_OTYPER_OT_12); // Select push-pull for PA12
    GPIOA->OTYPER &= ~(GPIO_OTYPER_OT_6); // Select push-pull for PA6
    GPIOA->OTYPER &= ~(GPIO_OTYPER_OT_7); // Select push-pull for PA7
    GPIOA->OTYPER &= ~(GPIO_OTYPER_OT_15); // Select push-pull for PA15
    GPIOB->OTYPER &= ~(GPIO_OTYPER_OT_3); // Select push-pull for PB3

    GPIOA->OSPEEDR |= (0x2 << GPIOA_MISO_PIN * 2); // Select Output speed of High speed for PA5
    GPIOA->OSPEEDR |= (0x2 << GPIOA_MOSI_PIN * 2); // Select Output speed of High speed for PA4
    GPIOA->OSPEEDR |= (0x2 << GPIOA_NSS_PIN * 2); // Select Output speed of High speed for PA6
    GPIOB->OSPEEDR |= (0x2 << GPIOB_SCLK_PIN * 2); // Select Output speed of High speed for PA7

    GPIOA->BSRR = GPIO_BSRR_BS_15; // Setting NSS High
    // Enabling TCXO POWER
    // GPIOA->ODR |= (0x1 << GPIOA_TCXO_POWER); // Outputs 1 through the PA12
    
    // SPI CONFIGURATION
    SPI1->CR1 |= (SPI_CR1_SSI | SPI_CR1_SSM);
    SPI1->CR1 &= ~(SPI_CR1_DFF | SPI_CR1_CPOL | SPI_CR1_CPHA); // Clears the data frame format to 0, make it 8-bit frame, and clock polarity and clock phase to 0.
    SPI1->CR1 |= (SPI_CR1_MSTR);// | SPI_CR1_SSM); // Sets the MCU to the master, sets software salve managment enabled.
    SPI1->I2SCFGR &= ~(SPI_I2SCFGR_I2SMOD); // SPI Mode is selected
    SPI1->CR1 |= (0x3 << SPI_CR1_BR_Pos); // Sets the SPI Baud rate to Fpclk / 16 = 1 MHz
    SPI1->CR2 |= (SPI_CR2_SSOE);
    SPI1->CR1 |= (SPI_CR1_SPE_Msk); // Enables SPI
}

void SPI_WriteRegister(__uint8_t address, __uint8_t data){
    /*
    This function writes a data (8 bit) to a register
    */
    address |= (0x1 > 7); // Adds 1 to the MSB of the address to represent a write operation
    __uint8_t word[2] = {address, data}; 
    // GPIOA->ODR &= ~(0x1 << GPIOA_NSS_PIN); // Driving NSS PIN LOW
    GPIOA->BSRR = GPIO_BSRR_BR_15;
    for(int i = 0; i < sizeof(word); i++){
        while((SPI1->SR >> SPI_SR_TXE_Pos) & 0x1 != 0x1); 
        SPI1->DR = word[i];
    } 
    // GPIOA->ODR |= (0x1 << GPIOA_NSS_PIN); // Driving NSS PIN HIGH
    GPIOA->BSRR = GPIO_BSRR_BS_15;
}

void SPI_ReadRegister(__uint8_t address){
    /*
    This function reads a data from address sent.
    it gets address as an argument send a read requests 
    and gets an answer that represent the data of the register
    at the time.
    
    */

    // Needs to make NSS low when starting a write sequence
    __uint8_t temp;
    GPIOA->BSRR = GPIO_BSRR_BR_15;
    // GPIOA->ODR &= ~(0x1 << GPIOA_NSS_PIN); // Driving NSS PIN LOW
    // Transmit operation
    while(!(SPI1->SR & SPI_SR_TXE)); 
    while(!(SPI1->SR & SPI_SR_RXNE)); // wait in a while loop until the busy flag is cleared by HW. 
    SPI1->DR = address; // Write char (8 bit) to the TX Buffer (8b-dataframe).
    temp = SPI1->DR;
    // Receive operation
    SPI1->DR = 0x0; // Sends a dummy data.
    while(!(SPI1->SR & SPI_SR_TXE)); 
    while(!(SPI1->SR & SPI_SR_RXNE)); // wait in a while loop until the busy flag is cleared by HW. 
    temp = SPI1->DR;  
    GPIOA->ODR |= (0x1 << GPIOA_NSS_PIN); // Setting NSS PIN HIGH
    GPIOA->BSRR = GPIO_BSRR_BS_15;
    UART_printf("The Read value from address 0x%x is: 0x%x\r\n", address, temp);
    
}
