/*******************************************************************************
	Title:	USB Pinguino Bootloader
	File:	usb.h
	Descr.: USB HID Bootloader specifications
        Note:   Make sure only one of the below #define is uncommented.
	Author:	Régis Blanchot <rblanchot@gmail.com>

	This file is part of Pinguino (http://www.pinguino.cc)
	Released under the LGPL license (http://www.gnu.org/licenses/lgpl.html)

 * 0.0.0    Microchip USB HID Bootloader adaptation to Pinguino - NOT WORKING -
 * 0.0.1    Fixed APP_RESET_ADDR and procdefs.ld to work with XC32 toolchain
 * 0.0.2    Fixed Config. Bits and $(PROC).ld to work with Pinguino toolchain 
 * 0.0.3    Fixed interrupt issue (Boot & App. must not share the same IVT)
Next :
 * 0.0.4    Goal : Fixed QUERY_DEVICE and GET_DATA issues (work in progress)
*******************************************************************************/

#ifndef _BOOT_H_
#define _BOOT_H_

#include "typedefs.h"

// Specific to Bootloader, can be also defined in the Pinguino Makefile
#ifndef USB_MAJOR_VER
#define USB_MAJOR_VER                       0       // Firmware version, major release number.
#endif
#ifndef USB_MINOR_VER
#define USB_MINOR_VER                       0       // Firmware version, minor release number.
#endif
#ifndef USB_DOT_VER
#define USB_DOT_VER                         3       // Firmware version, dot release number.
#endif

// Specific to USB Pinguino Device
#define USB_VENDOR_ID                       0x04D8  // MICROCHIP
#define USB_PRODUCT_ID                      0x003C  // PINGUINO (TODO : replace with FEAA)

// Describe USB transfer
#define USB_USE_HID

#define USB_SUPPORT_DEVICE

#define USB_POLLING
//#define USB_INTERRUPT

#define USB_PING_PONG__NO_PING_PONG         0x00
#define USB_PING_PONG__EP0_OUT_ONLY         0x01
#define USB_PING_PONG__FULL_PING_PONG       0x02
#define USB_PING_PONG__ALL_BUT_EP0          0x03
#define USB_PING_PONG_MODE                  USB_PING_PONG__FULL_PING_PONG
//#define USB_PING_PONG_MODE                USB_PING_PONG__NO_PING_PONG
//#define USB_PING_PONG_MODE                USB_PING_PONG__EP0_OUT_ONLY
//#define USB_PING_PONG_MODE                USB_PING_PONG__ALL_BUT_EP0

#define USB_PULLUP_ENABLE                   0x00
#define USB_PULLUP_DISABLE                  0x04
#define USB_PULLUP_OPTION                   USB_PULLUP_ENABLE
//#define USB_PULLUP_OPTION                 USB_PULLUP_DISABLED

#define USB_INTERNAL_TRANSCEIVER            0x00
#define USB_EXTERNAL_TRANSCEIVER            0x01
#define USB_TRANSCEIVER_OPTION              USB_INTERNAL_TRANSCEIVER
//#define USB_TRANSCEIVER_OPTION            USB_EXTERNAL_TRANSCEIVER

#define USB_FULL_SPEED                      0x04
#define USB_LOW_SPEED                       111     //???
#define USB_SPEED_OPTION                    USB_FULL_SPEED
//#define USB_SPEED_OPTION                  USB_LOW_SPEED

// Valid Options: 8, 16, 32, or 64 bytes.
#define USB_EP0_BUFF_SIZE                   64

// For tracking Alternate Setting
#define USB_MAX_NUM_INT                     1
#define USB_MAX_EP_NUMBER                   1
#define USB_NUM_STRING_DESCRIPTORS          3

#define USB_ENABLE_ALL_HANDLERS
//#define USB_ENABLE_SUSPEND_HANDLER
//#define USB_ENABLE_WAKEUP_FROM_SUSPEND_HANDLER
//#define USB_ENABLE_SOF_HANDLER
//#define USB_ENABLE_ERROR_HANDLER
//#define USB_ENABLE_OTHER_REQUEST_HANDLER
//#define USB_ENABLE_SET_DESCRIPTOR_HANDLER
//#define USB_ENABLE_INIT_EP_HANDLER
//#define USB_ENABLE_EP0_DATA_HANDLER
//#define USB_ENABLE_TRANSFER_COMPLETE_HANDLER

//Option to enable auto-arming of the status stage of control transfers, if no
//"progress" has been made for the USB_STATUS_STAGE_TIMEOUT value.
//If progress is made (any successful transactions completing on EP0 IN or OUT)
//the timeout counter gets reset to the USB_STATUS_STAGE_TIMEOUT value.
//
//During normal control transfer processing, the USB stack or the application
//firmware will call USBCtrlEPAllowStatusStage() as soon as the firmware is finished
//processing the control transfer.  Therefore, the status stage completes as
//quickly as is physically possible.  The USB_ENABLE_STATUS_STAGE_TIMEOUTS
//feature, and the USB_STATUS_STAGE_TIMEOUT value are only relevant, when:
//1.  The application uses the USBDeferStatusStage() API function, but never calls
//      USBCtrlEPAllowStatusStage().  Or:
//2.  The application uses host to device (OUT) control transfers with data stage,
//      and some abnormal error occurs, where the host might try to abort the control
//      transfer, before it has sent all of the data it claimed it was going to send.
//
//If the application firmware never uses the USBDeferStatusStage() API function,
//and it never uses host to device control transfers with data stage, then
//it is not required to enable the USB_ENABLE_STATUS_STAGE_TIMEOUTS feature.

#define USB_ENABLE_STATUS_STAGE_TIMEOUTS    //Comment this out to disable this feature.

//Section 9.2.6 of the USB 2.0 specifications indicate that:
//1.  Control transfers with no data stage: Status stage must complete within
//      50ms of the start of the control transfer.
//2.  Control transfers with (IN) data stage: Status stage must complete within
//      50ms of sending the last IN data packet in fullfilment of the data stage.
//3.  Control transfers with (OUT) data stage: No specific status stage timing
//      requirement.  However, the total time of the entire control transfer (ex:
//      including the OUT data stage and IN status stage) must not exceed 5 seconds.
//
//Therefore, if the USB_ENABLE_STATUS_STAGE_TIMEOUTS feature is used, it is suggested
//to set the USB_STATUS_STAGE_TIMEOUT value to timeout in less than 50ms.  If the
//USB_ENABLE_STATUS_STAGE_TIMEOUTS feature is not enabled, then the USB_STATUS_STAGE_TIMEOUT
//parameter is not relevant.

//Approximate timeout in milliseconds,
//except when USB_POLLING mode is used, and USBDeviceTasks() is called at < 1kHz
//In this special case, the timeout becomes approximately:
//Timeout(in milliseconds) = ((1000 * (USB_STATUS_STAGE_TIMEOUT - 1)) / (USBDeviceTasks() polling frequency in Hz))
#define USB_STATUS_STAGE_TIMEOUT     (UINT8)45

#if (USB_PING_PONG_MODE != USB_PING_PONG__FULL_PING_PONG)
    #error "PIC32 only supports full ping pong mode. Please edit the boot.h file."
#endif

#endif	// _BOOT_H
