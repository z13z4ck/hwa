
/* This file is part of the HWA project.
 * Copyright (c) 2012,2015 Christophe Duparquet.
 * All rights reserved. Read LICENSE.TXT for details.
 */

/**
 * @file
 * @brief 8-bit compare unit
 */

/**
 * @page atmelavr_cmp8b Class _cmp8b: 8-bit compare unit with waveform generator
 *
 * A class `_cmp8b` object is an 8-bit compare unit with waveform generator that
 * drives two complementary outputs. It has the following relative objects:
 *
 *  * one counter, whose name is given by `HW_RELATIVE( compare0, counter )`
 *
 *  * one output pin, whose name is given by `HW_RELATIVE( compare0, pin_h )`
 *
 *  * one complementary output pin, whose name is given by `HW_RELATIVE( compare0, pin_l )`
 *
 * It is used in:
 *
 * * @ref attinyx5 : `counter1compare0`, `counter1compare1`
 */
#define _hw_class__cmp8b


/**
 * @page atmelavr_cmp8b
 * @par Instructions that do not produce C code
 *
 * The `HW_BITS()` instruction returns the number of bits of the compare
 * register:
 *
 * @code
 * #if HW_BITS( compare0 ) != 8
 * #  error A 8-bit compare unit is required!
 * #endif
 * @endcode
 */
#define _hw_mtd_HW_BITS__cmp8b		, _hw_bn_cmp8b
#define _hw_bn_cmp8b(o,i,a,...)			8


/**
 * @page atmelavr_cmp8b
 * @par Interrupts
 *
 * The compare unit can trigger an @ref using_interrupts "anonymous IRQ" when
 * a compare event occurs:
 *
 * @code
 * HW_ISR( compare0 )
 * {
 *    // Process compare event
 * }
 * @endcode
 */


#if !defined __ASSEMBLER__

typedef struct {

  /*  Hardware registers
   */
  hwa_r8_t	reg ;

  /*  Registers used for high-level configuration
   */
  struct {
    uint8_t	outputh, outputl ;
  } config ;

} hwa_cmp8b_t ;

#endif
