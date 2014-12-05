/***********************************************************************
    Title:  Test
    File:   test.c
    Descr.: Blink Pinguino's built-in led with p32-gcc 
    Author:	Régis Blanchot <rblanchot@gmail.com>

    This file is part of Pinguino (http://www.pinguino.cc)
    Released under the LGPL license (http://www.gnu.org/licenses/lgpl.html)

 **********************************************************************/

#include "p32xxxx.h"                            // Registers definitions
#include "typedefs.h"                           // UINT8, UINT32, ...
#include "devcfg.h"                             // Configuration bits
//#include "config.h"                             // Config. bits
//#include "hardware.h"                           // Pinguino boards hardware description
//#include "delay.h"                              // DelayUs

/***********************************************************************
 * CONFIG
 * http://www.microchip.com/forums/m481840.aspx
 **********************************************************************/

#if 1

const int DEVCFG3_ __attribute__((section(".devcfg3"))) = 0xCFFFFFFF;
const int DEVCFG2_ __attribute__((section(".devcfg2"))) = 0xFFF979D9;
const int DEVCFG1_ __attribute__((section(".devcfg1"))) = 0xFF601EDB;
const int DEVCFG0_ __attribute__((section(".devcfg0"))) = 0x7FFFFFF3;

#else

const __DEVCFG3bits_t __attribute__((section(".devcfg3"))) devcfg3 =
{
    {
        .USERID     = DEVCFG3_USERID(0x1234),
        .PMDL1WAY   = DEVCFG3_PMDL1WAY_MULTI,
        .IOL1WAY    = DEVCFG3_IOL1WAY_MULTI,
        .FUSBIDIO   = DEVCFG3_FUSBIDIO_USB,
        .FVBUSONIO  = DEVCFG3_FVBUSONIO_USB
    }
};

const __DEVCFG2bits_t __attribute__((section(".devcfg2"))) devcfg2 =
{
    {
        .FPLLIDIV   = DEVCFG2_FPLLIDIV_2,
        .FPLLMUL    = DEVCFG2_FPLLMUL_20,
        .UPLLIDIV   = DEVCFG2_UPLLIDIV_2,
        .UPLLEN     = DEVCFG2_UPLLEN_ENABLED,
        .FPLLODIV   = DEVCFG2_FPLLODIV_1
    }
};

const __DEVCFG1bits_t __attribute__((section(".devcfg1"))) devcfg1 =
{
    {
        .FNOSC      = DEVCFG1_FNOSC_PRIPLL,
        .FSOSCEN    = DEVCFG1_FSOSCEN_DISABLED,
        .IESO       = DEVCFG1_IESO_ENABLED,
        .POSCMOD    = DEVCFG1_POSCMOD_HS,
        .OSCIOFNC   = DEVCFG1_OSCIOFNC_DISABLED,
        .FPBDIV     = DEVCFG1_FPBDIV_2,
        .FCKSM      = DEVCFG1_CSECMD,
        .WDTPS      = DEVCFG1_WDTPS_1024,
        .WINDIS     = DEVCFG1_WINDIS_DISABLED,
        .FWDTEN     = DEVCFG1_FWDTEN_DISABLED,
        .FWDTWINSZ  = DEVCFG1_FWDTWINSZ_50
    }
};

const __DEVCFG0bits_t __attribute__((section(".devcfg0"))) devcfg0 =
{
    {
        .DEBUG      = DEVCFG0_DEBUG_DISABLED,
        .JTAGEN     = DEVCFG0_JTAG_DISABLED,
        .ICESEL     = DEVCFG0_ICESEL2,
        .PWP        = DEVCFG0_PWP_DISABLED,
        .BWP        = DEVCFG0_BWP_DISABLED,
        .CP         = DEVCFG0_CP_DISABLED
    }
};

#endif

/***********************************************************************
 * CONSTANTS
 **********************************************************************/

#define USERLED                 0  // RA0 = D13

/***********************************************************************
 * FUNCTIONS
 **********************************************************************/

UINT32 MIPS32 ReadCoreTimer(void)
{
    UINT32 timer;
    asm volatile("mfc0  %0, $9" : "=r" (timer));
    return timer;
}

void DelayUs(UINT32 usec)
{
    //UINT32 stop = ReadCoreTimer() + usec * (UINT32)(FCP0 / 1000UL);
    UINT32 stop = ReadCoreTimer() + usec * 20000UL; // 20MHz

    // valid only when using a signed type
    while ((int) (ReadCoreTimer() - stop) < 0);
    
    #if 0
    UINT32 j;

    while (usec--)
    {
        j=0x4000;
        while (j--)
        {
            asm volatile("nop");
        }
    }
    #endif
}

/***********************************************************************
 * MAIN PROGRAM ENTRY POINT
 **********************************************************************/

int main(void)
{
    CFGCONbits.JTAGEN=0;        // Disable the JTAG port
    ANSELACLR = 1 << USERLED;     // USERLED as Digital Port
    TRISACLR  = 1 << USERLED;

    while(1)
    {
        //LATAINV = 1 << USERLED;
        LATASET = 1 << USERLED;
        DelayUs(100);
        LATACLR = 1 << USERLED;
        DelayUs(900);
    }

    return 0;
}
