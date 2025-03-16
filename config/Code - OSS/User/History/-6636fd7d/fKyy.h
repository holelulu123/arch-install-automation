#ifndef __BOARD_CONFIG_H
#define __BOARD_CONFIG_H

#include "gpio.h"
#include "stm32l072xx.h"

//  Pins Configuration
    /** 
    * This file declare the pins and GPIOs used for each pin I/O
    */
   
// GPIO configuration
#define MOSI_GPIO                   GPIOA 
#define MOSI_PIN                    7

#define MISO_GPIO                   GPIOA
#define MISO_PIN                    6 

#define SCLK_GPIO                   GPIOB
#define SCLK_PIN                    3

#define NSS_GPIO                    GPIOA
#define NSS_PIN                     15

#define SX1276_SPI                  SPI1


// SX1276 Pins
#define TCXO_POWER_GPIO             GPIOA
#define TCXO_POWER_PIN              12

#define ANT_SWITCH_RX_GPIO          GPIOA
#define ANT_SWITCH_RX_PIN           1

#define ANT_SWITCH_TX_BOOST_GPIO    GPIOC
#define ANT_SWITCH_TX_BOOST_PIN     1

#define ANT_SWITCH_TX_RFO_GPIO      GPIOC
#define ANT_SWITCH_TX_RFO_PIN       2

#define RESET_SX1276_GPIO           GPIOC
#define RESET_SX1276_PIN            0

// L80-M39 Pins
#define L80_UART_TX_GPIO            GPIOA
#define L80_UART_TX_PIN             9

#define L80_UART_RX_GPIO            GPIOA
#define L80_UART_RX_PIN             10

// Debugger Pins 
#define Debugger_UART_TX_GPIO       GPIOA
#define Debugger_UART_TX_PIN        2

#define Debugger_UART_RX_GPIO       GPIOA
#define Debugger_UART_RX_PIN        3


static const GPIO_Object GPIOB_5 = {
    .PinNumber          = GPIO_PIN_5,
    .Name               = GPIO_B,
    .Mode               = OutputMode,
    .Speed              = MediumSpeed,
    .Pupr               = NoPull,
    .Type               = PushPull,
    .AF                 = AF0
};

static const RCC_Object GPIOB_5 = {
    .FirstStageClockType    = HSI16,
    .PLLOn                  = PLL_On,
    .PLLMultiplier          = OutputMode,
    .PLLDivider             = MediumSpeed,
    .PLLSource              = NoPull,
    .Type                   = PushPull,
    .AF                     = AF0
};
// Initializtion of 
typedef struct RCC_Object {
    .FirstStageClockType            ;
    .PLLOn                          ;
    .PLLMultiplier                  ;
    .PLLDivider                     ;
    .PLLSource                      ;
    
}RCC_Object;

#endif