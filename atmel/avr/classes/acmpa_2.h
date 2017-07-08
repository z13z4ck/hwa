
/*
 * This file is part of the HWA project.
 * Copyright (c) 2012,2015 Christophe Duparquet.
 * All rights reserved. Read LICENSE.TXT for details.
 */

/**
 * @file
 * @brief Analog Comparator
 */

/**
 * @page atmelavr_acmpa
 * @section atmelavr_acmpa_config Configuration
 *
 * __Note__ When the analog comparator shares the analog multiplexer output
 * with the ADC, the ADC must be turned off for the analog comparator to be able
 * to use the analog multiplexer output.
 *
 * @code
 * hwa( configure, acmp0,
 * 
 *    [ edge,		 falling
 *		       | rising
 *		       | both,	 ]
 *
 *    [ positive_input,	 HW_PIN(ain0)
 *		       | bandgap,    ]
 *
 *    [ negative_input,	 HW_PIN(ain1)
 *		       | HW_PIN(adc0..7) ] );
 * @endcode
 */
/*
 *  NOTE: another solution could be the creation of an object `admux0` for
 *  the analog multiplexer that's shared between the ADC and the ACMP but the
 *  ADC and ACMP can not use the analog multiplexer at the same time. Then, it
 *  seems acceptable to make the configuration instructions of the ADC or of the
 *  ACMP drive the analog multiplexer.
 */
#define _hw_mtd_hwa_configure__acmpa	, _hwa_cfacmpa

/*  Optionnal parameter `edge`
 */
#define _hw_is_edge_edge		, 1
#define _hw_acmpa_edge_falling		, 2	/* ACIS */
#define _hw_acmpa_edge_rising		, 3
#define _hw_acmpa_edge_both		, 0

#define _hwa_cfacmpa(o,i,a,...)						\
  do {									\
    HW_Y(_hwa_cfacmpa_xedge,_hw_is_edge_##__VA_ARGS__)(o,__VA_ARGS__,,); \
  } while(0)

#define _hwa_cfacmpa_xedge_1(o,k,v,...)				\
  HW_Y(_hwa_cfacmpa_vedge,_hw_acmpa_edge_##v)(o,v,__VA_ARGS__)

#define _hwa_cfacmpa_vedge_0(o,v,...)					\
  HW_E_AVL(edge, v, falling | rising | both)

#define _hwa_cfacmpa_vedge_1(o,v,k,...)			\
  _hwa_write_reg(o,acis, HW_A1(_hw_acmpa_edge_##v));	\
  HW_G2(_hwa_cfacmpa_xposin,HW_IS(positive_input,k))(o,k,__VA_ARGS__)

#define _hwa_cfacmpa_xedge_0(o,k,...)					\
  HW_G2(_hwa_cfacmpa_xposin,HW_IS(positive_input,k))(o,k,__VA_ARGS__)

/*  Optionnal parameter `positive_input`
 */
#define _hw_is_positive_input_positive_input	, 1
#define _hw_is_bandgap_bandgap			, 1

#define _hwa_cfacmpa_xposin_1(o,k,v,...)				\
  HW_Y(_hwa_cfacmpa_vposin_bandgap,_hw_is_bandgap_##v)(o,v,__VA_ARGS__)

#define _hwa_cfacmpa_vposin_bandgap_1(o,v,k,...)			\
  _hwa_write_reg(o,acbg,1);						\
  HW_G2(_hwa_cfacmpa_xnegin,HW_IS(negative_input,k))(o,k,__VA_ARGS__)

#define _hwa_cfacmpa_vposin_bandgap_0(o,v,k,...)			\
  if ( HW_ID(v)==HW_ID(HW_PIN(ain0)) )					\
    _hwa_write_reg(o,acbg,0);						\
  else									\
    HWA_ERR("`positive_input` can be `HW_PIN(ain0) | bandgap`, but not`" #v "`."); \
  HW_G2(_hwa_cfacmpa_xnegin,HW_IS(negative_input,k))(o,k,__VA_ARGS__)

#define _hwa_cfacmpa_xposin_0(o,k,...)					\
  HW_G2(_hwa_cfacmpa_xnegin,HW_IS(negative_input,k))(o,k,__VA_ARGS__)

/*  Optionnal parameter `negative_input`
 */
#define _hw_is_negative_input_negative_input	, 1

#define _hwa_cfacmpa_xnegin_0(o,...)		HW_EOL(__VA_ARGS__)

#define _hwa_cfacmpa_xnegin_1(o,k,v,...)				\
  if ( HW_ID(v)==HW_ID(HW_PIN(ain1)) ) {				\
    _hwa_write_reg(o,acme,0);						\
  } else {								\
    _hwa_write_reg(o,acme,1);						\
    _hwa_write_reg(o,aden,0);						\
    if ( HW_ID(v) == HW_ID( HW_PIN(adc0) ) )				\
      _hwa_write_reg(o,admux, 0);					\
    else if ( HW_ID(v) == HW_ID( HW_PIN(adc1) ) )			\
      _hwa_write_reg(o,admux, 1);					\
    else if ( HW_ID(v) == HW_ID( HW_PIN(adc2) ) )			\
      _hwa_write_reg(o,admux, 2);					\
    else if ( HW_ID(v) == HW_ID( HW_PIN(adc3) ) )			\
      _hwa_write_reg(o,admux, 3);					\
    else if ( HW_ID(v) == HW_ID( HW_PIN(adc4) ) )			\
      _hwa_write_reg(o,admux, 4);					\
    else if ( HW_ID(v) == HW_ID( HW_PIN(adc5) ) )			\
      _hwa_write_reg(o,admux, 5);					\
    else if ( HW_ID(v) == HW_ID( HW_PIN(adc6) ) )			\
      _hwa_write_reg(o,admux, 6);					\
    else if ( HW_ID(v) == HW_ID( HW_PIN(adc7) ) )			\
      _hwa_write_reg(o,admux, 7);					\
    else								\
      HWA_ERR("`negative_input` can be `HW_PIN(ain1)`, or any "		\
	      "analog input pin, but not`" #v "`.");			\
  }									\
  HW_EOL(__VA_ARGS__)



/*******************************************************************************
 *									       *
 *	Context management						       *
 *									       *
 *******************************************************************************/

#define _hwa_setup__acmpa(o,i,a)	_hwa_setup_reg( o, csr	);

#define _hwa_init__acmpa(o,i,a)		_hwa_init_reg( o, csr, 0x00 );

#define _hwa_commit__acmpa(o,i,a)	_hwa_commit_reg( o, csr );


/**
 * @page atmelavr_acmpa
 * @section atmelavr_acmpa_internals Internals
 *
 * Class `_acmpa` objects hold the following hardware registers:
 *
 *  * `csr`: control/status register
 *
 * that hold the following logical registers:
 *
 *  * `acme`: analog comparator multiplexer enabled
 *  * `aden`: A/D converter enable
 *  * `admux`: analog multiplexer input select
 *  * `acd` : analog comparator disable (power management)
 *  * `acbg` : analog comparator bandgap select
 *  * `aco` : analog comparator output
 *  * `if` : analog comparator interrupt flag
 *  * `ie` : analog comparator interrupt enable
 *  * `acic` : analog comparator input capture enable
 *  * `acis` : analog comparator interrupt mode select
 *
 * These registers are accessible through the @ref public_ins
 * "register access intructions".
 */

/**
 * @page atmelavr_acmpa
 * <br>
 */
