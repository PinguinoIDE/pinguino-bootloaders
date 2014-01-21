/*---------------------------------------------------------------------
     _____  _____ __   _  ______ _     _ _____ __   _  _____ 
    |_____]   |   | \  | |  ____ |     |   |   | \  | |     |
    |       __|__ |  \_| |_____| |_____| __|__ |  \_| |_____|
                                                             
    Pinguino Programmer

    Author:         Regis Blanchot <rblanchot@gmail.com>
                    Based on Jean-Pierre Mandon's self programmer
                    and Microchip Application Note
    Description:    Turn your Pinguino board into a PIC Programmer
    First release:  2010
    Last release:   2013-11-15

    CHANGELOG:
    02/07/2011  Fixed bug PGM pin is no longer used
    15/11/2013  Added support to PIC18F2XXX, 4XXX, 2XJXX and 4XJXX
    
    USAGE:
    1/ Connect your Pinguino board to USB port
    2/ Compile and Upload this program
    3/ Deconnect your Pinguino Board
    4/ Wire your Pinguino to a blanck chip (see below)
    5/ Start programmer8.py (ex : programmer8.py blink.hex 18f47j53)
    6/ Connnect your Pinguino to USB port
    7/ That's it. The blank chip has been programmed.

    This program is free software you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the
    Free Software Foundation, Inc.
    51 Franklin Street, Fifth Floor
    Boston, MA  02110-1301  USA
---------------------------------------------------------------------*/

/// --------------------------------------------------------------------
/// Pins definition
/// --------------------------------------------------------------------

#define _PGC_ 1       // connected to the _PGC_ pin (RB6) of the blank chip
#define _PGD_ 2       // connected to the _PGD_ pin (RB7) of the blank chip
#define _VPP_ 3       // connected to the _VPP_ pin (MCLR) of the blank chip
#define _VCC_ 6       // connected to the VDD pin power on transistor
#define _LED_ 5       //
                    // VSS connected to ground
                    
/// --------------------------------------------------------------------
/// Timings
/// --------------------------------------------------------------------

#define P1                              1   // us
#define P2                              1   // us
#define P2A                             400 // ns
#define P2B                             400 // ns
#define P3                              15  // ns
#define P4                              15  // ns
#define P5                              40  // ns
#define P5A                             40  // ns
#define P6                              20  // ns

#define P9                              4   // ms PGC High Time (minimum programming time)

#define P10                             100 // us

#define P11                             600 // ms Delay to allow Bulk Erase to Occur

#define P11A                            4   // ms
#define P12                             2   // us

#define P13                             100 // ns
#define P14                             10  // ns
#define P15                             2   // us
#define P16                             0   // s
#define P17                             100 // ns max.
#define P18                             0   // us

#define P19                             4   // ms Delay from First MCLR ↓ to First PGC ↑ for Key Sequence on PGD 

#define P20                             50  // ns

/// --------------------------------------------------------------------
/// USB Block structure
/// --------------------------------------------------------------------

#define CMD                             0
#define DATA_LEN                        1
#define DATA_ADDR_LO                    2
#define DATA_ADDR_HI                    3
#define DATA_ADDR_UP                    4
#define DATA_START                      5
#define DATABLOCKSIZE                   32

/// --------------------------------------------------------------------
/// Commands
/// --------------------------------------------------------------------

#define READ_VERSION                    0x00
#define READ_FLASH                      0x01
#define WRITE_FLASH                     0x02
#define ERASE_FLASH                     0x03
#define READ_EEDATA                     0x04
#define WRITE_EEDATA                    0x05
#define READ_CONFIG                     0x06
#define WRITE_CONFIG                    0x07
#define RESET                           0xFF

/// --------------------------------------------------------------------
/// Globals
/// --------------------------------------------------------------------

u8 receivedbyte;        // number of recceived bytes
u8 devid,devid1,devid2; // Device ID
char usbBuf[64];        // data received from USB bus

/// --------------------------------------------------------------------
/// Send a command to the target chip
/// --------------------------------------------------------------------

void send_command(u8 cmd, u8 lowbyte, u8 highbyte)
{
    u8 i;

    // send command
    pinMode(_PGD_, INPUT);
    
    for (i=0; i<4; i++)
    {
        digitalWrite(_PGC_,HIGH);
        if ( (cmd & 1) == 1 )           // bit set ?
            digitalWrite(_PGD_,HIGH);
        else
            digitalWrite(_PGD_,LOW);
        digitalWrite(_PGC_,LOW);
        cmd = cmd >> 1;                 // next bit
    }
    
    // delay P5

    // send low byte
    
    for (i=0; i<8; i++)
    {
        digitalWrite(_PGC_,HIGH);
        if ((lowbyte&1)==1)
            digitalWrite(_PGD_,HIGH);
        else
            digitalWrite(_PGD_,LOW);
        digitalWrite(_PGC_,LOW);
        lowbyte=lowbyte>>1;
    }
    
    // send high byte
    
    for (i=0; i<8; i++)
    {
        digitalWrite(_PGC_,HIGH);
        if ((highbyte&1)==1)
            digitalWrite(_PGD_,HIGH);
        else
            digitalWrite(_PGD_,LOW);
        digitalWrite(_PGC_,LOW);
        highbyte=highbyte>>1;
    }
    
    digitalWrite(_PGC_,LOW);
    digitalWrite(_PGD_,LOW);
    
    // delay P5A
}

/// --------------------------------------------------------------------
/// Special end of command write
/// DS39622K Page 20 figure 3.5 (Flash Microcontroller Programming Specification)
/// --------------------------------------------------------------------

void end_writecmd()
{
    u8 i;
    
    digitalWrite(_PGD_,LOW);
    
    for (i=0; i<3; i++)
    {
        digitalWrite(_PGC_,HIGH);
        digitalWrite(_PGC_,LOW);
    }
    
    digitalWrite(_PGC_,HIGH);
    delay(P9);                      // JP : 6 ms
    digitalWrite(_PGC_,LOW);
    delayMicroseconds(P10);         // JP : 500 us
    
    for (i=0; i<16; i++)
    {
        digitalWrite(_PGC_,HIGH);
        digitalWrite(_PGC_,LOW);
    }
}

/// --------------------------------------------------------------------
/// Read Flash memory
/// Code memory is accessed one byte at a time via the
/// 4-bit command, ‘1001’ (table read, post-increment).
/// The contents of memory pointed to by the Table Pointer
/// (TBLPTRU:TBLPTRH:TBLPTRL) are serially output on _PGD_.
/// --------------------------------------------------------------------

u8 read_flash()
{
    u8 i, by, bi;

    pinMode(_PGD_, OUTPUT);

    for (i=0; i<8; i++)
    {
        digitalWrite(_PGC_, HIGH);

        bi = digitalRead(_PGD_);

        if (bi) // 1
            bitSet(by, i);
        else    // 0
            bitClear(by, i);

        digitalWrite(_PGC_, LOW);
    }

    pinMode(_PGD_, INPUT);
}

/// --------------------------------------------------------------------
/// IO initialization
/// --------------------------------------------------------------------

void setup()
{
    pinMode(_PGC_,OUTPUT);
    pinMode(_PGD_,OUTPUT);
    pinMode(_VPP_,OUTPUT);
    pinMode(_VCC_,OUTPUT);
    pinMode(_LED_,OUTPUT);

    digitalWrite(_PGC_,LOW);
    digitalWrite(_PGD_,LOW);
    digitalWrite(_VPP_,LOW);
    digitalWrite(_VCC_,HIGH);
    digitalWrite(_LED_,LOW);
}

/// --------------------------------------------------------------------
/// Wait for Commands from PC
/// --------------------------------------------------------------------

void loop()
{
    u8  i;
    u8  len;
    u8  code1, code2;
    u8  addrl, addrh, addru;
    u32 key = 0b01001101010000110100100001010000;

    receivedbyte=0;

    // -----------------------------------------------------------------
    // wait until data is received from PC
    // -----------------------------------------------------------------

    do {
      receivedbyte = USB.read(usbBuf);
    } while (receivedbyte == 0);

    // size of block
    len = usbBuf[DATA_LEN];

    // block's address
    addrl = usbBuf[DATA_ADDR_LO];
    addrh = usbBuf[DATA_ADDR_HI];
    addru = usbBuf[DATA_ADDR_UP];

    // -----------------------------------------------------------------
    // put target on
    // -----------------------------------------------------------------

    digitalWrite(_VCC_, LOW);              // target on
    delay(1000);
    digitalWrite(_LED_, HIGH);             // put _LED_ on

    // -----------------------------------------------------------------
    // entering high-voltage program/verify mode
    // -----------------------------------------------------------------

    if ( usbBuf[CMD] == 'j' )
    {
        // 1- Voltage is briefly applied to the MCLR pin.
        digitalWrite(_VPP_, HIGH);
        delayMicroseconds(4);
        digitalWrite(_VPP_, LOW);
        // delay P19 > 4ms
        
        // 2- A 32-bit key sequence (4D434850) is presented on _PGD_.
        for (i=0; i<32; i++)
        {
            digitalWrite(_PGC_, HIGH);
            if ((key & 1)==1)
                digitalWrite(_PGD_, HIGH);
            else
                digitalWrite(_PGD_, LOW);
            digitalWrite(_PGC_, LOW);
            key=key>>1;
        }
        
        // 3- Voltage is reapplied to MCLR and held.
        digitalWrite(_VPP_, HIGH);

        USB.write("OK", 2);
    }

    if ( usbBuf[CMD] == 'n' )
    {
        // 1- hold _PGC_ and _PGD_ low and then raise MCLR/_VPP_/RE3 to VIHH
        digitalWrite(_PGC_,LOW);
        digitalWrite(_PGD_,LOW);
        digitalWrite(_VPP_, HIGH);
        delayMicroseconds(4);

        USB.write("OK", 2);
    }

    // -----------------------------------------------------------------
    // read target flash memory
    // -----------------------------------------------------------------
    
    if ( usbBuf[CMD] == READ_FLASH )
    {
        // Set Table Pointer for Device ID to be read

        send_command(0x00,0x3F,0x0E);    // MOVLW    3Fh
        send_command(0x00,0xF8,0x6E);    // MOVWF    TBLPTRU 
        send_command(0x00,0xFF,0x0E);    // MOVLW    FFh
        send_command(0x00,0xF7,0x6E);    // MOVWF    TBLPRTH
        send_command(0x00,0xFE,0x0E);    // MOVLW    FEh
        send_command(0x00,0xF6,0x6E);    // MOVWF    TBLPTRL

        send_command(0x09,0x00,0x00);    // TBLRD*+ 
        devid1 = read_flash();

        send_command(0x09,0x00,0x00);    // TBLRD*+ 
        devid2 = read_flash();

        devid = (devid2 << 8) + devid1;
        devid &= 0xFFE0;

        USB.printf("%d", devid);
    }

    // -----------------------------------------------------------------
    // erase target chip
    // -----------------------------------------------------------------

    else if ( usbBuf[CMD] == ERASE_FLASH )
    {
        if ((devid2 == 0x4C) || (devid2 == 0x58)) // xxj50 or xxj53 
        {
            code1 = 0x01;
            code2 = 0x80;
        }
        else
        {
            code1 = 0x3F;
            code2 = 0x8F;
        }
        
        send_command(0x00,0x3C,0x0E);   // MOVLW    3Ch
        send_command(0x00,0xF8,0x6E);   // MOVWF    TBLPTRU
        send_command(0x00,0x00,0x0E);   // MOVLW    00h
        send_command(0x00,0xF7,0x6E);   // MOVWF    TBLPTRH
        send_command(0x00,0x05,0x0E);   // MOVLW    05h
        send_command(0x00,0xF6,0x6E);   // MOVWF    TBLPTRL
        send_command(0x0C,code1,code1); // Write code1 to 3C0005h
        send_command(0x00,0x3C,0x0E);   // MOVLW    3Ch
        send_command(0x00,0xF8,0x6E);   // MOVWF    TBLPTRU
        send_command(0x00,0x00,0x0E);   // MOVLW    00h
        send_command(0x00,0xF7,0x6E);   // MOVWF    TBLPTRH
        send_command(0x00,0x04,0x0E);   // MOVLW    04h
        send_command(0x00,0xF6,0x6E);   // MOVWF    TBLPTRL
        send_command(0x0C,code2,code2); // Write code2 to 3C0004h
        send_command(0x00,0x00,0x00);   // NOP,  to erase entire device.
        send_command(0x00,0x00,0x00);   // Hold _PGD_ low until erase completes.

        // delay P11 > 524 ms
        
        USB.write("OK", 2);
    }
    
    // -----------------------------------------------------------------
    // write target flash memory
    // -----------------------------------------------------------------

    /** CAUTION : WRITE BUFFER SIZES CAN CHANGE FROM A PIC TO ANOTHER **/
    
    else if ( usbBuf[CMD] == WRITE_FLASH )
    {
        // Direct access to code memory and enable writes

        if ((devid2 == 0x4C) || (devid2 == 0x58)) // xxj50 or xxj53 
        {
            send_command(0x00,0xA6,0x84);// BSF      EECON1, WREN
        }
        else
        {
            send_command(0x00,0xA6,0x8E);// BSF      EECON1, EE_PGD_
            send_command(0x00,0xA6,0x9C);// BCF      EECON1, CFGS
        }

        // Set Table Pointer for flash memory to be written

        send_command(0x00,0x00,0x0E);   // MOVLW    00h
        send_command(0x00,0xF8,0x6E);   // MOVWF    TBLPTRU
        send_command(0x00,0x00,0x0E);   // MOVLW    00h
        send_command(0x00,0xF7,0x6E);   // MOVWF    TBLPTRH
        send_command(0x00,0x00,0x0E);   // MOVLW    00h
        send_command(0x00,0xF6,0x6E);   // MOVWF    TBLPTRL

        // Repeat for all but the last two bytes.

        for (i=0; i< (len-2); i+=2)
        {
            send_command(   0b00001101, // Table Write, Post-Increment by 2  
                            usbBuf[DATA_START + i    ],
                            usbBuf[DATA_START + i + 1] );
        }

        // Load write buffer for last two bytes.

        send_command(       0b00001111, // Write 2 bytes and start programming
                            usbBuf[DATA_START + len - 2],
                            usbBuf[DATA_START + len - 1] );

        send_command(0x00,0x00,0x00);   // Hold _PGC_ high for time P9 and low for time P10.
        end_writecmd();

        toggle(_LED_);
        USB.write("OK", 2);
    }
    
    // -----------------------------------------------------------------
    // write target configuration bits
    // -----------------------------------------------------------------

    else if ( usbBuf[CMD] == WRITE_CONFIG )
    {
        digitalWrite(_LED_,HIGH);

        // Enable writes and direct access to configuration memory

        if ((devid2 == 0x4C) || (devid2 == 0x58)) // xxj50 or xxj53 
        {
            send_command(0x00,0xA6,0x84);// BSF      EECON1, WREN
            send_command(0x00,0xA6,0x88);// BSF      EECON1, FREE
            send_command(0x00,0xA6,0x82);// BSF      EECON1, WR
            send_command(0x00,0x00,0x00);// NOP - Hold _PGD_ high for time P10.
        }
        else
        {
            send_command(0x00,0xA6,0x8E);// BSF      EECON1, EE_PGD_
            send_command(0x00,0xA6,0x8C);// BSF      EECON1, CFGS
        }
    
        // Set Table Pointer for configuration byte to be written

        send_command(0x00,0x30,0x0E);    // MOVLW    30h
        send_command(0x00,0xF8,0x6E);    // MOVWF    TBLPTRU 
        send_command(0x00,0x00,0x0E);    // MOVLW    00h
        send_command(0x00,0xF7,0x6E);    // MOVWF    TBLPRTH
        send_command(0x00,0x00,0x0E);    // MOVLW    00h
        send_command(0x00,0xF6,0x6E);    // MOVWF    TBLPTRL
            
        // Write Config. bits

        for (i=0; i<len; i++)
        {
            // <MSB ignored><LSB> to even address
            // <MSB><LSB ignored> to odd address

            send_command(   0b00001111, // Table Write, Start Programming 
                            usbBuf[DATA_START + i],
                            usbBuf[DATA_START + i] );

            send_command(0x00,0x00,0x00);  // Hold _PGC_ high for time P9 and low for time P10.
            end_writecmd();                // _PGC_ High/Low
            send_command(0x00, (addrl + i), 0x0E);// MOVLF addrl
            send_command(0x00, 0xF6, 0x6E);// MOVWF TBLPTRL
        }

        if ((devid2 == 0x4C) || (devid2 == 0x58)) // xxj50 or xxj53 
        {
            send_command(0x00,0xA6,0x94);// BCF      EECON1, WREN
        }
        
        USB.write("OK", 2);
    }

    // -----------------------------------------------------------------
    // exiting high-voltage program/verify mode
    // -----------------------------------------------------------------

    /*
     * Exiting Program/Verify mode is done by removing VIH from MCLR.
     * The only requirement for exit is that an interval, P16, should elapse
     * between the last clock and program signals on _PGC_ and _PGD_ before
     * removing VIH.
     * When VIH is reapplied to MCLR, the device will enter the ordinary
     * operational mode and begin executing the application instructions.
    */

    else if ( usbBuf[CMD] == RESET )
    {
        delay(100);
        digitalWrite(_PGD_, LOW);
        digitalWrite(_PGC_, LOW);
        digitalWrite(_VPP_, LOW);
        delay(20);
        digitalWrite(_VCC_, HIGH); // Target off
        delay(1000);
        digitalWrite(_LED_, LOW);  // _LED_ off
        
        digitalWrite(_VPP_, HIGH); // the device will enter the ordinary
                                 // operational mode and begin executing
                                 // the application instructions.
        while(1);
    }
}
