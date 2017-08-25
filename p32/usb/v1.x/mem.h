/***********************************************************************
	Title:	USB Pinguino Bootloader
	File:	mem.h
	Descr.: memory regions description (bootloader and application)
	Author:	Régis Blanchot <rblanchot@gmail.com>

	This file is part of Pinguino (http://www.pinguino.cc)
	Released under the LGPL license (http://www.gnu.org/licenses/lgpl.html)
***********************************************************************/

#ifndef _MEM_H_
#define _MEM_H_

#include "p32xxxx.h"                    // Registers definitions
#include "flash.h"

// Cacheable memory is in KSEG0
#define KSEG0_FLASH_MEM_START           0x9D000000
//#define KSEG0_BOOT_MEM_START            0x9FC00000

// Non-Cacheable memory is in KSEG1
//#define KSEG1_FLASH_MEM_START           0xBD000000
//#define KSEG1_BOOT_MEM_START            0xBFC00000

// RAM (Vitual)
//#define DATA_MEM_START                  0xA0000000

#define RESET_VECTOR_MEM_LENGTH         0x10            // RESET VECTOR ONLY
#define STARTUP_MEM_LENGTH              0x200
#define IVT_MEM_LENGTH                  0x1000          // 0x200 + 64 vectors * 32 bytes

/*
#if defined(__32MX220F032B__) || \
    defined(__32MX250F128B__) || \
    defined(__32MX270F256B__)
*/

#if defined(__PIC32MX2__)

#define KSEG_BOOT_MEM_LENGTH            0xBF0

#else

#define KSEG_BOOT_MEM_LENGTH            0x2FF0

#endif

// PIC32MX270F256B issues
// - BMXPFMSZ returns 512K instead of 256K
// - BMXDRMSZ returns 128K instead of 64K

#if defined(__32MX270F256B__)

#define DATA_MEM_LENGTH                 (BMXDRMSZ/2)

#else

#define DATA_MEM_LENGTH                 BMXDRMSZ

#endif

#if defined(__32MX270F256B__) || defined(__32MX470F512H__)

#define BOOT_PROGRAM_LENGTH             0x2000  // 8K

// For compatibility with Microchip bootloader version (cf. interrupt.c)
#elif defined(__32MX220F032B__) || defined(__32MX250F128B__)

#define BOOT_PROGRAM_LENGTH             0x3000  // 12K

// For compatibility with old bootloader version (cf. interrupt.c)
#else //defined(__32MX440F256H__)

#define BOOT_PROGRAM_LENGTH             0x5000  // 20K

#endif

#define APP_EBASE_ADDR                  (KSEG0_FLASH_MEM_START + BOOT_PROGRAM_LENGTH)
#define APP_RESET_ADDR                  (APP_EBASE_ADDR + IVT_MEM_LENGTH)
#define APP_PROGRAM_ADDR_START          (APP_RESET_ADDR + RESET_VECTOR_MEM_LENGTH)
#define APP_PROGRAM_ADDR_END            FLASH_MEM_END
#define APP_PROGRAM_LENGTH              (APP_PROGRAM_ADDR_END - APP_PROGRAM_ADDR_START)

#endif /* _MEM_H_ */





/**********************************************************************/
#if 0
/**********************************************************************
 * BOOTLOADER and APPLICATION
 **********************************************************************/

// IVT = EXCEPTION_MEM in KSEG0_FLASH_MEM_START (4K aligned)
#define EXCEPTION_MEM                   KSEG0_FLASH_MEM_START

/**********************************************************************
 * BOOTLOADER
 **********************************************************************/

// RESET VECTOR must be in KSEG1 (non-cacheable)
#define BOOT_RESET_VECTOR_MEM           KSEG1_BOOT_MEM_START
// STARTUP SEQUENCE must be in KSEG0 (cacheable)
#define BOOT_STARTUP_MEM                (KSEG0_BOOT_MEM_START + RESET_VECTOR_MEM_LENGTH)
// BOOTLOADER CODE after STARTUP SEQUENCE in KSEG0
#define BOOT_PROGRAM_MEM                (KSEG0_FLASH_MEM_START + IVT_MEM_LENGTH)

/**********************************************************************
 * APPLICATION
 **********************************************************************/

// RESET VECTOR must be in KSEG1 (non-cacheable) after BOOTLOADER CODE in KSEG1
#define APP_RESET_VECTOR_MEM            (BOOT_PROGRAM_MEM + BOOT_PROGRAM_LENGTH)
// STARTUP SEQUENCE must be in KSEG0 (cacheable)
#define APP_STARTUP_MEM                 (APP_RESET_VECTOR_MEM + RESET_VECTOR_MEM_LENGTH)
// APPLICATION CODE after STARTUP SEQUENCE in KSEG0
#define APP_PROGRAM_MEM                 (APP_STARTUP_MEM  + STARTUP_MEM_LENGTH)

/**********************************************************************
 * OTHERS
 **********************************************************************/

#define EBASE_ADDR                      EXCEPTION_MEM
#define BOOT_RESET_ADDR                 BOOT_RESET_VECTOR_MEM
#define APP_RESET_ADDR                  APP_RESET_VECTOR_MEM
#define APP_PROGRAM_ADDR_START          APP_PROGRAM_MEM
#define APP_PROGRAM_ADDR_END            FLASH_MEM_END
#define APP_PROGRAM_LENGTH              (APP_PROGRAM_ADDR_END - APP_PROGRAM_ADDR_START)

#endif /* 0 */
/**********************************************************************/
