/**********************************************************************
    Title:  Pinguino USB Bootloader
    File:   test.c
    Descr.: 8-bit PIC USB bootloader
    Author: Régis Blanchot <rblanchot@gmail.com>
***********************************************************************
    This file is part of Pinguino (http://www.pinguino.cc)
    Released under the LGPL license (http://www.gnu.org/licenses/lgpl.html)
***********************************************************************/

#include "compiler.h"
#include "types.h"
#include "config.h"
#include "hardware.h"
#include "vectors.h"
#if (BOOT_USE_DEBUG)                    // cf. Makefile
#include "serial.h"
#endif

#if defined(__XC8__) 
u8 __section("usbram") dummy; // to prevent a compilation error
#if defined(__18f46j50) || defined(__18f47j53)
u8 _mediumconst=0;
u8 _smallconst=0;
#endif
#endif

u8 i=0;

/***********************************************************************
 * MAIN
 **********************************************************************/

void main(void)
{
    #if defined(__18f26j50) || defined(__18f46j50) || \
        defined(__18f26j53) || defined(__18f46j53) || \
        defined(__18f27j53) || defined(__18f47j53)

    u16  pll_counter = 600;

    #endif

    // Init. oscillator and I/O
    // -----------------------------------------------------------------

/**********************************************************************/
    #if defined(__16f1459)
/**********************************************************************/

        //OSCTUNE = 0;
        OSCCON = 0b11111100;        // Config. bits FOSC are set to use INTOSC
                                    // SPLLEN   : 1 = PLL is enabled (see config.h)
                                    // SPLLMULT : 1 = 3x PLL is enabled (16x3=48MHz)
                                    // IRCF     : 1111 = HFINTOSC (16 MHz)
                                    // SCS      : 00 = use clock determined by FOSC
        #if (CRYSTAL == INTOSC)     

        ACTCON = 0x90;              // Enable active clock tuning with USB

        while (!OSCSTATbits.HFIOFS);// wait HFINTOSC frequency is stable (HFIOFS=1) 

        #else

        ACTCON = 0x00;

        #endif
        
        // Wait until the PLLRDY bit is set in the OSCSTAT register
        // before attempting to set the USBEN bit.
        while (!OSCSTATbits.PLLRDY);

        // The state of the ANSELx bits has no effect on digital output functions.
        ANSELA = 0;                 // all I/O to Digital mode
        ANSELB = 0;                 // all I/O to Digital mode
        ANSELC = 0;                 // all I/O to Digital mode
        
/**********************************************************************/
    #elif defined(__18f13k50) || defined(__18f14k50)
/**********************************************************************/

        OSCCONbits.SCS  = 0;        // 00 = Primary clock (determined by CONFIG1H[FOSC<3:0>])

        #if (CRYSTAL == 48)

        OSCTUNEbits.SPLLEN = 0;     // SPLLEN   : 0 = 4x PLL is disabled (see config.h)

        #else                       // 12 MHZ (4x12=48Mhz)

        OSCTUNEbits.SPLLEN = 1;     // SPLLEN   : 1 = 4x PLL is enabled (see config.h)

        #endif

        ANSEL  = 0;                 // On a POR, pins are configured as analog inputs
        ANSELH = 0;                 // all I/O to Digital mode

/**********************************************************************/
    #elif defined(__18f2455)  || defined(__18f4455)  || \
          defined(__18f2550)  || defined(__18f4550)  || \
          defined(__18lf2550) || defined(__18lf4550)
/**********************************************************************/

        #if (CRYSTAL == INTOSC)

            #error "Internal Osc. not supported"
            
        #else
        
        ADCON1 = 0x0F;              // all I/O to Digital mode
        CMCON  = 0x07;              // all I/O to Digital mode
     
        #endif
        
/**********************************************************************/
    #elif defined(__18f25k50) || defined(__18f45k50)
    //cf Datasheet - 3.10 Power-up Delays
/**********************************************************************/

        #if (CRYSTAL == INTOSC)

        OSCCON = 0x70;              // 0b01110000 : 111 = HFINTOSC (16 MHz)
                                    // enable the 16 MHz internal clock
                                    // Primary clock source (HFINTOSC or HSPLL)
                                    // is defined by FOSC<2:0> (cf. config.h)

        while(!OSCCONbits.HFIOFS);  // wait HFINTOSC frequency is stable (HFIOFS=1) 

        #endif

        /* ALREADY SET UP IN CONFIG BITS
        OSCCON2bits.PLLEN = 1;      // Enable the PLL

        while (pll_counter--);      // Wait > 2ms until the PLL locks.
                                    // Must be done before enabling USB module
        */

        ANSELA = 0;                 // all I/O to Digital mode
        ANSELB = 0;                 // all I/O to Digital mode
        ANSELC = 0;                 // all I/O to Digital mode
            
        #if defined(__18f45k50)

        ANSELD = 0;                 // all I/O to Digital mode
        ANSELE = 0;                 // all I/O to Digital mode

        #endif
        
/**********************************************************************/
    #elif defined(__18f26j50) || defined(__18f46j50)
/**********************************************************************/

        #if (CRYSTAL == INTOSC)

        OSCCON = 0x70;              // 0b01110000 : 111 = INTOSC (8 MHz)
                                    // enable the 8 MHz internal clock
                                    // Primary clock source (INTOSC or HSPLL)
                                    // is defined by FOSC<2:0> (cf. config.h)

        #endif
        
        OSCTUNEbits.PLLEN = 1;      // Enable the PLL

        while (pll_counter--);      // Wait > 2ms until the PLL locks.
                                    // Must be done before enabling USB module

        ANCON0 = 0xFF;              // AN0 to AN7  are Digital I/O
        ANCON1 = 0x1F;              // AN8 to AN12 are Digital I/O

/**********************************************************************/
    #elif defined(__18f26j53) || defined(__18f46j53) || \
          defined(__18f27j53) || defined(__18f47j53)
/**********************************************************************/

        #if (CRYSTAL == INTOSC)

        OSCCON = 0x70;              // 0b01110000 : 111 = INTOSC (8 MHz)
                                    // enable the 8 MHz internal clock
                                    // Primary clock source (INTOSC or HSPLL)
                                    // is defined by FOSC<2:0> (cf. config.h)

        while (!OSCCONbits.FLTS);   // wait INTOSC frequency is stable (FLTS=1) 

        #endif
        
        OSCTUNEbits.PLLEN = 1;      // Enable the PLL

        while (pll_counter--);      // Wait > 2ms until the PLL locks.
                                    // Must be done before enabling USB module

        ANCON0 = 0xFF;              // AN0 to AN7  are Digital I/O
        ANCON1 = 0x1F;              // AN8 to AN12 are Digital I/O

/**********************************************************************/
    #else
/**********************************************************************/

        #error "    --------------------------    "
        #error "    PIC NOT YET SUPPORTED !       "
        #error "    Please contact the developer: "
        #error "    <rblanchot@pinguino.cc>       "
        #error "    --------------------------    "

/**********************************************************************/
    #endif
/**********************************************************************/

    // Init. Serial if DEBUG is activated
    // -----------------------------------------------------------------

    #if (BOOT_USE_DEBUG)
    SerialInit(9600);
    SerialPrintChar('\f');          // Clear screen
    #endif

    // Init. timer1 to overroll after 65536*8/12000 = 43.7 ms
    // -----------------------------------------------------------------

    /*
     * bit 7,6 TMR1CS  = 00, Timer1 clock source is FOSC/4
     * bit 5,4 T1CKPS  = 11, 1:8 prescaler value
     * bit 3   T1OSCEN = 0 , Timer1 crystal driver is off
     * bit 2   T1SYNC  = 0 , this bit is ignored when TMR1CS = 00
     * bit 1   RD16    = 0 , read/write of Timer1 in two 8-bit operations
     * bit 0   TMR1ON  = 1 , enables Timer 1
     */

    TMR1L = 0;                      // clear Timer 1 counter because
    TMR1H = 0;                      // counter get an unknown value at reset
    T1CON = 0x31; //0b00110001;     // clock source is Fosc/4 (0b00)
                                    // prescaler 8 (0b11), timer 1 On 
    // Init. LED and Switch
    // -----------------------------------------------------------------

    UserLedInit();                  // USERLED Pin Output
    UserLedOn();                    // USERLED On

    // TEST STARTS HERE
    // -----------------------------------------------------------------
    
    #if (BOOT_USE_DEBUG)            // cf. Makefile
    if (ResetButtonPressed())
        SerialPrint("Button Reset\r\n");
    else
        SerialPrint("Power-On Reset\r\n");
    ResetButtonInit();
    #endif
    
    while (1)
    {
        if (PIR1bits.TMR1IF)        // If timer 1 has overflowed
        {
            PIR1bits.TMR1IF = 0;    // Allow interrupt source again
            UserLedToggle();        // Toggle the led
        }
    }
}
