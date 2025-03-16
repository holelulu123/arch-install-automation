#ifndef __RCC_H
#define __RCC_H

typedef enum GPIONameTag : __uint8_t {
    GPIO_A          = 0,
    GPIO_B          = 1,
    GPIO_C          = 2,
    GPIO_D          = 3,
    GPIO_E          = 4,
    GPIO_H          = 5

}ClockType;


void SetClock_HSI16();
void SetClock_HSI16_PLL();

#endif