
/* This file is part of the HWA project.
 * Copyright (c) 2012,2015 Christophe Duparquet.
 * All rights reserved. Read LICENSE.TXT for details.
 */

/**
 * @file
 * @brief Atmel AVR ATtiny24/44/84
 */

#include "../hwa_1.h"

/**
 * @page attinyx4 ATtiny24/44/84
 * @section attinyx4_device Defined symbols
 *
 * HWA defines the following symbols describing the target device and its
 * hardware configuration:
 *
 * Symbol		    | Comments
 * :------------------------|:-----------
 * `HW_DEVICE_ATTINYX4`	    |Void.
 * `HW_DEVICE_RAM_START`    |Address of first RAM byte: `0x0060`.
 * `HW_DEVICE_APP_START`    |Address of first application byte (after IRQ vector table): `0x0022`.
 *
 * See also:
 * * @subpage attiny24
 * * @subpage attiny44
 * * @subpage attiny84
 */
#define HW_DEVICE_ATTINYX4
#define HW_DEVICE_RAM_START		0x0060
#define HW_DEVICE_APP_START		0x0022

/**
 * @page attinyx4
 * @section attinyx4_fuses Fuses
 *
 * ATtinyX4 devices hold 3 fuse bytes describing their hardware configuration.
 * HWA computes the value of these fuse bytes from the fuse values (detailed in
 * the following subsections) that must be defined prior to including the HWA
 * header. Undefined fuse values are replaced by factory default values.
 *
 * Symbol		| Comments
 * :--------------------|:-----------
 * `HW_DEVICE_FUSE_EB`	| Fuse extended byte
 * `HW_DEVICE_FUSE_HB`	| Fuse high byte
 * `HW_DEVICE_FUSE_LB`	| Fuse low byte
 */
#define HW_DEVICE_FUSE_EB			\
  0xFE | HW_DEVICE_SELFPRGEN

#define HW_DEVICE_FUSE_HB			\
  HW_DEVICE_RSTDISBL<<7 |			\
  HW_DEVICE_DWEN<<6 |				\
  HW_DEVICE_SPIEN<<5 |				\
  HW_DEVICE_WDTON<<4 |				\
  HW_DEVICE_EESAVE<<3 |				\
  HW_DEVICE_BODLEVEL

#define HW_DEVICE_FUSE_LB			\
  HW_DEVICE_CKDIV8<<7 |				\
  HW_DEVICE_CKOUT<<6 |				\
  HW_DEVICE_SUT10<<4 |				\
  HW_DEVICE_CKSEL31<<1 |			\
  HW_DEVICE_CKSEL0


/**
 * @page attinyx4
 *
 * Optionnaly, before including the device-specific header, fuses values can be
 * defined to describe the target more precisely. If these symbols are not
 * defined by the user, HWA will define them with the factory values.
 *
 */
#define _hw_is_external_external	, 1
#define _hw_is_rc_8MHz_rc_8MHz			, 1
#define _hw_is_rc_128kHz_rc_128kHz		, 1
#define _hw_is_low_freq_xosc_low_freq_xosc	, 1
#define _hw_is_xosc_xosc		, 1

/**
 * @page attinyx4
 * @subsection attinyx4_fuses_clocking Clocking
 *
 * Symbol		 |Valid values | Comments
 * :---------------------|-------------|:-----------
 * `HW_DEVICE_CLK_SRC`	 |`external`<br><b>`rc_8MHz`</b><br>`rc_128kHz`<br>`low_freq_xosc`<br>`xosc`|External source on pin CLKI<br>Internal RC oscillator<br>Internal RC oscillator (watchdog)<br>Crystal/resonator oscillator between pins XTAL1 and XTAL2<br> <br>
 * `HW_DEVICE_CLK_SRC_HZ`|Integer     |Clock frequency for `external`, `low_freq_xosc`, and `xosc` clock source
 * `HW_DEVICE_CLK_PSC`	 |<b>`8`</b><br>`1`|Clock divided by 8<br>Clock not divided
 */
#ifndef HW_DEVICE_CLK_SRC
#  define HW_DEVICE_CLK_SRC				rc_8MHz
#endif

#if HW_IS(external,HW_DEVICE_CLK_SRC)
#  define HW_DEVICE_CKSEL31				0
#  define HW_DEVICE_CKSEL0				0
#elif HW_IS(rc_8MHz,HW_DEVICE_CLK_SRC)
#  define HW_DEVICE_CKSEL31				1
#  define HW_DEVICE_CKSEL0				0
#  define hw_syshz_base					8000000
#elif HW_IS(rc_128kHz,HW_DEVICE_CLK_SRC)
#  define HW_DEVICE_CKSEL31				2
#  define HW_DEVICE_CKSEL0				0
#  define hw_syshz_base					128000
#elif HW_IS(HW_DEVICE_CLK_SRC, low_freq_xosc)
#  define HW_DEVICE_CKSEL31				3
#  define HW_DEVICE_CKSEL0				0
#  define hw_syshz_base					HW_DEVICE_CLK_SRC_HZ
#elif HW_IS(xosc, HW_DEVICE_CLK_SRC)
#  if HW_DEVICE_CLK_SRC_HZ < 900000
#    define HW_DEVICE_CKSEL31				4
#  elif HW_DEVICE_CLK_SRC_HZ < 3000000
#    define HW_DEVICE_CKSEL31				5
#  elif HW_DEVICE_CLK_SRC_HZ < 8000000
#    define HW_DEVICE_CKSEL31				6
#  else
#    define HW_DEVICE_CKSEL31				7
#  endif
#  define hw_syshz_base					HW_DEVICE_CLK_SRC_HZ
#else
#  error HW_DEVICE_CLK_SRC must be defined as one of `external`, `rc_8MHz`, `rc_128kHz`, `low_freq_xosc`, or `xosc`.
#endif

#if hw_syshz_base == 0
#  error HW_DEVICE_CLK_SRC_HZ must be defined when HW_DEVICE_CLK_SRC is `xosc` or `low_freq_xosc`.
#endif

#if !defined HW_DEVICE_CLK_PSC
#  define HW_DEVICE_CLK_PSC				8
#endif

#if HW_DEVICE_CLK_PSC == 8
#  define HW_DEVICE_CKDIV8				0
#  define hw_syshz					hw_syshz_base/8
#elif HW_DEVICE_CLK_PSC == 1
#  define HW_DEVICE_CKDIV8				1
#  define hw_syshz					hw_syshz_base
#else
HW_E_AVL('HW_DEVICE_CLK_PSC', HW_DEVICE_CLK_PSC, 1 | 8)
#endif

/**
 * @page attinyx4
 * @subsection attinyx4_fuses_sut Startup delays
 *
 * Symbol		| Valid values | Comments
 * :--------------------|--------------|:-----------
 * `HW_DEVICE_STARTUP_DELAYS` | `6CK_14CK`<br>`6CK_14CK_4ms`<br>`6CK_14CK_64ms`<br>`1KCK_4ms`<br>`1KCK_64ms`<br>`32KCK_64ms`<br>`258CK_14CK_4ms`<br>`258CK_14CK_64ms`<br>`1KCK_14CK`<br>`1KCK_14CK_4ms`<br>`1KCK_14CK_64ms`<br>`16KCK_14CK`<br>`16KCK_14CK_4ms`<br>`16KCK_14CK_64ms` | Valid values depend on the clocking configuration
*/
#define _hw_is_6CK_14CK_6CK_14CK			, 1
#define _hw_is_6CK_14CK_4ms_6CK_14CK_4ms		, 1
#define _hw_is_6CK_14CK_64ms_6CK_14CK_64ms		, 1
#define _hw_is_1KCK_4ms_1KCK_4ms			, 1
#define _hw_is_1KCK_64ms_1KCK_64ms			, 1
#define _hw_is_32KCK_64ms_32KCK_64ms			, 1
#define _hw_is_258CK_14CK_4ms_258CK_14CK_4ms		, 1
#define _hw_is_258CK_14CK_64ms_258CK_14CK_64ms		, 1
#define _hw_is_1KCK_14CK_1KCK_14CK			, 1
#define _hw_is_1KCK_14CK_4ms_1KCK_14CK_4ms		, 1
#define _hw_is_1KCK_14CK_64ms_1KCK_14CK_64ms		, 1
#define _hw_is_16KCK_14CK_16KCK_14CK			, 1
#define _hw_is_16KCK_14CK_4ms_16KCK_14CK_4ms		, 1
#define _hw_is_16KCK_14CK_64ms_16KCK_14CK_64ms		, 1

#if defined HW_DEVICE_STARTUP_DELAYS
#  if HW_DEVICE_CKSEL31 < 3
#    define HW_DEVICE_CKSEL0				0
#    if HW_IS(HW_DEVICE_STARTUP_DELAYS, 6CK_14CK)
#      define HW_DEVICE_SUT10				0
#    elif HW_IS(HW_DEVICE_STARTUP_DELAYS, 6CK_14CK_4ms)
#      define HW_DEVICE_SUT10				1
#    elif HW_IS(HW_DEVICE_STARTUP_DELAYS, 6CK_14CK_64ms)
#      define HW_DEVICE_SUT10				2
#    else
#      error HW_DEVICE_STARTUP_DELAYS must be defined as one of `6CK_14CK`, `6CK_14CK_4ms`, or `6CK_14CK_64ms`.
#    endif
#  elif HW_DEVICE_CKSEL31 == 3
#    if HW_IS(HW_DEVICE_STARTUP_DELAYS, 1KCK_4ms)
#      define HW_DEVICE_SUT10				0
#    elif HW_IS(HW_DEVICE_STARTUP_DELAYS, 1KCK_64ms)
#      define HW_DEVICE_SUT10				1
#    elif HW_IS(HW_DEVICE_STARTUP_DELAYS, 32KCK_64ms)
#      define HW_DEVICE_SUT10				2
#    else
#      error HW_DEVICE_STARTUP_DELAYS must be defined as one of `6CK_14CK`, `6CK_14CK_4ms`, or `6CK_14CK_64ms`.
#    endif
#  else
#    if HW_IS(HW_DEVICE_STARTUP_DELAYS, 258CK_14CK_4ms)
#      define HW_DEVICE_CKSEL0				0
#      define HW_DEVICE_SUT10				0
#    elif HW_IS(HW_DEVICE_STARTUP_DELAYS, 258CK_14CK_64ms)
#      define HW_DEVICE_CKSEL0				0
#      define HW_DEVICE_SUT10				1
#    elif HW_IS(HW_DEVICE_STARTUP_DELAYS, 1KCK_14CK)
#      define HW_DEVICE_CKSEL0				0
#      define HW_DEVICE_SUT10				2
#    elif HW_IS(HW_DEVICE_STARTUP_DELAYS, 1KCK_14CK_4ms)
#      define HW_DEVICE_CKSEL0				0
#      define HW_DEVICE_SUT10				3
#    elif HW_IS(HW_DEVICE_STARTUP_DELAYS, 1KCK_14CK_64ms)
#      define HW_DEVICE_CKSEL0				1
#      define HW_DEVICE_SUT10				0
#    elif HW_IS(HW_DEVICE_STARTUP_DELAYS, 16KCK_14CK)
#      define HW_DEVICE_CKSEL0				1
#      define HW_DEVICE_SUT10				1
#    elif HW_IS(HW_DEVICE_STARTUP_DELAYS, 16KCK_14CK_4ms)
#      define HW_DEVICE_CKSEL0				1
#      define HW_DEVICE_SUT10				2
#    elif HW_IS(HW_DEVICE_STARTUP_DELAYS, 16KCK_14CK_64ms)
#      define HW_DEVICE_CKSEL0				1
#      define HW_DEVICE_SUT10				3
#    else
#      error HW_DEVICE_STARTUP_DELAYS must be defined as one of `258CK_14CK_4ms`, `258CK_14CK_64ms`, `1KCK_14CK`, `1KCK_14CK_4ms`, `1KCK_14CK_64ms`, `16KCK_14CK`, `16KCK_14CK_4ms`, or `16KCK_14CK_64ms`.
#    endif
#  endif
#else
#  define HW_DEVICE_SUT10				2
#endif


/**
 * @page attinyx4
 * @subsection attinyx4_fuses_other Other fuses
 *
 * Symbol		  | Valid values | Comments
 * :----------------------|--------------|:-----------
 * `HW_DEVICE_EXTERNAL_RESET`	 |`enabled`<br>`disabled`|Whether the device can be reset via its RESET pin
 * `HW_DEVICE_SELF_PROGRAMMING`	 |`enabled`<br>`disabled`|Whether the device can write into its Flash program memory
 * `HW_DEVICE_SERIAL_PROGRAMMING`|`enabled`<br>`disabled`|Whether the device can be programmed via the SPI
 * `HW_DEVICE_PRESERVE_EEPROM_FROM_CHIP_ERASE`|`enabled`<br>`disabled`|Whether the EEPROM memory is erased when a chip erase occurs
 * `HW_DEVICE_DEBUG_WIRE`	 |`disabled`<br>`enabled`|Whether the Debug Wire is operationnal
 * `HW_DEVICE_WATCHDOG_ALWAYS_ON`|`no`<br>`yes`		 |Whether the watchdog is always running
 * `HW_DEVICE_CLOCK_OUTPUT`	 |`disabled`<br>`enabled`|Whether the device outputs its clock
 * `HW_DEVICE_BROWNOUT_DETECTION`|`2500_2900mV`<br>`1700_2000mV`<br>`4100_4500mV`|Brown-out detection level
 *
 */
#if !defined HW_DEVICE_CLOCK_OUTPUT
#  define HW_DEVICE_CLOCK_OUTPUT			disabled
#endif
#if HW_IS(HW_DEVICE_CLOCK_OUTPUT, enabled)
#  define HW_DEVICE_CKOUT				0
#elif HW_IS(HW_DEVICE_CLOCK_OUTPUT, disabled)
#  define HW_DEVICE_CKOUT				1
#else
#  error HW_DEVICE_CLOCK_OUTPUT must be defined as `enabled` or `disabled` (default).
#endif

#if !defined HW_DEVICE_SELF_PROGRAMMING
#  define HW_DEVICE_SELF_PROGRAMMING			enabled
#endif
#if HW_IS(HW_DEVICE_SELF_PROGRAMMING, enabled)
#  define HW_DEVICE_SELFPRGEN				0
#elif HW_IS(HW_DEVICE_SELF_PROGRAMMING, disabled)
#  define HW_DEVICE_SELFPRGEN				1
#else
#  error HW_DEVICE_SELF_PROGRAMMING must be defined as `enabled` or `disabled` (default).
#endif

#if !defined HW_DEVICE_EXTERNAL_RESET
#  define HW_DEVICE_EXTERNAL_RESET			enabled
#endif
#if HW_IS(HW_DEVICE_EXTERNAL_RESET, enabled)
#  define HW_DEVICE_RSTDISBL				1
#elif HW_IS(HW_DEVICE_EXTERNAL_RESET, disabled)
#  define HW_DEVICE_RSTDISBL				0
#else
#  error HW_DEVICE_EXTERNAL_RESET must be defined as `enabled` (default) or `disabled`.
#endif

#if !defined HW_DEVICE_DEBUG_WIRE
#  define HW_DEVICE_DEBUG_WIRE				disabled
#endif
#if HW_IS(HW_DEVICE_DEBUG_WIRE, enabled)
#  define HW_DEVICE_DWEN				0
#elif HW_IS(HW_DEVICE_DEBUG_WIRE, disabled)
#  define HW_DEVICE_DWEN				1
#else
#  error HW_DEVICE_DEBUG_WIRE must be defined as `enabled` or `disabled` (default).
#endif

#if !defined HW_DEVICE_SERIAL_PROGRAMMING
#  define HW_DEVICE_SERIAL_PROGRAMMING			enabled
#endif
#if HW_IS(HW_DEVICE_SERIAL_PROGRAMMING, enabled)
#  define HW_DEVICE_SPIEN				0
#elif HW_IS(HW_DEVICE_SERIAL_PROGRAMMING, disabled)
#  define HW_DEVICE_SPIEN				1
#else
#  error HW_DEVICE_SERIAL_PROGRAMMING must be defined as `enabled` (default) or `disabled`.
#endif

#if !defined HW_DEVICE_WATCHDOG_ALWAYS_ON
#  define HW_DEVICE_WATCHDOG_ALWAYS_ON			no
#endif
#if HW_IS(HW_DEVICE_WATCHDOG_ALWAYS_ON, yes)
#  define HW_DEVICE_WDTON				0
#elif HW_IS(HW_DEVICE_WATCHDOG_ALWAYS_ON, no)
#  define HW_DEVICE_WDTON				1
#else
#  error HW_DEVICE_WATCHDOG_ALWAYS_ON must be defined as `yes` or `no` (default).
#endif

#if !defined HW_DEVICE_PRESERVE_EEPROM_FROM_CHIP_ERASE
#  define HW_DEVICE_PRESERVE_EEPROM_FROM_CHIP_ERASE	no
#endif
#if HW_IS(HW_DEVICE_PRESERVE_EEPROM_FROM_CHIP_ERASE, yes)
#  define HW_DEVICE_EESAVE				0
#elif HW_IS(HW_DEVICE_PRESERVE_EEPROM_FROM_CHIP_ERASE, no)
#  define HW_DEVICE_EESAVE				1
#else
#  error HW_DEVICE_PRESERVE_EEPROM_FROM_CHIP_ERASE must be defined as `yes` or `no` (default).
#endif

#define _hw_is_1700_2000mV_1700_2000mV			, 1
#define _hw_is_2500_2900mV_2500_2900mV			, 1
#define _hw_is_4100_4500mV_4100_4500mV			, 1

#if !defined HW_DEVICE_BROWNOUT_DETECTION
#  define HW_DEVICE_BROWNOUT_DETECTION			off
#endif
#if HW_IS(HW_DEVICE_BROWNOUT_DETECTION, 1700_2000mV)
#  define HW_DEVICE_BODLEVEL				6
#elif HW_IS(HW_DEVICE_BROWNOUT_DETECTION, 2500_2900mV)
#  define HW_DEVICE_BODLEVEL				5
#elif HW_IS(HW_DEVICE_BROWNOUT_DETECTION, 4100_4500mV)
#  define HW_DEVICE_BODLEVEL				4
#elif HW_IS(HW_DEVICE_BROWNOUT_DETECTION, off)
#  define HW_DEVICE_BODLEVEL				7
#else
#  error HW_DEVICE_BROWNOUT_DETECTION must be defined as `1700_2000mV`, `2500_2900mV`, `4100_4500mV` or `off` (default).
#endif



/*******************************************************************************
 *									       *
 *	Peripherals							       *
 *									       *
 *******************************************************************************/

/**
 * @page attinyx4
 * @section attinyx4_object Peripherals
 * 
 * Object name	 |Class				  | Comments
 * :-------------|--------------------------------|:--------------------------------------
 * `core0`	 |@ref atmelavr_corea "_corea"	  | The core
 * `int0`	 |@ref atmelavr_inta "_inta"	  | External interrupt INT0
 * `port0`	 |@ref atmelavr_p8a "_p8a"	  | General purpose I/O port A (PORT0)
 * `port1`	 |@ref atmelavr_p8a "_p8a"	  | General purpose I/O port B (PORT1)
 * `pcic0`	 |@ref atmelavr_pcica "_pcica"	  | Pin change interrupt controller
 * `pcic1`	 |@ref atmelavr_pcica "_pcica"	  | Pin change interrupt controller
 * `watchdog0`	 |@ref atmelavr_wdoga "_wdoga"	  | Watchdog (WDG)
 * `prescaler0`	 |@ref atmelavr_psca "_psca"	  | hw_counter0/hw_counter1 prescaler (PSC0)
 * `counter0` |@ref atmelavr_c8a "_c8a"	  | 8-bit counter-timer (T0)
 * `counter0compare0`	 |@ref atmelavr_cmp8a "_cmp8a"	  | Compare unit 0 of hw_counter0 (OC0A)
 * `counter0compare1`	 |@ref atmelavr_cmp8a "_cmp8a"	  | Compare unit 1 of hw_counter0 (OC0B)
 * `counter1` |@ref atmelavr_c16a "_c16a"	  | 16-bit counter-timer (T1)
 * `counter1compare0`	 |@ref atmelavr_cmp16a "_cmp16a"	  | Compare unit 0 of hw_counter1 (OC1A)
 * `counter1compare1`	 |@ref atmelavr_cmp16a "_cmp16a"	  | Compare unit 1 of hw_counter1 (OC1B)
 * `counter1capture0`	 |@ref atmelavr_cap16a "_cap16a"	  | Capture unit 0 of hw_counter1 (ICP)
 * `usi0`	 |@ref atmelavr_usia "_usia"	  | Universal Serial Interface
 * `acmp0`	 |@ref atmelavr_acmpa "_acmpa"	  | Analog Comparator
 * `adc0`	 |@ref atmelavr_ad10a "_ad10a"	  | 10-bit Analog to Digital Converter
 * `eeprom0`	 |@ref atmelavr_eeproma "_eeproma"| Eeprom memory
 * `flash0`	 |@ref atmelavr_flasha "_flasha"  | Flash memory
 *
 * @subsection attinyx4_swobj Software-emulated peripherals
 * 
 * HWA provides the following software-emulated peripherals:
 * 
 * Name			  | Class		  | Comments
 * :----------------------|-----------------------|:--------------------------------------
 * `spimaster_swclk0`  | @ref atmelavr_usia_spimaster_swclk "_usia_spimaster_swclk" | Universal Serial Interface used as SPI master and clocked by software
 * `swuart0`		  | @ref atmelavr_swuarta "_swuarta" | Software UART
 * `swuart1`		  | @ref atmelavr_swuarta "_swuarta" | Software UART
 *
 * @subsection attinyx4_objrel Aliases and relations
 *
 * Some objects can be accessed from their relatives or can have more than one
 * name. There are the existing relations between the device's objects and their
 * different names:
 *
 * Name		 | Aliases		 | Relations
 * :-------------|-----------------------|:--------------------------------------
 * `prescaler0`	 | `counter0prescaler`	| `HW_RELATIVE(counter0, prescaler)`
 * `prescaler0`	 | `counter0prescaler0` | `HW_RELATIVE(counter0, prescaler0)`
 * `prescaler0`	 | `counter1prescaler`	| `HW_RELATIVE(counter1, prescaler)`
 * `prescaler0`	 | `counter1prescaler0` | `HW_RELATIVE(counter1, prescaler0)`
 * `counter0compare0`	 | `counter0compare0`	| `HW_RELATIVE(counter0, compare0)`
 * `counter0compare1`	 | `counter0compare1`	| `HW_RELATIVE(counter0, compare1)`
 * `counter0`	 | `counter0compare0counter`	   | `HW_RELATIVE(counter0compare0, counter)`
 * `counter0`	 | `counter0compare1counter`	   | `HW_RELATIVE(counter0compare1, counter)`
 * `pin_counter0compare0` | `counter0compare0pin`		   | `HW_RELATIVE(counter0compare0, pin)`
 * `pin_counter0compare1` | `counter0compare1pin`		   | `HW_RELATIVE(counter0compare1, pin)`
 * `counter1compare0`	 | `counter1compare0`	| `HW_RELATIVE(counter1, compare0)`
 * `counter1compare1`	 | `counter1compare1`	| `HW_RELATIVE(counter1, compare1)`
 * `counter1`	 | `counter1compare0counter`	   | `HW_RELATIVE(counter1compare0, counter)`
 * `counter1`	 | `counter1compare1counter`	   | `HW_RELATIVE(counter1compare1, counter)`
 * `pin_counter1compare0` | `counter1compare0pin`		   | `HW_RELATIVE(counter1compare0, pin)`
 * `pin_counter1compare1` | `counter1compare1pin`		   | `HW_RELATIVE(counter1compare1, pin)`
 */


/*******************************************************************************
 *									       *
 *	Interrupts							       *
 *									       *
 *******************************************************************************/

/**
 * @page attinyx4
 * @section attinyx4_interrupts Interrupts
 * 
 * Interrupt name	  | Atmel label | Comments
 * :----------------------|-------------|------------------------
 * `int0`		  | INT0	| External interrupt INT0
 * `pcic0`		  | PCINT0	| Pin-change interrupt
 * `pcic1`		  | PCINT1	| Pin-change interrupt
 * `watchdog0`		  | WDT		| Watchdog timeout
 * `counter1,capture0` | TIM1_CAPT	| Capture event on counter 1
 * `counter1,compare0` | TIM1_COMPA	| Compare-match A on counter 1
 * `counter1,compare1` | TIM1_COMPB	| Compare-match B on counter 1
 * `counter1,overflow` | TIM1_OVF	| Counter 1 overflow
 * `counter0,compare0` | TIM0_COMPA	| Compare-match A on counter 0
 * `counter0,compare1` | TIM0_COMPB	| Compare-match B on counter 0
 * `counter0,overflow` | TIM0_OVF	| Counter 0 overflow
 * `acmp0`		  | ANA_COMP	| Analog comparator
 * `adc0`		  | ADC		| ADC conversion complete
 * `eeprom0`		  | EE_RDY	| EEPROM ready
 * `eeprom0,ready`	  | EE_RDY	| EEPROM ready
 * `usi0,start`	  | USI_STR	| USI start
 * `usi0,overflow`	  | USI_OVF	| USI overflow
 * `usi0,txc`	  | USI_OVF	| USI overflow (transmit completed)
 */
#define _hw_irq_int0_			_irq, core0,	 1, ie,	 if
#define _hw_irq_pcic0_			_irq, pcic0,	 2, ie,	 if
#define _hw_irq_pcic1_			_irq, pcic1,	 3, ie,	 if
#define _hw_irq_watchdog0_		_irq, watchdog0,	 4, ie,	 if
#define _hw_irq_counter1_capture0	_irq, counter1capture0,	 5, ie,	 if
#define _hw_irq_counter1capture0_	_irq, counter1capture0,	 5, ie,	 if
#define _hw_irq_counter1_compare0	_irq, counter1compare0,	 6, ie,	 if
#define _hw_irq_counter1compare0_	_irq, counter1compare0,	 6, ie,	 if
#define _hw_irq_counter1_compare1	_irq, counter1compare1,	 7, ie,	 if
#define _hw_irq_counter1compare1_	_irq, counter1compare1,	 7, ie,	 if
#define _hw_irq_counter1_overflow	_irq, counter1,	 8, ie,	 if
#define _hw_irq_counter1_		_irq, counter1,	 8, ie,	 if
#define _hw_irq_counter0_compare0	_irq, counter0compare0,	 9, ie,	 if
#define _hw_irq_counter0compare0_	_irq, counter0compare0,	 9, ie,	 if
#define _hw_irq_counter0_compare1	_irq, counter0compare1,	10, ie,	 if
#define _hw_irq_counter0compare1_	_irq, counter0compare1,	10, ie,	 if
#define _hw_irq_counter0_overflow	_irq, counter0, 11, ie,	 if
#define _hw_irq_counter0_		_irq, counter0, 11, ie,	 if
#define _hw_irq_acmp0_			_irq, acmp0,	12, ie,	 if
#define _hw_irq_adc0_			_irq, adc0,	13, ie,	 if
#define _hw_irq_eeprom0_		_irq, eeprom0,	14, sie, /* no irq flag */
#define _hw_irq_eeprom0_ready		_irq, eeprom0,	14, sie,
#define _hw_irq_usi0_start		_irq, usi0,	15, ies,  ifs
#define _hw_irq_usi0_overflow		_irq, usi0,	16, ieov, ifov
#define _hw_irq_usi0_txc		_irq, usi0,	16, ieov, ifov


/*******************************************************************************
 *									       *
 *	Shared registers						       *
 *									       *
 *	This is an object used to gather hardware registers shared	       *
 *	by other objects						       *
 *									       *
 *******************************************************************************/

#define _hw_class__shared
#define _hw_def_shared			_shared, 101, 0

#define _hw_reg_shared_gimsk		_r8, 0x5B, 0x70, 0x00
#define _hw_reg_shared_gifr		_r8, 0x5A, 0x70, 0x70
#define _hw_reg_shared_timsk		_r8, 0x59, 0x7E, 0x00
#define _hw_reg_shared_tifr		_r8, 0x58, 0x7E, 0x7E
#define _hw_reg_shared_gtccr		_r8, 0x43, 0x81, 0x00
#define _hw_reg_shared_prr		_r8, 0x40, 0x0F, 0x00
#define _hw_reg_shared_gpior2		_r8, 0x35, 0xFF, 0x00
#define _hw_reg_shared_gpior1		_r8, 0x34, 0xFF, 0x00
#define _hw_reg_shared_gpior0		_r8, 0x33, 0xFF, 0x00
#define _hw_reg_shared_did		_r8, 0x21, 0xFF, 0x00

#if !defined __ASSEMBLER__
/*
 *  HWA context
 */
typedef struct {

  /*  Hardware registers
   */
  hwa_r8_t	gimsk ;
  hwa_r8_t	gifr ;
  hwa_r8_t	timsk ;
  hwa_r8_t	tifr ;
  hwa_r8_t	gtccr ;
  hwa_r8_t	prr ;
  hwa_r8_t	did ;
} hwa_shared_t ;
#endif


/*******************************************************************************
 *									       *
 *	Ports and pins							       *
 *									       *
 *******************************************************************************/

/**
 * @page attinyx4
 * @section attinyx4_pins Ports and pins
 *
 * Some of the pins `pb0`..`pb3` may not be available depending on
 * the fuses configuration.
 *
 * HWA name | 14pdip       | Class                      | Atmel name
 * ---------|--------------|----------------------------|-----------
 * `porta`  |              | @ref atmelavr_p8a "_p8a"   | PORTA
 * `pa0`    | `HW_PIN(13)` | @ref atmelavr_io1a "_io1a" | PA0
 * `pa1`    | `HW_PIN(12)` | @ref atmelavr_io1a "_io1a" | PA1
 * `pa2`    | `HW_PIN(11)` | @ref atmelavr_io1a "_io1a" | PA2
 * `pa3`    | `HW_PIN(10)` | @ref atmelavr_io1a "_io1a" | PA3
 * `pa4`    | `HW_PIN(9)`  | @ref atmelavr_io1a "_io1a" | PA4
 * `pa5`    | `HW_PIN(8)`  | @ref atmelavr_io1a "_io1a" | PA5
 * `pa6`    | `HW_PIN(7)`  | @ref atmelavr_io1a "_io1a" | PA6
 * `pa7`    | `HW_PIN(6)`  | @ref atmelavr_io1a "_io1a" | PA7
 * `portb`  |              | @ref atmelavr_p8a "_p8a"   | PORTB
 * `pb0`    | `HW_PIN(2)`  | @ref atmelavr_io1a "_io1a" | PB0
 * `pb1`    | `HW_PIN(3)`  | @ref atmelavr_io1a "_io1a" | PB1
 * `pb2`    | `HW_PIN(5)`  | @ref atmelavr_io1a "_io1a" | PB2
 * `pb3`    | `HW_PIN(4)`  | @ref atmelavr_io1a "_io1a" | PB3
 */

#include "../classes/p8a_1.h"
#include "../classes/io1a_1.h"

/*	Objects				class, id, address
 */
#define _hw_def_port0			_p8a, 102, 0x39
#define _hw_def_port1			_p8a, 103, 0x36

/*  Pins				class, id, peripheral, bn, bp
 */
#define _hw_def_pa0			_io1a, 104, port0, 1, 0
#define _hw_def_pa1			_io1a, 105, port0, 1, 1
#define _hw_def_pa2			_io1a, 106, port0, 1, 2
#define _hw_def_pa3			_io1a, 107, port0, 1, 3
#define _hw_def_pa4			_io1a, 108, port0, 1, 4
#define _hw_def_pa5			_io1a, 109, port0, 1, 5
#define _hw_def_pa6			_io1a, 110, port0, 1, 6
#define _hw_def_pa7			_io1a, 111, port0, 1, 7
#define _hw_def_porta			_io1a, 112, port0, 8, 0

#if !HW_IS(external,HW_DEVICE_CLK_SRC) && !HW_IS(xosc,HW_DEVICE_CLK_SRC)
#  define _hw_def_pb0			_io1a, 113, port1, 1, 0
#endif
#define _hw_def_pb1			_io1a, 113, port1, 1, 1
#if !HW_IS(enabled,HW_DEVICE_CLOCK_OUTPUT) && !HW_IS(xosc,HW_DEVICE_CLK_SRC)
#  define _hw_def_pb2			_io1a, 115, port1, 1, 2
#endif
#if !HW_IS(enabled,HW_DEVICE_EXTERNAL_RESET) && !HW_IS(enabled,HW_DEVICE_DEBUG_WIRE)
#  define _hw_def_pb3			_io1a, 116, port1, 1, 3
#endif
#define _hw_def_portb			_io1a, 114, port1, 4, 0

/*  Digital input disable bits for analog input pins
 */
#define _hw_reg_pa0_did			_xob1, shared, did, 1, 0
#define _hw_reg_pa1_did			_xob1, shared, did, 1, 1
#define _hw_reg_pa2_did			_xob1, shared, did, 1, 2
#define _hw_reg_pa3_did			_xob1, shared, did, 1, 3
#define _hw_reg_pa4_did			_xob1, shared, did, 1, 4
#define _hw_reg_pa5_did			_xob1, shared, did, 1, 5
#define _hw_reg_pa6_did			_xob1, shared, did, 1, 6
#define _hw_reg_pa7_did			_xob1, shared, did, 1, 7

/*  Relative pin change controllers
 */
#define _hw_rel_pa0_pcic		pcic0
#define _hw_rel_pa1_pcic		pcic0
#define _hw_rel_pa2_pcic		pcic0
#define _hw_rel_pa3_pcic		pcic0
#define _hw_rel_pa4_pcic		pcic0
#define _hw_rel_pa5_pcic		pcic0
#define _hw_rel_pa6_pcic		pcic0
#define _hw_rel_pa7_pcic		pcic0

#define _hw_rel_pb0_pcic		pcic1
#define _hw_rel_pb1_pcic		pcic1
#define _hw_rel_pb2_pcic		pcic1
#define _hw_rel_pb3_pcic		pcic1

/*  Canonical pin names
 */
#define _hw_pin_ain0			, pa1
#define _hw_pin_ain1			, pa2

#define _hw_pin_usck			, pa4
#define _hw_pin_do			, pa5
#define _hw_pin_di			, pa6

#define _hw_pin_usi0_ck			, pa4
#define _hw_pin_usi0_do			, pa5
#define _hw_pin_usi0_di			, pa6

#define _hw_pin_counter0compare0	, pb2
#define _hw_pin_counter0compare1	, pa7
#define _hw_pin_counter1compare0	, pa6
#define _hw_pin_counter1compare1	, pa5
#define _hw_pin_counter1capture0	, pa7
#define _hw_pin_icp			, pa7

#define _hw_pin_adc0			, pa0
#define _hw_pin_adc1			, pa1
#define _hw_pin_adc2			, pa2
#define _hw_pin_adc3			, pa3
#define _hw_pin_adc4			, pa4
#define _hw_pin_adc5			, pa5
#define _hw_pin_adc6			, pa6
#define _hw_pin_adc7			, pa7

#if defined HW_DEVICE_PACKAGE_14P3
#
#  define _hw_pin_2			, pb0
#  define _hw_pin_3			, pb1
#  define _hw_pin_4			, pb3
#  define _hw_pin_5			, pb2
#  define _hw_pin_6			, pa7
#  define _hw_pin_7			, pa6
#  define _hw_pin_8			, pa5
#  define _hw_pin_9			, pa4
#  define _hw_pin_10			, pa3
#  define _hw_pin_11			, pa2
#  define _hw_pin_12			, pa1
#  define _hw_pin_13			, pa0
#
#endif


/*******************************************************************************
 *									       *
 *	Core								       *
 *									       *
 *******************************************************************************/

#include "../classes/corea_1.h"

/*	Object				class, id, address
 */
#define _hw_def_core0			_corea, 115, 0

/*	Class hardware registers	class, address, write mask, flags mask
 */
#define _hw_reg__corea_sreg		_r8, 0x5F, 0xFF, 0x00
#define _hw_reg__corea_sph		_r8, 0x5E, 0x03, 0x00
#define _hw_reg__corea_spl		_r8, 0x5D, 0xFF, 0x00

/*	Object hardware registers	class, address, write mask, flags mask
 */
#define _hw_reg_core0_mcucr		_r8, 0x55, 0xFF, 0x00
#define _hw_reg_core0_mcusr		_r8, 0x54, 0x0F, 0x00
#define _hw_reg_core0_osccal		_r8, 0x51, 0xFF, 0x00

/*	Object logical registers
 */
#define _hw_reg_core0_bods		_ob1, mcucr, 1, 7
#define _hw_reg_core0_pud		_ob1, mcucr, 1, 6
#define _hw_reg_core0_se		_ob1, mcucr, 1, 5
#define _hw_reg_core0_sm		_ob1, mcucr, 2, 3
#define _hw_reg_core0_bodse		_ob1, mcucr, 1, 2

#define _hw_reg_core0_wdrf		_ob1, mcusr, 1, 3
#define _hw_reg_core0_borf		_ob1, mcusr, 1, 2
#define _hw_reg_core0_extrf		_ob1, mcusr, 1, 1
#define _hw_reg_core0_porf		_ob1, mcusr, 1, 0
#define _hw_reg_core0_allrf		_ob1, mcusr, 4, 0	/* convenient */


/*******************************************************************************
 *									       *
 *	External interrupt controller					       *
 *									       *
 *******************************************************************************/

/*	Object				class, id, address
 */
#include "../classes/inta_1.h"
#define _hw_def_int0			_inta, 116, 0

#define _hw_reg_int0_sc			_xob1, core0,  mcucr, 2, 0
#define _hw_reg_int0_ie			_xob1, shared, gimsk, 1, 6
#define _hw_reg_int0_if			_xob1, shared, gifr,  1, 6


/*******************************************************************************
 *									       *
 *	Pin-change interrupt controllers				       *
 *									       *
 *******************************************************************************/

#include "../classes/pcica_1.h"

/*	Object				class, id, address
 */
#define _hw_def_pcic0			_pcica, 117, 0

/*	Object hardware registers	class, address, write mask, flags mask
 */
#define _hw_reg_pcic0_msk		_r8, 0x32, 0xFF, 0x00

/*	Object logical registers
 */
#define _hw_reg_pcic0_ie		_xob1, shared, gimsk, 1, 4
#define _hw_reg_pcic0_if		_xob1, shared, gifr,  1, 4

/*	Object				class, id, address
 */
#define _hw_def_pcic1			_pcica, 118, 0

/*	Object hardware registers	class, address, write mask, flags mask
 */
#define _hw_reg_pcic1_msk		_r8, 0x40, 0x0F, 0x00

/*	Object logical registers
 */
#define _hw_reg_pcic1_ie		_xob1, shared, gimsk, 1, 5
#define _hw_reg_pcic1_if		_xob1, shared, gifr,  1, 5


/*******************************************************************************
 *									       *
 *	Watchdog							       *
 *									       *
 *******************************************************************************/

/*	Object				class, id, address
 */
#include "../classes/wdoga_1.h"
#define _hw_def_watchdog0		_wdoga, 119, 0

/*	Class registers			class, address, write mask, flags mask
 */
#define _hw_reg__wdoga_csr		_r8, 0x41, 0xFF, 0x80

#define _hw_reg__wdoga_if		_cb1, csr, 1, 7
#define _hw_reg__wdoga_ie		_cb1, csr, 1, 6
#define _hw_reg__wdoga_wdp		_cb2, csr, 1, 5, 3, csr, 3, 0, 0
#define _hw_reg__wdoga_wdce		_cb1, csr, 1, 4
#define _hw_reg__wdoga_wde		_cb1, csr, 1, 3

#define _hw_reg__wdoga_eie		_cb2, csr, 1, 3, 1, csr, 1, 6, 0 /* convenient */
#define _hw_reg__wdoga_ifie		_cb1, csr, 2, 6 /* convenient for clearing irq */

#define _hw_reg__wdoga_wdrf		_xob1, core0, mcusr, 1, 3


/*******************************************************************************
 *									       *
 *	Counter 0/1 prescaler						       *
 *									       *
 *******************************************************************************/

#include "../classes/psca_1.h"

/*	Object				class, id, address
 */
#define _hw_def_prescaler0		_psca, 120, 0

/*	Object logical registers
 */
#define _hw_reg_prescaler0_tsm		_xob1, shared, gtccr, 1, 7
#define _hw_reg_prescaler0_psr		_xob1, shared, gtccr, 1, 0

/*  This is a convenient logical register definition for stopping the prescaler
 */
#define _hw_reg_prescaler0_tsmpsr	_xob2, shared, gtccr, 1, 7, 1, gtccr, 1, 0, 0


/*******************************************************************************
 *									       *
 *	Counter 0 counting unit						       *
 *									       *
 *******************************************************************************/

/*	Object				class, id, address
 */
#include "../classes/c8a_1.h"
#define _hw_def_counter0		_c8a, 121, 0

/*	Object hardware registers	class, address, write mask, flags mask
 */
#define _hw_reg_counter0_imsk		_r8, 0x59, 0x07, 0x00
#define _hw_reg_counter0_ifr		_r8, 0x58, 0x07, 0x07
#define _hw_reg_counter0_ccrb		_r8, 0x53, 0xCF, 0x00
#define _hw_reg_counter0_count		_r8, 0x52, 0xFF, 0x00
#define _hw_reg_counter0_ccra		_r8, 0x50, 0xF3, 0x00

/*	Object logical registers
 */
#define _hw_reg_counter0_wgm		_ob2, ccra, 2, 0, 0, ccrb, 1, 3, 2
#define _hw_reg_counter0_cs		_ob1, ccrb, 3, 0
#define _hw_reg_counter0_ie		_ob1, imsk, 1, 0
#define _hw_reg_counter0_if		_ob1, ifr,  1, 0

/*	Relatives
 */
#define _hw_rel_counter0_prescaler0	prescaler0
#define _hw_rel_counter0_prescaler	prescaler0
#define _hw_rel_counter0_compare0	counter0compare0
#define _hw_rel_counter0_compare1	counter0compare1


/*******************************************************************************
 *									       *
 *	Counter 0 compare units (OCR0A, OCR0B)				       *
 *									       *
 *******************************************************************************/

#include "../classes/cmp8a_1.h"

/*	Object				class, id, address
 */
#define _hw_def_counter0compare0	_cmp8a, 122, 0

/*	Object hardware registers	class, address, write mask, flags mask
 */
#define _hw_reg_counter0compare0_reg	_r8, 0x56, 0xFF, 0x00	/* OCR0A */

/*	Object logical registers
 */
#define _hw_reg_counter0compare0_com	_xob1, counter0, ccra, 2, 6
#define _hw_reg_counter0compare0_force	_xob1, counter0, ccrb, 1, 7
#define _hw_reg_counter0compare0_ie	_xob1, counter0, imsk, 1, 1
#define _hw_reg_counter0compare0_if	_xob1, counter0, ifr,  1, 1

/*	Relatives
 */
#define _hw_rel_counter0compare0_pin		pb2
#define _hw_rel_counter0compare0_counter	counter0

/*	Object				class, id, address
 */
#define _hw_def_counter0compare1	_cmp8a, 123, 0

/*	Object hardware registers	class, address, write mask, flags mask
 */
#define _hw_reg_counter0compare1_reg	_r8, 0x5C, 0xFF, 0x00	/* OCR0B */

/*	Object logical registers
 */
#define _hw_reg_counter0compare1_com	_xob1, counter0, ccra, 2, 4
#define _hw_reg_counter0compare1_force	_xob1, counter0, ccrb, 1, 6
#define _hw_reg_counter0compare1_ie	_xob1, counter0, imsk, 1, 2
#define _hw_reg_counter0compare1_if	_xob1, counter0, ifr,  1, 2

/*	Relatives
 */
#define _hw_rel_counter0compare1_pin		pa7
#define _hw_rel_counter0compare1_counter	counter0


/*******************************************************************************
 *									       *
 *	Counter 1 counting unit						       *
 *									       *
 *******************************************************************************/

#include "../classes/c16a_1.h"

/*	Object				class, id, address
 */
#define _hw_def_counter1		_c16a, 124, 0

/*	Object hardware registers	class, address, write mask, flags mask
 */
#define _hw_reg_counter1_ccra		_r8,  0x4F,   0xF3,   0x00
#define _hw_reg_counter1_ccrb		_r8,  0x4E,   0xDF,   0x00
#define _hw_reg_counter1_ccrc		_r8,  0x42,   0xC0,   0x00
#define _hw_reg_counter1_count		_r16, 0x4C, 0xFFFF, 0x0000
#define _hw_reg_counter1_imsk		_r8,  0x2C,   0x27,   0x00
#define _hw_reg_counter1_ifr		_r8,  0x2B,   0x27,   0x27

/*	Object logical registers
 */
#define _hw_reg_counter1_cs		_ob1, ccrb, 3, 0
#define _hw_reg_counter1_wgm		_ob2, ccrb, 2, 3, 2, ccra, 2, 0, 0
#define _hw_reg_counter1_ie		_ob1, imsk, 1, 0
#define _hw_reg_counter1_if		_ob1, ifr, 1, 0

/*	Relatives
 */
#define _hw_rel_counter1_prescaler0	prescaler0
#define _hw_rel_counter1_prescaler	prescaler0
#define _hw_rel_counter1_compare0	counter1compare0
#define _hw_rel_counter1_compare1	counter1compare1
#define _hw_rel_counter1_capture0	counter1capture0


/*******************************************************************************
 *									       *
 *	Counter 1 compare units hw_counter1compare0, hw_counter1compare1 (OCR1A, OCR1B)		       *
 *									       *
 *******************************************************************************/

#include "../classes/cmp16a_1.h"

/*	Object				class, id, address
 */
#define _hw_def_counter1compare0	_cmp16a, 125, 0

/*	Object hardware registers	class, address, write mask, flags mask
 */
#define _hw_reg_counter1compare0_reg	_r16, 0x4A, 0xFFFF, 0x0000	/* OCR1A */

/*	Object logical registers
 */
#define _hw_reg_counter1compare0_com	_xob1, counter1, ccra, 2, 6
#define _hw_reg_counter1compare0_force	_xob1, counter1, ccrc, 1, 7
#define _hw_reg_counter1compare0_ie	_xob1, counter1, imsk, 1, 1
#define _hw_reg_counter1compare0_if	_xob1, counter1, ifr,  1, 1

/*	Relatives
 */
#define _hw_rel_counter1compare0_pin		pa6
#define _hw_rel_counter1compare0_counter	counter1

/*	Object				class, id, address
 */
#define _hw_def_counter1compare1	_cmp16a, 126, 0

/*	Object hardware registers	class, address, write mask, flags mask
 */
#define _hw_reg_counter1compare1_reg	_r16, 0x48, 0xFFFF, 0x0000	/* OCR1B */

/*	Object logical registers
 */
#define _hw_reg_counter1compare1_com	_xob1, counter1, ccra, 2, 4
#define _hw_reg_counter1compare1_force	_xob1, counter1, ccrc, 1, 6
#define _hw_reg_counter1compare1_ie	_xob1, counter1, imsk, 1, 2
#define _hw_reg_counter1compare1_if	_xob1, counter1, ifr,  1, 2

/*	Relatives
 */
#define _hw_rel_counter1compare1_pin		pa5
#define _hw_rel_counter1compare1_counter	counter1


/*******************************************************************************
 *									       *
 *	Counter 1 capture unit						       *
 *									       *
 *******************************************************************************/

#include "../classes/cap16a_1.h"

/*	Object				class, id, address
 */
#define _hw_def_counter1capture0	_cap16a, 127, 0

/*	Object hardware registers	class, address, write mask, flags mask
 */
#define _hw_reg_counter1capture0_reg	_r16, 0x44, 0xFFFF, 0x0000	/* ICR1 */

/*	Object logical registers
 */
#define _hw_reg_counter1capture0_acic	_xob1, acmp0, csr, 1, 2
#define _hw_reg_counter1capture0_icnc	_xob1, counter1, ccrb, 1, 7
#define _hw_reg_counter1capture0_ices	_xob1, counter1, ccrb, 1, 6
#define _hw_reg_counter1capture0_ie	_xob1, counter1, imsk, 1, 5
#define _hw_reg_counter1capture0_if	_xob1, counter1, ifr,  1, 5

/*	Relatives
 */
#define _hw_rel_counter1capture0_pin		pa7
#define _hw_rel_counter1capture0_counter	counter1


/*******************************************************************************
 *									       *
 *	Universal Serial Interface					       *
 *									       *
 *******************************************************************************/

/*	Instance			class, id, address
 */					
#include "../classes/usia_1.h"
#define _hw_def_usi0			_usia, 128, 0

/*	Class registers			class, address, write mask, flags mask
 */					
#define _hw_reg__usia_cr		_r8, 0x2D, 0xFF, 0x00
#define _hw_reg__usia_sr		_r8, 0x2E, 0xFF, 0xE0
#define _hw_reg__usia_dr		_r8, 0x2F, 0xFF, 0x00
#define _hw_reg__usia_br		_r8, 0x30, 0xFF, 0x00

#define _hw_reg__usia_ies		_cb1, cr, 1, 7
#define _hw_reg__usia_ieov		_cb1, cr, 1, 6
#define _hw_reg__usia_wm		_cb1, cr, 2, 4
#define _hw_reg__usia_cs		_cb1, cr, 2, 2
#define _hw_reg__usia_clk		_cb1, cr, 1, 1
#define _hw_reg__usia_tc		_cb1, cr, 1, 0

#define _hw_reg__usia_ifs		_cb1, sr, 1, 7
#define _hw_reg__usia_ifov		_cb1, sr, 1, 6
#define _hw_reg__usia_pf		_cb1, sr, 1, 5
#define _hw_reg__usia_dc		_cb1, sr, 1, 4
#define _hw_reg__usia_cnt		_cb1, sr, 4, 0


/*	USI used as SPI master with software clock
 */
#define _hw_def_spimaster_swclk0	_usia_spimaster_swclk, 129, usi0

/*	USI used as SPI master with counter0 overflow clock
 */
#define _hw_def_spimaster_c0clk0	_usia_spimaster_c0clk, 130, usi0


/*******************************************************************************
 *									       *
 *	Analog Comparator						       *
 *									       *
 *******************************************************************************/

/*	Instance			class, id, address
 */					
#include "../classes/acmpa_1.h"
#define _hw_def_acmp0			_acmpa, 131, 0

/*	Class registers			class, address, write mask, flags mask
 */					
#define _hw_reg__acmpa_csr		_r8, 0x28, 0xDF, 0x10

/*	Object registers		class, address, write mask, flags mask
 *
*	Required for _hw_counter1capture0_acic
 */
#define _hw_reg_acmp0_csr		_r8, 0x28, 0xDF, 0x10

#define _hw_reg_acmp0_acme		_xob1, adc0, srb,   1, 6
#define _hw_reg_acmp0_aden		_xob1, adc0, sra,   1, 7
#define _hw_reg_acmp0_admux		_xob1, adc0, admux, 6, 0


/*******************************************************************************
 *									       *
 *	Analog-to-Digital Converter					       *
 *									       *
 *******************************************************************************/

#include "../classes/ad10a_1.h"
#define _hw_def_adc0			_ad10a, 132, 0

#define _hw_reg__ad10a_admux		_r8,  0x27,   0xFF,   0x00
#define _hw_reg__ad10a_sra		_r8,  0x26,   0xFF,   0x10
#define _hw_reg__ad10a_adc		_r16, 0x24, 0x0000, 0x0000
#define _hw_reg__ad10a_adch		_r8,  0x25,   0x00,   0x00
#define _hw_reg__ad10a_adcl		_r8,  0x24,   0x00,   0x00
#define _hw_reg__ad10a_srb		_r8,  0x23,   0xD7,   0x00

#define _hw_reg_adc0_admux		_r8,  0x27,   0xFF,   0x00
#define _hw_reg_adc0_sra		_r8,  0x26,   0xFF,   0x10
#define _hw_reg_adc0_srb		_r8,  0x23,   0xD7,   0x00

#define _hw_reg__ad10a_refs		_cb1, admux, 2, 6
#define _hw_reg__ad10a_mux		_cb1, admux, 6, 0

#define _hw_reg__ad10a_en		_cb1, sra, 1, 7
#define _hw_reg__ad10a_sc		_cb1, sra, 1, 6
#define _hw_reg__ad10a_ate		_cb1, sra, 1, 5
#define _hw_reg__ad10a_if		_cb1, sra, 1, 4
#define _hw_reg__ad10a_ie		_cb1, sra, 1, 3
#define _hw_reg__ad10a_ps		_cb1, sra, 3, 0

#define _hw_reg__ad10a_bin		_cb1, srb, 1, 7
#define _hw_reg__ad10a_lar		_cb1, srb, 1, 4
#define _hw_reg__ad10a_ts		_cb1, srb, 3, 0


/*******************************************************************************
 *									       *
 *	EEPROM memory							       *
 *									       *
 *******************************************************************************/

/*	Class
 */
#define _hw_class__eeproma

/*	Object				class, id, address
 */
#define _hw_def_eeprom0			_eeproma, 133, 0

/*	Class regs			class, address, write mask, flags mask
 */
#define _hw_reg__eeproma_arh		_r8,  0x3F,   0x01, 0x00
#define _hw_reg__eeproma_arl		_r8,  0x3E,   0xFF, 0x00
#define _hw_reg__eeproma_ar		_r16, 0x3E, 0x01FF, 0x00
#define _hw_reg__eeproma_dr		_r8,  0x3D,   0xFF, 0x00
#define _hw_reg__eeproma_cr		_r8,  0x3C,   0x3F, 0x00

#define _hw_reg__eeproma_pm		_cb1, cr, 2, 4
#define _hw_reg__eeproma_rie		_cb1, cr, 1, 3
#define _hw_reg__eeproma_mpe		_cb1, cr, 1, 2
#define _hw_reg__eeproma_pe		_cb1, cr, 1, 1
#define _hw_reg__eeproma_re		_cb1, cr, 1, 0


/*******************************************************************************
 *									       *
 *	Flash memory							       *
 *									       *
 *******************************************************************************/

/*	Class
 */
#define _hw_class__flasha

/*	Object				class, id, address
 */
#define _hw_def_flash0			_flasha, 134, 0

/*	Class regs			class, address, write mask, flags mask
 */
#define _hw_reg__flasha_csr		_r8,  0x57, 0x3F, 0x00

#define _hw_reg__flasha_sigrd		_cb1, csr, 1, 5
#define _hw_reg__flasha_rsig		_cb1, csr, 1, 5	/* old definition */
#define _hw_reg__flasha_rwwsre		_cb1, csr, 1, 4
#define _hw_reg__flasha_ctpb		_cb1, csr, 1, 4	/* old definition */
#define _hw_reg__flasha_blbset		_cb1, csr, 1, 3
#define _hw_reg__flasha_rflb		_cb1, csr, 1, 3	/* old definition */
#define _hw_reg__flasha_pgwrt		_cb1, csr, 1, 2
#define _hw_reg__flasha_pgers		_cb1, csr, 1, 1
#define _hw_reg__flasha_spmen		_cb1, csr, 1, 0


/*******************************************************************************
 *									       *
 *	Software UART							       *
 *									       *
 *******************************************************************************/

#  define _hw_class__swuarta

/*	Object swuart0
 */
#if defined hw_swuart0_compare || defined hw_swuart0_clk_div || defined hw_swuart0_pin_txd || defined hw_swuart0_pin_rxd || defined hw_swuart0_starter || defined hw_swuart0_check_tx
#  if !defined hw_swuart0_compare
#    error hw_swuart0_compare is not defined
#  elif HW_ID(hw_swuart0_compare) == 0
#    error HWA: hw_swuart0_compare: invalid definition
#    undef hw_swuart0_compare
#  endif
#  if !defined hw_swuart0_clk_div
#    error hw_swuart0_clk_div is not defined
#    undef hw_swuart0_compare
#  endif
#  if defined hw_swuart0_pin_txd && HW_ID(hw_swuart0_pin_txd)==0
#    error invalid definition of hw_swuart0_pin_txd
#    undef hw_swuart0_compare
#  endif
#  if defined hw_swuart0_pin_rxd && HW_ID(hw_swuart0_pin_rxd)==0
#    error invalid definition of hw_swuart0_pin_rxd
#    undef hw_swuart0_compare
#  endif
#  if defined hw_swuart0_pin_rxd && !defined hw_swuart0_starter
#    warning hw_swuart0_starter is not defined, using default 'pcic'
#    undef hw_swuart0_compare
#  endif
#endif

#if defined hw_swuart0_compare
#  define _hw_rel_swuart0_compare	hw_swuart0_compare
#  define _hw_def_swuart0		_swuarta, 137, 0
#  define _hw_reg_swuart0_dtn		_HW_RC(_HW_RV(hw_swuart0_compare,counter),count), (intptr_t)&__hw_swuart0_dtn, -1, 0x00
#  define _hw_reg_swuart0_dt0		_HW_RC(_HW_RV(hw_swuart0_compare,counter),count), (intptr_t)&__hw_swuart0_dt0, -1, 0x00
#  define _hw_reg_swuart0__st		_HW_A(_HW_M(shared, gpior0))
#  define _hw_reg_swuart0_sr		_hw_reg_shared_gpior0
#  define _hw_reg_swuart0_synced	_ob1, sr, 1, 0
#endif


/*	Object swuart1
 */
#if defined hw_swuart1_compare || defined hw_swuart1_clk_div || defined hw_swuart1_pin_txd || defined hw_swuart1_pin_rxd || defined hw_swuart1_starter || defined hw_swuart1_check_tx
#  if !defined hw_swuart1_compare
#    error hw_swuart1_compare is not defined
#  elif HW_ID(hw_swuart1_compare) == 0
#    error HWA: hw_swuart1_compare: invalid definition
#    undef hw_swuart1_compare
#  endif
#  if !defined hw_swuart1_clk_div
#    error hw_swuart1_clk_div is not defined
#    undef hw_swuart1_compare
#  endif
#  if defined hw_swuart1_pin_txd && HW_ID(hw_swuart1_pin_txd)==0
#    error invalid definition of hw_swuart1_pin_txd
#    undef hw_swuart1_compare
#  endif
#  if defined hw_swuart1_pin_rxd && HW_ID(hw_swuart1_pin_rxd)==0
#    error invalid definition of hw_swuart1_pin_rxd
#    undef hw_swuart1_compare
#  endif
#  if defined hw_swuart1_pin_rxd && !defined hw_swuart1_starter
#    warning hw_swuart1_starter is not defined, using default 'pcic'
#    undef hw_swuart1_compare
#  endif
#endif

#if defined hw_swuart1_compare
#  define _hw_rel_swuart1_compare	hw_swuart1_compare
#  define _hw_def_swuart1		_swuarta, 138, 0
#  define _hw_reg_swuart1_dtn		_HW_RC(_HW_RV(hw_swuart1_compare,counter),count), (intptr_t)&__hw_swuart1_dtn, -1, 0x00
#  define _hw_reg_swuart1_dt0		_HW_RC(_HW_RV(hw_swuart1_compare,counter),count), (intptr_t)&__hw_swuart1_dt0, -1, 0x00
#  define _hw_reg_swuart1__st		_HW_A(_HW_M(shared, gpior1))
#  define _hw_reg_swuart1_sr		_hw_reg_shared_gpior1
#  define _hw_reg_swuart1_synced	_ob1, sr, 1, 0
#endif


/**
 * @page attinyx4
 *
 * @section attinyx4_pwr Power Management
 *
 * The following peripherals can be powered on/off with the `power` action to
 * manage power consumption of the device:
 *
 * * `counter0`
 * * `counter1`
 * * `usi0`
 * * `adc0`
 * * `acmp0`
 *
 * @code
 * hw( power, counter0, off );
 * @endcode
 */

#if !defined __ASSEMBLER__

/**
 * @brief The HWA context
 *
 * This structure is instanciated by hwa_begin() or hwa_begin_from_reset() and
 * used by all HWA asynchronous instructions to bufferize hardware accesses.
 */
typedef struct {
  uint8_t	commit ;	/*!< 1 if commit does write into hardware registers	*/

  hwa_shared_t	shared ;

  hwa_corea_t	core0 ;
  hwa_p8a_t	port0 ;
  hwa_p8a_t	port1 ;
  hwa_pcica_t	pcic0 ;
  hwa_pcica_t	pcic1 ;
  hwa_wdoga_t	watchdog0 ;

  hwa_cmp8a_t	counter0compare0 ;
  hwa_cmp8a_t	counter0compare1 ;
  hwa_c8a_t	counter0 ;

  hwa_cmp16a_t	counter1compare0 ;
  hwa_cmp16a_t	counter1compare1 ;
  hwa_cap16a_t	counter1capture0 ;
  hwa_c16a_t	counter1 ;

  hwa_usia_t	usi0 ;
  hwa_acmpa_t	acmp0 ;
  hwa_ad10a_t	adc0 ;
} hwa_t ;

#include "../hwa_2.h"
#include "../classes/corea_2.h"
#include "../classes/io1a_2.h"
#include "../classes/p8a_2.h"
#include "../classes/pcica_2.h"
#include "../classes/wdoga_2.h"
#include "../classes/psca_2.h"
#include "../classes/cmp8a_2.h"
#include "../classes/c8a_2.h"
#include "../classes/cmp16a_2.h"
#include "../classes/cap16a_2.h"
#include "../classes/c16a_2.h"
#include "../classes/usia_2.h"
#include "../classes/acmpa_2.h"
#include "../classes/ad10a_2.h"
#include "../classes/eeproma_2.h"
#include "../classes/flasha_2.h"
#include "../classes/swuarta_2.h"

HW_INLINE void _hwa_setup_context( hwa_t *hwa )
{
  _hwa_setup_reg( shared, gimsk );
  _hwa_setup_reg( shared, gifr	);
  _hwa_setup_reg( shared, prr	);
  _hwa_setup_reg( shared, gtccr );
  _hwa_setup_reg( shared, timsk );
  _hwa_setup_reg( shared, tifr	);
  _hwa_setup_reg( shared, did	);

  _hwa_setup( core0 );
  _hwa_setup( port0 );
  _hwa_setup( port1 );
  _hwa_setup( pcic0 );
  _hwa_setup( pcic1 );
  _hwa_setup( watchdog0 );

  _hwa_setup( counter0compare0 );
  _hwa_setup( counter0compare1 );
  _hwa_setup( counter0 );

  _hwa_setup( counter1compare0 );
  _hwa_setup( counter1compare1 );
  _hwa_setup( counter1capture0 );
  _hwa_setup( counter1 );

  _hwa_setup( usi0 );
  _hwa_setup( adc0 );

  _hwa_setup( acmp0 );
}


HW_INLINE void _hwa_init_context( hwa_t *hwa )
{
  _hwa_init_reg( shared, gimsk, 0 );
  _hwa_init_reg( shared, gifr,	0 );
  _hwa_init_reg( shared, prr,	0 );
  _hwa_init_reg( shared, gtccr, 0 );
  _hwa_init_reg( shared, timsk, 0 );
  _hwa_init_reg( shared, tifr,	0 );
  _hwa_init_reg( shared, did,	0 );

  _hwa_init( core0 );

  _hwa_init( port0 );
  _hwa_init( port1 );
  _hwa_init( pcic0 );
  _hwa_init( pcic1 );
  _hwa_init( watchdog0 );

  _hwa_init( counter0compare0 );
  _hwa_init( counter0compare1 );
  _hwa_init( counter0 );

  _hwa_init( counter1compare0 );
  _hwa_init( counter1compare1 );
  _hwa_init( counter1capture0 );
  _hwa_init( counter1 );

  _hwa_init( usi0 );
  _hwa_init( adc0 );
  _hwa_init( acmp0 );
}


HW_INLINE void _hwa_commit_context( hwa_t *hwa )
{
  /*  Solve the configuration of the counters
   *
   *  The configuration values are written here since the solve function only
   *  has the address of the counter and does not know its name.
   */
  _hwa_solve( counter0 );
  _hwa_solve( counter1 );

  _hwa_commit_reg( shared, gimsk );
  _hwa_commit_reg( shared, gifr	 );
  _hwa_commit( core0 );
  _hwa_commit_reg( shared, prr	 );

  _hwa_commit( watchdog0 );
  _hwa_commit( port0 );
  _hwa_commit( port1 );

  _hwa_commit( pcic0 );
  _hwa_commit( pcic1 );

  _hwa_commit_reg( shared, gtccr );
  _hwa_commit( counter0 );
  _hwa_commit( counter0compare0 );
  _hwa_commit( counter0compare1 );

  _hwa_commit( counter1 );
  _hwa_commit( counter1compare0 );
  _hwa_commit( counter1compare1 );
  _hwa_commit( counter1capture0 );

  _hwa_commit( usi0 );
  _hwa_commit( adc0 );
  _hwa_commit_reg( shared, did );
  _hwa_commit( acmp0 );
}

#endif /* !defined __ASSEMBLER__ */

/**
 * @page attinyx4
 * <br>
 */
