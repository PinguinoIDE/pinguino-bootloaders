/*******************************************************************************
	Title:	USB Pinguino Bootloader
	File:	config.h
	Descr.: configuration bits for supported PIC32MX
	Author:	Régis Blanchot <rblanchot@gmail.com>

	This file is part of Pinguino (http://www.pinguino.cc)
	Released under the LGPL license (http://www.gnu.org/licenses/lgpl.html)
*******************************************************************************/

#ifndef _CONFIG_H_
#define	_CONFIG_H_

// Is the current program compiled with the Pinguino Toolchain ?
#if defined(__P32GCC__)
    #include "devcfg.h"

    #if defined(__32MX220F032B__) || \
        defined(__32MX250F128B__) || \
        defined(__32MX270F256B__)

        //0xCFFFFFFF;
        int DEVCFG3_ __attribute__((section(".devcfg3"))) =
            DEVCFG3_USERID(0xffff) |    /* User-defined ID */
            DEVCFG3_FSRSSEL_7 |         /* Assign irq priority 7 to shadow set */
            DEVCFG3_FMIIEN |            /* Ethernet MII enable */
            DEVCFG3_FETHIO |            /* Ethernet pins default */
            DEVCFG3_FCANIO |            /* CAN pins default */
            DEVCFG3_FUSBIDIO |          /* USBID pin: controlled by USB */
            DEVCFG3_FVBUSONIO;          /* VBuson pin: controlled by USB */

        //0xFFF979D9;
        int DEVCFG2_ __attribute__((section(".devcfg2"))) =
            DEVCFG2_FPLLIDIV_2 |        /* PLL divider = 1/2 */
            DEVCFG2_FPLLMUL_20 |        /* PLL multiplier = 20x */
            DEVCFG2_UPLLIDIV_2 |        /* USB PLL divider = 1/2 */
            DEVCFG2_FPLLODIV_2;         /* PLL postscaler = 1/2 */

        //0xFF601EDB;
        int DEVCFG1_ __attribute__((section(".devcfg1"))) =
            DEVCFG1_FNOSC_PRIPLL |      /* Primary oscillator with PLL */
            DEVCFG1_IESO |              /* Internal-external switch over */
            DEVCFG1_POSCMOD_HS |        /* HS oscillator */
            DEVCFG1_FPBDIV_2 |          /* Peripheral bus clock = SYSCLK/2 */
            DEVCFG1_WDTPS_1;            /* Watchdog postscale = 1/1024 */

        //0x7FFFFFF3;
        int DEVCFG0_ __attribute__((section(".devcfg0"))) =
            DEVCFG0_DEBUG_DISABLED,     /* ICE debugger disabled */

    #else

        #error "Configuration Bits for this proc. must still be defined. in config.h"

    #endif

#else
 
    #if defined(__32MX220F032B__) || \
        defined(__32MX250F128B__) || \
        defined(__32MX270F256B__)

        // DEVCFG3
        // USERID = No Setting
        #pragma config PMDL1WAY = OFF           // Peripheral Module Disable Configuration (Allow multiple reconfigurations)
        #pragma config IOL1WAY = OFF            // Peripheral Pin Select Configuration (Allow multiple reconfigurations)
        #pragma config FUSBIDIO = ON            // USB USID Selection (Controlled by the USB Module)
        #pragma config FVBUSONIO = ON           // USB VBUS ON Selection (Controlled by USB Module)

        // DEVCFG2
        #pragma config FPLLIDIV = DIV_2         // PLL Input Divider (2x Divider)
        #pragma config FPLLMUL = MUL_20         // PLL Multiplier (20x Multiplier)
        #pragma config FPLLODIV = DIV_2         // System PLL Output Clock Divider (PLL Divide by 2)
        #pragma config UPLLIDIV = DIV_2         // USB PLL Input Divider (2x Divider)
        #pragma config UPLLEN = ON              // USB PLL Enable (Enabled)

        // DEVCFG1
        #pragma config FNOSC = PRIPLL           // Oscillator Selection Bits (Primary Osc w/PLL (XT+,HS+,EC+PLL))
        #pragma config FSOSCEN = OFF            // Secondary Oscillator Enable (Disabled)
        #pragma config IESO = ON                // Internal/External Switch Over (Enabled)
        #pragma config POSCMOD = HS             // Primary Oscillator Configuration (HS osc mode)
        #pragma config OSCIOFNC = OFF           // CLKO Output Signal Active on the OSCO Pin (Disabled)
        #pragma config FPBDIV = DIV_2           // Peripheral Clock Divisor (Pb_Clk is Sys_Clk/2)
        #pragma config FCKSM = CSECME           // Clock Switching and Monitor Selection (Clock Switch Enable, FSCM Enabled)
        #pragma config WDTPS = PS1              // Watchdog Timer Postscaler (1:1)
        #pragma config WINDIS = OFF             // Watchdog Timer Window Enable (Watchdog Timer is in Non-Window Mode)
        #pragma config FWDTEN = OFF             // Watchdog Timer Enable (WDT Disabled (SWDTEN Bit Controls))
        //#pragma config FWDTWINSZ = WISZ_25      // Watchdog Timer Window Size (Window Size is 25%)

        // DEVCFG0
        #pragma config JTAGEN = OFF             // JTAG Enable (JTAG Disabled)
        #pragma config ICESEL = ICS_PGx2        // ICE/ICD Comm Channel Select (Communicate on PGEC2/PGED2)
        #pragma config PWP = OFF                // Program Flash Write Protect (Disable)
        #pragma config BWP = OFF                // Boot Flash Write Protect bit (Protection Disabled)
        #pragma config CP = OFF                 // Code Protect (Protection Disabled)
        #pragma config DEBUG = OFF              // Background Debugger Disable

    #else

        // DEVCFG3
        // USERID = No Setting
        #pragma config PMDL1WAY = OFF           // Peripheral Module Disable Configuration (Allow multiple reconfigurations)
        #pragma config IOL1WAY = OFF            // Peripheral Pin Select Configuration (Allow multiple reconfigurations)
        #pragma config FUSBIDIO = ON            // USB USID Selection (Controlled by the USB Module)
        #pragma config FVBUSONIO = ON           // USB VBUS ON Selection (Controlled by USB Module)

        // DEVCFG2
        #pragma config FPLLIDIV = DIV_2         // PLL Input Divider (2x Divider)
        #pragma config FPLLMUL = MUL_20         // PLL Multiplier (20x Multiplier)
        #pragma config FPLLODIV = DIV_1         // System PLL Output Clock Divider (PLL Divide by 2)
        #pragma config UPLLIDIV = DIV_2         // USB PLL Input Divider (2x Divider)
        #pragma config UPLLEN = ON              // USB PLL Enable (Enabled)

        // DEVCFG1
        #pragma config FNOSC = PRIPLL           // Oscillator Selection Bits (Primary Osc w/PLL (XT+,HS+,EC+PLL))
        #pragma config FSOSCEN = OFF            // Secondary Oscillator Enable (Disabled)
        #pragma config IESO = ON                // Internal/External Switch Over (Enabled)
        #pragma config POSCMOD = HS             // Primary Oscillator Configuration (HS osc mode)
        #pragma config OSCIOFNC = OFF           // CLKO Output Signal Active on the OSCO Pin (Disabled)
        #pragma config FPBDIV = DIV_2           // Peripheral Clock Divisor (Pb_Clk is Sys_Clk/2)
        #pragma config FCKSM = CSECME           // Clock Switching and Monitor Selection (Clock Switch Enable, FSCM Enabled)
        #pragma config WDTPS = PS1              // Watchdog Timer Postscaler (1:1)
        #pragma config WINDIS = OFF             // Watchdog Timer Window Enable (Watchdog Timer is in Non-Window Mode)
        #pragma config FWDTEN = OFF             // Watchdog Timer Enable (WDT Disabled (SWDTEN Bit Controls))
        //#pragma config FWDTWINSZ = WISZ_25      // Watchdog Timer Window Size (Window Size is 25%)

        // DEVCFG0
        #pragma config JTAGEN = OFF             // JTAG Enable (JTAG Disabled)
        #pragma config ICESEL = ICS_PGx2        // ICE/ICD Comm Channel Select (Communicate on PGEC2/PGED2)
        #pragma config PWP = OFF                // Program Flash Write Protect (Disable)
        #pragma config BWP = OFF                // Boot Flash Write Protect bit (Protection Disabled)
        #pragma config CP = OFF                 // Code Protect (Protection Disabled)

    #endif

#endif  /* __P32GCC__ */

#endif	/* _CONFIG_H_ */

