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

void SerialInit(UINT32);
void SerialPrintChar(char);
void SerialPrint(const char *);
void SerialPrintNumber(INT32, UINT8);

#endif // _SERIAL_H_
