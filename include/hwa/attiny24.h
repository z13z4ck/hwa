
#ifndef HWA_H
#define HWA_H

/* This file is part of the HWA project.
 * Copyright (c) 2012,2015 Christophe Duparquet.
 * All rights reserved. Read LICENSE.TXT for details.
 */

/**
 * @file
 * @brief Atmel AVR ATtiny24
 */

/**
 * @page attiny24 ATtiny24
 * @section attiny24_symbols Defined symbols
 *
 * Symbol		       | Value
 * :---------------------------|:------------
 * `HW_DEVICE`                 |attiny24
 * `HW_DEVICE_ATTINY24`        |Void.
 * `HW_DEVICE_SIGNATURE`       |0x1E,0x92,0x0B
 * `HW_DEVICE_FLASH_SIZE`      |2048
 * `HW_DEVICE_FLASH_PAGE_SIZE` |32
 * `HW_DEVICE_EEPROM_SIZE`     |128
 * `HW_DEVICE_EEPROM_PAGE_SIZE`|4
 * `HW_DEVICE_RAM_SIZE`	       |128
 *
 * See also:
 * * @subpage attiny24a_pu
 * * @ref attinyx4
 */

#define HW_DEVICE			attiny24
#define HW_DEVICE_ATTINY24
#define HW_DEVICE_SIGNATURE		0x1E,0x91,0x0B
#define HW_DEVICE_FLASH_SIZE		2048
#define HW_DEVICE_FLASH_PAGE_SIZE	32
#define HW_DEVICE_EEPROM_SIZE		128
#define HW_DEVICE_EEPROM_PAGE_SIZE	4
#define HW_DEVICE_RAM_SIZE		128

#include "../../atmel/avr/devices/attinyx4.h"

#endif
