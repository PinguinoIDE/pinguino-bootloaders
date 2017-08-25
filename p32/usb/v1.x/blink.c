/***********************************************************************
    Title:  Blink
    File:   blink.c
    Descr.: Blink Pinguino's built-in led with p32-gcc 
    Author:	Régis Blanchot <rblanchot@gmail.com>

    This file is part of Pinguino (http://www.pinguino.cc)
    Released under the LGPL license (http://www.gnu.org/licenses/lgpl.html)

 **********************************************************************/

#include "p32xxxx.h"                // Registers definitions
#include "typedefs.h"               // UINT8, UINT32, ...
#include "config.h"                 // Config. bits
#include "hardware.h"               // Pinguino boards hardware description
#include "core.h"                   // FCPU, FCP0
#include "delay.h"                  // Delayms

int main(void)
{
    mLED_Init();
    //TRISGbits.TRISG6 = 0;
    //TRISDbits.TRISD1 = 0;
    while(1)
    {
        mLED_1_Off();
        mLED_2_On();
        //LATGbits.LATG6 = 0;
        //LATDbits.LATD1 = 1;
        Delayms(100);
        
        mLED_1_On();
        mLED_2_Off();
        //LATGbits.LATG6 = 1;
        //LATDbits.LATD1 = 0;
        Delayms(100);
    }
    return 0;
}
