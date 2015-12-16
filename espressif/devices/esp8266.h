
/* This file is part of the HWA project.
 * Copyright (c) 2012,2015 Christophe Duparquet.
 * All rights reserved. Read LICENSE.TXT for details.
 */

/**
 * @file
 * @brief Espressif's ESP8266
 */

#include "../hwa_1.h"

/**
 * @page esp8266 ESP8266
 */
#define hw_syshz				80000000
#define hw_apbhz				hw_syshz

#define hw_uptime_us				(*(volatile uint32_t*)0x3ff20c00)
#define hw_uptime				(hw_uptime_us / 1e6)


/*******************************************************************************
 *									       *
 *	Peripherals							       *
 *									       *
 *******************************************************************************/

/**
 * @page esp8266
 * @section esp8266_peripherals Peripherals
 *
 * @note All the peripherals are not completely implemented yet.
 *
 * Object name		  | Class		  | Comments
 * :----------------------|-----------------------|:--------------------------------------
 * `hw_port0`	 | @ref espressif_p16a "_p16a"	  | General purpose I/O port
 * `hw_timer1`	 | @ref espressif_tm23a "_tm23a"  | 23-bit timer
 * `hw_uart0`	 | @ref esp8266_uarta "_uarta"    | UART
 */


/*******************************************************************************
 *									       *
 *	Interrupts							       *
 *									       *
 *******************************************************************************/
/**
 * @ingroup atmegax8_interrupts
 * @brief Definition of the interrupts
 */
/*  Interrupt definitions
 *
 *  'vector' is used to store the type of interrupt
 *
 *					class, vector, object, ie, if
 */
#define _hw_irq_hw_timer1_nmi		_irq, nmi, hw_timer1, ie,
#define _hw_irq_hw_timer1_irq		_irq,   9, hw_timer1, ie,

#define _hw_handleirq_hw_timer1_nmi(fn)		NmiTimSetFunc(fn)
#define _hw_handleirq_hw_timer1_9(fn)		ets_isr_attach(9, fn, 0)

extern void NmiTimSetFunc(void (*isr)(void));
extern void ets_isr_unmask(unsigned intr);


/*******************************************************************************
 *									       *
 *	Shared registers						       *
 *									       *
 *	This is an object used to gather hardware registers shared	       *
 *	by several objects						       *
 *									       *
 *******************************************************************************/

/*	Object				class, id, address
 */
#define _hw_class__shared
#define _hw_shared			_shared, 0x3FF00000, 0x3FF00000

/*	Object hardware registers	class, address, write mask, w1tc mask
 */
#define _hw_shared__edgeie		_r32, 0x04, 0xFFFFFFFF, 0
#define _hw_shared__swap		_r32, 0x28, 0xFFFFFFFF, 0

#if !defined __ASSEMBLER__
/*
 *  HWA context
 */
typedef struct {

  /*  Hardware registers
   */
  hwa_r32_t	_edgeie ;
  hwa_r32_t	_swap ;
} hwa_shared_t ;


#define _hwa_setup__shared(o,i,a)	\
  _hwa_setup_reg( o, _edgeie );	\
  _hwa_setup_reg( o, _swap )

#define _hwa_init__shared(o,i,a)	\
  /* _edgeie is UNKNOWN and 0 causes errors */	\
  _hwa_init_reg( o, _swap, 0 )

#define _hwa_commit__shared(o,i,a)	\
  _hwa_commit_reg( o, _edgeie );	\
  _hwa_commit_reg( o, _swap )

#endif


/*******************************************************************************
 *									       *
 *	GPIO ports							       *
 *									       *
 *******************************************************************************/

/*	Objects				class, id, address
 */
#include "../classes/p16a_1.h"
#include "../classes/io1a_1.h"

/*	Object				class, id, address
 */
#define _hw_port0			_p16a, 0x60000300, 0x60000300


/*	Class hardware registers	class, address, write mask, flags mask
 *
 *	See:
 *	  * ESP8266_GPIO_Register_141105.ods
 *	  * 2C-ESP8266__SDK__Programming Guide__EN_v1.4
 *	  * 0D-ESP8266__Pin_List__Release_15-11-2014.ods
 *	  * ESP8266_RTOS_SDK/blob/master/examples/driver_lib/driver/gpio.c
 */
#define _hw__p16a__out			_r32, 0x00, 0xFFFFFFFF, 0
#define _hw__p16a__outw1ts		_r32, 0x04, 0x0000FFFF, 0
#define _hw__p16a__outw1tc		_r32, 0x08, 0x0000FFFF, 0
#define _hw__p16a__enb			_r32, 0x0C, 0x003FFFFF, 0
#define _hw__p16a__enbw1ts		_r32, 0x10, 0x0000FFFF, 0
#define _hw__p16a__enbw1tc		_r32, 0x14, 0x0000FFFF, 0
#define _hw__p16a__in			_r32, 0x18, 0x00000000, 0
#define _hw__p16a__ie			_r32, 0x1C, 0x0000FFFF, 0
#define _hw__p16a__iew1ts		_r32, 0x20, 0x0000FFFF, 0
#define _hw__p16a__iew1tc		_r32, 0x24, 0x0000FFFF, 0

/*	Class logical registers		class, reg, bn, bp
 */
#define _hw__p16a_btsel			_cb1, _out,     16, 16
#define _hw__p16a_out			_cb1, _out,     16,  0
#define _hw__p16a_outw1ts		_cb1, _outw1ts, 16,  0
#define _hw__p16a_outw1tc		_cb1, _outw1tc, 16,  0

#define _hw__p16a_sdiosel		_cb1, _enb,      6, 16
#define _hw__p16a_enb			_cb1, _enb,     16,  0
#define _hw__p16a_enbw1ts		_cb1, _enbw1ts, 16,  0
#define _hw__p16a_enbw1tc		_cb1, _enbw1tc, 16,  0

#define _hw__p16a_strapping		_cb1, _in,      16, 16
#define _hw__p16a_in			_cb1, _in,      16,  0

#define _hw__p16a_ie			_cb1, _ie,      16, 16
#define _hw__p16a_iew1ts		_cb1, _iew1ts,  16, 16
#define _hw__p16a_iew1tc		_cb1, _iew1tc,  16, 16


/*******************************************************************************
 *									       *
 *	I/O pins							       *
 *									       *
 *******************************************************************************/

/**
 * @page esp8266
 * @section esp8266_pins Pins
 *
 * Each pin can be connected to different signals with the `hw_config()` or
 * `hwa_config()` instruction.
 *
 * Pin    | HWA name        | Class                       | Functions
 * -------|-----------------|-----------------------------|-------------
 * GPIO0  | `hw_pin_gpio0`  | @ref esp8266_io1a "_io1a" | `gpio` / `spi_cs2` / `clk_out`
 * GPIO1  | `hw_pin_gpio1`  | @ref esp8266_io1a "_io1a" | `gpio` / `uart0_txd` / `spi_cs1` / `clk_rtc`
 * GPIO2  | `hw_pin_gpio2`  | @ref esp8266_io1a "_io1a" | `gpio` / `i2so_ws` / `uart1_txd` / `uart0_txd`
 * GPIO3  | `hw_pin_gpio3`  | @ref esp8266_io1a "_io1a" | `gpio` / `uart0_rxd` / `i2so_data` / `fn_clk_xtal`
 * GPIO4  | `hw_pin_gpio4`  | @ref esp8266_io1a "_io1a" | `gpio` / `clk_xtal`
 * GPIO5  | `hw_pin_gpio5`  | @ref esp8266_io1a "_io1a" | `gpio` / `clk_rtc`
 * GPIO6  | `hw_pin_gpio6`  | @ref esp8266_io1a "_io1a" | `gpio` / `sd_clk` / `spi_clk` / `uart1_cts`
 * GPIO7  | `hw_pin_gpio7`  | @ref esp8266_io1a "_io1a" | `gpio` / `sd_data0` / `spi_q` / `uart1_txd`
 * GPIO8  | `hw_pin_gpio8`  | @ref esp8266_io1a "_io1a" | `gpio` / `sd_data1` / `spi_d` / `uart1_rxd`
 * GPIO9  | `hw_pin_gpio9`  | @ref esp8266_io1a "_io1a" | `gpio` / `sd_data2` / `spi_hd` / `hspi_hd`
 * GPIO10 | `hw_pin_gpio10` | @ref esp8266_io1a "_io1a" | `gpio` / `sd_data3` / `spi_wp` / `hspi_wp`
 * GPIO11 | `hw_pin_gpio11` | @ref esp8266_io1a "_io1a" | `gpio` / `sd_cmd` / `spi_cs0` / `uart1_rts`
 * GPIO12 | `hw_pin_gpio12` | @ref esp8266_io1a "_io1a" | `gpio` / `mtdi` / `i2si_data` / `hspiq_miso` / `uart0_dtr`
 * GPIO13 | `hw_pin_gpio13` | @ref esp8266_io1a "_io1a" | `gpio` / `mtck` / `i2s_bck` / `hspi_mosi` / `uart0_cts`
 * GPIO14 | `hw_pin_gpio14` | @ref esp8266_io1a "_io1a" | `gpio` / `mtms` / `i2si_ws` / `hspi_clk` / `uart0_dsr`
 * GPIO15 | `hw_pin_gpio15` | @ref esp8266_io1a "_io1a" | `gpio` / `mtdo` / `i2so_bck` / `hspi_cs` / `uart0_rts`
 * GPIO16 | `hw_pin_gpio16` | @ref esp8266_io1a "_io1a" | `gpio` / `xpd_dcdc` / `ext_wakeup` / `deep_sleep` / `bt_xtal_en`
 */


/*	Pins				class, id, port, bn, bp
 */
#define _hw_pin_gpio0			_io1a, 300, hw_port0, 1, 0
#define _hw_pin_gpio1			_io1a, 301, hw_port0, 1, 1
#define _hw_pin_gpio2			_io1a, 302, hw_port0, 1, 2
#define _hw_pin_gpio3			_io1a, 303, hw_port0, 1, 3
#define _hw_pin_gpio4			_io1a, 304, hw_port0, 1, 4
#define _hw_pin_gpio5			_io1a, 305, hw_port0, 1, 5
#define _hw_pin_gpio6			_io1a, 306, hw_port0, 1, 6
#define _hw_pin_gpio7			_io1a, 307, hw_port0, 1, 7
#define _hw_pin_gpio8			_io1a, 308, hw_port0, 1, 8
#define _hw_pin_gpio9			_io1a, 309, hw_port0, 1, 9
#define _hw_pin_gpio10			_io1a, 310, hw_port0, 1, 10
#define _hw_pin_gpio11			_io1a, 311, hw_port0, 1, 11
#define _hw_pin_gpio12			_io1a, 312, hw_port0, 1, 12
#define _hw_pin_gpio13			_io1a, 313, hw_port0, 1, 13
#define _hw_pin_gpio14			_io1a, 314, hw_port0, 1, 14
#define _hw_pin_gpio15			_io1a, 315, hw_port0, 1, 15


/*	Pin GPIO16 (RTC_GPIO0 / xpd_dcdc) is of a different class
 *
 *	See esp_iot_sdk_v1.4.0/examples/driver_lib/driver/gpio16.c
 */
#define _hw_pin_gpio16			_io1b, 316, hw_port1, 1, 0


/*  Association of pin numbers and pin names
 */
#define hw_pin_8			hw_pin_gpio16
#define hw_pin_9			hw_pin_gpio14
#define hw_pin_10			hw_pin_gpio12
#define hw_pin_12			hw_pin_gpio13
#define hw_pin_13			hw_pin_gpio15
#define hw_pin_14			hw_pin_gpio2
#define hw_pin_15			hw_pin_gpio0
#define hw_pin_16			hw_pin_gpio4
#define hw_pin_18			hw_pin_gpio9
#define hw_pin_19			hw_pin_gpio10
#define hw_pin_20			hw_pin_gpio11
#define hw_pin_21			hw_pin_gpio6
#define hw_pin_22			hw_pin_gpio7
#define hw_pin_23			hw_pin_gpio8
#define hw_pin_24			hw_pin_gpio5
#define hw_pin_25			hw_pin_gpio3
#define hw_pin_26			hw_pin_gpio1


/*  Pin designed by a number stored in a variable
 */
#define _hw_class__ionum

#define _hw_is_var_var			, 1

#define hw_pin_gpio(x)			HW_G2(hw_pin_gpio_var,HW_IS(var,x))(x)

#define hw_pin_gpio_var_0(x)		hw_pin_gpio##x

#define hw_pin_gpio_var_1(x)		_ionum, 0, /* hw_port */, x


/*  Pin configuration objects
 *
 *	Each pin has a _pcfa object that handles its configuration.
 */
#include "../classes/pcfa_1.h"

#define _hw_pin_gpio12_cf		_pcfa, 0x60000804, 0x60000804
#define _hw_pin_gpio13_cf		_pcfa, 0x60000808, 0x60000808
#define _hw_pin_gpio14_cf		_pcfa, 0x6000080C, 0x6000080C
#define _hw_pin_gpio15_cf		_pcfa, 0x60000810, 0x60000810
#define _hw_pin_gpio3_cf		_pcfa, 0x60000814, 0x60000814
#define _hw_pin_gpio1_cf		_pcfa, 0x60000818, 0x60000818
#define _hw_pin_gpio6_cf		_pcfa, 0x6000081c, 0x6000081c
#define _hw_pin_gpio7_cf		_pcfa, 0x60000820, 0x60000820
#define _hw_pin_gpio8_cf		_pcfa, 0x60000824, 0x60000824
#define _hw_pin_gpio9_cf		_pcfa, 0x60000828, 0x60000828
#define _hw_pin_gpio10_cf		_pcfa, 0x6000082c, 0x6000082c
#define _hw_pin_gpio11_cf		_pcfa, 0x60000830, 0x60000830
#define _hw_pin_gpio0_cf		_pcfa, 0x60000834, 0x60000834
#define _hw_pin_gpio2_cf		_pcfa, 0x60000838, 0x60000838
#define _hw_pin_gpio4_cf		_pcfa, 0x6000083C, 0x6000083C
#define _hw_pin_gpio5_cf		_pcfa, 0x60000840, 0x60000840


/*  Pin functions
 *
 *	Each pin function is declared with the symbol _pinname_fn_function.
 *
 *	The value is the value of bits 8,5,4 (fn) in the I/O cf register.
 *
 *	The symbol _pinname_fns is used to build the error message when an
 *	unavailable function is chosen with hw_config().
 */
#define _hw_pin_gpio0_fn_gpio		, 0
#define _hw_pin_gpio0_fn_spi_cs2	, 1
#define _hw_pin_gpio0_fn_clk_out	, 4 /* RESET */
#define _hw_pin_gpio0_fns		"`gpio`, `spi_cs2`, or `clk_out`"

#define _hw_pin_gpio1_fn_uart0_txd	, 0, _hwa_write_reg( hw_uart0, swap, 0 );
#define _hw_pin_gpio1_fn_spi_cs1	, 1
#define _hw_pin_gpio1_fn_gpio		, 3
#define _hw_pin_gpio1_fn_clk_rtc	, 4
#define _hw_pin_gpio1_fns		"`gpio`, `uart0_txd`, `spi_cs1`, or `clk_rtc`"

#define _hw_pin_gpio2_fn_gpio		, 0
#define _hw_pin_gpio2_fn_i2so_ws	, 1
#define _hw_pin_gpio2_fn_uart1_txd	, 2
#define _hw_pin_gpio2_fn_uart0_txd	, 4, _hwa_write_reg( hw_uart0, swap, 0 );
#define _hw_pin_gpio2_fn_hw_uart0_txd	, 4
#define _hw_pin_gpio2_fns		"`gpio`, `i2so_ws`, `uart1_txd`, or `uart0_txd`"

#define _hw_pin_gpio3_fn_uart0_rxd	, 0, _hwa_write_reg( hw_uart0, swap, 0 );
#define _hw_pin_gpio3_fn_i2so_data	, 1
#define _hw_pin_gpio3_fn_gpio		, 3
#define _hw_pin_gpio3_fn_clk_xtal	, 4
#define _hw_pin_gpio3_fns		"`gpio`, `uart0_rxd`, `i2so_data`, or `fn_clk_xtal`"

#define _hw_pin_gpio4_fn_gpio		, 0
#define _hw_pin_gpio4_fn_clk_xtal	, 1
#define _hw_pin_gpio4_fns		"`gpio` or `clk_xtal`"

#define _hw_pin_gpio5_fn_gpio		, 0
#define _hw_pin_gpio5_fn_clk_rtc	, 1
#define _hw_pin_gpio5_fns		"`gpio`, or `clk_rtc`"

#define _hw_pin_gpio6_fn_sd_clk		, 0
#define _hw_pin_gpio6_fn_spi_clk	, 1
#define _hw_pin_gpio6_fn_gpio		, 3
#define _hw_pin_gpio6_fn_uart1_cts	, 4
#define _hw_pin_gpio6_fns		"`gpio`, `sd_clk`, `spi_clk`, or `uart1_cts`"

#define _hw_pin_gpio7_fn_sd_data0	, 0
#define _hw_pin_gpio7_fn_spi_q		, 1
#define _hw_pin_gpio7_fn_gpio		, 3
#define _hw_pin_gpio7_fn_uart1_txd	, 4
#define _hw_pin_gpio7_fns		"`gpio`, `sd_data0`, `spi_q`, or `uart1_txd`"

#define _hw_pin_gpio8_fn_sd_data1	, 0
#define _hw_pin_gpio8_fn_spi_d		, 1
#define _hw_pin_gpio8_fn_gpio		, 3
#define _hw_pin_gpio8_fn_u1_rxd		, 4
#define _hw_pin_gpio8_fns		"`gpio`, `sd_data1`, `spi_d`, or `u1_rxd`"

#define _hw_pin_gpio9_fn_sd_data2	, 0
#define _hw_pin_gpio9_fn_spi_hd		, 1
#define _hw_pin_gpio9_fn_gpio		, 3
#define _hw_pin_gpio9_fn_hspi_hd	, 4
#define _hw_pin_gpio9_fns		"`gpio`, `sd_data2`, `spi_hd`, or `hspi_hd`"

#define _hw_pin_gpio10_fn_sd_data3	, 0
#define _hw_pin_gpio10_fn_spi_wp	, 1
#define _hw_pin_gpio10_fn_gpio		, 3
#define _hw_pin_gpio10_fn_hspi_wp	, 4
#define _hw_pin_gpio10_fns		"`gpio`, `sd_data3`, `spi_wp`, or `hspi_wp`"

#define _hw_pin_gpio11_fn_sd_cmd	, 0
#define _hw_pin_gpio11_fn_spi_cs0	, 1
#define _hw_pin_gpio11_fn_gpio		, 3
#define _hw_pin_gpio11_fn_uart1_rts	, 4
#define _hw_pin_gpio11_fns		"`gpio`, `sd_cmd`, `spi_cs0`, or `uart1_rts`"

#define _hw_pin_gpio12_fn_mtdi		, 0
#define _hw_pin_gpio12_fn_i2si_data	, 1
#define _hw_pin_gpio12_fn_hspi_miso	, 2
#define _hw_pin_gpio12_fn_gpio		, 3
#define _hw_pin_gpio12_fn_uart0_dtr	, 4
#define _hw_pin_gpio12_fns		"`gpio`, `mtdi`, `i2si_data`, `hspi_miso`, or `uart0_dtr`"

#define _hw_pin_gpio13_fn_mtck		, 0
#define _hw_pin_gpio13_fn_i2s_bck	, 1
#define _hw_pin_gpio13_fn_hspi_mosi	, 2
#define _hw_pin_gpio13_fn_gpio		, 3
#define _hw_pin_gpio13_fn_uart0_cts	, 4
#define _hw_pin_gpio13_fn_uart0_rxd	, 4, _hwa_write_reg( hw_uart0, swap, 1 );
#define _hw_pin_gpio13_fns		"`gpio`, `mtck`, `i2s_bck`, `hspi_mosi`, or `uart0_cts`"

#define _hw_pin_gpio14_fn_mtms		, 0
#define _hw_pin_gpio14_fn_i2si_ws	, 1
#define _hw_pin_gpio14_fn_hspi_clk	, 2
#define _hw_pin_gpio14_fn_gpio		, 3
#define _hw_pin_gpio14_fn_uart0_dsr	, 4
#define _hw_pin_gpio14_fns		"`gpio`, `mtms`, `i2si_ws`, `hspi_clk`, or `uart0_dsr`"

#define _hw_pin_gpio15_fn_mtdo		, 0
#define _hw_pin_gpio15_fn_i2so_bck	, 1
#define _hw_pin_gpio15_fn_hspi_cs	, 2
#define _hw_pin_gpio15_fn_gpio		, 3
#define _hw_pin_gpio15_fn_uart0_rts	, 4
#define _hw_pin_gpio15_fn_uart0_txd	, 4, _hwa_write_reg( hw_uart0, swap, 1 );
#define _hw_pin_gpio15_fns		"`gpio`, `mtdo`, `i2so_bck`, `hspi_cs`, `uart0_rts`, or `uart0_txd`"

#define _hw_pin_gpio16_fn_xpd_dcdc	, 0
#define _hw_pin_gpio16_fn_rtc_gpio0	, 1
#define _hw_pin_gpio16_fn_gpio		, 1
#define _hw_pin_gpio16_fn_ext_wakeup	, 2
#define _hw_pin_gpio16_fn_deep_sleep	, 3
#define _hw_pin_gpio16_fn_bt_xtal_en	, 4
#define _hw_pin_gpio16_fns		"`gpio`, `xpd_dcdc`, `ext_wakeup`, `deep_sleep`, or `bt_xtal_en`"


/*******************************************************************************
 *									       *
 *	Hardware timer 1						       *
 *									       *
 *	23-bit timer (down counter) clocked by the APB			       *
 *									       *
 *	See: 8M-ESP8266__Interface__Timer_Registers__Release_18-11-2014	       *
 *									       *
 *******************************************************************************/

#include "../classes/tm23a_1.h"

/*	Object				class, id, address
 */
#define _hw_timer1			_tm23a, 0x60000600, 0x60000600

/*	Class hardware registers	class, address, write mask, w1tc mask
 */
#define _hw__tm23a__load		_r32, 0x00, 0x7FFFFF, 0
#define _hw__tm23a__count		_r32, 0x04, 0x0,      0
#define _hw__tm23a__ctrl		_r32, 0x08, 0x0CD,    0
#define _hw__tm23a__flags		_r32, 0x0C, 0x1,      0x1

/*	Class logical registers
 */
#define _hw__tm23a_int			_cb1, _ctrl,  1, 8	// Interrupt status
#define _hw__tm23a_en			_cb1, _ctrl,  1, 7	// Enable timer
#define _hw__tm23a_arl			_cb1, _ctrl,  1, 6	// Auto-reload after 0
#define _hw__tm23a_psc			_cb1, _ctrl,  2, 2	// Prescaler
#define _hw__tm23a_irqtype		_cb1, _ctrl,  1, 0	// 0:Edge/1:Level interrupt

#define _hw__tm23a_load			_cb1, _load,  23, 0	// Reload value
#define _hw__tm23a_count		_cb1, _count, 23, 0	// Count

#define _hw__tm23a_ifclr		_cb1, _flags,  1, 0	// Write 1 to clear interrupt flag

#define _hw_timer1_ie			_xob1, hw_shared, _edgeie, 1, 1


/*******************************************************************************
 *									       *
 *	Hardware timer 2						       *
 *									       *
 *	32-bit timer (up counter) clocked by the APB			       *
 *									       *
 *	See: 8M-ESP8266__Interface__Timer_Registers__Release_18-11-2014	       *
 *									       *
 *******************************************************************************/

/*	Object				class, id, address
 */
#define _hw_timer2			_tm32a, 0x60000620, 0x60000620

/*	Class hardware registers	class, address, write mask, flags mask
 */
#define _hw__tm32a__load		_r32, 0x00, 0xFFFFFFFF, 0
#define _hw__tm32a__count		_r32, 0x04, 0x00000000, 0
#define _hw__tm32a__ctrl		_r32, 0x08, 0x000000CD, 0
#define _hw__tm32a__flags		_r32, 0x0C, 0x00000001, 1
#define _hw__tm32a__alarm		_r32, 0x10, 0xFFFFFFFF, 0

/*	Class logical registers
 */
#define _hw__tm32a_int			_cb1, _ctrl,  1, 8	// Interrupt status
#define _hw__tm32a_en			_cb1, _ctrl,  1, 7	// Enable timer
#define _hw__tm32a_arl			_cb1, _ctrl,  1, 6	// Auto-reload after 0
#define _hw__tm32a_psc			_cb1, _ctrl,  2, 2	// Prescaler
#define _hw__tm32a_irqtype		_cb1, _ctrl,  1, 0	// 0:Edge/1:Level interrupt

#define _hw__tm32a_load			_cb1, _load,  32, 0	// Reload value
#define _hw__tm32a_count		_cb1, _count, 32, 0	// Count

#define _hw__tm32a_ifclr		_cb1, _flags,  1, 0	// Write 1 to clear interrupt flag

//#define _hw_timer1_ie			_xob1, hw_shared, _edgeie, 1, 1


/*******************************************************************************
 *									       *
 *	UARTS								       *
 *									       *
 *	See:								       *
 *       * 8E-ESP8266__Interface_UART__EN_v0.2.pdf			       *
 *	 * 8F-ESP8266__Interface__UART_Registers_v0.1			       *
 *	 * Arduino/cores/esp8266/HardwareSerial.h			       *
 *	 * Arduino/cores/esp8266/HardwareSerial.cpp			       *
 *	 * Arduino/cores/esp8266/esp8266_peri.h				       *
 *									       *
 *******************************************************************************/

#include "../classes/uarta_1.h"

/*	Objects				class, id, address
 */
#define _hw_uart0			_uarta, 0x60000000, 0x60000000
#define _hw_uart1			_uarta, 0x60000F00, 0x60000F00

/*	Class hardware registers	class, address, write mask, w1tc mask
 */
#define _hw__uarta__fifo		_r32, 0x00, 0x0, 0
#define _hw__uarta__ir			_r32, 0x04, 0x0, 0		// IRQ raw
#define _hw__uarta__if			_r32, 0x08, 0x0, 0		// IRQ stat
#define _hw__uarta__ie			_r32, 0x0C, 0x1FF, 0		// IRQ enable
#define _hw__uarta__ic			_r32, 0x10, 0x1FF, 0x1FF	// IRQ clear
#define _hw__uarta__clkdiv		_r32, 0x14, 0xFFFFF, 0
#define _hw__uarta__autobaud		_r32, 0x18, 0xFF01, 0
#define _hw__uarta__stat		_r32, 0x1C, 0x0, 0
#define _hw__uarta__conf0		_r32, 0x20, 0x01FE61FF, 0
#define _hw__uarta__conf1		_r32, 0x24, 0xFFFF7F7F, 0
#define _hw__uarta__lopulse		_r32, 0x28, 0x0, 0
#define _hw__uarta__hipulse		_r32, 0x2C, 0x0, 0
#define _hw__uarta__pulsnum		_r32, 0x30, 0x0, 0
#define _hw__uarta__date		_r32, 0x78, 0xFFFFFFFF, 0
#define _hw__uarta__id			_r32, 0x7C, 0xFFFFFFFF, 0

/*	Class logical registers
 */
#define _hw__uarta_rxqcnt		_cb1, _fifo, 8, 0	// RX fifo read position

#define _hw__uarta_irrxto		_cb1, _ir, 1, 8		// IRQ RX timeout
#define _hw__uarta_irrxbrk		_cb1, _ir, 1, 7		// IRQ break detected
#define _hw__uarta_ircts		_cb1, _ir, 1, 6		// IRQ CTS changed
#define _hw__uarta_irdsr		_cb1, _ir, 1, 5		// IRQ DSR changed
#define _hw__uarta_irrxof		_cb1, _ir, 1, 4		// IRQ RX overflow
#define _hw__uarta_irrxfe		_cb1, _ir, 1, 3		// IRQ frame error
#define _hw__uarta_irrxpe		_cb1, _ir, 1, 2		// IRQ parity error
#define _hw__uarta_irtxqe		_cb1, _ir, 1, 1		// IRQ TX fifo empty
#define _hw__uarta_irrxqf		_cb1, _ir, 1, 0		// IRQ RX fifo full

#define _hw__uarta_ifrxto		_cb1, _if, 1, 8		// IRQ RX timeout
#define _hw__uarta_ifrxbrk		_cb1, _if, 1, 7		// IRQ break detected
#define _hw__uarta_ifcts		_cb1, _if, 1, 6		// IRQ CTS changed
#define _hw__uarta_ifdsr		_cb1, _if, 1, 5		// IRQ DSR changed
#define _hw__uarta_ifrxof		_cb1, _if, 1, 4		// IRQ RX overflow
#define _hw__uarta_ifrxfe		_cb1, _if, 1, 3		// IRQ frame error
#define _hw__uarta_ifrxpe		_cb1, _if, 1, 2		// IRQ parity error
#define _hw__uarta_iftxqe		_cb1, _if, 1, 1		// IRQ TX fifo empty
#define _hw__uarta_ifrxqf		_cb1, _if, 1, 0		// IRQ RX fifo full

#define _hw__uarta_ierxto		_cb1, _ie, 1, 8		// IRQ RX timeout
#define _hw__uarta_ierxbrk		_cb1, _ie, 1, 7		// IRQ break detected
#define _hw__uarta_iects		_cb1, _ie, 1, 6		// IRQ CTS changed
#define _hw__uarta_iedsr		_cb1, _ie, 1, 5		// IRQ DSR changed
#define _hw__uarta_ierxof		_cb1, _ie, 1, 4		// IRQ RX overflow
#define _hw__uarta_ierxfe		_cb1, _ie, 1, 3		// IRQ frame error
#define _hw__uarta_ierxpe		_cb1, _ie, 1, 2		// IRQ parity error
#define _hw__uarta_ietxqe		_cb1, _ie, 1, 1		// IRQ TX fifo empty
#define _hw__uarta_ierxqf		_cb1, _ie, 1, 0		// IRQ RX fifo full

#define _hw__uarta_icrxto		_cb1, _ic, 1, 8		// IRQ RX timeout
#define _hw__uarta_icrxbrk		_cb1, _ic, 1, 7		// IRQ break detected
#define _hw__uarta_iccts		_cb1, _ic, 1, 6		// IRQ CTS changed
#define _hw__uarta_icdsr		_cb1, _ic, 1, 5		// IRQ DSR changed
#define _hw__uarta_icrxof		_cb1, _ic, 1, 4		// IRQ RX overflow
#define _hw__uarta_icrxfe		_cb1, _ic, 1, 3		// IRQ frame error
#define _hw__uarta_icrxpe		_cb1, _ic, 1, 2		// IRQ parity error
#define _hw__uarta_ictxqe		_cb1, _ic, 1, 1		// IRQ TX fifo empty
#define _hw__uarta_icrxqf		_cb1, _ic, 1, 0		// IRQ RX fifo full

#define _hw__uarta_clkdiv		_cb1, _clkdiv, 20, 0	// Baudrate divider

#define _hw__uarta_stxd			_cb1, _stat, 1, 31	// TX PIN Level
#define _hw__uarta_srtsn		_cb1, _stat, 1, 30	// RTS PIN Level
#define _hw__uarta_sdtrn		_cb1, _stat, 1, 29	// DTR PIN Level
#define _hw__uarta_stxcnt		_cb1, _stat, 8, 16	// TX FIFO COUNT (8bit)
#define _hw__uarta_srxd			_cb1, _stat, 1, 15	// RX PIN Level
#define _hw__uarta_sctsn		_cb1, _stat, 1, 14	// CTS PIN Level
#define _hw__uarta_sdsrn		_cb1, _stat, 1, 13	// DSR PIN Level
#define _hw__uarta_srxcnt		_cb1, _stat, 8,	 0	// RX FIFO COUNT (8bit)

#define _hw__uarta_cdtri		_cb1, _conf0, 1, 24	// Invert DTR
#define _hw__uarta_crtsi		_cb1, _conf0, 1, 23	// Invert RTS
#define _hw__uarta_ctxi			_cb1, _conf0, 1, 22	// Invert TX
#define _hw__uarta_cdsri		_cb1, _conf0, 1, 21	// Invert DSR
#define _hw__uarta_cctsi		_cb1, _conf0, 1, 20	// Invert CTS
#define _hw__uarta_crxi			_cb1, _conf0, 1, 19	// Invert RX
#define _hw__uarta_ctxrst		_cb1, _conf0, 1, 18	// Reset TX FIFO
#define _hw__uarta_crxrst		_cb1, _conf0, 1, 17	// Reset RX FIFO
#define _hw__uarta_ctxhfe		_cb1, _conf0, 1, 15	// Enable TX harware flow
#define _hw__uarta_clbe			_cb1, _conf0, 1, 14	// Enable loopback
#define _hw__uarta_cbrk			_cb1, _conf0, 1,  8	// Keep TXD low (BRK)
#define _hw__uarta_cswdtr		_cb1, _conf0, 1,  7	// Assert DTR
#define _hw__uarta_cswrts		_cb1, _conf0, 1,  6	// Assert RTS
#define _hw__uarta_csbn			_cb1, _conf0, 2,  4	// StopBits Count (2bit) 0:disable, 1:1bit, 2:1.5bit, 3:2bit
#define _hw__uarta_cbn			_cb1, _conf0, 2,  2	// DataBits Count (2bin) 0:5bit, 1:6bit, 2:7bit, 3:8bit
#define _hw__uarta_cpae			_cb1, _conf0, 1,  1	// Parity Enable
#define _hw__uarta_cpa			_cb1, _conf0, 1,  0	// Parity 0:even, 1:odd
#define _hw__uarta_cpx			_cb1, _conf0, 2,  0	// HWA convenience

#define _hw__uarta_ctoe			_cb1, _conf1, 1, 31	// RX TimeOut Enable
#define _hw__uarta_ctot			_cb1, _conf1, 7, 24	// RX TimeOut Treshold (7bit)
#define _hw__uarta_crxhfe		_cb1, _conf1, 1, 23	// RX Harware Flow Enable
#define _hw__uarta_crxhft		_cb1, _conf1, 7, 16	// RX Harware Flow Treshold (7bit)
#define _hw__uarta_cfet			_cb1, _conf1, 7,  8	// TX FIFO Empty Treshold (7bit)
#define _hw__uarta_cfft			_cb1, _conf1, 7,  0	// RX FIFO Full Treshold (7bit)

#define _hw__uarta_swap			_xob1, hw_shared, _swap, 1, 2

// #define _hw__uarta_		_cb1, _, 1,	//

/*  Relatives
 */
//#define hw_uart0pin_txd			hw_pin_gpio1
//#define hw_uart0pin_rxd			hw_pin_gpio3


/*******************************************************************************
 *									       *
 *	HWA context							       *
 *									       *
 *******************************************************************************/

#if !defined __ASSEMBLER__

/**
 * @brief The HWA context
 *
 * This structure is instanciated by hwa_begin() or hwa_begin_from_reset() and
 * used by all HWA asynchronous instructions to bufferize hardware accesses.
 */
typedef struct {
  uint8_t	commit ;	/*!< 1 if commit does write into hardware registers	*/

  hwa_shared_t	hw_shared ;

  hwa_p16a_t	hw_port0 ;
  hwa_pcfa_t	hw_pin_gpio0_cf ;
  hwa_pcfa_t	hw_pin_gpio1_cf ;
  hwa_pcfa_t	hw_pin_gpio2_cf ;
  hwa_pcfa_t	hw_pin_gpio3_cf ;
  hwa_pcfa_t	hw_pin_gpio4_cf ;
  hwa_pcfa_t	hw_pin_gpio5_cf ;
  hwa_pcfa_t	hw_pin_gpio6_cf ;
  hwa_pcfa_t	hw_pin_gpio7_cf ;
  hwa_pcfa_t	hw_pin_gpio8_cf ;
  hwa_pcfa_t	hw_pin_gpio9_cf ;
  hwa_pcfa_t	hw_pin_gpio10_cf ;
  hwa_pcfa_t	hw_pin_gpio11_cf ;
  hwa_pcfa_t	hw_pin_gpio12_cf ;
  hwa_pcfa_t	hw_pin_gpio13_cf ;
  hwa_pcfa_t	hw_pin_gpio14_cf ;
  hwa_pcfa_t	hw_pin_gpio15_cf ;
  hwa_tm23a_t	hw_timer1 ;
  hwa_uarta_t	hw_uart0 ;
} hwa_t ;


#include "../hwa_2.h"
//#include "../classes/iocfa_2.h"
#include "../classes/pcfa_2.h"
#include "../classes/p16a_2.h"
#include "../classes/io1a_2.h"
#include "../classes/tm23a_2.h"
#include "../classes/uarta_2.h"


HW_INLINE void _hwa_setup_context( hwa_t *hwa )
{
  _hwa_setup( hw_shared );
  _hwa_setup( hw_port0 );
  _hwa_setup( hw_pin_gpio0_cf );
  _hwa_setup( hw_pin_gpio1_cf );
  _hwa_setup( hw_pin_gpio2_cf );
  _hwa_setup( hw_pin_gpio3_cf );
  _hwa_setup( hw_pin_gpio4_cf );
  _hwa_setup( hw_pin_gpio5_cf );
  _hwa_setup( hw_pin_gpio6_cf );
  _hwa_setup( hw_pin_gpio7_cf );
  _hwa_setup( hw_pin_gpio8_cf );
  _hwa_setup( hw_pin_gpio9_cf );
  _hwa_setup( hw_pin_gpio10_cf );
  _hwa_setup( hw_pin_gpio11_cf );
  _hwa_setup( hw_pin_gpio12_cf );
  _hwa_setup( hw_pin_gpio13_cf );
  _hwa_setup( hw_pin_gpio14_cf );
  _hwa_setup( hw_pin_gpio15_cf );

  _hwa_setup( hw_timer1 );
  _hwa_setup( hw_uart0 );
}


HW_INLINE void _hwa_init_context( hwa_t *hwa )
{
  _hwa_init( hw_shared );
  _hwa_init( hw_port0 );
  /* _hwa_init( hw_pin_gpio0_cf ); */
  /* _hwa_init( hw_pin_gpio1_cf ); */
  /* _hwa_init( hw_pin_gpio2_cf ); */
  /* _hwa_init( hw_pin_gpio3_cf ); */
  /* _hwa_init( hw_pin_gpio4_cf ); */
  /* _hwa_init( hw_pin_gpio5_cf ); */
  /* _hwa_init( hw_pin_gpio6_cf ); */
  /* _hwa_init( hw_pin_gpio7_cf ); */
  /* _hwa_init( hw_pin_gpio8_cf ); */
  /* _hwa_init( hw_pin_gpio9_cf ); */
  /* _hwa_init( hw_pin_gpio10_cf ); */
  /* _hwa_init( hw_pin_gpio11_cf ); */
  /* _hwa_init( hw_pin_gpio12_cf ); */
  /* _hwa_init( hw_pin_gpio13_cf ); */
  /* _hwa_init( hw_pin_gpio14_cf ); */
  /* _hwa_init( hw_pin_gpio15_cf ); */
  _hwa_init( hw_timer1 );
  _hwa_init( hw_uart0 );
}


HW_INLINE void _hwa_commit_context( hwa_t *hwa )
{
  _hwa_commit( hw_pin_gpio0_cf );
  _hwa_commit( hw_pin_gpio1_cf );
  _hwa_commit( hw_pin_gpio2_cf );
  _hwa_commit( hw_pin_gpio3_cf );
  _hwa_commit( hw_pin_gpio4_cf );
  _hwa_commit( hw_pin_gpio5_cf );
  _hwa_commit( hw_pin_gpio6_cf );
  _hwa_commit( hw_pin_gpio7_cf );
  _hwa_commit( hw_pin_gpio8_cf );
  _hwa_commit( hw_pin_gpio9_cf );
  _hwa_commit( hw_pin_gpio10_cf );
  _hwa_commit( hw_pin_gpio11_cf );
  _hwa_commit( hw_pin_gpio12_cf );
  _hwa_commit( hw_pin_gpio13_cf );
  _hwa_commit( hw_pin_gpio14_cf );
  _hwa_commit( hw_pin_gpio15_cf );
  _hwa_commit( hw_port0 );

  _hwa_commit( hw_timer1 );
  _hwa_commit( hw_uart0 );

  //  _hwa_commit_reg( hw_shared, _edgeie ); /* Process IRQ at last */
  _hwa_commit( hw_shared );
}

#endif /* !defined __ASSEMBLER__ */

/**
 * @page esp8266
 * <br>
 */