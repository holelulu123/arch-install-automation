#include <stdio.h>
#include "stm32l072xx.h"
#include "board_config.h"
#include "spi.h"
#include "uart.h"
#include "sx_1276.h"


void Configurates_SPI(SPI_TypeDef *SPI_interface){
    /*

    This function sets the registers for SPI1 communication
    for half-duplex.
    */
    
    // Enable Clock for SPI1, GPIOA and GPIOB
    RCC->APB2ENR |= (RCC_APB2ENR_SPI1EN); // Enable the SPI1 Clock
    RCC->IOPENR |= (RCC_IOPENR_GPIOAEN); // Enable the Clock of GPIO A
    RCC->IOPENR |= (RCC_IOPENR_GPIOBEN); // Enable the Clock of GPIO B
    
    // GPIO CONFIGURATION 
    MISO_GPIO->MODER &= ~(0x3 << MISO_PIN * 2); // Clear MODER Register 
    MOSI_GPIO->MODER &= ~(0x3 << MOSI_PIN * 2); // Clear MODER Register 
    NSS_GPIO->MODER &= ~(0x3 << NSS_PIN * 2); // Clear MODER Register 
    SCLK_GPIO->MODER &= ~(0x3 << SCLK_PIN * 2); // Clear MODER Register 
    
    MISO_GPIO->MODER |= (0x2 << MISO_PIN * 2); // Select MISO for Altenrate function
    MOSI_GPIO->MODER |= (0x2 << MOSI_PIN * 2); // Select MOSI for Altenrate function
    NSS_GPIO->MODER |= (0x1 << NSS_PIN * 2); // Select NSS for output mode
    SCLK_GPIO->MODER |= (0x2 << SCLK_PIN * 2); // Select SCLK for Altenrate function
    
    // GPIOA->AFR[1] |= (0x0 << (GPIOA_TCXO_POWER - 8) * 4); // Select PA12 for normal GPIO -> Needs to change Here!
    MISO_GPIO->AFR[0] &= ~(0xF << MISO_PIN * 4); // Select PA6 for MISO (AFSEL 0)
    MOSI_GPIO->AFR[0] &= ~(0xF << MOSI_PIN * 4); // Select PA7 for MOSI (AFSEL 0) 
    NSS_GPIO->AFR[1] &= ~(0xF << (NSS_PIN - 8) * 4); // Select PA15 for NSS (AFSEL 0)
    SCLK_GPIO->AFR[0] &= ~(0xF << SCLK_PIN * 4); // Select PB3 for PSCLK (AFSEL 0)

    // GPIOA->PUPDR &= ~(0x3 << GPIOA_TCXO_POWER); // Resets value for PA12 
    // GPIOA->PUPDR |= (0x1 << GPIOA_TCXO_POWER); // Select pull-up for PA12
    MISO_GPIO->PUPDR |= (0x0 << MISO_PIN * 2); // Select Pull-down for PA6
    MOSI_GPIO->PUPDR |= (0x0 << MOSI_PIN * 2); // Select Pull-down for PA7
    NSS_GPIO->PUPDR &= ~(0x3 << NSS_PIN * 2); // Select no-pull for PA15
    SCLK_GPIO->PUPDR |= (0x0 << SCLK_PIN * 2); // Select pull-down for PB3

    // GPIOA->OTYPER &= ~(GPIO_OTYPER_OT_12); // Select push-pull for PA12
    MISO_GPIO->OTYPER &= ~(GPIO_OTYPER_OT_6); // Select push-pull for PA6
    MOSI_GPIO->OTYPER &= ~(GPIO_OTYPER_OT_7); // Select push-pull for PA7
    NSS_GPIO->OTYPER &= ~(GPIO_OTYPER_OT_15); // Select push-pull for PA15
    SCLK_GPIO->OTYPER &= ~(GPIO_OTYPER_OT_3); // Select push-pull for PB3

    MISO_GPIO->OSPEEDR |= (0x2 << MISO_PIN * 2); // Select Output speed of High speed for PA5
    MOSI_GPIO->OSPEEDR |= (0x2 << MOSI_PIN * 2); // Select Output speed of High speed for PA4
    NSS_GPIO->OSPEEDR |= (0x2 << NSS_PIN * 2); // Select Output speed of High speed for PA6
    SCLK_GPIO->OSPEEDR |= (0x2 << SCLK_PIN * 2); // Select Output speed of High speed for PA7

    NSS_GPIO->BSRR = GPIO_BSRR_BS_15; // Setting NSS High
    
    // Enabling TCXO POWER
    TCXO_POWER_GPIO->ODR |= (0x1 << TCXO_POWER_PIN); // Outputs 1 through the PA12
    
    // SPI CONFIGURATION
    SPI_interface->CR1 |= (SPI_CR1_SSI | SPI_CR1_SSM);
    SPI_interface->CR1 &= ~(SPI_CR1_DFF | SPI_CR1_CPOL | SPI_CR1_CPHA); // Clears the data frame format to 0, make it 8-bit frame, and clock polarity and clock phase to 0.
    SPI_interface->CR1 |= (SPI_CR1_MSTR);// | SPI_CR1_SSM); // Sets the MCU to the master, sets software salve managment enabled.
    SPI_interface->I2SCFGR &= ~(SPI_I2SCFGR_I2SMOD); // SPI Mode is selected
    SPI_interface->CR1 |= (0x3 << SPI_CR1_BR_Pos); // Sets the SPI Baud rate to Fpclk / 16 = 1 MHz
    SPI_interface->CR2 |= (SPI_CR2_SSOE);
    SPI_interface->CR1 |= (SPI_CR1_SPE_Msk); // Enables SPI
}

void SPI_WriteRegister(__uint8_t address, __uint8_t data, SPI_TypeDef *SPI_interface){
    /*
    This function writes a data (8 bit) to a register
    */
    address |= (0x1 << 7); // Adds 1 to the MSB of the address to represent a write operation
    __uint8_t word[2] = {address, data}; 
    __uint8_t temp;
    NSS_GPIO->BSRR = GPIO_BSRR_BR_15;
    for(int i = 0; i < sizeof(word); i++){
        while(!(SPI_interface->SR & SPI_SR_TXE)); 
        SPI_interface->DR = word[i];
        while(!(SPI_interface->SR & SPI_SR_RXNE));
        temp = SPI_interface->DR;
    } 
    NSS_GPIO->BSRR = GPIO_BSRR_BS_15;
}

__uint8_t SPI_ReadRegister(__uint8_t address, SPI_TypeDef *SPI_interface){
    /*
    This function reads a data from address sent.
    it gets address as an argument send a read requests 
    and gets an answer that represent the data of the register
    at the time.
    
    */

    // Needs to make NSS low when starting a write sequence
    __uint8_t temp;
    __uint8_t data[2] = {address & 0x7F, 0x0}; // Creates a data and dummy to let the NSS stays and Sclk to continue transfering clock
    NSS_GPIO->BSRR = GPIO_BSRR_BR_15;
    // Transmit operation
    for (int i = 0; i < sizeof(data); i++){
        SPI_interface->DR = data[i]; // Write char (8 bit) to the TX Buffer (8b-dataframe).
        while(!(SPI_interface->SR & SPI_SR_TXE)); 
        while(!(SPI_interface->SR & SPI_SR_RXNE)); // wait in a while loop until the busy flag is cleared by HW. 
        temp = SPI_interface->DR;
    }
    NSS_GPIO->BSRR = GPIO_BSRR_BS_15;
    return temp;
}
