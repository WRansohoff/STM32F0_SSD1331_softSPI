#include "main.h"

// Simple delay method, with instructions not to optimize.
// It doesn't accurately delay a precise # of cycles,
// it's just a rough scale.
void __attribute__((optimize("O0"))) delay_cycles(uint32_t cyc) {
  uint32_t d_i;
  for (d_i = 0; d_i < cyc; ++d_i) {
    asm("NOP");
  }
}

/**
 * Main program.
 */
int main(void) {
  // Enable the GPIOB peripheral in 'RCC_AHBENR'.
  RCC->AHBENR   |= RCC_AHBENR_GPIOBEN;
  /* Initialize the GPIOB pins.
   * For software sspi with no MISO, use push-pull
   * outputs at high speed for each pin. */
  // Mode: Output
  GPIOB->MODER   &= ~((0x3 << (PB_MOSI * 2)) |
                      (0x3 << (PB_SCK  * 2)) |
                      (0x3 << (PB_DC   * 2)) |
                      (0x3 << (PB_CS   * 2)) |
                      (0x3 << (PB_RST  * 2)));
  GPIOB->MODER   |=  ((0x1 << (PB_MOSI * 2)) |
                      (0x1 << (PB_SCK  * 2)) |
                      (0x1 << (PB_DC   * 2)) |
                      (0x1 << (PB_CS   * 2)) |
                      (0x1 << (PB_RST  * 2)));
  // Output type: Push-pull
  GPIOB->OTYPER  &= ~((0x1 << PB_MOSI) |
                      (0x1 << PB_SCK)  |
                      (0x1 << PB_DC)   |
                      (0x1 << PB_CS)   |
                      (0x1 << PB_RST));
  // No pull-up or pull-down resistors.
  GPIOB->PUPDR   &= ~((0x3 << (PB_MOSI * 2)) |
                      (0x3 << (PB_SCK  * 2)) |
                      (0x3 << (PB_DC   * 2)) |
                      (0x3 << (PB_CS   * 2)) |
                      (0x3 << (PB_RST  * 2)));
  // High-speed - 50MHz maximum
  // (Setting all '1's, so no need to clear bits first.)
  // (The 'reset' pin doesn't need to be high-speed.)
  GPIOB->OSPEEDR |=  ((0x3 << (PB_MOSI * 2)) |
                      (0x3 << (PB_SCK  * 2)) |
                      (0x3 << (PB_DC   * 2)) |
                      (0x3 << (PB_CS   * 2)));

  // Perform SSD1331 panel initialization.
  // Set initial pin values.
  //   (The 'Chip Select' pin tells the display if it
  //    should be listening. '0' means 'hey, listen!', and
  //    '1' means 'ignore the SCK/MOSI/DC pins'.)
  GPIOB->ODR &= ~(1 << PB_CS);
  //   (See the 'sspi_cmd' method for 'DC' pin info.)
  GPIOB->ODR |=  (1 << PB_DC);
  // Reset the display by pulling the reset pin low,
  // delaying a bit, then pulling it high.
  GPIOB->ODR &= ~(1 << PB_RST);
  // Delay at least 100ms; meh, call it 10 million no-ops.
  delay_cycles(10000000);
  GPIOB->ODR |=  (1 << PB_RST);
  delay_cycles(10000000);

  // Send initialization commands for a 96x64 display.
  // TODO: Constants/macros for command values.
  // 'Display off'.
  sspi_cmd(0xAE);
  // 'Remap display settings'.
  sspi_cmd(0xA0);
  // We just want 65K colors format 1 (bits [7:6] = 01).
  sspi_cmd(0x70);
  // (Or, also set bit 2 to flip the display vertically.)
  //sspi_cmd(0x72);
  // 'Start line.'
  sspi_cmd(0xA1);
  sspi_cmd(0x00);
  // 'Display Offset'.
  sspi_cmd(0xA2);
  sspi_cmd(0x00);
  // 'Normal Display Mode.'
  sspi_cmd(0xA4);
  // 'Set Multiplexing'.
  // (I think 64 should be the reset default, but...)
  sspi_cmd(0xA8);
  sspi_cmd(0x3F);
  // 'Set Master Display Voltage Source'.
  // This is not a choice; the SSD1331 only accepts
  // external voltage. But I guess it should be set.
  sspi_cmd(0xAD);
  sspi_cmd(0x8E);
  // 'Set Power Save Mode'. 0x1A = On, 0x0B = Off.
  sspi_cmd(0xB0);
  sspi_cmd(0x0B);
  // 'Set Precharge Adjustment'.
  sspi_cmd(0xB1);
  sspi_cmd(0x31);
  // 'Set Clock Divider Frequency'.
  // [7:4] = osc frequency, [3:0]+1 = div ratio.
  sspi_cmd(0xF0);
  // 'Set Precharge A'.
  sspi_cmd(0x8A);
  sspi_cmd(0x64);
  // 'Set Precharge B'.
  sspi_cmd(0x8B);
  sspi_cmd(0x78);
  // 'Set Precharge C'.
  sspi_cmd(0x8C);
  sspi_cmd(0x64);
  // 'Set Precharge Level'.
  sspi_cmd(0xBB);
  sspi_cmd(0x3A);
  // 'Set COM Deselect Voltage Level'. (to 0.83 * VCC)
  sspi_cmd(0xBE);
  sspi_cmd(0x3E);
  // 'Set Master Current Division'. (~'Set Brightness')
  sspi_cmd(0x87);
  sspi_cmd(0x06);
  // 'Set Contrast A'.
  sspi_cmd(0x81);
  sspi_cmd(0x91);
  // 'Set Contrast B'.
  sspi_cmd(0x82);
  sspi_cmd(0x50);
  // 'Set Contrast C'.
  sspi_cmd(0x83);
  sspi_cmd(0x7D);
  // 'Display On'.
  sspi_cmd(0xAF);

  // (Main loop - draw to the screen.)
  // Uh...soon. TODO
  uint16_t px_i = 0;
  while (1) {
    px_i += 1;
  }
}
