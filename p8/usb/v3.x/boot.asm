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

;-----------------------------------------------------------------------------
; BootLoader Main code
;-----------------------------------------------------------------------------
	#include "p18f4550.inc"
	#include "boot.inc"
	#include "io_cfg.inc"
	#include "usb_defs.inc"
	#include "usb_desc.inc"
	#include "usb.inc"
	#include "boot_if.inc"
;-----------------------------------------------------------------------------
; Configuration Bits 
;-----------------------------------------------------------------------------
	CONFIG  PLLDIV = 5			; OSC/5 for 20MHz = 4MHz
	CONFIG  CPUDIV = OSC1_PLL2	; CPU_clk = PLL/2 = 48MHz
	CONFIG  USBDIV = 2			; USB_clk = PLL/2 = 48MHz
	CONFIG  FOSC = HSPLL_HS		; HS osc PLL
	CONFIG  FCMEN = ON			; Fail Safe Clock Monitor
	CONFIG  IESO = OFF			; Int/Ext switchover mode
	CONFIG  PWRT = ON			; PowerUp Timer
	CONFIG  BOR = OFF			; Brown Out
	CONFIG  VREGEN = ON			; Int Voltage Regulator
	CONFIG  WDT = OFF			; WatchDog Timer
	CONFIG  MCLRE = ON			; MCLR
	CONFIG  LPT1OSC = OFF		; Low Power OSC
	CONFIG  PBADEN = ON			; PORTB<4:0> A/D
	CONFIG  CCP2MX = ON			; CCP2 Mux RC1
	CONFIG  STVREN = ON			; Stack Overflow Reset
	CONFIG  LVP = OFF			; Low Voltage Programming
	CONFIG  ICPRT = OFF			; ICP
;-----------------------------------------------------------------------------
	CONFIG  XINST = ON			; Ext. CPU Instruction Set
;-----------------------------------------------------------------------------
	CONFIG  DEBUG = OFF			; Background Debugging
	CONFIG  CP0 = OFF			; Code Protect
	CONFIG  CP1 = OFF
	CONFIG  CP2 = OFF
	CONFIG  CPB = OFF   		; Boot Sect Code Protect (OxOOO to 0x7FF)
	CONFIG  CPD = OFF  			; EEPROM Data Protect
	CONFIG  WRT0 = OFF 			; Table Write Protect
	CONFIG  WRT1 = OFF
	CONFIG  WRT2 = OFF 
	CONFIG  WRTB = OFF 			; Boot Table Write Protect
	CONFIG  WRTC = OFF 			; CONFIG Write Protect
	CONFIG  WRTD = OFF 			; EEPROM Write Protect
	CONFIG  EBTR0 = OFF			; Ext Table Read Protect
	CONFIG  EBTR1 = OFF
	CONFIG  EBTR2 = OFF
	CONFIG  EBTRB = OFF 		; Boot Table Read Protect
;--------------------------------------------------------------------------
; External declarations
;--------------------------------------------------------------------------
	extern	usb_sm_state
	extern	usb_sm_ctrl_state
	extern	ep1Bo
	extern	ep1Bi
	extern	SetupPkt
	extern	SetupPktCopy
	extern	pSrc
	extern	pDst
	extern	Count
	extern	ctrl_trf_session_owner
	extern	ctrl_trf_mem
;--------------------------------------------------------------------------
; Variables
;--------------------------------------------------------------------------
BOOT_DATA	UDATA
	global	boot_cmd; 
	global	boot_rep;
	global	active_protocol
	global	idle_rate
active_protocol	res	1
idle_rate		res	1
boot_cmd		res	BOOT_CMD_SIZE
boot_rep		res	BOOT_REP_SIZE
;--------------------------------------------------------------------------
; HID buffers
;--------------------------------------------------------------------------
USB_HID		UDATA	0x500
	global	hid_report_out
	global	hid_report_in
hid_report_out	res	HID_OUT_EP_SIZE	; OUT packet buffet
hid_report_in	res	HID_IN_EP_SIZE	; IN packed buffer
;--------------------------------------------------------------------------
BOOT_ASM_CODE CODE
;--------------------------------------------------------------------------
	extern	usb_init
	extern	usb_sm_ctrl
	extern	usb_sm_reset
	extern	usb_sm_prepare_next_setup_trf
	extern	copy_boot_rep
	extern	USB_HID_DESC
	extern	USB_HID_RPT
	extern	hid_process_cmd
;--------------------------------------------------------------------------
; main
; DESCR :	Boot Loader main routine.
; INPUT :	no
; OUTPUT: no
;--------------------------------------------------------------------------

; F = Fosc/4 = 12MHz, T = 1/12MHz = 83.33 ns
; prescaler = 8, Timer1 is not preloaded
; 8 * 256 * 256 * 229 * 83.33 ns =  10.005 sec.
t1_count		equ 229			; wait for 229 timer1 overload
; led_count blink tempo
lc_lo			equ 0xFF
lc_hi			equ 0x77

	global	main
main
	; Init led blinking
	bcf		LED_TRIS, LED_PIN	; LED_PIN output
	bsf		LED, LED_PIN		; led off
	movlw	b'00110001'			; Timer 1 on, prescaler 8 (0b11)
	movwf	T1CON
	bsf		PIE1, TMR1IE		; Enable Timer1 interrupt
	rcall	usb_init			; init USB
bootloader_loop
	rcall	usb_state_machine	; Process low level action on USB controller
	rcall	hid_process_cmd		; Initialize Endpoints for HID
	rcall	hid_send_report		; Process USB HID requests
led_count
	decfsz	lc_lo	, f			; led_count--, skip if zero
	bra		timer1_test			; if cd_lo > 0 go to timer1_test
	decfsz	lc_hi	, f			; led_count--, skip if zero
	bra		timer1_test			; if cd_hi > 0 go to timer1_test
reload_led_count				; if cd_up == 0 reload led_count
;	movlw	0xFF
;	movwf	lc_lo	
	movlw	0x77	
	movwf	lc_hi	
toggle_led
	movlw	LED_MASK			; toggle
	xorwf	LED, f				; the led
timer1_test
	btfss	PIR1, TMR1IF		; skip if PIR1.TMR1IF == 1
	bra		bootloader_loop		; no, then loop
	bcf		PIR1, TMR1IF		; yes, PIR1.TMR1IF = 0
	decfsz	t1_count, f			; t1_count-- and skip if zero
	bra		bootloader_loop		; if t1_count > 0 then loop
disable_boot					; 10 sec. has expired
	clrf	T1CON				; disable timer 1
	clrf	UCON				; disable USB
	bcf		LED, LED_PIN		; led off
	goto	APP_RESET_VECTOR	; Run Application FW
	reset
	
;-----------------------------------------------------------------------------
; usb_state_machine
; DESCR :	Handles USB state machine according to USB Spec.
; 		Process low level action on USB controller.
; INPUT :	no
; OUTPUT: no
;-----------------------------------------------------------------------------
	global	usb_state_machine
usb_state_machine
	; Bus Activity detected after IDLE state
usb_state_machine_actif
	btfss	UIR, ACTVIF
	bra		usb_state_machine_actif_end
	btfss	UIE, ACTVIE
	bra		usb_state_machine_actif_end
	bcf		UCON, SUSPND
	bcf		UIE, ACTVIE
	bcf		UIR, ACTVIF
usb_state_machine_actif_end
	; Pointless to continue servicing if the device is in suspend mode.
	btfsc		UCON, SUSPND
	return
	; USB Bus Reset Interrupt.
	; When bus reset is received during suspend, ACTVIF will be set first,
	; once the UCONbits.SUSPND is clear, then the URSTIF bit will be asserted.


	; This is why URSTIF is checked after ACTVIF.
	;
	; The USB reset flag is masked when the USB state is in
	; DETACHED_STATE or ATTACHED_STATE, and therefore cannot
	; cause a USB reset event during these two states.
usb_state_machine_rstif
	btfss		UIR, URSTIF
	bra		usb_state_machine_rstif_end
	btfss		UIE, URSTIE
	bra		usb_state_machine_rstif_end
	rcall		usb_sm_reset
usb_state_machine_rstif_end
	; Idle condition detected
usb_state_machine_idleif
	btfss		UIR, IDLEIF
	bra		usb_state_machine_idleif_end
	btfss		UIE, IDLEIE
	bra		usb_state_machine_idleif_end
	bsf		UIE, ACTVIE	; Enable bus activity interrupt
	bcf		UIR, IDLEIF
	bsf		UCON, SUSPND	; Put USB module in power conserve
				; mode, SIE clock inactive
        ; Now, go into power saving
        bcf		PIR2, USBIF	; Clear flag
        bsf		PIE2, USBIE	; Set wakeup source
	sleep
        bcf		PIE2, USBIE
usb_state_machine_idleif_end
	; SOF Flag
usb_state_machine_sof
	btfss		UIR, UERRIF
	bra		usb_state_machine_sof_end
	btfss		UIE, UERRIE
	bra		usb_state_machine_sof_end
	bcf		UIR, SOFIF
usb_state_machine_sof_end

	; A STALL handshake was sent by the SIE
usb_state_machine_stallif
	btfss	UIR, STALLIF
	bra		usb_state_machine_stallif_end
	btfss	UIE, STALLIE
	bra		usb_state_machine_stallif_end
	btfss	UEP0, EPSTALL
	bra		usb_state_machine_stallif_clr
	rcall	usb_sm_prepare_next_setup_trf	; Firmware Work-Around
	bcf		UEP0, EPSTALL
usb_state_machine_stallif_clr
	bcf		UIR, STALLIF
usb_state_machine_stallif_end
	; USB Error flag
usb_state_machine_err
	btfss	UIR, UERRIF
	bra		usb_state_machine_err_end
	btfss	UIE, UERRIE
	bra		usb_state_machine_err_end
	bcf		UIR, UERRIF
usb_state_machine_err_end
	; Pointless to continue servicing if the host has not sent a bus reset.
	; Once bus reset is received, the device transitions into the DEFAULT
	; state and is ready for communication.
;    if( usb_sm_state < USB_SM_DEFAULT ) 
;		return;
	movlw	(USB_SM_DEFAULT - 1)	; Be carefull while changing USB_SM_* constants
	cpfsgt	usb_sm_state
	return
	; Detect Interrupt bit
usb_state_machine_trnif
	btfss	UIR, TRNIF
	bra		usb_state_machine_trnif_end
	btfss	UIE, TRNIE
	bra		usb_state_machine_trnif_end
	; Only services transactions over EP0.
	; Ignore all other EP transactions.
	rcall	usb_sm_ctrl
	bcf		UIR, TRNIF
usb_state_machine_trnif_end
	return

;-----------------------------------------------------------------------------
; HID
;-----------------------------------------------------------------------------
; usb_sm_HID_init_EP
; DESCR : Initialize Endpoints for HID
; INPUT : no
; OUTPUT: no
;-----------------------------------------------------------------------------
	global	usb_sm_HID_init_EP
usb_sm_HID_init_EP
#define USE_HID_EP_OUT 0
#if USE_HID_EP_OUT
	movlw	EP_OUT_IN | HSHK_EN
	movwf	HID_UEP		; Enable 2 data pipes
	movlb	HIGH(HID_BD_OUT)
	movlw	HID_OUT_EP_SIZE
	movwf	BDT_CNT(HID_BD_OUT)
	movlw	LOW(hid_report_out)
	movwf	BDT_ADRL(HID_BD_OUT)
	movlw	HIGH(hid_report_out)
	movwf	BDT_ADRH(HID_BD_OUT)
	movlw	(_USIE | _DAT0 | _DTSEN)
	movwf	BDT_STAT(HID_BD_OUT)
#else
	movlw	(EP_IN | HSHK_EN)
	movwf	HID_UEP		; Enable 1 data pipe
#endif
	movlb	HIGH(HID_BD_IN)
	movlw	LOW(hid_report_in)
	movwf	BDT_ADRL(HID_BD_IN)
	movlw	HIGH(hid_report_in)
	movwf	BDT_ADRH(HID_BD_IN)
	movlw	(_UCPU | _DAT1)
	movwf	BDT_STAT(HID_BD_IN)
	movlb	0
	clrf		(boot_rep + cmd)
	return
;--------------------------------------------------------------------------
; hid_send_report
; DESCR : Sends HID reports to host
; INPUT : no
; OUTPUT: no
; Resources:
;	FSR2:	BDTs manipulations
;-----------------------------------------------------------------------------
	global	hid_send_report
hid_send_report
	movf		(boot_rep + cmd), W	; Z flag affected
	bz		hid_send_report_end
	lfsr		FSR2, BDT_STAT(HID_BD_IN)
	btfsc		POSTINC2, UOWN		; BDT_STAT(HID_BD_IN)
	bra		hid_send_report_end
	; Copy boot_rep into hid_report_in
	; Clear bytes of boot_rep
	rcall		copy_boot_rep	; BSR not changed
	; BSR still valid for HID_BD_IN
	movlw	BOOT_REP_SIZE
	; FSR2 points to BDT_CNT(HID_BD_IN)
	movwf	POSTDEC2	; BDT_CNT(HID_BD_IN)
	; FSR2 points to BDT_STAT(HID_BD_IN)
	movlw	_DTSMASK
	andwf	INDF2, F	; BDT_STAT(HID_BD_IN), Save only DTS bit
	movlw	(1 << DTS)
	xorwf	INDF2, F	; BDT_STAT(HID_BD_IN), Toggle DTS bit
	movlw	_USIE|_DTSEN
	iorwf		INDF2, F	; BDT_STAT(HID_BD_IN), Turn ownership to SIE
hid_send_report_end
	return

;--------------------------------------------------------------------------
; usb_sm_HID_request
; DESCR : Process USB HID requests
; INPUT : no
; OUTPUT: no
;-----------------------------------------------------------------------------
	global	usb_sm_HID_request
usb_sm_HID_request
	movf		(SetupPktCopy + Recipient), W
	andlw	RCPT_MASK
	sublw	RCPT_INTF
	btfss		STATUS, Z
	return
usb_sm_HID_rq_rcpt
	movf		(SetupPktCopy + bIntfID), W
	sublw	HID_INTF_ID
	btfss		STATUS, Z
	return
usb_sm_HID_rq_rcpt_id
	; There are two standard requests that we may support.
	; 1. GET_DSC(DSC_HID,DSC_RPT,DSC_PHY);
	; 2. SET_DSC(DSC_HID,DSC_RPT,DSC_PHY);
	movf		(SetupPktCopy + bRequest), W
	sublw	GET_DSC
	bnz		usb_sm_HID_rq_cls
	movf		(SetupPktCopy + bDscType), W
	; WREG = WREG - DSC_HID !!!
	addlw	(-DSC_HID)	; DSC_HID = 0x21
	bz		usb_sm_HID_rq_dsc_hid
	dcfsnz	WREG		; DSC_RPT = 0x22
	bra		usb_sm_HID_rq_dsc_rpt
	dcfsnz	WREG		; DSC_PHY = 0x23
	bra		usb_sm_HID_rq_dsc_phy
usb_sm_HID_rq_dsc_unknown
	bra		usb_sm_HID_rq_cls
;--------	Get DSC_HID descrptor address
usb_sm_HID_rq_dsc_hid
	movlw	LOW(USB_HID_DESC)
	movwf	pSrc
	movlw	HIGH(USB_HID_DESC)
	movwf	(pSrc + 1)
	movlw	USB_HID_DESC_SIZE
usb_sm_HID_rq_dsc_hid_end
	bra		usb_sm_HID_rq_dsc_end
;--------	Get DSC_RPT descrptor address
usb_sm_HID_rq_dsc_rpt
	movlw	LOW(USB_HID_RPT)
	movwf	pSrc
	movlw	HIGH(USB_HID_RPT)
	movwf	(pSrc + 1)
	movlw	USB_HID_RPT_SIZE
usb_sm_HID_rq_dsc_rpt_end
	bra		usb_sm_HID_rq_dsc_end
;--------	Get DSC_PHY descrptor address
usb_sm_HID_rq_dsc_phy
usb_sm_HID_rq_dsc_phy_end
	bra		usb_sm_HID_request_end
;--------
usb_sm_HID_rq_dsc_end
	movwf	Count
	bsf		ctrl_trf_session_owner, 0
	bsf		ctrl_trf_mem, _RAM
	bra		usb_sm_HID_request_end
;--------
; Class Request
usb_sm_HID_rq_cls
	movf		(SetupPktCopy + bmRequestType), W
	andlw	RQ_TYPE_MASK
	sublw	CLASS
	bz		usb_sm_HID_rq_cls_rq
	return
;--------
usb_sm_HID_rq_cls_rq
	movf	(SetupPktCopy + bRequest), W
	dcfsnz	WREG	; GET_REPORT = 0x01
	bra		usb_sm_HID_rq_cls_rq_grpt
	dcfsnz	WREG	; GET_IDLE = 0x02
	bra		usb_sm_HID_request_end
	dcfsnz	WREG	; GET_PROTOCOL = 0x03
	bra		usb_sm_HID_request_end
	; SET_REPORT = 0x09 -> 9 - 3 = 6
	; WREG = WREG - 6 !!!
	addlw	(-(SET_REPORT - GET_PROTOCOL))
	bz		usb_sm_HID_rq_cls_rq_srpt
	dcfsnz	WREG	; SET_IDLE = 0x0A
	bra		usb_sm_HID_request_end
	dcfsnz	WREG	; SET_PROTOCOL = 0x0B
	bra		usb_sm_HID_request_end
usb_sm_HID_rq_cls_rq_unknown
	bra		usb_sm_HID_request_end
;--------	GET_REPORT
usb_sm_HID_rq_cls_rq_grpt
	movlw	0		; No data to be transmitted
usb_sm_HID_rq_cls_rq_grpt_end
	bra		usb_sm_HID_rq_cls_rq_end
;--------	SET_REPORT
usb_sm_HID_rq_cls_rq_srpt
	movlw	LOW(boot_cmd)
	movwf	pDst
	movlw	HIGH(boot_cmd)
	movwf	(pDst + 1)
usb_sm_HID_rq_cls_rq_srpt_end
	bra		usb_sm_HID_rq_cls_rq_end_ses
usb_sm_HID_rq_cls_rq_end
	movwf	Count
	bcf		ctrl_trf_mem, _RAM
usb_sm_HID_rq_cls_rq_end_ses
	bsf		ctrl_trf_session_owner, 0
;--------
usb_sm_HID_request_end
	return
;--------------------------------------------------------------------------
	END
