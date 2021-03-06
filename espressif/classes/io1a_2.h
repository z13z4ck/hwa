
/* This file is part of the HWA project.
 * Copyright (c) 2012,2015 Christophe Duparquet.
 * All rights reserved. Read LICENSE.TXT for details.
 */

/**
 * @file
 * @brief ESP8266 I/O
 */

/**
 * @page esp8266_io1a
 * @section esp8266_io1a_config Configuration
 *
 * The `hw(configure,)` and `hwa(configure,...)` instructions configure _io1a class I/O
 * definitions:
 *
 * @code
 * hw/hwa( configure, pa0,
 *
 *		[ function,    FUNCTION, ] // FUNCTION is dependent of the I/O that is configured.
 *
 *		[ direction,   input
 *			     | output_when_awake
 *			     | output,		 ]
 *
 *		[ pullup,      on
 *			     | when_awake
 *			     | off,	  ]
 *		  );
 * @endcode
 *
 * __Note__ `direction` and `pullup` should only be used when FUNCTION is
 * `gpio`. There is no checking here.
 */
#define _hw_mtd_hw_configure__io1a		, _hw_cfio1a
#define _hw_mtd_hwa_configure__io1a		, _hwa_cfio1a

#define _hw_cfio1a( o,i, p,bn,bp, ...)		_hw_cfio1a2( o, o##_cf, p,bn,bp, __VA_ARGS__)
#define _hw_cfio1a2(...)			_hw_cfio1a3( __VA_ARGS__ )
#define _hw_cfio1a3( o,cf,p,bn,bp, ...)					\
  do{									\
    typedef struct {							\
      uint8_t		commit ;					\
      hwa_shared_t	shared ;					\
      hwa_p16a_t	p ;						\
      hwa_pcfa_t	cf ;						\
    } hwa_t ;								\
    hwa_t hwa_st ; hwa_t *hwa= &hwa_st ;				\
    _hwa_setup( shared ); _hwa_setup( p ); _hwa_setup( cf );		\
    HW_Y(_hwa_cfio1a_kfunction,_hw_is_function_##__VA_ARGS__)(o,cf,p,bn,bp,__VA_ARGS__,,); \
    hwa->commit = 1;							\
    _hwa_commit( shared ); _hwa_commit( p ); _hwa_commit( cf );		\
  }while(0)

#define _hwa_cfio1a( o,i, p,bn,bp, ...)		_hwa_cfio1a2( o, o##_cf, p,bn,bp, __VA_ARGS__)
#define _hwa_cfio1a2(...)			_hwa_cfio1a3( __VA_ARGS__ )
#define _hwa_cfio1a3( o,cf,p,bn,bp, ...)				\
  HW_Y(_hwa_cfio1a_kfunction,_hw_is_function_##__VA_ARGS__)(o,cf,p,bn,bp,__VA_ARGS__,,); \

/*  Optionnal parameter `function`
 */
#define _hwa_cfio1a_kfunction_1(o,cf,p,bn,bp,k,v,...)				\
  HW_Y(_hwa_cfio1a_vfunction,_hw_##o##_fn_##v)(o,cf,p,bn,bp,v,__VA_ARGS__)

#define _hwa_cfio1a_vfunction_0(o,cf,p,bn,bp,v,...)	HW_E_IOFN(o, function, v, _hw_##o##_fns)

#define _hwa_cfio1a_vfunction_1(o,cf,p,bn,bp,v,k,...)			\
  _hwa_write_reg( cf, fn, HW_A1(_hw_##o##_fn_##v) );			\
  HW_A2(_##o##_fn_##v) /* Optionnal supplement of actions, e.g. swap  */ \
  HW_Y(_hwa_cfio1a_kdirection,_hw_is_direction_##k)(o,cf,p,bn,bp,k,__VA_ARGS__)

#define _hwa_cfio1a_kfunction_0(o,cf,p,bn,bp,k,...)				\
  HW_Y(_hwa_cfio1a_kdirection,_hw_is_direction_##k)(o,cf,p,bn,bp,k,__VA_ARGS__)

#define _hw_is_function_function	, 1


/*  Optionnal parameter `direction`
 */
#define _hwa_cfio1a_kdirection_1(o,cf,p,bn,bp,k,v,...)				\
  HW_Y(_hwa_cfio1a_vdirection,_hw_cfio1a_direction_##v)(o,cf,p,bn,bp,v,__VA_ARGS__)

#define _hwa_cfio1a_vdirection_0(o,cf,p,bn,bp,v,...)	HW_E_AVL(direction, v, input | output | output_when_awake)

#define _hwa_cfio1a_vdirection_1(o,cf,p,bn,bp,v,k,...)			\
  _hwa_write_reg( cf, oex, HW_A1(_hw_cfio1a_direction_##v) );	\
  if ( HW_A1(_hw_cfio1a_direction_##v) != 0 )				\
    _hwa_write_reg_m( p, _enb, 1UL<<bp, 1UL<<bp );			\
  HW_Y(_hwa_cfio1a_kpullup,_hw_is_pullup_##k)(o,cf,p,bn,bp,k,__VA_ARGS__)

#define _hwa_cfio1a_kdirection_0(o,cf,p,bn,bp,k,...)				\
  HW_Y(_hwa_cfio1a_kpullup,_hw_is_pullup_##k)(o,cf,p,bn,bp,k,__VA_ARGS__)

#define _hw_is_direction_direction	, 1
#define _hw_cfio1a_direction_input	, 0	/* oex */
#define _hw_cfio1a_direction_output_when_awake	, 1
#define _hw_cfio1a_direction_output_when_sleeping	, 2
#define _hw_cfio1a_direction_output	, 3


/*  Optionnal parameter `pullup`
 */
#define _hwa_cfio1a_kpullup_1(o,cf,p,bn,bp,k,v,...)				\
    HW_Y(_hwa_cfio1a_vpullup0,_hw_cfio1a_pullup_##v)(o,cf,p,bn,bp,v,__VA_ARGS__)

#define _hwa_cfio1a_vpullup_1(o,cf,p,bn,bp,v,...)	\
    _hwa_write_reg( cf, pux, HW_A1(_hw_cfio1a_pullup_##v) );	\
    HW_EOL(__VA_ARGS__)

#define _hwa_cfio1a_vpullup_0(o,cf,p,bn,bp,v,...)	HW_AVL(pullup, v, on | off | when_awake)
#define _hwa_cfio1a_vpullup0_0(o,cf,p,bn,bp,v,...)	HW_AVL(pullup, v, on | off | when_awake)
#define _hwa_cfio1a_kpullup_0(o,cf,p,bn,bp,...)		HW_EOL(__VA_ARGS__)

#define _hw_is_pullup_pullup		, 1
#define _hw_cfio1a_pullup_off		, 0	/* pux */
#define _hw_cfio1a_pullup_when_sleeping	, 1
#define _hw_cfio1a_pullup_when_awake	, 2
#define _hw_cfio1a_pullup_on		, 3


/**
 * @page esp8266_io1a
 * @section esp8266_io1a_read State
 *
 * The instruction `read` returns the state of the I/O object:
 *
 * @code
 * uint8_t value = hw( read, gpio0 );
 * @endcode
 */
#define _hw_mtd_hw_read__io1a		, _hw_read_io1a

#define _hw_read_io1a(o,i, p,bn,bp,...)				\
  HW_TX( ((_hw_read_reg(p, _in) & (((1UL<<bn)-1)<<bp))>>bp),	\
	 __VA_ARGS__)


/**
 * @page esp8266_io1a
 *
 * The instruction `hw_write()` changes the state of the I/O object:
 *
 * @code
 * hw( write, gpio0, value );
 * @endcode
 */
#define _hw_mtd_hw_write__io1a		, _hw_wrio1a

#define _hw_wrio1a(o,i, p,bn,bp, v,...)			\
  HW_TX( _hw_write_reg_m(p, _out, ((1UL<<bn)-1)<<bp, (v)<<bp),	\
	 __VA_ARGS__ )


/**
 * @page esp8266_io1a
 *
 * @code
 * hwa( write, gpio0, value );
 * @endcode
 */
#define _hw_mtd_hwa_write__io1a		, _hwa_wrio1a

#define _hwa_wrio1a(o,i, p,bn,bp, v, ...)				\
  HW_TX(_hwa_write_reg_m(&hwa->p._out, ((1UL<<bn)-1)<<bp, (v)<<bp)),	\
    __VA_ARGS__)


/**
 * @page esp8266_io1a
 * @code
 * hw( toggle, gpio0 );	//  Toggle one or several consecutive pins at once
 * @endcode
 */
#define _hw_mtd_hw_toggle__io1a		, _hw_tgio1a

#define _hw_tgio1a(o,i,p,bn,bp,...)	HW_TX( _hw_tgio1a_2(_HW_A(_HW_R(p,_out)),(((1UL<<bn)-1)<<bp)), __VA_ARGS__)
#define _hw_tgio1a_2(r,msk)		*(volatile uint32_t *)r = *(volatile uint32_t *)r ^ msk

/*
 * @page esp8266_io1a
 * @code
 * // All the pins of the same I/O port toggled in the same transaction will be
 * // toggled at once by the `hwa_commit()` instruction.
 * //
 * hwa_toggle( pa0 );
 * @endcode
 */

/* #define _hwa_toggle_io1a(o,i, p,...)		_hwa_toggle_io1a_2(_hw_reg(p,pin),__VA_ARGS__) */
/* #define _hwa_toggle_io1a_2(...)			_hwa_toggle_io1a_3(__VA_ARGS__) */
/* #define _hwa_toggle_io1a_3(_m1,p,a,r,rw,ra,rwm,rfm,_bn,_bp,bn,bp,...)	\ */
/*   HW_TX(_hwa_write(_m1,p,a,r,rw,ra,rwm,rfm,bn,bp, 1),__VA_ARGS__) */

/**
 * @page esp8266_io1a
 * <br>
 */
