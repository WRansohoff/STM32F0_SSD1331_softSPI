#include "main.h"

/**
 * Main program.
 */
int main(void) {
  // Enable the GPIOB peripheral.
  #ifdef VVC_F0
    RCC->AHBENR   |= RCC_AHBENR_GPIOBEN;
  #elif  VVC_L0
    RCC->IOPENR   |= RCC_IOPENR_IOPBEN;
  #endif
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
  // Delay at least 100ms; meh, call it 2 million no-ops.
  delay_cycles(2000000);
  GPIOB->ODR |=  (1 << PB_RST);
  delay_cycles(2000000);

  // Send initialization commands for a 96x64 display.
  ssd1331_init();

  // (Main loop - draw to the screen.)
  uint16_t px_i = 0;
  uint16_t px_col = 0;
  uint16_t px_val = 0;
  while (1) {
    // Draw the buffer.
    for (px_i = 0; px_i < OLED_BUF_SIZE; ++px_i) {
      px_col = oled_buffer[px_i] >> 4;
      if (px_col >= 0 && px_col <= 15) {
        px_val = oled_colors[px_col];
      }
      else {
        px_val = OLED_BLK;
      }
      sspi_w(px_val >> 8);
      sspi_w(px_val & 0x00FF);
      px_col = oled_buffer[px_i] & 0x0F;
      if (px_col >= 0 && px_col <= 15) {
        px_val = oled_colors[px_col];
      }
      else {
        px_val = OLED_BLK;
      }
      sspi_w(px_val >> 8);
      sspi_w(px_val & 0x00FF);
    }
  }
}
