;--------------------------------------------------------
; File Created by SDCC : free open source ANSI-C Compiler
; Version 3.3.1 #8894 (Oct 11 2013) (Linux)
; This file was generated Tue Dec 17 16:05:28 2013
;--------------------------------------------------------
; PIC16 port for the Microchip 16-bit core micros
;--------------------------------------------------------
	list	p=18f26j50
	radix	dec
	CONFIG	OSC=HSPLL
	CONFIG	PLLDIV=5
	CONFIG	CPUDIV=OSC1
	CONFIG	WDTEN=OFF
	CONFIG	STVREN=ON
	CONFIG	XINST=OFF
	CONFIG	CP0=OFF
	CONFIG	IESO=OFF
	CONFIG	FCMEN=OFF
	CONFIG	LPT1OSC=OFF
	CONFIG	T1DIG=ON
	CONFIG	WDTPS=256
	CONFIG	DSWDTPS=8192
	CONFIG	DSWDTEN=OFF
	CONFIG	DSBOREN=OFF
	CONFIG	RTCOSC=INTOSCREF
	CONFIG	DSWDTOSC=INTOSCREF
	CONFIG	MSSP7B_EN=MSK7
	CONFIG	IOL1WAY=OFF
	CONFIG	WPCFG=OFF
	CONFIG	WPEND=PAGE_0
	CONFIG	WPFP=PAGE_1
	CONFIG	WPDIS=OFF


;--------------------------------------------------------
; public variables in this module
;--------------------------------------------------------
	global	_disable_boot
	global	_main
	global	_usb_ep_data_out_callback

;--------------------------------------------------------
; extern variables in this module
;--------------------------------------------------------
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
	extern	_SetupPacket
	extern	_controlTransferBuffer
	extern	_bootCmd
	extern	_outPtr
	extern	_inPtr
	extern	_wCount
	extern	_deviceState
	extern	_selfPowered
	extern	_currentConfiguration
	extern	_ep_bdt
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
	extern	_EnableUSBModule
	extern	_ProcessUSBTransactions
	extern	_device_descriptor
	extern	_configuration_descriptor
	extern	_string_descriptor

;--------------------------------------------------------
;	Equates to used internal registers
;--------------------------------------------------------
STATUS	equ	0xfd8
FSR0L	equ	0xfe9
FSR0H	equ	0xfea
FSR1L	equ	0xfe1
FSR2L	equ	0xfd9
INDF0	equ	0xfef
POSTDEC1	equ	0xfe5
PREINC1	equ	0xfe4
PLUSW2	equ	0xfdb
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

;--------------------------------------------------------
; interrupt vector
;--------------------------------------------------------

;--------------------------------------------------------
; global & static initialisations
;--------------------------------------------------------
; I code from now on!
; ; Starting pCode block
S_main__main	code
_main:
;	.line	137; src/main.c	OSCTUNEbits.PLLEN = 1;      // Enable the PLL
	BSF	_OSCTUNEbits, 6
;	.line	139; src/main.c	while (pll_counter--);      // Wait > 2ms until the PLL locks.
	MOVLW	0x58
	MOVWF	r0x00
	MOVLW	0x02
	MOVWF	r0x01
_00113_DS_:
	MOVFF	r0x00, r0x02
	MOVFF	r0x01, r0x03
	MOVLW	0xff
	ADDWF	r0x00, F
	ADDWFC	r0x01, F
	MOVF	r0x02, W
	IORWF	r0x03, W
	BNZ	_00113_DS_
;	.line	142; src/main.c	ANCON0 = 0xFF;              // AN0 to AN7  are Digital I/O
	MOVLW	0xff
	BANKSEL	_ANCON0
	MOVWF	_ANCON0, B
;	.line	143; src/main.c	ANCON1 = 0x1F;              // AN8 to AN12 are Digital I/O
	MOVLW	0x1f
	BANKSEL	_ANCON1
	MOVWF	_ANCON1, B
;	.line	186; src/main.c	if (RCONbits.NOT_POR == 0)
	BTFSC	_RCONbits, 1
	BRA	_00117_DS_
;	.line	188; src/main.c	RCON |= 0b10010011;     // reset all reset flag
	MOVLW	0x93
	IORWF	_RCON, F
	goto 0x0C00
	
_00117_DS_:
;	.line	197; src/main.c	if (RCONbits.IPEN == 0)
	BTFSC	_RCONbits, 7
	BRA	_00130_DS_
;	.line	203; src/main.c	RCONbits.IPEN   = 1;        // enables priority levels on
	BSF	_RCONbits, 7
;	.line	223; src/main.c	TMR1L = 0;                  // clear Timer 1 counter because
	CLRF	_TMR1L
;	.line	224; src/main.c	TMR1H = 0;                  // counter get an unknown value at reset
	CLRF	_TMR1H
;	.line	225; src/main.c	T1CON = 0b00110001;         // clock source is Fosc/4 (0b00)
	MOVLW	0x31
	MOVWF	_T1CON
	bcf _TRISC, 2 ; led output
	bsf _LATC, 2 ; led on
	
;	.line	254; src/main.c	UCFG = 0b00010100;          // (0x14) full speed mode
	MOVLW	0x14
	BANKSEL	_UCFG
	MOVWF	_UCFG, B
;	.line	259; src/main.c	EP_IN_BD(1).ADDR = (unsigned long)&bootCmd;
	MOVLW	LOW(_bootCmd)
	MOVWF	r0x00
	MOVLW	HIGH(_bootCmd)
	MOVWF	r0x01
	CLRF	r0x02
	CLRF	r0x03
	MOVF	r0x00, W
	BANKSEL	(_ep_bdt + 14)
	MOVWF	(_ep_bdt + 14), B
	MOVF	r0x01, W
; removed redundant BANKSEL
	MOVWF	(_ep_bdt + 15), B
	BANKSEL	_currentConfiguration
;	.line	260; src/main.c	currentConfiguration = 0x00;
	CLRF	_currentConfiguration, B
	BANKSEL	_deviceState
;	.line	261; src/main.c	deviceState = DETACHED;
	CLRF	_deviceState, B
;	.line	266; src/main.c	do
	CLRF	r0x00
	CLRF	r0x01
	CLRF	r0x02
	CLRF	r0x03
_00120_DS_:
;	.line	268; src/main.c	EnableUSBModule();
	CALL	_EnableUSBModule
;	.line	269; src/main.c	ProcessUSBTransactions();
	CALL	_ProcessUSBTransactions
;	.line	271; src/main.c	if (usb_counter == 0xFFFFF)
	MOVF	r0x00, W
	XORLW	0xff
	BNZ	_00159_DS_
	MOVF	r0x01, W
	XORLW	0xff
	BNZ	_00159_DS_
	MOVF	r0x02, W
	XORLW	0x0f
	BNZ	_00159_DS_
	MOVF	r0x03, W
	BZ	_00160_DS_
_00159_DS_:
	BRA	_00119_DS_
_00160_DS_:
;	.line	273; src/main.c	disable_boot();     // disable boot and jump to user app.
	CALL	_disable_boot
_00119_DS_:
;	.line	275; src/main.c	usb_counter++;
	INCF	r0x00, F
	BNC	_00161_DS_
	INCF	r0x01, F
	BNC	_00161_DS_
	INFSNZ	r0x02, F
	INCF	r0x03, F
_00161_DS_:
	BANKSEL	_deviceState
;	.line	277; src/main.c	while (deviceState != CONFIGURED);
	MOVF	_deviceState, W, B
	XORLW	0x05
	BNZ	_00120_DS_
_00126_DS_:
;	.line	284; src/main.c	ProcessUSBTransactions();
	CALL	_ProcessUSBTransactions
;	.line	287; src/main.c	if (PIR1bits.TMR1IF == 1)
	CLRF	r0x00
	BTFSC	_PIR1bits, 0
	INCF	r0x00, F
	MOVF	r0x00, W
	XORLW	0x01
	BNZ	_00126_DS_
;	.line	290; src/main.c	PIR1bits.TMR1IF = 0;
	BCF	_PIR1bits, 0
	movlw 1 << 2 ; toggle
	xorwf _LATC, f ; the led
	
	BRA	_00126_DS_
_00130_DS_:
	RETURN	

; ; Starting pCode block
S_main__usb_ep_data_out_callback	code
_usb_ep_data_out_callback:
;	.line	309; src/main.c	void usb_ep_data_out_callback(char end_point)
	MOVFF	FSR2L, POSTDEC1
	MOVFF	FSR1L, FSR2L
	MOVFF	r0x00, POSTDEC1
	MOVFF	r0x01, POSTDEC1
	MOVFF	r0x02, POSTDEC1
	MOVFF	r0x03, POSTDEC1
	MOVFF	r0x04, POSTDEC1
	MOVFF	r0x05, POSTDEC1
	MOVLW	0x02
	MOVFF	PLUSW2, r0x00
	bsf _LATC, 2 ; led on
	
;	.line	324; src/main.c	EP_IN_BD(end_point).Cnt = 0;
	RLNCF	r0x00, W
	ANDLW	0xfe
	MOVWF	r0x01
	INCF	r0x01, W
; #	MOVWF	r0x00
; #;;multiply lit val:0x04 by variable r0x00 and store in r0x00
; #	MOVF	r0x00, W
	MOVWF	r0x00
	MULLW	0x04
	MOVF	PRODH, W
	BTFSC	r0x00, 7
	SUBLW	0x04
	MOVWF	r0x02
	MOVFF	PRODL, r0x00
	MOVLW	LOW(_ep_bdt)
	ADDWF	r0x00, W
	MOVWF	r0x03
	MOVLW	HIGH(_ep_bdt)
	ADDWFC	r0x02, W
	MOVWF	r0x04
	INFSNZ	r0x03, F
	INCF	r0x04, F
	MOVFF	r0x03, FSR0L
	MOVFF	r0x04, FSR0H
	MOVLW	0x00
	MOVWF	INDF0
;	.line	327; src/main.c	TBLPTRU = bootCmd.addru;
	MOVFF	(_bootCmd + 4), _TBLPTRU
;	.line	328; src/main.c	TBLPTRH = bootCmd.addrh;
	MOVFF	(_bootCmd + 3), _TBLPTRH
;	.line	329; src/main.c	TBLPTRL = bootCmd.addrl;
	MOVFF	(_bootCmd + 2), _TBLPTRL
	BANKSEL	_bootCmd
;	.line	332; src/main.c	if (bootCmd.cmd ==  RESET)
	MOVF	_bootCmd, W, B
	XORLW	0xff
	BNZ	_00185_DS_
;	.line	335; src/main.c	disable_boot();
	CALL	_disable_boot
	BRA	_00186_DS_
_00185_DS_:
	BANKSEL	_bootCmd
;	.line	339; src/main.c	else if (bootCmd.cmd == READ_VERSION)
	MOVF	_bootCmd, W, B
	BNZ	_00182_DS_
;	.line	342; src/main.c	bootCmd.buffer[2] = MINOR_VERSION;
	MOVLW	0x0e
; removed redundant BANKSEL
	MOVWF	(_bootCmd + 2), B
;	.line	343; src/main.c	bootCmd.buffer[3] = MAJOR_VERSION;
	MOVLW	0x04
; removed redundant BANKSEL
	MOVWF	(_bootCmd + 3), B
;	.line	346; src/main.c	EP_IN_BD(end_point).Cnt = 4;
	MOVLW	LOW(_ep_bdt)
	ADDWF	r0x00, W
	MOVWF	r0x03
	MOVLW	HIGH(_ep_bdt)
	ADDWFC	r0x02, W
	MOVWF	r0x04
	INFSNZ	r0x03, F
	INCF	r0x04, F
	MOVFF	r0x03, FSR0L
	MOVFF	r0x04, FSR0H
	MOVLW	0x04
	MOVWF	INDF0
	BRA	_00186_DS_
_00182_DS_:
	BANKSEL	_bootCmd
;	.line	350; src/main.c	else if (bootCmd.cmd == READ_FLASH)
	MOVF	_bootCmd, W, B
	XORLW	0x01
	BNZ	_00179_DS_
;	.line	353; src/main.c	for (counter=0; counter < bootCmd.len; counter++)
	CLRF	r0x03
_00193_DS_:
	BANKSEL	(_bootCmd + 1)
	MOVF	(_bootCmd + 1), W, B
	SUBWF	r0x03, W
	BC	_00170_DS_
	tblrd*+
;	.line	357; src/main.c	bootCmd.xdat[counter] = TABLAT;
	MOVLW	LOW(_bootCmd + 5)
	ADDWF	r0x03, W
	MOVWF	r0x04
	CLRF	r0x05
	MOVLW	HIGH(_bootCmd + 5)
	ADDWFC	r0x05, F
	MOVFF	r0x04, FSR0L
	MOVFF	r0x05, FSR0H
	MOVFF	_TABLAT, INDF0
;	.line	353; src/main.c	for (counter=0; counter < bootCmd.len; counter++)
	INCF	r0x03, F
	BRA	_00193_DS_
_00170_DS_:
;	.line	361; src/main.c	EP_IN_BD(end_point).Cnt = 5 + bootCmd.len;
	MOVLW	LOW(_ep_bdt)
	ADDWF	r0x00, W
	MOVWF	r0x03
	MOVLW	HIGH(_ep_bdt)
	ADDWFC	r0x02, W
	MOVWF	r0x04
	INFSNZ	r0x03, F
	INCF	r0x04, F
	MOVLW	0x05
	BANKSEL	(_bootCmd + 1)
	ADDWF	(_bootCmd + 1), W, B
	MOVWF	r0x05
	MOVFF	r0x03, FSR0L
	MOVFF	r0x04, FSR0H
	MOVFF	r0x05, INDF0
	BRA	_00186_DS_
_00179_DS_:
	BANKSEL	_bootCmd
;	.line	365; src/main.c	else if (bootCmd.cmd == WRITE_FLASH)
	MOVF	_bootCmd, W, B
	XORLW	0x02
	BZ	_00251_DS_
	BRA	_00176_DS_
_00251_DS_:
;	.line	382; src/main.c	EECON1 = 0b10100100;
	MOVLW	0xa4
	MOVWF	_EECON1
;	.line	437; src/main.c	for (counter=0; counter < bootCmd.len; counter+=2)
	CLRF	r0x03
_00196_DS_:
	BANKSEL	(_bootCmd + 1)
	MOVF	(_bootCmd + 1), W, B
	SUBWF	r0x03, W
	BC	_00171_DS_
;	.line	439; src/main.c	TBLPTRL =  bootCmd.addrl + counter; // address of the byte to write
	MOVF	r0x03, W
; removed redundant BANKSEL
	ADDWF	(_bootCmd + 2), W, B
	MOVWF	_TBLPTRL
;	.line	441; src/main.c	TABLAT = bootCmd.xdat[counter];     // load value in the holding registers
	MOVLW	LOW(_bootCmd + 5)
	ADDWF	r0x03, W
	MOVWF	r0x04
	CLRF	r0x05
	MOVLW	HIGH(_bootCmd + 5)
	ADDWFC	r0x05, F
	MOVFF	r0x04, FSR0L
	MOVFF	r0x05, FSR0H
	MOVFF	INDF0, _TABLAT
	tblwt*+
;	.line	445; src/main.c	TABLAT = bootCmd.xdat[counter+1];   // load value in the holding registers
	INCF	r0x03, W
	MOVWF	r0x04
	CLRF	r0x05
	MOVLW	LOW(_bootCmd + 5)
	ADDWF	r0x04, F
	MOVLW	HIGH(_bootCmd + 5)
	ADDWFC	r0x05, F
	MOVFF	r0x04, FSR0L
	MOVFF	r0x05, FSR0H
	MOVFF	INDF0, _TABLAT
	tblwt*
;	.line	452; src/main.c	EECON2 = 0x55;      // unlock sequence
	MOVLW	0x55
	MOVWF	_EECON2
;	.line	453; src/main.c	EECON2 = 0xAA;      // unlock sequence
	MOVLW	0xaa
	MOVWF	_EECON2
;	.line	454; src/main.c	EECON1bits.WR = 1;  // start 2-byte write operation
	BSF	_EECON1bits, 1
;	.line	437; src/main.c	for (counter=0; counter < bootCmd.len; counter+=2)
	INCF	r0x03, F
	INCF	r0x03, F
	BRA	_00196_DS_
_00171_DS_:
;	.line	460; src/main.c	EP_IN_BD(end_point).Cnt = 1;
	MOVLW	LOW(_ep_bdt)
	ADDWF	r0x00, W
	MOVWF	r0x03
	MOVLW	HIGH(_ep_bdt)
	ADDWFC	r0x02, W
	MOVWF	r0x04
	INFSNZ	r0x03, F
	INCF	r0x04, F
	MOVFF	r0x03, FSR0L
	MOVFF	r0x04, FSR0H
	MOVLW	0x01
	MOVWF	INDF0
	BRA	_00186_DS_
_00176_DS_:
	BANKSEL	_bootCmd
;	.line	464; src/main.c	else if (bootCmd.cmd == ERASE_FLASH)
	MOVF	_bootCmd, W, B
	XORLW	0x03
	BNZ	_00186_DS_
;	.line	482; src/main.c	EECON1 = 0b10100100;
	MOVLW	0xa4
	MOVWF	_EECON1
;	.line	485; src/main.c	for (counter=0; counter < bootCmd.len; counter++)
	CLRF	r0x03
_00199_DS_:
	BANKSEL	(_bootCmd + 1)
	MOVF	(_bootCmd + 1), W, B
	SUBWF	r0x03, W
	BC	_00172_DS_
;	.line	493; src/main.c	EECON1bits.FREE = 1;// perform erase operation
	BSF	_EECON1bits, 4
;	.line	494; src/main.c	EECON2 = 0x55;      // unlock sequence
	MOVLW	0x55
	MOVWF	_EECON2
;	.line	495; src/main.c	EECON2 = 0xAA;      // unlock sequence
	MOVLW	0xaa
	MOVWF	_EECON2
;	.line	496; src/main.c	EECON1bits.WR = 1;  // start write or erase operation
	BSF	_EECON1bits, 1
;	.line	497; src/main.c	EECON1bits.FREE = 0;// back to write operation
	BCF	_EECON1bits, 4
	movlw 0x04 ; 0x04 -> W
	addwf _TBLPTRH, 1 ; (W) + (TBLPTRH) -> TBLPTRH
	; (C) is affected
	movlw 0x00 ; 0x00 -> W
	addwfc _TBLPTRU, 1 ; (W) + (TBLPTRU) + (C) -> TBLPTRU
	; Add W and Carry Bit to F
	
;	.line	485; src/main.c	for (counter=0; counter < bootCmd.len; counter++)
	INCF	r0x03, F
	BRA	_00199_DS_
_00172_DS_:
;	.line	554; src/main.c	EP_IN_BD(end_point).Cnt = 1;
	MOVLW	LOW(_ep_bdt)
	ADDWF	r0x00, W
	MOVWF	r0x03
	MOVLW	HIGH(_ep_bdt)
	ADDWFC	r0x02, W
	MOVWF	r0x04
	INFSNZ	r0x03, F
	INCF	r0x04, F
	MOVFF	r0x03, FSR0L
	MOVFF	r0x04, FSR0H
	MOVLW	0x01
	MOVWF	INDF0
_00186_DS_:
;	.line	560; src/main.c	if (EP_IN_BD(end_point).Cnt > 0)
	MOVLW	LOW(_ep_bdt)
	ADDWF	r0x00, F
	MOVLW	HIGH(_ep_bdt)
	ADDWFC	r0x02, F
	INFSNZ	r0x00, F
	INCF	r0x02, F
	MOVFF	r0x00, FSR0L
	MOVFF	r0x02, FSR0H
	MOVFF	INDF0, r0x00
	MOVF	r0x00, W
	BZ	_00191_DS_
	BANKSEL	(_ep_bdt + 12)
;	.line	563; src/main.c	if (EP_IN_BD(1).Stat.DTS)
	BTFSS	(_ep_bdt + 12), 6, B
	BRA	_00188_DS_
;	.line	564; src/main.c	EP_IN_BD(1).Stat.uc = 0b10001000; // UOWN 1 DTS 0 DTSEN 1
	MOVLW	0x88
; removed redundant BANKSEL
	MOVWF	(_ep_bdt + 12), B
	BRA	_00191_DS_
_00188_DS_:
;	.line	566; src/main.c	EP_IN_BD(1).Stat.uc = 0b11001000; // UOWN 1 DTS 1 DTSEN 1
	MOVLW	0xc8
	BANKSEL	(_ep_bdt + 12)
	MOVWF	(_ep_bdt + 12), B
; ;multiply lit val:0x04 by variable r0x01 and store in r0x01
_00191_DS_:
;	.line	570; src/main.c	EP_OUT_BD(end_point).Cnt = EP1_BUFFER_SIZE;
	MOVF	r0x01, W
	MULLW	0x04
	MOVF	PRODH, W
	BTFSC	r0x01, 7
	SUBLW	0x04
	MOVWF	r0x00
	MOVFF	PRODL, r0x01
	MOVLW	LOW(_ep_bdt)
	ADDWF	r0x01, W
	MOVWF	r0x02
	MOVLW	HIGH(_ep_bdt)
	ADDWFC	r0x00, W
	MOVWF	r0x03
	INFSNZ	r0x02, F
	INCF	r0x03, F
	MOVFF	r0x02, FSR0L
	MOVFF	r0x03, FSR0H
	MOVLW	0x40
	MOVWF	INDF0
;	.line	573; src/main.c	EP_OUT_BD(end_point).Stat.uc = 0x80;      // UOWN set to 1
	MOVLW	LOW(_ep_bdt)
	ADDWF	r0x01, F
	MOVLW	HIGH(_ep_bdt)
	ADDWFC	r0x00, F
	MOVFF	r0x01, FSR0L
	MOVFF	r0x00, FSR0H
	MOVLW	0x80
	MOVWF	INDF0
	MOVFF	PREINC1, r0x05
	MOVFF	PREINC1, r0x04
	MOVFF	PREINC1, r0x03
	MOVFF	PREINC1, r0x02
	MOVFF	PREINC1, r0x01
	MOVFF	PREINC1, r0x00
	MOVFF	PREINC1, FSR2L
	RETURN	

; ; Starting pCode block
S_main__disable_boot	code
_disable_boot:
;	.line	32; src/main.c	void disable_boot(void) //__naked
	MOVFF	r0x00, POSTDEC1
	MOVFF	r0x01, POSTDEC1
	MOVFF	r0x02, POSTDEC1
	MOVFF	r0x03, POSTDEC1
;	.line	36; src/main.c	T1CON = 0;                  // disable timer 1
	CLRF	_T1CON
;	.line	45; src/main.c	UCONbits.SUSPND = 0;
	BCF	_UCONbits, 1
;	.line	46; src/main.c	UCONbits.USBEN  = 0;
	BCF	_UCONbits, 3
;	.line	48; src/main.c	while (counter--);          // force timeout on USB
	MOVLW	0xff
	MOVWF	r0x00
	MOVWF	r0x01
_00105_DS_:
	MOVFF	r0x00, r0x02
	MOVFF	r0x01, r0x03
	MOVLW	0xff
	ADDWF	r0x00, F
	ADDWFC	r0x01, F
	MOVF	r0x02, W
	IORWF	r0x03, W
	BNZ	_00105_DS_
;	.line	51; src/main.c	RCONbits.NOT_POR = 0;       // set Power-on Reset
	BCF	_RCONbits, 1
	bsf _TRISC, 2 ; led input
	bcf _LATC, 2 ; led off
	
	reset 
	MOVFF	PREINC1, r0x03
	MOVFF	PREINC1, r0x02
	MOVFF	PREINC1, r0x01
	MOVFF	PREINC1, r0x00
	RETURN	



; Statistics:
; code size:	  880 (0x0370) bytes ( 0.67%)
;           	  440 (0x01b8) words
; udata size:	    0 (0x0000) bytes ( 0.00%)
; access size:	    6 (0x0006) bytes


	end
