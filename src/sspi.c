#include "sspi.h"

/*
 * Write a byte of data using software SPI. For each bit:
 * 1. Pull the clock pin low.
 * 2. Set the 'MOSI' data pin to the correct value.
 * 3. Pull the clock pin high.
 */
inline void sspi_w(uint8_t dat) {
  uint8_t sspi_i;
  // Send 8 bits, with the MSB first.
  for (sspi_i = 0x80; sspi_i != 0x00; sspi_i >>= 1) {
    GPIOB->ODR &= ~(1 << PB_SCK);
    if (dat & sspi_i) {
      GPIOB->ODR |=  (1 << PB_MOSI);
    }
    else {
      GPIOB->ODR &= ~(1 << PB_MOSI);
    }
    GPIOB->ODR |=  (1 << PB_SCK);
  }
}

/*
 * Write a 'Command byte' over software SPI.
 * "4-wire" SPI interfaces are common in devices like
 * screens. A 'Data/Command' pin determines whether
 * the SPI data is an instruction for the display,
 * or pixel data for the framebuffer RAM.
 * Here, '0' means 'Command' and '1' means 'Data'.
 */
inline void sspi_cmd(uint8_t cdat) {
  // Pull the 'D/C' pin low, write data, pull 'D/C' high.
  GPIOB->ODR &= ~(1 << PB_DC);
  sspi_w(cdat);
  GPIOB->ODR |=  (1 << PB_DC);
}
