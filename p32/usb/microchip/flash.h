/*******************************************************************************
	Title:	USB Pinguino Bootloader
	File:	flash.h
	Descr.: flash operations for supported PIC32MX
	Author:	Régis Blanchot <rblanchot@gmail.com>

	This file is part of Pinguino (http://www.pinguino.cc)
	Released under the LGPL license (http://www.gnu.org/licenses/lgpl.html)
*******************************************************************************/

#ifndef FLASH_H
#define FLASH_H

#include <GenericTypeDefs.h>

#define FLASH_NOP               0x4000      // NOP operation
#define FLASH_WORD_WRITE        0x4001      // Word program operation
#define FLASH_ROW_WRITE         0x4003      // Row program operation
#define FLASH_PAGE_ERASE        0x4004      // Page erase operation
#define FLASH_ALL_ERASE         0x4005      // Program Flash Memory erase operation

#if 0
unsigned int FlashOperation(unsigned int operation);
unsigned int FlashWriteWord(void* address, unsigned int data);
unsigned int FlashErasePage(void* address);
unsigned int FlashWriteRow(void* address, void* data);
unsigned int FlashClearError(void);
#endif

/*******************************************************************
 * To translate the kernel address (KSEG0 or KSEG1) to a physical address,
 * perform a "Bitwise AND" operation of the virtual address with 0x1FFFFFFF:
 * Physical Address = Virtual Address and 0x1FFFFFFF
 *******************************************************************/

#define KVA_TO_PA(kva)      ( (UINT32) (kva) & 0x1FFFFFFF )

/*******************************************************************
 * For physical address to KSEG0 virtual address translation,
 * perform a "Bitwise OR" operation of the physical address with 0x80000000:
 * KSEG0 Virtual Address = Physical Address | 0x80000000
 *******************************************************************/

#define PA_TO_KVA0(pa)      ( (void*) ((pa)  | 0x80000000) )

/*******************************************************************
 * For physical address to KSEG1 virtual address translation,
 * perform a "Bitwise OR" operation of the physical address with 0xA0000000:
 * KSEG1 Virtual Address = Physical Address | 0xA0000000
 ******************************************************************/

#define PA_TO_KVA1(pa)      ( (void*) ((pa)  | 0xA0000000) )

/*******************************************************************
 * To translate from KSEG0 to KSEG1 virtual address,
 * perform a "Bitwise OR" operation of the KSEG0 virtual address with 0x20000000:
 * KSEG1 Virtual Address = KSEG0 Virtual Address | 0x20000000
 *******************************************************************/

#define KVA0_TO_KVA1(kva)   ( (void*) ((unsigned)(kva) |  0x20000000) )
#define KVA1_TO_KVA0(kva)   ( (void*) ((unsigned)(kva) & ~0x20000000) )

#endif /* FLASH_H */
