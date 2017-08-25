/***********************************************************************
	Title:	USB Pinguino Bootloader
	File:	delay.c
	Descr.: Microsecond delay routine for MIPS processor
	Author:	Régis Blanchot <rblanchot@gmail.com>

	This file is part of Pinguino (http://www.pinguino.cc)
	Released under the LGPL license (http://www.gnu.org/licenses/lgpl.html)
***********************************************************************/

#ifndef __DELAY_C
#define __DELAY_C

#include <p32xxxx.h>
#include "typedefs.h"
#include "core.h"           // Core Timer functions, FCP0
#include "delay.h"
#include "debug.h"          // Debug functions

#if 0//(_DEBUG_ENABLE_)        // defined in hardware.h
#include "serial.h"         // UART functions
#endif

/*	--------------------------------------------------------------------
    Wait for us microseconds
    ------------------------------------------------------------------*/

void Delayus(UINT32 us)
{
    UINT32 stop = ReadCoreTimer() + us * (UINT32)(FCP0 / 1000000UL);

    #if 0//(_DEBUG_ENABLE_)
    SerialPrint("start = ");
    SerialPrintNumber(ReadCoreTimer(), 10);
    SerialPrint("\r\n");
    SerialPrint("stop = ");
    SerialPrintNumber(stop, 10);
    SerialPrint("\r\n");
    #endif

    // valid only when using a signed type
    while ((INT32) (ReadCoreTimer() - stop) < 0);
}

/*	--------------------------------------------------------------------
    Wait for ms milliseconds
    ------------------------------------------------------------------*/

#if (_TEST_ENABLE_)
void Delayms(UINT32 ms)
{
    /*
    do
    {
        Delayus(1000); // 1 ms
    }
    while(--ms);
    */
    UINT32 stop = ReadCoreTimer() + ms * (UINT32)(FCP0 / 1000UL);
    while ((INT32) (ReadCoreTimer() - stop) < 0);
}
#endif

#endif // __DELAY_C
