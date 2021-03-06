
/* This file is part of the HWA project.
 * Copyright (c) 2012,2015 Christophe Duparquet.
 * All rights reserved. Read LICENSE.TXT for details.
 */

/**
 * @file
 * @brief 8-bit counter
 */

/**
 * @page atmelavr_c8b
 * @section atmelavr_c8b_acfg Configuration
 *
 * __Note__ The configuration of a class `_c8b` counter must be done inside the
 * same context as its compare units if they are used, so that HWA has all the
 * necessary informations to choose the correct register settings.
 *
 * @code
 * hwa( configure, counter0,
 *
 *	//  Clock source
 *	//
 *	clock,	     none			 // No clock, counter stopped
 *		   | prescaler_output(	    0	 // No clock, counter stopped
 *				      |	    1	 // System clock
 *				      |	    2	 // System clock divided by 2
 *				      |	    4	 // System clock divided by 4
 *				      |	    8
 *				      |	   16
 *				      |	   32
 *				      |	   64
 *				      |	  128
 *				      |	  256
 *				      |	  512
 *				      |	 1024
 *				      |	 2048
 *				      |	 4096
 *				      |	 8192
 *				      | 16384 ),  // System clock divided by 16384
 *
 *	//  Class _c8b counters all loop from 0 to top
 *	//
 *    [ countmode,   up_loop, ]
 *    [ bottom,	     0,	      ]
 *
 *	//  The maximum value the counter reaches (the default is `max`)
 *	//
 *    [ top,	     0xFF | 0x00FF | 255	  // Hardware fixed value 0xFF
 *		   | max			  // Hardware fixed value 0xFF
 *		   | compare2, ]		  // Value stored in the compare unit 2
 *
 *	//  When the overflow flag is set
 *	//
 *    [ overflow,    at_bottom ]		  // When the counter restarts from 0
 *     );
 * @endcode
 */
#define _hw_mtd_hwa_configure__c8b	, _hwa_cfc8b

/*  Mandatory argument `clock`
 *
 *    Add 2 void arguments to the end of the list so that there are always
 *    3 arguments following the last non-void argument.
 */
#define _hw_is_clock_clock		,  1
#define _hw_c8b_clock_none		,  0	/* , CS */
#define _hw_c8b_clock_stop		,  0
#define _hw_c8b_clock_0			,  0
#define _hw_c8b_clock_prescaler_output_0	,  0
#define _hw_c8b_clock_prescaler_output_1	,  1
#define _hw_c8b_clock_prescaler_output_2	,  2
#define _hw_c8b_clock_prescaler_output_4	,  3
#define _hw_c8b_clock_prescaler_output_8	,  4
#define _hw_c8b_clock_prescaler_output_16	,  5
#define _hw_c8b_clock_prescaler_output_32	,  6
#define _hw_c8b_clock_prescaler_output_64	,  7
#define _hw_c8b_clock_prescaler_output_128	,  8
#define _hw_c8b_clock_prescaler_output_256	,  9
#define _hw_c8b_clock_prescaler_output_512	, 10
#define _hw_c8b_clock_prescaler_output_1024	, 11
#define _hw_c8b_clock_prescaler_output_2048	, 12
#define _hw_c8b_clock_prescaler_output_4096	, 13
#define _hw_c8b_clock_prescaler_output_8192	, 14
#define _hw_c8b_clock_prescaler_output_16384	, 15
#define _hw_c8b_clock_prescaler_output(x)	HW_G2(_hw_c8b_clock_prescaler_output,x)

#define _hwa_cfc8b(o,i,a, ...)						\
  do { HW_Y(_hwa_cfc8b_kclock,_hw_is_clock_##__VA_ARGS__)(o,__VA_ARGS__,,) } while(0)

#define _hwa_cfc8b_kclock_0(o,k,...)		HW_E_VL(k,clock)
#define _hwa_cfc8b_kclock_1(o,k,v,...)		HW_Y(_hwa_cfc8b_vclock,_hw_c8b_clock_##v)(o,v,__VA_ARGS__)

#define _hwa_cfc8b_vclock_0(o,v,...)					\
  HW_E_AVL(clock, v, none | prescaler_output( 0 | 1 | 2 | 4 | 8 | 16 | 32 | 64 | 128 | 256 | 512 | 1024 | 2048 | 4096 | 8192 | 16384 ))

#define _hwa_cfc8b_vclock_1(o,v,k,...)					\
  _hwa_write_reg(o, cs, HW_A1(_hw_c8b_clock_##v));			\
  HW_Y(_hwa_cfc8b_kcountmode,_hw_is_countmode_##k)(o,k,__VA_ARGS__)

/*  Optionnal argument `countmode`
 */
#define _hw_is_countmode_countmode	, 1
#define _hw_is_up_loop_up_loop		, 1
#define _hw_is_up_up_loop_loop		, 1

#define _hwa_cfc8b_kcountmode_1(o,k,v,...)				\
  HW_G2(_hwa_cfc8b_vcountmode,HW_IS(up_loop,v))(o,v,__VA_ARGS__)

#define _hwa_cfc8b_vcountmode_0(o,v,...)				\
  HW_E_AVL(`countmode`, v, `up_loop`)

#define _hwa_cfc8b_vcountmode_1(o,v,...)	\
  _hwa_cfc8b_kcountmode_0(o,__VA_ARGS__)

#define _hwa_cfc8b_kcountmode_0(o,k,...)				\
  HW_Y(_hwa_cfc8b_kbottom,_hw_is_bottom_##k)(o,k,__VA_ARGS__)

/*  Optionnal argument `bottom`
 */
#define _hw_is_bottom_bottom		, 1

#define _hwa_cfc8b_kbottom_1(o,k,v,...)				\
    HW_G2(_hwa_cfc8b_vbottom,HW_IS(0,v))(o,v,__VA_ARGS__)

#define _hwa_cfc8b_vbottom_0(o,v,...)				\
  HW_E_AVL(`bottom`, v, `0`)

#define _hwa_cfc8b_vbottom_1(o,v,...)				\
  _hwa_cfc8b_kbottom_0(o,__VA_ARGS__)

#define _hwa_cfc8b_kbottom_0(o,k,...)				\
  HW_Y(_hwa_cfc8b_ktop,_hw_is_top_##k)(o,k,__VA_ARGS__)


/*  Optionnal argument `top`
 */
#define _hw_is_top_top			, 1	/* , CTC */
#define _hw_c8b_top_0xFF		, 0
#define _hw_c8b_top_0x00FF		, 0
#define _hw_c8b_top_255			, 0
#define _hw_c8b_top_max			, 0
#define _hw_c8b_top_compare2		, 1

#define _hwa_cfc8b_ktop_1(o,k,v,...)					\
  HW_Y(_hwa_cfc8b_vtop,_hw_c8b_top_##v)(o,v,__VA_ARGS__)

#define _hwa_cfc8b_vtop_0(o,v,...)\
  HW_E_AVL(top, v, 0xFF | 0x00FF | 255 | max | compare2)

#define _hwa_cfc8b_vtop_1(o,v,...)\
  _hwa_write_reg(o, ctc, HW_A1(_hw_c8b_top_##v));	\
  _hwa_cfc8b_ktop_0(v,__VA_ARGS__)

#define _hwa_cfc8b_ktop_0(o,k,...)					\
  HW_Y(_hwa_cfc8b_koverflow,_hw_is_overflow_##k)(o,k,__VA_ARGS__)


/*  Optionnal argument `overflow`
 */
#define _hw_is_overflow_overflow	, 1
#define _hw_is_at_bottom_at_bottom	, 1

#define _hwa_cfc8b_koverflow_1(o,k,v,...)				\
    HW_G2(_hwa_cfc8b_voverflow,HW_IS(at_bottom,v))(o,v,__VA_ARGS__)

#define _hwa_cfc8b_voverflow_0(o,v,...)				\
  HW_E_AVL(`overflow`, v, `at_bottom`)

#define _hwa_cfc8b_voverflow_1(o,v,...)				\
  _hwa_cfc8b_koverflow_0(o,__VA_ARGS__)

#define _hwa_cfc8b_koverflow_0(o,...)		HW_EOL(__VA_ARGS__)


/**
 * @page atmelavr_c8b
 * @section atmelavr_c8b_cnt Count value
 *
 * The count value is accessible for reading and writing through the following
 * instructions:
 */

/**
 * @page atmelavr_c8b
 * @code
 * hw( read, counter0 );
 * @endcode
 */
#define _hw_mtd_hw_read__c8b		, _hw_c8brd
#define _hw_c8brd(o,i,a,...)		HW_TX( _hw_read_reg(o,count), __VA_ARGS__)

/**
 * @page atmelavr_c8b
 * @code
 * hw( write, counter0, value );
 * @endcode
 */
#define _hw_mtd_hw_write__c8b		, _hw_c8bwr
#define _hw_c8bwr(o,i,a,v,...)		HW_TX( _hw_write_reg(o,count,v), __VA_ARGS__)

/**
 * @page atmelavr_c8b
 * @code
 * hwa( write, counter0, value );
 * @endcode
 */
#define _hw_mtd_hwa_write__c8b		, _hwa_c8bwr
#define _hwa_c8bwr(o,i,a,v,...)		HW_TX( _hwa_write_reg(o,count,v), __VA_ARGS__)


/**
 * @page atmelavr_c8b
 * @section atmelavr_c8b_st Status
 *
 * The overflow flag can be accessed through interrupt-related instructions:
 *
 * @code
 * if ( hw( read, HW_IRQFLAG( counter0 ) ) ) {	// Read overflow IRQ flag
 *   hw( clear, HW_IRQFLAG( counter0 ) );		// Clear overflow IRQ flag
 *   hw( turn, HW_IRQ( counter0, off ) );		// Disable overflow IRQs
 * }
 * @endcode
 */


/*******************************************************************************
 *									       *
 *	Context management						       *
 *									       *
 *******************************************************************************/

/**
 * @brief Solve the configuration of the counter and its related peripherals.
 */
/*  This kind of counter does not trigger overflow IRQs on compare match with
 *  OCRC if no compare output is in pwm mode:
 *
 *  "In the normal mode an overflow interrupt (TOV1) is generated when
 *  Timer/Counter1 counts from $FF to $00, while in the PWM mode the overflow
 *  interrupt is generated when Timer/Counter1 counts either from $FF to $00 or
 *  from OCR1C to $00. The inverted PWM outputs OC1A and OC1B are not connected
 *  in normal mode."
 *
 * -> If one compare output has been put in PWM mode, the other must have been
 *    too if it us used.
 * -> If 
 */
#define _hwa_solve__c8b( o,i,a )					\
  _hwa_solve__c8b_2( o, _HW_REL(o,compare0), _HW_REL(o,compare1) )

#define _hwa_solve__c8b_2(...)	_hwa_solve_c8b(__VA_ARGS__)

#define _hwa_solve_c8b(counter, compare0, compare1)			\
    _hwa_solve_cmp8b( compare0 );					\
    _hwa_solve_cmp8b( compare1 );					\
    if ( _hwa_mmask(compare0, pwm) && _hwa_mmask(compare1, pwm)		\
	 && _hwa_mvalue(compare0, pwm) != _hwa_mvalue(compare1, pwm) )	\
      HWA_ERR("used compare outputs must be in the same NORMAL or PWM mode."); \
    else if ( _hwa_mmask(counter, ctc) && _hwa_mvalue(counter, ctc)==0	\
	      && (   (_hwa_mmask(compare0, pwm) && _hwa_mvalue(compare1, pwm)) \
		     || (_hwa_mmask(compare1, pwm) && _hwa_mvalue(compare1, pwm))) ) \
      HWA_ERR("`top` must be `compare2` for `" #counter "` when using the PWM mode."); \
    if ( _hwa_mvalue(counter, ie)==1					\
	 && _hwa_mvalue(counter, ctc)==1				\
	 && _hwa_mvalue(compare0, pwm) == 0				\
	 && _hwa_mvalue(compare1, pwm) == 0 ) {				\
      if ( _hwa_mmask(compare0, pwm)==1 || _hwa_mmask(compare1, pwm)== 1 ) \
	HWA_ERR("`" #counter "` does not trigger overflow IRQs when not using PWM mode for outputs."); \
      else								\
	if ( _hwa_mmask(compare0, pwm)==0 )				\
	  _hwa_write_reg(compare0,pwm,1);				\
	else								\
	  _hwa_write_reg(compare1,pwm,1);				\
    }

#define _hwa_setup__c8b(o,i,a)			\
  _hwa_setup_reg( o, ccr      );		\
  _hwa_setup_reg( o, count    );		\
  _hwa_setup_reg( o, compare2 );

#define _hwa_init__c8b(o,i,a)			\
  _hwa_init_reg( o, ccr, 0	);		\
  _hwa_init_reg( o, count, 0	);		\
  _hwa_init_reg( o, compare2, 0 );

#define _hwa_commit__c8b(o,i,a)			\
  _hwa_commit_reg( o, ccr      );		\
  _hwa_commit_reg( o, count    );		\
  _hwa_commit_reg( o, compare2 );


/**
 * @page atmelavr_c8b
 * @section atmelavr_c8b_internals Internals
 *
 * Class `_c8b` objects hold the following hardware registers:
 *
 *  * `ccr`: control register
 *  * `count`: count register
 *  * `compare2`: compare register c, storing the top value
 *
 * and 1 external register, that hold the following logical registers:
 *
 *  * `ctc`: clear on compare
 *  * `cs`: clock selection
 *  * `ie`: overflow interrupt mask
 *  * `if`: overflow interrupt flag
 *
 * These registers are accessible through the @ref public_ins
 * "register access intructions".
 */


/**
 * @page atmelavr_c8b
 * <br>
 */
