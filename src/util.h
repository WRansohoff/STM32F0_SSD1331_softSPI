#ifndef _VVC_UTIL_H
#define _VVC_UTIL_H

// Standard C includes.
#include <stdint.h>
// Vendor-provided STM32 includes.
#include "stm32f031x6.h"

// Project includes.
#include "sspi.h"

/* Utility method declarations. */
void ssd1331_init(void);
void delay_cycles(uint32_t cyc);

#endif
