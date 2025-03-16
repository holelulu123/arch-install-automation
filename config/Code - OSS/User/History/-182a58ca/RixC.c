#include "stm32l072xx.h"

int main(void)
{
    // Enable GPIOA clock
    RCC->IOPENR |= (0x1 << 0);
    RCC->IOPSMENR |= (0x1 << 0);

    GPIOA->MODER &= ~(0x3 << 10);
    GPIOA->MODER |= (0x1 << 10);  // Set as output
    // GPIOA->OTYPER |= (0x0 << 5); // Output Push-pull 

    // GPIOA->ODR |= (0x1 << 5);

    // Infinite loop to keep the program running
    // GPIOA->BSRR |= (0x1 << 21);
    
    // while (1){
        // GPIOA->BSRR |= (0x1 << 5);
        // for(int i = 0; i < 1000000; i++);
        // GPIOA->BSRR |= (0x1 << 21);
    // }
    while(1);
}
