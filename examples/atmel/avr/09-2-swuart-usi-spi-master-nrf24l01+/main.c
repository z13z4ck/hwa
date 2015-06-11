
/*  This file is part of the HWA project.
 *  Copyright (c) 2012,2015 Christophe Duparquet.
 *  All rights reserved. Read LICENSE.TXT for details.
 */

/**
 * @example
 * 
 * @par A simple interface between software UART and USI configured as SPI master.
 *
 * This is a variant of the previous example, showing how using
 * `hw_spimaster0_swclk` instead of `hw_usi0` can simplify the application.
 * 
 * @par nRF24L01+ module wiring
 *
 *              Gnd  [1](2)  Vcc
 *       Gnd <- CE   (3)(4)  CSN  -> 
 *       SCL <- SCK  (5)(6)  MOSI -> MISO
 *      MOSI <- MISO (7)(8)  IRQ
 * 
 * NOTE: pin MISO, output of the nRF, has to be connected to pin MOSI of the MCU
 * (considered as a slave regarding the pin names).
 * 
 * @par Test application
 *
 *     ./main.py
 * 
 * should display the values of the registers of an nRF24L01+ connected to
 * USI. It should be 0x08 for the config register and 0x3F for the EN_AA
 * register.
 * 
 * @par config.h
 * @include 09-2-swuart-usi-spi-master-nrf24l01+/config.h
 * 
 * Symbols:
 *
 * * `BOARD_H` is the name of the target board header file. It can be defined at
 * compile time via the command line. For example, `make BOARD=nanodccduino`
 * will define `BOARD_H` as `<boards/nanodccduino.h>`. See @ref boards for the
 * board definitions provided with HWA.
 *
 * @par main.c
 */


#include "config.h"


/*  SPI master (over USI) with software-managed clocking handled by HWA
 */
#define SPI             hw_spimaster0_swclk
#define NRF_CSN         hw_pin_6


int
main ( )
{
  /*  Create a HWA context to collect the hardware configuration
   *  Preload this context with RESET values
   */
  hwa_begin_from_reset();

  /*  Configure the software UART
   */
  hwa_config( UART );

  /*  Configure SPI (SPI master clocked by software over USI)
   */
  hwa_config( SPI );

  /*  Configure nRF CSN pin
   */
  hwa_config( NRF_CSN, direction, output );
  hwa_write(  NRF_CSN, 1 );

  /*  Write this configuration into the hardware
   */
  hwa_commit();

  hw_enable_interrupts();

  /*  Wait for UART synchronization
   */
  while ( !hw_stat(UART).synced ) {}

  /*  Process commands from host
   */
  for(;;) {

    /*  Prompt
     */
    hw_write( UART, '$' );

    /*  Get command
     */
    uint8_t c = hw_read( UART );
    if ( c == '=' ) {

      /*  Number of bytes to send to SPI slave
       */
      uint8_t ntx = hw_read( UART );
      if ( ntx < 1 || ntx > 33 )
        goto error ;

      /*  Number of bytes to send back to talker
       */
      uint8_t nrx = hw_read( UART );
      if ( nrx > 32 )
        goto error ;

      /*  Select SPI slave and send data
       */
      hw_write( NRF_CSN, 0 );
      while ( ntx-- ) {
        c = hw_read( UART );
        hw_write( SPI, c );
      }

      /*  Send reply to talker and deselect SPI slave
       */
      while ( nrx-- ) {
        hw_write( SPI, 0 );
        c = hw_read( SPI );
        hw_write( UART, c );
      }
      hw_write( NRF_CSN, 1 );
    }
    else {
      /*
       *  First byte of command must be '='. Send '!' as error indicator and
       *  wait for '\n' as error acknowledgement.
       */
      do {
      error:
        hw_write( UART, '!' );
        c = hw_read( UART );
      } while ( c != '\n' ) ;
    }
  }
}
