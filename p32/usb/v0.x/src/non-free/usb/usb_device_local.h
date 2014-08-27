/********************************************************************
  File Information:
    FileName:     	usb_device_local.h
    Dependencies:	See INCLUDES section
    Processor:		PIC18,PIC24, PIC32 and dsPIC33E USB Microcontrollers
    Hardware:		The code is natively intended to be used on the following
    				hardware platforms: PICDEM™ FS USB Demo Board, 
    				PIC18F87J50 FS USB Plug-In Module, or
    				Explorer 16 + compatible USB PIM.  The firmware may be
    				modified for use on other USB platforms by editing the
    				HardwareProfile.h file.
    Complier:  	    Microchip C18 (for PIC18),C30 (for PIC24 and dsPIC33E)
                    and C32 (for PIC32)
    Company:		Microchip Technology, Inc.
    
    Software License Agreement:
    
    The software supplied herewith by Microchip Technology Incorporated
    (the “Company”) for its PIC® Microcontroller is intended and
    supplied to you, the Company’s customer, for use solely and
    exclusively on Microchip PIC Microcontroller products. The
    software is owned by the Company and/or its supplier, and is
    protected under applicable copyright laws. All rights are reserved.
    Any use in violation of the foregoing restrictions may subject the
    user to criminal sanctions under applicable laws, as well as to
    civil liability for the breach of the terms and conditions of this
    license.
    
    THIS SOFTWARE IS PROVIDED IN AN “AS IS” CONDITION. NO WARRANTIES,
    WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
    TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
    PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT,
    IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR
    CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.

  Summary:
    This file contains functions, macros, definitions, variables,
    datatypes, etc. that are required for usage with the MCHPFSUSB device
    stack. This file should be included in projects that use the device stack. 
    
    This file is located in the "\<Install Directory\>\\Microchip\\USB"
    directory.

  Description:
    USB Device Stack File
    
    This file contains functions, macros, definitions, variables,
    datatypes, etc. that are required for usage with the MCHPFSUSB device
    stack. This file should be included in projects that use the device stack.
    
    This file is located in the "\<Install Directory\>\\Microchip\\USB"
    directory.
    
    When including this file in a new project, this file can either be
    referenced from the directory in which it was installed or copied
    directly into the user application folder. If the first method is
    chosen to keep the file located in the folder in which it is installed
    then include paths need to be added so that the library and the
    application both know where to reference each others files. If the
    application folder is located in the same folder as the Microchip
    folder (like the current demo folders), then the following include
    paths need to be added to the application's project:
    
    .
    ..\\..\\MicrochipInclude
    
    If a different directory structure is used, modify the paths as
    required. An example using absolute paths instead of relative paths
    would be the following:
    
    C:\\Microchip Solutions\\Microchip\\Include
    
    C:\\Microchip Solutions\\My Demo Application 

********************************************************************
 File Description:

 Change History:
  Rev    Description
  ----   -----------
  2.8    Initial revision.  Contents extracted from the previous usb_device.c 
         file, so as to make the usb_device.c file less cluttered.
         Fixed BD() and EP() macro computation error, when running in
         USB_PING_PONG__EP0_OUT_ONLY mode.
********************************************************************/
#include "usb_config.h"

/* Short Packet States - Used by Control Transfer Read  - CTRL_TRF_TX */
#define SHORT_PKT_NOT_USED  0
#define SHORT_PKT_PENDING   1
#define SHORT_PKT_SENT      2

/* Control Transfer States */
#define WAIT_SETUP          0
#define CTRL_TRF_TX         1
#define CTRL_TRF_RX         2


typedef union
{
    struct
    {
        unsigned char ping_pong_state :1;
        unsigned char transfer_terminated :1;
    } bits;
    BYTE Val;
} EP_STATUS;

#if (USB_PING_PONG_MODE == USB_PING_PONG__FULL_PING_PONG)
    #define USB_NEXT_EP0_OUT_PING_PONG 0x0008
    #define USB_NEXT_EP0_IN_PING_PONG 0x0008
    #define USB_NEXT_PING_PONG 0x0008
    #define EP0_OUT_EVEN    0
    #define EP0_OUT_ODD     1
    #define EP0_IN_EVEN     2
    #define EP0_IN_ODD      3
    #define EP1_OUT_EVEN    4
    #define EP1_OUT_ODD     5
    #define EP1_IN_EVEN     6
    #define EP1_IN_ODD      7
    #define EP2_OUT_EVEN    8
    #define EP2_OUT_ODD     9
    #define EP2_IN_EVEN     10
    #define EP2_IN_ODD      11
    #define EP3_OUT_EVEN    12
    #define EP3_OUT_ODD     13
    #define EP3_IN_EVEN     14
    #define EP3_IN_ODD      15
    #define EP4_OUT_EVEN    16
    #define EP4_OUT_ODD     17
    #define EP4_IN_EVEN     18
    #define EP4_IN_ODD      19
    #define EP5_OUT_EVEN    20
    #define EP5_OUT_ODD     21
    #define EP5_IN_EVEN     22
    #define EP5_IN_ODD      23
    #define EP6_OUT_EVEN    24
    #define EP6_OUT_ODD     25
    #define EP6_IN_EVEN     26
    #define EP6_IN_ODD      27
    #define EP7_OUT_EVEN    28
    #define EP7_OUT_ODD     29
    #define EP7_IN_EVEN     30
    #define EP7_IN_ODD      31
    #define EP8_OUT_EVEN    32
    #define EP8_OUT_ODD     33
    #define EP8_IN_EVEN     34
    #define EP8_IN_ODD      35
    #define EP9_OUT_EVEN    36
    #define EP9_OUT_ODD     37
    #define EP9_IN_EVEN     38
    #define EP9_IN_ODD      39
    #define EP10_OUT_EVEN   40
    #define EP10_OUT_ODD    41
    #define EP10_IN_EVEN    42
    #define EP10_IN_ODD     43
    #define EP11_OUT_EVEN   44
    #define EP11_OUT_ODD    45
    #define EP11_IN_EVEN    46
    #define EP11_IN_ODD     47
    #define EP12_OUT_EVEN   48
    #define EP12_OUT_ODD    49
    #define EP12_IN_EVEN    50
    #define EP12_IN_ODD     51
    #define EP13_OUT_EVEN   52
    #define EP13_OUT_ODD    53
    #define EP13_IN_EVEN    54
    #define EP13_IN_ODD     55
    #define EP14_OUT_EVEN   56
    #define EP14_OUT_ODD    57
    #define EP14_IN_EVEN    58
    #define EP14_IN_ODD     59
    #define EP15_OUT_EVEN   60
    #define EP15_OUT_ODD    61
    #define EP15_IN_EVEN    62
    #define EP15_IN_ODD     63

    #define EP(ep,dir,pp) (4*ep+2*dir+pp)
    #define BD(ep,dir,pp) (8*(4*ep+2*dir+pp))

#else
    #error "Bad ping pong mode defined."
#endif

/****** Event callback enabling/disabling macros ********************
    This section of code is used to disable specific USB events that may not be
    desired by the user.  This can save code size and increase throughput and
    decrease CPU utiliazation.
********************************************************************/

#define USB_SUSPEND_HANDLER(event,pointer,size)             USER_USB_CALLBACK_EVENT_HANDLER(event,pointer,size)
#define USB_WAKEUP_FROM_SUSPEND_HANDLER(event,pointer,size) USER_USB_CALLBACK_EVENT_HANDLER(event,pointer,size)   
#define USB_SOF_HANDLER(event,pointer,size)                 USER_USB_CALLBACK_EVENT_HANDLER(event,pointer,size)
#define USB_TRANSFER_TERMINATED_HANDLER(event,pointer,size) USER_USB_CALLBACK_EVENT_HANDLER(event,pointer,size)
#define USB_ERROR_HANDLER(event,pointer,size)               USER_USB_CALLBACK_EVENT_HANDLER(event,pointer,size)
#define USB_DISABLE_NONSTANDARD_EP0_REQUEST_HANDLER(event,pointer,size)       USER_USB_CALLBACK_EVENT_HANDLER(event,pointer,size)
#define USB_SET_DESCRIPTOR_HANDLER(event,pointer,size)      USER_USB_CALLBACK_EVENT_HANDLER(event,pointer,size) 
#define USB_SET_CONFIGURATION_HANDLER(event,pointer,size)   USER_USB_CALLBACK_EVENT_HANDLER(event,pointer,size)
#define USB_TRASFER_COMPLETE_HANDLER(event,pointer,size)    USER_USB_CALLBACK_EVENT_HANDLER(event,pointer,size)

