#ifndef _VVC_UTIL_H
#define _VVC_UTIL_H

// Standard C includes.
#include <stdint.h>
// Vendor-provided STM32 includes.
#ifdef VVC_F0
  #include "stm32f031x6.h"
#elif  VVC_L0
  #include "stm32l031xx.h"
#endif

// Project includes.
#include "sspi.h"

/* Utility method declarations. */
void ssd1331_init(void);
void delay_cycles(uint32_t cyc);

#endif
