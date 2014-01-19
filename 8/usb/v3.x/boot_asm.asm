;-----------------------------------------------------------------------------
;  BootLoader.
;  Copyright (C) 2007 Diolan ( http://www.diolan.com )
;
;  This program is free software: you can redistribute it and/or modify
;  it under the terms of the GNU General Public License as published by
;  the Free Software Foundation, either version 3 of the License, or
;  (at your option) any later version.
;
;  This program is distributed in the hope that it will be useful,
;  but WITHOUT ANY WARRANTY; without even the implied warranty of
;  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;  GNU General Public License for more details.
;
;  You should have received a copy of the GNU General Public License
;  along with this program.  If not, see <http://www.gnu.org/licenses/>
;-----------------------------------------------------------------------------

;-----------------------------------------------------------------------------
; Flash Reading / Writing
;-----------------------------------------------------------------------------

	#include "p18f4550.inc"
	#include "boot.inc"
	#include "boot_if.inc"
	#include "usb_defs.inc"

;-----------------------------------------------------------------------------
; Constants
;-----------------------------------------------------------------------------
; boot_cmd & boot_rep
CMD_OFFS		equ	0
ID_OFFS			equ	1	; echo
ADDR_LO_OFFS	equ	2
ADDR_HI_OFFS	equ	3
ADDR_UP_OFFS	equ	4
SIZE_OFFS		equ	5
CODE_OFFS		equ	6
VER_MAJOR_OFFS	equ	2
VER_MINOR_OFFS	equ	3
VER_SMINOR_OFFS	equ	4
EEDATA_OFFS		equ	6

;-----------------------------------------------------------------------------
; Global Variables
;-----------------------------------------------------------------------------

	EXTERN	boot_cmd
	EXTERN	boot_rep
	EXTERN	hid_report_in

;-----------------------------------------------------------------------------
; Local Variables
;-----------------------------------------------------------------------------

BOOT_DATA	UDATA
cntr		res	1
hold_r		res	1	; Current Holding Register for tblwt
_fsr		res	4	; Temporary storage for FSR's
xinst		res 1	; Extended Instruction Flag
_config		res 32	; Temporary storage for config. registers

#define		_CONFIG	_CONFIG1L
;-----------------------------------------------------------------------------
; START
;-----------------------------------------------------------------------------

BOOT_ASM_CODE	CODE
	GLOBAL	hid_process_cmd
	GLOBAL	copy_boot_rep
	GLOBAL	enable_xinst
	GLOBAL	disable_xinst

;-----------------------------------------------------------------------------
; erase_code 
;-----------------------------------------------------------------------------
; DESCR : Erase 64 bytes block
; INPUT : boot_cmd
; OUTPUT: 
; NOTES : Assume TBLPTRU=0
;-----------------------------------------------------------------------------

erase_code
	rcall	load_address	; TBLPTR = addr

erase_code_loop
	; while( size_x64 )
	rcall	flash_erase		; Erase block
	
	; TBLPTR += 64
	movlw	0x40
	addwf	TBLPTRL
	movlw	0x00
	addwfc	TBLPTRH
	
	decfsz	boot_cmd + SIZE_OFFS
	bra		erase_code_loop
	return

;-----------------------------------------------------------------------------
; read_device_id
;-----------------------------------------------------------------------------
; DESCR : read device id at <0x3FFFFE:0x3FFFFF>
; INPUT :
; OUTPUT: boot_rep
; NOTES : added 15-01-2012 - regis blanchot <rblanchot@gmail.com>
;-----------------------------------------------------------------------------

read_deviceid
	movlw	0x3F
	movwf	TBLPTRU
	movlw	0xFF
	movwf	TBLPTRH
	movlw	0xFE
	movwf	TBLPTRL		; TBLPTR=0x3FFFFE
	movlw	.2
	movwf	cntr		; cntr=2
	movwf	boot_rep + SIZE_OFFS
	lfsr	FSR0, boot_rep + CODE_OFFS	; FSR0=&boot_rep.data
	rcall	flash_read
	;clrf	TBLPTRU
	return

;-----------------------------------------------------------------------------
; write_code 
;-----------------------------------------------------------------------------
; DESCR : write 32-byte blocks in flash
; INPUT : boot_cmd
; OUTPUT: 
; NOTES : Assume TBLPTRU=0
;-----------------------------------------------------------------------------

write_code
	; TBLPTR = addr
	rcall	load_address_size8			; TBLPTR=addr cntr=size8 & 0x3C
	lfsr	FSR0, boot_cmd + CODE_OFFS	; FSR0=&boot_cmd.data
	tblrd*-								; TBLPTR--
	
	; while( cntr-- )
write_code_loop
	movff	POSTINC0, TABLAT
	tblwt+*						; *(++Holding_Register) = *data++
	incf	hold_r				; hold_r++
	btfsc	hold_r, 5			; if( hold_r == 0x20 )  End of Holding Area
	rcall	flash_write			; dump holding area to flash
	decfsz	cntr
	bra		write_code_loop

	tstfsz	hold_r				; if( hold_r != 0 )     Holding Area not dumped
	rcall	flash_write			; dump holding area to flash
	return

;-----------------------------------------------------------------------------
;       hid_process_cmd
;-----------------------------------------------------------------------------
; DESCR : Process HID command in boot_cmd
; INPUT : 
; OUTPUT: 
; NOTES : 
;-----------------------------------------------------------------------------

hid_process_cmd
	movf	boot_cmd + CMD_OFFS, W			; W = boot_cmd.cmd
	bz		return_hid_process_cmd			; if( boot_cmd.cmd == 0 ) return

	; Start processing
	movwf	boot_rep + CMD_OFFS			; boot_rep.cmd = boot_cmd.cmd
;-----------------------------------------------------------------------------
; TODO : remove
	movff	boot_cmd + ID_OFFS, boot_rep + ID_OFFS	; boot_rep.id = boot_cmd.id
;-----------------------------------------------------------------------------
	clrf	boot_cmd + CMD_OFFS			; boot_cmd.cmd = 0
	; switch( boot_cmd.cmd )
	dcfsnz	WREG
	bra		write_code		; cmd=1 BOOT_WRITE_FLASH
	dcfsnz	WREG
	bra		erase_code		; cmd=2 BOOT_ERASE_FLASH
	dcfsnz	WREG
	bra		get_fw_version	; cmd=3 BOOT_GET_FW_VER
	dcfsnz	WREG
	bra		soft_reset		; cmd=4 BOOT_RESET
	dcfsnz	WREG
	bra		read_deviceid	; cmd=5 BOOT_READ_DEVID

	; If command is not processed sned back BOOT_CMD_UNKNOWN                                                                         
unknown_cmd
	movlw	BOOT_CMD_UNKNOWN
	movwf	boot_rep + CMD_OFFS	; boot_rep.cmd = BOOT_CMD_UNKNOWN
	
return_hid_process_cmd
	return
	
;-----------------------------------------------------------------------------
;       get_fw_version 
;-----------------------------------------------------------------------------
; DESCR : get_fw_version
; INPUT : 
; OUTPUT: 
; NOTES : 
;-----------------------------------------------------------------------------

get_fw_version
	movlw	FW_VER_MAJOR
	movwf	boot_rep + VER_MAJOR_OFFS 
	movlw	FW_VER_MINOR
	movwf	boot_rep + VER_MINOR_OFFS 
	movlw	FW_VER_SUB_MINOR
	movwf	boot_rep + VER_SMINOR_OFFS
	return

;-----------------------------------------------------------------------------
; write_config
;-----------------------------------------------------------------------------
; DESCR : Disable Extented Instruction Set Config. Bit
; INPUT : boot_cmd
; OUTPUT:
; NOTES : added 15-02-2012 - regis blanchot <rblanchot@gmail.com>
;			1- copy 32 bytes (in which config. registers) into _config
;			1- init _config with 32 x 0xFF
;			2- erase the block (64 bytes)
;			3- modify XINST bit copy into _config
;			4- write _config block into config. registers
;		  The default value of the holding registers on
;		  device Resets and after write operations is FFh.
;-----------------------------------------------------------------------------

enable_xinst
	bsf		xinst, 0
	bra		copy_config
disable_xinst
	bcf		xinst, 0

;	1- copy 32 bytes (in which config. registers) into _config
copy_config
	movlw	.32					; number of bytes in erase block
	movwf	cntr
	lfsr	FSR0, _config		; TO   = _config
	rcall	point_config		; FROM = 0x300000
	rcall	flash_read			; copy 32 bytes

;	2- erase the block (64 bytes)
erase_config
	btfsc	xinst, 0			; is xinst flag set ?
	bra		modify_copy
;	rcall	point_config		; FROM = 0x300000
	clrf	TBLPTRU
	movlw	0x60
	movwf	TBLPTRH
	clrf	TBLPTRL
	rcall	config_erase		; erase 64 bytes

;	3- modify XINST bit copy into _config
modify_copy
	btfss	xinst, 0			; is xinst flag set ?
	bra		xinst_disable
xinst_enable
	bsf		_config + 6, 6		; XINST = 1
	bra		write_config
xinst_disable
	bcf		_config + 6, 6		; XINST = 0

;	4- write _config block into config. registers
write_config
	movlw	.32					; number of bytes in holding register
	movwf	cntr
	lfsr	FSR0, _config		; FROM = _config
	;rcall	point_config		; TO   = 0x300000
	clrf	TBLPTRU
	movlw	0x60
	movwf	TBLPTRH
	clrf	TBLPTRL
	;tblrd*-						; to point on the good address
write_config_loop
	movff	POSTINC0, TABLAT	; TABLAT = _config
	tblwt*+						; buffer++ = TABLAT
	decfsz	cntr
	bra		write_config_loop
	rcall	config_write		; write buffer in flash

	clrf	TBLPTRU
	return

point_config					; TBLPTR = 0x300000
	movlw	0x30				; UPPER(_CONFIG)
	movwf	TBLPTRU
	clrf	TBLPTRH
	clrf	TBLPTRL
	return

;-----------------------------------------------------------------------------
;       soft_reset       
;-----------------------------------------------------------------------------
; DESCR : Reset (called by uploader)         
; INPUT : 
; OUTPUT: 
; NOTES : Disable also Extended Instruction Set
;		  added 15-02-2012 - regis blanchot <rblanchot@gmail.com>
;-----------------------------------------------------------------------------

	; Soft Reset and run Application FW
soft_reset
	; Disable USB Engine
	bcf     UCON, USBEN
	; Delay to show USB device reset
;	clrf	cntr
;	clrf	WREG
;	decfsz	WREG
;	bra		$ - 2
;	decfsz	cntr
;	bra		$ - 8
	; Disable Extended Instruction Set (XINST=OFF)
	rcall	disable_xinst
	; Now, bootloader code will not work anymore
	; Until Extended Instruction Set is enabled again.
	; Launch user app.
	goto	APP_RESET_VECTOR
	;reset

;-----------------------------------------------------------------------------
;       copy_boot_rep 
;-----------------------------------------------------------------------------
; DESCR : boot_rep => hid_report_in, boot_rep <= 0
; INPUT : boot_rep
; OUTPUT: 
; NOTES : 
;-----------------------------------------------------------------------------

copy_boot_rep
	; Store FSR1,FSR2
	movff	FSR1L, _fsr                         
	movff	FSR1H, _fsr + 1
	movff	FSR2L, _fsr + 2
	movff	FSR2H, _fsr + 3
	
	lfsr	FSR0, boot_rep
	lfsr	FSR1, hid_report_in

	; while( w )
	movlw	HID_IN_EP_SIZE
copy_boot_rep_loop
	movff	INDF0, POSTINC1
	clrf	POSTINC0
	decfsz	WREG
	bra		copy_boot_rep_loop
	
	; restore FSR1,FSR2 and return
	movff	_fsr, FSR1L                        
	movff	_fsr + 1, FSR1H                          
	movff	_fsr + 2, FSR2L                        
	movff	_fsr + 3, FSR2H                          

	return

;-----------------------------------------------------------------------------
; Local Functions
;-----------------------------------------------------------------------------
; NOTES : regis blanchot <rblanchot@gmail.com>
;		  added 15-02-2012 - TBLPTRU support
;		  added 16-02-2012 - config_erase, flash_erase, config_write and flash_read
;-----------------------------------------------------------------------------
; cntr = boot_rep_size8 = boot_cmd.size8 & 0x3C

load_address_size8
	movf	boot_cmd + SIZE_OFFS, W
	andlw	0x3C
	movwf	cntr
	movwf	boot_rep + SIZE_OFFS
; TBLPTR = boot_rep.addr = boot_cmd.addr
load_address
	movf	boot_cmd + ADDR_UP_OFFS, W
	movwf	TBLPTRU
	movwf	boot_rep + ADDR_UP_OFFS
	movf	boot_cmd + ADDR_HI_OFFS, W
	movwf	TBLPTRH
	movwf	boot_rep + ADDR_HI_OFFS
	movf	boot_cmd + ADDR_LO_OFFS, W
	movwf	TBLPTRL
	movwf	boot_rep + ADDR_LO_OFFS
	andlw	0x1F
	movwf	hold_r			; hold_r = boot_cmd.addr_lo & 0x1F
	return
config_erase				; NB: EECON1.FREE will be cleared by Hardware
	bsf		EECON1, CFGS	; access config. register (not flash)
	bsf		EECON1, FREE	; Enable row Erase (not PROGRAMMING)
	bra		do_write		; perform write
config_write
	bsf		EECON1, CFGS	; access config. register (not flash)
	bra		do_write		; perform write
flash_erase					; NB: EECON1.FREE will be cleared by Hardware
	bcf		EECON1, CFGS	; access flash memory (not config)
	bsf		EECON1, FREE	; Enable row Erase (not PROGRAMMING)
	bra		do_write		; perform write
flash_write
	bcf		EECON1, CFGS	; access flash program memory (not config)
do_write
	bsf		EECON1, EEPGD	; access flash program memory (not eeprom)
	bsf		EECON1, WREN	; enable write to memory
	movlw	0x55
	movwf	EECON2
	movlw	0xAA
	movwf	EECON2
	bsf		EECON1, WR		; start flash/eeprom writing
	nop						; proc. can forget to execute the first operation
	bcf		EECON1, WREN	; disable write to memory
	clrf	hold_r			; hold_r=0
	return
flash_read
	tblrd*+					; address is in FSR0
	movff	TABLAT, POSTINC0
	decfsz	cntr			; while( cntr-- )
	bra		flash_read
	return
;-----------------------------------------------------------------------------
	END
