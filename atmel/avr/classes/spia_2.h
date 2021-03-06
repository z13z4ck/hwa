
/* This file is part of the HWA project.
 * Copyright (c) 2012,2015 Christophe Duparquet.
 * All rights reserved. Read LICENSE.TXT for details.
 */

/**
 * @file
 * @brief Serial Peripheral Interface
 */

/**
 * @page atmelavr_spia
 * @section atmelavr_spia_cf Configuration
 *
 * __Note 1__ When configured in master or slave mode, the SPI automatically
 * configures its related I/O pins as inputs or outputs according to the mode.
 *
 * __Note 2__ In master mode, if the SS pin is configured as an input, it must
 *  be held high.  Otherwise the SPI will be automatically turned into slave
 *  mode by hardware as soon as the SS pin goes low.

 * @code
 * hwa( configure, spi0,
 *
 *	       //  Mode
 *	       //
 *	       mode,		master
 *			      | slave
 *			      | off,
 *
 *	       //  Clock frequency
 *	       //
 *	       clock,		sysclk_div(    2
 *					   |   4
 *					   |   8
 *					   |  16
 *					   |  32
 *					   |  64
 *					   | 128 ),
 *
 *	       //  State of SCK when idle
 *	       //
 *	       sck_idle,	low
 *			      | high,
 *
 *	       //  Sampling clock edge
 *	       //
 *	       sampling_edge,	falling
 *			      | rising
 *			      | first
 *			      | second
 *	     );
 * @endcode
 */
#define _hw_mtd_hwa_configure__spia	, _hwa_cfspia

/*	Mandatory parameter `mode`
 */
#define _hw_is_mode_mode		, 1
#define _hw_spia_mode_master		, 1, 1	/* , en , mstr */
#define _hw_spia_mode_slave		, 1, 0
#define _hw_spia_mode_off		, 0, 0

#define _hwa_cfspia(o,i,a,...)						\
  do {									\
    HW_Y(_hwa_cfspia_kmode,_hw_is_mode_##__VA_ARGS__)(o,__VA_ARGS__,,); \
  } while(0)

#define _hwa_cfspia_kmode_0(o,k,...)					\
  HW_E_VL(k,mode)

#define _hwa_cfspia_kmode_1(o,k,v,...)				\
  HW_Y(_hwa_cfspia_vmode,_hw_spia_mode_##v)(o,v,__VA_ARGS__)

#define _hwa_cfspia_vmode_0(o,v,...)					\
  HW_E_AVL(mode, v, master | slave | off)

#define _hwa_cfspia_vmode_1(o,v,k,...)				\
  _hwa_write_reg(o,en, HW_A1(_hw_spia_mode_##v));		\
  _hwa_write_reg(o,mstr, HW_A2(_hw_spia_mode_##v));		\
  if ( HW_A1(_hw_spia_mode_##v) == 1 ) {			\
    if ( HW_A2(_hw_spia_mode_##v) == 0 )			\
      _hwa( configure, _HW_REL(o,pin_miso), direction, output );	\
    else {							\
      _hwa( configure, _HW_REL(o,pin_mosi), direction, output );	\
      _hwa( configure, _HW_REL(o,pin_sck), direction, output );	\
    }								\
  }								\
  HW_Y(_hwa_cfspia_kclock,_hw_is_clock_##k)(o,k,__VA_ARGS__)

/*	Mandatory parameter `clock`
 */
#define _hw_is_clock_clock		, 1
#define _hw_spia_clock_sysclk_div_2	, 4	/* , xpr */
#define _hw_spia_clock_sysclk_div_4	, 0
#define _hw_spia_clock_sysclk_div_8	, 5
#define _hw_spia_clock_sysclk_div_16	, 1
#define _hw_spia_clock_sysclk_div_32	, 6
#define _hw_spia_clock_sysclk_div_64	, 2
#define _hw_spia_clock_sysclk_div_128	, 3
#define _hw_spia_clock_sysclk_div(x)		HW_G2(_hw_spia_clock_sysclk_div,x)

#define _hwa_cfspia_kclock_0(o,k,...)			\
  HW_E_VL(k,clock)

#define _hwa_cfspia_kclock_1(o,k,v,...)					\
  HW_Y(_hwa_cfspia_vclock,_hw_spia_clock_##v)(o,v,__VA_ARGS__)

#define _hwa_cfspia_vclock_0(o,v,...)					\
  HW_E_AVL(clock, v, sysclk_div( 1 | 2 | 4 | 8 | 16 | 32 | 64 | 128 ))

#define _hwa_cfspia_vclock_1(o,v,k,...)				\
  _hwa_write_reg(o,xpr, HW_A1(_hw_spia_clock_##v));			\
  HW_G2(_hwa_cfspia_ksck_idle,HW_IS(sck_idle,k))(o,k,__VA_ARGS__)

/*	Mandatory parameter `sck_idle`
 */
#define _hw_is_sck_idle_sck_idle	, 1
#define _hw_spia_sck_idle_low		, 0	/* , cpol */
#define _hw_spia_sck_idle_high		, 1

#define _hwa_cfspia_ksck_idle_0(o,k,...)		\
  HW_E_VL(k,sck_idle)

#define _hwa_cfspia_ksck_idle_1(o,k,v,...)				\
  HW_Y(_hwa_cfspia_vsck_idle,_hw_spia_sck_idle_##v)(o,v,__VA_ARGS__)

#define _hwa_cfspia_vsck_idle_0(o,v,...)			\
  HW_E_AVL(sck_idle, v, low | high)

#define _hwa_cfspia_vsck_idle_1(o,v,k,...)				\
  _hwa_write_reg(o,cpol, HW_A1(_hw_spia_sck_idle_##v));		\
  HW_G2(_hwa_cfspia_ksampling_edge,HW_IS(sampling_edge,k))(o,v,k,__VA_ARGS__)

/*	Mandatory parameter `sampling_edge`
 */
#define _hw_is_sampling_edge_sampling_edge	, 1
#define _hw_spia_sck_idle_low_sampling_edge_rising	, 0	/* , cpol */
#define _hw_spia_sck_idle_low_sampling_edge_falling	, 1
#define _hw_spia_sck_idle_high_sampling_edge_rising	, 1
#define _hw_spia_sck_idle_high_sampling_edge_falling	, 0
#define _hw_spia_sck_idle_low_sampling_edge_first	, 0
#define _hw_spia_sck_idle_low_sampling_edge_second	, 1
#define _hw_spia_sck_idle_high_sampling_edge_first	, 0
#define _hw_spia_sck_idle_high_sampling_edge_second	, 1

#define _hwa_cfspia_ksampling_edge_0(o,v,k,...)			\
  HW_E_VL(k,sampling_edge)

#define _hwa_cfspia_ksampling_edge_1(o,idle,k,v,...)			\
  HW_G2(_hwa_cfspia_vsampling_edge,					\
	HW_IS(,_hw_spia_sck_idle_##idle##_sampling_edge_##v))(o,idle,v,__VA_ARGS__)

#define _hwa_cfspia_vsampling_edge_0(o,idle,v,...)			\
  HW_E_AVL(sampling_edge, v, low | high)

#define _hwa_cfspia_vsampling_edge_1(o,idle,v,k,...)			\
  HW_TX( _hwa_write_reg(o,cpha, HW_A1(_hw_spia_sck_idle_##idle##_sampling_edge_##v)),\
	 __VA_ARGS__)


/**
 * @page atmelavr_spia
 * @section atmelavr_spia_data Data
 */

/**
 * @page atmelavr_spia
 * @code
 * hw( read, spi0 );
 * @endcode
 */
#define _hw_mtd_hw_read__spia		, _hw_read_spia
#define _hw_read_spia(o,i,a,...)	HW_TX(_hw_read_reg(o,dr),__VA_ARGS__)


/**
 * @page atmelavr_spia
 * @code
 * hw_write( spi0, value );
 * @endcode
 */
#define _hw_mtd_hw_write__spia		, _hw_wrspia
#define _hw_wrspia(o,i,a,v,...)		HW_TX(_hw_write_reg(o,dr,v),__VA_ARGS__)

/* #define _hw_wrspia(o,i,a,v,...)						\ */
/*   HW_TX( __hw_wrspia( _hw_rap(o,if), _HW_A(_HW_M(o,dr)), v ), __VA_ARGS__) */

/* HW_INLINE uint8_t __hw_wrspia( intptr_t flag_addr, uint8_t flag_bp, intptr_t dr, uint8_t v ) */
/* { */
/*   uint8_t flag = 1U<<flag_bp ; */
/*   *(volatile uint8_t*)dr = v ; */
/*   while (1) { */
/*     uint8_t sr = *(volatile uint8_t*)flag_addr ; */
/*     if ( sr & flag ) */
/*	 return *(volatile uint8_t*)dr ; */
/*   } */
/* } */

/* /\** */
/*  * @page atmelavr_spia */
/*  * @code */
/*  * hwa_write( spi0, value ); */
/*  * @endcode */
/*  *\/ */
/* #define _hw_mtd_hwa_write__spia	, _hwa_write_spia */
/* #define _hwa_write_spia(o,i,a,v)	_hwa_write_reg(o,dr,v) */


/**
 * @page atmelavr_spia
 * @section atmelavr_spia_turn Turning on/off
 *
 * When the SPI is turned on, it takes control of the MOSI and MISO pins.
 *
 * @code
 * hw( turn, spi0, on | off );
 * @endcode
 */
#define _hw_mtd_hw_turn__spia		, _hw_turn_spia

#define _hw_turn_spia(o,i,a, ...)					\
  HW_Y(_hw_turn_spia,_hw_state_##__VA_ARGS__)(o,__VA_ARGS__)
#define _hw_turn_spia_0(o,v, ...)			\
  HW_E_ST(v)
#define _hw_turn_spia_1(o,v, ...)				\
  HW_TX(_hw_write_reg(o, en, HW_A1(_hw_state_##v)),__VA_ARGS__)

/**
 * @page atmelavr_spia
 * @code
 * hwa( turn, spi0, on | off );
 * @endcode
 */
#define _hw_mtd_hwa_turn__spia		, _hwa_turn_spia

#define _hwa_turn_spia(o,i,a, ...)					\
  HW_Y(_hwa_turn_spia,_hw_state_##__VA_ARGS__)(o,__VA_ARGS__)
#define _hwa_turn_spia_0(o,v, ...)			\
  HW_E_ST(v)
#define _hwa_turn_spia_1(o,v, ...)				\
  HW_TX(_hwa_write_reg(o, en, HW_A1(_hw_state_##v)),__VA_ARGS__)


/**
 * @page atmelavr_spia
 * @section atmelavr_spia_st Status
 *
 * Reading the "transfer complete" flag:
 *
 * @code
 * if ( hw( read, HW_IRQFLAG( spi0 ) ) ) {  // Read "transfer complete" IRQ flag
 *   hw( turn, HW_IRQ( spi0, off ) );		   // Disable transfer complete IRQs
 * }
 * @endcode
 *
 * The "transfer complete" flag is cleared by hardware when the corresponding
 * ISR is executed. It is also cleared when the SPI is read or written after the
 * flag has been read:
 *
 * @code
 * if ( hw( read, HW_IRQFLAG( spi0 ) ) )  // Read transfer complete IRQ flag
 *   data = hw( read, spi0 );		   // Read data and clear transfer complete IRQ flag
 * @endcode
 *
 * The `hw(stat,)` instruction lets you read the `collision` flag:
 *
 * @code
 * //  Create a structure to receive the counter status
 * //
 * hw_stat_t( spi0 ) st ;
 *
 * //  Copy the SPI status into the structure
 * //
 * st = hw( stat, spi0 );
 *
 * //  Process the collision flag
 * //
 * if ( st.collision )
 *   n_collisions++ ;
 * @endcode
 */
#define _hw_mtd_hw_stat__spia		, _hw_stat_spia
#define _hw_mtd_hw_stat_t__spia	, _hw_statt_spia

#define _hw_statt_spia(o,i,a,...)	HW_TX(_hw_spia_stat_t, __VA_ARGS__)

typedef union {
  uint8_t	  byte ;
  struct {
    unsigned int  _0_5	    : 6 ;
    unsigned int  collision : 1 ;
    unsigned int  _7	    : 1 ;
  };
} _hw_spia_stat_t ;

#define _hw_stat_spia(o,i,a,...)	HW_TX(_hw_spia_stat(_hw_read_reg(o, sr)), __VA_ARGS__)

HW_INLINE _hw_spia_stat_t _hw_spia_stat( uint8_t byte )
{
  _hw_spia_stat_t	st ;
  st.byte = byte ;
  return st ;
}


/*******************************************************************************
 *									       *
 *	Context management						       *
 *									       *
 *******************************************************************************/

#define _hwa_setup__spia(o,i,a)		\
  _hwa_setup_reg( o, cr );			\
  _hwa_setup_reg( o, sr )

#define _hwa_init__spia(o,i,a)			\
  _hwa_init_reg( o, cr, 0x00 );			\
  _hwa_init_reg( o, sr, 0x00 )

#define _hwa_commit__spia(o,i,a)		\
  _hwa_commit_reg( o, cr );			\
  _hwa_commit_reg( o, sr )



/**
 * @page atmelavr_spia
 * @section atmelavr_spia_internals Internals
 *
 * Class `_spia` objects hold the following hardware registers:
 *
 *  * `cr`: control register
 *  * `sr`: status register
 *  * `dr`: data register
 *
 * that hold the following logical registers:
 *
 *  * `en`: spi enable
 *  * `dord`: data order
 *  * `mstr`: master/slave mode
 *  * `cpol`: clock polarity
 *  * `cpha`: clock phase
 *  * `xpr`: clock prescaler
 *  * `wcol`: write collision
 *  * `sp2x`: double clock speed
 *  * `ie`: overflow interrupt mask
 *  * `if`: overflow interrupt flag
 *
 * These registers are accessible through the @ref public_ins
 * "register access intructions".
 */


/**
 * @page atmelavr_spia
 * <br>
 */
