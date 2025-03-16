#include "stm32l072xx.h"

int main()
{
    // Enable GPIOB clock
    RCC->IOPENR |= (0 << 1);
    
    
    GPIOA->MODER |= (0xFFFFFFFF); // Sets all bits to reset state
    
    GPIOA->MODER |= (0x1 << 10);  // Set as output
    GPIOA->OTYPER |= (0x0 << 5); // Output Push-pull 

    GPIOA->ODR |= (0x1 << 5);

    // Infinite loop to keep the program running
    while (1);
}
