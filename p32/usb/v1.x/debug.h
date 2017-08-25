/***********************************************************************
	Title:	USB Pinguino Bootloader
	File:	debug.h
	Descr.: Load debug functions if necessary
	Author:	Régis Blanchot <rblanchot@gmail.com>

	This file is part of Pinguino (http://www.pinguino.cc)
	Released under the LGPL license (http://www.gnu.org/licenses/lgpl.html)
***********************************************************************/

#ifndef _DEBUG_H_
#define _DEBUG_H_

// Bootloader compiled from the Makefile with PINGUINO/P32-GCC
#if defined(__P32GCC__)
    //#if (_DEBUG_ENABLE_)
    //    #define _DEBUG_ENABLE_
    //#endif
    
// Bootloader compiled with MPLABX
#else

#endif

#endif // _DEBUG_H_
