#include "stm32l072xx.h"





int main(void)
{
    
    RCC->IOPENR |= (0x1 << 0); // Enable GPIOA clock
    RCC->IOPSMENR |= (0x1 << 0); //Enable the GPIO A

    GPIOA->MODER &= ~(0x3 << 10); // Resets all the unnecessary pins
    GPIOA->MODER |= (0x1 << 10);  // Set as output

    GPIOA->ODR |= (0x1 << 5); // Outputs 1 through the PA5 

    while(1);
}
