
/*  This file is part of the HWA project.
 *  Copyright (c) 2012,2015 Christophe Duparquet.
 *  All rights reserved. Read LICENSE.TXT for details.
 */

/**
 * @file
 * @brief I/O (not connected).
 */

/**
 * @page atmelavr_io0a Class _io0a: General Purpose Input/Output
 *
 * A class `_io0a` object is a not connected pin.
 */
#define _hw_class__io0a


#define _hw_mtd_HW_BITS__io0a		, _hw_bn_io0a
#define _hw_bn_io0a(o,i, cn,bn,bp,...)	       0


#define _hw_mtd_HW_RELATIVE__io0a		, _HW_REL_io0a
#define _HW_REL_io0a(o,...)		HW_E(`o` has no relatives)
