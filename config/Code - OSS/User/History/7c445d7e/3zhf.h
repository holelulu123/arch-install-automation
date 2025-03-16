#ifndef __RCC_H
#define __RCC_H

#define HSI_ClockFreq 16000000
#define MSI_ClockFreq 2097000
#define HSE_ClockFreq 16000000

typedef enum RCC_FirstStageClockType : __uint8_t {
    MSI             = 0x0,
    HSI16           = 0x1,
    HSE             = 0x2,
    
}RCC_FirstStageClockType;

typedef enum RCC_PLLOnTag : __uint8_t {
    PLL_Off             = 0x0,
    PLL_On              = 0x1,
    
}RCC_PLLOn;

typedef enum RCC_PLLMultiplierTag : __uint8_t {
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

typedef enum RCC_PLLDividerTag : __uint8_t {
    Divider_2        = 0x1,
    Divider_3        = 0x2,
    Divider_4        = 0x3,
    
}RCC_PLLDivider;

typedef enum RCC_PLLSourceTag : __uint8_t {
    HSI16            = 0x0,
    HSE              = 0x1,
    
}RCC_PLLSource;

typedef enum RCC_SystemClockTag : __uint8_t {
    MSI              = 0x0,
    HSI16            = 0x1,
    HSE              = 0x2,
    PLL              = 0x3,
    
}RCC_SystemClock;

/**
 * RCC Object 
 */
typedef struct RCC_Object {
    RCC_FirstStageClockType     FirstStageClockType;
    RCC_PLLOn                   PLLOn;
    RCC_PLLMultiplier           PLLMultiplier;
    RCC_PLLDivider              PLLDivider;
    RCC_PLLSource               PLLSource;
    
}RCC_Object;


extern int SystemClock;

void SetClock_HSI16();
void SetClock_HSI16_PLL();

#endif