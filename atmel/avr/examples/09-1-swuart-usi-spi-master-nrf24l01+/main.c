
/*  This file is part of the HWA project.
 *  Copyright (c) 2012,2015 Christophe Duparquet.
 *  All rights reserved. Read LICENSE.TXT for details.
 */

/**
 * @example
 * 
 * @par A simple interface between software UART and USI configured as SPI master.
 * 
 * @par nRF24L01+ module wiring
 *
 *                   Gnd  [1](2)  Vcc
 *            Gnd <- CE   (3)(4)  CSN  -> 
 *            SCL <- SCK  (5)(6)  MOSI -> DO or MOSI
 *     MISO or DI <- MISO (7)(8)  IRQ
 * 
 * __Note__: for devices that use an USI to emulate an SPI interface, the MCU is
 * considered a slave regarding the SPI pin names. Pin MISO, output of the nRF,
 * has to be connected to pin MOSI/DI of the MCU, and pin MOSI, input of the
 * nRF, has to be connected to pin MISO/DO of the MCU.
 *
 * @par Test application
 *
 *     ./main.py
 * 
 * should display:
 *
 *     Register CONFIG    : 0x00 = 08
 *     Register EN_AA     : 0x01 = 3F
 *     Register EN_RX_ADDR: 0x02 = 03
 *     Register SETUP_AW  : 0x03 = 03
 *     Register SETUP_RETR: 0x04 = 03
 *     Register RF_CH     : 0x05 = 02
 *     Register RF_SETUP  : 0x06 = 0F
 *     Register STATUS    : 0x07 = 0E
 *     Register RX_ADDR_P0: 0x0A = E7 E7 E7 E7 E7
 *     Register RX_ADDR_P1: 0x0B = C2 C2 C2 C2 C2
 *     Register DYNPD     : 0x1C = 00
 *     Register FEATURE   : 0x1D = 00
 * 
 * @par config.h
 * @include 09-1-swuart-usi-spi-master-nrf24l01+/config.h
 * 
 * Symbols:
 *
 * * `BOARD_H` is the name of the target board header file. It can be defined at
 * compile time via the command line. For example, `make BOARD=nanodccduino`
 * will define `BOARD_H` as `<boards/nanodccduino.h>`. See @ref atmelavr_boards for the
 * board definitions provided with HWA.
 * 
 * @par main.c
 **/

#include "config.h"

/*  We need a device with an USI
 */
#if hw_id(hw_usi0) == 0
HW_ERROR( device `HW_DEVICE` does not have a USI. )
#endif


#if defined ARDUINO
#  define USI		hw_spi0
#  define NRF_CSN	PIN_D2
#else
#  define USI		hw_usi0
#  define NRF_CSN	hw_pin_3
#endif


/*  USI is configured as SPI master clocked by software.
 *  We then need to clock it ourselves!
 */
static void write_usi ( char c )
{
  hw_write( USI, c );
  hw_clear_irqf( USI, txc );
  do {
    hw_trigger( USI );
    // hw_delay_cycles( 50e-6 * hw_syshz );
  }
  while ( !hw_stat_irqf(USI,txc) );
}


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

  /*  Configure the USI as SPI master clocked by software
   */
  hwa_config( USI,
	      mode,  spi_master,
	      clock, software );

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
  while ( !hw_stat(UART).sync ) {}

  /*  Process commands from host
   */
  for(;;) {

    /*	Prompt
     */
    hw_write( UART, '$' );

    /*	The host sends commands starting with '=' and followed by:
     *	  * the number of bytes to send to SPI slave (1 byte)
     *	  * the number of bytes to read (1 byte)
     *	  * the bytes to send
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
	write_usi( c );
      }

      /*  Send reply to host and deselect SPI slave
       */
      while ( nrx-- ) {
	write_usi( 0 );
	c = hw_read( USI );
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
