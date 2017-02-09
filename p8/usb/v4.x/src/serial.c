/***********************************************************************
    Title:  USB Pinguino Bootloader
    File:   serial.c
    Descr.: UART functions (useful  for debugging)
    Author: Régis Blanchot <rblanchot@gmail.com>

    This file is part of Pinguino (http://www.pinguino.cc)
    Released under the LGPL license (http://www.gnu.org/licenses/lgpl.html)
***********************************************************************/

#ifndef __SERIAL_C
#define __SERIAL_C

#include "compiler.h"
#include "types.h"
//#include "boot.h"

#if (BOOT_USE_DEBUG)            // This to prevent SDCC to compile serial.c
                                // when it is not necessary. XC8 is smarter.

#include "serial.h"

#define highByte(x)             ((u8) ((x) >> 8))
#define lowByte(x)              ((u8) ((x) & 0xFF))

/*
 * Compute the 16-bit baud rate divisor, given the bus frequency and baud rate
 * When BRG = 1 and BRG16 = 1 : BaudRate = FOSC / [4 (SPRG+1)]
 **/

#define BaudRateDivisor(f, b)   ((f/(4*b))-1)

/*
 * Enabling the EUSART for 8-bit asynchronous operation
 * 16F1459 : RB5 = RX (input), RB7 = TX (output)
 **/

void SerialInit(u32 baudrate)
{
    u16 spbrg = (u16)BaudRateDivisor(48*1000000UL, baudrate);

    #if defined(__16f1459)

    // 8-bit asynchronous operation
    RCSTA = 0;                  // 8-bit RX (RX9=0)
    TXSTA = 0;                  // 8-bit TX (TX9=0), asynchronous (SYNC=0)
    BAUDCON = 0;                // polarity : non-inverted
    
    // IO's
    //TRISBbits.TRISB5 = 1;       // RX is an input
    //TRISBbits.TRISB7 = 0;       // see SPEN bit below

    // Baud Rate
    SPBRGH = highByte(spbrg);   // set UART speed SPBRGH
    SPBRGL = lowByte(spbrg);    // set UART speed SPBRGL
    TXSTAbits.BRGH = 1;         // High Baud Rate
    BAUDCONbits.BRG16 = 1;      // Use 16-bit baud rate generator
    
    // Enable the Serial port
    TXSTAbits.TXEN = 1;         // Transmit Enabled
    RCSTAbits.CREN = 1;         // Receiver Enabled
    RCSTAbits.SPEN = 1;         // Serial Port Enabled (RX/TX pins as input/output)

    #elif defined(__18f1220)  || defined(__18f1320) || \
          defined(__18f14k22) || defined(__18lf14k22)

    // 8-bit asynchronous operation
    RCSTA = 0;                  // 8-bit RX (RX9=0)
    TXSTA = 0;                  // 8-bit TX (TX9=0), asynchronous (SYNC=0)
    BAUDCON = 0;                // polarity : non-inverted

    // Baud Rate
    SPBRGH = highByte(spbrg);   // set UART speed SPBRGH
    SPBRGL = lowByte(spbrg);    // set UART speed SPBRGL
    TXSTAbits.BRGH=1;           // set BRGH bit
    BAUDCTLbits.BRG16=1;        // set 16 bits SPBRG

    // Enable the Serial port
    TXSTAbits.TXEN = 1;         // Transmit Enabled
    RCSTAbits.CREN = 1;         // Receiver Enabled
    RCSTAbits.SPEN = 1;         // Serial Port Enabled (RX/TX pins as input/output)

    #elif defined(__18f2455)  || defined(__18f4455) || \
          defined(__18f2550)  || defined(__18f4550) || \
          defined(__18lf2550) || defined(__18lf4550) || \
          defined(__18f25k50) || defined(__18f45k50)

    // 8-bit asynchronous operation
    RCSTA = 0;                  // 8-bit RX (RX9=0)
    TXSTA = 0;                  // 8-bit TX (TX9=0), asynchronous (SYNC=0)
    BAUDCON = 0;                // polarity : non-inverted

    // IO's
    //TRISCbits.TRISC7= 1;        // RX1    set input

    // Baud Rate
    SPBRGH = highByte(spbrg);   // set UART speed SPBRGH
    SPBRG  = lowByte(spbrg);    // set UART speed SPBRGL
    TXSTAbits.BRGH = 1;         // set BRGH bit
    BAUDCONbits.BRG16 = 1;      // set 16 bits SPBRG

    // Enable the Serial port
    TXSTAbits.TXEN = 1;         // Transmit Enabled
    RCSTAbits.CREN = 1;         // Receiver Enabled
    RCSTAbits.SPEN = 1;         // Serial Port Enabled (RX/TX pins as input/output)

    #elif defined(__18f26j50) || defined(__18f46j50) || \
          defined(__18f26j53) || defined(__18f46j53) || \
          defined(__18f27j53) || defined(__18f47j53)

    // 8-bit asynchronous operation
    RCSTA = 0;                  // 8-bit RX (RX9=0)
    TXSTA = 0;                  // 8-bit TX (TX9=0), asynchronous (SYNC=0)
    BAUDCON = 0;                // polarity : non-inverted

    // IO's
    //TRISCbits.TRISC7= 1;        // RX1    set input

    // Baud Rate
    SPBRGH = highByte(spbrg);  // set UART speed SPBRGH
    SPBRG  = lowByte(spbrg);   // set UART speed SPBRGL
    TXSTA1bits.BRGH = 1;       // set BRGH bit
    BAUDCON1bits.BRG16 = 1;    // set 16 bits SPBRG

    // Enable the Serial port
    TXSTA1bits.TXEN = 1;        // Transmit Enabled
    RCSTA1bits.CREN = 1;        // Receiver Enabled
    RCSTA1bits.SPEN = 1;        // Serial Port Enabled (RX/TX pins as input/output)

    #endif
}

/*
 * Disabling the EUSART
 **/

#if defined(__18f26j50) || defined(__18f46j50) || \
    defined(__18f26j53) || defined(__18f46j53) || \
    defined(__18f27j53) || defined(__18f47j53)

    #define SerialDisable()         { TXSTA1 = 0; RCSTA1 = 0; }

#else

    #define SerialDisable()         { TXSTA = 0; RCSTA = 0; }

#endif

void SerialPrintChar(char c)
{
    #if defined(__18f26j50) || defined(__18f46j50) || \
        defined(__18f26j53) || defined(__18f46j53) || \
        defined(__18f27j53) || defined(__18f47j53)

    while (!TXSTA1bits.TRMT);
    TXREG1 = c;

    #else

    //RB 20160708 : breaks the USB process
    while (!TXSTAbits.TRMT);
    TXREG = c;

    #endif
}

void SerialPrint(const char *s)
{
    //for (; *s; ++s)
    while (*s)
        SerialPrintChar(*s++);
}

void SerialPrintLN(const char *s)
{
    //for (; *s; ++s)
    while (*s)
        SerialPrintChar(*s++);
    SerialPrintChar('\r');
    SerialPrintChar('\n');
}

// Note : max. signed value = 32768 = 5 digits
void SerialPrintNumber(s16 value, u8 base)
{
    u8 sign;

    s16 i;
    u16 v;    // absolute value

    u8 tmp[5];
    u8 *tp = tmp;       // pointer on tmp

    u8 string[5];
    u8 *sp = string;    // pointer on string

    if (value==0)
    {
        SerialPrintChar('0');
        return;
    }

    sign = ( (base == 10) && (value < 0) );

    if (sign)
        v = -value;
    else
        v = (u16)value;

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

#endif // BOOT_USE_DEBUG

#endif // __SERIAL_C
