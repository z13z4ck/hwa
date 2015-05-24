
/* This file is part of the HWA project.
 * Copyright (c) Christophe Duparquet <duparq at free dot fr>
 * All rights reserved. Read LICENSE.TXT for details.
 */


HW_INLINE void __hwa_begin__usia ( hwa_usia_t *p, intptr_t a )
{
  _hwa_begin_reg_p( p, a, _usia, cr );
}


HW_INLINE void __hwa_init__usia ( hwa_usia_t *p )
{
  _hwa_set_r8( &p->cr, 0x00 );
}


HW_INLINE void __hwa_commit__usia ( hwa_t *hwa, hwa_usia_t *p )
{
  _hwa_commit_reg_p( p, _usia, cr );
}


/**
 * @page atmelavr_usia
 * @section atmelavr_usia_config Configuring the USI
 *
 * @code
 * hwa_config( USI,
 *
 *             //  How the USI behaves
 *             //
 *             mode,    disconnected          // The USI is disabled
 *                    | spi_master            // The USI acts as a SPI master
 *                    | spi_slave             // The USI acts as a SPI slave
 *                    | 2wire_master          // The USI acts as a I²C master
 *                    | 2wire_slave,          // The USI acts as a I²C slave
 *
 *             //  How is it clocked
 *             //
 *             clock,   software              // Clocked by software
 *                    | hw_counter0_compare   // Clocked by compare unit A of counter 0
 *                    | external_rising       // Clocked by external source rising edge
 *                    | external_falling      // Clocked by external source falling edge
 *            );
 * @endcode
 */
#define _hw_mthd_hwa_config__usia	, _hwa_cfusia

#define hw_usia_mode_disconnected	, 1
#define hw_usia_mode_spi_master		, 2
#define hw_usia_mode_spi_slave		, 3
#define hw_usia_mode_2wire_master	, 4
#define hw_usia_mode_2wire_slave	, 5

#define hw_usia_clock_software		, 0
#define hw_usia_clock_counter0_compare	, 1	/* FIXME: compare or overflow? */
#define hw_usia_clock_external_rising	, 2
#define hw_usia_clock_external_falling	, 3

#define _hw_is_mode_mode		, 1

#define _hwa_cfusia( obj,i,a,... )					\
  do {									\
    uint8_t mode, clock ;						\
    HW_G2(_hwa_cfusia_xmode, HW_IS(mode,__VA_ARGS__))(obj,__VA_ARGS__,)	\
      } while(0)
#define _hwa_cfusia_xmode_0(obj,...)					\
  HW_ERR("expected `mode` instead of `" HW_QUOTE(__VA_ARGS__) "`.")
#define _hwa_cfusia_xmode_1(obj,kw,...)					\
  HW_G2(_hwa_cfusia_vmode, HW_IS(,hw_usia_mode_##__VA_ARGS__))(obj,__VA_ARGS__)
#define _hwa_cfusia_vmode_0(obj,v,...)					\
  HW_ERR("`mode` can be `spi_master` or `spi_slave` but not`"#v"`.")
#define _hwa_cfusia_vmode_1(obj,v,...)					\
  mode = HW_A1(hw_usia_mode_##v);					\
  HW_G2(_hwa_cfusia_xclock, HW_IS(clock,__VA_ARGS__))(obj,__VA_ARGS__)

#define _hwa_cfusia_xclock_0(obj,...)					\
  HW_ERR("expected `clock` instead of `" HW_QUOTE(__VA_ARGS__) "`.")
#define _hwa_cfusia_xclock_1(obj,kw,...)				\
  HW_G2(_hwa_cfusia_vclock, HW_IS(,hw_usia_clock_##__VA_ARGS__))(obj,__VA_ARGS__)
#define _hwa_cfusia_vclock_0(obj,v,...)					\
  HW_ERR("`clock` can be `software`, `counter0_compare0`, `external_rising` " \
	 "or `external_falling` but not`"#v"`.")
#define _hwa_cfusia_vclock_1(obj,v,...)					\
  clock = HW_A1(hw_usia_clock_##v);					\
  HW_TX(_hwa_docfusia(hwa, &hwa->obj, mode, clock),__VA_ARGS__);


HW_INLINE void _hwa_docfusia( hwa_t *hwa, hwa_usia_t *p, uint8_t mode, uint8_t clock )
{
  if ( mode != HW_A1(hw_usia_mode_spi_master)
       && mode != HW_A1(hw_usia_mode_spi_slave) ) {
    HWA_ERR("sorry, desired mode is not supported yet.");
    return ;
  }
  
  if ( clock != HW_A1(hw_usia_clock_software) ) {
    HWA_ERR("sorry, desired clock mode is not supported yet.");
    return ;
  }

  if ( mode == HW_A1(hw_usia_mode_spi_master) ) {
    hwa_config(hw_pin_usck, direction, output);
    hwa_config(hw_pin_do,   direction, output);
    hwa_config(hw_pin_di,   direction, input);

    _hwa_write_creg( p, _usia, wm,  1 );
    _hwa_write_creg( p, _usia, cs,  2 );
    _hwa_write_creg( p, _usia, clk, 1 );
  }
  else {
    _hwa_write_creg( p, _usia, wm,  1 );
    _hwa_write_creg( p, _usia, cs,  2 );
    _hwa_write_creg( p, _usia, clk, 0 );
  }
}


/**
 * @page atmelavr_usia
 * @section atmelavr_usia_read Getting the last received byte
 *
 * @code
 * uint8_t byte = hw_read( hw_usi0 );
 * @endcode
 */
#define _hw_mthd_hw_read__usia		, _hw_rdusia

/*  FIXME: the datasheet advices using br instead of dr but does not tell at
 *  what moment br is valid. Reading br returns weird values...
 */
#define _hw_rdusia(o,i,a,...)		HW_TX( _hw_read_reg( o, dr ), __VA_ARGS__ )


/**
 * @page atmelavr_usia
 * @section atmelavr_usia_write Sending one byte
 *
 * @code
 * hw_write( hw_usi0, 'A' );
 * @endcode
 */
#define _hw_mthd_hw_write__usia		, _hw_wrusia

#define _hw_wrusia(o,i,a,v,...)		HW_TX( _hw_write_reg( o, dr, v ), __VA_ARGS__ )


/**
 * @page atmelavr_usia
 * @section atmelavr_usia_stat Getting the status
 *
 * @code
 * uint8_t byte ;
 * hw_stat_t( USI ) st ;       // Structure of USI status
 * st = hw_stat( USI );        // Read the status
 * if ( st.txc )               // Transmission complete?
 *   byte = hw_read( USI );
 * @endcode
 */
typedef union {
  uint8_t		byte ;
  struct {
    unsigned int __0_4    : 5 ;
    unsigned int stop     : 1 ;
    unsigned int overflow : 1 ;
    unsigned int start	  : 1 ;
  } ;
  struct {
    unsigned int __0_5    : 6 ;
    unsigned int txc      : 1 ;
    unsigned int __7	  : 1 ;
  } ;
} _hw_usia_stat_t ;


HW_INLINE _hw_usia_stat_t _hw_usia_stat( uint8_t byte )
{
  _hw_usia_stat_t	st ;
  st.byte = byte ;
  return st ;
}

#define _hw_mthd_hw_stat__usia		, _hw_stusia
#define _hw_stusia(p,i,a,...)		HW_TX(_hw_usia_stat(_hw_read_reg(p,sr)), \
					      __VA_ARGS__)

#define _hw_mthd_hw_stat_t__usia	, _hw_usia_stat_t
#define _hw_usia_stat_t(p,i,a,...)	HW_TX( usi_status_t, __VA_ARGS__)


/**
 * @page atmelavr_usia
 * @section atmelavr_usia_trigger Clocking
 *
 * FIXME: 2 types of clocking should be handled. Look at the datasheet.
 *
 * @code
 * hw_trigger( hw_usi0 );
 * @endcode
 */
#define _hw_mthd_hw_trigger__usia	, _hw_tgusia

#define _hw_tgusia(o,i,a,...)		HW_TX( _hw_write_reg(o,tc,1),__VA_ARGS__ )



/**
 * @page atmelavr_usia_spimaster_swclk
 * @section atmelavr_usia_spimaster_swclk_config Configuring the SPI
 *
 * @code
 * #define SPI		hw_spimaster0_swclk
 *
 * hw_config( SPI );
 * @endcode
 */
#define _hw_mthd_hwa_config__usia_spimaster_swclk		, _hwa_cfspimswclk

#define _hwa_cfspimswclk(p,i,usin,...)		\
  HW_TX( _hwa_docfspimswclk(hwa, &hwa->usin), __VA_ARGS__ )

HW_INLINE void _hwa_docfspimswclk( hwa_t *hwa, hwa_usia_t *p )
{
  hwa_config(hw_pin_usck, direction, output);
  hwa_config(hw_pin_do,   direction, output);

  _hwa_write_creg( p, _usia, wm,  1 );
  _hwa_write_creg( p, _usia, cs,  2 );
  _hwa_write_creg( p, _usia, clk, 1 );
}


/**
 * @page atmelavr_usia_spimaster_swclk
 * @section atmelavr_usia_spimaster_swclk_read Getting the last received byte
 *
 * @code
 * #define SPI		hw_spimaster0_swclk
 *
 * uint8_t byte = hw_read( SPI );
 * @endcode
 */
#define _hw_mthd_hw_read__usia_spimaster_swclk			, _hw_rdspimswclk

#define _hw_rdspimswclk(o,i,usio,...)		HW_TX( _hw_read_reg( usio, dr ), __VA_ARGS__ )


/**
 * @page atmelavr_usia_spimaster_swclk
 * @section atmelavr_usia_spimaster_swclk_write Sending one byte
 *
 * @code
 * hw_write( hw_spimaster0_swclk, 'A' );
 * @endcode
 */
#define _hw_mthd_hw_write__usia_spimaster_swclk		, _hw_wrspimswclk

#define _hw_wrspimswclk(p,i,usin,v,...)			\
  HW_TX(						\
	do {						\
	  _hw_write_reg(usin, dr, v );			\
	  _hw_write_reg(usin, oif, 1 );			\
	  do						\
	    _hw_write_reg(usin, tc, 1);			\
	  while( _hw_read_reg(usin, oif) == 0 );	\
	}while(0), __VA_ARGS__ )



/*	Configuration of USI as SPI master with counter0_overflow clock
 */
#define _hw_mthd_hwa_config_usia_spimaster_c0clk		, _hwa_cfspimc0clk

#define x_hwa_cfspimc0clk(c,n,i, usin)		\
  _hwa_docfspimswclk(hwa, &hwa->usin)


HW_INLINE void _hwa_docfspimc0clk( hwa_t *hwa, hwa_usia_t *p )
{
  hwa_config(hw_pin_usck, direction, output);
  hwa_config(hw_pin_do,   direction, output);
  hwa_config(hw_pin_di,   direction, input);

  _hwa_write_creg( p, _usia, wm,  1 );
  _hwa_write_creg( p, _usia, cs,  1 );
  _hwa_write_creg( p, _usia, clk, 0 );
}


#define _hw_mthd_hw_write_usia_spimaster_c0clk	, _hw_write_usia_spimaster_c0clk

#define _hw_write_usia_spimaster_c0clk(c,n,i, usin, v)	\
  _hw_write_reg(hw_##usin, dr, v )


#define _hw_mthd_hw_read_usia_spimaster_c0clk	, _hw_read_usia
