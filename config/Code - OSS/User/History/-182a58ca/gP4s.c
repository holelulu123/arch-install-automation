#include "stm32l072xx.h"





int main(void)
{
    
    RCC->IOPENR |= (0x1 << 0); // Enable GPIOA clock
    RCC->IOPSMENR |= (0x1 << 0); //Enable the GPIO A
    
    RCC->IOPENR |= (0x1 << 1); // Enable GPIOB clock
    RCC->IOPSMENR |= (0x1 << 1); //Enable the GPIO B

    GPIOA->MODER &= ~(0x3 << 10); // Resets all the unnecessary pins
    GPIOA->MODER |= (0x1 << 10);  // Set as output
    
    GPIOB->MODER &= ~(0x3 << 10); // Resets all the unnecessary pins
    GPIOB->MODER |= (0x1 << 10);  // Set as output
    
    GPIOB->MODER &= ~(0x3 << 12); // Resets all the unnecessary pins
    GPIOB->MODER |= (0x1 << 12);  // Set as output
    
    GPIOB->MODER &= ~(0x3 << 14); // Resets all the unnecessary pins
    GPIOB->MODER |= (0x1 << 14);  // Set as output

    GPIOA->ODR |= (0x1 << 5); // Outputs 1 through the PA5 
    GPIOB->ODR |= (0x1 << 5); // Outputs 1 through the PB5 
    GPIOB->ODR |= (0x1 << 6); // Outputs 1 through the PB5 
    GPIOB->ODR |= (0x1 << 7); // Outputs 1 through the PB5 

    while(1);
}
