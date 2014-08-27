/*******************************************************************************
	Title:	USB Pinguino Bootloader
	File:	hardware.h
	Descr.: hardware bootloader def. (led and switch)
	Author:	Régis Blanchot <rblanchot@gmail.com>

	This file is part of Pinguino (http://www.pinguino.cc)
	Released under the LGPL license (http://www.gnu.org/licenses/lgpl.html)
*******************************************************************************/

#ifndef HARDWARE_H
#define HARDWARE_H

// Pinguino 32MX220 and Pinguino 32MX250
#if defined(__32MX220F032B__) || defined(__32MX250F128B__)

    /** LED, USERLED or BOOTLED **/

    #define BOOTLED             0

    #define mLED_Init()         TRISACLR = 1<<BOOTLED;\
                                LATACLR  = 1<<BOOTLED;
    #define mLED_On()           LATASET  = 1<<BOOTLED;
    #define mLED_Off()          LATACLR  = 1<<BOOTLED;
    #define mLED_Toggle()       LATAINV  = 1<<BOOTLED;

    /** SWITCH or USERBUTTON **/

    #define BOOTSWITCH          1

    #define mSWITCH_Init()      TRISASET = 1<<BOOTSWITCH;
    #define SWITCH              (PORTA & (1<<BOOTSWITCH))

#else

    #error "To do. Please contact <rblanchot@gmail.com>"

#endif

#endif  //HARDWARE_H
