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

#ifndef _PICUSB_H
#define _PICUSB_H

#include <pic18fregs.h>
#include "types.h"

/**
	macros
**/

// transform 32-bit to 16-bit address
//#define PTR16(x) ((unsigned int)(((unsigned long)x) & 0xFFFF))

// transform decimal to bcd number
#define BCD(x)   ((( x / 10 ) << 4) | ( x % 10 ))

/**
	definition for the initial ep0
	most are defined here since they are used by the user in the usb_config.h file
    NB: a transaction is limited in size to 8 bytes for low-speed devices
    * TODO
    * change uploader8.py to manage 8-bit transaction
**/

#if SPEED == LOW_SPEED
    #define MAX_PACKET_SIZE         8
#else
    #define MAX_PACKET_SIZE         64
#endif

#define EP0_BUFFER_SIZE				MAX_PACKET_SIZE
#define EP1_BUFFER_SIZE				MAX_PACKET_SIZE

// Control Transfer Stages - see USB spec chapter 5
#define SETUP_STAGE    0                // Start of a control transfer (followed by 0 or more data stages)
#define DATA_OUT_STAGE 1                // Data from host to device
#define DATA_IN_STAGE  2                // Data from device to host
#define STATUS_STAGE   3                // Unused - if data I/O went ok, then back to Setup

// USB direction
#define OUT							0
#define IN							1

// How many different configuration should be available.
// At least configuration 1 must exist.
#define USB_MAX_CONFIGURATION       1

/* UEPn Initialization Parameters */
#define EP_CTRL						0x06 // Cfg Control pipe for this ep
#define EP_OUT						0x0C // Cfg OUT only pipe for this ep
#define EP_IN						0x0A // Cfg IN only pipe for this ep
#define EP_OUT_IN					0x0E // Cfg both OUT & IN pipes for this ep
#define HSHK_EN						0x10 // Enable handshake packet
										 // Handshake should be disable for isoch
//
// Standard Request Codes USB 2.0 Spec Ref Table 9-4
//
#define GET_STATUS					0
#define CLEAR_FEATURE				1
#define SET_FEATURE					3
#define SET_ADDRESS					5
#define GET_DESCRIPTOR				6
#define SET_DESCRIPTOR				7
#define GET_CONFIGURATION			8
#define SET_CONFIGURATION			9
#define GET_INTERFACE				10
#define SET_INTERFACE				11
#define SYNCH_FRAME					12

// Descriptor Types
#define DEVICE_DESCRIPTOR			0x01
#define CONFIGURATION_DESCRIPTOR	0x02
#define STRING_DESCRIPTOR			0x03
#define INTERFACE_DESCRIPTOR		0x04
#define ENDPOINT_DESCRIPTOR			0x05
#define DEVICE_QUALIFIER_DESCRIPTOR	0x06

// Standard Feature Selectors
#define DEVICE_REMOTE_WAKEUP		0x01
#define ENDPOINT_HALT				0x00

// Buffer Descriptor bit masks (from PIC datasheet)
#define BDS_COWN					0x00 // CPU Own Bit
#define BDS_UOWN					0x80 // USB Own Bit
#define BDS_DTS						0x40 // Data Toggle Synchronization Bit
#define BDS_KEN						0x20 // BD Keep Enable Bit
#define BDS_INCDIS					0x10 // Address Increment Disable Bit
#define BDS_DTSEN					0x08 // Data Toggle Synchronization Enable Bit
#define BDS_BSTALL					0x04 // Buffer Stall Enable Bit
#define BDS_BC9						0x02 // Byte count bit 9
#define BDS_BC8						0x01 // Byte count bit 8

// Device states (Chap 9.1.1)
#define DETACHED					0
#define ATTACHED					1
#define POWERED						2
#define DEFAULT						3
#define ADDRESS						4
#define CONFIGURED					5

#define DEVICE_DESCRIPTOR_SIZE		0x12
#define CONFIG_HEADER_SIZE			0x09
#define HID_DESCRIPTOR_SIZE			0x20
#define HID_HEADER_SIZE				0x09

// Buffer Descriptor Status Register
typedef union
{
    unsigned char uc;
    struct{
        unsigned BC8:1;
        unsigned BC9:1;
        unsigned BSTALL:1;              /* Buffer Stall Enable */
        unsigned DTSEN:1;               /* Data Toggle Synch Enable */
        unsigned INCDIS:1;              /* Address Increment Disable */
        unsigned KEN:1;                 /* BD Keep Enable */
        unsigned DTS:1;                 /* Data Toggle Synch Value */
        unsigned UOWN:1;                /* USB Ownership */
    };
    struct{
        unsigned :2;
        unsigned PID0:1;                /* Packet Identifier, bit 0 */
        unsigned PID1:1;                /* Packet Identifier, bit 1 */
        unsigned PID2:1;                /* Packet Identifier, bit 2 */
        unsigned PID3:1;                /* Packet Identifier, bit 3 */
        unsigned :2;
    };
    struct{
        unsigned :2;
        unsigned PID:4;                 /* Packet Identifier */
        unsigned :2;
    };
} BDStat;

// Buffer Descriptor Table
typedef union
{
    struct
    {
        BDStat Stat;                    /* Buffer Descriptor Status Register */
        unsigned char Cnt;              /* Number of bytes to send/sent/(that can be )received */
        __data unsigned long *ADDR;      /* a 12-bit pointer in data RAM memory */
    };
} BufferDescriptorTable;


// USB Device Descriptor
typedef struct
{
    unsigned char bLength;              /* Size of the Descriptor in Bytes (18 bytes = 0x12) */
    unsigned char bDescriptorType;      /* Device Descriptor (0x01) */
    unsigned int  bcdUSB;               /* USB Specification Number which device complies to. */
    unsigned char bDeviceClass;         /* Class Code (Assigned by USB Org).
                                           If equal to Zero, each interface specifies it’s own class code.
                                           If equal to 0xFF, the class code is vendor specified.
                                           Otherwise field is valid Class Code.*/
    unsigned char bDeviceSubClass;      /* Subclass Code (Assigned by USB Org) */
    unsigned char bDeviceProtocol;      /* Protocol Code (Assigned by USB Org) */
    unsigned char bMaxPacketSize0;      /* Maximum Packet Size for Zero Endpoint.
                                           Valid Sizes are 8, 16, 32, 64 */
    unsigned int  idVendor;             /* Vendor ID (Assigned by USB Org).
                                           Microchip Vendor ID is 0x04D8 */
    unsigned int  idProduct;            /* Product ID (Assigned by Manufacturer) */
    unsigned int  bcdDevice;            /* Device Release Number */
    unsigned char iManufacturer;        /* Index of Manufacturer String Descriptor */
    unsigned char iProduct;             /* Index of Product String Descriptor */
    unsigned char iSerialNumber;        /* Index of Serial Number String Descriptor */
    unsigned char bNumConfigurations;   /* Number of Possible Configurations */
} USB_Device_Descriptor;


// added on 11/10/13
// USB Device Qualifier Descriptor
typedef struct
{
    unsigned char bLength;              /* Size of the Descriptor in Bytes (18 bytes = 0x12) */
    unsigned char bDescriptorType;      /* Device Descriptor (0x01) */
    unsigned int  bcdUSB;               /* USB Specification Number which device complies to. */
    unsigned char bDeviceClass;         /* Class Code (Assigned by USB Org).
                                           If equal to Zero, each interface specifies it’s own class code.
                                           If equal to 0xFF, the class code is vendor specified.
                                           Otherwise field is valid Class Code.*/
    unsigned char bDeviceSubClass;      /* Subclass Code (Assigned by USB Org) */
    unsigned char bDeviceProtocol;      /* Protocol Code (Assigned by USB Org) */
    unsigned char bMaxPacketSize0;      /* Maximum Packet Size for Zero Endpoint.
                                           Valid Sizes are 8, 16, 32, 64 */
    unsigned char bNumConfigurations;   /* Number of Possible Configurations */
    unsigned char bReserved;
} USB_Device_Qualifier_Descriptor;


// Configuration Descriptors
typedef struct
{
    unsigned char bLength;              /* Size of Descriptor in Bytes */
    unsigned char bDescriptorType;      /* Configuration Descriptor (0x02) */
    unsigned int  wTotalLength;         /* Total length in bytes of data returned (Configuration Descriptor + Interface Descriptor + n* Endpoint Descriptor */
    unsigned char bNumInterfaces;       /* Number of Interfaces */
    unsigned char bConfigurationValue;  /* Value to use as an argument to select this configuration */
    unsigned char iConfiguration;       /* Index of String Descriptor describing this configuration */
    unsigned char bmAttributes;         /* D7 Reserved, set to 1.(USB 1.0 Bus Powered).
                                           D6 Self Powered.
                                           D5 Remote Wakeup.
                                           D4..0 Reserved, set to 0.
                                           0b10000000*/
    unsigned char bMaxPower;            /* Maximum Power Consumption in 2mA units */
} USB_Configuration_Descriptor_Header;

/**
Every device request starts with an 8 byte setup packet (USB 2.0, chap 9.3)
with a standard layout.  The meaning of wValue and wIndex will
vary depending on the request type and specific request.
See also: http://www.beyondlogic.org/usbnutshell/usb6.htm
TODO: split this Array up to be more precise
TODO: use word instead of LSB/MSB bytes
**/

typedef union
{
  struct {
    byte bmRequestType; /* D7 Data Phase Transfer Direction
                             0 = Host to Device
                             1 = Device to Host
                           D6..5 Type
                             0 = Standard
                             1 = Class
                             2 = Vendor
                             3 = Reserved
                           D4..0 Recipient
                             0 = Device
                             1 = Interface
                             2 = Endpoint
                             3 = Other */
    byte bRequest;      // Specific request
    byte wValue0;       // LSB of wValue
    byte wValue1;       // MSB of wValue
    byte wIndex0;       // LSB of wIndex
    byte wIndex1;       // MSB of wIndex
    word wLength;       // Number of bytes to transfer if there's a data stage
  };
  struct {
    byte extra[EP0_BUFFER_SIZE];     // Fill out to same size as Endpoint 0 max buffer
  };
} setupPacketStruct;


// Interface Descriptors
typedef struct
{
    unsigned char bLength;              /* Size of Descriptor in Bytes (9 Bytes) */
    unsigned char bDescriptorType;      /* Interface Descriptor (0x04) */
    unsigned char bInterfaceNumber;     /* Number of Interface */
    unsigned char bAlternateSetting;    /* Value used to select alternative setting */
    unsigned char bNumEndpoints;        /* Number of Endpoints used for this interface */
    unsigned char bInterfaceClass;      /* Class Code (Assigned by USB Org) e.g. HID, communications, mass storage etc.*/
    unsigned char bInterfaceSubClass;   /* Subclass Code (Assigned by USB Org) */
    unsigned char bInterfaceProtocol;   /* Protocol Code (Assigned by USB Org) */
    unsigned char iInterface;           /* Index of String Descriptor Describing this interface */
} USB_Interface_Descriptor;

// Endpoint Descriptor
typedef struct
{
    unsigned char bLength;              /* Size of Descriptor in Bytes (7 bytes) */
    unsigned char bDescriptorType;      /* Endpoint Descriptor (0x05) */
    unsigned char bEndpointAddress;     /* Endpoint Address<ul><li>Bits 0..3b Endpoint Number.</li><li>Bits 4..6b Reserved. Set to Zero</li><li>Bits 7 Direction 0 = Out, 1 = In (Ignored for Control Endpoints)</li></ul> */
    unsigned char bmAttributes;         /* Bits 0..1 Transfer Type
                                            * 00 = Control
                                            * 01 = Isochronous
                                            * 10 = Bulk
                                            * 11 = Interrupt
                                            Bits 2..7 are reserved.
                                            If Isochronous endpoint,
                                            Bits 3..2 = Synchronisation Type (Iso Mode)
                                            * 00 = No Synchonisation
                                            * 01 = Asynchronous
                                            * 10 = Adaptive
                                            * 11 = Synchronous
                                            Bits 5..4 = Usage Type (Iso Mode)
                                            * 00 = Data Endpoint
                                            * 01 = Feedback Endpoint
                                            * 10 = Explicit Feedback Data Endpoint
                                            * 11 = Reserved */
    unsigned int  wMaxPacketSize;       /* Maximum Packet Size this endpoint is capable of sending or receiving */
    unsigned char bInterval;            /* Interval for polling endpoint data transfers. Value in frame counts. Ignored for Bulk & Control Endpoints. Isochronous must equal 1 and field may range from 1 to 255 for interrupt endpoints. */
} USB_Endpoint_Descriptor;

/**
    Example Composite of Configuration Header and Interface Descriptors for an ACM Header
    See USB CDC 1.1 Page 15
    Define your own Configuration Descriptor here.
**/

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
        unsigned char cmd;
        unsigned char len;
        unsigned char addrl;
        unsigned char addrh;
        unsigned char addru;
        unsigned char xdat[EP1_BUFFER_SIZE-5];
    };
    unsigned char buffer[ EP1_BUFFER_SIZE ];
} allcmd;

extern volatile setupPacketStruct SetupPacket;
extern volatile byte controlTransferBuffer[EP0_BUFFER_SIZE];
extern volatile allcmd bootCmd;

// inPtr/OutPtr are used to move data from user memory (RAM/ROM/EEPROM) buffers
// from/to USB dual port buffers.
extern byte *outPtr;        // Address of buffer to send to host
extern byte *inPtr;         // Address of buffer to receive data from host
extern unsigned int wCount; // Total # of bytes to move

// USB Descriptors
// __code : 21-bit code pointer in FLASH memory

extern __code USB_Device_Descriptor device_descriptor;
extern __code USB_Configuration_Descriptor configuration_descriptor;
//#if (STRING == 1)
extern const char * const string_descriptor[];
//#endif

// Global variables
extern byte deviceState;    // Visible device states (from USB 2.0, chap 9.1.1)
extern byte selfPowered;
extern byte currentConfiguration;

///
/// ep_bdt[4]  changed to ep_bdt[32] par André, le 04-07-2012
/// ep_bdt[32] changed to ep_bdt[4] par Régis, le 07-11-2013
///

#if   defined(__18f14k50) || defined(__18f14k50)  // Bank 2
extern volatile BufferDescriptorTable __at (0x200) ep_bdt[4];
#elif defined(__18f26j53) || defined(__18f46j53) || \
      defined(__18f27j53) || defined(__18f47j53)  // Bank 13
extern volatile BufferDescriptorTable __at (0xD00) ep_bdt[4];
#else                                             // Bank 4
extern volatile BufferDescriptorTable __at (0x400) ep_bdt[4];
#endif

// Out buffer descriptor of endpoint ep
#define EP_OUT_BD(ep) (ep_bdt[(ep << 1)])
// In buffer descriptor of endpoint ep
#define EP_IN_BD(ep)  (ep_bdt[(ep << 1) + 1])

// USB Functions
void EnableUSBModule(void);
void ProcessUSBTransactions(void);

#endif //_PICUSB_H
