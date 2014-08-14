
/* This file is part of the HWA project.
 * Copyright (c) Christophe Duparquet <duparq at free dot fr>
 * All rights reserved. Read LICENSE.TXT for details.
 */

#include "hwa_atmelavr_counters_1.h"
#include "hwa_atmelavr_c8a_1.h"
#include "hwa_atmelavr_c16a_1.h"


/*	Instance			class, name, id, address
 */
#define hw_counter0			c8a, counter0, 400, 0

/*	Interrupts			class, vector, controller, enable, flag
 */
#define hw_irq_counter0_compare_a	irq,  9, counter0, ociea, ocfa
#define hw_irq_counter0_compare_b	irq, 10, counter0, ocieb, ocfb
#define hw_irq_counter0_overflow	irq, 11, counter0, oie, ov

/*	Class registers			class, rw, ra, rwm, rfm
 */
#define hw_c8a_ocrb			crg, 8, 0x5C, 0xFF, 0x00
#define hw_c8a_compare_b		crg, 8, 0x5C, 0xFF, 0x00
#define hw_c8a_imsk			crg, 8, 0x59, 0x07, 0x00
#define hw_c8a_ifr			crg, 8, 0x58, 0x07, 0x07
#define hw_c8a_ocra			crg, 8, 0x56, 0xFF, 0x00
#define hw_c8a_compare_a		crg, 8, 0x56, 0xFF, 0x00
#define hw_c8a_ccrb			crg, 8, 0x53, 0xCF, 0x00
#define hw_c8a_count			crg, 8, 0x52, 0xFF, 0x00
#define hw_c8a_ccra			crg, 8, 0x50, 0xF3, 0x00

#define hw_c8a_coma			cb1, ccra, 2, 6
#define hw_c8a_comb			cb1, ccra, 2, 4
#define hw_c8a_wgm			cb2, ccra, 2, 0, 0, ccrb, 1, 3, 2

#define hw_c8a_foca			cb1, ccrb, 1, 7
#define hw_c8a_focb			cb1, ccrb, 1, 6
#define hw_c8a_cs			cb1, ccrb, 3, 0

#define hw_c8a_ocieb			cb1, imsk, 1, 2
#define hw_c8a_ociea			cb1, imsk, 1, 1
#define hw_c8a_oie			cb1, imsk, 1, 0

#define hw_c8a_ocfb			cb1, ifr, 1, 2
#define hw_c8a_ocfa			cb1, ifr, 1, 1
#define hw_c8a_ov			cb1, ifr, 1, 0


/*	Counter0 compare units
 *
 *	Instance			class, name, id, counter, ocn, ion
 */
#define hw_counter0_compare_a		ocu, counter0_compare_a, 401, counter0, a, pin_pb2
#define hw_counter0_compare_b		ocu, counter0_compare_b, 402, counter0, b, pin_pa7


/*	Instance			class, name, id, address
 */
#define hw_counter1			c16a, counter1, 410, 0

/*	Interrupts			class, vector, controller, enable, flag
 */
#define hw_irq_counter1_capture		irq, 5, counter1, icie, icf
#define hw_irq_counter1_compare_a	irq, 6, counter1, ociea, ocfa
#define hw_irq_counter1_compare_b	irq, 7, counter1, ocieb, ocfb
#define hw_irq_counter1_overflow	irq, 8, counter1, oie, ov

/*	Class registers			class, rw, ra, rwm, rfm
 */
#define hw_c16a_ccra			crg,  8, 0x4F, 0xF3,   0x00
#define hw_c16a_ccrb			crg,  8, 0x4E, 0xDF,   0x00
#define hw_c16a_ccrc			crg,  8, 0x42, 0xC0,   0x00
#define hw_c16a_count			crg, 16, 0x4C, 0xFFFF, 0x0000
#define hw_c16a_countl			crg,  8, 0x4C, 0xFF,   0x00
#define hw_c16a_counth			crg,  8, 0x4D, 0xFF,   0x00
#define hw_c16a_ocra			crg, 16, 0x4A, 0xFFFF, 0x0000
#define hw_c16a_compare_a		crg, 16, 0x4A, 0xFFFF, 0x0000
#define hw_c16a_ocral			crg,  8, 0x4A, 0xFF,   0x00
#define hw_c16a_ocrah			crg,  8, 0x4B, 0xFF,   0x00
#define hw_c16a_ocrb			crg, 16, 0x48, 0xFFFF, 0x0000
#define hw_c16a_compare_b		crg, 16, 0x48, 0xFFFF, 0x0000
#define hw_c16a_ocrbl			crg,  8, 0x48, 0xFF,   0x00
#define hw_c16a_ocrbh			crg,  8, 0x49, 0xFF,   0x00
#define hw_c16a_icr			crg, 16, 0x44, 0xFFFF, 0x0000
#define hw_c16a_capture			crg, 16, 0x44, 0xFFFF, 0x0000
#define hw_c16a_icrl			crg,  8, 0x44, 0xFF,   0x00
#define hw_c16a_icrh			crg,  8, 0x45, 0xFF,   0x00
#define hw_c16a_imsk			crg,  8, 0x2C, 0x27,   0x00
#define hw_c16a_ifr			crg,  8, 0x2B, 0x27,   0x27

#define hw_c16a_acic			irg, hw_acmp0, acic

#define hw_c16a_coma			cb1, ccra, 2, 6
#define hw_c16a_comb			cb1, ccra, 2, 4

#define hw_c16a_icnc			cb1, ccrb, 1, 7
#define hw_c16a_ices			cb1, ccrb, 1, 6
#define hw_c16a_cs			cb1, ccrb, 3, 0
#define hw_c16a_wgm			cb2, ccrb, 2, 3, 2, ccra, 2, 0, 0

#define hw_c16a_foca			cb1, ccrc, 1, 7
#define hw_c16a_focb			cb1, ccrc, 1, 6

#define hw_c16a_icie			cb1, imsk, 1, 5
#define hw_c16a_ocieb			cb1, imsk, 1, 2
#define hw_c16a_ociea			cb1, imsk, 1, 1
#define hw_c16a_oie			cb1, imsk, 1, 0

#define hw_c16a_icf			cb1, ifr, 1, 5
#define hw_c16a_ocfb			cb1, ifr, 1, 2
#define hw_c16a_ocfa			cb1, ifr, 1, 1
#define hw_c16a_ov			cb1, ifr, 1, 0


/*	Counter1 compare units	class, name, id, counter, channel, io
 */
#define hw_counter1_compare_a	ocu, counter1_compare_a, 411, counter1, a, pin_pa6 /* dil#7 */
#define hw_counter1_compare_b	ocu, counter1_compare_b, 412, counter1, b, pin_pa5 /* dil#8 */


/*	Counter1 capture unit		class, name, id, counter, reg, io
 */
#define hw_counter1_capture	icu, counter1_capture, 413, counter1, icr, pin_pa7 /* dil#6 */


#define HWA_DCL_COUNTERS			\
  hwa_c8a_t	counter0 ;			\
  hwa_c16a_t	counter1
