/***********************************************************************
    Title:  USB Pinguino Bootloader
    File:   config.h
    Descr.: configuration bits for supported PIC32MX
    Author: Régis Blanchot <rblanchot@gmail.com>
************************************************************************
    TODO :
       - DEVCFG2 according FCPUMHZ
       - check if MPLAB supports __PIC32MX2__
************************************************************************
    This file is part of Pinguino (http://www.pinguino.cc)
    Released under the LGPL license (http://www.gnu.org/licenses/lgpl.html)
***********************************************************************/

#ifndef _CONFIG_H_
#define	_CONFIG_H_

#if !defined(__P32GCC__) // MPLABX/XC32-GCC

    // DEVCFG3
    // USERID = No Setting
    #if defined(__32MX470F512H__)
    #pragma config FSRSSEL = PRIORITY_7     // Shadow Register Set Priority Select (SRS Priority 7)
    #endif
    //#if !defined(__32MX440F256H__)
    #if defined(__PIC32MX2__) || defined(__32MX470F512H__)
    #pragma config PMDL1WAY = OFF           // Peripheral Module Disable Configuration (Allow multiple reconfigurations)
    #pragma config IOL1WAY = OFF            // Peripheral Pin Select Configuration (Allow multiple reconfigurations)
    #pragma config FUSBIDIO = ON            // USB USID Selection (Controlled by the USB Module)
    #pragma config FVBUSONIO = ON           // USB VBUS ON Selection (Controlled by USB Module)
    #endif

    // DEVCFG2 : 8MHz/2*20/2 = 40MHz
    #pragma config FPLLIDIV = DIV_2         // PLL Input Divider (2x Divider)
    #pragma config FPLLMUL = MUL_20         // PLL Multiplier (20x Multiplier)
    #pragma config FPLLODIV = DIV_2         // System PLL Output Clock Divider (PLL Divide by 2)
    #pragma config UPLLEN = ON              // USB PLL Enable (Enabled)
    #pragma config UPLLIDIV = DIV_2         // USB PLL Input Divider (2x Divider)

    // DEVCFG1
    #pragma config FNOSC = PRIPLL           // Oscillator Selection Bits (Primary Osc w/PLL (XT+,HS+,EC+PLL))
    #pragma config FSOSCEN = OFF            // Secondary Oscillator Enable (Disabled)
    #pragma config IESO = ON                // Internal/External Switch Over (Enabled)
    #pragma config POSCMOD = HS             // Primary Oscillator Configuration (HS osc mode)
    #pragma config OSCIOFNC = OFF           // CLKO Output Signal Active on the OSCO Pin (Disabled)
    #pragma config FPBDIV = DIV_2           // Peripheral Clock Divisor (Pb_Clk is Sys_Clk/2)
    #pragma config FCKSM = CSECME           // Clock Switching and Monitor Selection (Clock Switch Enable, FSCM Enabled)
    #pragma config WDTPS = PS1              // Watchdog Timer Postscaler (1:1)
    #pragma config FWDTEN = OFF             // Watchdog Timer Enable (WDT Disabled (SWDTEN Bit Controls))
    #if defined(__PIC32MX2__) || defined(__32MX470F512H__)
    #pragma config WINDIS = OFF             // Watchdog Timer Window Enable (Watchdog Timer is in Non-Window Mode)
    #pragma config FWDTWINSZ = WISZ_25      // Watchdog Timer Window Size (Window Size is 25%)
    #endif

    // DEVCFG0
    #pragma config DEBUG = OFF              // Background Debugger Disable
    //#if !defined(__32MX440F256H__)
    #if defined(__PIC32MX2__) || defined(__32MX470F512H__)
    #pragma config JTAGEN = OFF             // JTAG Enable (JTAG Disabled)
    #pragma config ICESEL = ICS_PGx2        // ICE/ICD Comm Channel Select (Communicate on PGEC2/PGED2)
    #endif
    #pragma config PWP = OFF                // Program Flash Write Protect (Disable)
    #pragma config BWP = OFF                // Boot Flash Write Protect bit (Protection Disabled)
    #pragma config CP = OFF                 // Code Protect (Protection Disabled)

#endif  /* __P32GCC__ */

#endif	/* _CONFIG_H_ */

/*
 *  PIC32MZ2048ECH100
 * 

#pragma config FMIIEN = ON              // Ethernet RMII/MII Enable (MII Enabled)
#pragma config FETHIO = ON              // Ethernet I/O Pin Select (Default Ethernet I/O)
#pragma config PGL1WAY = ON             // Permission Group Lock One Way Configuration (Allow only one reconfiguration)
#pragma config PMDL1WAY = ON            // Peripheral Module Disable Configuration (Allow only one reconfiguration)
#pragma config IOL1WAY = ON             // Peripheral Pin Select Configuration (Allow only one reconfiguration)
#pragma config FUSBIDIO = OFF           // USB USBID Selection (Controlled by Port Function)

// DEVCFG2  OSC_EC24PLL
#pragma config FPLLIDIV = DIV_3         // System PLL Input Divider (1x Divider)
#pragma config FPLLRNG = RANGE_5_10_MHZ // System PLL Input Range (5-10 MHz Input)
#pragma config FPLLICLK = PLL_POSC      // System PLL Input Clock Selection (POSC is input to the System PLL)
#pragma config FPLLMULT = MUL_50        // System PLL Multiplier (PLL Multiply by 50)
#pragma config FPLLODIV = DIV_2
#pragma config UPLLFSEL = FREQ_24MHZ    // USB PLL Input Frequency Selection (USB PLL input is 12 MHz)
#pragma config UPLLEN = ON              // USB PLL Enable (USB PLL is enabled)

// DEVCFG1
#pragma config FNOSC = SPLL             // Oscillator Selection Bits (Fast RC Osc w/Div-by-N (FRCDIV))
#pragma config DMTINTV = WIN_127_128    // DMT Count Window Interval (Window/Interval value is 127/128 counter value)
#pragma config FSOSCEN = OFF            // Secondary Oscillator Enable (Disable SOSC)
#pragma config IESO = OFF               // Internal/External Switch Over (Disabled)
#pragma config POSCMOD = EC             // Primary Oscillator Configuration (Primary osc disabled)
#pragma config OSCIOFNC = ON            // CLKO Output Signal Active on the OSCO Pin (Enabled)
#pragma config FCKSM = CSECME           // Clock Switching and Monitor Selection (Clock Switch enabled, FSCM enabled)
#pragma config WDTPS = PS1048576        // Watchdog Timer Postscaler (1:1048576)
#pragma config WDTSPGM = STOP           // Watchdog Timer Stop During Flash Programming (WDT stops during Flash programming)
#pragma config WINDIS = NORMAL          // Watchdog Timer Window Mode (Watchdog Timer is in non-Window mode)
#pragma config FWDTEN = OFF             // Watchdog Timer Enable (WDT Disabled)
#pragma config FWDTWINSZ = WINSZ_25     // Watchdog Timer Window Size (Window size is 25%)
// DMTCNT = No Setting
#pragma config FDMTEN = OFF             // Deadman Timer Enable (Deadman Timer is disabled)

// DEVCFG0
#pragma config EJTAGBEN = NORMAL
#pragma config DBGPER = ALLOW_PG2
#pragma config FSLEEP = OFF
#pragma config FECCCON = OFF_UNLOCKED
#pragma config BOOTISA = MIPS32
#pragma config TRCEN = OFF
#pragma config ICESEL = ICS_PGx1
#pragma config DEBUG = OFF
 
// DEVCP0
#pragma config CP = OFF                 // Code Protect (Protection Disabled)
#pragma config_alt FWDTEN=OFF
#pragma config_alt USERID = 0x1234u
#pragma config JTAGEN = OFF

*/ 
