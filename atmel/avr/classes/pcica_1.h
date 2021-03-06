
/* This file is part of the HWA project.
 * Copyright (c) 2012,2015 Christophe Duparquet.
 * All rights reserved. Read LICENSE.TXT for details.
 */

/**
 * @file
 * @brief Pin change interrupt controller
 */

/**
 * @page atmelavr_pcica Class _pcica: pin change interrupt controller
 *
 * A class `_pcica` object is a pin change interrupt controller.
 *
 * It is used in:
 *
 * * @ref attinyx4 : `pcic0`, `pcic1`
 * * @ref attinyx5 : `pcic0`
 * * @ref atmegax8 : `pcic0`, `pcic1`, `pcic2`
 */
#define _hw_class__pcica


/**
 * @page atmelavr_pcica
 * @par Interrupts
 *
 * Class `_pcica` objects can trigger the following IRQs as soon as the state of
 * one of its monitored pins changes:
 *
 *  * `pcic0`: monitored pin change event
 */


#if !defined __ASSEMBLER__

typedef struct {
  hwa_r8_t	msk ;
} hwa_pcica_t ;

#endif
