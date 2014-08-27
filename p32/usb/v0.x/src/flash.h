/*******************************************************************************
	Title:	USB Pinguino Bootloader
	File:	flash.c
	Descr.: flash operations for supported PIC32MX
	Author:	Régis Blanchot <rblanchot@gmail.com>

	This file is part of Pinguino (http://www.pinguino.cc)
	Released under the LGPL license (http://www.gnu.org/licenses/lgpl.html)
*******************************************************************************/

#ifndef FLASH_H
#define FLASH_H

#include <GenericTypeDefs.h>

#define PAGE_SIZE               1024        // Number of 32-bit Instructions per Page
#define ROW_SIZE                128         // Number of 32-bit Instructions per Row
#define NUM_ROWS_PAGE           8           // Number of Rows per Page

// Pinguino 32MX220 and Pinguino 32MX250
#if defined(__32MX220F032B__) || defined(__32MX250F128B__)
    #define BYTE_PAGE_SIZE       PAGE_SIZE // Page size in Bytes
    #define BYTE_ROW_SIZE        ROW_SIZE // # Row size in Bytes
#else
    #define BYTE_PAGE_SIZE       (4 * PAGE_SIZE) // Page size in Bytes
    #define BYTE_ROW_SIZE        (4 * ROW_SIZE) // # Row size in Bytes
#endif

#define FLASH_PAGE_SIZE         BYTE_PAGE_SIZE

#define NVMOP_WORD_PGM          0x4001      // Word program operation
#define NVMOP_PAGE_ERASE        0x4004      // Page erase operation
#define NVMOP_ROW_PGM           0x4003      // Row program operation
#define NVMOP_NOP               0x4000      // NOP operation

UINT FlashWriteWord(void* address, UINT data);
UINT FlashErasePage(void* address);

#if 0
extern UINT FlashWriteRow(void* address, void* data);
extern UINT FlashClearError(void);
#endif

// Convert a virtual address to a physical address
#define KVA_TO_PA(v)            ((unsigned long)(v) & 0x1fffffff)
//#define PA_TO_KVA0(pa)          ((void *) ((pa) | 0x80000000))
#define PA_TO_KVA1(pa)          ((void *) ((pa) | 0xA0000000))

#endif /* FLASH_H */
