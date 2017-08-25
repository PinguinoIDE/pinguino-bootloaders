/***********************************************************************
    Title:  USB Pinguino Bootloader
    File:   core.c
    Descr.: Core routine for MIPS processor
    Author:	Régis Blanchot <rblanchot@gmail.com>

    This file is part of Pinguino (http://www.pinguino.cc)
    Released under the LGPL license (http://www.gnu.org/licenses/lgpl.html)
***********************************************************************/

#ifndef __CORE_C
#define __CORE_C

#include <p32xxxx.h>
#include "typedefs.h"
#include "core.h"           // Core Timer functions, FCPU, FCP0

const UINT32 lookupFPLLIDIVvalue[8] = {1,2,3,4,5,6,10,12};
const UINT32 lookupFPLLMULvalue[8] = {15,16,17,18,19,20,21,24};
const UINT32 lookupFPLLODIVvalue[8] = {1,2,4,8,16,32,64,256};
const UINT32 lookupFPBDIVvalue[4] = {1,2,4,8};

/*******************************************************************
 * Triggers the software reset
 * After that the bootloader will start again
 *******************************************************************/

void SoftReset(void)
{
    // Suspend or Disable all Interrupts
    //DisableInterrupt();

    //if (!(DMACON & _DMACON_SUSPEND_MASK) )
    if (!DMACONbits.SUSPEND)
    {
        //DMACONSET = _DMACON_SUSPEND_MASK;
        DMACONbits.SUSPEND = 1;
        while (DMACON & _DMACON_DMABUSY_MASK);
    }

    // Step 1 - Execute "unlock" sequence to access the RSWRST register
    SYSKEY = 0;
    SYSKEY = 0xAA996655;
    SYSKEY = 0x556699AA;

    // Step 2 - Write a '1' to RSWRST.SWRST bit to arm the software reset
    //RSWRSTSET = _RSWRST_SWRST_MASK;
    RSWRSTbits.SWRST = 1;

    // Step 3 - Read of the RSWRST register triggers the software reset
    (void) RSWRST;

    // Note: The read instruction must be following with either 4 nop
    // instructions (fills the instruction pipe) or a while(1)loop to
    // ensure no instructions can access the bus before reset occurs.
    while (1);
}

/*******************************************************************
 * Clear an array of bytes
 *******************************************************************/

void MemClear(void *address, UINT32 nbytes)
{
    UINT32 *wordp = (UINT32*) address;
    UINT32 nwords32 = nbytes / WORDSIZE;

    while (nwords32--)
    {
        *wordp++ = 0;
    }
}

/*******************************************************************
 * Copy an array of bytes
 *******************************************************************/

void MemCopy (void *from, void *to, UINT32 nbytes)
{
    UINT32 *src = (UINT32*) from;
    UINT32 *dst = (UINT32*) to;
    UINT32 nwords32 = nbytes / WORDSIZE;

    while (nwords32--)
    {
        *dst++ = *src++;
    }
}

/*******************************************************************
 * Disable the hardware interrupts.
 * Save the interrupt state into the supplied variable.
 *******************************************************************/

UINT32 MIPS32 DisableInterrupt(void)
{
    int status;
    asm volatile ("di	%0" : "=r" (status));
    return status;
}

/*******************************************************************
 * Enable hardware interrupts.
 *******************************************************************/

UINT32 MIPS32 EnableInterrupt(void)
{
    int status;
    asm volatile ("ei	%0" : "=r" (status));
    return status;
}

/*******************************************************************
 * Enable hardware interrupts.
 *******************************************************************/

void MIPS32 EnableMultiVectoredInt(void)
{
    unsigned int val;
    // set the CP0 cause IV bit high
    asm volatile("mfc0 %0,$13" : "=r"(val));
    val |= 0x00800000;
    asm volatile("mtc0 %0,$13" : "+r"(val));
    INTCONSET = _INTCON_MVEC_MASK;
    // set the CP0 status IE bit high to turn on interrupts
    EnableInterrupt();
}

/*******************************************************************
 * Reset the Core Timer
 *******************************************************************/

void MIPS32 ResetCoreTimer(void)
{
    asm volatile("mtc0	%0, $9" : : "r" (0));
}

/*******************************************************************
 * Read the Core Timer
 *******************************************************************/

UINT32 MIPS32 ReadCoreTimer(void)
{
    UINT32 timer;
    asm volatile("mfc0	%0, $9" : "=r" (timer));
    return timer;
}

/*******************************************************************
 * Explicit hazard barrier.
 *******************************************************************/

static void inline __attribute__ ((always_inline))
mips_ehb()
{
    asm volatile ("ehb");
}

#endif // __CORE_C
