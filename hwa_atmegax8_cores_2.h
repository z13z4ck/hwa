
/* This file is part of the HWA project.
 * Copyright (c) Christophe Duparquet <duparq at free dot fr>
 * All rights reserved. Read LICENSE.TXT for details.
 */


HW_INLINE void _hwa_begin_cores ( hwa_t *hwa )
{
  hwa->core0.entering_sleep = 0xFF ;
  hwa->core0.sleep_mode = 0xFF ;

  _hwa_begin_reg( hw_core0, smcr  );
  /* _hwa_begin_reg( hw_core0, gimsk  ); */
  /* _hwa_begin_reg( hw_core0, gifr   ); */
  /* _hwa_begin_reg( hw_core0, mcucr  ); */
  /* _hwa_begin_reg( hw_core0, mcusr  ); */
  /* _hwa_begin_reg( hw_core0, osccal ); */
  /* _hwa_begin_reg( hw_core0, gpior2 ); */
  /* _hwa_begin_reg( hw_core0, gpior1 ); */
  /* _hwa_begin_reg( hw_core0, gpior0 ); */
  /* _hwa_begin_reg( hw_core0, prr    ); */
}


HW_INLINE void _hwa_init_cores ( hwa_t *hwa )
{
  _hwa_set_r8( &hwa->core0.smcr,  0x00 );
  /* _hwa_set_r8( &hwa->core0.gimsk,  0x00 ); */
  /* _hwa_set_r8( &hwa->core0.gifr,   0x00 ); */
  /* _hwa_set_r8( &hwa->core0.mcucr,  0x00 ); */
  /* _hwa_set_r8( &hwa->core0.mcusr,  0x00 ); */
  /* _hwa_set_r8( &hwa->core0.osccal, 0x00 ); */
  /* _hwa_set_r8( &hwa->core0.gpior2, 0x00 ); */
  /* _hwa_set_r8( &hwa->core0.gpior1, 0x00 ); */
  /* _hwa_set_r8( &hwa->core0.gpior0, 0x00 ); */
  /* _hwa_set_r8( &hwa->core0.prr,    0x00 ); */
}


HW_INLINE void _hwa_solve_cores( hwa_t *hwa )
{
  if ( hwa->core0.entering_sleep != 0xFF )
    _hwa_write_reg( hw_core0, se, hwa->core0.entering_sleep );
  if ( hwa->core0.sleep_mode != 0xFF )
    _hwa_write_reg( hw_core0, sm, hwa->core0.sleep_mode );
}


HW_INLINE void _hwa_commit_cores ( hwa_t *hwa )
{
  _hwa_commit_reg( hw_core0, smcr,  hwa->commit );
  /* _hwa_commit_reg( hw_core0, gimsk,  hwa->commit ); */
  /* _hwa_commit_reg( hw_core0, gifr,   hwa->commit ); */
  /* _hwa_commit_reg( hw_core0, mcucr,  hwa->commit ); */
  /* _hwa_commit_reg( hw_core0, mcusr,  hwa->commit ); */
  /* _hwa_commit_reg( hw_core0, osccal, hwa->commit ); */
  /* _hwa_commit_reg( hw_core0, gpior2, hwa->commit ); */
  /* _hwa_commit_reg( hw_core0, gpior1, hwa->commit ); */
  /* _hwa_commit_reg( hw_core0, gpior0, hwa->commit ); */
  /* _hwa_commit_reg( hw_core0, prr,    hwa->commit ); */
}


/*	Configure the MCU sleep mode
 */
#define hw_is_entering_sleep_entering_sleep	, 1
#define hw_is_sleep_mode_sleep_mode		, 1

#define hw_core_sm_idle				, 0
#define hw_core_sm_adc_noise_reduction		, 1
#define hw_core_sm_power_down			, 2
#define hw_core_sm_power_save			, 3
#define hw_core_sm_standby			, 6
#define hw_core_sm_external_standby		, 7


#define hw_def_hwa_config_core		, _hwa_cfcore

/*  c,n,i,a is hw_core0, no need to propagate it
 */
#define _hwa_cfcore(c,n,i,a,...)					\
  do {									\
    HW_G2(_hwa_cfcore_xenteringsleep, HW_IS(entering_sleep,__VA_ARGS__))(__VA_ARGS__,);	\
  } while(0)

#define _hwa_cfcore_xenteringsleep_1(kw,...)					\
  HW_G2(_hwa_cfcore_venteringsleep, HW_IS(,hw_state_##__VA_ARGS__))(__VA_ARGS__)

#define _hwa_cfcore_venteringsleep_1(v,...)					\
  hwa->core0.entering_sleep = HW_A1(hw_state_##v);\
  HW_G2(_hwa_cfcore_xsleepmode, HW_IS(sleep_mode,__VA_ARGS__))(__VA_ARGS__)

#define _hwa_cfcore_venteringsleep_0(...)					\
  HW_ERR("expected `enabled` or `disabled`, got `" HW_QUOTE(__VA_ARGS__) "` instead.")

#define _hwa_cfcore_xenteringsleep_0(...)					\
  HW_G2(_hwa_cfcore_xsleepmode, HW_IS(sleep_mode,__VA_ARGS__))(__VA_ARGS__)

#define _hwa_cfcore_xsleepmode_1(kw,...)				\
  HW_G2(_hwa_cfcore_vsleepmode, HW_IS(,hw_core_sm_##__VA_ARGS__))(__VA_ARGS__)

#define _hwa_cfcore_vsleepmode_1(v,...)			\
  hwa->core0.sleep_mode = HW_A1(hw_core_sm_##v);	\
  HW_TX(,__VA_ARGS__)

#define _hwa_cfcore_vsleepmode_0(...)					\
  HW_ERR("`sleep_mode` can be `idle`, `adc_noise_reduction`, `power_down`," \
	 "`power_save`, `standby`, or `external_standby`, but not `" \
	 HW_QUOTE(__VA_ARGS__) "`.")

#define _hwa_cfcore_xsleepmode_0(...)					\
  HW_TX(,__VA_ARGS__)
//  HW_G2(_hwa_cfcore_xeol, HW_IS(,__VA_ARGS__))(__VA_ARGS__)

/* #define _hwa_cfcore_xeol_0(...)					\ */
/*   HW_ERR("expected `entering_sleep` or `sleep_mode`, got `" HW_QUOTE(__VA_ARGS__) "` instead.") */

/* #define _hwa_cfcore_xeol_1(...) */


/*	Power management
 */
#define hw_power_counter1		prtim1
#define hw_power_counter0		prtim0
#define hw_power_usi0			prusi
#define hw_power_adc0			pradc

#define hw_def_hw_turn_c8a		, _hw_turn_power
#define hw_def_hw_turn_c16a		, _hw_turn_power
#define hw_def_hw_turn_usia		, _hw_turn_power
#define hw_def_hw_turn_ad10a		, _hw_turn_power

#define _hw_turn_power(c,n,i,a,vstate)\
  HW_G2(_hw_turn_power, HW_IS(,hw_state_##vstate))(c,n,i,a,vstate)
#define _hw_turn_power_0(c,n,i,a, vstate)			\
  HW_ERR("expected `on` or `off`, got `" #vstate "` instead.")
#define _hw_turn_power_1(c,n,i,a, vstate)	\
  _hw_write_reg(hw_core0, hw_power_##n, (HW_A1(hw_state_##vstate)==0))


#define hw_def_hwa_turn_c8a	, _hwa_turn_power
#define hw_def_hwa_turn_c16a	, _hwa_turn_power
#define hw_def_hwa_turn_usia	, _hwa_turn_power
#define hw_def_hwa_turn_ad10a	, _hwa_turn_power

#define _hwa_turn_power(c,n,i,a,vstate)\
  HW_G2(_hwa_turn_power, HW_IS(,hw_state_##vstate))(c,n,i,a,vstate)
#define _hwa_turn_power_0(c,n,i,a, vstate)			\
  HW_ERR("expected `on` or `off`, got `" #vstate "` instead.")
#define _hwa_turn_power_1(c,n,i,a, vstate)	\
  _hwa_write_reg(hw_core0, hw_power_##n, (HW_A1(hw_state_##vstate)==0))
