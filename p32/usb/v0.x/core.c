/*******************************************************************************
	Title:	USB Pinguino Bootloader
	File:	core.c
	Descr.: Core routine for MIPS processor
	Author:	Régis Blanchot <rblanchot@gmail.com>

	This file is part of Pinguino (http://www.pinguino.cc)
	Released under the LGPL license (http://www.gnu.org/licenses/lgpl.html)
*******************************************************************************/

#ifndef __CORE_C
#define __CORE_C

#include <p32xxxx.h>
#include "typedefs.h"
#include "core.h"           // Core Timer functions, FCP0

/*******************************************************************
 * Triggers the software reset
 *******************************************************************/

void SoftReset(void)
{
    // Suspend or Disable all Interrupts
    DisableInterrupt();

    if (! (DMACON & _DMACON_SUSPEND_MASK) )
    {
        DMACONSET = _DMACON_SUSPEND_MASK;
        while (DMACON & _DMACON_DMABUSY_MASK);
    }

    // Step 1 - Execute "unlock" sequence to access the RSWRST register.
    SYSKEY = 0;
    SYSKEY = 0xAA996655;
    SYSKEY = 0x556699AA;

    // Step 2 - Write a '1' to RSWRST.SWRST bit to arm the software reset.
    RSWRSTSET = _RSWRST_SWRST_MASK;

    // Step 3 - A Read of the RSWRST register must follow the write.
    // This action triggers the software reset, which should occur on the next clock cycle.
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
#if 0
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
#endif
/*******************************************************************
 * Reset the Core Timer
 *******************************************************************/
#if (TEST)
void MIPS32 ResetCoreTimer(void)
{
    asm volatile("mtc0	%0, $9" : : "r" (0));
}
#endif
/*******************************************************************
 * Read the Core Timer
 *******************************************************************/

UINT32 MIPS32 ReadCoreTimer(void)
{
    UINT32 timer;
    asm volatile("mfc0	%0, $9" : "=r" (timer));
    return timer;
}

#endif // __CORE_C
