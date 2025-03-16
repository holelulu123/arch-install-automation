#include "stm32l072xx.h"
#include "rcc.h"
#include "uart.h"

extern float SystemClock = 0;
extern float PLLVCO = 0;

void SetClock(RCC_Object Obj){
    /**
     * @brief functions sets the System clock of the STM32
     * @param Obj is a struct that defined by the user in board_config.h and sets the
     * wanted settings for the MCU
     * PLL cant be turned on together with MSI
     * 
     *      
     * */ 
    

    // Disable PLL
    RCC->CR & ~(RCC_CR_PLLON);
    while((RCC->CR >> RCC_CR_PLLRDY_Pos) & 0x1);
    
    float defaultFreq;
    // Powering-On The main clock 
    switch(Obj.FirstStageClockType){
        case 0:
            RCC->CR |= (RCC_CR_MSION);
            while(!((RCC->CR >> RCC_CR_MSIRDY_Pos) & 0x1));
            RCC->CR &= ~(RCC_CFGR_SW_Msk);
            RCC->CR |=  (RCC_CFGR_SW_MSI);    
            while(((RCC->CR >> RCC_CFGR_SWS_Pos) & 0x3) != 0x0);
            defaultFreq = MSI_ClockFreq;
            SystemClock = MSI_ClockFreq;
            return; 
        case 1:
            RCC->CR |= (RCC_CR_HSION);
            while(!((RCC->CR >> RCC_CR_HSIRDY_Pos) & 0x1));
            if(!Obj.PLLOn){
                // UART_printf("im here\r\n");
                RCC->CR &= ~(RCC_CFGR_SW_Msk);
                RCC->CR |=  (RCC_CFGR_SW_HSI);
                while(((RCC->CR >> RCC_CFGR_SWS_Pos) & 0x3) != 0x1);
                SystemClock = HSI_ClockFreq;
            }
            defaultFreq = HSI_ClockFreq;
            break; 
        case 2:
            RCC->CR |= (RCC_CR_HSEON);
            while(!((RCC->CR >> RCC_CR_HSERDY_Pos) & 0x1));
            if(!Obj.PLLOn){
                RCC->CR &= ~(RCC_CFGR_SW_Msk);
                RCC->CR |=  (RCC_CFGR_SW_HSE);    
                while(((RCC->CR >> RCC_CFGR_SWS_Pos) & 0x3) != 0x2);
                SystemClock = HSE_ClockFreq;
            }
            defaultFreq = HSE_ClockFreq;
            break;
    }
    // Sets the Primary clock as the system clock
    if (!Obj.PLLOn){
        
    }
    
    else {
        float divider, multiplier;
        switch(Obj.PLLMultiplier){
            case 0:
                multiplier = 3;
                break;
            case 1:
                multiplier = 4;
                break;
            case 2:
                multiplier = 6;
                break;
            case 3:
                multiplier = 8;
                break;
            case 4:
                multiplier = 12;
                break;
            case 5:
                multiplier = 16;
                break;
            case 6:
                multiplier = 24;
                break;
            case 7:
                multiplier = 32;
                break;
            case 8:
                multiplier = 48;
                break;
        }
        switch (Obj.PLLDivider)
        {
        case 1:
            divider = 2;
            break;
        case 2:
            divider = 3;
            break;
        case 3:
            divider = 4;
            break;
        }

        PLLVCO = defaultFreq * multiplier;
        SystemClock = (defaultFreq * multiplier) / divider; 
        FLASH->ACR |= (FLASH_ACR_LATENCY);

        // Exceed Limit
        if (SystemClock > PLL_MAX_FREQ_RANGE_1 || PLLVCO > VCO_MAX_FREQ_RANGE_1){
            return;
        }
        // Range 1 Case (1.8V) VCO > 48mhz and pll is 16  
        else if(PLLVCO > VCO_MAX_FREQ_RANGE_2 && SystemClock == PLL_MAX_FREQ_RANGE_1){
            // PWR->CR |= (RANGE_1);
            // To config the PLL mult and divider
            //Switch to 1 wait - state 



        }
        // Range 2 Case (1.5V)  VCO > 24 mhz
        else if(PLLVCO > VCO_MAX_FREQ_RANGE_3 && PLLVCO <= VCO_MAX_FREQ_RANGE_2 && SystemClock == PLL_MAX_FREQ_RANGE_2){
            // PWR->CR |= (RANGE_2);
            RCC->CFGR |= (Obj.PLLMultiplier << RCC_CFGR_PLLMUL_Pos);
            RCC->CFGR |= (Obj.PLLDivider << RCC_CFGR_PLLDIV_Pos); 
            RCC->CFGR &= ~(RCC_CFGR_PLLSRC);  
        }
        else {
            // PWR->CR |= (RANGE_3);
        }

        RCC->CFGR |=  (Obj.PLLSource << RCC_CFGR_PLLSRC_Pos);
        // Enable the PLL
        RCC->CR |= (RCC_CR_PLLON);
        while(!((RCC->CR >> RCC_CR_PLLRDY_Pos) & 0x1)); // Waits for the PLL to be Locked

        RCC->CFGR &= ~(RCC_CFGR_SW_Msk); 
        RCC->CFGR |= (RCC_CFGR_SW_PLL); 
        while(((RCC->CFGR >> RCC_CFGR_SWS_Pos) & 0x3) != 0x3); // wait for the switch status to be locked on PLL


        FLASH->ACR &= ~(FLASH_ACR_LATENCY);
    }

}

void SetClock_HSI16(){
    /*
    Uses Internal-clock as the main clock.
    */
    
    // Disable PLL 
    RCC->CR & ~(RCC_CR_PLLON);
    while((RCC->CR >> RCC_CR_PLLRDY_Pos) & 0x1);
    
    // HSI16
    RCC->CR |=  (RCC_CR_HSION);
    while(!((RCC->CR >> RCC_CR_HSIRDY_Pos) & 0x1));
    
    
    RCC->CR &= ~(RCC_CFGR_SW_Msk);
    RCC->CR |=  (RCC_CFGR_SW_HSI);
    while(((RCC->CR >> RCC_CFGR_SWS_Pos) & 0x3) != 0x1);
    
    
    // RCC->CR &= ~(0x1 << 24); // Disable the PLL
    // while(((RCC->CR >> 25) & 0b1) == 0b1); // Waits for the PLL to be clear
    
    // Control Registers
    // RCC->CR |= (0x3 << 20); // RTC prescalar is set to 16, needs to learn more about the RTC clock and his purpose.
    // RCC->CR &= ~(0x1 << 18); // Disable the bypass of the External clock (Using local Crystal Oscillator). 
    // RCC->CR &= ~(0x1 << 8); // Disable the MSI Clock
    // RCC->CR &= ~(0x1 << 16); // Disable the HSE Clock
    // RCC->CR |= (0x1 << 0); // Enable the HSI16 Clock
    // while(((RCC->CR >> 2) & 0b1) != 1); // wait for HSI16 clock to be stable and ready
    
    
    // RCC->CFGR |= (0x1 << 0); // System clock switch - HSI16 Choosed
    // while(((RCC->CFGR >> 2) & 0b11) != 0b01); // wait for the switch status to be locked on HSI16
    
    // RCC->CFGR &= ~(0xF << 24); // MCO output disabled, no clock on MCO
}

void SetClock_HSI16_PLL(){
    /*
    Uses Internal-clock as the main clock with PLL stablization
    in this mode we enable HSI16 clock type and PLL, in the RCC section.
    */

    // Changing Voltage scaling range to 1.8V in order to reach VCO of 96MHz and then output clock 32MHz
    PWR->CR |= (0x1 << 12); 
    PWR->CR &= ~(0x1 << 11); 
    while((PWR->CR >> 11) & 0b11 != 0b01);
    
    
    RCC->CR &= ~(0x1 << 24); // Disable the PLL
    while(((RCC->CR >> 25) & 0b1) == 0b1); // Waits for the PLL to be clear
    
    //Switch to 1 wait - state 
    FLASH->ACR |= (0x1 << 0);

    // Control Registers
    RCC->CR |= (0x3 << 20); // RTC prescalar is set to 16, needs to learn more about the RTC clock and his purpose.
    RCC->CR &= ~(0x1 << 18); // Disable the bypass of the External clock (Using local Crystal Oscillator). 
    RCC->CR &= ~(0x1 << 8); // Disable the MSI Clock
    RCC->CR &= ~(0x1 << 16); // Disable the HSE Clock
    RCC->CR |= (0x1 << 0); // Enable the HSI16 Clock
    while(((RCC->CR >> 2) & 0b1) != 1); // wait for HSI16 clock to be stable and ready
    
    
    RCC->CFGR |= (0x1 << 0); // System clock switch - HSI16 Choosed
    while(((RCC->CFGR >> 2) & 0b11) != 0b01); // wait for the switch status to be locked on HSI16
    // Internal Clock sources calibration registers
    // TODO: To check what is the HSI Trim and calibration meaning
    // Clock configuration registers
    
    RCC->CFGR &= ~(0xF << 24); // MCO output disabled, no clock on MCO
    RCC->CFGR |= (0x1 << 18); // PLL multiplication factor by 4
    RCC->CFGR |= (0x1 << 22); // PLL Divider by 2
    RCC->CFGR &= ~(0x1 << 16); // PLL clock source
    
    // Enable the PLL
    RCC->CR |= (0x1 << 24);
    while(((RCC->CR >> 25) & 0b1) != 0b1); // Waits for the PLL to be Locked
    
    RCC->CFGR &= ~(0x3 << 0); // System clock data reset
    RCC->CFGR |= (0x3 << 0); // System clock switch - PLL Choosed
    while(((RCC->CFGR >> 2) & 0b11) != 0b11); // wait for the switch status to be locked on PLL
    

}