#ifndef __SPI_H
#define __SPI_H
#include <stdio.h>

// SPI GPIO configuration
#define GPIOA_MOSI_PIN 4
#define GPIOA_MISO_PIN 5 // Used only in full-duplex mode
#define GPIOA_NSS_PIN 6
#define GPIOA_SCLK_PIN 7
#define SPI1_SX1276_AF 0

void SetRegisters_SPI1();
void SPI_Write(__uint8_t *data);
void SPI_Read();

#endif