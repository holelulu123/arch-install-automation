#ifndef __SX_1276_H
#define __SX_1276_H
#include <stdio.h>
#include "spi.h"
#include "gpio.h"


#define FEI_CONSTANT                   (65.536)
#define PayloadMaxSize                 ((__uint8_t)0xFF)
#define ZeroByte                       ((__uint8_t)0x00)
#define FXOSC                          32000000
#define CONST_2_19                     524288
#define FSTEP                          61.03515625
#define SX1276_FifoTxBaseAddr          ((__uint8_t)0x80)
#define SX1276_FifoTxMaxAddr           ((__uint8_t)0xFF)

#define SX1276_FifoRxBaseAddr          ((__uint8_t)0x00)
#define SX1276_FifoRxMaxAddr           ((__uint8_t)0x80)

// Registers Map
#define RegFifo                        ((__uint8_t)0x00) 
#define RegOpMode                      ((__uint8_t)0x01)
#define RegFrfMsb                      ((__uint8_t)0x06)
#define RegFrfMid                      ((__uint8_t)0x07)
#define RegFrfLsb                      ((__uint8_t)0x08)
#define RegPaConfig                    ((__uint8_t)0x09)
#define RegPaRamp                      ((__uint8_t)0x0A)
#define RegOcp                         ((__uint8_t)0x0B)
#define RegLna                         ((__uint8_t)0x0C)
#define RegFifoAddrPtr                 ((__uint8_t)0x0D)
#define RegFifoTxBaseAddr              ((__uint8_t)0x0E)
#define RegFifoRxBaseAddr              ((__uint8_t)0x0F)
#define RegFifoRxCurrentAddr           ((__uint8_t)0x10)
#define RegIrqFlagsMask                ((__uint8_t)0x11)
#define RegIrqFlags                    ((__uint8_t)0x12)
#define RegRxNbBytes                   ((__uint8_t)0x13)
#define RegRxHeaderCntValueMsb         ((__uint8_t)0x14)
#define RegRxHeaderCntValueLsb         ((__uint8_t)0x15)
#define RegRxPacketCntValueMsb         ((__uint8_t)0x16)
#define RegRxPacketCntValueLsb         ((__uint8_t)0x17)
#define RegModemStat                   ((__uint8_t)0x18)
#define RegPktSnrValue                 ((__uint8_t)0x19)
#define RegPktRssiValue                ((__uint8_t)0x1A)
#define RegRssiValue                   ((__uint8_t)0x1B)
#define RegHopChannel                  ((__uint8_t)0x1C)
#define RegModemConfig1                ((__uint8_t)0x1D)
#define RegModemConfig2                ((__uint8_t)0x1E)
#define RegSymbTimeoutLsb              ((__uint8_t)0x1F)
#define RegPreambleMsb                 ((__uint8_t)0x20)
#define RegPreambleLsb                 ((__uint8_t)0x21)
#define RegPayloadLength               ((__uint8_t)0x22)
#define RegMaxPayloadLength            ((__uint8_t)0x23)
#define RegHopPeriod                   ((__uint8_t)0x24)
#define RegFifoRxByteAddr              ((__uint8_t)0x25)
#define RegModemConfig3                ((__uint8_t)0x26)
#define RegFeiMsb                      ((__uint8_t)0x28)
#define RegFeiMid                      ((__uint8_t)0x29)
#define RegFeiLsb                      ((__uint8_t)0x2A)
#define RegRssiWideband                ((__uint8_t)0x2C)
#define RegIfFreq1                     ((__uint8_t)0x2F)
#define RegIfFreq2                     ((__uint8_t)0x30)
#define RegDetectOptimize              ((__uint8_t)0x31)
#define RegInvertIQ                    ((__uint8_t)0x33)
#define RegHighBwOptimize1             ((__uint8_t)0x36)
#define RegDetectionThreshold          ((__uint8_t)0x37)
#define RegSyncWord                    ((__uint8_t)0x39)
#define RegHighBwOptimize2             ((__uint8_t)0x3A)
#define RegInvertIQ2                   ((__uint8_t)0x3B)
#define RegDioMapping1                 ((__uint8_t)0x40)
#define RegDioMapping2                 ((__uint8_t)0x41)
#define RegVersion                     ((__uint8_t)0x42)
#define RegTcxo                        ((__uint8_t)0x4B)
#define RegPaDac                       ((__uint8_t)0x4D)
#define RegFormerTemp                  ((__uint8_t)0x5B)
#define RegAgcRef                      ((__uint8_t)0x61)
#define RegAgcThresh1                  ((__uint8_t)0x62)
#define RegAgcThresh2                  ((__uint8_t)0x63)
#define RegAgcThresh3                  ((__uint8_t)0x64)
#define RegPll                         ((__uint8_t)0x70)

/**
 * Common Settings for the SX1276
 */

#define RegOpMode_LongRangeMode        ((__uint8_t)0x1 << 7)
#define RegOpMode_AccessSharedReg      ((__uint8_t)0x1 << 6)
#define RegOpMode_LowFrequnecyModeOn   ((__uint8_t)0x1 << 3)
#define RegOpMode_Mode_Sleep           ((__uint8_t)0x0 << 0)     
#define RegOpMode_Mode_Stdby           ((__uint8_t)0x1 << 0)
#define RegOpMode_Mode_FSTX            ((__uint8_t)0x2 << 0)
#define RegOpMode_Mode_TX              ((__uint8_t)0x3 << 0)
#define RegOpMode_Mode_FSRX            ((__uint8_t)0x4 << 0)
#define RegOpMode_Mode_RXContinoues    ((__uint8_t)0x5 << 0)
#define RegOpMode_Mode_RXSingle        ((__uint8_t)0x6 << 0)
#define RegOpMode_Mode_CAD             ((__uint8_t)0x7 << 0)
#define RegOpMode_Mode_Msk             ((__uint8_t)0x7 << 0)
#define RegOpMode_Mode_Pos             ((__uint8_t)0)

/**
 * Registers for RF Blocks
 */

#define RegPaConfig_PaSelect           ((__uint8_t)0x1 << 7)
#define RegPaConfig_MaxPower_Pos       ((__uint8_t)4)
#define RegPaConfig_OutputPower_Pos    ((__uint8_t)0)
#define RegPaRamp_OutputPower_Pos      ((__uint8_t)0) // TODO: Check this register
#define RegOcp_OcpOn                   ((__uint8_t)0x1 << 5) 
#define RegOcp_OcpTrim_Pos             ((__uint8_t)5) 
#define RegLna_LnaGain_G1              ((__uint8_t)0x1 << 5) 
#define RegLna_LnaGain_G2              ((__uint8_t)0x2 << 5) 
#define RegLna_LnaGain_G3              ((__uint8_t)0x3 << 5) 
#define RegLna_LnaGain_G4              ((__uint8_t)0x4 << 5) 
#define RegLna_LnaGain_G5              ((__uint8_t)0x5 << 5) 
#define RegLna_LnaGain_G6              ((__uint8_t)0x6 << 5) 
#define RegLna_LnaBoostHf              ((__uint8_t)0x3 << 0)

/**
 * Other Registers for LoRa Mode 
 */
#define RegHopChannel_PllTimeout             ((__uint8_t)0x1 << 7)
#define RegHopChannel_CrcOnPayload           ((__uint8_t)0x1 << 6)
#define RegHopChannel_FhssPresentChannel_Pos ((__uint8_t)0)

#define RegModemConfig1_Bw_Pos               ((__uint8_t)4)
#define RegModemConfig1_Bw_7_8               ((__uint8_t)0x0 << 4)
#define RegModemConfig1_Bw_10_4              ((__uint8_t)0x1 << 4)
#define RegModemConfig1_Bw_15_6              ((__uint8_t)0x2 << 4)
#define RegModemConfig1_Bw_20_8              ((__uint8_t)0x3 << 4)
#define RegModemConfig1_Bw_31_25             ((__uint8_t)0x4 << 4)    
#define RegModemConfig1_Bw_41_7              ((__uint8_t)0x5 << 4)
#define RegModemConfig1_Bw_62_5              ((__uint8_t)0x6 << 4)
#define RegModemConfig1_Bw_125               ((__uint8_t)0x7 << 4)
#define RegModemConfig1_Bw_250               ((__uint8_t)0x8 << 4)
#define RegModemConfig1_Bw_500               ((__uint8_t)0x9 << 4)
#define RegModemConfig1_Bw_Msk               ((__uint8_t)0xF << 4)
#define RegModemConfig1_CR_4_5               ((__uint8_t)0x1 << 1)
#define RegModemConfig1_CR_4_6               ((__uint8_t)0x2 << 1)
#define RegModemConfig1_CR_4_7               ((__uint8_t)0x3 << 1)
#define RegModemConfig1_CR_4_8               ((__uint8_t)0x4 << 1)
#define RegModemConfig1_ImplicitHeaderModeOn ((__uint8_t)0x1 << 0)
#define RegModemConfig2_SF_Pos               ((__uint8_t)4)
#define RegModemConfig2_SF_6                 ((__uint8_t)0x6 << 4)
#define RegModemConfig2_SF_7                 ((__uint8_t)0x7 << 4)
#define RegModemConfig2_SF_8                 ((__uint8_t)0x8 << 4)
#define RegModemConfig2_SF_9                 ((__uint8_t)0x9 << 4)
#define RegModemConfig2_SF_10                ((__uint8_t)0xA << 4)
#define RegModemConfig2_SF_11                ((__uint8_t)0xB << 4)
#define RegModemConfig2_SF_12                ((__uint8_t)0xC << 4)
#define RegModemConfig2_SF_Msk               ((__uint8_t)0xF << 4)
#define RegModemConfig2_TxContinousMode      ((__uint8_t)0x1 << 3)
#define RegModemConfig2_RxPayloadCrcOn       ((__uint8_t)0x1 << 2)

#define RegIrqFlags_CadDetected_Pos          ((__uint8_t)0)
#define RegIrqFlags_FhssChangeChannel_Pos    ((__uint8_t)1)
#define RegIrqFlags_CadDone_Pos              ((__uint8_t)2)
#define RegIrqFlags_TxDone_Pos               ((__uint8_t)3)
#define RegIrqFlags_ValidHeader_Pos          ((__uint8_t)4)
#define RegIrqFlags_PayloadCrcError_Pos      ((__uint8_t)5)
#define RegIrqFlags_RxDone_Pos               ((__uint8_t)6)
#define RegIrqFlags_RxTimeout_Pos            ((__uint8_t)7)



typedef struct SX1276_Object {
    SPI_Object      SPI_Obj;
    GPIO_Object     DIO0;     
    GPIO_Object     DIO1;     
    GPIO_Object     DIO2;     
    GPIO_Object     DIO3;     
    GPIO_Object     DIO4;     
    GPIO_Object     DIO5;     
    GPIO_Object     TCXO;


}SX1276_Object;

static const SX1276_Object SX1276_Obj = {
    .SPI_Obj        = SPI_SX1276,
    .DIO0           = DIO0_SX1276,
    .DIO1           = DIO1_SX1276,
    .DIO2           = DIO2_SX1276,
    .DIO3           = DIO3_SX1276,
    .DIO4           = DIO4_SX1276,
    .DIO5           = DIO5_SX1276,
    .TCXO           = TCXO_SX1276

};

typedef enum LoraBw : __uint8_t {
    LoRa_Bw_7_8     = 0x0,
    LoRa_Bw_10_4    = 0x1,
    LoRa_Bw_15_6    = 0x2,
    LoRa_Bw_20_8    = 0x3,
    LoRa_Bw_31_25   = 0x4,
    LoRa_Bw_41_7    = 0x5,
    LoRa_Bw_62_5    = 0x6,
    LoRa_Bw_125     = 0x7,
    LoRa_Bw_250     = 0x8,
    LoRa_Bw_500     = 0x9
    
}LoraBw;

typedef enum LoraSpreadingFactor : __uint8_t {
    LoRa_SF_6    = 0x6,
    LoRa_SF_7    = 0x7,
    LoRa_SF_8    = 0x8,
    LoRa_SF_9    = 0x9,
    LoRa_SF_10   = 0xA,
    LoRa_SF_11   = 0xB,
    LoRa_SF_12   = 0xC
    
}LoraSpreadingFactor;

typedef enum LoraModes : __uint8_t {
    Mode_Sleep          = 0x0,
    Mode_Stdby          = 0x1,
    Mode_FSTX           = 0x2,
    Mode_Tx             = 0x3,
    Mode_FSRX           = 0x4,
    Mode_RXContinues    = 0x5,
    Mode_RxSingle       = 0x6,
    Mode_CAD            = 0x7
    
}LoraModes;

void SX1276_Init(SX1276_Object Obj);
void SX1276_SetMode(SX1276_Object Obj, enum LoraModes Mode);

// PHY level configuration
void SX1276_SetBW(SX1276_Object Obj, enum LoraBw BW);
void SX1276_SetSF(SX1276_Object Obj, enum LoraSpreadingFactor SF);
void SX1276_SetFreq(SX1276_Object Obj, float freq);
void SX1276_GetFeiValue(SX1276_Object Obj);

// Commuication Data Stracture
void SX1276_SetSyncWord(SX1276_Object Obj, __uint8_t SyncWord);
__uint8_t SX1276_GetSyncWord(SX1276_Object Obj);
void SX1276_SetImplicitMode(SX1276_Object Obj);
void SX1276_ResetImplicitMode(SX1276_Object Obj);
void SX1276_SetPrambleLength(SX1276_Object Obj, __uint16_t preambleValue);
void SX1276_SetPayloadSize(SX1276_Object Obj, __uint8_t payloadsize);
__uint8_t SX1276_GetPayloadSize(SX1276_Object Obj);
void SX1276_RxPayloadCrc(SX1276_Object Obj, __uint8_t setReset);
int SX1276_GetRssiValue(SX1276_Object Obj);

// SX1276 Helpers
void SX1276_ClearIrq(SX1276_Object Obj, __uint8_t bitToClear);

// SX1276 Main Purpose tasks
void       SX1276_Tx   (SX1276_Object Obj, __uint8_t *transmit_word, __uint8_t size);
void       SX1276_RxCon(SX1276_Object Obj);

#endif