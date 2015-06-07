
/*  This file is part of the HWA project.
 *  Copyright (c) 2012,2015 Christophe Duparquet.
 *  All rights reserved. Read LICENSE.TXT for details.
 */

/**
 * @page boards
 *
 * @section boards_nanodccduino NANO DCCduino.h
 *
 * This is an Arduino clone with a MEGA328P-AU microcontroller, a 16 MHz crystal
 * and a CH340 USB/Serial adapter.
 *
 * The CH340 does not work on my board. Then I use an external USB/Serial
 * adapter:
 *
 *     BOARD -> TX1 : RXD <- Serial Adapter
 *              RX0 : TXD
 *               5V : DTR
 *              GND : GND
 *
 * Wiring for USBASP:
 *
 *      USBASP -> 1  MOSI  D11 <- BOARD
 *                2  VCC   5V
 *                4  GND   GND
 *                5  RST   RST
 *                7  SCK   D13
 *                9  MISO  D12
 *
 * @par nanodccduino.h
 * @include nanodccduino.h
 * <br>
 */

/*  These are mostly the values as the board is shipped, except the bootsection
 *  size which is 1024 with Diabolo instead of 2048 with the Arduino bootloader.
 */
#define HW_DEVICE_BOOT                  bootloader
#define HW_DEVICE_BOOTSECTION_SIZE      1024

#define HW_DEVICE_CLK_SRC               low_power_xosc
#define HW_DEVICE_CLK_SRC_HZ            16000000
#define HW_DEVICE_CLK_PSC               1
#define HW_DEVICE_STARTUP_DELAYS        16KCK_14CK_64ms
#define HW_DEVICE_BROWNOUT_DETECTION    4100_4500mV
#define HW_DEVICE_SELF_PROGRAMMING      enabled

#define HW_DEVICE_EXTERNAL_RESET        enabled
#define HW_DEVICE_DEBUG_WIRE            disabled
#define HW_DEVICE_WATCHDOG_ALWAYS_ON    no
#define HW_DEVICE_CLOCK_OUTPUT          disabled

/*  Settings for Diabolo
 */
#define DIABOLO_PIN_RX                  hw_pin_rxd
#define DIABOLO_PIN_TX                  hw_pin_txd
#define DIABOLO_BPS                     230400          // Default transfer rate
#define DIABOLO_RESET_SIGNAL            DTR

#define ARDUINO

#define PIN_D3                          hw_pin_pd3      // OC2B INT1
#define PIN_D5                          hw_pin_pd5      // OC0B T1
#define PIN_D6                          hw_pin_pd6      // OC0A AIN0
#define PIN_D9                          hw_pin_pb1      // OC1A
#define PIN_D10                         hw_pin_pb2      // OC1B
#define PIN_D11                         hw_pin_pb3      // OC2A MOSI

#define PIN_A0                          hw_pin_adc0

#define PIN_LED                         hw_pin_pb5

/*  Device
 */
#include <hwa/atmega328p_au.h>
