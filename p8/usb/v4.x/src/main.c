/**********************************************************************
    Title:      Pinguino USB Bootloader
    File:       main.c
    Descr.:     8-bit PIC USB bootloader
    Author:     Régis Blanchot <rblanchot@gmail.com>
                André Gentric
    Comment:    Based on Albert Faber's JAL bootloader
                and Alexander Enzmann's USB Framework
                see also : http://www.usbmadesimple.co.uk/ums_3.htm
                https://github.com/majbthrd/pic16f1454-bootloader
***********************************************************************
    This file is part of Pinguino (http://www.pinguino.cc)
    Released under the LGPL license (http://www.gnu.org/licenses/lgpl.html)
***********************************************************************/

/** --------------------------------------------------------------------
    Includes
    -----------------------------------------------------------------**/

#include "compiler.h"
#include "types.h"
#include "config.h"
//#include "boot.h"
#include "hardware.h"
#include "flash.h"
#include "usb.h"
#include "vectors.h"
#if (BOOT_USE_DEBUG)
#include "serial.h"
#endif

/** --------------------------------------------------------------------
    Global variables
    -----------------------------------------------------------------**/

#ifdef LOWPOWER
u8 userApp = TRUE;
#endif

extern u8 deviceState;
extern u8 currentConfiguration;
extern u8 controlTransferBuffer[EP0_BUFFER_SIZE];
extern BufferDescriptorTable ep_bdt[2*NB_ENDPOINTS];
extern setupPacketStruct SetupPacket;
extern allcmd bootCmd;

/***********************************************************************
    BOOTLOADER COMMANDS
    General Data Packet Structure:

    __________________           RxBuffer fields
    |    COMMAND     |   0       [CMD]
    |      LEN       |   1       [LEN]
    |     ADDRL      |   2       [    ]  [addrl]
    |     ADDRH      |   3       [ADDR]: [addrh]
    |     ADDRU      |   4       [    ]  [addru]
    |                |   5       [DATA]
    |                |
    .      DATA      .
    .                .
    |                |   62
    |________________|   63

***********************************************************************/
/*
#define BOOT_READ_VERSION               0x00
#define BOOT_READ_FLASH                 0x01
#define BOOT_WRITE_FLASH                0x02
#define BOOT_ERASE_FLASH                0x03
#define BOOT_RESET_DEVICE               0xFF
*/
enum
{
    BOOT_READ_VERSION = 0x00,
    BOOT_READ_FLASH,
    BOOT_WRITE_FLASH,
    BOOT_ERASE_FLASH,
    BOOT_RESET_DEVICE = 0xFF
};

/***********************************************************************
 * Jump to user application

    u16 *UserAppPtr;
    void  (*UserApp)(void);
    UserAppPtr = (u16*)APPSTART;
    UserApp = (void (*)(void))APPSTART;
 **********************************************************************/

//#pragma inline UserApp
#ifdef __XC8__
inline void UserApp(void)
{
    #asm
    LJMP APPSTART
    #endasm
}
#else
void UserApp(void)
{
    __asm
    GOTO APPSTART
    __endasm;
}
#endif

/***********************************************************************
 * Prepare jump to user application
 * When disabling the USB module, make sure the SUSPND bit (UCON<1>)
 * is clear prior to clearing the USBEN bit. Clearing the USBEN bit
 * when the module is in the suspended state may prevent the module
 * from fully powering down.
 **********************************************************************/

void UsbBootExit(void)
{
    u16 counter = 0xFFFF;

    T1CON = 0;                      // Disable timer 1
    UCONbits.SUSPND = 0;            // Unsuspend first
    UCONbits.USBEN  = 0;            // Then disable USB

    while (counter--);              // Force timeout on USB
                                    // NB : if too short CDC won't work

    LedOff();                       // Led Off

    #if 0 //(BOOT_USE_DEBUG)
    SerialPrintLN("Bootloader disabled");
    SerialPrintLN("Starting user app.");
    #endif
    
    UserApp();                      // Jump to user app. (reset vector)
}

/***********************************************************************
 * MAIN
 **********************************************************************/

void main(void)
{
    #if defined(__18f26j50) || defined(__18f46j50) || \
        defined(__18f26j53) || defined(__18f46j53) || \
        defined(__18f27j53) || defined(__18f47j53)

    u16  pll_counter = 600;

    #endif

    // Init. oscillator and I/O
    // -----------------------------------------------------------------

/**********************************************************************/
    #if defined(__16f1459)
/**********************************************************************/

        //OSCTUNE = 0;
        OSCCON = 0b11111100;        // Config. bits FOSC are set to use INTOSC
                                    // SPLLEN   : 1 = PLL is enabled (see config.h)
                                    // SPLLMULT : 1 = 3x PLL is enabled (16x3=48MHz)
                                    // IRCF     : 1111 = HFINTOSC (16 MHz)
                                    // SCS      : 00 = use clock determined by FOSC
        #if (CRYSTAL == INTOSC)     

        ACTCON = 0x90;              // Enable active clock tuning with USB

        while (!OSCSTATbits.HFIOFS);// wait HFINTOSC frequency is stable (HFIOFS=1) 

        #else

        ACTCON = 0x00;

        #endif
        
        // Wait until the PLLRDY bit is set in the OSCSTAT register
        // before attempting to set the USBEN bit.
        while (!OSCSTATbits.PLLRDY);

        // The state of the ANSELx bits has no effect on digital output functions.
        ANSELA = 0;                 // all I/O to Digital mode
        ANSELB = 0;                 // all I/O to Digital mode
        ANSELC = 0;                 // all I/O to Digital mode
        
/**********************************************************************/
    #elif defined(__18f13k50) || defined(__18f14k50)
/**********************************************************************/

        OSCCONbits.SCS  = 0;        // 00 = Primary clock (determined by CONFIG1H[FOSC<3:0>])

        #if (CRYSTAL == 48)

        OSCTUNEbits.SPLLEN = 0;     // SPLLEN   : 0 = 4x PLL is disabled (see config.h)

        #else // INTOSC 16MHz or ext. 12 MHZ
        
        OSCTUNEbits.SPLLEN = 1;     // SPLLEN   : 1 = 4x PLL is enabled (see config.h)

        #endif

        ANSEL  = 0;                 // all I/O to Digital mode
        ANSELH = 0;

/**********************************************************************/
    #elif defined(__18f2455)  || defined(__18f4455)  || \
          defined(__18f2550)  || defined(__18f4550)  || \
          defined(__18lf2550) || defined(__18lf4550)
/**********************************************************************/

        #if (CRYSTAL == INTOSC)

            #error "Internal Osc. not supported"
            
        #else
        
        ADCON1 = 0x0F;              // all I/O to Digital mode
        CMCON  = 0x07;              // all I/O to Digital mode
     
        #endif
        
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

        while (!OSCCONbits.FLTS);   // wait INTOSC frequency is stable (FLTS=1) 

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
        #error "    PIC NOT YET SUPPORTED !       "
        #error "    Please contact the developer: "
        #error "    <rblanchot@pinguino.cc>       "
        #error "    --------------------------    "

/**********************************************************************/
    #endif
/**********************************************************************/

    // Init. Serial if DEBUG is activated
    // -----------------------------------------------------------------

    #if (BOOT_USE_DEBUG)
    SerialInit(9600);
    //SerialInit(115200);
    SerialPrintLN("***");
    /*
    SerialPrint("*** PINGUINO BOOTLOADER v");
    SerialPrintNumber(MAJOR_VERSION, 10);
    SerialPrintChar('.');
    SerialPrintNumber(MINOR_VERSION, 10);
    SerialPrintLN(" ***");
    */
    #endif
    
    // Init. led
    // -----------------------------------------------------------------

    LedOut();                       // Led Pin Output
    LedOn();                        // Led On

    // Detect VBUS
    // D+ and D- pins are input only, TRIS bits will always read as 1
    // -----------------------------------------------------------------

    //VBUS_TRIS |= VBUS_MASK;         // VBUS Pin as Input

    #if 0 //(BOOT_USE_DEBUG)
    SerialPrint("VBUS = ");
    SerialPrintNumber(VBUS_PORT & VBUS_MASK, 2);
    SerialPrint("\r\n");
    #endif

    // The bootloader starts if :
    // - Reset button has been pressed
    // - there is no user application
    // - USB is On
    // -----------------------------------------------------------------

/**********************************************************************/
    #if defined(__16F1459)
/**********************************************************************/

    if (UsbOff() || PCONbits.nRMCLR)// If not a MCLR Reset
    {                               // Then it could be a POR or a BOR
        PCONbits.nPOR = 1;          // POR and BOR flags must be cleared by
        PCONbits.nBOR = 1;          // software to allow a new detection

/**********************************************************************/
    #else
/**********************************************************************/

    RCONbits.IPEN = 1;              // Enables priority levels on
                                    // interrupts (cf. vectors.c/.h)
                                    // MUST BE SET OR INT. WON'T WORK !
                                    // NB: MCLR clears this bit

    if (UsbOff() || !RCONbits.NOT_POR)// If Power-On-Reset
    {
        RCONbits.NOT_POR = 1;
        RCONbits.NOT_BOR = 1;
        
/**********************************************************************/
    #endif
/**********************************************************************/

        #if 0 //(BOOT_USE_DEBUG)
        SerialPrintLN("Power-On-Reset detected");
        SerialPrintLN("Starting user app.");
        #endif

        UserApp();                  // Jump to user app. (reset vector)

        #if 0 //(BOOT_USE_DEBUG)
        SerialPrintLN("No user app.");
        #endif
        
        #if (BOOT_USE_LOWPOWER)
        userApp = FALSE;            // otherwise, go on with the bootloader
        #endif
    }

    // USB bootloader's code start here
    // -----------------------------------------------------------------
    
    #if 0 //(BOOT_USE_DEBUG)
    SerialPrintLN("Reset detected");
    SerialPrintLN("Starting bootloader");
    #endif

    /* bit 4   UPUEN    = 1  : USB On-Chip Pull-up Enable bit
     * bit 3   UTRDIS   = 0  : On-Chip Transceiver Disable bit
     * bit 2   FSEN     = 1  : Full-Speed Enable bit
     * bit 1,0 PPB<1:0> = 00 : Ping-Pong Buffers disabled
     */

    #if (SPEED == LOW_SPEED)

        //UCFG = 0x10;                // 0b00010000 low speed mode
        #ifdef __XC8__
        UCFG = _UCFG_UPUEN_MASK;
        #else
        UCFG = _UPUEN;
        #endif

    #else

        //UCFG = 0x14;                // 0b00010100 full speed mode
        #ifdef __XC8__
        UCFG = _UCFG_UPUEN_MASK | _UCFG_FSEN_MASK;
        #else
        UCFG = _UPUEN | _FSEN;
        #endif

    #endif

    EP_IN_BD(1).ADDR = (u16)&bootCmd;
    currentConfiguration = 0;
    deviceState = DETACHED;

    // Init. timer1 to overroll after 65536*8/12000 = 43.7 ms
    // -----------------------------------------------------------------

    /*
     * bit 7,6 TMR1CS  = 00, Timer1 clock source is FOSC/4
     * bit 5,4 T1CKPS  = 11, 1:8 prescaler value
     * bit 3   T1OSCEN = 0 , Timer1 crystal driver is off
     * bit 2   T1SYNC  = 0 , this bit is ignored when TMR1CS = 00
     * bit 1   RD16    = 0 , read/write of Timer1 in two 8-bit operations
     * bit 0   TMR1ON  = 1 , enables Timer 1
     */

    TMR1L = 0;                      // clear Timer 1 counter because
    TMR1H = 0;                      // counter get an unknown value at reset
    T1CON = 0x31; //0b00110001;     // clock source is Fosc/4 (0b00)
                                    // prescaler 8 (0b11), timer 1 On 

    // Wait for request from host
    // -----------------------------------------------------------------

    while (1)
    {
        UsbUpdate();                // Check the USB bus
        UsbProcessEvents();         // Service USB interrupts

        if (PIR1bits.TMR1IF)        // If timer 1 has overflowed
        {
            PIR1bits.TMR1IF = 0;    // Allow interrupt source again
            LedToggle();            // Toggle the led
        }
    }
}

/** --------------------------------------------------------------------
    bootloader commands management
    -----------------------------------------------------------------**/
    
void UsbBootCmd(void)
{
/**********************************************************************/
    #if defined(__16F1459)
/**********************************************************************/

    // PIC16F handle data in 14-bit chunks
    u8  counter = bootCmd.len >> 1;// / WORDSIZE;
    u16 *pdata  = (u16*)bootCmd.xdat;

/**********************************************************************/
    #else
/**********************************************************************/

    // PIC18F handle data in 8-bit chunks
    u8  counter = bootCmd.len;
    u8  *pdata  = (u8*)bootCmd.xdat;

/**********************************************************************/
    #endif
/**********************************************************************/
    
    LedOn();                        // Whatever the command, keep Led On

    //LedOn();
    //while (1);

    //T1CON = 0;                    // and disable timer 1
 
    EP_IN_BD(1).CNT = 0;            // Clears the number of byte(s) to return

    // Address of the block to deal with
    // -----------------------------------------------------------------

/**********************************************************************/
    #if defined(__16F1459)
/**********************************************************************/

    PMADRH = bootCmd.addrh;         // load table pointer
    PMADRL = bootCmd.addrl;

/**********************************************************************/
    #else
/**********************************************************************/
    
    TBLPTRU = bootCmd.addru;        // load table pointer
    TBLPTRH = bootCmd.addrh;
    TBLPTRL = bootCmd.addrl;

/**********************************************************************/
    #endif
/**********************************************************************/

    // Init write/erase register (EECON1 or PMCON1)
    // -----------------------------------------------------------------

    /*
     * bit 7, EEPGD = 1, memory is flash (unimplemented on PIC16F and J PIC18F)
     * bit 6, CFGS  = 0, enable acces to program or conf. memory (unimplemented on J PIC)
 18F * bit 5, WPROG = 1, enable single word write (unimplemented on non-J PIC)
 16F * bit 5, LWLO  = 1, starts to load the write latches when set, starts to write when clear
     * bit 4, FREE  = 0, enable write operation (1 if erase operation)
     * bit 3, WRERR = 0, 
     * bit 2, WREN  = 1, enable write to memory
     * bit 1, WR    = 0,
     * bit 0, RD    = 0, EEPROM read (unimplemented on J PIC)
     */

/**********************************************************************/
    #if defined(__16F1459)
/**********************************************************************/

    PMCON1 = 0xA4;                      // 0b10100100;
 
/**********************************************************************/
    #else
/**********************************************************************/

    EECON1 = 0xA4;                      // 0b10100100;

/**********************************************************************/
    #endif
/**********************************************************************/

    #if 0 //(BOOT_USE_DEBUG)
    SerialPrint("CMD=");
    SerialPrintNumber(bootCmd.cmd, 10);
    SerialPrint("\r\n");
    #endif

///---------------------------------------------------------------------
    if (bootCmd.cmd ==  BOOT_RESET_DEVICE)
///---------------------------------------------------------------------
    {
        UsbBootExit();
    }
///---------------------------------------------------------------------
    else if (bootCmd.cmd == BOOT_READ_VERSION)
///---------------------------------------------------------------------
    {
        #if 0 //(BOOT_USE_DEBUG)
        SerialPrintLN("READ_VERSION");
        #endif

        bootCmd.buffer[2] = MINOR_VERSION;
        bootCmd.buffer[3] = MAJOR_VERSION;
        EP_IN_BD(1).CNT = 4;        // 4 byte(s) to return
    }
///---------------------------------------------------------------------
    else if (bootCmd.cmd == BOOT_READ_FLASH)
///---------------------------------------------------------------------
    {
        #if 0 //(BOOT_USE_DEBUG)
        SerialPrintLN("READ_FLASH");
        #endif

/**********************************************************************/
        #if defined(__16F1459)
/**********************************************************************/
        
        //PMCON1bits.CFGS = (bootCmd.addrh & 0x80) ? 1:0;
        PMCON1bits.CFGS = 1;        // Access Configuration registers

        while (counter--)
        {
            PMCON1bits.RD = 1;
            asm("NOP");
            asm("NOP");
            *pdata++ = PMDAT;
            PMADR++;
        }

/**********************************************************************/
        #else
/**********************************************************************/

        #if defined(__18f13k50) || defined(__18f14k50) || \
            defined(__18f2455)  || defined(__18f4455)  || \
            defined(__18f2550)  || defined(__18f4550)  || \
            defined(__18lf2550) || defined(__18lf4550) || \
            defined(__18f25k50) || defined(__18f45k50)
            
        // Access Configuration registers regardless of EEPGD
        //EECON1bits.CFGS = (bootCmd.addru & 0x20) ? 1:0;
        EECON1bits.CFGS = 1;

        #endif
        
        // Table reads from program memory are performed one byte at a time.
        //for (counter=0; counter < bootCmd.len; counter++)
        while (counter--)
        {
            // TBLPTR is incremented after the read
            __asm__("TBLRD*+");
            *pdata++ = TABLAT;
        }

/**********************************************************************/
        #endif
/**********************************************************************/

        EP_IN_BD(1).CNT = 5 + bootCmd.len;// Number of byte(s) to return
    }
///---------------------------------------------------------------------
    else if (bootCmd.cmd == BOOT_ERASE_FLASH)
///---------------------------------------------------------------------
    {
        #if 0 //(BOOT_USE_DEBUG)
        SerialPrintLN("ERASE_FLASH");
        #endif

        #if defined(__16F1459)
        counter = bootCmd.len;
        #endif

        //for (counter=0; counter < bootCmd.len; counter++)
        while (counter--)           // num. of blocks to erase
        {
            EraseOn();              // Must stay in the loop
            Unlock();
            EraseOff();
            __asm__("NOP");         // proc. can forget to execute the first operation on some PIC
            NextBlock();            // += FLASHBLOCKSIZE;
        }
        EP_IN_BD(1).CNT = 1;        // number of byte(s) to return
    }
///---------------------------------------------------------------------
    else if (bootCmd.cmd == BOOT_WRITE_FLASH)
///---------------------------------------------------------------------
    {
        #if 0 //(BOOT_USE_DEBUG)
        SerialPrintLN("WRITE_FLASH");
        #endif

/**********************************************************************/
        #if defined(__16F1459)
/**********************************************************************/

        while (counter-- > 1)       // until we reach the last word
        {
            PMDAT = *pdata++;       // next word to load
            Unlock();
            PMADR++;                // next address
        }
        PMDAT = *pdata++;           // next word to load
        PMCON1bits.LWLO = 0;        // Write Latches to Flash
        Unlock();

/**********************************************************************/
        #elif defined(__18f13k50) || defined(__18f14k50) || \
              defined(__18f2455)  || defined(__18f4455)  || \
              defined(__18f2550)  || defined(__18f4550)  || \
              defined(__18lf2550) || defined(__18lf4550) || \
              defined(__18f25k50) || defined(__18f45k50)
/**********************************************************************/

        /// Word or byte programming is not supported by these chips.
        /// The programming block is 32 bytes for all chips except x5k50
        /// The programming block is 64 bytes for x5k50.
        /// It is not necessary to load all holding register before a write operation
        /// NB:
        /// * High Speed USB has a Max. packet size of 64 bytes
        /// * Uploader (uploader8.py) sends 32-byte Data block + 5-byte Command block 

        while (counter--)           // Load max. 32 holding registers
        {
            TABLAT = *pdata++;      // present data to table latch
            __asm__("TBLWT*+");     // write data in TBLWT holding register
        }                           // TBLPTR is incremented after the read/write
        __asm__("TBLRD*-");         // start block write one step back (Datasheet 6.5.1)
        Unlock();                   // to be inside the 32 bytes range
        __asm__("NOP");             // proc. can forget to execute the first operation on some PIC
  
/**********************************************************************/
        #elif defined(__18f26j50) || defined(__18f46j50) || \
              defined(__18f26j53) || defined(__18f46j53) || \
              defined(__18f27j53) || defined(__18f47j53)
/**********************************************************************/
        
        /// 1/
        /// The max. USB packet size is 64-byte long.
        /// The bootloader command sequence is 5-byte long,
        /// So we only have 59 bytes free and we can only write 32 bytes at a time.
        /// 2/
        /// Blocks must be erased before written.
        /// uploader8.py erases the whole memory once at the begining of upload.
        /// We can write only one time at the same place.
        /// If we used 64-byte write mode, 32 bytes would be written 2 times :
        /// 0   : write [address]       + 64 bytes
        /// 1   : write [address + 32]  + 64 bytes
        /// ...
        /// n   : write [address + 32n] + 64 bytes
        /// which is not possible. That's why we use 2-byte write instead.
        
        counter = bootCmd.len >> 1; // / WORDSIZE; //>> 1; // 1 word = 2 bytes
        while (counter--)
        {
            //**********************************************************
            TBLPTRL++;              // address of the byte to write
            //**********************************************************
            TABLAT = *pdata++;      // load 1st value in the holding registers
            __asm__("TBLWT*+");     // then write the 1rst byte
                                    // and increment TBLPTR after the write
            TABLAT = *pdata++;      // load 2nd value in the holding registers
            __asm__("TBLWT*");      // then write the 2nd byte
                                    // The last table write must not increment
                                    // the table pointer !
                                    // The table pointer needs to point to the
                                    // MSB before starting the write operation.
            Unlock();               // start to write the word
        }

/**********************************************************************/
        #endif
/**********************************************************************/

        EP_IN_BD(1).CNT = 1;        // number of byte(s) to return
    }

///---------------------------------------------------------------------

    if (EP_IN_BD(1).CNT > 0)        // is there something to return ?
    {
        if (EP_IN_BD(1).STAT.DTS)   // data packet toggle
            EP_IN_BD(1).STAT.val = BDS_UOWN | BDS_DTSEN;
        else
            EP_IN_BD(1).STAT.val = BDS_UOWN | BDS_DTSEN | BDS_DTS;
    }

    // reset size
    EP_OUT_BD(1).CNT = EP1_BUFFER_SIZE;

    EP_OUT_BD(1).STAT.val = BDS_UOWN;// free the BD and its corresponding buffer
}
