/***********************************************************************
    Title:	USB Pinguino Bootloader
    File:	hardware.h
    Descr.: bootloader def. (version, led, tempo.)
    Author:	André Gentric
            Régis Blanchot <rblanchot@gmail.com>
            Based on Albert Faber's JAL bootloader
            and Alexander Enzmann's USB Framework
    This file is part of Pinguino (http://www.pinguino.cc)
    Released under the LGPL license (http://www.gnu.org/licenses/lgpl.html)
***********************************************************************/

#if SDCC < 320
    #error "*******************************************"
    #error "*          Outdated SDCC version          *"
    #error "* try to update to version 3.2.0 or newer *"
    #error "*******************************************"
#endif

/**********************************************************************/

#include <pic18fregs.h>
#include "config.h"
#include "hardware.h"
#include "types.h"
#include "picUSB.h"

/** --------------------------------------------------------------------
    Prepare jump to user application
    -----------------------------------------------------------------**/

void disable_boot(void) //__naked
{
    word counter = 0xFFFF;

    T1CON = 0;                  // disable timer 1

    /*
     * When disabling the USB module, make sure the SUSPND bit (UCON<1>)
     * is clear prior to clearing the USBEN bit. Clearing the USBEN bit
     * when the module is in the suspended state may prevent the module
     * from fully powering down.
     */

    UCONbits.SUSPND = 0;
    UCONbits.USBEN  = 0;
    
    while (counter--);          // force timeout on USB
                                // if too short CDC won't work

    RCONbits.NOT_POR = 0;       // set Power-on Reset

    __asm                       // switch off the led

        bsf     LED_TRIS, LED_PIN   ; led input
        bcf     LED_PORT, LED_PIN   ; led off

    __endasm;

    Reset();                    // reset the PIC
}

/** --------------------------------------------------------------------
    Main loop
    -----------------------------------------------------------------**/

void main(void)
{
    #if defined(__18f25k50) || defined(__18f45k50) || \
        defined(__18f26j50) || defined(__18f46j50) || \
        defined(__18f26j53) || defined(__18f46j53) || \
        defined(__18f27j53) || defined(__18f47j53)

    word  pll_counter = 600;

    #endif
    
    dword usb_counter = 0;
    
    // Init. oscillator and I/O
    // -----------------------------------------------------------------

/**********************************************************************/
    #if defined(__18f13k50) || defined(__18f14k50) || \
        defined(__18f2455)  || defined(__18f4455)  || \
        defined(__18f2550)  || defined(__18f4550)        
/**********************************************************************/

        ADCON1 = 0x0F;              // all I/O to Digital mode
        CMCON  = 0x07;              // all I/O to Digital mode
     
/**********************************************************************/
    #elif defined(__18f25k50) || defined(__18f45k50)
    //cf Datasheet - 3.10 Power-up Delays
/**********************************************************************/

        #if (CRYSTAL == INTOSC)

        OSCCON = 0x70;              // 0b01110000 : 111 = HFINTOSC (16 MHz)
                                    // enable the 16 MHz internal clock
                                    // Primary clock source (HFINTOSC or HSPLL)
                                    // is defined by FOSC<2:0> (cf. config.h)

        while(!OSCCONbits.HFIOFS);  // wait HFINTOSC frequency is stable (HFIOFS=1) 

        #endif
/* ALREADY SET UP IN CONFIG BITS
        OSCCON2bits.PLLEN = 1;      // Enable the PLL

        while (pll_counter--);      // Wait > 2ms until the PLL locks.
                                    // Must be done before enabling USB module
*/
        ANSELA = 0;                 // all I/O to Digital mode
        ANSELB = 0;                 // all I/O to Digital mode
        ANSELC = 0;                 // all I/O to Digital mode
            
        #if defined(__18f45k50)

        ANSELD = 0;                 // all I/O to Digital mode
        ANSELE = 0;                 // all I/O to Digital mode

        #endif
        
/**********************************************************************/
    #elif defined(__18f26j50) || defined(__18f46j50)
/**********************************************************************/

        #if (CRYSTAL == INTOSC)

        OSCCON = 0x70;              // 0b01110000 : 111 = INTOSC (8 MHz)
                                    // enable the 8 MHz internal clock
                                    // Primary clock source (INTOSC or HSPLL)
                                    // is defined by FOSC<2:0> (cf. config.h)

        #endif
        
        OSCTUNEbits.PLLEN = 1;      // Enable the PLL

        while (pll_counter--);      // Wait > 2ms until the PLL locks.
                                    // Must be done before enabling USB module

        ANCON0 = 0xFF;              // AN0 to AN7  are Digital I/O
        ANCON1 = 0x1F;              // AN8 to AN12 are Digital I/O

/**********************************************************************/
    #elif defined(__18f26j53) || defined(__18f46j53) || \
          defined(__18f27j53) || defined(__18f47j53)
/**********************************************************************/

        #if (CRYSTAL == INTOSC)

        OSCCON = 0x70;              // 0b01110000 : 111 = INTOSC (8 MHz)
                                    // enable the 8 MHz internal clock
                                    // Primary clock source (INTOSC or HSPLL)
                                    // is defined by FOSC<2:0> (cf. config.h)

        while(!OSCCONbits.FLTS);    // wait INTOSC frequency is stable (FLTS=1) 

        #endif
        
        OSCTUNEbits.PLLEN = 1;      // Enable the PLL

        while (pll_counter--);      // Wait > 2ms until the PLL locks.
                                    // Must be done before enabling USB module

        ANCON0 = 0xFF;              // AN0 to AN7  are Digital I/O
        ANCON1 = 0x1F;              // AN8 to AN12 are Digital I/O

/**********************************************************************/
    #else
/**********************************************************************/

            #error "    --------------------------    "
            #error "    PIC NO YET SUPPORTED !        "
            #error "    Please contact developer.     "
            #error "    <rblanchot@gmail.com>         "
            #error "    --------------------------    "

/**********************************************************************/
        #endif
/**********************************************************************/

    // If power-on reset, jump to user application
    // -----------------------------------------------------------------

    if (RCONbits.NOT_POR == 0)
    {
        RCON |= 0b10010011;     // reset all reset flag
        __asm
        goto ENTRY
        __endasm;
    }
    
    // If MCLR reset, jump to bootloader
    // -----------------------------------------------------------------

    if (RCONbits.IPEN == 0)
    {

    // Init. interrupt : no jump to interrupt vectors
    // -----------------------------------------------------------------

        RCONbits.IPEN   = 1;        // enables priority levels on
                                    // interrupts (cf. vectors.h)
                                    // MUST BE SET OR INTERRUPT WILL NOT WORK !!!
        //INTCONbits.GIEH = 0;        // Disable global HP interrupts
        //INTCONbits.GIEL = 0;        // Disable global LP interrupts

    // Init. timer1 to overroll after 65536*8*83ns = 43.5 ms
    // -----------------------------------------------------------------

    /*
     * bit 7,6 TMR1CS  = 00, Timer1 clock source is FOSC/4
     * bit 5,4 T1CKPS  = 11, 1:8 prescaler value
     * bit 3   T1OSCEN = 0 , Timer1 crystal driver is off
     * bit 2   T1SYNC  = 0 , this bit is ignored when TMR1CS = 00
     * bit 1   RD16    = 0 , read/write of Timer1 in two 8-bit operations
     * bit 0   TMR1ON  = 1 , enables Timer 1
     */

        //PIE1bits.TMR1IE = 0;        // TMR1 Interrupt is disabled
        //IPR1bits.TMR1IP = 0;        // TMR1 Interrupt gets LP
        TMR1L = 0;                  // clear Timer 1 counter because
        TMR1H = 0;                  // counter get an unknown value at reset
        T1CON = 0b00110001;         // clock source is Fosc/4 (0b00)
                                    // prescaler 8 (0b11), timer 1 On 

    // Init. led
    // -----------------------------------------------------------------

        __asm
        
            bcf     LED_TRIS, LED_PIN   ; led output
            bsf     LED_PORT, LED_PIN   ; led on

        __endasm;
        
    // Init. USB
    // -----------------------------------------------------------------
    
    /*
     * bit 4   UPUEN    = 1  : USB On-Chip Pull-up Enable bit
     * bit 3   UTRDIS   = 0  : On-Chip Transceiver Disable bit
     * bit 2   FSEN     = 1  : Full-Speed Enable bit
     * bit 1,0 PPB<1:0> = 00 : Ping-Pong Buffers disabled
     */
        
        #if (SPEED == LOW_SPEED)
        
        UCFG = 0b00010000;          // (0x10) low speed mode

        #else

        UCFG = 0b00010100;          // (0x14) full speed mode

        #endif

        //EP_IN_BD(1).ADDR = PTR16(&bootCmd);
        EP_IN_BD(1).ADDR = (unsigned long)&bootCmd;
        currentConfiguration = 0x00;
        deviceState = DETACHED;

    // Try to detect USB activity for about 5s
    // -----------------------------------------------------------------

        do
        {
            EnableUSBModule();
            ProcessUSBTransactions();

            if (usb_counter == 0xFFFFF)
            {
                disable_boot();     // disable boot and jump to user app.
            }
            usb_counter++;
        }
        while (deviceState != CONFIGURED);

    // Wait for User Upload
    // -----------------------------------------------------------------
        
        while (1)
        {
            ProcessUSBTransactions();

            // Timer 1 overflow ?
            if (PIR1bits.TMR1IF == 1)
            {
                // allow interrupt source again
                PIR1bits.TMR1IF = 0;

                // toggle the led
                __asm
                
                movlw   LED_MASK    ; toggle
                xorwf   LED_PORT, f ; the led
                
                __endasm;
            }
        }

    }
}

/** --------------------------------------------------------------------
    bootloader commands management
    -----------------------------------------------------------------**/
    
void usb_ep_data_out_callback(char end_point)
{
    byte counter;

    // whatever the command, keep LED high
    __asm
    
    bsf     LED_PORT, LED_PIN   ; led on
    
    __endasm;
    
    // whatever the command, disable timer 1
    //T1CON = 0;
 
    // number of byte(s) to return
    EP_IN_BD(end_point).Cnt = 0;

    // load table pointer
    TBLPTRU = bootCmd.addru;
    TBLPTRH = bootCmd.addrh;
    TBLPTRL = bootCmd.addrl;

/**********************************************************************/
    if (bootCmd.cmd ==  RESET)
/**********************************************************************/
    {
        disable_boot();
    }

/**********************************************************************/
    else if (bootCmd.cmd == READ_VERSION)
/**********************************************************************/
    {
        bootCmd.buffer[2] = MINOR_VERSION;
        bootCmd.buffer[3] = MAJOR_VERSION;

        // number of byte(s) to return
        EP_IN_BD(end_point).Cnt = 4;
    }

/**********************************************************************/
    else if (bootCmd.cmd == READ_FLASH)
/**********************************************************************/
    {
        for (counter=0; counter < bootCmd.len; counter++)
        {
            // TBLPTR is incremented after the read
            __asm__("tblrd*+");
            bootCmd.xdat[counter] = TABLAT;
        }

        // Number of byte(s) to return
        EP_IN_BD(end_point).Cnt = 5 + bootCmd.len;
    }

/**********************************************************************/
    else if (bootCmd.cmd == WRITE_FLASH)
/**********************************************************************/
    {
        // Init write/erase register (EECON1)
        // -----------------------------------------------------------------

        /*
         * bit 7, EEPGD = 1, memory is flash (unimplemented on J PIC)
         * bit 6, CFGS  = 0, enable acces to flash (unimplemented on J PIC)
         * bit 5, WPROG = 1, enable single word write (unimplemented on non-J PIC)
         * bit 4, FREE  = 0, enable write operation (1 if erase operation)
         * bit 3, WRERR = 0, 
         * bit 2, WREN  = 1, enable write to memory
         * bit 1, WR    = 0,
         * bit 0, RD    = 0, (unimplemented on J PIC)
         */

        EECON1 = 0b10100100;


        #if defined(__18f13k50) || defined(__18f14k50) || \
            defined(__18f2455)  || defined(__18f4455)  || \
            defined(__18f2550)  || defined(__18f4550)  || \
            defined(__18f25k50) || defined(__18f45k50)

        /// Word or byte programming is not supported by these chips.
        /// The programming block is 32 bytes for all chips except x5k50
        /// The programming block is 64 bytes for x5k50.
        /// It is not necessary to load all holding register before a write operation
        /// NB:
        /// * High Speed USB has a Max. packet size of 64 bytes
        /// * Uploader (uploader8.py) sends 32-byte Data block + 5-byte Command block 

        // Load max. 32 holding registers
        for (counter=0; counter < bootCmd.len; counter++)
        {
            TABLAT = bootCmd.xdat[counter]; // present data to table latch
            __asm__("tblwt*+"); // write data in TBLWT holding register
                                // TBLPTR is incremented after the read/write
        }
        
        // start block write
        __asm__("tblrd*-");     // one step back to be inside the 32 bytes range

        EECON2 = 0x55;
        EECON2 = 0xAA;
        
        EECON1bits.WR = 1;      // WR = 1; start write or erase operation
                                // WR cannot be cleared, only set, in software.
                                // It is cleared in hardware at the completion
                                // of the write or erase operation.
                                // CPU stall here for 2ms
        __asm__("nop");         // proc. can forget to execute the first
                                // operation on some PIC
 
        #elif defined(__18f26j50) || defined(__18f46j50) || \
              defined(__18f26j53) || defined(__18f46j53) || \
              defined(__18f27j53) || defined(__18f47j53)
        
        /// max USB packet size is 64 bytes long
        /// bootloader command sequence is 5 bytes long
        /// we must write 32 bytes at a time because we don't have 64 bytes
        /// blocks must be erased before written
        /// uploader8.py erases the whole memory once at the begining of upload
        /// so we can't write only one time at the same place
        /// 0   : write [address]       + 64 bytes
        /// 1   : write [address + 32]  + 64 bytes
        /// ...
        /// n   : write [address + 32n] + 64 bytes
        /// if we used 64-byte write mode, 32 bytes were written 2 times : not possible
        /// that's why we use 2-byte write instead
        
        for (counter=0; counter < bootCmd.len; counter+=2)
        {
            TBLPTRL =  bootCmd.addrl + counter; // address of the byte to write

            TABLAT = bootCmd.xdat[counter];     // load value in the holding registers
            __asm__("tblwt*+");                 // then write the 1rst byte and
                                                // increment TBLPTR after the write

            TABLAT = bootCmd.xdat[counter+1];   // load value in the holding registers
            __asm__("tblwt*");                  // then write the 2nd byte
                                                // The last table write must not increment
                                                // the table pointer !
                                                // The table pointer needs to point to the
                                                // MSB before starting the write operation.
            // start  write
            EECON2 = 0x55;      // unlock sequence
            EECON2 = 0xAA;      // unlock sequence
            EECON1bits.WR = 1;  // start 2-byte write operation
        }

        #endif

        // number of byte(s) to return
        EP_IN_BD(end_point).Cnt = 1;
    }

/**********************************************************************/
    else if (bootCmd.cmd == ERASE_FLASH)
/**********************************************************************/
    {

        // Init write/erase register (EECON1)
        // -----------------------------------------------------------------

        /*
         * bit 7, EEPGD = 1, memory is flash (unimplemented on J PIC)
         * bit 6, CFGS  = 0, enable acces to flash (unimplemented on J PIC)
         * bit 5, WPROG = 1, enable single word write (unimplemented on non-J PIC)
         * bit 4, FREE  = 0, enable write operation (1 if erase operation)
         * bit 3, WRERR = 0, 
         * bit 2, WREN  = 1, enable write to memory
         * bit 1, WR    = 0,
         * bit 0, RD    = 0, (unimplemented on J PIC)
         */

        EECON1 = 0b10100100;

        // bootCmd.len = num. of blocks to erase
        for (counter=0; counter < bootCmd.len; counter++)
        {

            /*
             * erase current block pointed by TBLPTR
             * NB : FREE mus be set/unset here or it won't work
             */

            EECON1bits.FREE = 1;// perform erase operation
            EECON2 = 0x55;      // unlock sequence
            EECON2 = 0xAA;      // unlock sequence
            EECON1bits.WR = 1;  // start write or erase operation
            EECON1bits.FREE = 0;// back to write operation

            #if defined(__18f13k50) || defined(__18f14k50) || \
                defined(__18f2455)  || defined(__18f4455)  || \
                defined(__18f2550)  || defined(__18f4550)  || \
                defined(__18f25k50) || defined(__18f45k50)

            __asm__("nop");     // proc. can forget to execute
                                // the first operation on some PIC

            /*
             * the erase block is 64-byte long
             * next block to erase is at TBLPTR = TBLPTR + 64
             * This can not be used in SDCC because
             * TBLPTR is at the same address as TBLPRTL
             */

            __asm

                movlw	0x40            ; 0x40 + (TBLPTRL) -> TBLPTRL
                addwf	_TBLPTRL, 1     ;  (W) + (TBLPTRL) -> TBLPTRL
                                        ;  (C) is affected
                movlw	0x00            ; 0x00 + (TBLPTRH) + (C) -> TBLPTRH
                addwfc	_TBLPTRH, 1     ;  (W) + (TBLPTRH) + (C) -> TBLPTRH

            __endasm;

            #elif defined(__18f26j50) || defined(__18f46j50) || \
                  defined(__18f26j53) || defined(__18f46j53) || \
                  defined(__18f27j53) || defined(__18f47j53)

            /*
             * the erase block is 1024-byte long
             * next block to erase is at TBLPTR = TBLPTR + 1024
             * This can not be used in SDCC because
             * TBLPTR is at the same address as TBLPRTL
             * addwf f,1 means the result is stored back in register f
             * bcf STATUS, C will clear the carry bit
             * clrf WREG is the same as movlw	0x00            ; 0x00 -> W
             */

            __asm
            
                movlw	0x04            ; 0x04 -> W
                addwf	_TBLPTRH, 1     ;  (W) + (TBLPTRH) -> TBLPTRH
                                        ;  (C) is affected
                movlw	0x00            ; 0x00 -> W
                addwfc	_TBLPTRU, 1     ;  (W) + (TBLPTRU) + (C) -> TBLPTRU
                                        ; Add W and Carry Bit to F

            __endasm;

            #endif

        }

        // number of byte(s) to return
        EP_IN_BD(end_point).Cnt = 1;
    }

/**********************************************************************/

    // is there something to return ?
    if (EP_IN_BD(end_point).Cnt > 0)
    {
        // data packet toggle
        if (EP_IN_BD(1).Stat.DTS)
            EP_IN_BD(1).Stat.uc = 0b10001000; // UOWN 1 DTS 0 DTSEN 1
        else
            EP_IN_BD(1).Stat.uc = 0b11001000; // UOWN 1 DTS 1 DTSEN 1
    }

    // reset size
    EP_OUT_BD(end_point).Cnt = EP1_BUFFER_SIZE;

    // free the BD and its corresponding buffer
    EP_OUT_BD(end_point).Stat.uc = 0x80;      // UOWN set to 1

}
