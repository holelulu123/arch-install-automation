#include "stm32l072xx.h"





int main(void)
{
    // Enable GPIOA clock
    RCC->IOPENR |= (0x1 << 0);
    RCC->IOPSMENR |= (0x1 << 0);

    GPIOA->MODER &= ~(0x3 << 10); // Resets all the unnecessary pins
    GPIOA->MODER |= (0x1 << 10);  // Set as output

    GPIOA->ODR |= (0x1 << 5);

    while(1);
}
