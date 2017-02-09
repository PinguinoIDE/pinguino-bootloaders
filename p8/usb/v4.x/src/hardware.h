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

/***********************************************************************
    LED pin
***********************************************************************/

#if   defined(__18f2455)  || defined(__18f4455)  || \
      defined(__18f2550)  || defined(__18f4550)  || \
      defined(__18lf2550) || defined(__18lf4550) || \
      defined(__18f25k50) || defined(__18f45k50)

    #define LED_PIN                 4
    #define LED_PORT                LATA
    #define LED_TRIS                TRISA

#elif defined(__16f1459)  || \
      defined(__18f13k50) || defined(__18f14k50) || \
      defined(__18f26j50) || defined(__18f46j50) || \
      defined(__18f26j53) || defined(__18f46j53) || \
      defined(__18f27j53) || defined(__18f47j53)

    #define LED_PIN                 2
    #define LED_PORT                LATC
    #define LED_TRIS                TRISC

#endif

#define LED_MASK                    (1 << LED_PIN)

#define LedOut()                    LED_TRIS &= ~LED_MASK
#define LedOn()                     LED_PORT |= LED_MASK
#define LedOff()                    LED_PORT &= ~LED_MASK
#define LedToggle()                 LED_PORT ^= LED_MASK

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

#endif

#define VBUS_MASK                   (1 << VBUS_PIN)
#define UsbOn()                     (!(VBUS_PORT & VBUS_MASK))
#define UsbOff()                    (VBUS_PORT & VBUS_MASK)

#endif //_HARDWARE_H
