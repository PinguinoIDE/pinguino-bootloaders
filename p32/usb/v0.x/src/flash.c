/*******************************************************************************
	Title:	USB Pinguino Bootloader
	File:	flash.c
	Descr.: flash operations for supported PIC32MX
	Author:	Régis Blanchot <rblanchot@gmail.com>

	This file is part of Pinguino (http://www.pinguino.cc)
	Released under the LGPL license (http://www.gnu.org/licenses/lgpl.html)
*******************************************************************************/

#include <p32xxxx.h>
#include <GenericTypeDefs.h>
#include "flash.h"

#define FlashIsError()    (NVMCON & (_NVMCON_WRERR_MASK | _NVMCON_LVDERR_MASK))

/********************************************************************
* Function: 	FlashOperation()
*
* Precondition: 
*
* Input: 		NV operation
*
* Output:		NV eror
*
* Side Effects:	This function must generate MIPS32 code only and 
				hence the attribute (nomips16)
*
* Overview:     Performs reuested operation.
*
*			
* Note:		 	None.
********************************************************************/

UINT FlashOperation(UINT nvmop)
//UINT __attribute__((nomips16)) FlashOperation(UINT nvmop)
{
    //unsigned long t0 = _CP0_GET_COUNT();
    static WORD delay_count = 500;
    #if 0
    int	int_status;
    int	susp;
    #endif
    
    // Disable DMA & Disable Interrupts
    #if 0
    #ifdef _DMAC
    int_status = INTDisableInterrupts();
    susp = DmaSuspend();
    #else
    int_status = INTDisableInterrupts();
    #endif	// _DMAC
    #endif

    //asm("di"); // Disable all interrupts

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

    //asm("ei"); // Enable all interrupts

    // Enable DMA & Enable Interrupts
    #if 0
    #ifdef _DMAC
    DmaResume(susp);
    INTRestoreInterrupts(int_status);
    #else
    INTRestoreInterrupts(int_status);
    #endif // _DMAC
    #endif

    // Return Error Status
    return(FlashIsError());
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
UINT FlashErasePage(void* address)
{
    UINT res;

    // Convert Address to Physical Address
    NVMADDR = KVA_TO_PA((unsigned int)address);

    // Unlock and Erase Page
    res = FlashOperation(NVMOP_PAGE_ERASE);

    // Return WRERR state.
    return res;
}

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
 * Example:         NVMWriteWord((void*) 0xBD000000, 0x12345678)
 ********************************************************************/
UINT FlashWriteWord(void* address, UINT data)
{
    UINT res;

    NVMADDR = KVA_TO_PA((unsigned int)address);

    // Load data into NVMDATA register
    NVMDATA = data;

    // Unlock and Write Word
    res = FlashOperation(NVMOP_WORD_PGM);

    return res;
}

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
UINT FlashWriteRow(void* address, void* data)
{
    unsigned int res;

    // Set NVMADDR to Address of row t program
    NVMADDR = KVA_TO_PA((unsigned int)address);

    // Set NVMSRCADDR to the SRAM data buffer Address
    NVMSRCADDR = KVA_TO_PA((unsigned int)data);

    // Unlock and Write Row
    res = FlashOperation(NVMOP_ROW_PGM);

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
UINT FlashClearError(void)
{
    unsigned int res;

    res = FlashOperation(NVMOP_NOP);

    return res;
}
#endif
/***********************End of File*************************************************************/
