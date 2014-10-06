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

// Pinguino 32MX220, 32MX250 and 32MX270
#if defined(__32MX220F032B__) || \
    defined(__32MX250F128B__) || \
    defined(__32MX270F256B__)

    //cf. config.h
    #define FCPU_MHZ            40

    /** LED, USERLED or BOOTLED **/

    #define BOOTLED1             0

    #define mLED_Init()         TRISACLR = 1<<BOOTLED1      // Output
    #define mLED_1_On()         LATASET  = 1<<BOOTLED1
    #define mLED_1_Off()        LATACLR  = 1<<BOOTLED1
    #define mLED_Toggle()       LATAINV  = 1<<BOOTLED1

    /** SWITCH or USERBUTTON **/

    #define BOOTSWITCH          1

    #define mSWITCH_Init()      TRISASET = 1<<BOOTSWITCH    // Input
    #define mSWITCH_Pressed()   (!((PORTA) & (1<<BOOTSWITCH)))

    #define mLED_Both_Off()     mLED_1_Off()
    #define mLED_Both_On()      mLED_1_On()
    #define mLED_Only_1_On()    mLED_1_On()
    #define mLED_Only_2_On()    Nop()

    /** MEMORY AREA **/

    // The Flash page size on PIC32MX-1XX/2XX devices is 1 KB
    #define FLASH_PAGE_SIZE     1024

    // BootMemStart - virtual address with which application code starts
    // ie. reset vector of the application code (_RESET_ADDR) ???
    #define BootMemStart        0x9D000000

    // Address where user's program starts
    // Note: The NVM Address register must be loaded
    // with the physical address of the Flash memory
    // and not the virtual address
    #define UserAppMemStart     0x1D005000

#else

    //cf. config.h
    #define FCPU_MHZ            80

    /** LED, USERLED or BOOTLED **/

    #define BOOTLED1             0
    #define BOOTLED2             0

    #define mLED_Init()         {TRISACLR = 1<<BOOTLED1; TRISACLR = 1<<BOOTLED2;}
    #define mLED_1_On()         LATASET  = 1<<BOOTLED1
    #define mLED_1_Off()        LATACLR  = 1<<BOOTLED1
    #define mLED_2_On()         LATASET  = 1<<BOOTLED2
    #define mLED_2_Off()        LATACLR  = 1<<BOOTLED2
    #define mLED_Toggle()       {LATAINV  = 1<<BOOTLED1; LATAINV  = 1<<BOOTLED2;}

    /** SWITCH or USERBUTTON **/

    #define BOOTSWITCH          1

    #define mSWITCH_Init()      TRISASET = 1<<BOOTSWITCH
    #define mSWITCH_Pressed()   (PORTA & (1<<BOOTSWITCH))

    #define mLED_Both_Off()     {mLED_1_Off();mLED_2_Off();}
    #define mLED_Both_On()      {mLED_1_On();mLED_2_On();}
    #define mLED_Only_1_On()    {mLED_1_On();mLED_2_Off();}
    #define mLED_Only_2_On()    {mLED_1_Off();mLED_2_On();}

    /** MEMORY AREA **/

    #define FLASH_PAGE_SIZE     4096

    //Beginning of bootloader memory.
    #define BootMemStart	0x9D006000

    //Beginning of application program memory (not occupied by bootloader).
    #define UserAppMemStart	0x1D005000

    #error "This proc. is not yet supported. Please contact <rblanchot@gmail.com>"

#endif

#endif  //HARDWARE_H

