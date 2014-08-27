/*******************************************************************************
	Title:	USB Pinguino Bootloader
	File:	config.h
	Descr.: configuration bits for supported PIC32MX
	Author:	Régis Blanchot <rblanchot@gmail.com>

	This file is part of Pinguino (http://www.pinguino.cc)
	Released under the LGPL license (http://www.gnu.org/licenses/lgpl.html)
*******************************************************************************/

#ifndef CONFIG_H
#define CONFIG_H

// Pinguino 32MX220 and Pinguino 32MX250
#if defined(__32MX220F032B__) || defined(__32MX250F128B__)

    #pragma config FNOSC    = PRIPLL        // Default Osc. is Primary Osc. with PLL Enabled
    #pragma config POSCMOD  = HS            // Primary Oscillator is a High-speed Crystal (8 MHz)
    #pragma config FPLLIDIV = DIV_2         // PLL Input  Divider (8/2 = 4 MHz)
    #pragma config FPLLODIV = DIV_2         // PLL Output Divider (4/2 = 2 MHz)
    #pragma config FPLLMUL  = MUL_20        // PLL Multiplier (2*20 = 40 MHz)

    #pragma config FPBDIV   = DIV_2         // Peripheral Clock divisor (40/2 = 20 MHz)

    #pragma config UPLLEN   = ON            // USB PLL Enabled
    #pragma config UPLLIDIV = DIV_2         // USB PLL Input Divider (8/2 = 4 MHz)
    #pragma config FCKSM    = CSECME        // Enable Clock Switching & Fail Safe Clock Monitor

    #pragma config FSOSCEN  = OFF           // Secondary Oscillator Enable (KLO was off)
    #pragma config IESO     = ON            // Internal/External Switch-over
    #pragma config FWDTEN   = OFF           // Watchdog Timer
    #pragma config WDTPS    = PS1           // Watchdog Timer Postscale
    #pragma config OSCIOFNC = OFF           // CLKO Enable
    #pragma config CP       = OFF           // Code Protect
    #pragma config BWP      = OFF           // Boot Flash Write Protect
    #pragma config PWP      = OFF           // Program Flash Write Protect
    #pragma config ICESEL   = ICS_PGx2      // ICE/ICD Comm Channel Select
    #pragma config DEBUG    = ON            // Background Debugger Enable
    #pragma config JTAGEN   = OFF           // JTAG Disable

    #define BootMemStart    0x9FC00970      // Address where the bootloader code starts
    #define ProgramMemStart 0x1D004800      // Address where user's program starts

#else

    #pragma config FNOSC    = PRIPLL        // Default Osc. is Primary Osc. with PLL Enabled
    #pragma config POSCMOD  = HS            // Primary Oscillator is a High-speed Crystal (8 MHz)
    #pragma config FPLLIDIV = DIV_2         // PLL Input Divider (8/2 = 4 MHz)
    #pragma config FPLLODIV = DIV_1         // PLL Output Divider (4/1 = 4 MHz)
    #pragma config FPLLMUL  = MUL_15        // PLL Multiplier (4*15 = 60 MHz)

    #pragma config FPBDIV   = DIV_1         // Peripheral Clock divisor (60/1 = 60 MHz)

    #pragma config UPLLEN   = ON            // USB PLL Enabled
    #pragma config UPLLIDIV = DIV_2         // USB PLL Input Divider (8/2 = 4 MHz)
    #pragma config FCKSM    = CSECME        // Enable Clock Switching & Fail Safe Clock Monitor

    #pragma config FSOSCEN  = OFF           // Secondary Oscillator Enable (KLO was off)
    #pragma config IESO     = ON            // Internal/External Switch-over
    #pragma config FWDTEN   = OFF           // Watchdog Timer
    #pragma config WDTPS    = PS1           // Watchdog Timer Postscale
    #pragma config OSCIOFNC = OFF           // CLKO Enable
    #pragma config CP       = OFF           // Code Protect
    #pragma config BWP      = OFF           // Boot Flash Write Protect
    #pragma config PWP      = OFF           // Program Flash Write Protect
    #pragma config ICESEL   = ICS_PGx2      // ICE/ICD Comm Channel Select
    #pragma config DEBUG    = ON            // Background Debugger Enable
    #pragma config JTAGEN   = OFF           // JTAG Disable

    #define BootMemStart    0x9D006000
    #define ProgramMemStart 0x1D005000

#endif

#endif  //CONFIG_H
