/*******************************************************************************
	Title:	USB Pinguino Bootloader
	File:	flash.c
	Descr.: flash operations for supported PIC32MX
	Author:	Régis Blanchot <rblanchot@gmail.com>

	This file is part of Pinguino (http://www.pinguino.cc)
	Released under the LGPL license (http://www.gnu.org/licenses/lgpl.html)
*******************************************************************************/

#include "p32xxxx.h"
#include "GenericTypeDefs.h"
#include "flash.h"
#include "hardware.h"

extern void DelayUs (unsigned);

/********************************************************************
* This function must generate MIPS32
********************************************************************/

unsigned int FlashOperation(unsigned int operation, void* address, unsigned int data)
{
    unsigned int status;
    
    // Suspend or Disable all Interrupts
    asm volatile ("di %0" : "=r" (status));

    // Enable Flash Write/Erase Operations and Select
    // Flash operation to perform
    // operation bits are writable when WREN = 0 (bit 14).
    NVMCON = 0x4000 | operation;

    // Data sheet prescribes 6us delay for LVD to become stable.
    // To be on the safer side, we shall set 7us delay.
    DelayUs(7);

    // Write Keys
    NVMKEY = 0xAA996655;
    NVMKEY = 0x556699AA;

    // Start the operation (WR=1)
    NVMCONSET = 0x8000;

    // Wait for operation to complete (WR=0)
    while (NVMCON & 0x8000);

    // Restore Interrupts
    if (status & 0x00000001)
    {
        asm volatile ("ei");
    }
    else
    {
        asm volatile ("di");
    }

    // Return NVMERR and LVDERR Error Status Bits
    //#define FlashIsError()    (NVMCON & (_NVMCON_WRERR_MASK | _NVMCON_LVDERR_MASK))
    return (NVMCON & 0x3000); // bits 12 and 13
}

/*********************************************************************
 * Function:        unsigned int NVMErasePage(void* address)
 *
 * Description:     A page erase will erase a single page of program flash,
 *                  which equates to 1k instructions (4KBytes). The page to
 *                  be erased is selected using NVMADDR. The lower bytes of
 *                  the address given by NVMADDR are ignored in page selection.
 *
 * PreCondition:    None
 *
 * Inputs:          address:  Destination page address to Erase.
 *
 * Output:          '0' if operation completed successfully.
 *
 * Example:         FlashErasePage((void*) 0xBD000000)
 ********************************************************************/

#if 0
unsigned int FlashEraseAll(void)
{
    unsigned int res;
    
    // Unlock and Erase Program Flash
    res = FlashOperation(FLASH_ALL_ERASE);

    // Return Result
    return res;
}

unsigned int FlashErasePage(void* address)
{
    unsigned int res;

    // Convert Virtual Address to Physical Address
    // as NVMADDR only accept Physical Address
    //NVMADDR = KVA_TO_PA((unsigned int)address);
    //NVMADDR = (unsigned long)address & 0x1FFFFFFF;
    NVMADDR = KVA_TO_PA(address);

    // Unlock and Erase Page
    res = FlashOperation(FLASH_PAGE_ERASE);

    // Return WRERR state.
    return res;
}
#endif

/*********************************************************************
 * Function:        unsigned int NVMWriteWord(void* address, unsigned int data)
 *
 * Description:     The smallest block of data that can be programmed in
 *                  a single operation is 1 instruction word (4 Bytes).  The word at
 *                  the location pointed to by NVMADDR is programmed.
 *
 * PreCondition:    None
 *
 * Inputs:          address:   Destination address to write.
 *                  data:      Word to write.
 *
 * Output:          '0' if operation completed successfully.
 *
 * Example:         FlashWriteWord((DWORD*) Address.Val, (unsigned int) ProgrammingBuffer[i++]);
 *                  NVMWriteWord((void*) 0xBD000000, 0x12345678)
 ********************************************************************/

#if 0
unsigned int FlashWriteWord(void* address, unsigned int data)
{
    unsigned int res;

    // Convert Virtual Address to Physical Address
    // as NVMADDR only accept Physical Address
    //NVMADDR = KVA_TO_PA((unsigned int)address);
    //NVMADDR = (unsigned long)address & 0x1FFFFFFF;
    NVMADDR = KVA_TO_PA(address);

    // Load data into NVMDATA register
    NVMDATA = data;

    // Unlock and Write Word
    res = FlashOperation(FLASH_WORD_WRITE);

    // Return WRERR state.
    return res;
}
#endif

/*********************************************************************
 * Function:        unsigned int NVMWriteRow(void* address, void* data)
 *
 * Description:     The largest block of data that can be programmed in
 *                  a single operation is 1 row 128 instructions (512 Bytes).  The row at
 *                  the location pointed to by NVMADDR is programmed with the data buffer
 *                  pointed to by NVMSRCADDR.
 *
 * PreCondition:    The row of data must be pre-loaded into a buffer in RAM.
 *
 * Inputs:          address:  Destination Row address to write.
 *                  data:  Location of data to write.
 *
 * Output:          '0' if operation completed successfully.
 *
 * Example:         NVMWriteRow((void*) 0xBD000000, (void*) 0xA0000000)
 ********************************************************************/

#if 0
unsigned int FlashWriteRow(void* address, void* data)
{
    unsigned int res;

    // Set NVMADDR to Address of row t program
    NVMADDR = KVA_TO_PA((unsigned int)address);

    // Set NVMSRCADDR to the SRAM data buffer Address
    NVMSRCADDR = KVA_TO_PA((unsigned int)data);

    // Unlock and Write Row
    res = FlashOperation(FLASH_ROW_WRITE);

    return res;
}
#endif

/*********************************************************************
 * Function:        unsigned int NVMClearError(void)
 *
 * Description:		Clears the NVMCON error flag.
 *
 * PreCondition:    None
 *
 * Inputs:			None
 *
 * Output:          '0' if operation completed successfully.
 *
 * Example:			NMVClearError()
 ********************************************************************/
#if 0
unsigned int FlashClearError(void)
{
    unsigned int res;

    res = FlashOperation(FLASH_NOP);

    return res;
}
#endif
