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
; Main definitions
;-----------------------------------------------------------------------------
; Bootloader Version
#define FW_VER_MAJOR		3
#define FW_VER_MINOR		0
#define FW_VER_SUB_MINOR	0
#define FW_VER_CODE		((FW_VER_MAJOR << 8) | (FW_VER_MINOR << 4) | (FW_VER_SUB_MINOR))
;-----------------------------------------------------------------------------
; Application FW RESET and INTERRUPT VECTORS
#define	APP_RESET_VECTOR       		0x000800
#define	APP_HIGH_INTERRUPT_VECTOR	APP_RESET_VECTOR + 0x08
#define	APP_LOW_INTERRUPT_VECTOR	APP_RESET_VECTOR + 0x18
;-----------------------------------------------------------------------------
; EEPROM Mark to run bootloader
#define USE_EEPROM_MARK		0	; Enable EEPROM MARK. To disable = 0
#define EEPROM_MARK_ADDR	0x00	; EEPROM Address for MARK
#define EEPROM_MARK			0x5A	; MARK value

#define UART_DEBUG	0
