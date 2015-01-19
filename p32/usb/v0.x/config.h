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

#if defined(__P32GCC__) // PINGUINO/P32-GCC

    // Microchip added a proprietary set of #pragma config's to their C compiler,
    // but these are not supported by the Pinguino Toolchain.
    // So we use this simple solution instead :

    #include "devcfg.h"

    #if defined(__32MX220F032B__) || \
        defined(__32MX250F128B__) || \
        defined(__32MX270F256B__)

        #if 0
        const int devcfg3 __attribute__((section(".devcfg3"))) = 0xCFFFFFFF;
        const int devcfg2 __attribute__((section(".devcfg2"))) = 0xFFF979D9;
        const int devcfg0 __attribute__((section(".devcfg0"))) = 0x7FFFFFF3;
        const int devcfg1 __attribute__((section(".devcfg1"))) = 0xFF601EDB;
        #endif

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

        // FCPUMHZ is defined in the Makefile
        const __DEVCFG2bits_t __attribute__((section(".devcfg2"))) devcfg2 =
        {
            {
                #if   (FCPUMHZ==42)
                    .FPLLIDIV   = DEVCFG2_FPLLIDIV_2,
                    .FPLLMUL    = DEVCFG2_FPLLMUL_21,
                    .FPLLODIV   = DEVCFG2_FPLLODIV_2,
                #elif (FCPUMHZ==48)
                    .FPLLIDIV   = DEVCFG2_FPLLIDIV_2,
                    .FPLLMUL    = DEVCFG2_FPLLMUL_24,
                    .FPLLODIV   = DEVCFG2_FPLLODIV_2,
                #elif (FCPUMHZ==60)
                    .FPLLIDIV   = DEVCFG2_FPLLIDIV_2,
                    .FPLLMUL    = DEVCFG2_FPLLMUL_15,
                    .FPLLODIV   = DEVCFG2_FPLLODIV_1,
                #elif (FCPUMHZ==64)
                    .FPLLIDIV   = DEVCFG2_FPLLIDIV_2,
                    .FPLLMUL    = DEVCFG2_FPLLMUL_16,
                    .FPLLODIV   = DEVCFG2_FPLLODIV_1,
                #elif (FCPUMHZ==68)
                    .FPLLIDIV   = DEVCFG2_FPLLIDIV_2,
                    .FPLLMUL    = DEVCFG2_FPLLMUL_17,
                    .FPLLODIV   = DEVCFG2_FPLLODIV_1,
                #else // 40MHz
                    .FPLLIDIV   = DEVCFG2_FPLLIDIV_2,
                    .FPLLMUL    = DEVCFG2_FPLLMUL_20,
                    .FPLLODIV   = DEVCFG2_FPLLODIV_2,
                #endif
                .UPLLIDIV   = DEVCFG2_UPLLIDIV_2,
                .UPLLEN     = DEVCFG2_UPLLEN_ENABLED
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
                .FPBDIV     = DEVCFG1_FPBDIV_1,
                .FCKSM      = DEVCFG1_CSECMD,
                .WDTPS      = DEVCFG1_WDTPS_1024,
                .WINDIS     = DEVCFG1_WINDIS_DISABLED,
                .FWDTEN     = DEVCFG1_FWDTEN_DISABLED
                //.FWDTWINSZ  = DEVCFG1_FWDTWINSZ_50
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

    #else

        #error "Configuration Bits for this proc. must still be defined. in config.h"

    #endif

#else // MPLABX/XC32-GCC
 
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

