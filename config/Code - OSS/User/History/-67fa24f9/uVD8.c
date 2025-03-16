#include <stdio.h>
#include <cstring>
#include "board_config.h"
#include "sx_1276.h"
#include "gpio.h"
#include "stm32l072xx.h"
#include "spi.h"
#include "uart.h"

static __uint8_t RxPayloadCrcOn = 0;
static __uint8_t SX1276_FifoPtrTx = SX1276_FifoTxBaseAddr; 
static __uint8_t SX1276_FifoPtrRx = SX1276_FifoRxBaseAddr;
void SX1276_RxPayloadCrc(SX1276_Object Obj, __uint8_t setReset){
    /**
     * Sets / Resets the RxPayloadCrcOn bit on the RegModemConfig2 Register
     * Set = 1, Reset 0
     */
    __uint8_t ModemConfig2 = SPI_ReadRegister(RegModemConfig2, Obj.SPI_Obj);
    if (setReset){
        ModemConfig2 |= (RegModemConfig2_RxPayloadCrcOn);
        RxPayloadCrcOn = 1;
    }
    else{
        ModemConfig2 &= (RegModemConfig2_RxPayloadCrcOn);
        RxPayloadCrcOn = 0;
    }
    SPI_WriteRegister(RegModemConfig2, ModemConfig2);

}

void SX1276_ClearIrq(SX1276_Object Obj, __uint8_t bitToClear){
    /**
     * This function writes 1 to ReqIrqFlags register specific bit and clears the IRQ
     */
    __uint8_t reg_status = SPI_ReadRegister(RegIrqFlagsMask, Obj.SPI_Obj);
    reg_status |= (0x1 << bitToClear);
    SPI_WriteRegister(RegIrqFlags, reg_status, Obj.SPI_Obj);
}

__uint8_t SX1276_GetSyncWord(SX1276_Object Obj){
    /**
     * Reads the value of the sync word of the device
     */
    __uint8_t word;
    word = SPI_ReadRegister(RegSyncWord, Obj.SPI_Obj);
    return word;
}

void SX1276_GetFeiValue(SX1276_Object Obj){
    /**
     * Reads the Inidicated RF Center frequency error and return the value
     */
    int value = 0;
    __uint8_t sign = ((SPI_ReadRegister(RegFeiMsb, Obj.SPI_Obj) >> 19) & 0x1); 
    value |= ((SPI_ReadRegister(RegFeiMsb, Obj.SPI_Obj) >> 16) & 0x7); 
    value |= ((SPI_ReadRegister(RegFeiMid, Obj.SPI_Obj) >> 8)  & 0xFF); 
    value |= ((SPI_ReadRegister(RegFeiLsb, Obj.SPI_Obj))       & 0xFF);
    float Hz;
    switch (sign){
        case 0:
            Hz = value * FEI_CONSTANT;
            break;
        case 1:
            Hz = (-1) * (value * FEI_CONSTANT);
            break;
    }
    UART_printf("The error is: %f\r\n", Hz); 

}   

__uint8_t SX1276_GetPayloadSize(SX1276_Object Obj){
    /**
     * Reads the Payload Size register
     */
    __uint8_t size = SPI_ReadRegister(RegPayloadLength, Obj.SPI_Obj);
    return size;
}

void SX1276_ResetImplicitMode(SX1276_Object Obj){
    /**
     * @brief Resets Implicit mode (Set explicit mode) for the comminication.
     * which means that header is sent.
     */
    __uint8_t readValue = SPI_ReadRegister(RegModemConfig1, Obj.SPI_Obj);
    readValue &= ~(RegModemConfig1_ImplicitHeaderModeOn);
    SPI_WriteRegister(RegModemConfig1, readValue, Obj.SPI_Obj);
}

void SX1276_SetImplicitMode(SX1276_Object Obj){
    /**
     * @brief Sets Implicit mode for the comminication, means that no header is sent.
     */
    __uint8_t readValue = SPI_ReadRegister(RegModemConfig1, Obj.SPI_Obj);
    readValue &= ~(RegModemConfig1_ImplicitHeaderModeOn);
    readValue |=  (RegModemConfig1_ImplicitHeaderModeOn);
    SPI_WriteRegister(RegModemConfig1, readValue, Obj.SPI_Obj);

}

void SX1276_SetMode(SX1276_Object Obj, enum LoraModes Mode){
    /**
     * @brief Switch the SX1276 to Stdby mode.
     */

    __uint8_t readValue = SPI_ReadRegister(RegOpMode, Obj.SPI_Obj);
    readValue &= ~(RegOpMode_Mode_Msk);
    readValue |=  (Mode << RegOpMode_Mode_Pos);
    SPI_WriteRegister(RegOpMode, readValue, Obj.SPI_Obj);


}   

void SX1276_Init(SX1276_Object Obj){
    /** 
     * @brief Initializes the registers of the SX1276 IC,  
     */
     // Init All the GPIOS, DIOx, TCXOx and reset and RF switches
    GPIO_Init(Obj.TCXO);
    GPIO_Set(Obj.TCXO);
    SPI_SetRegisters(Obj.SPI_Obj);
    SPI_WriteRegister(RegOpMode, RegOpMode_Mode_Sleep, Obj.SPI_Obj);
    SPI_WriteRegister(RegOpMode, RegOpMode_LongRangeMode, Obj.SPI_Obj);

}

void SX1276_SetPrambleLength(SX1276_Object Obj, __uint16_t preambleValue){
    /**
     * @brief Sets the preamble size of each packet, from 8-65535
     */
    __uint8_t preambleLsb =  preambleValue & 0xFF;
    __uint8_t preambleMsb = (preambleValue >> 8) & 0xFF;
    SPI_WriteRegister(RegPreambleLsb, preambleLsb, Obj.SPI_Obj);
    SPI_WriteRegister(RegPreambleMsb, preambleMsb, Obj.SPI_Obj);
}

void SX1276_SetFreq(SX1276_Object Obj, float freq){
    /**
     * @brief Sets the RF Carrier Frequncey of the LORA Transceiver
     * @param freq -> sets the RF carrier frequency of the LO of the SX1276
     */
    // Switch to stdby mode
    SX1276_SetMode(Obj, Mode_Stdby);

    // Calculate the freq and divide it to lsb, msb, mid 
    int frf = (freq / FSTEP);
    UART_printf("Freq is: %d\r\n", frf); // To Delete this line 
    UART_printf("Step is: %f\r\n", FSTEP); // To Delete this line
    __uint8_t msb = 0xFF & (frf >> 16);
    __uint8_t mid = 0xFF & (frf >> 8);
    __uint8_t lsb = 0xFF & frf;
    // Write the new frequnecy to the registers 
    SPI_WriteRegister(RegFrfMsb, msb, Obj.SPI_Obj);
    SPI_WriteRegister(RegFrfMid, mid, Obj.SPI_Obj);
    SPI_WriteRegister(RegFrfLsb, lsb, Obj.SPI_Obj);

}

void SX1276_SetSF(SX1276_Object Obj, enum LoraSpreadingFactor SF){
    /**
     * Set Spreading Factor for tranmission, Check enum in SX1276.h for options.
     */
    
    SX1276_SetMode(Obj, Mode_Stdby);
    __uint8_t readValue = SPI_ReadRegister(RegModemConfig2, Obj.SPI_Obj);
    readValue &= ~(RegModemConfig2_SF_Msk); 
    readValue |=  (SF << RegModemConfig2_SF_Pos); 
    SPI_WriteRegister(RegModemConfig2, readValue, Obj.SPI_Obj);

}

void SX1276_SetPayloadSize(SX1276_Object Obj, __uint8_t payloadsize){
    /**
     * Sets the payload size of the lora communication packet
     * Number from 1-255
     */
    SPI_WriteRegister(RegPayloadLength, payloadsize, Obj.SPI_Obj);

}

void SX1276_SetBW(SX1276_Object Obj, enum LoraBw BW){
    /**
     * Sets Bandwidth of LoRa Communication, Check enum in SX1276.h for options.
     */

    SX1276_SetMode(Obj, Mode_Stdby);
    __uint8_t readValue = SPI_ReadRegister(RegModemConfig1, Obj.SPI_Obj);
    readValue &= ~(RegModemConfig1_Bw_Msk);
    readValue |=  (BW << RegModemConfig1_Bw_Pos);
    SPI_WriteRegister(RegModemConfig1, readValue, Obj.SPI_Obj);
}

void SX1276_Tx(SX1276_Object Obj, __uint8_t* transmit_word, __uint8_t size){
    /**
     *  Sends a byte to the internal FIFO of the sx1276 to be transmitted. 
     * */    

    // Sets the payload size before (maybe)
    
    SX1276_FifoPtrTx = SX1276_FifoTxBaseAddr;
    // Change payload size
    SX1276_SetPayloadSize(Obj, size);
    // Switch to stdby mode and Check if it switched to stdby
    SX1276_SetMode(Obj, Mode_Stdby);
    __uint8_t index = 0;
    while(index < size){
    // Write to the pointer register, and then to the fifo.
        SPI_WriteRegister(RegFifoAddrPtr, SX1276_FifoPtrTx, Obj.SPI_Obj);
        SPI_WriteRegister(RegFifo, transmit_word[index], Obj.SPI_Obj);
        SX1276_FifoPtrTx++;
        index++;
    }
    SX1276_FifoPtrTx = SX1276_FifoTxBaseAddr;
    SPI_WriteRegister(RegFifoAddrPtr, SX1276_FifoPtrTx, Obj.SPI_Obj);
    __uint8_t TxDone = 0;
    SX1276_SetMode(Obj, Mode_Tx);
    while(!TxDone){
        TxDone = SPI_ReadRegister(RegIrqFlags, Obj.SPI_Obj);
        TxDone = (TxDone >> 3) & 0x1; 
    }
    SX1276_ClearIrq(Obj, RegIrqFlags_TxDone_Pos);


}

void SX1276_RxCon(SX1276_Object Obj){
    /**
     * This function switch the SX1276 to Receiver mode, it Waits for a signal to be received and return
     * the array of bytes that sent.
     */
    
    // In this mode, the device is in continues reception mode, when the device caught a packet,
    // it Sets up the RxDone interupt, the Register RegRxNbBytes says the packet size. 

    // Switch to RX mode
    static char message[PayloadMaxSize];
    memset(message, 0, PayloadMaxSize);

    SX1276_SetMode(Obj, Mode_RXContinues);
    // Check for the reception to finished which means all the interupts 
    __uint8_t flag_finished = 0;
    while(!flag_finished){
        flag_finished = SPI_ReadRegister(RegIrqFlags, Obj.SPI_Obj);
        flag_finished = (flag_finished >> RegIrqFlags_RxDone_Pos) & 0x1;
    }    
    flag_finished = 0;
    SX1276_ClearIrq(Obj, RegIrqFlags_RxDone_Pos);
    __uint8_t start_reg   = SPI_ReadRegister(RegFifoRxCurrentAddr, Obj.SPI_Obj);
    __uint8_t end_reg     = SPI_ReadRegister(RegFifoRxByteAddr, Obj.SPI_Obj);
    __uint8_t payloadSize = SPI_ReadRegister(RegRxNbBytes, Obj.SPI_Obj);
    // Another Crc check at the payload could be done, the bit CrcOnPayload should be turned on, but it
    // and it checked if the header has it (only on explicit header mode), If The transmitter has implicit header
    // The PayloadCrcError bit is not helpful and couldnt be set. 
    
    __uint8_t index = 0;
    while(!flag_finished){
        SPI_WriteRegister(RegFifoAddrPtr, start_reg, Obj.SPI_Obj);
        message[index] = SPI_ReadRegister(RegFifo, Obj.SPI_Obj);
        index++;
        start_reg++;
        if((start_reg - end_reg) == 0){
            flag_finished = 1;
        }
    } 
    __uint8_t crcError = SPI_ReadRegister(RegIrqFlags, Obj.SPI_Obj);
    crcError = (crcError >> RegIrqFlags_PayloadCrcError_Pos) & 0x1;

    SX1276_SetMode(Obj, Mode_Stdby);
    UART_printf("%s\r\n", message);
    // return message;

}