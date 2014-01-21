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
	global	_deviceAddress
	global	_selfPowered
	global	_EnableUSBModule
	global	_ProcessUSBTransactions
	global	_deviceState
	global	_currentConfiguration
	global	_ctrlTransferStage
	global	_requestHandled
	global	_outPtr
	global	_inPtr
	global	_wCount
	global	_ep_bdt
	global	_SetupPacket
	global	_controlTransferBuffer
	global	_bootCmd
	global	_InDataStage
	global	_WaitForSetupStage
	global	_device_descriptor
	global	_configuration_descriptor
	global	_lang
	global	_manu
	global	_prod
	global	_string_descriptor

;--------------------------------------------------------
; extern variables in this module
;--------------------------------------------------------
	extern	__gptrget1
	extern	__gptrput1
	extern	_PPSCONbits
	extern	_UEP0bits
	extern	_UEP1bits
	extern	_UEP2bits
	extern	_UEP3bits
	extern	_UEP4bits
	extern	_UEP5bits
	extern	_UEP6bits
	extern	_UEP7bits
	extern	_UEP8bits
	extern	_UEP9bits
	extern	_UEP10bits
	extern	_UEP11bits
	extern	_UEP12bits
	extern	_UEP13bits
	extern	_UEP14bits
	extern	_UEP15bits
	extern	_UIEbits
	extern	_UEIEbits
	extern	_UADDRbits
	extern	_UCFGbits
	extern	_PADCFG1bits
	extern	_REFOCONbits
	extern	_RTCCALbits
	extern	_RTCCFGbits
	extern	_ODCON3bits
	extern	_ODCON2bits
	extern	_ODCON1bits
	extern	_ANCON0bits
	extern	_ANCON1bits
	extern	_DSWAKELbits
	extern	_DSWAKEHbits
	extern	_DSCONLbits
	extern	_DSCONHbits
	extern	_TCLKCONbits
	extern	_CVRCONbits
	extern	_UFRMLbits
	extern	_UFRMHbits
	extern	_UIRbits
	extern	_UEIRbits
	extern	_USTATbits
	extern	_UCONbits
	extern	_CMSTATbits
	extern	_CMSTATUSbits
	extern	_SSP2CON2bits
	extern	_SSP2CON1bits
	extern	_SSP2STATbits
	extern	_SSP2ADDbits
	extern	_T4CONbits
	extern	_T3CONbits
	extern	_BAUDCON2bits
	extern	_BAUDCONbits
	extern	_BAUDCON1bits
	extern	_BAUDCTLbits
	extern	_PORTAbits
	extern	_PORTBbits
	extern	_PORTCbits
	extern	_HLVDCONbits
	extern	_DMACON2bits
	extern	_DMACON1bits
	extern	_LATAbits
	extern	_LATBbits
	extern	_LATCbits
	extern	_ALRMRPTbits
	extern	_ALRMCFGbits
	extern	_TRISAbits
	extern	_TRISBbits
	extern	_TRISCbits
	extern	_T3GCONbits
	extern	_T1GCONbits
	extern	_OSCTUNEbits
	extern	_RCSTA2bits
	extern	_PIE1bits
	extern	_PIR1bits
	extern	_IPR1bits
	extern	_PIE2bits
	extern	_PIR2bits
	extern	_IPR2bits
	extern	_PIE3bits
	extern	_PIR3bits
	extern	_IPR3bits
	extern	_EECON1bits
	extern	_TXSTA2bits
	extern	_RCSTAbits
	extern	_RCSTA1bits
	extern	_TXSTAbits
	extern	_TXSTA1bits
	extern	_CTMUICONbits
	extern	_CTMUCONLbits
	extern	_CTMUCONHbits
	extern	_CCP2CONbits
	extern	_ECCP2CONbits
	extern	_ECCP2DELbits
	extern	_PWM2CONbits
	extern	_ECCP2ASbits
	extern	_PSTR2CONbits
	extern	_CCP1CONbits
	extern	_ECCP1CONbits
	extern	_ECCP1DELbits
	extern	_PWM1CONbits
	extern	_ECCP1ASbits
	extern	_PSTR1CONbits
	extern	_WDTCONbits
	extern	_ADCON1bits
	extern	_ADCON0bits
	extern	_SSP1CON2bits
	extern	_SSPCON2bits
	extern	_SSP1CON1bits
	extern	_SSPCON1bits
	extern	_SSP1STATbits
	extern	_SSPSTATbits
	extern	_SSP1ADDbits
	extern	_SSPADDbits
	extern	_T2CONbits
	extern	_T1CONbits
	extern	_RCONbits
	extern	_CM2CONbits
	extern	_CM2CON1bits
	extern	_CM1CONbits
	extern	_CM1CON1bits
	extern	_OSCCONbits
	extern	_T0CONbits
	extern	_STATUSbits
	extern	_INTCON3bits
	extern	_INTCON2bits
	extern	_INTCONbits
	extern	_STKPTRbits
	extern	_RPOR0
	extern	_RPOR1
	extern	_RPOR2
	extern	_RPOR3
	extern	_RPOR4
	extern	_RPOR5
	extern	_RPOR6
	extern	_RPOR7
	extern	_RPOR8
	extern	_RPOR9
	extern	_RPOR10
	extern	_RPOR11
	extern	_RPOR12
	extern	_RPOR13
	extern	_RPOR17
	extern	_RPOR18
	extern	_RPINR1
	extern	_RPINR2
	extern	_RPINR3
	extern	_RPINR4
	extern	_RPINR6
	extern	_RPINR7
	extern	_RPINR8
	extern	_RPINR12
	extern	_RPINR13
	extern	_RPINR16
	extern	_RPINR17
	extern	_RPINR21
	extern	_RPINR22
	extern	_RPINR23
	extern	_RPINR24
	extern	_PPSCON
	extern	_UEP0
	extern	_UEP1
	extern	_UEP2
	extern	_UEP3
	extern	_UEP4
	extern	_UEP5
	extern	_UEP6
	extern	_UEP7
	extern	_UEP8
	extern	_UEP9
	extern	_UEP10
	extern	_UEP11
	extern	_UEP12
	extern	_UEP13
	extern	_UEP14
	extern	_UEP15
	extern	_UIE
	extern	_UEIE
	extern	_UADDR
	extern	_UCFG
	extern	_PADCFG1
	extern	_REFOCON
	extern	_RTCCAL
	extern	_RTCCFG
	extern	_ODCON3
	extern	_ODCON2
	extern	_ODCON1
	extern	_ANCON0
	extern	_ANCON1
	extern	_DSWAKEL
	extern	_DSWAKEH
	extern	_DSCONL
	extern	_DSCONH
	extern	_DSGPR0
	extern	_DSGPR1
	extern	_TCLKCON
	extern	_CVRCON
	extern	_UFRM
	extern	_UFRML
	extern	_UFRMH
	extern	_UIR
	extern	_UEIR
	extern	_USTAT
	extern	_UCON
	extern	_DMABCH
	extern	_DMABCL
	extern	_RXADDRH
	extern	_RXADDRL
	extern	_TXADDRH
	extern	_TXADDRL
	extern	_CMSTAT
	extern	_CMSTATUS
	extern	_SSP2CON2
	extern	_SSP2CON1
	extern	_SSP2STAT
	extern	_SSP2ADD
	extern	_SSP2BUF
	extern	_T4CON
	extern	_PR4
	extern	_TMR4
	extern	_T3CON
	extern	_TMR3
	extern	_TMR3L
	extern	_TMR3H
	extern	_BAUDCON2
	extern	_SPBRGH2
	extern	_BAUDCON
	extern	_BAUDCON1
	extern	_BAUDCTL
	extern	_SPBRGH
	extern	_SPBRGH1
	extern	_PORTA
	extern	_PORTB
	extern	_PORTC
	extern	_HLVDCON
	extern	_DMACON2
	extern	_DMACON1
	extern	_LATA
	extern	_LATB
	extern	_LATC
	extern	_ALRMVALL
	extern	_ALRMVALH
	extern	_ALRMRPT
	extern	_ALRMCFG
	extern	_TRISA
	extern	_TRISB
	extern	_TRISC
	extern	_T3GCON
	extern	_RTCVALL
	extern	_RTCVALH
	extern	_T1GCON
	extern	_OSCTUNE
	extern	_RCSTA2
	extern	_PIE1
	extern	_PIR1
	extern	_IPR1
	extern	_PIE2
	extern	_PIR2
	extern	_IPR2
	extern	_PIE3
	extern	_PIR3
	extern	_IPR3
	extern	_EECON1
	extern	_EECON2
	extern	_TXSTA2
	extern	_TXREG2
	extern	_RCREG2
	extern	_SPBRG2
	extern	_RCSTA
	extern	_RCSTA1
	extern	_TXSTA
	extern	_TXSTA1
	extern	_TXREG
	extern	_TXREG1
	extern	_RCREG
	extern	_RCREG1
	extern	_SPBRG
	extern	_SPBRG1
	extern	_CTMUICON
	extern	_CTMUCONL
	extern	_CTMUCONH
	extern	_CCP2CON
	extern	_ECCP2CON
	extern	_CCPR2
	extern	_CCPR2L
	extern	_CCPR2H
	extern	_ECCP2DEL
	extern	_PWM2CON
	extern	_ECCP2AS
	extern	_PSTR2CON
	extern	_CCP1CON
	extern	_ECCP1CON
	extern	_CCPR1
	extern	_CCPR1L
	extern	_CCPR1H
	extern	_ECCP1DEL
	extern	_PWM1CON
	extern	_ECCP1AS
	extern	_PSTR1CON
	extern	_WDTCON
	extern	_ADCON1
	extern	_ADCON0
	extern	_ADRES
	extern	_ADRESL
	extern	_ADRESH
	extern	_SSP1CON2
	extern	_SSPCON2
	extern	_SSP1CON1
	extern	_SSPCON1
	extern	_SSP1STAT
	extern	_SSPSTAT
	extern	_SSP1ADD
	extern	_SSPADD
	extern	_SSP1BUF
	extern	_SSPBUF
	extern	_T2CON
	extern	_PR2
	extern	_TMR2
	extern	_T1CON
	extern	_TMR1
	extern	_TMR1L
	extern	_TMR1H
	extern	_RCON
	extern	_CM2CON
	extern	_CM2CON1
	extern	_CM1CON
	extern	_CM1CON1
	extern	_OSCCON
	extern	_T0CON
	extern	_TMR0
	extern	_TMR0L
	extern	_TMR0H
	extern	_STATUS
	extern	_FSR2L
	extern	_FSR2H
	extern	_PLUSW2
	extern	_PREINC2
	extern	_POSTDEC2
	extern	_POSTINC2
	extern	_INDF2
	extern	_BSR
	extern	_FSR1L
	extern	_FSR1H
	extern	_PLUSW1
	extern	_PREINC1
	extern	_POSTDEC1
	extern	_POSTINC1
	extern	_INDF1
	extern	_WREG
	extern	_FSR0L
	extern	_FSR0H
	extern	_PLUSW0
	extern	_PREINC0
	extern	_POSTDEC0
	extern	_POSTINC0
	extern	_INDF0
	extern	_INTCON3
	extern	_INTCON2
	extern	_INTCON
	extern	_PROD
	extern	_PRODL
	extern	_PRODH
	extern	_TABLAT
	extern	_TBLPTR
	extern	_TBLPTRL
	extern	_TBLPTRH
	extern	_TBLPTRU
	extern	_PC
	extern	_PCL
	extern	_PCLATH
	extern	_PCLATU
	extern	_STKPTR
	extern	_TOS
	extern	_TOSL
	extern	_TOSH
	extern	_TOSU
	extern	_usb_ep_data_out_callback

;--------------------------------------------------------
;	Equates to used internal registers
;--------------------------------------------------------
STATUS	equ	0xfd8
WREG	equ	0xfe8
TBLPTRL	equ	0xff6
TBLPTRH	equ	0xff7
TBLPTRU	equ	0xff8
TABLAT	equ	0xff5
FSR0L	equ	0xfe9
POSTINC1	equ	0xfe6
POSTDEC1	equ	0xfe5
PREINC1	equ	0xfe4
PRODL	equ	0xff3
PRODH	equ	0xff4


; Internal registers
.registers	udata_ovr	0x0000
r0x00	res	1
r0x01	res	1
r0x02	res	1
r0x03	res	1
r0x04	res	1
r0x05	res	1
r0x06	res	1
r0x07	res	1
r0x08	res	1
r0x09	res	1
r0x0a	res	1


usbram5	udata
_controlTransferBuffer	res	64
_SetupPacket	res	64

udata_picUSB_0	udata
_selfPowered	res	1

udata_picUSB_1	udata
_deviceAddress	res	1

udata_picUSB_2	udata
_wCount	res	2

udata_picUSB_3	udata
_inPtr	res	3

udata_picUSB_4	udata
_outPtr	res	3

udata_picUSB_5	udata
_ctrlTransferStage	res	1

udata_picUSB_6	udata
_deviceState	res	1

udata_picUSB_7	udata
_currentConfiguration	res	1

udata_picUSB_8	udata
_requestHandled	res	1

udata_picUSB_9	udata
_bootCmd	res	64


ustat_picUSB_00	udata	0X0400
_ep_bdt        	res	16

;--------------------------------------------------------
; global & static initialisations
;--------------------------------------------------------
; I code from now on!
; ; Starting pCode block
S_picUSB__ProcessUSBTransactions	code
_ProcessUSBTransactions:
;	.line	255; src/picUSB.c	void ProcessUSBTransactions()
	MOVFF	r0x00, POSTDEC1
	MOVFF	r0x01, POSTDEC1
	MOVFF	r0x02, POSTDEC1
	MOVFF	r0x03, POSTDEC1
	MOVFF	r0x04, POSTDEC1
	MOVFF	r0x05, POSTDEC1
	MOVFF	r0x06, POSTDEC1
	MOVFF	r0x07, POSTDEC1
	MOVFF	r0x08, POSTDEC1
	MOVFF	r0x09, POSTDEC1
	MOVFF	r0x0a, POSTDEC1
; #	MOVF	_deviceState, W, B
; #	BTFSS	STATUS, 2
; #	GOTO	_00164_DS_
; #	GOTO	_00267_DS_
; #	BTFSS	_UIRbits, 2
	BANKSEL	_deviceState
;	.line	258; src/picUSB.c	if(deviceState == DETACHED)
	MOVF	_deviceState, W, B
;	.line	259; src/picUSB.c	return;
	BTFSC	STATUS, 2
;	.line	263; src/picUSB.c	if(UIRbits.ACTVIF && UIEbits.ACTVIE)
	GOTO	_00267_DS_
	BTFSS	_UIRbits, 2
	BRA	_00166_DS_
	BANKSEL	_UIEbits
	BTFSS	_UIEbits, 2, B
	BRA	_00166_DS_
;	.line	265; src/picUSB.c	UCONbits.SUSPND = 0;
	BCF	_UCONbits, 1
; removed redundant BANKSEL
;	.line	266; src/picUSB.c	UIEbits.ACTVIE = 0;
	BCF	_UIEbits, 2, B
;	.line	267; src/picUSB.c	UIRbits.ACTVIF = 0;
	BCF	_UIRbits, 2
_00166_DS_:
;	.line	271; src/picUSB.c	if(UCONbits.SUSPND == 1)
	CLRF	r0x00
	BTFSC	_UCONbits, 1
	INCF	r0x00, F
	MOVF	r0x00, W
	XORLW	0x01
	BNZ	_00169_DS_
;	.line	272; src/picUSB.c	return;
	GOTO	_00267_DS_
_00169_DS_:
;	.line	275; src/picUSB.c	if (UIRbits.URSTIF && UIEbits.URSTIE)
	BTFSS	_UIRbits, 0
	BRA	_00174_DS_
	BANKSEL	_UIEbits
	BTFSS	_UIEbits, 0, B
	BRA	_00174_DS_
;	.line	277; src/picUSB.c	UEIR  = 0x00;
	CLRF	_UEIR
;	.line	278; src/picUSB.c	UIR   = 0x00;
	CLRF	_UIR
;	.line	280; src/picUSB.c	UEIE  = 0x9f; // 0b10011111
	MOVLW	0x9f
	BANKSEL	_UEIE
	MOVWF	_UEIE, B
;	.line	282; src/picUSB.c	UIE   = 0x7F; //0x7b; // 0b01111011
	MOVLW	0x7f
	BANKSEL	_UIE
	MOVWF	_UIE, B
	BANKSEL	_UADDR
;	.line	283; src/picUSB.c	UADDR = 0x00;
	CLRF	_UADDR, B
;	.line	286; src/picUSB.c	UEP0 = EP_CTRL | HSHK_EN;
	MOVLW	0x16
	BANKSEL	_UEP0
	MOVWF	_UEP0, B
_00170_DS_:
;	.line	289; src/picUSB.c	while (UIRbits.TRNIF == 1)
	CLRF	r0x00
	BTFSC	_UIRbits, 3
	INCF	r0x00, F
	MOVF	r0x00, W
	XORLW	0x01
	BNZ	_00172_DS_
;	.line	290; src/picUSB.c	UIRbits.TRNIF = 0;
	BCF	_UIRbits, 3
	BRA	_00170_DS_
_00172_DS_:
;	.line	293; src/picUSB.c	UCONbits.PKTDIS = 0;
	BCF	_UCONbits, 4
;	.line	296; src/picUSB.c	WaitForSetupStage();
	CALL	_WaitForSetupStage
	BANKSEL	_selfPowered
;	.line	299; src/picUSB.c	selfPowered = 0;                      // Self powered is off by default
	CLRF	_selfPowered, B
	BANKSEL	_currentConfiguration
;	.line	300; src/picUSB.c	currentConfiguration = 0;             // Clear active configuration
	CLRF	_currentConfiguration, B
;	.line	301; src/picUSB.c	deviceState = DEFAULT;
	MOVLW	0x03
	BANKSEL	_deviceState
	MOVWF	_deviceState, B
_00174_DS_:
;	.line	305; src/picUSB.c	if (UIRbits.IDLEIF && UIEbits.IDLEIE)
	BTFSS	_UIRbits, 4
	BRA	_00177_DS_
	BANKSEL	_UIEbits
	BTFSS	_UIEbits, 4, B
	BRA	_00177_DS_
; removed redundant BANKSEL
;	.line	307; src/picUSB.c	UIEbits.ACTVIE = 1;
	BSF	_UIEbits, 2, B
;	.line	308; src/picUSB.c	UIRbits.IDLEIF = 0;
	BCF	_UIRbits, 4
;	.line	309; src/picUSB.c	UCONbits.SUSPND = 1;
	BSF	_UCONbits, 1
;	.line	320; src/picUSB.c	PIR2bits.USBIF = 0;
	BCF	_PIR2bits, 4
;	.line	321; src/picUSB.c	INTCONbits.RBIF = 0;
	BCF	_INTCONbits, 0
;	.line	322; src/picUSB.c	PIE2bits.USBIE = 1;
	BSF	_PIE2bits, 4
;	.line	323; src/picUSB.c	INTCONbits.RBIE = 1;
	BSF	_INTCONbits, 3
_00177_DS_:
;	.line	331; src/picUSB.c	if (UIRbits.SOFIF && UIEbits.SOFIE)
	BTFSS	_UIRbits, 6
; #	GOTO	_00180_DS_
; #	BTFSS	_UIEbits, 6, B
; #	GOTO	_00180_DS_
; #	BCF	_UIRbits, 6
; #	BTFSS	_UIRbits, 5
;	.line	333; src/picUSB.c	UIRbits.SOFIF = 0;
	BRA	_00180_DS_
	BANKSEL	_UIEbits
;	.line	337; src/picUSB.c	if (UIRbits.STALLIF && UIEbits.STALLIE)
	BTFSC	_UIEbits, 6, B
	BCF	_UIRbits, 6
_00180_DS_:
	BTFSS	_UIRbits, 5
	BRA	_00185_DS_
	BANKSEL	_UIEbits
	BTFSS	_UIEbits, 5, B
	BRA	_00185_DS_
;	.line	339; src/picUSB.c	if(UEP0bits.EPSTALL == 1)
	CLRF	r0x00
	BANKSEL	_UEP0bits
	BTFSC	_UEP0bits, 0, B
	INCF	r0x00, F
	MOVF	r0x00, W
	XORLW	0x01
	BNZ	_00183_DS_
;	.line	342; src/picUSB.c	WaitForSetupStage();
	CALL	_WaitForSetupStage
	BANKSEL	_UEP0bits
;	.line	343; src/picUSB.c	UEP0bits.EPSTALL = 0;
	BCF	_UEP0bits, 0, B
_00183_DS_:
;	.line	345; src/picUSB.c	UIRbits.STALLIF = 0;
	BCF	_UIRbits, 5
_00185_DS_:
;	.line	349; src/picUSB.c	if (UIRbits.UERRIF && UIEbits.UERRIE)
	BTFSS	_UIRbits, 1
; #	GOTO	_00188_DS_
; #	BTFSS	_UIEbits, 1, B
; #	GOTO	_00188_DS_
; #	BCF	_UIRbits, 1
; #	MOVLW	0x03
;	.line	350; src/picUSB.c	UIRbits.UERRIF = 0;// Clear errors
	BRA	_00188_DS_
	BANKSEL	_UIEbits
;	.line	353; src/picUSB.c	if (deviceState < DEFAULT)  // DETACHED, ATTACHED or POWERED
	BTFSC	_UIEbits, 1, B
	BCF	_UIRbits, 1
_00188_DS_:
	MOVLW	0x03
; #	SUBWF	_deviceState, W, B
; #	BTFSC	STATUS, 0
; #	GOTO	_00191_DS_
; #	GOTO	_00267_DS_
; #	BTFSS	_UIRbits, 3
	BANKSEL	_deviceState
;	.line	354; src/picUSB.c	return;
	SUBWF	_deviceState, W, B
;	.line	365; src/picUSB.c	if(UIRbits.TRNIF && UIEbits.TRNIE)
	BTFSS	STATUS, 0
	GOTO	_00267_DS_
	BTFSS	_UIRbits, 3
	GOTO	_00267_DS_
	BANKSEL	_UIEbits
	BTFSS	_UIEbits, 3, B
	GOTO	_00267_DS_
;	.line	370; src/picUSB.c	byte end_point = USTAT >> 3;
	SWAPF	_USTAT, W
	RLNCF	WREG, W
	ANDLW	0x1f
	MOVWF	r0x00
;	.line	373; src/picUSB.c	if (end_point == 0) // Endpoint 0
	MOVF	r0x00, W
	BTFSS	STATUS, 2
	BRA	_00259_DS_
;	.line	376; src/picUSB.c	if (USTATbits.DIR == OUT)
	BTFSC	_USTATbits, 2
	BRA	_00254_DS_
;	.line	380; src/picUSB.c	PID = (EP_OUT_BD(0).Stat.uc & 0x3C) >> 2;
	MOVLW	0x3c
	BANKSEL	_ep_bdt
	ANDWF	_ep_bdt, W, B
	MOVWF	r0x01
	RRNCF	r0x01, W
	RRNCF	WREG, W
	ANDLW	0x3f
; #	MOVWF	r0x01
; #	MOVF	r0x01, W
;	.line	390; src/picUSB.c	if (PID == 0x0D)
	XORLW	0x0d
	BZ	_00423_DS_
	BRA	_00240_DS_
_00423_DS_:
	BANKSEL	(_ep_bdt + 4)
;	.line	394; src/picUSB.c	EP_IN_BD(0).Stat.uc &= ~BDS_UOWN;
	MOVF	(_ep_bdt + 4), W, B
	MOVWF	r0x01
	BCF	r0x01, 7
	MOVF	r0x01, W
; removed redundant BANKSEL
	MOVWF	(_ep_bdt + 4), B
; removed redundant BANKSEL
;	.line	395; src/picUSB.c	EP_OUT_BD(0).Stat.uc &= ~BDS_UOWN;
	MOVF	_ep_bdt, W, B
	MOVWF	r0x01
	BCF	r0x01, 7
	MOVF	r0x01, W
; removed redundant BANKSEL
	MOVWF	_ep_bdt, B
	BANKSEL	_ctrlTransferStage
;	.line	398; src/picUSB.c	ctrlTransferStage = SETUP_STAGE;
	CLRF	_ctrlTransferStage, B
	BANKSEL	_requestHandled
;	.line	399; src/picUSB.c	requestHandled = 0;                   // Default is that request hasn't been handled
	CLRF	_requestHandled, B
	BANKSEL	_wCount
;	.line	400; src/picUSB.c	wCount = 0;                           // No bytes transferred
	CLRF	_wCount, B
; removed redundant BANKSEL
	CLRF	(_wCount + 1), B
	BANKSEL	_SetupPacket
;	.line	410; src/picUSB.c	if((SetupPacket.bmRequestType & 0x60) != 0x00)
	MOVF	_SetupPacket, W, B
; #	ANDLW	0x60
; #	BTFSC	STATUS, 2
; #	GOTO	_00193_DS_
; #	GOTO	_00267_DS_
; #	MOVF	(_SetupPacket + 1), W, B
;	.line	411; src/picUSB.c	return;
	ANDLW	0x60
;	.line	413; src/picUSB.c	if (SetupPacket.bRequest == SET_ADDRESS)
	BTFSS	STATUS, 2
	BRA	_00267_DS_
; removed redundant BANKSEL
	MOVF	(_SetupPacket + 1), W, B
	XORLW	0x05
	BNZ	_00222_DS_
;	.line	419; src/picUSB.c	requestHandled = 1;
	MOVLW	0x01
	BANKSEL	_requestHandled
	MOVWF	_requestHandled, B
;	.line	420; src/picUSB.c	deviceState = ADDRESS;
	MOVLW	0x04
	BANKSEL	_deviceState
	MOVWF	_deviceState, B
;	.line	421; src/picUSB.c	deviceAddress = SetupPacket.wValue0;
	MOVFF	(_SetupPacket + 2), _deviceAddress
	BRA	_00223_DS_
_00222_DS_:
	BANKSEL	(_SetupPacket + 1)
;	.line	424; src/picUSB.c	else if (SetupPacket.bRequest == GET_DESCRIPTOR)
	MOVF	(_SetupPacket + 1), W, B
	XORLW	0x06
	BZ	_00430_DS_
	BRA	_00219_DS_
_00430_DS_:
	BANKSEL	_SetupPacket
;	.line	426; src/picUSB.c	if(SetupPacket.bmRequestType == 0x80)
	MOVF	_SetupPacket, W, B
	XORLW	0x80
	BZ	_00432_DS_
	BRA	_00223_DS_
_00432_DS_:
	BANKSEL	(_SetupPacket + 3)
;	.line	431; src/picUSB.c	if (SetupPacket.wValue1 == DEVICE_DESCRIPTOR)
	MOVF	(_SetupPacket + 3), W, B
	XORLW	0x01
	BNZ	_00200_DS_
;	.line	433; src/picUSB.c	requestHandled = 1;
	MOVLW	0x01
	BANKSEL	_requestHandled
	MOVWF	_requestHandled, B
;	.line	434; src/picUSB.c	outPtr = (byte *)&device_descriptor;
	MOVLW	UPPER(_device_descriptor)
	BANKSEL	(_outPtr + 2)
	MOVWF	(_outPtr + 2), B
	MOVLW	HIGH(_device_descriptor)
; removed redundant BANKSEL
	MOVWF	(_outPtr + 1), B
	MOVLW	LOW(_device_descriptor)
; removed redundant BANKSEL
	MOVWF	_outPtr, B
;	.line	435; src/picUSB.c	wCount = sizeof(USB_Device_Descriptor);
	MOVLW	0x12
	BANKSEL	_wCount
	MOVWF	_wCount, B
; removed redundant BANKSEL
	CLRF	(_wCount + 1), B
	BRA	_00223_DS_
_00200_DS_:
	BANKSEL	(_SetupPacket + 3)
;	.line	438; src/picUSB.c	else if (SetupPacket.wValue1 == CONFIGURATION_DESCRIPTOR)
	MOVF	(_SetupPacket + 3), W, B
	XORLW	0x02
	BNZ	_00197_DS_
;	.line	440; src/picUSB.c	requestHandled = 1;
	MOVLW	0x01
	BANKSEL	_requestHandled
	MOVWF	_requestHandled, B
;	.line	441; src/picUSB.c	outPtr = (byte *)&configuration_descriptor;
	MOVLW	UPPER(_configuration_descriptor)
	BANKSEL	(_outPtr + 2)
	MOVWF	(_outPtr + 2), B
	MOVLW	HIGH(_configuration_descriptor)
; removed redundant BANKSEL
	MOVWF	(_outPtr + 1), B
	MOVLW	LOW(_configuration_descriptor)
; removed redundant BANKSEL
	MOVWF	_outPtr, B
;	.line	442; src/picUSB.c	wCount = configuration_descriptor.Header.wTotalLength;
	MOVLW	LOW(_configuration_descriptor + 2)
	MOVWF	TBLPTRL
	MOVLW	HIGH(_configuration_descriptor + 2)
	MOVWF	TBLPTRH
	MOVLW	UPPER(_configuration_descriptor + 2)
	MOVWF	TBLPTRU
	TBLRD*+	
	MOVFF	TABLAT, _wCount
	TBLRD*+	
	MOVFF	TABLAT, (_wCount + 1)
	BRA	_00223_DS_
_00197_DS_:
	BANKSEL	(_SetupPacket + 3)
;	.line	445; src/picUSB.c	else if (SetupPacket.wValue1 == STRING_DESCRIPTOR)
	MOVF	(_SetupPacket + 3), W, B
	XORLW	0x03
	BZ	_00438_DS_
	BRA	_00223_DS_
_00438_DS_:
;	.line	447; src/picUSB.c	requestHandled = 1;
	MOVLW	0x01
	BANKSEL	_requestHandled
	MOVWF	_requestHandled, B
; ;multiply lit val:0x03 by variable (_SetupPacket + 2) and store in r0x01
	BANKSEL	(_SetupPacket + 2)
;	.line	449; src/picUSB.c	outPtr = string_descriptor[SetupPacket.wValue0];
	MOVF	(_SetupPacket + 2), W, B
	MULLW	0x03
	MOVF	PRODH, W
	MOVWF	r0x02
	MOVFF	PRODL, r0x01
	CLRF	r0x03
	MOVLW	LOW(_string_descriptor)
	ADDWF	r0x01, F
	MOVLW	HIGH(_string_descriptor)
	ADDWFC	r0x02, F
	MOVLW	UPPER(_string_descriptor)
	ADDWFC	r0x03, F
	MOVFF	r0x01, TBLPTRL
	MOVFF	r0x02, TBLPTRH
	MOVFF	r0x03, TBLPTRU
	TBLRD*+	
	MOVFF	TABLAT, _outPtr
	TBLRD*+	
	MOVFF	TABLAT, (_outPtr + 1)
	TBLRD*+	
	MOVFF	TABLAT, (_outPtr + 2)
;	.line	450; src/picUSB.c	wCount = *outPtr;
	MOVFF	_outPtr, r0x01
	MOVFF	(_outPtr + 1), r0x02
	MOVFF	(_outPtr + 2), r0x03
	MOVFF	r0x01, FSR0L
	MOVFF	r0x02, PRODL
	MOVF	r0x03, W
	CALL	__gptrget1
	MOVWF	r0x01
	MOVFF	r0x01, _wCount
	BANKSEL	(_wCount + 1)
	CLRF	(_wCount + 1), B
	BRA	_00223_DS_
_00219_DS_:
	BANKSEL	(_SetupPacket + 1)
;	.line	466; src/picUSB.c	else if (SetupPacket.bRequest == SET_CONFIGURATION)
	MOVF	(_SetupPacket + 1), W, B
	XORLW	0x09
	BNZ	_00216_DS_
;	.line	468; src/picUSB.c	requestHandled = 1;
	MOVLW	0x01
	BANKSEL	_requestHandled
	MOVWF	_requestHandled, B
;	.line	479; src/picUSB.c	UEP1 = 0b00011110;
	MOVLW	0x1e
	BANKSEL	_UEP1
	MOVWF	_UEP1, B
; #	MOVLW	0x40
; #	MOVWF	(_ep_bdt + 12), B
; #	MOVLW	0x40
;	.line	483; src/picUSB.c	EP_IN_BD(1).Stat.uc  = 0b01000000;
	MOVLW	0x40
	BANKSEL	(_ep_bdt + 12)
;	.line	486; src/picUSB.c	EP_OUT_BD(1).Cnt  = EP1_BUFFER_SIZE;
	MOVWF	(_ep_bdt + 12), B
; removed redundant BANKSEL
	MOVWF	(_ep_bdt + 9), B
;	.line	488; src/picUSB.c	EP_OUT_BD(1).ADDR = (unsigned long)&bootCmd;
	MOVLW	LOW(_bootCmd)
; removed redundant BANKSEL
	MOVWF	(_ep_bdt + 10), B
	MOVLW	HIGH(_bootCmd)
; removed redundant BANKSEL
	MOVWF	(_ep_bdt + 11), B
;	.line	490; src/picUSB.c	EP_OUT_BD(1).Stat.uc = 0b10000000;
	MOVLW	0x80
; removed redundant BANKSEL
	MOVWF	(_ep_bdt + 8), B
;	.line	492; src/picUSB.c	currentConfiguration = SetupPacket.wValue0;
	MOVFF	(_SetupPacket + 2), _currentConfiguration
	BANKSEL	_currentConfiguration
;	.line	495; src/picUSB.c	if (currentConfiguration == 0)
	MOVF	_currentConfiguration, W, B
	BNZ	_00205_DS_
;	.line	498; src/picUSB.c	deviceState = ADDRESS;
	MOVLW	0x04
	BANKSEL	_deviceState
	MOVWF	_deviceState, B
	BRA	_00223_DS_
_00205_DS_:
;	.line	501; src/picUSB.c	deviceState = CONFIGURED;
	MOVLW	0x05
	BANKSEL	_deviceState
	MOVWF	_deviceState, B
	BRA	_00223_DS_
_00216_DS_:
	BANKSEL	(_SetupPacket + 1)
;	.line	508; src/picUSB.c	else if (SetupPacket.bRequest == GET_CONFIGURATION)
	MOVF	(_SetupPacket + 1), W, B
	XORLW	0x08
	BNZ	_00213_DS_
;	.line	510; src/picUSB.c	requestHandled = 1;
	MOVLW	0x01
	BANKSEL	_requestHandled
	MOVWF	_requestHandled, B
;	.line	511; src/picUSB.c	outPtr = (byte*)&currentConfiguration;
	MOVLW	HIGH(_currentConfiguration)
	BANKSEL	(_outPtr + 1)
	MOVWF	(_outPtr + 1), B
	MOVLW	LOW(_currentConfiguration)
; removed redundant BANKSEL
	MOVWF	_outPtr, B
	MOVLW	0x80
; removed redundant BANKSEL
	MOVWF	(_outPtr + 2), B
;	.line	512; src/picUSB.c	wCount = 1;
	MOVLW	0x01
	BANKSEL	_wCount
	MOVWF	_wCount, B
; removed redundant BANKSEL
	CLRF	(_wCount + 1), B
	BRA	_00223_DS_
_00213_DS_:
	BANKSEL	(_SetupPacket + 1)
;	.line	527; src/picUSB.c	else if (SetupPacket.bRequest == GET_INTERFACE)
	MOVF	(_SetupPacket + 1), W, B
	XORLW	0x0a
	BNZ	_00210_DS_
;	.line	531; src/picUSB.c	requestHandled = 1;
	MOVLW	0x01
	BANKSEL	_requestHandled
	MOVWF	_requestHandled, B
	BANKSEL	_controlTransferBuffer
;	.line	532; src/picUSB.c	controlTransferBuffer[0] = 0;
	CLRF	_controlTransferBuffer, B
;	.line	533; src/picUSB.c	outPtr = (byte *)&controlTransferBuffer;
	MOVLW	HIGH(_controlTransferBuffer)
	BANKSEL	(_outPtr + 1)
	MOVWF	(_outPtr + 1), B
	MOVLW	LOW(_controlTransferBuffer)
; removed redundant BANKSEL
	MOVWF	_outPtr, B
	MOVLW	0x80
; removed redundant BANKSEL
	MOVWF	(_outPtr + 2), B
;	.line	534; src/picUSB.c	wCount = 1;
	MOVLW	0x01
	BANKSEL	_wCount
	MOVWF	_wCount, B
; removed redundant BANKSEL
	CLRF	(_wCount + 1), B
	BRA	_00223_DS_
_00210_DS_:
	BANKSEL	(_SetupPacket + 1)
;	.line	537; src/picUSB.c	else if (SetupPacket.bRequest == SET_INTERFACE)
	MOVF	(_SetupPacket + 1), W, B
	XORLW	0x0b
	BNZ	_00223_DS_
;	.line	540; src/picUSB.c	requestHandled = 1;
	MOVLW	0x01
	BANKSEL	_requestHandled
	MOVWF	_requestHandled, B
_00223_DS_:
	BANKSEL	_requestHandled
;	.line	553; src/picUSB.c	if (!requestHandled)
	MOVF	_requestHandled, W, B
	BNZ	_00230_DS_
;	.line	556; src/picUSB.c	EP_OUT_BD(0).Cnt = EP0_BUFFER_SIZE;
	MOVLW	0x40
	BANKSEL	(_ep_bdt + 1)
	MOVWF	(_ep_bdt + 1), B
;	.line	558; src/picUSB.c	EP_OUT_BD(0).ADDR = (unsigned long)&SetupPacket;
	MOVLW	LOW(_SetupPacket)
; removed redundant BANKSEL
	MOVWF	(_ep_bdt + 2), B
	MOVLW	HIGH(_SetupPacket)
; removed redundant BANKSEL
	MOVWF	(_ep_bdt + 3), B
; #	MOVLW	0x84
; #	MOVWF	_ep_bdt, B
; #	MOVLW	0x84
;	.line	559; src/picUSB.c	EP_OUT_BD(0).Stat.uc = BDS_UOWN | BDS_BSTALL;
	MOVLW	0x84
; removed redundant BANKSEL
;	.line	560; src/picUSB.c	EP_IN_BD(0).Stat.uc = BDS_UOWN | BDS_BSTALL;
	MOVWF	_ep_bdt, B
; removed redundant BANKSEL
	MOVWF	(_ep_bdt + 4), B
	BRA	_00231_DS_
_00230_DS_:
	BANKSEL	_SetupPacket
;	.line	563; src/picUSB.c	else if (SetupPacket.bmRequestType & 0x80)
	BTFSS	_SetupPacket, 7, B
	BRA	_00227_DS_
	BANKSEL	(_wCount + 1)
;	.line	566; src/picUSB.c	if(SetupPacket.wLength < wCount)
	MOVF	(_wCount + 1), W, B
	BANKSEL	(_SetupPacket + 7)
	SUBWF	(_SetupPacket + 7), W, B
	BNZ	_00448_DS_
	BANKSEL	_wCount
	MOVF	_wCount, W, B
	BANKSEL	(_SetupPacket + 6)
	SUBWF	(_SetupPacket + 6), W, B
_00448_DS_:
	BC	_00225_DS_
;	.line	567; src/picUSB.c	wCount = SetupPacket.wLength;
	MOVFF	(_SetupPacket + 6), _wCount
	MOVFF	(_SetupPacket + 7), (_wCount + 1)
_00225_DS_:
;	.line	569; src/picUSB.c	InDataStage();
	CALL	_InDataStage
;	.line	570; src/picUSB.c	ctrlTransferStage = DATA_IN_STAGE;
	MOVLW	0x02
	BANKSEL	_ctrlTransferStage
	MOVWF	_ctrlTransferStage, B
;	.line	572; src/picUSB.c	EP_OUT_BD(0).Cnt = EP0_BUFFER_SIZE;
	MOVLW	0x40
	BANKSEL	(_ep_bdt + 1)
	MOVWF	(_ep_bdt + 1), B
;	.line	574; src/picUSB.c	EP_OUT_BD(0).ADDR = (unsigned long)&SetupPacket;
	MOVLW	LOW(_SetupPacket)
; removed redundant BANKSEL
	MOVWF	(_ep_bdt + 2), B
	MOVLW	HIGH(_SetupPacket)
; removed redundant BANKSEL
	MOVWF	(_ep_bdt + 3), B
;	.line	575; src/picUSB.c	EP_OUT_BD(0).Stat.uc = BDS_UOWN;
	MOVLW	0x80
; removed redundant BANKSEL
	MOVWF	_ep_bdt, B
;	.line	579; src/picUSB.c	EP_IN_BD(0).ADDR = (unsigned long)&controlTransferBuffer;
	MOVLW	LOW(_controlTransferBuffer)
; removed redundant BANKSEL
	MOVWF	(_ep_bdt + 6), B
	MOVLW	HIGH(_controlTransferBuffer)
; removed redundant BANKSEL
	MOVWF	(_ep_bdt + 7), B
;	.line	581; src/picUSB.c	EP_IN_BD(0).Stat.uc = BDS_UOWN | BDS_DTS | BDS_DTSEN;
	MOVLW	0xc8
; removed redundant BANKSEL
	MOVWF	(_ep_bdt + 4), B
	BRA	_00231_DS_
_00227_DS_:
;	.line	587; src/picUSB.c	ctrlTransferStage = DATA_OUT_STAGE;
	MOVLW	0x01
	BANKSEL	_ctrlTransferStage
	MOVWF	_ctrlTransferStage, B
	BANKSEL	(_ep_bdt + 5)
;	.line	590; src/picUSB.c	EP_IN_BD(0).Cnt = 0;
	CLRF	(_ep_bdt + 5), B
;	.line	591; src/picUSB.c	EP_IN_BD(0).Stat.uc = BDS_UOWN | BDS_DTS | BDS_DTSEN;
	MOVLW	0xc8
; removed redundant BANKSEL
	MOVWF	(_ep_bdt + 4), B
;	.line	594; src/picUSB.c	EP_OUT_BD(0).Cnt = EP0_BUFFER_SIZE;
	MOVLW	0x40
; removed redundant BANKSEL
	MOVWF	(_ep_bdt + 1), B
;	.line	596; src/picUSB.c	EP_OUT_BD(0).ADDR = (unsigned long)&controlTransferBuffer;
	MOVLW	LOW(_controlTransferBuffer)
; removed redundant BANKSEL
	MOVWF	(_ep_bdt + 2), B
	MOVLW	HIGH(_controlTransferBuffer)
; removed redundant BANKSEL
	MOVWF	(_ep_bdt + 3), B
;	.line	598; src/picUSB.c	EP_OUT_BD(0).Stat.uc = BDS_UOWN | BDS_DTS | BDS_DTSEN;
	MOVLW	0xc8
; removed redundant BANKSEL
	MOVWF	_ep_bdt, B
_00231_DS_:
;	.line	602; src/picUSB.c	UCONbits.PKTDIS = 0;
	BCF	_UCONbits, 4
	BRA	_00260_DS_
_00240_DS_:
	BANKSEL	_ctrlTransferStage
;	.line	605; src/picUSB.c	else if (ctrlTransferStage == DATA_OUT_STAGE)
	MOVF	_ctrlTransferStage, W, B
	XORLW	0x01
	BZ	_00450_DS_
	BRA	_00237_DS_
_00450_DS_:
;	.line	614; src/picUSB.c	bufferSize = ((0x03 & EP_OUT_BD(0).Stat.uc) << 8) | EP_OUT_BD(0).Cnt;
	MOVLW	0x03
	BANKSEL	_ep_bdt
	ANDWF	_ep_bdt, W, B
	CLRF	r0x02
	MOVWF	r0x04
	CLRF	r0x03
	MOVFF	(_ep_bdt + 1), r0x01
	MOVF	r0x03, W
	IORWF	r0x01, F
	MOVF	r0x04, W
	IORWF	r0x02, F
;	.line	617; src/picUSB.c	wCount = wCount + bufferSize;
	MOVF	r0x01, W
	BANKSEL	_wCount
	ADDWF	_wCount, F, B
	MOVF	r0x02, W
; removed redundant BANKSEL
	ADDWFC	(_wCount + 1), F, B
;	.line	619; src/picUSB.c	outPtr = (byte*)&controlTransferBuffer;
	MOVLW	HIGH(_controlTransferBuffer)
	BANKSEL	(_outPtr + 1)
	MOVWF	(_outPtr + 1), B
	MOVLW	LOW(_controlTransferBuffer)
; removed redundant BANKSEL
	MOVWF	_outPtr, B
	MOVLW	0x80
; removed redundant BANKSEL
	MOVWF	(_outPtr + 2), B
;	.line	621; src/picUSB.c	for (i=0;i<bufferSize;i++)
	CLRF	r0x04
_00265_DS_:
	MOVF	r0x02, W
	SUBWF	r0x04, W
	BNZ	_00453_DS_
	MOVF	r0x01, W
	SUBWF	r0x03, W
_00453_DS_:
	BC	_00232_DS_
;	.line	622; src/picUSB.c	*inPtr++ = *outPtr++;
	MOVFF	_inPtr, r0x05
	MOVFF	(_inPtr + 1), r0x06
	MOVFF	(_inPtr + 2), r0x07
	MOVFF	_outPtr, r0x08
	MOVFF	(_outPtr + 1), r0x09
	MOVFF	(_outPtr + 2), r0x0a
	MOVFF	r0x08, FSR0L
	MOVFF	r0x09, PRODL
	MOVF	r0x0a, W
	CALL	__gptrget1
	MOVWF	r0x08
	BANKSEL	_outPtr
	INCF	_outPtr, F, B
	BNC	_00454_DS_
; removed redundant BANKSEL
	INCFSZ	(_outPtr + 1), F, B
	BRA	_10163_DS_
; removed redundant BANKSEL
	INCF	(_outPtr + 2), F, B
_10163_DS_:
_00454_DS_:
	MOVFF	r0x08, POSTDEC1
	MOVFF	r0x05, FSR0L
	MOVFF	r0x06, PRODL
	MOVF	r0x07, W
	CALL	__gptrput1
	BANKSEL	_inPtr
	INCF	_inPtr, F, B
	BNC	_00455_DS_
; removed redundant BANKSEL
	INCFSZ	(_inPtr + 1), F, B
	BRA	_20164_DS_
; removed redundant BANKSEL
	INCF	(_inPtr + 2), F, B
_20164_DS_:
_00455_DS_:
;	.line	621; src/picUSB.c	for (i=0;i<bufferSize;i++)
	INFSNZ	r0x03, F
	INCF	r0x04, F
	BRA	_00265_DS_
_00232_DS_:
	BANKSEL	_ep_bdt
;	.line	625; src/picUSB.c	if(EP_OUT_BD(0).Stat.DTS)
	BTFSS	_ep_bdt, 6, B
	BRA	_00234_DS_
;	.line	626; src/picUSB.c	EP_OUT_BD(0).Stat.uc = BDS_UOWN | BDS_DTSEN;
	MOVLW	0x88
; removed redundant BANKSEL
	MOVWF	_ep_bdt, B
	BRA	_00260_DS_
_00234_DS_:
;	.line	628; src/picUSB.c	EP_OUT_BD(0).Stat.uc = BDS_UOWN | BDS_DTS | BDS_DTSEN;
	MOVLW	0xc8
	BANKSEL	_ep_bdt
	MOVWF	_ep_bdt, B
	BRA	_00260_DS_
_00237_DS_:
;	.line	634; src/picUSB.c	WaitForSetupStage();
	CALL	_WaitForSetupStage
	BRA	_00260_DS_
_00254_DS_:
	BANKSEL	_UADDR
;	.line	641; src/picUSB.c	if ((UADDR == 0) && (deviceState == ADDRESS))
	MOVF	_UADDR, W, B
	BNZ	_00245_DS_
	BANKSEL	_deviceState
	MOVF	_deviceState, W, B
	XORLW	0x04
	BNZ	_00245_DS_
;	.line	645; src/picUSB.c	UADDR = SetupPacket.wValue0;
	MOVFF	(_SetupPacket + 2), _UADDR
	BANKSEL	_UADDR
;	.line	646; src/picUSB.c	if(UADDR == 0)
	MOVF	_UADDR, W, B
	BNZ	_00245_DS_
;	.line	649; src/picUSB.c	deviceState = DEFAULT;
	MOVLW	0x03
	BANKSEL	_deviceState
	MOVWF	_deviceState, B
_00245_DS_:
	BANKSEL	_ctrlTransferStage
;	.line	652; src/picUSB.c	if (ctrlTransferStage == DATA_IN_STAGE)
	MOVF	_ctrlTransferStage, W, B
	XORLW	0x02
	BNZ	_00251_DS_
;	.line	655; src/picUSB.c	InDataStage();
	CALL	_InDataStage
	BANKSEL	(_ep_bdt + 4)
;	.line	658; src/picUSB.c	if(EP_IN_BD(0).Stat.DTS)
	BTFSS	(_ep_bdt + 4), 6, B
	BRA	_00248_DS_
;	.line	659; src/picUSB.c	EP_IN_BD(0).Stat.uc = BDS_UOWN | BDS_DTSEN;
	MOVLW	0x88
; removed redundant BANKSEL
	MOVWF	(_ep_bdt + 4), B
	BRA	_00260_DS_
_00248_DS_:
;	.line	661; src/picUSB.c	EP_IN_BD(0).Stat.uc = BDS_UOWN | BDS_DTS | BDS_DTSEN;
	MOVLW	0xc8
	BANKSEL	(_ep_bdt + 4)
	MOVWF	(_ep_bdt + 4), B
	BRA	_00260_DS_
_00251_DS_:
;	.line	667; src/picUSB.c	WaitForSetupStage();
	CALL	_WaitForSetupStage
	BRA	_00260_DS_
_00259_DS_:
;	.line	676; src/picUSB.c	if (!USTATbits.DIR) // If OUT
	BTFSC	_USTATbits, 2
	BRA	_00260_DS_
;	.line	677; src/picUSB.c	usb_ep_data_out_callback(end_point);
	MOVF	r0x00, W
	MOVWF	POSTDEC1
	CALL	_usb_ep_data_out_callback
	MOVF	POSTINC1, F
_00260_DS_:
;	.line	681; src/picUSB.c	UIRbits.TRNIF = 0;
	BCF	_UIRbits, 3
_00267_DS_:
	MOVFF	PREINC1, r0x0a
	MOVFF	PREINC1, r0x09
	MOVFF	PREINC1, r0x08
	MOVFF	PREINC1, r0x07
	MOVFF	PREINC1, r0x06
	MOVFF	PREINC1, r0x05
	MOVFF	PREINC1, r0x04
	MOVFF	PREINC1, r0x03
	MOVFF	PREINC1, r0x02
	MOVFF	PREINC1, r0x01
	MOVFF	PREINC1, r0x00
	RETURN	

; ; Starting pCode block
S_picUSB__EnableUSBModule	code
_EnableUSBModule:
;	.line	231; src/picUSB.c	if(UCONbits.USBEN == 0)
	BTFSC	_UCONbits, 3
	BRA	_00142_DS_
;	.line	233; src/picUSB.c	UCON = 0;               // USB Control Register
	CLRF	_UCON
	BANKSEL	_UIE
;	.line	234; src/picUSB.c	UIE = 0;                // Disable USB Interrupt Register
	CLRF	_UIE, B
;	.line	235; src/picUSB.c	UCONbits.USBEN = 1;     // Enable USB module
	BSF	_UCONbits, 3
;	.line	236; src/picUSB.c	deviceState = ATTACHED;
	MOVLW	0x01
	BANKSEL	_deviceState
	MOVWF	_deviceState, B
_00142_DS_:
	BANKSEL	_deviceState
;	.line	240; src/picUSB.c	if ((deviceState == ATTACHED) && !UCONbits.SE0)
	MOVF	_deviceState, W, B
	XORLW	0x01
	BNZ	_00146_DS_
	BTFSC	_UCONbits, 5
	BRA	_00146_DS_
;	.line	242; src/picUSB.c	UIR = 0;
	CLRF	_UIR
	BANKSEL	_UIE
;	.line	243; src/picUSB.c	UIE = 0;                // Disable USB Interrupt Register
	CLRF	_UIE, B
	BANKSEL	_UIEbits
;	.line	244; src/picUSB.c	UIEbits.URSTIE = 1;     // Enable USB Reset Interrupt
	BSF	_UIEbits, 0, B
;	.line	245; src/picUSB.c	UIEbits.IDLEIE = 1;     // Enable IDle Detect USB Interrupt
	BSF	_UIEbits, 4, B
;	.line	246; src/picUSB.c	deviceState = POWERED;
	MOVLW	0x02
	BANKSEL	_deviceState
	MOVWF	_deviceState, B
_00146_DS_:
	RETURN	

; ; Starting pCode block
S_picUSB__WaitForSetupStage	code
_WaitForSetupStage:
	BANKSEL	_ctrlTransferStage
;	.line	218; src/picUSB.c	ctrlTransferStage = SETUP_STAGE;
	CLRF	_ctrlTransferStage, B
;	.line	219; src/picUSB.c	EP_OUT_BD(0).Cnt = EP0_BUFFER_SIZE;
	MOVLW	0x40
	BANKSEL	(_ep_bdt + 1)
	MOVWF	(_ep_bdt + 1), B
;	.line	221; src/picUSB.c	EP_OUT_BD(0).ADDR = (unsigned long)&SetupPacket;
	MOVLW	LOW(_SetupPacket)
; removed redundant BANKSEL
	MOVWF	(_ep_bdt + 2), B
	MOVLW	HIGH(_SetupPacket)
; removed redundant BANKSEL
	MOVWF	(_ep_bdt + 3), B
;	.line	223; src/picUSB.c	EP_OUT_BD(0).Stat.uc = BDS_UOWN | BDS_DTSEN;
	MOVLW	0x88
; removed redundant BANKSEL
	MOVWF	_ep_bdt, B
; removed redundant BANKSEL
;	.line	224; src/picUSB.c	EP_IN_BD(0).Stat.uc = 0x00;           // Give control to CPU
	CLRF	(_ep_bdt + 4), B
	RETURN	

; ; Starting pCode block
S_picUSB__InDataStage	code
_InDataStage:
;	.line	180; src/picUSB.c	void InDataStage()
	MOVFF	r0x00, POSTDEC1
	MOVFF	r0x01, POSTDEC1
	MOVFF	r0x02, POSTDEC1
	MOVFF	r0x03, POSTDEC1
	MOVFF	r0x04, POSTDEC1
	MOVFF	r0x05, POSTDEC1
	MOVFF	r0x06, POSTDEC1
	MOVFF	r0x07, POSTDEC1
	MOVFF	r0x08, POSTDEC1
;	.line	186; src/picUSB.c	if(wCount < EP0_BUFFER_SIZE)
	MOVLW	0x00
	BANKSEL	(_wCount + 1)
	SUBWF	(_wCount + 1), W, B
	BNZ	_00124_DS_
	MOVLW	0x40
; removed redundant BANKSEL
	SUBWF	_wCount, W, B
_00124_DS_:
	BC	_00106_DS_
;	.line	187; src/picUSB.c	bufferSize = wCount;
	MOVFF	_wCount, r0x00
	MOVFF	(_wCount + 1), r0x01
	BRA	_00107_DS_
_00106_DS_:
;	.line	189; src/picUSB.c	bufferSize = EP0_BUFFER_SIZE;
	MOVLW	0x40
	MOVWF	r0x00
	CLRF	r0x01
_00107_DS_:
	BANKSEL	(_ep_bdt + 4)
;	.line	193; src/picUSB.c	EP_IN_BD(0).Stat.uc &= ~(BDS_BC8 | BDS_BC9);
	MOVF	(_ep_bdt + 4), W, B
	MOVWF	r0x02
	MOVLW	0xfc
	ANDWF	r0x02, F
	MOVF	r0x02, W
; removed redundant BANKSEL
	MOVWF	(_ep_bdt + 4), B
;	.line	194; src/picUSB.c	EP_IN_BD(0).Stat.uc |= (byte)((bufferSize & 0x0300) >> 8);
	MOVLW	0x03
	ANDWF	r0x01, W
; #	MOVWF	r0x03
; #	MOVF	r0x03, W
	MOVWF	r0x02
; removed redundant BANKSEL
	MOVF	(_ep_bdt + 4), W, B
	IORWF	r0x02, F
	MOVF	r0x02, W
; removed redundant BANKSEL
	MOVWF	(_ep_bdt + 4), B
; #	MOVF	r0x00, W
; #	MOVWF	r0x02
; #	MOVF	r0x02, W
;	.line	195; src/picUSB.c	EP_IN_BD(0).Cnt = (byte)(bufferSize & 0xFF);
	MOVF	r0x00, W
	MOVWF	r0x02
; removed redundant BANKSEL
	MOVWF	(_ep_bdt + 5), B
;	.line	197; src/picUSB.c	EP_IN_BD(0).ADDR = (unsigned long)&controlTransferBuffer;
	MOVLW	LOW(_controlTransferBuffer)
; removed redundant BANKSEL
	MOVWF	(_ep_bdt + 6), B
	MOVLW	HIGH(_controlTransferBuffer)
; removed redundant BANKSEL
	MOVWF	(_ep_bdt + 7), B
;	.line	202; src/picUSB.c	wCount = wCount - bufferSize;
	MOVF	r0x00, W
	BANKSEL	_wCount
	SUBWF	_wCount, F, B
	MOVF	r0x01, W
; removed redundant BANKSEL
	SUBWFB	(_wCount + 1), F, B
;	.line	205; src/picUSB.c	inPtr = (byte *)&controlTransferBuffer;
	MOVLW	HIGH(_controlTransferBuffer)
	BANKSEL	(_inPtr + 1)
	MOVWF	(_inPtr + 1), B
	MOVLW	LOW(_controlTransferBuffer)
; removed redundant BANKSEL
	MOVWF	_inPtr, B
	MOVLW	0x80
; removed redundant BANKSEL
	MOVWF	(_inPtr + 2), B
;	.line	207; src/picUSB.c	for (i=0;i<bufferSize;i++)
	CLRF	r0x02
_00110_DS_:
	MOVFF	r0x02, r0x03
	CLRF	r0x04
	MOVF	r0x01, W
	SUBWF	r0x04, W
	BNZ	_00129_DS_
	MOVF	r0x00, W
	SUBWF	r0x03, W
_00129_DS_:
	BC	_00112_DS_
;	.line	208; src/picUSB.c	*inPtr++ = *outPtr++;
	MOVFF	_inPtr, r0x03
	MOVFF	(_inPtr + 1), r0x04
	MOVFF	(_inPtr + 2), r0x05
	MOVFF	_outPtr, r0x06
	MOVFF	(_outPtr + 1), r0x07
	MOVFF	(_outPtr + 2), r0x08
	MOVFF	r0x06, FSR0L
	MOVFF	r0x07, PRODL
	MOVF	r0x08, W
	CALL	__gptrget1
	MOVWF	r0x06
	BANKSEL	_outPtr
	INCF	_outPtr, F, B
	BNC	_00130_DS_
; removed redundant BANKSEL
	INCFSZ	(_outPtr + 1), F, B
	BRA	_30165_DS_
; removed redundant BANKSEL
	INCF	(_outPtr + 2), F, B
_30165_DS_:
_00130_DS_:
	MOVFF	r0x06, POSTDEC1
	MOVFF	r0x03, FSR0L
	MOVFF	r0x04, PRODL
	MOVF	r0x05, W
	CALL	__gptrput1
	BANKSEL	_inPtr
	INCF	_inPtr, F, B
	BNC	_00131_DS_
; removed redundant BANKSEL
	INCFSZ	(_inPtr + 1), F, B
	BRA	_40166_DS_
; removed redundant BANKSEL
	INCF	(_inPtr + 2), F, B
_40166_DS_:
_00131_DS_:
;	.line	207; src/picUSB.c	for (i=0;i<bufferSize;i++)
	INCF	r0x02, F
	BRA	_00110_DS_
_00112_DS_:
	MOVFF	PREINC1, r0x08
	MOVFF	PREINC1, r0x07
	MOVFF	PREINC1, r0x06
	MOVFF	PREINC1, r0x05
	MOVFF	PREINC1, r0x04
	MOVFF	PREINC1, r0x03
	MOVFF	PREINC1, r0x02
	MOVFF	PREINC1, r0x01
	MOVFF	PREINC1, r0x00
	RETURN	

; ; Starting pCode block for Ival
	code
_device_descriptor:
	DB	0x12, 0x01, 0x00, 0x02, 0xff, 0xff, 0xff, 0x40, 0xd8, 0x04, 0xaa, 0xfe
	DB	0x14, 0x04, 0x01, 0x02, 0x00, 0x01
; ; Starting pCode block for Ival
_configuration_descriptor:
	DB	0x09, 0x02, 0x20, 0x00, 0x01, 0x01, 0x00, 0xc0, 0x7d, 0x09, 0x04, 0x00
	DB	0x00, 0x02, 0xff, 0xff, 0xff, 0x00, 0x07, 0x05, 0x01, 0x02, 0x40, 0x00
	DB	0x00, 0x07, 0x05, 0x81, 0x02, 0x40, 0x00, 0x00
; ; Starting pCode block for Ival
_lang:
	DB	0x04, 0x03, 0x09, 0x04
; ; Starting pCode block for Ival
_manu:
	DB	0x0c, 0x03, 0x52, 0x00, 0x42, 0x00, 0x2b, 0x00, 0x41, 0x00, 0x47, 0x00
; ; Starting pCode block for Ival
_prod:
	DB	0x06, 0x03, 0x50, 0x00, 0x38, 0x00
; ; Starting pCode block for Ival
_string_descriptor:
	DB	LOW(_lang), HIGH(_lang), UPPER(_lang), LOW(_manu), HIGH(_manu), UPPER(_manu), LOW(_prod), HIGH(_prod), UPPER(_prod)


; Statistics:
; code size:	 1580 (0x062c) bytes ( 1.21%)
;           	  790 (0x0316) words
; udata size:	  222 (0x00de) bytes ( 6.31%)
; access size:	   11 (0x000b) bytes


	end
