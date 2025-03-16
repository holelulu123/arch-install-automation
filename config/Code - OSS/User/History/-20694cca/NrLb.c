#define STM32L0xx
#include "stm32l072xx.h"
#include "rcc.h"
#include "gpio.h"
#include "uart.h"
#include "board_config.h"
#include "main.h"
#include "spi.h"
#include "l80_m39.h"
#include "sx_1276.h"

void SystemInit()
{  
}

void delay_ms(uint32_t ms) {
    for (int i = 0; i < ms * SystemClock / 1000; i++);
}

int main(void)
{
    SetClock(Clock_Configuration);
    UART_SetRegisters(UART_Debug);
    __uint8_t transmit_word[] = {0x68, 0x65 ,0x6C ,0x6C, 0x6F ,0x20 ,0x77 ,0x6F ,0x72 ,0x6C ,0x64};
    // __uint8_t transmit_word[] = {0x68};
    __uint8_t arr_size = sizeof(transmit_word) / sizeof(transmit_word[0]);
    float freq = 915000000;
    __uint16_t preamble = 0;
    __uint8_t* text;
    __uint8_t sync_word;
    __uint8_t  transmitter = 1; // this value defines if the device is transmitter (master) or receiver (slave)
    SX1276_Init(SX1276_Obj);
    SX1276_SetFreq(SX1276_Obj, freq);
    SX1276_SetSF(SX1276_Obj, LoRa_SF_9);
    SX1276_SetBW(SX1276_Obj, LoRa_Bw_62_5);
    while(1){
        switch (transmitter){
            case 0:
                text = SX1276_RxCon(SX1276_Obj);
                UART_printf("%s\r\n",text);
                break;
            case 1:
                SX1276_GetSyncWord
                // SX1276_SetPrambleLength(SX1276_Obj, preamble);
                // SX1276_Tx(SX1276_Obj, transmit_word, arr_size);
                // delay_ms(1);
                
                // SX1276_ResetImplicitMode(SX1276_Obj);
                // SX1276_Tx(SX1276_Obj, transmit_word, arr_size);
                // delay_ms(1);
                break;
        }
    }   

}

