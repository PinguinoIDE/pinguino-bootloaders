/***********************************************************************
    Title:  USB Pinguino Bootloader
    File:   flash.h
    Descr.: Write, erase and read flash functions
    Author: Régis Blanchot <rblanchot@gmail.com>

    This file is part of Pinguino (http://www.pinguino.cc)
    Released under the LGPL license (http://www.gnu.org/licenses/lgpl.html)
***********************************************************************/

#ifndef _FLASH_H
#define _FLASH_H

/**********************************************************************/
#if defined(__16f1459)
/**********************************************************************/

    #define FLASHBLOCKSIZE          32      // 32 words
    
    #define Unlock()                {                       \
                                      PMCON2 = 0x55;        \
                                      PMCON2 = 0xAA;        \
                                      PMCON1bits.WR = 1;    \
                                      NOP();                \
                                      NOP();                \
                                    }

    /*
    #define Unlock()    { \
    asm("BANKSEL PMCON2          ;                                  ");\
    asm("MOVLW   0x55            ; PMCON2 = 0x55                    ");\
    asm("MOVWF   PMCON2 & 0x7F   ;                                  ");\
    asm("MOVLW   0xAA            ; PMCON2 = 0xAA                    ");\
    asm("MOVWF   PMCON2 & 0x7F   ;                                  ");\
    asm("BSF     PMCON1 & 0x7F,1 ; PMCON1bits.WR = 1                ");\
    asm("NOP                     ;                                  ");\
    asm("NOP                     ;                                  ");\
    }
    */
    
    #define NextBlock() { \
    asm("BANKSEL PMADRL          ; Select Bank for registers        ");\
    asm("MOVLW   0x20            ; 0x20 + (PMADRL) -> PMADRL        ");\
    asm("ADDWF   PMADRL & 0x7F,F ;  (W) + (PMADRL) -> PMADRL        ");\
    asm("                        ;  (C) is affected                 ");\
    asm("MOVLW   0x00            ; 0x00 + (PMADRH) + (C) -> PMADRH  ");\
    asm("ADDWFC  PMADRH & 0x7F,F ;  (W) + (PMADRH) + (C) -> PMADRH  ");\
    asm("                        ; Add W and Carry Bit to F         ");\
    }
    
    #define EraseOn()               { PMCON1bits.FREE = 1; }
    #define EraseOff()              { PMCON1bits.FREE = 0; }
    
/**********************************************************************/
#elif defined(__18f13k50) || defined(__18f14k50) || \
      defined(__18f2455)  || defined(__18f4455)  || \
      defined(__18f2550)  || defined(__18f4550)  || \
      defined(__18lf2550) || defined(__18lf4550) || \
      defined(__18f25k50) || defined(__18f45k50)
/**********************************************************************/

    #define FLASHBLOCKSIZE          64      // the erase block is 64-byte long
    
    #define Unlock()                {                       \
                                      EECON2 = 0x55;        \
                                      EECON2 = 0xAA;        \
                                      EECON1bits.WR = 1;    \
                                    }
    
    #ifdef __XC8__

    #define NextBlock() { \
    asm("BANKSEL TBLPTRL         ; Select Bank for registers        ");\
    asm("MOVLW   0x40            ; 0x40 + (TBLPTRL) -> TBLPTRL      ");\
    asm("ADDWF   TBLPTRL,F       ;  (W) + (TBLPTRL) -> TBLPTRL      ");\
    asm("                        ;  (C) is affected                 ");\
    asm("MOVLW   0x00            ; 0x00 + (TBLPTRH) + (C) -> TBLPTRH");\
    asm("ADDWFC  TBLPTRH,F       ;  (W) + (TBLPTRH) + (C) -> TBLPTRH");\
    asm("                        ; Add W and Carry Bit to F         ");\
    }

    #else

    #define NextBlock() { \
    __asm__("BANKSEL _TBLPTRL    ; Select Bank for registers        ");\
    __asm__("MOVLW   0x40        ; 0x40 + (TBLPTRL) -> TBLPTRL      ");\
    __asm__("ADDWF   _TBLPTRL,F  ;  (W) + (TBLPTRL) -> TBLPTRL      ");\
    __asm__("                    ;  (C) is affected                 ");\
    __asm__("MOVLW   0x00        ; 0x00 + (TBLPTRH) + (C) -> TBLPTRH");\
    __asm__("ADDWFC  _TBLPTRH,F  ;  (W) + (TBLPTRH) + (C) -> TBLPTRH");\
    __asm__("                    ; Add W and Carry Bit to F         ");\
    }

    #endif

    #define EraseOn()               { EECON1bits.FREE = 1; }
    #define EraseOff()              { EECON1bits.FREE = 0; }

/**********************************************************************/
#elif defined(__18f26j50) || defined(__18f46j50) || \
      defined(__18f26j53) || defined(__18f46j53) || \
      defined(__18f27j53) || defined(__18f47j53)
/**********************************************************************/

    #define FLASHBLOCKSIZE          1024    // the erase block is 1024-byte long
    
    #define Unlock()                {                       \
                                      EECON2 = 0x55;        \
                                      EECON2 = 0xAA;        \
                                      EECON1bits.WR = 1;    \
                                    }
    
    #ifdef __XC8__

    #define NextBlock() { \
    asm("BANKSEL TBLPTRH         ; Select Bank for registers        ");\
    asm("MOVLW   0x04            ; 0x04 -> W                        ");\
    asm("ADDWF   TBLPTRH, F      ;  (W) + (TBLPTRH) -> TBLPTRH      ");\
    asm("                        ;  (C) is affected                 ");\
    asm("MOVLW   0x00            ; 0x00 -> W                        ");\
    asm("ADDWFC  TBLPTRU, F      ;  (W) + (TBLPTRU) + (C) -> TBLPTRU");\
    asm("                        ; Add W and Carry Bit to F         ");\
    }
    #else

    #define NextBlock() { \
    __asm__("BANKSEL _TBLPTRH    ; Select Bank for registers        ");\
    __asm__("MOVLW   0x04        ; 0x04 -> W                        ");\
    __asm__("ADDWF   _TBLPTRH, F ;  (W) + (TBLPTRH) -> TBLPTRH      ");\
    __asm__("                    ;  (C) is affected                 ");\
    __asm__("MOVLW   0x00        ; 0x00 -> W                        ");\
    __asm__("ADDWFC  _TBLPTRU, F ;  (W) + (TBLPTRU) + (C) -> TBLPTRU");\
    __asm__("                    ; Add W and Carry Bit to F         ");\
    }

    #endif

    #define EraseOn()               { EECON1bits.FREE = 1; }
    #define EraseOff()              { EECON1bits.FREE = 0; }

/**********************************************************************/
#endif
/**********************************************************************/

#endif //_FLASH_H
