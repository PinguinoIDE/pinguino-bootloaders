/***********************************************************************
    Title:  USB Pinguino Bootloader
    File:   config.c
    Descr.: configuration bits for supported PIC32MX
    Author: Régis Blanchot <rblanchot@gmail.com>

    This file is part of Pinguino (http://www.pinguino.cc)
    Released under the LGPL license (http://www.gnu.org/licenses/lgpl.html)
***********************************************************************/

#ifndef _CONFIG_C_
#define	_CONFIG_C_

#if defined(__P32GCC__) // PINGUINO/P32-GCC

    #include "p32xxxx.h"                            // Registers definitions
    #include "typedefs.h"                           // UINT8, UINT32, ...

    // Microchip added a proprietary set of #pragma config's to their C compiler,
    // but these are not supported by the Pinguino Toolchain.
    // So we use this simple solution instead :

    #include "config.h"

    const __DEVCFG3bits_t __attribute__((section(".devcfg3"))) devcfg3 =
    {{
        .USERID     = DEVCFG3_USERID('P'+'I'+'N'+'G'+'U'+'I'+'N'+'O'),
        #if defined(__32MX470F512H__)
        .FSRSSEL = DEVCFG3_PRIORITY_7,          // Shadow Register Set Priority Select (SRS Priority 7)
        #endif
        //#if !defined(__32MX440F256H__)
        #if defined(__PIC32MX2__) || defined(__32MX470F512H__)
        .PMDL1WAY   = DEVCFG3_PMDL1WAY_MULTI,   // Peripheral Module Disable Configuration (Allow multiple reconfigurations)
        .IOL1WAY    = DEVCFG3_IOL1WAY_MULTI,    // Peripheral Pin Select Configuration (Allow multiple reconfigurations)
        .FUSBIDIO   = DEVCFG3_FUSBIDIO_USB,     // USB USID Selection (Controlled by the USB Module)
        .FVBUSONIO  = DEVCFG3_FVBUSONIO_USB     // USB VBUS ON Selection (Controlled by USB Module)
        #endif
    }};

    // CRYSTAL and CPU frequencies are defined in the Makefile
    // FCPU = CRYSTAL / FPLLIDIV * FPLLMUL / FPLLODIV
    const __DEVCFG2bits_t __attribute__((section(".devcfg2"))) devcfg2 =
    {{
        #if   (FCPUMHZ==192) // DOESN'T WORK
            .FPLLIDIV   = DEVCFG2_FPLLIDIV_1,
            .FPLLMUL    = DEVCFG2_FPLLMUL_24,
            .FPLLODIV   = DEVCFG2_FPLLODIV_1,
        #elif (FCPUMHZ==168) // Upper limit for family 3 and above
            .FPLLIDIV   = DEVCFG2_FPLLIDIV_1,
            .FPLLMUL    = DEVCFG2_FPLLMUL_21,
            .FPLLODIV   = DEVCFG2_FPLLODIV_1,
        #elif (FCPUMHZ==160)
            .FPLLIDIV   = DEVCFG2_FPLLIDIV_1,
            .FPLLMUL    = DEVCFG2_FPLLMUL_20,
            .FPLLODIV   = DEVCFG2_FPLLODIV_1,
        #elif (FCPUMHZ==152)
            .FPLLIDIV   = DEVCFG2_FPLLIDIV_1,
            .FPLLMUL    = DEVCFG2_FPLLMUL_19,
            .FPLLODIV   = DEVCFG2_FPLLODIV_1,
        #elif (FCPUMHZ==144)
            .FPLLIDIV   = DEVCFG2_FPLLIDIV_1,
            .FPLLMUL    = DEVCFG2_FPLLMUL_18,
            .FPLLODIV   = DEVCFG2_FPLLODIV_1,
        #elif (FCPUMHZ==136)
            .FPLLIDIV   = DEVCFG2_FPLLIDIV_1,
            .FPLLMUL    = DEVCFG2_FPLLMUL_17,
            .FPLLODIV   = DEVCFG2_FPLLODIV_1,
        #elif (FCPUMHZ==128)
            .FPLLIDIV   = DEVCFG2_FPLLIDIV_1,
            .FPLLMUL    = DEVCFG2_FPLLMUL_16,
            .FPLLODIV   = DEVCFG2_FPLLODIV_1,
        #elif (FCPUMHZ==120)
            .FPLLIDIV   = DEVCFG2_FPLLIDIV_1,
            .FPLLMUL    = DEVCFG2_FPLLMUL_15,
            .FPLLODIV   = DEVCFG2_FPLLODIV_1,
        #elif (FCPUMHZ==96)
            .FPLLIDIV   = DEVCFG2_FPLLIDIV_2,
            .FPLLMUL    = DEVCFG2_FPLLMUL_24,
            .FPLLODIV   = DEVCFG2_FPLLODIV_1,
        #elif (FCPUMHZ==84)
            .FPLLIDIV   = DEVCFG2_FPLLIDIV_2,
            .FPLLMUL    = DEVCFG2_FPLLMUL_21,
            .FPLLODIV   = DEVCFG2_FPLLODIV_1,
        #elif (FCPUMHZ==80)
            .FPLLIDIV   = DEVCFG2_FPLLIDIV_2,
            .FPLLMUL    = DEVCFG2_FPLLMUL_20,
            .FPLLODIV   = DEVCFG2_FPLLODIV_1,
        #elif (FCPUMHZ==76)
            .FPLLIDIV   = DEVCFG2_FPLLIDIV_2,
            .FPLLMUL    = DEVCFG2_FPLLMUL_19,
            .FPLLODIV   = DEVCFG2_FPLLODIV_1,
        #elif (FCPUMHZ==72)
            .FPLLIDIV   = DEVCFG2_FPLLIDIV_2,
            .FPLLMUL    = DEVCFG2_FPLLMUL_18,
            .FPLLODIV   = DEVCFG2_FPLLODIV_1,
        #elif (FCPUMHZ==68)
            .FPLLIDIV   = DEVCFG2_FPLLIDIV_2,
            .FPLLMUL    = DEVCFG2_FPLLMUL_17,
            .FPLLODIV   = DEVCFG2_FPLLODIV_1,
        #elif (FCPUMHZ==64) // Upper limit for family 2
            .FPLLIDIV   = DEVCFG2_FPLLIDIV_2,
            .FPLLMUL    = DEVCFG2_FPLLMUL_16,
            .FPLLODIV   = DEVCFG2_FPLLODIV_1,
        #elif (FCPUMHZ==60)
            .FPLLIDIV   = DEVCFG2_FPLLIDIV_2,
            .FPLLMUL    = DEVCFG2_FPLLMUL_15,
            .FPLLODIV   = DEVCFG2_FPLLODIV_1,
        #elif (FCPUMHZ==48)
            .FPLLIDIV   = DEVCFG2_FPLLIDIV_2,
            .FPLLMUL    = DEVCFG2_FPLLMUL_24,
            .FPLLODIV   = DEVCFG2_FPLLODIV_2,
        #elif (FCPUMHZ==42)
            .FPLLIDIV   = DEVCFG2_FPLLIDIV_2,
            .FPLLMUL    = DEVCFG2_FPLLMUL_21,
            .FPLLODIV   = DEVCFG2_FPLLODIV_2,
        #elif (FCPUMHZ==40)
            .FPLLIDIV   = DEVCFG2_FPLLIDIV_2,   // PLL Input Divider (2x Divider)
            .FPLLMUL    = DEVCFG2_FPLLMUL_20,   // PLL Multiplier (20x Multiplier)
            .FPLLODIV   = DEVCFG2_FPLLODIV_2,   // System PLL Output Clock Divider (PLL Divide by 2)
        #endif
        .UPLLIDIV   = DEVCFG2_UPLLIDIV_2,       // USB PLL Input Divider (2x Divider)
        .UPLLEN     = DEVCFG2_UPLLEN_ENABLED    // USB PLL Enable (Enabled)
    }};
    
    const __DEVCFG1bits_t __attribute__((section(".devcfg1"))) devcfg1 =
    {{
        .FNOSC      = DEVCFG1_FNOSC_PRIPLL,     // Oscillator Selection Bits (Primary Osc w/PLL (XT+,HS+,EC+PLL)
        .FSOSCEN    = DEVCFG1_FSOSCEN_DISABLED, // Secondary Oscillator Enable (Disabled)
        .IESO       = DEVCFG1_IESO_ENABLED,     // Internal/External Switch Over (Enabled)
        .POSCMOD    = DEVCFG1_POSCMOD_HS,       // Primary Oscillator Configuration (HS osc mode)
        .OSCIOFNC   = DEVCFG1_OSCIOFNC_DISABLED,// CLKO Output Signal Active on the OSCO Pin (Disabled)
        .FPBDIV     = DEVCFG1_FPBDIV_2,         // Peripheral Clock Divisor (Pb_Clk is Sys_Clk/2)
        .FCKSM      = DEVCFG1_CSECME,           // Clock Switching and Monitor Selection (Clock Switch Enable, FSCM Enabled)
        .WDTPS      = DEVCFG1_WDTPS_1,          // Watchdog Timer Postscaler (1:1)
        #if defined(__PIC32MX2__) || defined(__32MX470F512H__)
        .WINDIS     = DEVCFG1_WINDIS_DISABLED,  // Watchdog Timer Window Enable (Watchdog Timer is in Non-Window Mode)
        #endif
        .FWDTEN     = DEVCFG1_FWDTEN_DISABLED,  // Watchdog Timer Enable (WDT Disabled (SWDTEN Bit Controls))
        #if defined(__PIC32MX2__) || defined(__32MX470F512H__)
        .FWDTWINSZ  = DEVCFG1_FWDTWINSZ_25      // Watchdog Timer Window Size (Window Size is 25%)
        #endif
    }};

    const __attribute__((section(".devcfg0"))) devcfg0 = 0x7FFFFFF3;
    /*
    const __DEVCFG0bits_t __attribute__((section(".devcfg0"))) devcfg0 =
    {{
        .DEBUG      = DEVCFG0_DEBUG_DISABLED,   // Background Debugger Disable
        //#if !defined(__32MX440F256H__)
        #if defined(__PIC32MX2__) || defined(__32MX470F512H__)
        .JTAGEN     = DEVCFG0_JTAG_DISABLED,    // JTAG Disabled
        #endif
        .ICESEL     = DEVCFG0_ICESEL2,          // ICE/ICD Comm Channel Select (Communicate on PGEC2/PGED2)
        .PWP        = DEVCFG0_PWP_DISABLED,     // Program Flash Write Protect (Disabled)
        .BWP        = DEVCFG0_BWP_DISABLED,     // Boot Flash Write Protect bit (Disabled)
        .CP         = DEVCFG0_CP_DISABLED       // Code Protect (Disabled)
    }};
    */
#endif  /* __P32GCC__ */

#endif	/* _CONFIG_C_ */
