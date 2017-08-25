/***********************************************************************
    Title:	USB Pinguino Bootloader
    File:	hardware.h
    Descr.:     hardware bootloader def.
                mainly leds and switch pins
    Author:	Régis Blanchot <rblanchot@gmail.com>

    This file is part of Pinguino (http://www.pinguino.cc)
    Released under the LGPL license (http://www.gnu.org/licenses/lgpl.html)
***********************************************************************/

#ifndef _HARDWARE_H_
#define _HARDWARE_H_

// Pinguino 32MX220, 32MX250 and 32MX270
#if defined(__32MX220F032B__) || \
    defined(__32MX250F128B__) || \
    defined(__32MX270F256B__)

    /** LED, USERLED or BOOTLED **/

    #define BOOTLED1            0  // RA0=D13
    #define BOOTLED2            4  // RA4=D7

    /*****************************/

    #define BITBOOTLED1         1<<BOOTLED1
    #define BITBOOTLED2         1<<BOOTLED2

    #define mLED_Init()         { ANSELACLR = BITBOOTLED1 | BITBOOTLED2;\
                                  TRISACLR  = BITBOOTLED1 | BITBOOTLED2; }

    #define mLED_1_On()         LATASET  = BITBOOTLED1
    #define mLED_1_Off()        LATACLR  = BITBOOTLED1
    #define mLED_1_Toggle()     LATAINV  = BITBOOTLED1

    #define mLED_2_On()         LATASET  = BITBOOTLED2
    #define mLED_2_Off()        LATACLR  = BITBOOTLED2
    #define mLED_2_Toggle()     LATAINV  = BITBOOTLED2

    #define mLED_Toggle()       LATAINV  = BITBOOTLED1 | BITBOOTLED2

    /** SWITCH or USERBUTTON *****/

    #define BOOTSWITCH          1   // RA1=D14

    /*****************************/

    #define BITBOOTSWITCH       1<<BOOTSWITCH
    #define mSWITCH_Init()      { ANSELACLR = BITBOOTSWITCH; TRISASET = BITBOOTSWITCH; }
    #define mSWITCH_Pressed()   (!((PORTA) & BITBOOTSWITCH))
    #define mSWITCH_NotPressed() (PORTA & BITBOOTSWITCH)

#elif defined(__32MX470F512H__)

    /** LED, USERLED or BOOTLED **/

    #define BOOTLED1             13 // RB13
    #define BOOTLED2             0  // RB0 (To be defined)

    /*****************************/

    #define BITBOOTLED1         1<<BOOTLED1
    #define BITBOOTLED2         1<<BOOTLED2
    #define mLED_Init()         { ANSELBCLR = BITBOOTLED1 | BITBOOTLED2;\
                                  TRISBCLR  = BITBOOTLED1 | BITBOOTLED2; }

    #define mLED_1_On()         LATBSET  = BITBOOTLED1
    #define mLED_1_Off()        LATBCLR  = BITBOOTLED1
    #define mLED_1_Toggle()     LATBINV  = BITBOOTLED1

    #define mLED_2_On()         LATBSET  = BITBOOTLED2
    #define mLED_2_Off()        LATBCLR  = BITBOOTLED2
    #define mLED_2_Toggle()     LATBINV  = BITBOOTLED2

    #define mLED_Toggle()       LATBINV  = BITBOOTLED1 | BITBOOTLED2

    /** SWITCH or USERBUTTON *****/

    #define BOOTSWITCH          14 // RB14

    /*****************************/

    #define BITBOOTSWITCH       1<<BOOTSWITCH
    #define mSWITCH_Init()      { ANSELBCLR = BITBOOTSWITCH; TRISBSET = BITBOOTSWITCH; }
    #define mSWITCH_Pressed()   (!((PORTB) & BITBOOTSWITCH))
    #define mSWITCH_NotPressed() (PORTB & BITBOOTSWITCH)

#elif defined(__32MX440F256H__)

    /** LED, USERLED or BOOTLED **/

    #define BOOTLED1            6  // RG6 // green LED
    #define BOOTLED2            1  // RD1 // yellow LED

    /*****************************/

    #define BITBOOTLED1         1<<BOOTLED1
    #define BITBOOTLED2         1<<BOOTLED2

    #define mLED_1_On()         LATGSET  = BITBOOTLED1
    #define mLED_1_Off()        LATGCLR  = BITBOOTLED1
    #define mLED_1_Toggle()     LATGINV  = BITBOOTLED1

    #define mLED_2_On()         LATDSET  = BITBOOTLED2
    #define mLED_2_Off()        LATDCLR  = BITBOOTLED2
    #define mLED_2_Toggle()     LATDINV  = BITBOOTLED2

    #define mLED_Init()         { TRISGCLR = BITBOOTLED1; TRISDCLR = BITBOOTLED2; }
    #define mLED_Toggle()       { LATGINV  = BITBOOTLED1; LATDINV  = BITBOOTLED2; }

    /** SWITCH or USERBUTTON *****/

    #define BOOTSWITCH          0  // RD0

    /*****************************/

    #define BITBOOTSWITCH       1<<BOOTSWITCH
    #define mSWITCH_Init()      TRISDSET = BITBOOTSWITCH
    #define mSWITCH_Pressed()   (!((PORTD) & (BITBOOTSWITCH)))
    #define mSWITCH_NotPressed() (PORTD & BITBOOTSWITCH)

#endif

#define mLED_Both_Off()     { mLED_1_Off(); mLED_2_Off(); }
#define mLED_Both_On()      { mLED_1_On();  mLED_2_On();  }
#define mLED_Only_1_On()    { mLED_1_On();  mLED_2_Off(); }
#define mLED_Only_2_On()    { mLED_1_Off(); mLED_2_On();  }

#endif  //_HARDWARE_H_
