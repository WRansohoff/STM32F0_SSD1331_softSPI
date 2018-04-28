#ifndef _VVC_SSPI_H
#define _VVC_SSPI_H

// Standard C includes.
#include <stdint.h>
// Vendor-provided STM32 includes.
#ifdef VVC_F0
  #include "stm32f031x6.h"
#elif  VVC_L0
  #include "stm32l031xx.h"
#endif

// Define GPIOB pin mappings for software '4-wire' SPI.
#define PB_MOSI (6)
#define PB_SCK  (7)
#define PB_DC   (1)
#define PB_CS   (0)
#define PB_RST  (5)

// Write a byte of data using software SPI.
inline void sspi_w(uint8_t dat);
// Write a 'command' byte for 4-wire SPI interfaces.
inline void sspi_cmd(uint8_t cdat);

#endif
