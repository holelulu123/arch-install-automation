#include "stm32l072xx.h"

int main()
{
    // 1. Enable GPIOB clock
    RCC->IOPENR |= (1 << 1);
    
    // 2. Set PA5 as output
    GPIOA->MODER &= ~(0x3 << 10); // Clear bits
    GPIOA->MODER |= (0x1 << 10);  // Set as output
    // 3. Set PA5 HIGH (turn on LED)
    GPIOA->ODR |= (0x1 << 5);

    // Infinite loop to keep the program running
    while (1);
}
