/***********************************************************************
	Title:	USB Pinguino Bootloader
	File:	flash.c
	Descr.: flash operations for supported PIC32MX
	Author:	Régis Blanchot <rblanchot@gmail.com>

	This file is part of Pinguino (http://www.pinguino.cc)
	Released under the LGPL license (http://www.gnu.org/licenses/lgpl.html)
***********************************************************************/

#ifndef __FLASH_C
#define __FLASH_C

#include <p32xxxx.h>
#include "typedefs.h"
#include "flash.h"              // ConvertToPhysicalAddress
#include "hardware.h"
#include "delay.h"              // Delayus
#include "core.h"
#if (_DEBUG_ENABLE_)            // defined in m
#include "serial.h"             // UART functions
#endif

/***********************************************************************
 * Performs flash Write/Erase operation
 * This function must generate MIPS32 code only
 **********************************************************************/

UINT8 FlashOperation(UINT8 op)
{
    UINT8 res;
    //UINT32 status;
    //UINT32 delay_count = 1500;

    // Suspend or Disable all Interrupts
    //status = DisableInterrupt();

    // 1-Select Flash operation to perform
    // Enable writes to WR bit and LVD circuit
    NVMCON = _NVMCON_WREN_MASK | op;

    // 2-Wait for LVD to become stable (at least 6us).
    Delayus(7);
    // Assume we're running at max frequency (80 MHz) so we're always safe
    // 1 cycle = 1/80MHz = 12.5 ns so 6us is about 500 cycles
    //while (delay_count--);
    
    // 3-Write unlock sequence before the WR bit is set
    NVMKEY = 0xAA996655;
    NVMKEY = 0x556699AA;

    // 4-Start the operation (WR=1)
    // Must be an atomic instruction
    NVMCONSET = _NVMCON_WR_MASK;
    //NVMCON |= _NVMCON_WR_MASK;
    //NVMCONbits.WR = 1;

    // 5-Wait for operation to complete (WR=0)
    while (NVMCON & _NVMCON_WR_MASK);
    //while (NVMCONbits.WR);

    // 6-Disable Flash Write/Erase operations
    NVMCONCLR = _NVMCON_WREN_MASK;
    //NVMCONbits.WREN = 0;

    // Restore Interrupts if necessary
    #if 0
    if (status & 1)
        EnableInterrupt();
    else
        DisableInterrupt();
    #endif

    res = FlashError();
    
    #if (_DEBUG_ENABLE_)
    if (res)
        SerialPrint("Error\r\n");
    #endif
    
    return res;
}

/*
UINT8 FlashOperation(UINT8 nvmop)
{
    static WORD delay_count = 500;

    // Enable Flash Write/Erase Operations
    NVMCON = NVMCON_WREN | nvmop;

    // Wait at least 6 us = 6000 ns for LVD start-up
    // Assume we're running at max frequency (80 MHz) so we're always safe
    // 1 cycle = 1/80MHz = 12.5 ns so 6us is about 500 cycles
    //while (_CP0_GET_COUNT() - t0 < (80/2)*6);

    while (delay_count--);

    NVMKEY 	= 0xAA996655;
    NVMKEY 	= 0x556699AA;
    NVMCONSET 	= NVMCON_WR;

    // Wait for WR bit to clear
    while(NVMCON & NVMCON_WR);
    
    // Disable Flash Write/Erase operations
    NVMCONCLR = NVMCON_WREN;  

    // Return Error Status
    return(FlashError());
}
*/

/***********************************************************************
 * Erase a single page of program flash,
 * The page to be erased is selected using NVMADDR.
 * Returns '0' if operation completed successfully.
 **********************************************************************/

UINT8 FlashErasePage(void* address)
{
    UINT8 res;

    // Convert Address to Physical Address
    NVMADDR = ConvertToPhysicalAddress(address);

    #if (_DEBUG_ENABLE_)
        SerialPrint("Erase page at 0x");
        SerialPrintNumber(NVMADDR,16);
        SerialPrint("\r\n");
    #endif

    // Unlock and Erase Page
    res = FlashOperation(FLASH_PAGE_ERASE);

    // Return WRERR state.
    return res;
}

/***********************************************************************
 * Writes a word (4 Bytes) pointed to by NVMADDR.
 * Returns '0' if operation completed successfully.
 **********************************************************************/

UINT8 FlashWriteWord(void* address, UINT32 data)
{
    UINT8 res;

    NVMADDR = ConvertToPhysicalAddress(address);

    // Load data into NVMDATA register
    NVMDATA = data;

    #if (_DEBUG_ENABLE_)
        SerialPrint("Write 0x");
        SerialPrintNumber(NVMADDR,16);
        SerialPrint(" with word 0x");
        SerialPrintNumber(NVMDATA,16);
        SerialPrint("\r\n");
    #endif

    // Unlock and Write Word
    res = FlashOperation(FLASH_WORD_WRITE);

    return res;
}

/***********************************************************************
 * Writes a block of data (1 row is 128 instructions = 512 Bytes).
 * The row at the location pointed to by NVMADDR is programmed with
 * the data buffer pointed to by NVMSRCADDR.
 * Returns '0' if operation completed successfully.
 **********************************************************************/

UINT8 FlashWriteRow(void* address, void* data)
{
    UINT32 res;

    // Set NVMADDR to Address of row t program
    NVMADDR = ConvertToPhysicalAddress(address);

    // Set NVMSRCADDR to the SRAM data buffer Address
    NVMSRCADDR = ConvertToPhysicalAddress(data);

    // Unlock and Write Row
    res = FlashOperation(FLASH_ROW_WRITE);

    return res;
}

/***********************************************************************
 * Clears the NVMCON error flag.
 * returns '0' if operation completed successfully.
 **********************************************************************/
/*
UINT8 FlashClearError()
{
    UINT8 res;

    res = FlashOperation(FLASH_NOP);

    return res;
}
*/
#endif // __FLASH_C
