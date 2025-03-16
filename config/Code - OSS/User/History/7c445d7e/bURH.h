#ifndef __RCC_H
#define __RCC_H

typedef enum RCC_FirstStageClockType : __uint8_t {
    MSI             = 0,
    HSI16           = 1,
    HSE             = 2,

}RCC_FirstStageClockType;

typedef enum  RCC_PLLOnTag : __uint8_t {
    PLL_Off             = 0,
    PLL_On              = 1,
}RCC_PLLOn;


void SetClock_HSI16();
void SetClock_HSI16_PLL();

#endif