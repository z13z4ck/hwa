
/* This file is part of the HWA project.
 * Copyright (c) Christophe Duparquet <duparq at free dot fr>
 * All rights reserved. Read LICENSE.TXT for details.
 */

#error

#define HW_DEVICE_ATTINY85
#define HW_DEVICE_SIGNATURE				0x1E,0x93,0x0B
#define HW_DEVICE_FLASH_SIZE				8192
#define HW_DEVICE_FLASH_PAGE_SIZE			64
#define HW_DEVICE_EEPROM_SIZE				512
#define HW_DEVICE_EEPROM_PAGE_SIZE			4
#define HW_DEVICE_RAM_SIZE				512
#define HW_DEVICE_RAM_START				0x0060
#define HW_DEVICE_APP_START				0x001E

#include "attinyx5_1.h"