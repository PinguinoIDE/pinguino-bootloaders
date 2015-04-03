/*******************************************************************************
	Title:	USB Pinguino Bootloader
	File:	flash.c
	Descr.: flash operations for supported PIC32MX
	Author:	Régis Blanchot <rblanchot@gmail.com>

	This file is part of Pinguino (http://www.pinguino.cc)
	Released under the LGPL license (http://www.gnu.org/licenses/lgpl.html)
*******************************************************************************/

#ifndef __FLASH_C
#define __FLASH_C

#include <p32xxxx.h>
#include "typedefs.h"
#include "flash.h"              // ConvertToPhysicalAddress
#include "hardware.h"
#include "delay.h"              // Delayus
#include "core.h"
#if defined(DEBUG)              // defined in hardware.h
#include "serial.h"             // UART functions
#endif

/********************************************************************
 * Performs flash Write/Erase operation
 * Note : Datasheet recommend to clear WREN before any write to NVMOP
 * but it seems not to work
********************************************************************/

void FlashOperation(UINT32 op, void* addr, UINT32 data)
{
    //UINT32 status;

    // NVMADDR only accept Physical Address
    NVMADDR = ConvertToPhysicalAddress(addr);

    #if 0
    #if defined(DEBUG)
        SerialPrint("FlashOperation / ");
        if (op == FLASH_WORD_WRITE)
        {
            SerialPrint("Write Address 0x");
            SerialPrintNumber(NVMADDR,16);
            SerialPrint("\r\n");
            mLED_1_Toggle();
        }
        if (op == FLASH_PAGE_ERASE)
        {
            SerialPrint("Erase Address 0x");
            SerialPrintNumber(NVMADDR,16);
            SerialPrint("\r\n");
            mLED_2_Toggle();
        }
    #endif
    #endif

    // Load data into NVMDATA register
    NVMDATA = data;

    // Suspend or Disable all Interrupts
    //status = DisableInterrupt();

    // Enable Flash Write/Erase Operations

    // 1-Select Flash operation to perform
    // Enable writes to WR bit and LVD circuit
    NVMCON = _NVMCON_WREN_MASK | op;

    // 2-Wait for LVD to become stable (at least 6us).
    Delayus(7);

    // 3-Write unlock sequence before the WR bit is set
    NVMKEY = 0xAA996655;
    NVMKEY = 0x556699AA;

    // 4-Start the operation (WR=1)
    NVMCONSET = _NVMCON_WR_MASK;

    // 5-Wait for operation to complete (WR=0)
    while (NVMCON & _NVMCON_WR_MASK);

    // 6-Disable Flash Write/Erase operations
    NVMCONCLR = _NVMCON_WREN_MASK;

    // Restore Interrupts if necessary
    #if 0
    if (status & 1)
    {
        EnableInterrupt();
    }
    else
        DisableInterrupt();
    #endif

    #if 0
    // Return NVMERR and LVDERR Error Status Bits
    if (NVMCON & (_NVMCON_WRERR_MASK | _NVMCON_LVDERR_MASK))
    {
        while (1)
        {
            mLED_2_Toggle();
            DelayUs(500);
        }
    }
    #endif
}

#endif // __FLASH_C
