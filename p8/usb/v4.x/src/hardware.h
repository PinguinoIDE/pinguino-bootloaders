/*******************************************************************************
	Title:	USB Pinguino Bootloader
	File:	hardware.h
	Descr.: bootloader def. (version, speed, led, tempo.)
	Author:	Régis Blanchot <rblanchot@gmail.com>

	This file is part of Pinguino (http://www.pinguino.cc)
	Released under the LGPL license (http://www.gnu.org/licenses/lgpl.html)
*******************************************************************************/

/*******************************************************************************
	boot blinking led
*******************************************************************************/

#if defined(__18f13k50) || defined(__18f14k50) || \
    defined(__18f2455)  || defined(__18f4455)  || \
    defined(__18f2550)  || defined(__18f4550)  || \
    defined(__18f25k50) || defined(__18f45k50)

	#define LED_PIN							4
	#define LED_PORT						_LATA
	#define LED_TRIS						_TRISA

#endif

#if defined(__18f26j50) || defined(__18f46j50) || \
    defined(__18f26j53) || defined(__18f46j53) || \
    defined(__18f27j53) || defined(__18f47j53)

	#define LED_PIN							2
	#define LED_PORT						_LATC
	#define LED_TRIS						_TRISC

#endif

#define LED_MASK							1 << LED_PIN

/*******************************************************************************
	BOOTLOADER COMMANDS
	General Data Packet Structure:
	.
	__________________           RxBuffer fields
	|    COMMAND     |   0       [CMD]
	|      LEN       |   1       [LEN]
	|     ADDRL      |   2       [        ]  [addrl]
	|     ADDRH      |   3       [ADR.pAdr]: [addrh]
	|     ADDRU      |   4       [        ]  [addru]
	|                |   5       [DATA]
	|                |
	.      DATA      .
	.                .
	|                |   62
	|________________|   63

*******************************************************************************/

#define READ_VERSION     					0x00
#define READ_FLASH       					0x01
#define WRITE_FLASH      					0x02
#define ERASE_FLASH      					0x03
#define READ_EEDATA							0x04 // unused
#define WRITE_EEDATA						0x05 // unused
#define READ_CONFIG							0x06 // unused
#define WRITE_CONFIG						0x07 // unused
#define RESET								0xFF
