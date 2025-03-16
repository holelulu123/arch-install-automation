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

typedef enum  RCC_PLLMultiplierTag : __uint8_t {
    Multiplier_3        = 0x0,
    Multiplier_4        = 0x1,
    Multiplier_6        = 0x2,
    Multiplier_8        = 0x3,
    Multiplier_12       = 0x4,
    Multiplier_16       = 0x5,
    Multiplier_24       = 0x6,
    Multiplier_32       = 0x7,
    Multiplier_48       = 0x8,

}RCC_PLLMultiplier;


void SetClock_HSI16();
void SetClock_HSI16_PLL();

#endif