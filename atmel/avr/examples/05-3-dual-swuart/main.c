
/*  This file is part of the HWA project.
 *  Copyright (c) 2012,2015 Christophe Duparquet.
 *  All rights reserved. Read LICENSE.TXT for details.
 */

/**
 * @example
 *
 * @par Dual software UART
 *
 * Echo what is received from one software UART to both. The first UART that
 * synchronizes sets the other up.
 *
 * This is totally reversible, the LED shows when one UART resynchronizes.
 *
 * Notes:
 *
 * 1. Do not leave an RX pin floating!
 *
 * 2. Choose a moderate baudrate so that the CPU has the time to service
 * interrupts in a relatively reasonable time (115200 bps seems OK for
 * `hw_counter1` with internal 8 MHz RC oscilltor and OSCCAL=0xFF or the 16 MHz
 * Nanodccduino).
 *
 * 3. CH340 USB/Serial adapter does not send parity bits under Linux (though no
 * error is triggered). Then, the software UART never resynchronizes (it works
 * in a Windows XP VirtualBoxed under Linux, so there must be a bug in the Linux
 * driver).
 *
 * @par Test application
 *
 *      ./main.py -b BAUDRATE
 *
 * @par config.h
 * @include 05-3-dual-swuart/config.h
 *
 * @par main.c
 */

#include "config.h"

#if defined HW_DEVICE_ATTINYX5
#  error ATtinyX5 devices are not supported because the same pin-change interrupt controller can not be used for 2 different software UARTS
#endif


int
main ( )
{
  /*  Increase the frequency of the RC oscillator to the max
   */
  hw_write_reg( hw_core0, osccal, 0xFF );

  /*  Create a HWA context to collect the hardware configuration
   *  Preload this context with RESET values
   */
  hwa_begin_from_reset();

  /*  Configure the software UARTs
   */
  hwa_config( hw_swuart0 );
  hwa_config( hw_swuart1 );

  hwa_config( PIN_LED, direction, output );

  /*  Have the CPU enter idle mode when the 'sleep' instruction is executed.
   */
  hwa_config( hw_core0,
              sleep,      enabled,
              sleep_mode, idle );

  /*  Write this configuration into the hardware
   */
  hwa_commit();

  /*  Interrupt requests must be serviced for swuart
   */
  hw_enable_interrupts();


  /*  Main loop
   */
  for(;;) {
    /*
     *  Wait that one UART resynchronizes and configure the other with the same
     *  baudrate
     */
    hw_write( PIN_LED, 1 );

    hw_reset( hw_swuart0 );
    hw_reset( hw_swuart1 );
    for(;;) {
      hw_sleep();
      if ( hw_stat(hw_swuart0).synced ) {
        hw_write( hw_swuart0, '$');     /* signal the synchronization */
        hw_write_reg( hw_swuart1, dt0, hw_read_reg( hw_swuart0, dt0 ) );
        hw_write_reg( hw_swuart1, dtn, hw_read_reg( hw_swuart0, dtn ) );
        hw_write_reg( hw_swuart1, synced, 1 );
        hw_write( hw_swuart1, '$');     /* signal the synchronization */
        break ;
      }
      if ( hw_stat(hw_swuart1).synced ) {
        hw_write( hw_swuart1, '$');     /* signal the synchronization */
        hw_write_reg( hw_swuart0, dt0, hw_read_reg( hw_swuart1, dt0 ) );
        hw_write_reg( hw_swuart0, dtn, hw_read_reg( hw_swuart1, dtn ) );
        hw_write_reg( hw_swuart0, synced, 1 );
        hw_write( hw_swuart0, '$');     /* signal the synchronization */
        break ;
      }
    }
    hw_write( PIN_LED, 0 );

    /*
     *  Send on one UART what has been received from the other
     */
    for(;;) {
      hw_sleep();
      if ( hw_stat(hw_swuart0).rxc ) {
        /*
         *  UART0 -> UART0 + UART1
         */
        if ( hw_stat(hw_swuart0).stop == 0 )
          break ;       /* null stop bit -> resynchronize */

        uint8_t byte = hw_read(hw_swuart0);
        hw_write( hw_swuart0, byte );
        hw_write( hw_swuart1, byte );
      }
      if ( hw_stat(hw_swuart1).rxc ) {
        /*
         *  UART1 -> UART1 + UART0
         */
        if ( hw_stat(hw_swuart1).stop == 0 )
          break ;       /* null stop bit -> resynchronize */

        uint8_t byte = hw_read(hw_swuart1);
        hw_write( hw_swuart1, byte );
        hw_write( hw_swuart0, byte );
      }
    }
  }
}