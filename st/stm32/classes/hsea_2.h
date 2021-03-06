
/* This file is part of the HWA project.
 * Copyright (c) 2017 Christophe Duparquet.
 * All rights reserved. Read LICENSE.TXT for details.
 */

/**
 * @file
 * @brief HSE
 */

 /* * You should define the HW_HSEHZ symbol as the crystal frequency if you use the */
 /* * HSE oscillator. */

/**
 * @page stm32_hsea Class _hsea: HSE oscillator
 *
 * `power`:
 * @code
 * hw | hwa ( power, hse,   on
 *                        | off );
 * @endcode
 */

/* Implemented in stm32/hwa_2.h */

/**
 * @page stm32_hsea
 * <br>
 * `stat`:
 * @code
 * while ( ! hw(stat,hse).ready ) {}	// Wait for the HSE clock to be stable.
 * @endcode
 * <br>
 */
typedef struct {
    unsigned int _0_16  : 17 ;
    unsigned int ready  :  1 ;
    unsigned int _18_31 : 14 ;
} _hw_sthse_t ;

#define _hw_mtd_hw_stat_t_hse		, _hw_stthse
#define _hw_stthse(o,i,a,...)		HW_TX( _hw_sthse_t, __VA_ARGS__)

#define _hw_mtd_hw_stat_hse		, _hw_sthse
#define _hw_sthse(i,a,g,...)		HW_Y(_hw_sthse1,g)(g)
#define _hw_sthse1_0(g)			HW_E_G(g)
#define _hw_sthse1_1(g)			(*(volatile _hw_sthse_t*)_HW_A(_HW_R(rcc,cr)))
