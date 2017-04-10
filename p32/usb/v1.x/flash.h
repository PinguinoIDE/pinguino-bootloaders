/*******************************************************************************
	Title:	USB Pinguino Bootloader
	File:	flash.h
	Descr.: flash operations for supported PIC32MX
	Author:	Régis Blanchot <rblanchot@gmail.com>

	This file is part of Pinguino (http://www.pinguino.cc)
	Released under the LGPL license (http://www.gnu.org/licenses/lgpl.html)
*******************************************************************************/

#ifndef _FLASH_H_
#define _FLASH_H_

#include "typedefs.h"

void FlashOperation(UINT32, void *, UINT32);

#define FLASH_NOP               0x0000      // NOP operation
#define FLASH_WORD_WRITE        0x0001      // Word program operation
#define FLASH_ROW_WRITE         0x0003      // Row program operation
#define FLASH_PAGE_ERASE        0x0004      // Page erase operation
#define FLASH_ALL_ERASE         0x0005      // Program Flash Memory erase operation

/*******************************************************************
 * To translate the kernel address (KSEG0 or KSEG1) to a physical address,
 * perform a "Bitwise AND" operation of the virtual address with 0x1FFFFFFF:
 * Physical Address = Virtual Address and 0x1FFFFFFF
 *******************************************************************/

#define KVA_TO_PA(va)       ( (UINT32) (va) & 0x1FFFFFFF )
//#define KVA_TO_PA(va)       ( va & 0x1FFFFFFF )

#if 1

#define ConvertToPhysicalAddress(a) KVA_TO_PA(a)

#else

static inline void * ConvertToPhysicalAddress (volatile void *addr)
{
    UINT32 virt = (UINT32) addr;
    UINT32 phys;

    if (virt & 0x80000000)
    {
        if (virt & 0x40000000)
        {
            // kseg2 or kseg3 - no mapping
            phys = virt;
        }
        else
        {
            // kseg0¸ kseg1, cut bits A[31:29]
            phys = virt & 0x1fffffff;
        }
    }
    else
    {
    // kuseg
        phys = virt + 0x40000000;
    }
    return (void*) phys;
}
#endif

/*******************************************************************
 * For physical address to KSEG0 virtual address translation,
 * perform a "Bitwise OR" operation of the physical address with 0x80000000:
 * KSEG0 Virtual Address = Physical Address | 0x80000000
 *******************************************************************/

#define PA_TO_KVA0(pa)      ( (UINT32) (pa)  | 0x80000000 )
#define ConvertFlashToVirtualAddress(a)  PA_TO_KVA0(a)

/*******************************************************************
 * For physical address to KSEG1 virtual address translation,
 * perform a "Bitwise OR" operation of the physical address with 0xA0000000:
 * KSEG1 Virtual Address = Physical Address | 0xA0000000
 ******************************************************************/

#define PA_TO_KVA1(pa)      ( (UINT32) (pa)  | 0xA0000000 )
#define ConvertRAMToVirtualAddress(a)  PA_TO_KVA1(a)

/*******************************************************************
 * To translate from KSEG0 to KSEG1 virtual address,
 * perform a "Bitwise OR" operation of the KSEG0 virtual address with 0x20000000:
 * KSEG1 Virtual Address = KSEG0 Virtual Address | 0x20000000
 *******************************************************************/

#define KVA0_TO_KVA1(va)   ( (UINT32) (va) |  0x20000000 )
#define KVA1_TO_KVA0(va)   ( (UINT32) (va) & ~0x20000000 )

#endif /* _FLASH_H_ */
