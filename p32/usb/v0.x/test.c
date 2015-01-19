/***********************************************************************
    Title:  Test
    File:   test.c
    Descr.: Blink Pinguino's built-in led with p32-gcc 
    Author:	Régis Blanchot <rblanchot@gmail.com>

    This file is part of Pinguino (http://www.pinguino.cc)
    Released under the LGPL license (http://www.gnu.org/licenses/lgpl.html)

 **********************************************************************/

#include "p32xxxx.h"                // Registers definitions
#include "typedefs.h"               // UINT8, UINT32, ...
#include "config.h"                 // Config. bits
#include "hardware.h"               // Pinguino boards hardware description
#include "delay.h"                  // DelayUs
#include "core.h"                   // Core Timer functions, FCPU, FCP0
#include "serial.h"                 // UART functions

#define COREMARK100 769231          // counter value when FCPU=40MHz

/***********************************************************************
 * MAIN PROGRAM ENTRY POINT
 **********************************************************************/

int main(void)
{
    UINT32 counter=0, sec=0;

    mLED_Init();
    SerialInit(9600);
    SerialPrintChar(12); // CLS
    SerialPrint("RESET\r\n");

    while(1)
    {
        mLED_1_On();
        Delayms(100);
        mLED_1_Off();

        // display current CPU frequency
        SerialPrint("Fcpu = ");
        SerialPrintNumber(FCPU/1000000, 10);
        SerialPrint("MHz\r\n");

        // increment counter for 1 sec.
        #if 1
        counter = 0;
        ResetCoreTimer();
        while (ReadCoreTimer() < FCP0)
        {
            counter++;
        }
        #else
        Delayms(900);
        #endif
        
        // display counter
        SerialPrint("counter = ");
        SerialPrintNumber(counter, 10);
        SerialPrint("\r\n");

        SerialPrint("CoreMark = ");
        SerialPrintNumber(100*counter/COREMARK100, 10);
        SerialPrint("\r\n");
    }
    
    return 0;
}
