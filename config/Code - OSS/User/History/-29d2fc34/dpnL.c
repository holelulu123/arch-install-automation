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
    switch(Obj.Mode) {
        
    }


    UART_printf("GPIO Pin number is: %d \r\n",Obj.PinNumber);
    UART_printf("GPIO Mode is: %d\r\n",Obj.Mode);
    UART_printf("GPIO Speed is: %d\r\n",Obj.Speed);
    UART_printf("GPIO pupr is: %d\r\n",Obj.Pupr);
    UART_printf("GPIO Type is: %d\r\n",Obj.Type);
    UART_printf("GPIO AF is: %d\r\n",Obj.AF);
    
    
    
    
}