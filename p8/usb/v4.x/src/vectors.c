/***********************************************************************
    Title:	USB Pinguino Bootloader
    File:	vectors.c
    Descr.: move interrupt vectors
            minimal initialisation routine
    Author:	Régis Blanchot <rblanchot@gmail.com>
************************************************************************
    Changelog
    * 2015-09-11    RB - added PIC16F interrupt vector
************************************************************************
    This file is part of Pinguino (http://www.pinguino.cc)
    Released under the LGPL license (http://www.gnu.org/licenses/lgpl.html)
************************************************************************
    NB : XC8
************************************************************************
    NB : SDCC
************************************************************************
1. The linker needs to know where the call stack should be placed.
    (a) You can use the likes of #pragma stack 0x200 0xff to tell it.
        This approach will emit a udata section placed at 0x200,
        and reserve 0xff (or even 0x100?) bytes there. Global labels
        _stack and _stack_end will be generated to refer to this piece
        of memory.
    (b) You can link against a file that provides  _stack and _stack_end
        symbols. The linker (well, read on) will then use these to set up
        the stack.
    (c) You can link with libsdcc.lib, which contains the assembled
        result of device/lib/pic16/libsdcc/stack/stack.S, which provides
        a stack as in (b) at a fixed location (0x200..0x2FF). If you want
        to override this stack, have the linker look at your .o file
        before it looks at the library. For this, the order of files on
        the command line matters...
2. At runtime, your PIC needs to initialize its FSRn registers.
    (a) You can do this yourself. No compiler support/linker support is
        needed, though you somehow have to make sure, the memory will not
        be otherwise. You will probably reserve a bank for this purpose
        or allocate a large array at some known address.
    (b) You can use the linker to provide your routines with the current
        address of your array (from (a)). You no longer need to place it
        at a defined location, but still need to set the FSRn registers
        (a la LFSR 0, _array_end).
    (c) You can let SDCC's PIC libraries' crt-*.o inside libsdcc.lib do
        the job: By convention, the libraries initialize FSR1 and FSR2 to
        point to "_stack_end", the value of _stack_end is filled in by
        the linker and used at runtime to define the register contents.

The common case (sdcc -mpic16 -p18f452 singlefileproject.c) will work
just fine, using the library-provided stack at 0x200-0x2FF from
libsdcc.lib UNLESS singleproject.c specifies another stack (the symbol
stack_end is required to point to its end), either via the #pragma stack
or "manually" via something like
   char __at(0x200) _stack[0xFF]; // reserve the memory region
   char __at(0x2FF) _stack_end;   // provide initial stack pointer
In any case, the approach above will link in crt0i.o and install its
RESET vector, so that the stack pointers will be set up correctly and
global and static variables be properly initialized.

Using
   sdcc -mpic16 -p18f452 -o part1.o -c part1.c
   sdcc -mpic16 -p18f452 -o part1.o -c part2.c
   gplink -o project.hex part1.o part2.o
is a different story, as neither libsdcc.lib nor crt0i.o are linked into
the project. In other words, neither the stack from the library, nor
(and worse) the RESET handler from crt0*.o is included in the final .hex
file. As a consequence, your global variables won't be initialized, your
stack pointers will point somewhere (most likely 0x00, which wraps round
right into the SFRs at 0xFFF after the first values are pushed on the
stack) and your project will run only a short while.
***********************************************************************/

#include "vectors.h"

#define PIC16F_ISR_OFFSET       0x0004
#define PIC18F_HI_ISR_OFFSET    0x0008
#define PIC18F_LO_ISR_OFFSET    0x0018

#ifdef __XC8__

    #ifdef _PIC14E

        // 0x0004, call user code interrupt vector
        void interrupt PIC16F_isr(void)
        {
            #asm
            LJMP    APPSTART + PIC16F_ISR_OFFSET
            #endasm;
        }

    #else

        // 0x0008
        void interrupt high_priority high_priority_isr(void)
        {
            #asm
            LJMP    APPSTART + PIC18F_HI_ISR_OFFSET
            #endasm;
        }

        // 0x0018
        void interrupt low_priority low_priority_isr(void)
        {
            #asm
            LJMP    APPSTART + PIC18F_LO_ISR_OFFSET
            #endasm;
        }

    #endif

#else // SDCC

    /*  never use --ivt-loc=$(ENTRY) to move the vectors
     *  as it will also move the Reset vector from 0 to ENTRY
     * 
     *  When generating assembly code for ISR the code generator places
     *  a goto instruction at the Interrupt Vector Address which points at
     *  the generated ISR. When declaring interrupt functions as _naked :
     *  1/ this goto instruction is not generated so we can place our own,
     *  2/ no registers are stored or restored.
     */

    /*  Emit a udata section and reserve bytes there.
     *  Global labels _stack and _stack_end will be generated
     *  to refer to this piece of memory.
     *  I choose 0x100 = bank1 because bank2, bank4, bank5 or bank13
     *  can be used as USB RAM depending on PIC18F family.
     *  bank1 is always free.
     */

    #pragma stack 0x100 0xFF

    // 0x0000

    void reset_isr(void) __naked __interrupt 0
    {
        __asm
        goto    _startup
        __endasm;
    }

    // 0x000C (if 0x0000 - 0x0C00 is not protected in the linker file) 

    void startup(void)
    {
        __asm
        lfsr    1, _stack_end       ; initialize the stack pointer
        lfsr    2, _stack_end
        __endasm;
        
        main();                     // start bootloader code

        __asm
    lockup:
        bra     lockup              ; returning from main will lock up
        __endasm;
    }

    #ifdef __SDCC_pic14

    // 0x0004
    void PIC16F_isr(void) __naked __interrupt 1
    {
        __asm
        goto    APPSTART + PIC16F_ISR_OFFSET
        __endasm;
    }

    #else
    
    // 0x0004 --> free

    // 0x0008
    void high_priority_isr(void) __naked __interrupt 1
    {
        __asm
        goto    APPSTART + PIC18F_HI_ISR_OFFSET
        __endasm;
    }

    // 0x0018
    void low_priority_isr(void) __naked __interrupt 2
    {
        __asm
        goto    APPSTART + PIC18F_LO_ISR_OFFSET
        __endasm;
    }

    // 0x001C --> start of bootloader code (cf. linker file)

    #endif

#endif
