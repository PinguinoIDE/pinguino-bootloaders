/*******************************************************************************
	Title:	USB Pinguino Bootloader
	File:	core.h
	Descr.: Core routine for MIPS processor
	Author:	Régis Blanchot <rblanchot@gmail.com>

	This file is part of Pinguino (http://www.pinguino.cc)
	Released under the LGPL license (http://www.gnu.org/licenses/lgpl.html)
*******************************************************************************/

#ifndef _CORE_H_
#define _CORE_H_

#include "p32xxxx.h"                            // Registers definitions
#include "typedefs.h"

// For all PIC32 supported
#define CRYSTALFREQUENCY        8000000UL       // 8 MHz
#define PLLIDIV                 (1  + DEVCFG2bits.FPLLIDIV)  // Only valid if divider <= 6
#define PLLODIV                 (1 << OSCCONbits.PLLODIV)
#define PLLMULT                 (15 + OSCCONbits.PLLMULT)
#define PBDIV                   (1 << OSCCONbits.PBDIV)
#define FCPU                    (CRYSTALFREQUENCY / PLLIDIV * PLLMULT / PLLODIV)
#define FPB                     (FCPU / PBDIV)
#define FCP0                    (FCPU / 2)    // Core Timer runs at half the CPU rate

//UINT32 MIPS32 DisableInterrupt(void);
//UINT32 MIPS32 EnableInterrupt(void);
//void MIPS32 EnableMultiVectoredInt(void);

UINT32 MIPS32 ReadCoreTimer(void);
//void   MIPS32 ResetCoreTimer(void);

void MemClear (void *, UINT32);
void MemCopy  (void *, void *, UINT32);

void SoftReset(void);

#endif // _CORE_H_
