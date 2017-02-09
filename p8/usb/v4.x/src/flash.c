/***********************************************************************
    Title:  USB Pinguino Bootloader
    File:   flash.c
    Descr.: Write, erase and read flash functions
    Author: Régis Blanchot <rblanchot@gmail.com>

    This file is part of Pinguino (http://www.pinguino.cc)
    Released under the LGPL license (http://www.gnu.org/licenses/lgpl.html)
***********************************************************************/

#ifndef _FLASH_C
#define _FLASH_C

#include "compiler.h"
#include "flash.h"

/**********************************************************************/
#if defined(__16f1459)
/**********************************************************************/
/*
void Unlock()
{
    PMCON2 = 0x55;
    PMCON2 = 0xAA;
    PMCON1bits.WR = 1;
    NOP();
    NOP();
}
*/
/**********************************************************************/
#elif defined(__18f13k50) || defined(__18f14k50) || \
      defined(__18f2455)  || defined(__18f4455)  || \
      defined(__18f2550)  || defined(__18f4550)  || \
      defined(__18lf2550) || defined(__18lf4550) || \
      defined(__18f25k50) || defined(__18f45k50) || \
      defined(__18f26j50) || defined(__18f46j50) || \
      defined(__18f26j53) || defined(__18f46j53) || \
      defined(__18f27j53) || defined(__18f47j53)
/**********************************************************************/
/*
void Unlock()
{
    EECON2 = 0x55;
    EECON2 = 0xAA;
    EECON1bits.WR = 1;
}
*/
/**********************************************************************/
#endif
/**********************************************************************/

#endif //_FLASH_C
