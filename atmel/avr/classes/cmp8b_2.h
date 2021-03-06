
/* This file is part of the HWA project.
 * Copyright (c) 2012,2015 Christophe Duparquet.
 * All rights reserved. Read LICENSE.TXT for details.
 */

/**
 * @file
 * @brief 8-bit compare unit
 */

/**
 * @page atmelavr_cmp8b
 * @section atmelavr_cmp8b_acfg Configuration
 *
 * @code
 * hwa( configure, compare0,
 *
 *	   [   update,	   at_top, ]
 *
 *	   [   output
 *	     | output_h,   disconnected
 *			 | toggle_on_match
 *			 | clear_on_match
 *			 | set_on_match
 *			 | set_at_bottom_clear_on_match
 *			 | clear_at_bottom_set_on_match, ]
 *
 *	   [   output_l,   disconnected
 *			 | clear_at_bottom_set_on_match	 ]
 *	     );
 * @endcode
 */

#define _hw_mtd_hwa_configure__cmp8b	, _hwa_cfcmp8b


/*  Optionnal argument `update`
 *
 *    Add a second void argument to the end of the list so that there are always
 *    at least 2 arguments following the last non-void argument.
 */
#define _hw_is_update_update		, 1
#define _hw_is_at_top_at_top		, 1

#define _hwa_cfcmp8b(o,i,a, ...)						\
  do {									\
    HW_Y(_hwa_cfcmp8b_kupdate,_hw_is_update_##__VA_ARGS__)(o,__VA_ARGS__,) \
      } while(0)

#define _hwa_cfcmp8b_kupdate_1(o,kw,v,...)				\
  HW_G2(_hwa_cfcmp8b_vupdate,HW_IS(at_top,v))(o,v,__VA_ARGS__)

#define _hwa_cfcmp8b_vupdate_0(o,v,...)				\
  HW_E_AVL(`update`, v, `at_top`)

#define _hwa_cfcmp8b_vupdate_1(o,v,...)		\
  _hwa_cfcmp8b_kupdate_0(o,__VA_ARGS__)

#define _hwa_cfcmp8b_kupdate_0(o,kw,...)					\
  HW_Y(_hwa_cfcmp8b_koutputh,_hw_cmp8b_koutputh_##kw)(o,kw,__VA_ARGS__)


/*  Optionnal argument `output` (or `output_h`)
 */
#define _hw_cmp8b_koutputh_output	, 1
#define _hw_cmp8b_koutputh_output_h	, 1

/*								// COM	PWM
 */
#define _hw_cmp8b_voutputh_disconnected	, 0	// 0	0
#define _hw_cmp8b_voutputh_toggle_on_match	, 1	// 1	0
#define _hw_cmp8b_voutputh_clear_on_match	, 2	// 2	0
#define _hw_cmp8b_voutputh_set_on_match	, 3	// 3	0
#define _hw_cmp8b_voutputh_set_at_bottom_clear_on_match	, 4	// 1,2	1
#define _hw_cmp8b_voutputh_clear_at_bottom_set_on_match	, 5	// 3	1


#define _hwa_cfcmp8b_koutputh_1(o,kw,v,...)				\
  HW_Y(_hwa_cfcmp8b_voutputh,_hw_cmp8b_voutputh_##v)(o,v,__VA_ARGS__)

#define _hwa_cfcmp8b_voutputh_0(o,v,...)					\
  HW_E_AVL(`output_h` (or `output`), v, `disconnected | toggle_on_match | clear_on_match | set_on_match | set_at_bottom_clear_on_match | clear_at_bottom_set_on_match`)

#define _hwa_cfcmp8b_voutputh_1(o,v,...)		\
  hwa->o.config.outputh = HW_A1(_hw_cmp8b_voutputh_##v);	\
  _hwa_cfcmp8b_koutputh_0(o,__VA_ARGS__)

#define _hwa_cfcmp8b_koutputh_0(o,kw,...)				\
  HW_G2(_hwa_cfcmp8b_koutputl,HW_IS(output_l,kw))(o,kw,__VA_ARGS__)


/*  Optionnal argument `output_l`
 */
#define _hw_is_output_l_output_l	, 1

/*								// COM	  PWM
 */
#define _hw_cmp8b_voutputl_disconnected	, 0	// 0,2,3  0,1
#define _hw_cmp8b_voutputl_clear_at_bottom_set_on_match	, 1	// 1	  1

#define _hwa_cfcmp8b_koutputl_1(o,kw,v,...)				\
  HW_Y(_hwa_cfcmp8b_voutputl,_hw_cmp8b_voutputl_##v)(o,v,__VA_ARGS__)

#define _hwa_cfcmp8b_voutputl_0(o,v,...)				\
  HW_E_AVL(output_l, v, disconnected | clear_at_bottom_set_on_match)

#define _hwa_cfcmp8b_voutputl_1(o,v,...)			\
  hwa->o.config.outputl = HW_A1(_hw_cmp8b_voutputl_##v);	\
  _hwa_cfcmp8b_koutputl_0(o,__VA_ARGS__)

#define _hwa_cfcmp8b_koutputl_0(o,...)		\
  HW_EOL(__VA_ARGS__)


/**
 * @page atmelavr_cmp8b
 * @section atmelavr_cmp8b_cr Compare value
 *
 * The compare value is set using the hw_write() or hwa_write() instruction:
 *
 * @code
 * hw_write( compare0, value );
 * @endcode
 */
#define _hw_mtd_hw_write__cmp8b	, _hw_wrcmp8b
#define _hw_wrcmp8b(o,i,a,v,...)		HW_TX(_hw_write_reg(o,reg,v), __VA_ARGS__)

/**
 * @page atmelavr_cmp8b
 *
 * @code
 * hwa_write( compare0, value );
 * @endcode
 */
#define _hw_mtd_hwa_write__cmp8b	, _hwa_wrcmp8b
#define _hwa_wrcmp8b(o,i,a,v,...)	HW_TX(_hwa_write_reg(o,reg,v), __VA_ARGS__)


/**
 * @page atmelavr_cmp8b
 *
 * The compare value is read using the`read`instruction:
 *
 * @code
 * uint16_t ocr = hw( read, compare0 );
 * @endcode
 */
#define _hw_mtd_hw_read__cmp8b		, _hw_read_cmp8b
#define _hw_read_cmp8b(o,i,a,...)	HW_TX(_hw_read_reg(o,reg),__VA_ARGS__)


/**
 * @page atmelavr_cmp8b
 * @section atmelavr_cmp8b_st Status
 *
 * The compare event flag can be accessed through interrupt-related
 * instructions:
 *
 * @code
 * if ( hw( read, HW_IRQFLAG( compare0 ) ) ) {	// Read compare IRQ flag
 *   hw( clear, HW_IRQFLAG( compare0 ) );		// Clear compare IRQ flag
 *   hw( turn, HW_IRQ( compare0, off ) );		// Disable compare IRQs
 * }
 * @endcode
 */


/*******************************************************************************
 *									       *
 *	Context management						       *
 *									       *
 *******************************************************************************/

#define _hwa_setup__cmp8b(o,i,a)			\
  _hwa_setup_reg( o, reg );			\
  hwa->o.config.outputh = 0xFF ;			\
  hwa->o.config.outputl = 0xFF ;


#define _hwa_init__cmp8b(o,i,a)			_hwa_init_reg( o, reg, 0x00 );

#define _hwa_solve_cmp8b(o)						\
  if ( hwa->o.config.outputh != 0xFF || hwa->o.config.outputl != 0xFF ) {	\
  if ( hwa->o.config.outputl == HW_A1(_hw_cmp8b_voutputl_clear_at_bottom_set_on_match) ) { \
    if ( hwa->o.config.outputh != HW_A1(_hw_cmp8b_voutputh_set_at_bottom_clear_on_match) ) \
      HWA_ERR("`output_h` must be `set_at_bottom_clear_on_match` "	\
	      "when `output_l` is `clear_at_bottom_set_on_match`.");	\
    _hwa_write_reg(o,pwm,1);						\
    _hwa_write_reg(o,mode,1);						\
  }									\
  else if ( hwa->o.config.outputh == HW_A1(_hw_cmp8b_voutputh_disconnected) ) { \
    _hwa_write_reg(o,pwm,0);					\
    _hwa_write_reg(o,mode,0);						\
  }									\
  else if ( hwa->o.config.outputh == HW_A1(_hw_cmp8b_voutputh_toggle_on_match) ) { \
    _hwa_write_reg(o,pwm,0);					\
    _hwa_write_reg(o,mode,1);						\
  }									\
  else if ( hwa->o.config.outputh == HW_A1(_hw_cmp8b_voutputh_clear_on_match) ) { \
    _hwa_write_reg(o,pwm,0);					\
    _hwa_write_reg(o,mode,2);						\
  }									\
  else if ( hwa->o.config.outputh == HW_A1(_hw_cmp8b_voutputh_set_on_match) ) { \
    _hwa_write_reg(o,pwm,0);					\
    _hwa_write_reg(o,mode,3);						\
  }									\
  else if ( hwa->o.config.outputh == HW_A1(_hw_cmp8b_voutputh_set_at_bottom_clear_on_match) ) { \
    _hwa_write_reg(o,pwm,1);						\
    _hwa_write_reg(o,mode,2);						\
  }									\
  else if ( hwa->o.config.outputh == HW_A1(_hw_cmp8b_voutputh_clear_at_bottom_set_on_match) ) { \
    _hwa_write_reg(o,pwm,1);						\
    _hwa_write_reg(o,mode,3);						\
  }									\
  if ( hwa->o.config.outputh != 0xFF && hwa->o.config.outputh != 0 )	\
    _hwa( configure, _HW_REL(o,pin_h), direction, output );		\
  if ( hwa->o.config.outputl != 0xFF && hwa->o.config.outputl != 0 )	\
    _hwa( configure, _HW_REL(o,pin_l), direction, output );			\
  }


#define _hwa_commit__cmp8b(o,i,a)		_hwa_commit_reg(o,reg);


/**
 * @page atmelavr_cmp8b
 * <br>
 */
