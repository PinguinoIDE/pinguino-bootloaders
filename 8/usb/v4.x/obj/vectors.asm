;--------------------------------------------------------
; File Created by SDCC : free open source ANSI-C Compiler
; Version 3.3.1 #8894 (Oct 11 2013) (Linux)
; This file was generated Tue Dec 17 16:05:28 2013
;--------------------------------------------------------
; PIC16 port for the Microchip 16-bit core micros
;--------------------------------------------------------
	list	p=18f26j50
	radix	dec


;--------------------------------------------------------
; public variables in this module
;--------------------------------------------------------
	global	_stack
	global	_stack_end
	global	_reset_isr
	global	_high_priority_isr
	global	_startup
	global	_low_priority_isr

;--------------------------------------------------------
; extern variables in this module
;--------------------------------------------------------
	extern	_main


ustat_vectors_00	udata	0X0100
_stack         	res	254
_stack_end     	res	1

;--------------------------------------------------------
; global & static initialisations
;--------------------------------------------------------
; ; Starting pCode block
S_vectors__reset_isr	code	0X000000
_reset_isr:
	goto _startup
	
; ; Starting pCode block
S_vectors__high_priority_isr	code	0X000008
_high_priority_isr:
	goto 0x0C00 + 0x08
	
; ; Starting pCode block
S_vectors__low_priority_isr	code	0X000018
_low_priority_isr:
	goto 0x0C00 + 0x18
	
; I code from now on!
; ; Starting pCode block
S_vectors__startup	code
_startup:
	lfsr 1, _stack_end ; initialize the stack pointer
	lfsr 2, _stack_end
	
;	.line	116; src/vectors.c	main();                     // start bootloader code
	CALL	_main
lockup:
	bra lockup ; returning from main will lock up
	


; Statistics:
; code size:	   28 (0x001c) bytes ( 0.02%)
;           	   14 (0x000e) words
; udata size:	  255 (0x00ff) bytes ( 7.24%)
; access size:	    0 (0x0000) bytes


	end
