# Overview

This project demonstrates example code for displaying an image on a 96x64-pixel SSD1331 16-bit color OLED display, using software SPI on an STM32F031K6 board.

On ST's "Nucleo" board for this chip, the pins used in this example are:

* PB0 / 'D3': "Chip Select" pin.

* PB1 / 'D6': "Data / Command" pin.

* PB5 / 'D11': "Reset" pin.

* PB6 / 'D5': "SPI Host Data Out" (MOSI) pin.

* PB7 / 'D4': "SPI Clock" (SCLK) pin.

# Uploading

You can build the code with `make`, and upload it with the 'st-flash' utility:

`st-flash write main.bin 0x08000000`
