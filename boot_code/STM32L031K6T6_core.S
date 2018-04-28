/*
 * Test program to boot an STM32 chip with the absolute
 * minimum required code for teaching about the chips.
 *
 * Copyright William Ransohoff, Vivonomicon, LLC, 2017
 *
 * Open source under the MIT License
 */

.syntax unified
.cpu cortex-m0plus
.fpu softvfp
.thumb

// Global values.
.global reset_handler

/*
 * The Reset handler. Called on reset.
 */
.type reset_handler, %function
reset_handler:
  // Set the stack pointer to the end of the stack.
  // The '_estack' value is defined in our linker script.
  LDR  r0, =_estack
  MOV  sp, r0

  // Copy data from flash to RAM data init section.
  // R2 will store our progress along the sidata section.
  MOVS r0, #0
  // Load the start/end addresses of the data section,
  // and the start of the data init section.
  LDR  r1, =_sdata
  LDR  r2, =_edata
  LDR  r3, =_sidata
  B    copy_sidata_loop

  copy_sidata:
    // Offset the data init section by our copy progress.
    LDR  r4, [r3, r0]
    // Copy the current word into data, and increment.
    STR  r4, [r1, r0]
    ADDS r0, r0, #4

  copy_sidata_loop:
    // Unless we've copied the whole data section, copy the
    // next word from sidata->data.
    ADDS r4, r0, r1
    CMP  r4, r2
    BCC  copy_sidata

  // Once we are done copying the data section into RAM,
  // move on to filling the BSS section with 0s.
  MOVS r0, #0
  LDR  r1, =_sbss
  LDR  r2, =_ebss
  B    reset_bss_loop

  // Zero out the BSS segment.
  reset_bss:
    // Store a 0 and increment by a word.
    STR  r0, [r1]
    ADDS r1, r1, #4

  reset_bss_loop:
    // We'll use R1 to count progress here; if we aren't
    // done, reset the next word and increment.
    CMP  r1, r2
    BCC  reset_bss

  reset_chip_state:
    // Clock registers:
    // rcc_cr_base:           0x40021000
    // rcc_cfgr_base:         0x4002100C
    // rcc_cier_base:         0x40021010
    // Disable RCC interrupts.
    LDR  r0, =0x40021010
    LDR  r2, =0x00000000
    STR  r2, [r0]
    // Reset/set the Flash ACR to enable prefetching and
    // use a 1-wait state.
    LDR  r0, =0x40022000
    LDR  r2, =0xFFFFFFFC
    LDR  r1, [r0]
    ANDS r1, r1, r2
    LDR  r2, =0x00000003
    ORRS r1, r1, r2
    STR  r1, [r0]
    // The L0 line has a 16MHz HSI, not 8MHz.
    // But it boots to a 2MHz 'MSI' oscillator.
    // So, enable the HSI16 oscillator.
    LDR  r0, =0x40021000
    LDR  r2, =0x00000001
    LDR  r1, [r0]
    ORRS r1, r1, r2
    STR  r1, [r0]
    // And wait for the HSI oscillator to stabilize.
    LDR  r2, =0x00000004
    hsi16_ready_loop:
      LDR  r1, [r0]
      ANDS r1, r1, r2
      // TODO: If ANDS sets status bits, is CMP needed?
      CMP  r1, r2
      BEQ  hsi16_ready_done
      B    hsi16_ready_loop
    hsi16_ready_done:
    // Configure the PLL to (HSI / 2) * 4.
    // So, PLLDIV = 0b01 and PLLMUL = 0b0001
    // (Also reset PLLSRC, why not)
    LDR  r0, =0x4002100C
    LDR  r2, =0xFF02FFFF
    LDR  r1, [r0]
    ANDS r1, r1, r2
    LDR  r2, =0x00440000
    ORRS r1, r1, r2
    STR  r1, [r0]
    // Enable the PLL.
    LDR  r0, =0x40021000
    LDR  r2, =0x01000000
    LDR  r1, [r0]
    ORRS r1, r1, r2
    STR  r1, [r0]
    // Wait for the PLL to be ready.
    LDR  r2, =0x02000000
    pll_ready_loop:
      LDR  r1, [r0]
      ANDS r1, r1, r2
      CMP  r1, r2
      BEQ  pll_ready_done
      B    pll_ready_loop
    pll_ready_done:

    // Select the PLL as the system clock source.
    LDR  r0, =0x4002100C
    LDR  r2, =0xFFFFFFFC
    LDR  r1, [r0]
    ANDS r1, r1, r2
    STR  r1, [r0]
    LDR  r2, =0x00000003
    LDR  r1, [r0]
    ORRS r1, r1, r2
    STR  r1, [r0]
    // Wait for PLL to be the system clock source.
    LDR  r2, =0x0000000C
    pll_is_sys_clock_loop:
      LDR  r1, [r0]
      ANDS r1, r1, r2
      CMP  r1, r2
      BEQ  pll_is_sys_clock_done
      B    pll_is_sys_clock_loop
    pll_is_sys_clock_done:
    // Done! The system clock is now a 48MHz PLL.

  // Branch to the 'main' method.
  B    main
.size reset_handler, .-reset_handler
