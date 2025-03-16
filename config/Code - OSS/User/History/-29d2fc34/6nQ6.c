#include <stdio.h>
#include "stm32l072xx.h"
#include "gpio.h"
#include "uart.h"

void GPIO_Init(GPIO_Object Obj, GPIO_TypeDef *GPIO){
   /** 
    * @brief Initiate a specific GPIO
    * 
    * It takes all the arguments that has received to the function and sets 
    * the right registers in order to fullfill the requirements.                 
    *
    */
    
    // Clock Initializaiton
    switch (Obj.Name) {
        case 0:
            RCC->IOPENR |= (RCC_IOPENR_GPIOAEN);
            break;
        case 1:
            RCC->IOPENR |= (RCC_IOPENR_GPIOBEN);
            break;
        case 2:
            RCC->IOPENR |= (RCC_IOPENR_GPIOCEN);
            break;
        case 3:
            RCC->IOPENR |= (RCC_IOPENR_GPIODEN);
            break;
        case 4:
            RCC->IOPENR |= (RCC_IOPENR_GPIOEEN);
            break;
        case 5:
            RCC->IOPENR |= (RCC_IOPENR_GPIOHEN);
            break;

    }

    // GPIO Mode Configuration
    GPIO->MODER   &= ~(0x3 << Obj.PinNumber * 2);
    GPIO->MODER   |=  (Obj.Mode << Obj.PinNumber * 2);
    
    // GPIO Output Speed Configuration
    GPIO->OSPEEDR &= ~(0x3 << Obj.PinNumber * 2);
    GPIO->OSPEEDR |=  (Obj.Speed << Obj.PinNumber * 2);
    
    // GPIO PullUp - PullDown
    GPIO->PUPDR   &= ~(0x3 << Obj.PinNumber * 2);
    GPIO->PUPDR   |=  (Obj.Pupr << Obj.PinNumber * 2);
    
    // GPIO Output Type
    GPIO->OTYPER  &= ~(0x1 << Obj.PinNumber);
    if (Obj.Type & 0x1) {
        GPIO->OTYPER  |= (0x1 << Obj.PinNumber);
    }
    switch (Obj.PinNumber) {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
            GPIO->AFR[0] &= ~(0x7 << Obj.PinNumber * 4); 
            GPIO->AFR[0] |=  (Obj.AF << Obj.PinNumber * 4); 
            break;
        default:
            GPIO->AFR[1] &= ~(0x7 << (Obj.PinNumber - 8) * 4); 
            GPIO->AFR[1] |=  (Obj.AF << (Obj.PinNumber - 8) * 4); 
            break;
    }    
}

void GPIO_Set(GPIO_Object Obj, GPIO_TypeDef *GPIO){
    /**
    * @brief Sets the GPIO to output digital high
    * Works only if the GPIO is set as output
    * 
    **/     
    GPIO->ODR |= (0x1 << Obj.PinNumber); 
}

void GPIO_Reset(GPIO_Object Obj, GPIO_TypeDef *GPIO){
    /**
    * @brief Resets the GPIO to output digital Low
    * Works only if the GPIO is set as output
    * */    
    GPIO->ODR &= ~(0x1 << Obj.PinNumber); 

}