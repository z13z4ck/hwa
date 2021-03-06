
/*  This file is part of the HWA project.
 *  Copyright (c) 2012,2015 Christophe Duparquet.
 *  All rights reserved. Read LICENSE.TXT for details.
 */

/**
 * @example
 *
 * Fade a LED connected to a counter compare output (method 2).
 *
 * Changes versus method 1:
 *
 *   * define PWM as `hw_ocXY` and use `HW_RELATIVE()` to retrieve the name of the
 *     counter;
 *
 *   * use a HWA context in the ISR and the couple `hwa_nocommit()` /
 *     `hwa_commit()` to write the changes to the hardware;
 *
 *   * use a seperate function to store the hardware configuration
 *     into a HWA context.
 */

#include BOARD_H


/*  The counter
 */
#define PWM			counter0compare0
#define CLKDIV			64
#define COUNTMODE		up_loop
#define TOP			0xFF

/*  Compare strings
 */
#define STRCMP(s1,s2)		__builtin_strcmp(s1,s2)


/*  Store the hardware configuration into a HWA context
 */
HW_INLINE void setup_hwa_context ( hwa_t *hwa )
{
  /*  Have the CPU enter idle mode when the 'sleep' instruction is executed.
   */
  hwa( configure,  core0,
       sleep,	   enabled,
       sleep_mode, idle );

  /*  Configure the counter to count between 0 and 0xFF
   */
  hwa( configure, HW_RELATIVE(PWM,counter),
       clock,	  prescaler_output(CLKDIV),
       countmode, COUNTMODE,
       bottom,	  0,
       top,	  TOP );

  if ( !STRCMP(HW_QUOTE(COUNTMODE),"updown_loop") )
    hwa( configure, PWM, output, clear_on_match_up_set_on_match_down );
  else /* up_loop */
    hwa( configure, PWM, output, set_at_bottom_clear_on_match );

  /*  Enable overflow IRQ
   */
  hwa( turn, HW_IRQ(HW_RELATIVE(PWM,counter),overflow), on );
}


/*  Service the counter overflow IRQ: compute the next value of the compare unit
 *
 *    Phase 0: increase duty cycle from 0 to 255
 *    Phase 1: decrease duty cycle from 255 to 0 (use ~duty)
 *    Phase 2: off
 *    Phase 3: off
 */
HW_ISR( HW_RELATIVE(PWM,counter), overflow )
{
  static uint8_t	duty ;
  static uint8_t	phase ;

  if ( phase == 0 )
    hw( write, PWM, duty );
  else if ( phase == 1 )
    hw( write, PWM, ~duty );

  duty++ ;

  if ( duty==0 ) {
    phase = (phase + 1) & 3 ;

    /*	In 'up_loop' counting mode, we must disconnect/reconnect the output of
     *	the compare unit as it can not provide pulses of less than 1 cycle.
     */
    if ( !STRCMP(HW_QUOTE(COUNTMODE),"up_loop") ) {

      /*  Start from the hardawre configuration used at initialization
       */
      hwa_begin_from_reset();
      setup_hwa_context( hwa );

      if ( phase == 2 ) {
	/*
	 *  Change the compare output config from 'set_at_bottom_clear_on_match'
	 *  to 'disconnected'
	 */
	hwa( configure, PWM, output, set_at_bottom_clear_on_match );
	hwa_nocommit();
	hwa( configure, PWM, output, disconnected );
	hwa_commit();
      }
      else if ( phase == 0 ) {
	/*
	 *  Change the compare output config from 'disconnected' to
	 *  'set_at_bottom_clear_on_match'
	 */
	hwa( configure, PWM, output, disconnected );
	hwa_nocommit();
	hwa( configure, PWM, output, set_at_bottom_clear_on_match );
	hwa_commit();
      }
    }
  }
}


int main ( )
{
  /*  Create a HWA context to collect the hardware configuration
   *  Preload this context with RESET values
   */
  hwa_begin_from_reset();

  /*  Store the hardware configuration into the HWA context
   */
  setup_hwa_context( hwa );

  /*  Write this configuration into the hardware
   */
  hwa_commit();

  hw_enable_interrupts();

  for(;;)
    hw_sleep();
}
