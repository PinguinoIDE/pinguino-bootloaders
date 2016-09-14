// Firmware framework for USB I/O on PIC 18F2455 (and siblings)
// Copyright (C) 2005 Alexander Enzmann
// 2012 - modified for Pinguino by r.blanchot & a.gentric
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//

#ifndef _USB_H
#define _USB_H

#include "compiler.h"
#include "types.h"

// Prototypes (functions called in main.c)
//u16  GetLinearAddress(void *);
void UsbUpdate(void);
void UsbResetEvent(void);
void UsbSuspendEvent(void);
void UsbProcessEvents(void);
void UsbTransEvent(void);
void UsbErrorEvent(void);
//void UsbInitEP0(void);
void UsbSetupStage(void);
//void UsbInitEP1(void);
void UsbInDataStage(void);


// Endpoints
#if SPEED == LOW_SPEED
    #define MAX_PACKET_SIZE         8
#else
    #define MAX_PACKET_SIZE         64
#endif

#define NB_ENDPOINTS                2   // EP0 & EP1
#define EP0_BUFFER_SIZE             MAX_PACKET_SIZE
#define EP1_BUFFER_SIZE             MAX_PACKET_SIZE

// USB RAM / Buffer Descriptor Table

#if   defined(__16f1459)
    // Datasheet 26.4 :
    // The address of BDnSTAT is accessible in linear data space at
    // 2000h + (4n – 1) with n being the buffer descriptor number.
    #define BD_ADDR                 0x2000

#elif defined(__18f13k50) || defined(__18f14k50)

    #define BD_ADDR                 0x200

#elif defined(__18f26j53) || defined(__18f46j53) || \
      defined(__18f27j53) || defined(__18f47j53)

    #define BD_ADDR                 0xD00

#else

    #define BD_ADDR                 0x400

#endif

// Out buffer descriptor of endpoint ep
#define EP_OUT_BD(ep)               ep_bdt[(ep << 1)]
// In buffer descriptor of endpoint ep
#define EP_IN_BD(ep)                ep_bdt[(ep << 1) + 1]

// transform decimal to bcd number
#define BCD(x)                      ((( x / 10 ) << 4) | ( x % 10 ))

// Control Transfer Stages - see USB spec chapter 5
#define SETUP_STAGE                 0   // Start of a control transfer (followed by 0 or more data stages)
#define DATA_OUT_STAGE              1   // Data from host to device
#define DATA_IN_STAGE               2   // Data from device to host
#define STATUS_STAGE                3   // Unused - if data I/O went ok, then back to Setup

// USB direction
#define OUT                         0   // OUT always means from host to device
#define IN                          1   // IN always means from device to host

#define BULK_TRANSFER               0x02

// How many different configuration should be available.
// At least configuration 1 must exist.
#define USB_MAX_CONFIGURATION       1

// UEPn Initialization Parameters 
#define EP_CTRL                     0x06 // Cfg Control pipe for this ep
#define EP_OUT                      0x0C // Cfg OUT only pipe for this ep
#define EP_IN                       0x0A // Cfg IN only pipe for this ep
#define EP_OUT_IN                   0x0E // Cfg both OUT & IN pipes for this ep
#define HSHK_EN                     0x10 // Enable handshake packet
                                         // Handshake should be disable for isoch

// Standard Request Codes (USB 2.0 Spec Ref Table 9-4)
#define GET_STATUS                  0
#define CLEAR_FEATURE               1
#define SET_FEATURE                 3
#define SET_ADDRESS                 5
#define GET_DESCRIPTOR              6
#define SET_DESCRIPTOR              7
#define GET_CONFIGURATION           8
#define SET_CONFIGURATION           9
#define GET_INTERFACE               10
#define SET_INTERFACE               11
#define SYNCH_FRAME                 12

// Descriptor Types
#define DEVICE_DESCRIPTOR           0x01
#define CONFIGURATION_DESCRIPTOR    0x02
#define STRING_DESCRIPTOR           0x03
#define INTERFACE_DESCRIPTOR        0x04
#define ENDPOINT_DESCRIPTOR         0x05
#define DEVICE_QUALIFIER_DESCRIPTOR 0x06

// Standard Feature Selectors
//#define DEVICE_REMOTE_WAKEUP      0x01
//#define ENDPOINT_HALT             0x00

// Buffer Descriptor bit masks (from PIC datasheet)
#define BDS_COWN                    0x00 // CPU Own Bit
#define BDS_UOWN                    0x80 // USB Own Bit
#define BDS_DTS                     0x40 // Data Toggle Synchronization Bit
#define BDS_KEN                     0x20 // BD Keep Enable Bit
#define BDS_INCDIS                  0x10 // Address Increment Disable Bit
#define BDS_DTSEN                   0x08 // Data Toggle Synchronization Enable Bit
#define BDS_BSTALL                  0x04 // Buffer Stall Enable Bit
#define BDS_BC9                     0x02 // Byte count bit 9
#define BDS_BC8                     0x01 // Byte count bit 8

// Device states (Chap 9.1.1)
#define DETACHED                    0
#define ATTACHED                    1
#define POWERED                     2
#define DEFAULT                     3
#define ADDRESS                     4
#define CONFIGURED                  5

#define DEVICE_DESCRIPTOR_SIZE      0x12
#define CONFIG_HEADER_SIZE          0x09
#define HID_DESCRIPTOR_SIZE         0x20
#define HID_HEADER_SIZE             0x09

#define PID_OUT                     0x01
#define PID_IN                      0x09
#define PID_SOF                     0x05
#define PID_SETUP                   0x0D
#define PID_DATA0                   0x03
#define PID_DATA1                   0x0B
#define PID_DATA2                   0x07
#define PID_MDATA                   0x0F
#define PID_ACK                     0x02
#define PID_NAK                     0x0A
#define PID_STALL                   0x0E
#define PID_NYET                    0x06
#define PID_PRE                     0x0C
#define PID_ERR                     0x0C
#define PID_SPLIT                   0x08
#define PID_PING                    0x04
#define PID_RESERVED                0x00

// bmAttributes

#define DEFAULTATTR                 1<<7    // Bit 7 = 1
#define SELFPOWERED                 1<<6    // Bit 6 = 1
#define BUSPOWERED                  0<<6    // Bit 6 = 0
// Enables a suspended USB device to tell the host that the device wants to communicate.
#define REMOTEWAKEUP                1<<5    // Bit 5 = 1

/*
#define STR(x)                      #x  // Unicode version
#define QUOTE(x)                    STR(x)
#define SERIAL                      QUOTE(CHIP)
*/

// String Descriptor
typedef struct
{
    u8  bLength;
    u8  bDescriptorType;
    u16 string[9];
} USB_String_Descriptor;

// Buffer Descriptor Status Register
typedef union
{
    u8 val;
    struct {
        /* When receiving from the SIE. (USB Mode) */
        u8 BC8 : 1;
        u8 BC9 : 1;
        u8 PID : 4; /* See enum PID */
        u8 reserved: 1;
        u8 UOWN : 1;
    };
    struct {
        /* When giving to the SIE (CPU Mode) */
        u8 /*BC8*/ : 1;
        u8 /*BC9*/ : 1;
        u8 BSTALL : 1;
        u8 DTSEN : 1;
        u8 INCDIS : 1;
        u8 KEN : 1;
        u8 DTS : 1;
        u8 /*UOWN*/ : 1;
    };
    /*
    struct{
        unsigned BC8:1;
        unsigned BC9:1;
        unsigned BSTALL:1;              // Buffer Stall Enable 
        unsigned DTSEN:1;               // Data Toggle Synch Enable 
        unsigned INCDIS:1;              // Address Increment Disable 
        unsigned KEN:1;                 // BD Keep Enable 
        unsigned DTS:1;                 // Data Toggle Synch Value 
        unsigned UOWN:1;                // USB Ownership 
    };
    struct{
        unsigned :2;
        unsigned PID0:1;                // Packet Identifier, bit 0 
        unsigned PID1:1;                // Packet Identifier, bit 1 
        unsigned PID2:1;                // Packet Identifier, bit 2 
        unsigned PID3:1;                // Packet Identifier, bit 3 
        unsigned :2;
    };
    struct{
        unsigned :2;
        unsigned PID:4;                 // Packet Identifier 
        unsigned :2;
    };
    */
} BDStat;

// Buffer Descriptor Table
// always occur as a four-byte block in the sequence,
// STAT:CNT:ADRL:ADRH.
typedef union
{
    struct
    {
        BDStat STAT;                    // Buffer Descriptor Status Register
        u8     CNT;                     // Number of bytes to send/sent/(that can be )received
        u16    ADDR;                    // Pointer in data RAM memory
    };
} BufferDescriptorTable;

// USB Device Descriptor
typedef struct
{
    u8 bLength;                         // Size of the Descriptor in Bytes (18 bytes = 0x12) 
    u8 bDescriptorType;                 // Device Descriptor (0x01) 
    u16 bcdUSB;                         // USB Specification Number which device complies to. 
    u8 bDeviceClass;                    // Class Code (Assigned by USB Org).
                                        // If equal to Zero, each interface specifies it’s own class code.
                                        // If equal to 0xFF, the class code is vendor specified.
                                        // Otherwise field is valid Class Code.
    u8 bDeviceSubClass;                 // Subclass Code (Assigned by USB Org) 
    u8 bDeviceProtocol;                 // Protocol Code (Assigned by USB Org) 
    u8 bMaxPacketSize0;                 // Maximum Packet Size for Zero Endpoint.
                                        // Valid Sizes are 8, 16, 32, 64 
    u16 idVendor;                       // Vendor ID (Assigned by USB Org).
                                        // Microchip Vendor ID is 0x04D8 
    u16 idProduct;                      // Product ID (Assigned by Manufacturer) 
    u16 bcdDevice;                      // Device Release Number 
    u8 iManufacturer;                   // Index of Manufacturer String Descriptor 
    u8 iProduct;                        // Index of Product String Descriptor 
    u8 iSerialNumber;                   // Index of Serial Number String Descriptor 
    u8 bNumConfigurations;              // Number of Possible Configurations 
} USB_Device_Descriptor;


// added on 11/10/13
// USB Device Qualifier Descriptor
/*
typedef struct
{
    u8 bLength;                         // Size of the Descriptor in Bytes (18 bytes = 0x12)
    u8 bDescriptorType;                 // Device Descriptor (0x01)
    u16  bcdUSB;                        // USB Specification Number which device complies to.
    u8 bDeviceClass;                    // Class Code (Assigned by USB Org).
                                        // If equal to Zero, each interface specifies it’s own class code.
                                        // If equal to 0xFF, the class code is vendor specified.
                                        // Otherwise field is valid Class Code.
    u8 bDeviceSubClass;                 // Subclass Code (Assigned by USB Org)
    u8 bDeviceProtocol;                 // Protocol Code (Assigned by USB Org)
    u8 bMaxPacketSize0;                 // Maximum Packet Size for Zero Endpoint.
                                        // Valid Sizes are 8, 16, 32, 64
    u8 bNumConfigurations;              // Number of Possible Configurations
    u8 bReserved;
} USB_Device_Qualifier_Descriptor;
*/

// Configuration Descriptors
typedef struct
{
    u8 bLength;                         // Size of Descriptor in Bytes 
    u8 bDescriptorType;                 // Configuration Descriptor (0x02) 
    u16 wTotalLength;                   // Total length in bytes of data returned (Configuration Descriptor + Interface Descriptor + n* Endpoint Descriptor 
    u8 bNumInterfaces;                  // Number of Interfaces 
    u8 bConfigurationValue;             // Value to use as an argument to select this configuration 
    u8 iConfiguration;                  // Index of String Descriptor describing this configuration 
    u8 bmAttributes;                    // D7 Reserved, set to 1.(USB 1.0 Bus Powered).
                                        // D6 Self Powered.
                                        // D5 Remote Wakeup.
                                        // D4..0 Reserved, set to 0.
                                        // 0b10000000
    u8 bMaxPower;                       // Maximum Power Consumption in 2mA units 
} USB_Configuration_Descriptor_Header;

// Device request starts with an 8 byte setup packet
typedef union
{
    struct
    {
        u8 bmRequestType;               // D7 Data Phase Transfer Direction
                                        //   0 = Host to Device
                                        //   1 = Device to Host
                                        // D6..5 Type
                                        //   00 = Standard
                                        //   01 = Class
                                        //   10 = Vendor
                                        //   11 = Reserved
                                        // D4..0 Recipient
                                        //   0 = Device
                                        //   1 = Interface
                                        //   2 = Endpoint
                                        //   3 = Other 
        u8 bRequest;                    // Specific request
        u8 wValue0;                     // LSB of wValue
        u8 wValue1;                     // MSB of wValue
        u8 wIndex0;                     // LSB of wIndex
        u8 wIndex1;                     // MSB of wIndex
        u16 wLength;                    // Number of bytes to transfer if there's a data stage
    };
    struct
    {
        u8 extra[EP0_BUFFER_SIZE];     // Fill out to same size as Endpoint 0 max buffer
    };
} setupPacketStruct;


// Interface Descriptors
typedef struct
{
    u8 bLength;                         // Size of Descriptor in Bytes (9 Bytes) 
    u8 bDescriptorType;                 // Interface Descriptor (0x04) 
    u8 bInterfaceNumber;                // Number of Interface 
    u8 bAlternateSetting;               // Value used to select alternative setting 
    u8 bNumEndpoints;                   // Number of Endpoints used for this interface 
    u8 bInterfaceClass;                 // Class Code (Assigned by USB Org) e.g. HID, communications, mass storage etc.
    u8 bInterfaceSubClass;              // Subclass Code (Assigned by USB Org) 
    u8 bInterfaceProtocol;              // Protocol Code (Assigned by USB Org) 
    u8 iInterface;                      // Index of String Descriptor Describing this interface 
} USB_Interface_Descriptor;

// Endpoint Descriptor
typedef struct
{
    u8 bLength;                         // Size of Descriptor in Bytes (7 bytes) 
    u8 bDescriptorType;                 // Endpoint Descriptor (0x05) 
    u8 bEndpointAddress;                // Endpoint Address<ul><li>Bits 0..3b Endpoint Number.</li><li>Bits 4..6b Reserved. Set to Zero</li><li>Bits 7 Direction 0 = Out, 1 = In (Ignored for Control Endpoints)</li></ul> 
    u8 bmAttributes;                    // Bits 0..1 Transfer Type
                                        //  * 00 = Control
                                        //  * 01 = Isochronous
                                        //  * 10 = Bulk
                                        //  * 11 = Interrupt
                                        // Bits 2..7 are reserved.
                                        //  If Isochronous endpoint,
                                        //  Bits 3..2 = Synchronisation Type (Iso Mode)
                                        //  * 00 = No Synchonisation
                                        //  * 01 = Asynchronous
                                        //  * 10 = Adaptive
                                        //  * 11 = Synchronous
                                        //  Bits 5..4 = Usage Type (Iso Mode)
                                        //  * 00 = Data Endpoint
                                        //  * 01 = Feedback Endpoint
                                        //  * 10 = Explicit Feedback Data Endpoint
                                        //  * 11 = Reserved 
    u16 wMaxPacketSize;                 // Maximum Packet Size this endpoint is capable of sending or receiving 
    u8 bInterval;                       // Interval for polling endpoint data transfers. Value in frame counts. Ignored for Bulk & Control Endpoints. Isochronous must equal 1 and field may range from 1 to 255 for interrupt endpoints. 
} USB_Endpoint_Descriptor;

// Pinguino own Configuration Descriptor
typedef struct
{
    USB_Configuration_Descriptor_Header  Header;
    USB_Interface_Descriptor DataInterface;
    //    USB_Endpoint_Descriptor ep_config;  // unused
    USB_Endpoint_Descriptor ep_out;
    USB_Endpoint_Descriptor ep_in;
} USB_Configuration_Descriptor;

// Packet structure sent by the uploader
typedef union
{
    struct
    {
        u8 cmd;
        u8 len;
        u8 addrl;
        u8 addrh;
        //#if defined(__16F1459)
        //u8 xdat[EP1_BUFFER_SIZE-4];
        //#else
        u8 addru;
        u8 xdat[EP1_BUFFER_SIZE-5];
        //#endif
    };
    u8 buffer[EP1_BUFFER_SIZE];
} allcmd;

#endif //_USB_H
