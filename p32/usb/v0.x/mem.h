/*******************************************************************************
	Title:	USB Pinguino Bootloader
	File:	mem.h
	Descr.: memory regions description (bootloader and application)
	Author:	Régis Blanchot <rblanchot@gmail.com>

	This file is part of Pinguino (http://www.pinguino.cc)
	Released under the LGPL license (http://www.gnu.org/licenses/lgpl.html)
*******************************************************************************/

#ifndef _MEM_H_
#define _MEM_H_

#include "p32xxxx.h"                    // Registers definitions

#define FLASH_MEM_START                 0x9D000000
#define DATA_MEM_START                  0xA0000000
#define KSEG1_BOOT_MEM_START            0xBFC00000
#define KSEG0_BOOT_MEM_START            0x9FC00000
#define STARTUP_MEM_LENGTH              0x200

// The Flash page size is
// - 1 KB on PIC32MX-1XX/2XX devices
// - 4 KB on PIC32MX-3XX/7XX devices

#if defined(__32MX220F032B__) || defined(__32MX250F128B__) || defined(__32MX270F256B__)
#define FLASH_PAGE_SIZE                 0x400
#define KSEG_BOOT_MEM_LENGTH            0xBF0
#else
#define FLASH_PAGE_SIZE                 0x1000
#define KSEG_BOOT_MEM_LENGTH            0x2FF0
#endif

// PIC32MX270F256B issues
// - BMXPFMSZ returns 512K instead of 256K
// - BMXDRMSZ returns 128K instead of 64K

#if defined(__32MX270F256B__)
#define FLASH_TOTAL_LENGTH              (BMXPFMSZ/2)
#define DATA_MEM_LENGTH                 (BMXDRMSZ/2)
#else
#define FLASH_TOTAL_LENGTH              BMXPFMSZ
#define DATA_MEM_LENGTH                 BMXDRMSZ
#endif

/**********************************************************************
 * BOOTLOADER and APPLICATION (ex. PIC32MX270F256B)
 * IVT = from 0x9D000000 to 0x9D001000
 **********************************************************************/

#define BOOT_EBASE_ADDR                 FLASH_MEM_START
#define BOOT_GEN_EXCPT_ADDR             (BOOT_EBASE_ADDR + 0x180)
#define BOOT_EXCEPTION_MEM              BOOT_EBASE_ADDR
#define BOOT_EXCEPTION_MEM_LENGTH       0x1000

#define APP_EBASE_ADDR                  FLASH_MEM_START
#define APP_GEN_EXCPT_ADDR              (APP_EBASE_ADDR + 0x180)
#define APP_EXCEPTION_MEM               APP_EBASE_ADDR
#define APP_EXCEPTION_MEM_LENGTH        0x1000

/**********************************************************************
 * BOOTLOADER (ex. PIC32MX270F256B)
 **********************************************************************/

#define BOOT_PROGRAM_LENGTH             0x2800  // 10K

// from 0x9D001000 to 0x9D003400
#define BOOT_KSEG0_PROGRAM_MEM          (BOOT_EXCEPTION_MEM + BOOT_EXCEPTION_MEM_LENGTH)
#define BOOT_KSEG0_PROGRAM_MEM_LENGTH   BOOT_PROGRAM_LENGTH
// from 0xBFC00000 to 0xBFC00010
#define BOOT_RESET_ADDR                 KSEG1_BOOT_MEM_START
#define BOOT_KSEG1_BOOT_MEM             BOOT_RESET_ADDR
#define BOOT_KSEG1_BOOT_MEM_LENGTH      0x10
// from 0x9FC00010 to 0x9FC00210
#define BOOT_KSEG0_BOOT_MEM             (KSEG0_BOOT_MEM_START + BOOT_KSEG1_BOOT_MEM_LENGTH)
#define BOOT_KSEG0_BOOT_MEM_LENGTH      STARTUP_MEM_LENGTH

/**********************************************************************
 * APPLICATION (ex. PIC32MX270F256B)
 **********************************************************************/

// from 0x9D003400 to 0x9D040000
#define APP_KSEG0_PROGRAM_MEM           (BOOT_KSEG0_PROGRAM_MEM + BOOT_KSEG0_PROGRAM_MEM_LENGTH)
#define APP_KSEG0_PROGRAM_MEM_LENGTH    (FLASH_MEM_START + FLASH_TOTAL_LENGTH - APP_KSEG0_PROGRAM_MEM)

#define APP_PROGRAM_ADDR_START          APP_KSEG0_PROGRAM_MEM
#define APP_PROGRAM_LENGTH              APP_KSEG0_PROGRAM_MEM_LENGTH
#define APP_PROGRAM_ADDR_END            (FLASH_MEM_START + FLASH_TOTAL_LENGTH)

#define APP_RESET_ADDR                  APP_KSEG0_PROGRAM_MEM
#define APP_KSEG1_BOOT_MEM              APP_RESET_ADDR
#define APP_KSEG1_BOOT_MEM_LENGTH       0x10

#define APP_KSEG0_BOOT_MEM              (APP_KSEG1_BOOT_MEM + APP_KSEG1_BOOT_MEM_LENGTH)
#define APP_KSEG0_BOOT_MEM_LENGTH       STARTUP_MEM_LENGTH

/*
// from 0xBFC00400 to 0xBFC00410
#define APP_RESET_ADDR                  (KSEG1_BOOT_MEM_START + FLASH_PAGE_SIZE)
#define APP_KSEG1_BOOT_MEM              APP_RESET_ADDR
#define APP_KSEG1_BOOT_MEM_LENGTH       0x10
// from 0x9FC00410 to 0x9FC00610
#define APP_KSEG0_BOOT_MEM              (KSEG0_BOOT_MEM_START + FLASH_PAGE_SIZE + APP_KSEG1_BOOT_MEM_LENGTH)
#define APP_KSEG0_BOOT_MEM_LENGTH       STARTUP_MEM_LENGTH
*/

#endif /* _MEM_H_ */
