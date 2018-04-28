#include "util.h"

// Initialize a 96x64-px SSD1331 display.
// TODO: Constants/macros for command values.
void ssd1331_init(void) {
  // Instead of taking an existing library's word for
  // these, use a command sequence listed in the
  // SSD1331 datasheet's "Command Table", Section 8.
  // (But check out Adafruit for boards and a library!)
  // That table may be sorted by hex value rather than
  // ideal command ordering, but let's see what happens.
  // 'Unlock Display.' - 0xFD/0x16 can 'Lock' it,
  // in which case all other commands are ignored.
  sspi_cmd(0xFD);
  sspi_cmd(0x12);
  // (Turn the display off.)
  sspi_cmd(0xAE);

  // 'Set Column Address' - default is 0-95, which is
  // also what we want.
  sspi_cmd(0x15);
  sspi_cmd(0x00);
  sspi_cmd(0x5F);
  // 'Set Row Address' - default is 0-63, which is good.
  sspi_cmd(0x75);
  sspi_cmd(0x00);
  sspi_cmd(0x3F);

  // 'Set Color A Contrast' - default is 128.
  sspi_cmd(0x81);
  sspi_cmd(0x80);
  // 'Set Color B Contrast' - default is 128, use 96.
  sspi_cmd(0x82);
  sspi_cmd(0x60);
  // 'Set Color C Contrast' - default is 128.
  sspi_cmd(0x83);
  sspi_cmd(0x80);
  // 'Set Master Current Control' - default is 15, but
  // use 8 for ~half. (~= 'Set Brightness')
  sspi_cmd(0x87);
  sspi_cmd(0x08);
  // 'Set Precharge A' - default is 'Color A Contrast'.
  sspi_cmd(0x8A);
  sspi_cmd(0x80);
  // 'Set Precharge B' - default is 'Color B Contrast'.
  sspi_cmd(0x8B);
  sspi_cmd(0x60);
  // 'Set Precharge C' - default is 'Color C Contrast'.
  sspi_cmd(0x8C);
  sspi_cmd(0x80);
  // 'Remap Display Settings' - default is 0x40.
  // Use 0x60 to avoid drawing lines in odd-even order.
  sspi_cmd(0xA0);
  sspi_cmd(0x60);
  // 'Set Display Start Row' - default is 0.
  sspi_cmd(0xA1);
  sspi_cmd(0x00);
  // 'Set Vertical Offset' - default is 0.
  sspi_cmd(0xA2);
  sspi_cmd(0x00);
  // 'Set Display Mode' - default is 'A4'. 'A7' = invert.
  // (The actual command byte sets the mode; no 'arg')
  sspi_cmd(0xA4);
  // 'Set Multiplex Ratio.' I think this is how many
  // rows of pixels are actually enabled; default is 63.
  sspi_cmd(0xA8);
  sspi_cmd(0x3F);
  // (I am going to ignore the 0xAB 'Dim Mode Settings'
  // command - it looks like it only matters if we use
  // the 0xAC 'Dim Display' command; we will use 0xAF.)
  // 'Set Voltage Supply Configuration'. The SSD1331 has
  // no onboard charge pump, so we must use external
  // voltage. (0x8E)
  sspi_cmd(0xAB);
  sspi_cmd(0x8E);
  // 'Set Power Save Mode'. Default enabled; disable it.
  // ('on' is 0x1A, 'off' is 0x0B)
  sspi_cmd(0xB0);
  sspi_cmd(0x0B);
  // 'Adjust Precharge Phases.' Bits [7:4] set the
  // precharge stage 2 period, bits [3:0] set phase 1.
  // Default is 0x74.
  sspi_cmd(0xB1);
  sspi_cmd(0x74);
  // 'Set Clock Divider Frequency'. Bits [7:4] set the
  // oscillator frequency, bits [3:0]+1 set the
  // clock division ratio. Default is 0xD0.
  sspi_cmd(0xB3);
  sspi_cmd(0xD0);
  // (I am going to ignore the 'Set Grayscale Table'
  // command - it has a bunch of gamma curve settings.)
  // So, the 'Reset to Default Grayscale Table'
  // command does make sense to call.
  sspi_cmd(0xB9);
  // 'Set Precharge Level'. Default is 0x3E.
  sspi_cmd(0xBB);
  sspi_cmd(0x3E);
  // 'Set Logic 0 Threshold'. Default is 0x3E = 0.83*VCC.
  sspi_cmd(0xBE);
  sspi_cmd(0x3E);
  // 'Display On'.
  sspi_cmd(0xAF);
}

// Simple delay method, with instructions not to optimize.
// It doesn't accurately delay a precise # of cycles,
// it's just a rough scale.
void __attribute__((optimize("O0"))) delay_cycles(uint32_t cyc) {
  uint32_t d_i;
  for (d_i = 0; d_i < cyc; ++d_i) {
    asm("NOP");
  }
}
