#ifndef __GPIO_H
#define __GPIO_H
#include <stdio.h>
#include "stm32l072xx.h"

/**
 * changes the mode of the GPIO Pin for either reception or 
 * transmission of data, or either analog. 
 */
typedef enum : __uint8_t {


}GPIOPinNumber;

typedef enum : __uint8_t {
    InputMode       = 0x0,
    OutputMode      = 0x1,
    AlternateMode   = 0x2,
    AnalogMode      = 0x3,
    
}GPIOPinModes;

typedef enum : __uint8_t {
    PushPull        = 0x0,
    OpenDrain       = 0x1,

}GPIOPinType;

/**
 * GPIO Output speed.
 */
typedef enum : __uint8_t {
    LowSpeed        = 0x0,
    MediumSpeed     = 0x1,
    HighSpeed       = 0x2,
    VeryHighSpeed   = 0x3,

}GPIOPinOutputSpeed;

/**
 * 
 * GPIO Pullup-PullDown, GPIO can be pulled to a voltage high, low or none.
 * 
 */
typedef enum : __uint8_t {
    NoPull          = 0x0,    
    PullUp          = 0x1,    
    PullDown        = 0x2,    

}GPIOPinPUPR;
/**
 * GPIO Alternate functions, GPIO functionality can be changed, respectfully to 
 * the datasheet 
 */ 
typedef enum : __uint8_t {
    AF0             = 0x0,
    AF1             = 0x1,
    AF2             = 0x2,
    AF3             = 0x3,
    AF4             = 0x4,
    AF5             = 0x5,
    AF6             = 0x6,
    AF7             = 0x7,

}GPIOAlternateFunction; 

typedef enum : char* {
    GPIO_A          = "GPIOA",
    GPIO_B          = "GPIOB",
    GPIO_C          = "GPIOC",
    GPIO_D          = "GPIOD",
    GPIO_E          = "GPIOE",

}GPIOName;

typedef struct {
    GPIOName                Name;
    GPIOPinModes            Mode;
    GPIOPinOutputSpeed      Speed;
    GPIOPinPUPR             Pupr;
    GPIOPinType             Type;
    GPIOAlternateFunction   AF;  

}GPIO_Object;

void GPIO_Init(GPIO_Object *Obj ,GPIO_TypeDef *GPIO, __uint8_t Pin_number);

#endif