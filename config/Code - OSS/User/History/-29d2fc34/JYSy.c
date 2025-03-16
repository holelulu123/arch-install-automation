#include <stdio.h>
#include "stm32l072xx.h"
#include "gpio.h"
#include "uart.h"

void GPIO_Init(GPIO_Object Obj){
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
    Obj.GPIOP->MODER   &= ~(0x3 << Obj.PinNumber * 2);
    Obj.GPIOP->MODER   |=  (Obj.Mode << Obj.PinNumber * 2);
    
    switch (Obj.PinNumber) {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
            Obj.GPIOP->AFR[0] &= ~(0x7 << Obj.PinNumber * 4); 
            Obj.GPIOP->AFR[0] |=  (Obj.AF << Obj.PinNumber * 4); 
            break;
        default:
            GPIO_Set(GPIOB_5);
            Obj.GPIOP->AFR[1] &= ~(0x7 << (Obj.PinNumber - 8) * 4); 
            Obj.GPIOP->AFR[1] |=  (Obj.AF << (Obj.PinNumber - 8) * 4); 
            break;
    }    

    // GPIO PullUp - PullDown
    Obj.GPIOP->PUPDR   &= ~(0x3 << Obj.PinNumber * 2);
    Obj.GPIOP->PUPDR   |=  (Obj.Pupr << Obj.PinNumber * 2);
    
    // GPIO Output Type
    Obj.GPIOP->OTYPER  &= ~(0x1 << Obj.PinNumber);
    Obj.GPIOP->OTYPER  |=  (Obj.Type << Obj.PinNumber);
    
    // GPIO Output Speed Configuration
    Obj.GPIOP->OSPEEDR &= ~(0x3 << Obj.PinNumber * 2);
    Obj.GPIOP->OSPEEDR |=  (Obj.Speed << Obj.PinNumber * 2);

}

void GPIO_Set(GPIO_Object Obj){
    /**
    * @brief Sets the GPIO to output digital high
    * Works only if the GPIO is set as output
    * 
    **/     
    Obj.GPIOP->ODR |= (0x1 << Obj.PinNumber); 
}

void GPIO_Reset(GPIO_Object Obj){
    /**
     * @brief Resets the GPIO to output digital Low
     * Works only if the GPIO is set as output
     * */    
    Obj.GPIOP->ODR &= ~(0x1 << Obj.PinNumber); 

}

__uint8_t GPIO_Read(GPIO_Object Obj){
    /**
     * @brief Reads the GPIO digital value
     * works only if GPIO is set to be Input.
     * Returns the value of the GPIO Pin.
     */
    return ((Obj.GPIOP->IDR >> Obj.PinNumber) & 0x1);
}