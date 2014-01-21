/***********************************************************************
	Title:	USB Pinguino Bootloader
	File:	vectors.h
	Descr.: move interrupt vectors
            minimal initialisation routine
	Author:	Régis Blanchot <rblanchot@gmail.com>

	This file is part of Pinguino (http://www.pinguino.cc)
	Released under the LGPL license (http://www.gnu.org/licenses/lgpl.html)
***********************************************************************/

#ifndef _VECTORS_H
#define _VECTORS_H

extern stack_end;

extern void main(void);

void           startup(void) __naked;
void         reset_isr(void) __naked __interrupt 0;
void high_priority_isr(void) __naked __interrupt 1;
void  low_priority_isr(void) __naked __interrupt 2;

#endif
