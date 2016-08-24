/*******************************************************************************
    Title:	USB Pinguino Bootloader
    File:	serial.h
    Descr.:     UART functions
    Author:	Régis Blanchot <rblanchot@gmail.com>

    This file is part of Pinguino (http://www.pinguino.cc)
    Released under the LGPL license (http://www.gnu.org/licenses/lgpl.html)
*******************************************************************************/

#ifndef _SERIAL_H_
#define _SERIAL_H_

void SerialInit(u32);
void SerialPrintChar(char);
void SerialPrint(const char *);
void SerialPrintLN(const char *);
void SerialPrintNumber(s16, u8);

#endif // _SERIAL_H_
