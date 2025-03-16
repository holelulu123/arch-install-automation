#ifndef __GPIO_H
#define __GPIO_H
#include <stdio.h>
#include "stm32l072xx.h"

/**
 * changes the mode of the GPIO Pin for either reception or 
 * transmission of data, or either analog. 
 */
typedef enum GPIOPinNumberTag : __uint8_t {
    GPIO_PIN_0      = 0x0,
    GPIO_PIN_1      = 0x1,
    GPIO_PIN_2      = 0x2,
    GPIO_PIN_3      = 0x3,
    GPIO_PIN_4      = 0x4,
    GPIO_PIN_5      = 0x5,
    GPIO_PIN_6      = 0x6,
    GPIO_PIN_7      = 0x7,
    GPIO_PIN_8      = 0x8,
    GPIO_PIN_9      = 0x9,
    GPIO_PIN_10     = 0xA,
    GPIO_PIN_11     = 0xB,
    GPIO_PIN_12     = 0xC,
    GPIO_PIN_13     = 0xD,
    GPIO_PIN_14     = 0xE,
    GPIO_PIN_15     = 0xF

}GPIOPinNumber;

typedef enum GPIOPinModesTag : __uint8_t {
    InputMode       = 0x0,
    OutputMode      = 0x1,
    AlternateMode   = 0x2,
    AnalogMode      = 0x3
    
}GPIOPinModes;

typedef enum GPIOPinTypeTag : __uint8_t {
    PushPull        = 0x0,
    OpenDrain       = 0x1

}GPIOPinType;

/**
 * GPIO Output speed.
 */
typedef enum GPIOPinOutputSpeed : __uint8_t {
    LowSpeed        = 0x0,
    MediumSpeed     = 0x1,
    HighSpeed       = 0x2,
    VeryHighSpeed   = 0x3

}GPIOPinOutputSpeed;

/**
 * 
 * GPIO Pullup-PullDown, GPIO can be pulled to a voltage high, low or none.
 * 
 */
typedef enum GPIOPinPUPRTag : __uint8_t {
    NoPull          = 0x0,    
    PullUp          = 0x1,    
    PullDown        = 0x2    

}GPIOPinPUPR;
/**
 * GPIO Alternate functions, GPIO functionality can be changed, respectfully to 
 * the datasheet 
 */ 
typedef enum GPIOAlternateFunctionTag : __uint8_t {
    AF0             = 0x0,
    AF1             = 0x1,
    AF2             = 0x2,
    AF3             = 0x3,
    AF4             = 0x4,
    AF5             = 0x5,
    AF6             = 0x6,
    AF7             = 0x7

}GPIOAlternateFunction; 

typedef enum GPIONameTag : __uint8_t {
    GPIO_A          = 0,
    GPIO_B          = 1,
    GPIO_C          = 2,
    GPIO_D          = 3,
    GPIO_E          = 4,
    GPIO_F          = 5

}GPIOName;

typedef struct GPIO_Object{
    GPIOPinNumber           PinNumber;
    GPIOName                Name;
    GPIOPinModes            Mode;
    GPIOPinOutputSpeed      Speed;
    GPIOPinPUPR             Pupr;
    GPIOPinType             Type;
    GPIOAlternateFunction   AF;  

}GPIO_Object;



void GPIO_Init(GPIO_Object Obj ,GPIO_TypeDef *GPIO);

#endif