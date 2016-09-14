/***********************************************************************
    Title:	USB Pinguino Bootloader
    File:	compiler.h
    Descr.: manage compatibility between SDCC and XC8 compiler
    Author:	Régis Blanchot <rblanchot@gmail.com>
    This file is part of Pinguino (http://www.pinguino.cc)
    Released under the LGPL license (http://www.gnu.org/licenses/lgpl.html)
***********************************************************************/

#ifndef _COMPILER_H
#define _COMPILER_H

    #ifdef __XC8__

        #pragma warning disable 1088
        #pragma warning disable 1262 // object "_ep_bdt" lies outside available data space
        #pragma warning disable 1471 // UserAppFnc() indirect function call via a NULL pointer ignored

        #include <xc.h>

        #ifdef _16F1459
            #define __16f1459
        #endif
        
        #ifdef _18F13K50
            #define __18f13k50
        #endif
        #ifdef _18F14K50
            #define __18f14k50
        #endif
        #ifdef _18F2455
            #define __18f2455
        #endif
        #ifdef _18F4455
            #define __18f4455
        #endif
        #ifdef _18F2550
            #define __18f2550
        #endif
        #ifdef _18LF2550
            #define __18lf2550
        #endif
        #ifdef _18F4550
            #define __18f4550
        #endif
        #ifdef _18LF4550
            #define __18lf4550
        #endif
        #ifdef _18F25K50
            #define __18f25k50
        #endif
        #ifdef _18F45K50
            #define __18f45k50
        #endif
        #ifdef _18F26J50
            #define __18f26j50
        #endif
        #ifdef _18F46J50
            #define __18f46j50
        #endif
        #ifdef _18F26J53
            #define __18f26j53
        #endif
        #ifdef _18F46J53
            #define __18f46j53
        #endif
        #ifdef _18F27J53
            #define __18f27j53
        #endif
        #ifdef _18F47J53
            #define __18f47j53
        #endif
        
        #define __asm__(x) asm(x)

    #else // SDCC

        #ifdef __SDCC_pic14
            #include <pic16regs.h>
        #else
            #include <pic18fregs.h>
        #endif

        #ifdef __SDCC_PIC16F1459
            #define __16f1459
        #endif

    #endif

    #ifdef __16f1459
        #define SERIAL      {'1','6','F','1','4','5','9'}
    #endif
    #ifdef __18f13k50
        #define SERIAL      {'1','8','F','1','3','K','5','0'}
    #endif
    #ifdef __18f14k50
        #define SERIAL      {'1','8','F','1','4','K','5','0'}
    #endif
    #ifdef __18f2455
        #define SERIAL      {'1','8','F','2','4','5','5'}
    #endif
    #ifdef __18f4455
        #define SERIAL      {'1','8','F','4','4','5','5'}
    #endif
    #ifdef __18f2550
        #define SERIAL      {'1','8','F','2','5','5','0'}
    #endif
    #ifdef __18lf2550
        #define SERIAL      {'1','8','L','F','2','5','5','0'}
    #endif
    #ifdef __18f4550
        #define SERIAL      {'1','8','F','4','5','5','0'}
    #endif
    #ifdef __18lf4550
        #define SERIAL      {'1','8','L','F','4','5','5','0'}
    #endif
    #ifdef __18f25k50
        #define SERIAL      {'1','8','F','2','5','K','5','0'}
    #endif
    #ifdef __18f45k50
        #define SERIAL      {'1','8','F','4','5','K','5','0'}
    #endif
    #ifdef __18f26j50
        #define SERIAL      {'1','8','F','2','6','J','5','0'}
    #endif
    #ifdef __18f46j50
        #define SERIAL      {'1','8','F','4','6','J','5','0'}
    #endif
    #ifdef __18f26j53
        #define SERIAL      {'1','8','F','2','6','J','5','3'}
    #endif
    #ifdef __18f46j53
        #define SERIAL      {'1','8','F','4','6','J','5','3'}
    #endif
    #ifdef __18f27j53
        #define SERIAL      {'1','8','F','2','7','J','5','3'}
    #endif
    #ifdef __18f47j53
        #define SERIAL      {'1','8','F','4','7','J','5','3'}
    #endif

#endif /* _COMPILER_H */
