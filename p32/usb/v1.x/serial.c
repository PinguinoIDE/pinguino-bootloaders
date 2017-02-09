/*******************************************************************************
    Title:	USB Pinguino Bootloader
    File:	serial.c
    Descr.:     UART functions (useful  for debugging)
    Author:	Régis Blanchot <rblanchot@gmail.com>

    This file is part of Pinguino (http://www.pinguino.cc)
    Released under the LGPL license (http://www.gnu.org/licenses/lgpl.html)
*******************************************************************************/

#ifndef __SERIAL_C
#define __SERIAL_C

#include "p32xxxx.h"
#include "typedefs.h"
#include "serial.h"
#include "core.h"

// Compute the 16-bit baud rate divisor, given the bus frequency and baud rate.
#define BaudRateDivisor(baud)	((( (FPB) / 8 + (baud) ) / (baud) / 2) - 1)

// U1RX on RB2 (Pinguino pin 10)
// U1TX on RB3 (Pinguino pin 9 )
void SerialInit(UINT32 baudrate)
{
    // PPS for UART1 pins
    #if defined(__32MX220F032B__) || \
        defined(__32MX250F128B__) || \
        defined(__32MX270F256B__)

        SYSKEY = 0;                // ensure OSCCON is locked
        SYSKEY = 0xAA996655;       // Write Key1 to SYSKEY
        SYSKEY = 0x556699AA;       // Write Key2 to SYSKEY
        CFGCONbits.IOLOCK=0;       // unlock configuration
        CFGCONbits.PMDLOCK=0;
        U1RXRbits.U1RXR = 0b0100;  // Define U1RX as RB2 ( D10 )
        RPB3Rbits.RPB3R = 0b0001;  // Define U1TX as RB3 ( D9 )
        CFGCONbits.IOLOCK=1;       // relock configuration
        CFGCONbits.PMDLOCK=1;
        SYSKEY = 0x12345678;       // Write any value other than Key1 or Key2

        // UART1 pins direction
        TRISBbits.TRISB2=1;        // RB2 is input ( RX )
        TRISBbits.TRISB3=0;        // RB3 is output ( TX )

    #else

        #error "Debug option still to implement for this proc."

    #endif

    // UART1 configuration
    U1BRG     = BaudRateDivisor(baudrate);
    U1MODE    = 0;
    U1STA     = 0;
    // Receiver and Transmitter enable
    U1STASET  = _U1STA_URXEN_MASK | _U1STA_UTXEN_MASK;
    // UART Enable, 8-bit data, no parity
    // BRGH=0 : Standard Speed mode ? 16x baud clock enabled
    U1MODESET = _U1MODE_ON_MASK;
}

void SerialPrintChar(char c)
{
    // Wait for transmitter shift register empty.
    while (!(U1STA & _U1STA_TRMT_MASK));

    // Send byte
    U1TXREG = c;

    // Wait for transmitter shift register empty.
    while (!(U1STA & _U1STA_TRMT_MASK));
}

void SerialPrint(const char *s)
{
    for (; *s; ++s)
        SerialPrintChar(*s);
}

void SerialPrintNumber(INT32 value, UINT8 base)
{
    UINT8 sign;

    INT32 i;
    UINT32 v;    // absolute value

    UINT8 tmp[12];
    UINT8 *tp = tmp;       // pointer on tmp

    UINT8 string[12];
    UINT8 *sp = string;    // pointer on string

    if (value==0)
    {
        SerialPrintChar('0');
        return;
    }

    sign = ( (base == 10) && (value < 0) );

    if (sign)
        v = -value;
    else
        v = (UINT32)value;

    //while (v || tp == tmp)
    while (v)
    {
        i = v % base;
        v = v / base;

        if (i < 10)
            *tp++ = i + '0';
        else
            *tp++ = i + 'A' - 10;
    }

    // start of string
    if (sign)
        *sp++ = '-';

    // backwards writing
    while (tp > tmp)
        *sp++ = *--tp;

    // end of string
    *sp = 0;

    SerialPrint(string);
}

#endif // __SERIAL_C
