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
#include "delay.h"                  // Delayus, Delayms
#include "core.h"                   // Core Timer functions, FCPU, FPB, FCP0
#include "serial.h"                 // UART functions

#define COREMARK100MIPS32 133334    // PIC32MX 40MHz MIPS32
#define COREMARK100MIPS16 350878    // PIC32MX 40MHz MIPS16

/***********************************************************************
 * MAIN PROGRAM ENTRY POINT
 **********************************************************************/

int main(void)
{
    UINT32 counter=0, sec=0;
    
    mLED_Init();
    
    SerialInit(9600);
    SerialPrintChar(12); // CLS
    
    while(1)
    {
        mLED_1_Off();
        mLED_2_On();

        Delayms(100);

        mLED_1_On();
        mLED_2_Off();

        // display current CPU frequency
        SerialPrint("FCPU = ");
        SerialPrintNumber(FCPU/1000000, 10);
        SerialPrint("MHz, ");

        // display current peripheral frequency
        SerialPrint("FPB = ");
        SerialPrintNumber(FPB/1000000, 10);
        SerialPrint("MHz, ");

        // increment counter for 1 sec.
        counter = 0;
        ResetCoreTimer();
        while (ReadCoreTimer() < FCP0)
            counter++;
        
        // display counter
        SerialPrint("Coremark = ");
        SerialPrintNumber(counter, 10);
        SerialPrint(", ");

        //SerialPrint("CoreMark = ");
        SerialPrintNumber((UINT32)(100*counter/COREMARK100MIPS16-100), 10);
        if (counter == COREMARK100MIPS16)
            SerialPrint("%, same speed ");
        else if (counter > COREMARK100MIPS16)
            SerialPrint("% faster ");
        else
            SerialPrint("% slower ");
        SerialPrint("than a PIC32MX @ 40MHz/MIPS16\r\n");

        //Delayms(900);
    }
    
    return 0;
}
