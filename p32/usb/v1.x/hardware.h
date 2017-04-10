/*******************************************************************************
    Title:	USB Pinguino Bootloader
    File:	hardware.h
    Descr.:     hardware bootloader def.
                mainly leds and switch pins
    Author:	Régis Blanchot <rblanchot@gmail.com>

    This file is part of Pinguino (http://www.pinguino.cc)
    Released under the LGPL license (http://www.gnu.org/licenses/lgpl.html)
*******************************************************************************/

#ifndef _HARDWARE_H_
#define _HARDWARE_H_

// Pinguino 32MX220, 32MX250 and 32MX270
#if defined(__32MX220F032B__) || \
    defined(__32MX250F128B__) || \
    defined(__32MX270F256B__)

    /** LED, USERLED or BOOTLED **/

    #define BOOTLED1            0  // RA0=D13
    #define BOOTLED2            4  // RA4=D7

    #define mLED_Init()         { ANSELA = 0; \
                                  TRISACLR = (1<<BOOTLED1) | (1<<BOOTLED2); }

    #define mLED_1_On()         LATASET  = 1<<BOOTLED1
    #define mLED_1_Off()        LATACLR  = 1<<BOOTLED1
    #define mLED_1_Toggle()     LATAINV  = 1<<BOOTLED1

    #define mLED_2_On()         LATASET  = 1<<BOOTLED2
    #define mLED_2_Off()        LATACLR  = 1<<BOOTLED2
    #define mLED_2_Toggle()     LATAINV  = 1<<BOOTLED2

    #define mLED_Toggle()       LATAINV  = (1<<BOOTLED1) | (1<<BOOTLED2)

    /** SWITCH or USERBUTTON **/

    #define BOOTSWITCH          1   // RA1=D14

    #define mSWITCH_Init()      { ANSELA = 0; \
                                  TRISASET = 1<<BOOTSWITCH; }    // Input
    #define mSWITCH_Pressed()   (!((PORTA) & (1<<BOOTSWITCH)))

#else // __32MX470F512H__

    //cf. config.h
    #define FCPU_MHZ            80

    /** LED, USERLED or BOOTLED **/

    #define BOOTLED1             13 // RB13
    #define BOOTLED2             0  // Debug LED, TBD

    #define mLED_Init()         { ANSELB = 0; \
                                  TRISBCLR = (1<<BOOTLED1) | (1<<BOOTLED2); }

    #define mLED_1_On()         LATBSET  = 1<<BOOTLED1
    #define mLED_1_Off()        LATBCLR  = 1<<BOOTLED1
    #define mLED_1_Toggle()     LATBINV  = 1<<BOOTLED1

    #define mLED_2_On()         LATBSET  = 1<<BOOTLED2
    #define mLED_2_Off()        LATBCLR  = 1<<BOOTLED2
    #define mLED_2_Toggle()     LATBINV  = 1<<BOOTLED2

    #define mLED_Toggle()       LATBINV  = (1<<BOOTLED1) | (1<<BOOTLED2)

    /** SWITCH or USERBUTTON **/

    #define BOOTSWITCH          14 // RB14

    #define mSWITCH_Init()      { ANSELB = 0; \
                                  TRISBSET = 1<<BOOTSWITCH; }    // Input
    #define mSWITCH_Pressed()   (!((PORTB) & (1<<BOOTSWITCH)))

#endif

    /*
    #define mLED_Both_Off()     mLED_1_Off()
    #define mLED_Both_On()      mLED_1_On()
    #define mLED_Only_1_On()    mLED_1_On()
    #define mLED_Only_2_On()    Nop()
    */
    #define mLED_Both_Off()     { mLED_1_Off(); mLED_2_Off(); }
    #define mLED_Both_On()      { mLED_1_On();  mLED_2_On();  }
    #define mLED_Only_1_On()    { mLED_1_On();  mLED_2_Off(); }
    #define mLED_Only_2_On()    { mLED_1_Off(); mLED_2_On();  }

#endif  //_HARDWARE_H_
