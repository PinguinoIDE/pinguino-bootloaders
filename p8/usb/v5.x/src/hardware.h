/***********************************************************************
	Title:	USB Pinguino Bootloader
	File:	hardware.h
	Descr.: bootloader def. (version, speed, led, tempo.)
	Author:	Régis Blanchot <rblanchot@gmail.com>

	This file is part of Pinguino (http://www.pinguino.cc)
	Released under the LGPL license (http://www.gnu.org/licenses/lgpl.html)
***********************************************************************/

#ifndef _HARDWARE_H
#define _HARDWARE_H

#include "compiler.h"

/***********************************************************************
    USERLED pin
***********************************************************************/

#if   defined(__18f2455)  || defined(__18f4455)  || \
      defined(__18f2550)  || defined(__18f4550)  || \
      defined(__18lf2550) || defined(__18lf4550) || \
      defined(__18f25k50) || defined(__18f45k50)

    #define USERLED_PIN             4
    #define USERLED_PORT            LATA
    #define USERLED_TRIS            TRISA

#elif defined(__16f1459)  || \
      defined(__18f13k50) || defined(__18f14k50) || \
      defined(__18f26j50) || defined(__18f46j50) || \
      defined(__18f26j53) || defined(__18f46j53) || \
      defined(__18f27j53) || defined(__18f47j53)

    #define USERLED_PIN             2
    #define USERLED_PORT            LATC
    #define USERLED_TRIS            TRISC

#else

        #error "    --------------------------    "
        #error "    PIC NOT YET SUPPORTED !       "
        #error "    Please contact the developer: "
        #error "    <rblanchot@pinguino.cc>       "
        #error "    --------------------------    "

#endif

#define USERLED_MASK                (1 << USERLED_PIN)

#define UserLedInit()               USERLED_TRIS &= ~USERLED_MASK
#define UserLedOn()                 USERLED_PORT |= USERLED_MASK
#define UserLedOff()                USERLED_PORT &= ~USERLED_MASK
#define UserLedToggle()             USERLED_PORT ^= USERLED_MASK

/***********************************************************************
    USERBUTTON switch
***********************************************************************/

#if   defined(__18f2455)  || defined(__18f4455)  || \
      defined(__18f2550)  || defined(__18f4550)  || \
      defined(__18lf2550) || defined(__18lf4550) || \
      defined(__18f25k50) || defined(__18f45k50)

    #define USERBUTTON_PIN          5
    #define USERBUTTON_PORT         PORTA
    #define USERBUTTON_TRIS         TRISA

#elif defined(__16f1459)  || \
      defined(__18f13k50) || defined(__18f14k50) || \
      defined(__18f26j50) || defined(__18f46j50) || \
      defined(__18f26j53) || defined(__18f46j53) || \
      defined(__18f27j53) || defined(__18f47j53)

    #define USERBUTTON_PIN          2
    #define USERBUTTON_PORT         PORTD
    #define USERBUTTON_TRIS         TRISD

#else

        #error "    --------------------------    "
        #error "    PIC NOT YET SUPPORTED !       "
        #error "    Please contact the developer: "
        #error "    <rblanchot@pinguino.cc>       "
        #error "    --------------------------    "

#endif

#define USERBUTTON_MASK             (1 << USERBUTTON_PIN)

// 0 when pressed
#define UserButtonInit()            USERBUTTON_TRIS |= USERBUTTON_MASK
#define UserButtonNotPressed()      ((USERBUTTON_PORT) & (USERBUTTON_MASK))
#define UserButtonPressed()         !((USERBUTTON_PORT) & (USERBUTTON_MASK))

/***********************************************************************
    RESET switch
***********************************************************************/

#if   defined(__16f1459)

    #define ResetButtonNotPressed() PCONbits.nRMCLR
    #define ResetButtonPressed()    !PCONbits.nRMCLR

#else

    #define ResetButtonPressed()     (RCONbits.NOT_POR && RCONbits.NOT_BOR && RCONbits.IPEN == 0)
    #define ResetButtonNotPressed() !(RCONbits.NOT_POR && RCONbits.NOT_BOR && RCONbits.IPEN == 0)

#endif

/***********************************************************************
    VBUS pin
***********************************************************************/

#if   defined(__16f1459)  || \
      defined(__18f13k50) || defined(__18f14k50)

    // RA0 and RA1 are input only
    // TRIS bits will always read as ‘1’
    // #define VBUS_TRIS               TRISA

    #define VBUS_PORT               PORTA
    #if (SPEED == LOW_SPEED)
    #define VBUS_PIN                1   // D-
    #else
    #define VBUS_PIN                0   // D+
    #endif

#elif defined(__18f2455)  || defined(__18f4455)  || \
      defined(__18f2550)  || defined(__18f4550)  || \
      defined(__18lf2550) || defined(__18lf4550) || \
      defined(__18f25k50) || defined(__18f45k50) || \
      defined(__18f26j50) || defined(__18f46j50) || \
      defined(__18f26j53) || defined(__18f46j53) || \
      defined(__18f27j53) || defined(__18f47j53)
      
    // RA4 and RA5 are input only
    // TRIS bits will always read as ‘1’
    // #define VBUS_TRIS               TRISC

    #define VBUS_PORT               PORTC
    #if (SPEED == LOW_SPEED)
    #define VBUS_PIN                4   // D-
    #else
    #define VBUS_PIN                5   // D+
    #endif

#else

        #error "    --------------------------    "
        #error "    PIC NOT YET SUPPORTED !       "
        #error "    Please contact the developer: "
        #error "    <rblanchot@pinguino.cc>       "
        #error "    --------------------------    "

#endif

#define VBUS_MASK                   (1 << VBUS_PIN)
#define UsbOff()                    (!(VBUS_PORT & VBUS_MASK))
#define UsbOn()                     (VBUS_PORT & VBUS_MASK)

/**********************************************************************/

#endif //_HARDWARE_H
