;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;  BootLoader.                                                             ;;
;;  Copyright (C) 2007 Diolan ( http://www.diolan.com )                     ;;
;;                                                                          ;;
;;  This program is free software: you can redistribute it and/or modify    ;;
;;  it under the terms of the GNU General Public License as published by    ;;
;;  the Free Software Foundation, either version 3 of the License, or       ;;
;;  (at your option) any later version.                                     ;;
;;                                                                          ;;
;;  This program is distributed in the hope that it will be useful,         ;;
;;  but WITHOUT ANY WARRANTY; without even the implied warranty of          ;;
;;  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           ;;
;;  GNU General Public License for more details.                            ;;
;;                                                                          ;;
;;  You should have received a copy of the GNU General Public License       ;;
;;  along with this program.  If not, see <http://www.gnu.org/licenses/>    ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

	#include "p18f4550.inc"
	#include "boot.inc"
	#include "io_cfg.inc"

	extern	main  
	extern	enable_xinst

;-----------------------------------------------------------------------------
;--- START - RESET Vector
;-----------------------------------------------------------------------------
VECTORS		CODE	0x0000
	org		0x0000
	rcall	enable_xinst
	clrf	TBLPTRU
	clrf	TBLPTRH
	bra		pre_main
;-----------------------------------------------------------------------------
;--- HIGH Interrupt Vector
;-----------------------------------------------------------------------------
	org		0x0008
	goto	APP_HIGH_INTERRUPT_VECTOR		; 0x0008
;-----------------------------------------------------------------------------
pre_main
;-----------------------------------------------------------------------------
	; All I/O to Digital mode
	movlw	0x0F							; 0x000A
	movwf	ADCON1							; 0x000C
	; LED_PIN output
	bcf		LED_TRIS, LED_PIN				; 0x000E
	; led on
	bsf		LED, LED_PIN					; 0x0010
	bra		main							; 0x0012
	;free	address							; 0x0014
	;free	address							; 0x0016
;-----------------------------------------------------------------------------
;--- LOW Interrupt Vector
;-----------------------------------------------------------------------------
	org		0x0018
	goto	APP_LOW_INTERRUPT_VECTOR		; 0x0018
;-----------------------------------------------------------------------------
;--- APPLICATION STUB
;-----------------------------------------------------------------------------
APPSTRT CODE APP_RESET_VECTOR
	bra		$								; 0x001A
;-----------------------------------------------------------------------------
	END
