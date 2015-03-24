/*******************************************************************************
	Title:	USB Pinguino Bootloader
	File:	usb.h
	Descr.: USB definitions
	Author:	Régis Blanchot <rblanchot@gmail.com>

	This file is part of Pinguino (http://www.pinguino.cc)
	Released under the LGPL license (http://www.gnu.org/licenses/lgpl.html)
*******************************************************************************/

#ifndef _USB_H_
#define _USB_H_

#include "p32xxxx.h"
#include "typedefs.h"
#include "hardware.h"
#include "boot.h"

/*******************************************************************************
********************************************************************************
********************************************************************************
    This section contains implementation specific information that may vary
    between releases as the implementation needs to change.  This section is
    included for compilation reasons only.
********************************************************************************
********************************************************************************
*******************************************************************************/

#if defined(USB_POLLING)
    #define USB_VOLATILE
#else
    #define USB_VOLATILE volatile
#endif

#define CTRL_TRF_RETURN void
#define CTRL_TRF_PARAMS void

// USB_HANDLE is a pointer to an entry in the BDT.  This pointer can be used
// to read the length of the last transfer, the status of the last transfer,
// and various other information.  Insure to initialize USB_HANDLE objects
// to NULL so that they are in a known state during their first usage.
#define USB_HANDLE void*

#define USB_EP0_ROM                 0x00     //Data comes from RAM
#define USB_EP0_RAM                 0x01     //Data comes from ROM
#define USB_EP0_BUSY                0x80     //The PIPE is busy
#define USB_EP0_INCLUDE_ZERO        0x40     //include a trailing zero packet
#define USB_EP0_NO_DATA             0x00     //no data to send
#define USB_EP0_NO_OPTIONS          0x00     //no options set

/*******************************************************************************
 HID
*******************************************************************************/

/* Class-Specific Requests */
#define GET_REPORT                  0x01
#define GET_IDLE                    0x02
#define GET_PROTOCOL                0x03
#define SET_REPORT                  0x09
#define SET_IDLE                    0x0A
#define SET_PROTOCOL                0x0B

/* Class Descriptor Types */
#define DSC_HID                     0x21
#define DSC_RPT                     0x22
#define DSC_PHY                     0x23

/* Protocol Selection */
#define BOOT_PROTOCOL               0x00
#define RPT_PROTOCOL                0x01

/* HID Interface Class Code */
#define HID_INTF                    0x03

/* HID Interface Class SubClass Codes */
#define BOOT_INTF_SUBCLASS          0x01

/* HID Interface Class Protocol Codes */
#define HID_PROTOCOL_NONE           0x00
#define HID_PROTOCOL_KEYBOARD       0x01
#define HID_PROTOCOL_MOUSE          0x02

/* HID */
#define HID_INTF_ID                 0x00
#define HID_EP                      1
#define HID_INT_EP_SIZE             64
#define HID_INT_OUT_EP_SIZE         HID_INT_EP_SIZE
#define HID_INT_IN_EP_SIZE          HID_INT_EP_SIZE
#define HID_NUM_OF_DSC              1
#define HID_RPT01_SIZE              29 // 63
#define HID_INPUT_REPORT            0x01
#define HID_OUTPUT_REPORT           0x02
#define HID_FEATURE_REPORT          0x03

/********************************************************************
 * Standard Request Codes
 * USB 2.0 Spec Ref Table 9-4
 *******************************************************************/

/* USB Device States as returned by USBGetDeviceState().  Only the defintions
   for these states should be used.  The actual value for each state should
   not be relied upon as constant and may change based on the implementation. */

typedef enum
{
    /* Detached is the state in which the device is not attached to the bus.  When
    in the detached state a device should not have any pull-ups attached to either
    the D+ or D- line.  */
    DETACHED_STATE = 0x00,
    /* Attached is the state in which the device is attached ot the bus but the
    hub/port that it is attached to is not yet configured. */
    ATTACHED_STATE = 0x01,
    /* Powered is the state in which the device is attached to the bus and the
    hub/port that it is attached to is configured. */
    POWERED_STATE = 0x02,
    /* Default state is the state after the device receives a RESET command from
    the host. */
    DEFAULT_STATE = 0x04,
    /* Address pending state is not an official state of the USB defined states.
    This state is internally used to indicate that the device has received a
    SET_ADDRESS command but has not received the STATUS stage of the transfer yet.
    The device is should not switch addresses until after the STATUS stage is
    complete.  */
    ADR_PENDING_STATE = 0x08,
    /* Address is the state in which the device has its own specific address on the
    bus. */
    ADDRESS_STATE = 0x10,
    /* Configured is the state where the device has been fully enumerated and is
    operating on the bus.  The device is now allowed to excute its application
    specific tasks.  It is also allowed to increase its current consumption to the
    value specified in the configuration descriptor of the current configuration.
    */
    CONFIGURED_STATE = 0x20
} USB_DEVICE_STATE;

// *****************************************************************************
/* USB Events

This enumeration identifies USB events that occur.  It is used to
inform USB drivers and applications of events on the bus.  It is passed
as a parameter to the event-handling routine, which must match the
prototype of the USB_CLIENT_EVENT_HANDLER data type, when an event occurs.
*/

typedef enum
{
    // No event occured (NULL event)
    EVENT_NONE = 0,

    EVENT_DEVICE_STACK_BASE = 1,

    EVENT_HOST_STACK_BASE = 100,

    // A USB hub has been attached.  Hub support is not currently available.
    EVENT_HUB_ATTACH,

    // A stall has occured.  This event is not used by the Host stack.
    EVENT_STALL,

    // VBus SRP Pulse, (VBus > 2.0v),  Data: UINT8 Port Number (For future support)
    EVENT_VBUS_SES_REQUEST,

    // The voltage on Vbus has dropped below 4.4V/4.7V.  The application is
    // responsible for monitoring Vbus and calling USBHostVbusEvent() with this
    // event.  This event is not generated by the stack.
    EVENT_VBUS_OVERCURRENT,

    // An enumerating device is requesting power.  The data associated with this
    // event is of the data type USB_VBUS_POWER_EVENT_DATA.  Note that
    // the requested current is specified in 2mA units, identical to the power
    // specification in a device's Configuration Descriptor.
    EVENT_VBUS_REQUEST_POWER,

    // Release power from a detaching device. The data associated with this
    // event is of the data type USB_VBUS_POWER_EVENT_DATA.  The current value
    // specified in the data can be ignored.
    EVENT_VBUS_RELEASE_POWER,

    // The voltage on Vbus is good, and the USB OTG module can be powered on.
    // The application is responsible for monitoring Vbus and calling
    // USBHostVbusEvent() with this event.  This event is not generated by the
    // stack.  If the application issues an EVENT_VBUS_OVERCURRENT, then no
    // power will be applied to that port, and no device can attach to that
    // port, until the application issues the EVENT_VBUS_POWER_AVAILABLE for
    // the port.
    EVENT_VBUS_POWER_AVAILABLE,

    // The attached device is not supported by the application.  The attached
    // device is not allowed to enumerate.
    EVENT_UNSUPPORTED_DEVICE,

    // Cannot enumerate the attached device.  This is generated if communication
    // errors prevent the device from enumerating.
    EVENT_CANNOT_ENUMERATE,

    // The client driver cannot initialize the the attached device.  The
    // attached is not allowed to enumerate.
    EVENT_CLIENT_INIT_ERROR,

    // The Host stack does not have enough heap space to enumerate the device.
    // Check the amount of heap space allocated to the application.  In MPLAB,
    // select Project> Build Options...> Project.  Select the appropriate
    // linker tab, and inspect the "Heap size" entry.
    EVENT_OUT_OF_MEMORY,

    // Unspecified host error. (This error should not occur).
    EVENT_UNSPECIFIED_ERROR,

    // USB cable has been detached.  The data associated with this event is the
    // address of detached device, a single UINT8.
    EVENT_DETACH,

    // A USB transfer has completed.  The data associated with this event is of
    // the data type HOST_TRANSFER_DATA if the event is generated from the host
    // stack.
    EVENT_TRANSFER,

    // A USB Start of Frame token has been received.  This event is not
    // used by the Host stack.
    EVENT_SOF,

    // Device-mode resume received.  This event is not used by the Host stack.
    EVENT_RESUME,

    // Device-mode suspend/idle event received.  This event is not used by the
    // Host stack.
    EVENT_SUSPEND,

    // Device-mode bus reset received.  This event is not used by the Host
    // stack.
    EVENT_RESET,

    // In Host mode, an isochronous data read has completed.  This event will only
    // be passed to the DataEventHandler, which is only utilized if it is defined.
    // Note that the DataEventHandler is called from within the USB interrupt, so
    // it is critical that it return in time for the next isochronous data packet.
    EVENT_DATA_ISOC_READ,

    // In Host mode, an isochronous data write has completed.  This event will only
    // be passed to the DataEventHandler, which is only utilized if it is defined.
    // Note that the DataEventHandler is called from within the USB interrupt, so
    // it is critical that it return in time for the next isochronous data packet.
    EVENT_DATA_ISOC_WRITE,

    // In Host mode, this event gives the application layer the option to reject
    // a client driver that was selected by the stack.  This is needed when multiple
    // devices are supported by class level support, but one configuration and client
    // driver is preferred over another.  Since configuration number is not guaranteed,
    // the stack cannot do this automatically.  This event is issued only when
    // looking through configuration descriptors; the driver selected at the device
    // level cannot be overridden, since there shouldn't be any other options to
    // choose from.
    EVENT_OVERRIDE_CLIENT_DRIVER_SELECTION,

    // In host mode, this event is thrown for every millisecond that passes.  Like all
    // events, this is thrown from the USBHostTasks() or USBTasks() routine so its
    // timeliness will be determined by the rate that these functions are called.  If
    // they are not called very often, then the 1ms events will build up and be
    // dispatched as the USBTasks() or USBHostTasks() functions are called (one event
    // per call to these functions.
    EVENT_1MS,

    // Class-defined event offsets start here:
    EVENT_GENERIC_BASE  = 400,      // Offset for Generic class events

    EVENT_MSD_BASE      = 500,      // Offset for Mass Storage Device class events

    EVENT_HID_BASE      = 600,      // Offset for Human Interface Device class events

    EVENT_PRINTER_BASE  = 700,      // Offset for Printer class events

    EVENT_CDC_BASE      = 800,      // Offset for CDC class events

    EVENT_CHARGER_BASE  = 900,      // Offset for Charger client driver events.

    EVENT_AUDIO_BASE    = 1000,      // Offset for Audio client driver events.

    EVENT_USER_BASE     = 10000,    // Add integral values to this event number
                                    // to create user-defined events.

    // There was a transfer error on the USB.  The data associated with this
    // event is of data type HOST_TRANSFER_DATA.
    EVENT_BUS_ERROR     = 0xFFFF

} USB_EVENT;

/* USB device stack events description here - DWF */

typedef enum
{
    // Notification that a SET_CONFIGURATION() command was received (device)
    EVENT_CONFIGURED = EVENT_DEVICE_STACK_BASE,

    // A SET_DESCRIPTOR request was received (device)
    EVENT_SET_DESCRIPTOR,

    // An endpoint 0 request was received that the stack did not know how to
    // handle.  This is most often a request for one of the class drivers.
    // Please refer to the class driver documenation for information related
    // to what to do if this request is received. (device)
    EVENT_EP0_REQUEST,

    #if 0
    // A USB transfer has completed.  The data associated with this event is of
    // the data type HOST_TRANSFER_DATA if the event is generated from the host
    // stack.
    EVENT_TRANSFER,

    // A USB Start of Frame token has been received.  This event is not
    // used by the Host stack.
    EVENT_SOF,

    // Device-mode resume received.  This event is not used by the Host stack.
    EVENT_RESUME,

    // Device-mode suspend/idle event received.  This event is not used by the
    // Host stack.
    EVENT_SUSPEND,

    // Device-mode bus reset received.  This event is not used by the Host
    // stack.
    EVENT_RESET,

    // Device Mode: A setup packet received (data: SETUP_PKT).  This event is
    // not used by the Host stack.
    EVENT_SETUP,
    #endif

    // Device-mode USB cable has been attached.  This event is not used by the
    // Host stack.  The client driver may provide an application event when a
    // device attaches.
    EVENT_ATTACH,

    // A user transfer was terminated by the stack.  This event will pass back
    // the value of the handle that was terminated.  Compare this value against
    // the current valid handles to determine which transfer was terminated.
    EVENT_TRANSFER_TERMINATED

} USB_DEVICE_STACK_EVENTS;

#define USB_NEXT_PING_PONG                      0x0008
#define USB_NEXT_EP0_OUT_PING_PONG              (USB_NEXT_PING_PONG)
#define USB_NEXT_EP0_IN_PING_PONG               (USB_NEXT_PING_PONG)
#define EP0_OUT_EVEN                            0
#define EP0_OUT_ODD                             1
#define EP0_IN_EVEN                             2
#define EP0_IN_ODD                              3
#define EP(ep,dir,pp)                           (4*ep+2*dir+pp)
#define BD(ep,dir,pp)                           (8*(4*ep+2*dir+pp))

/* Short Packet States - Used by Control Transfer Read  - CTRL_TRF_TX */
#define SHORT_PKT_NOT_USED                      0
#define SHORT_PKT_PENDING                       1
#define SHORT_PKT_SENT                          2

/* Control Transfer States */
#define WAIT_SETUP                              0
#define CTRL_TRF_TX                             1
#define CTRL_TRF_RX                             2

// ******************************************************************
// Section: USB Specification Constants
// ******************************************************************

// Section: Valid PID Values

#define PID_SETUP                               0xD     // PID for a SETUP token

// Section: Standard Device Requests

#define USB_REQUEST_GET_STATUS                  0       // Standard Device Request - GET STATUS
#define USB_REQUEST_CLEAR_FEATURE               1       // Standard Device Request - CLEAR FEATURE
#define USB_REQUEST_SET_FEATURE                 3       // Standard Device Request - SET FEATURE
#define USB_REQUEST_SET_ADDRESS                 5       // Standard Device Request - SET ADDRESS
#define USB_REQUEST_GET_DESCRIPTOR              6       // Standard Device Request - GET DESCRIPTOR
#define USB_REQUEST_SET_DESCRIPTOR              7       // Standard Device Request - SET DESCRIPTOR
#define USB_REQUEST_GET_CONFIGURATION           8       // Standard Device Request - GET CONFIGURATION
#define USB_REQUEST_SET_CONFIGURATION           9       // Standard Device Request - SET CONFIGURATION
#define USB_REQUEST_GET_INTERFACE               10      // Standard Device Request - GET INTERFACE
#define USB_REQUEST_SET_INTERFACE               11      // Standard Device Request - SET INTERFACE
#define USB_REQUEST_SYNCH_FRAME                 12      // Standard Device Request - SYNCH FRAME

#define USB_FEATURE_ENDPOINT_HALT               0       // CLEAR/SET FEATURE - Endpoint Halt
#define USB_FEATURE_DEVICE_REMOTE_WAKEUP        1       // CLEAR/SET FEATURE - Device remote wake-up
#define USB_FEATURE_TEST_MODE                   2       // CLEAR/SET FEATURE - Test mode

// Section: Setup Data Constants

#define USB_SETUP_HOST_TO_DEVICE                0x00    // Device Request bmRequestType transfer direction - host to device transfer
#define USB_SETUP_DEVICE_TO_HOST                0x80    // Device Request bmRequestType transfer direction - device to host transfer
#define USB_SETUP_TYPE_STANDARD                 0x00    // Device Request bmRequestType type - standard
#define USB_SETUP_TYPE_CLASS                    0x20    // Device Request bmRequestType type - class
#define USB_SETUP_TYPE_VENDOR                   0x40    // Device Request bmRequestType type - vendor
#define USB_SETUP_RECIPIENT_DEVICE              0x00    // Device Request bmRequestType recipient - device
#define USB_SETUP_RECIPIENT_INTERFACE           0x01    // Device Request bmRequestType recipient - interface
#define USB_SETUP_RECIPIENT_ENDPOINT            0x02    // Device Request bmRequestType recipient - endpoint
#define USB_SETUP_RECIPIENT_OTHER               0x03    // Device Request bmRequestType recipient - other

#define USB_SETUP_HOST_TO_DEVICE_BITFIELD       (USB_SETUP_HOST_TO_DEVICE>>7)       // Device Request bmRequestType transfer direction - host to device transfer - bit definition
#define USB_SETUP_DEVICE_TO_HOST_BITFIELD       (USB_SETUP_DEVICE_TO_HOST>>7)       // Device Request bmRequestType transfer direction - device to host transfer - bit definition
#define USB_SETUP_TYPE_STANDARD_BITFIELD        (USB_SETUP_TYPE_STANDARD>>5)        // Device Request bmRequestType type - standard
#define USB_SETUP_TYPE_CLASS_BITFIELD           (USB_SETUP_TYPE_CLASS>>5)           // Device Request bmRequestType type - class
#define USB_SETUP_TYPE_VENDOR_BITFIELD          (USB_SETUP_TYPE_VENDOR>>5)          // Device Request bmRequestType type - vendor
#define USB_SETUP_RECIPIENT_DEVICE_BITFIELD     (USB_SETUP_RECIPIENT_DEVICE)        // Device Request bmRequestType recipient - device
#define USB_SETUP_RECIPIENT_INTERFACE_BITFIELD  (USB_SETUP_RECIPIENT_INTERFACE)     // Device Request bmRequestType recipient - interface
#define USB_SETUP_RECIPIENT_ENDPOINT_BITFIELD   (USB_SETUP_RECIPIENT_ENDPOINT)      // Device Request bmRequestType recipient - endpoint
#define USB_SETUP_RECIPIENT_OTHER_BITFIELD      (USB_SETUP_RECIPIENT_OTHER)         // Device Request bmRequestType recipient - other

/********************************************************************
USB Endpoint Definitions
USB Standard EP Address Format: DIR:X:X:X:EP3:EP2:EP1:EP0
This is used in the descriptors.
********************************************************************/

#define _EP_IN                                  0x80
#define _EP_OUT                                 0x00

/* Configuration Attributes */
#define _DEFAULT                                (0x01<<7) //Default Value (Bit 7 is set)
#define _SELF                                   (0x01<<6) //Self-powered (Supports if set)
#define _RWU                                    (0x01<<5) //Remote Wakeup (Supports if set)
#define _HNP                                    (0x01<<1) //HNP (Supports if set)
#define _SRP                                    (0x01)	  //SRP (Supports if set)

/* Endpoint Transfer Type */
#define _CTRL                                   0x00      //Control Transfer
#define _ISO                                    0x01      //Isochronous Transfer
#define _BULK                                   0x02      //Bulk Transfer
#define _INTERRUPT                              0x03      //Interrupt Transfer

//These are the directional indicators used for USBTransferOnePacket()
// and other functions that compute addresses of, and manipulate, BDT entries.
#define OUT_FROM_HOST                           0
#define IN_TO_HOST                              1

// *****************************************************************************
// Section: USB Descriptors
// *****************************************************************************

#define USB_DESCRIPTOR_DEVICE                   0x01    // bDescriptorType for a Device Descriptor.
#define USB_DESCRIPTOR_CONFIGURATION            0x02    // bDescriptorType for a Configuration Descriptor.
#define USB_DESCRIPTOR_STRING                   0x03    // bDescriptorType for a String Descriptor.
#define USB_DESCRIPTOR_INTERFACE                0x04    // bDescriptorType for an Interface Descriptor.
#define USB_DESCRIPTOR_ENDPOINT                 0x05    // bDescriptorType for an Endpoint Descriptor.
#define USB_DESCRIPTOR_DEVICE_QUALIFIER         0x06    // bDescriptorType for a Device Qualifier.
#define USB_DESCRIPTOR_OTHER_SPEED              0x07    // bDescriptorType for a Other Speed Configuration.
#define USB_DESCRIPTOR_INTERFACE_POWER          0x08    // bDescriptorType for Interface Power.
#define USB_DESCRIPTOR_OTG                      0x09    // bDescriptorType for an OTG Descriptor.

/*******************************************************************************
 USB Data Types
*******************************************************************************/

/* Buffer Descriptor Status Register Initialization Parameters */

//The _BSTALL definition is changed from 0x04 to 0x00 to
// fix a difference in the PIC18 and PIC24 definitions of this
// bit.  This should be changed back once the definitions are
// synced.
#define _BSTALL                 0x04        //Buffer Stall enable
#define _DTSEN                  0x08        //Data Toggle Synch enable
#define _DAT0                   0x00        //DATA0 packet expected next
#define _DAT1                   0x40        //DATA1 packet expected next
#define _DTSMASK                0x40        //DTS Mask
#define _USIE                   0x80        //SIE owns buffer
#define _UCPU                   0x00        //CPU owns buffer

#define _STAT_MASK              0xFC

#define USTAT_EP0_PP_MASK       ~0x04
#define USTAT_EP_MASK           0xFC
#define USTAT_EP0_OUT           0x00
#define USTAT_EP0_OUT_EVEN      0x00
#define USTAT_EP0_OUT_ODD       0x04

#define USTAT_EP0_IN            0x08
#define USTAT_EP0_IN_EVEN       0x08
#define USTAT_EP0_IN_ODD        0x0C
#define ENDPOINT_MASK           0xF0

//**********************************************************************

#if defined(USB_DISABLE_SOF_HANDLER)
    #define USB_SOF_INTERRUPT 0x00
#else
    #define USB_SOF_INTERRUPT   0x04
#endif

#if defined(USB_DISABLE_ERROR_HANDLER)
    #define USB_ERROR_INTERRUPT 0x02
#else
    #define USB_ERROR_INTERRUPT 0x02
#endif

#define EP_CTRL                 0x0C            // Cfg Control pipe for this ep
#define EP_OUT                  0x18            // Cfg OUT only pipe for this ep
#define EP_IN                   0x14            // Cfg IN only pipe for this ep
#define EP_OUT_IN               0x1C            // Cfg both OUT & IN pipes for this ep
#define HSHK_EN                 0x01            // Enable handshake packet
                                                // Handshake should be disable for isoch
#define USB_HANDSHAKE_ENABLED   0x01
#define USB_HANDSHAKE_DISABLED  0x00

#define USB_OUT_ENABLED         0x08
#define USB_OUT_DISABLED        0x00

#define USB_IN_ENABLED          0x04
#define USB_IN_DISABLED         0x00

#define USB_ALLOW_SETUP         0x00
#define USB_DISALLOW_SETUP      0x10

#define USB_STALL_ENDPOINT      0x02

/*******************************************************************************
 Typedef
*******************************************************************************/

typedef union
{
    struct
    {
        unsigned ping_pong_state :1;
        unsigned transfer_terminated :1;
    } bits;
    UINT8 Val;
} EP_STATUS;

// Defintion of the PIPE structure
// This structure is used to keep track of data that is sent out of
// the stack automatically.

typedef struct __attribute__ ((packed))
{
    union __attribute__ ((packed))
    {
        //Various options of pointers that are available to
        // get the data from
        UINT8 *bRam;
        const UINT8 *bRom;
        UINT16 *wRam;
        const UINT16 *wRom;
    }pSrc;

    union __attribute__ ((packed))
    {
        struct __attribute__ ((packed))
        {
            //is this transfer from RAM or ROM?
            unsigned ctrl_trf_mem          :1;
            unsigned reserved              :5;
            //include a zero length packet after
            //data is done if data_size%ep_size = 0?
            unsigned includeZero           :1;
            //is this PIPE currently in use
            unsigned busy                  :1;
        }bits;
        UINT8 Val;
    }info;

    WORD_VAL __attribute__((aligned)) wCount;

}IN_PIPE;

typedef struct __attribute__ ((packed))
{
    union __attribute__ ((packed))
    {
        //Various options of pointers that are available to get the data from
        UINT8 *bRam;
        UINT16 *wRam;
    }pDst;

    union __attribute__ ((packed))
    {
        struct __attribute__ ((packed))
        {
            unsigned reserved              :7;
            //is this PIPE currently in use
            unsigned busy                  :1;
        }bits;
        UINT8 Val;
    }info;

    WORD_VAL wCount;
    CTRL_TRF_RETURN (*pFunc)(CTRL_TRF_PARAMS);

}OUT_PIPE;

// *****************************************************************************
/* USB Device Descriptor Structure

This struct defines the structure of a USB Device Descriptor.  Note that this
structure may need to be packed, or even accessed as bytes, to properly access
the correct fields when used on some device architectures.
*/
typedef struct __attribute__ ((packed)) _USB_DEVICE_DESCRIPTOR
{
    UINT8  bLength;               // Length of this descriptor.
    UINT8  bDescriptorType;       // DEVICE descriptor type (USB_DESCRIPTOR_DEVICE).
    UINT16 bcdUSB;                // USB Spec Release Number (BCD).
    UINT8  bDeviceClass;          // Class code (assigned by the USB-IF). 0xFF-Vendor specific.
    UINT8  bDeviceSubClass;       // Subclass code (assigned by the USB-IF).
    UINT8  bDeviceProtocol;       // Protocol code (assigned by the USB-IF). 0xFF-Vendor specific.
    UINT8  bMaxPacketSize0;       // Maximum packet size for endpoint 0.
    UINT16 idVendor;              // Vendor ID (assigned by the USB-IF).
    UINT16 idProduct;             // Product ID (assigned by the manufacturer).
    UINT16 bcdDevice;             // Device release number (BCD).
    UINT8  iManufacturer;         // Index of String Descriptor describing the manufacturer.
    UINT8  iProduct;              // Index of String Descriptor describing the product.
    UINT8  iSerialNumber;         // Index of String Descriptor with the device's serial number.
    UINT8  bNumConfigurations;    // Number of possible configurations.
} USB_DEVICE_DESCRIPTOR;

// *****************************************************************************
/* USB Configuration Descriptor Structure

This struct defines the structure of a USB Configuration Descriptor.  Note that this
structure may need to be packed, or even accessed as bytes, to properly access
the correct fields when used on some device architectures.
*/
typedef struct __attribute__ ((packed)) _USB_CONFIGURATION_DESCRIPTOR
{
    UINT8  bLength;               // Length of this descriptor.
    UINT8  bDescriptorType;       // CONFIGURATION descriptor type (USB_DESCRIPTOR_CONFIGURATION).
    UINT16 wTotalLength;          // Total length of all descriptors for this configuration.
    UINT8  bNumInterfaces;        // Number of interfaces in this configuration.
    UINT8  bConfigurationValue;   // Value of this configuration (1 based).
    UINT8  iConfiguration;        // Index of String Descriptor describing the configuration.
    UINT8  bmAttributes;          // Configuration characteristics.
    UINT8  bMaxPower;             // Maximum power consumed by this configuration.
} USB_CONFIGURATION_DESCRIPTOR;

// *****************************************************************************
/* USB Interface Descriptor Structure

This struct defines the structure of a USB Interface Descriptor.  Note that this
structure may need to be packed, or even accessed as bytes, to properly access
the correct fields when used on some device architectures.
*/
typedef struct __attribute__ ((packed)) _USB_INTERFACE_DESCRIPTOR
{
    UINT8 bLength;               // Length of this descriptor.
    UINT8 bDescriptorType;       // INTERFACE descriptor type (USB_DESCRIPTOR_INTERFACE).
    UINT8 bInterfaceNumber;      // Number of this interface (0 based).
    UINT8 bAlternateSetting;     // Value of this alternate interface setting.
    UINT8 bNumEndpoints;         // Number of endpoints in this interface.
    UINT8 bInterfaceClass;       // Class code (assigned by the USB-IF).  0xFF-Vendor specific.
    UINT8 bInterfaceSubClass;    // Subclass code (assigned by the USB-IF).
    UINT8 bInterfaceProtocol;    // Protocol code (assigned by the USB-IF).  0xFF-Vendor specific.
    UINT8 iInterface;            // Index of String Descriptor describing the interface.
} USB_INTERFACE_DESCRIPTOR;

// *****************************************************************************
/* USB Endpoint Descriptor Structure

This struct defines the structure of a USB Endpoint Descriptor.  Note that this
structure may need to be packed, or even accessed as bytes, to properly access
the correct fields when used on some device architectures.
*/
typedef struct __attribute__ ((packed)) _USB_ENDPOINT_DESCRIPTOR
{
    UINT8  bLength;               // Length of this descriptor.
    UINT8  bDescriptorType;       // ENDPOINT descriptor type (USB_DESCRIPTOR_ENDPOINT).
    UINT8  bEndpointAddress;      // Endpoint address. Bit 7 indicates direction (0=OUT, 1=IN).
    UINT8  bmAttributes;          // Endpoint transfer type.
    UINT16 wMaxPacketSize;        // Maximum packet size.
    UINT8  bInterval;             // Polling interval in frames.
} USB_ENDPOINT_DESCRIPTOR;

/* Descriptor IDs
The descriptor ID type defines the information required by the HOST during a
GET_DESCRIPTOR request
*/
typedef struct
{
    UINT8  index;
    UINT8  type;
    UINT16 language_id;

} DESCRIPTOR_ID;

// ******************************************************************
// Section: USB String Descriptor Structure
// ******************************************************************
// This structure describes the USB string descriptor.  The string
// descriptor provides user-readable information about various aspects of
// the device.  The first string desriptor (string descriptor zero (0)),
// provides a list of the number of languages supported by the set of
// string descriptors for this device instead of an actual string.
//
// Note: The strings are in 2-byte-per-character unicode, not ASCII.
//
// Note: This structure only describes the "header" of the string
// descriptor.  The actual data (either the language ID array or the
// array of unicode characters making up the string, must be allocated
// immediately following this header with no padding between them.

typedef struct __attribute__ ((packed)) _USB_STRING_DESCRIPTOR
{
    UINT8   bLength;             // Size of this descriptor
    UINT8   bDescriptorType;     // Type, USB_DSC_STRING

} USB_STRING_DESCRIPTOR;

#define USB_STRING_INIT(nchars) struct {\
    UINT8  bLength;          \
    UINT8  bDescriptorType;  \
    UINT16 string[nchars];  \
}
// ******************************************************************
// Section: USB Device Qualifier Descriptor Structure
// ******************************************************************
// This structure describes the device qualifier descriptor.  The device
// qualifier descriptor provides overall device information if the device
// supports "other" speeds.
//
// Note: A high-speed device may support "other" speeds (ie. full or low).
// If so, it may need to implement the the device qualifier and other
// speed descriptors.

#if 0
typedef struct __attribute__ ((packed)) _USB_DEVICE_QUALIFIER_DESCRIPTOR
{
    UINT8 bLength;               // Size of this descriptor
    UINT8 bType;                 // Type, always USB_DESCRIPTOR_DEVICE_QUALIFIER
    UINT16 bcdUSB;                // USB spec version, in BCD
    UINT8 bDeviceClass;          // Device class code
    UINT8 bDeviceSubClass;       // Device sub-class code
    UINT8 bDeviceProtocol;       // Device protocol
    UINT8 bMaxPacketSize0;       // EP0, max packet size
    UINT8 bNumConfigurations;    // Number of "other-speed" configurations
    UINT8 bReserved;             // Always zero (0)

} USB_DEVICE_QUALIFIER_DESCRIPTOR;
#endif

// ******************************************************************
// Section: USB HID Descriptor Structure
// ******************************************************************
// USB HID Descriptor as detailed in section "6.2.1 HID Descriptor"
// of the HID class definition specification

typedef struct __attribute__ ((packed)) _USB_HID_DESCRIPTOR
{
    UINT8  bLength;              // Length of this descriptor
    UINT8  bDescriptorType;      // INTERFACE descriptor type
    UINT16 bcdHID;               //
    UINT8  bCountryCode;         //
    UINT8  bNumDescriptors;      //
    UINT8  bRDescriptorType;     //
    UINT16 wDescriptorLength;    //
} USB_HID_DESCRIPTOR;

#if 0
typedef struct _USB_HID_DSC
{
    UINT8 bLength;		//offset 0
    UINT8 bDescriptorType;	//offset 1
    UINT16 bcdHID;		//offset 2
    UINT8 bCountryCode;		//offset 4
    UINT8 bNumDsc;		//offset 5

    //USB_HID_DSC_HEADER hid_dsc_header[HID_NUM_OF_DSC];
    /* HID_NUM_OF_DSC is defined in usbcfg.h */

} USB_HID_DSC;

typedef struct _USB_HID_DSC_HEADER
{
    UINT8 bDescriptorType;	//offset 9
    UINT16 wDscLength;		//offset 10
} USB_HID_DSC_HEADER;
#endif

// ******************************************************************
// Section: USB Setup Packet Structure
// ******************************************************************
// This structure describes the data contained in a USB standard device
// request's setup packet.  It is the data packet sent from the host to
// the device to control and configure the device.
//
// Note: Refer to the USB 2.0 specification for additional details on the
// usage of the setup packet and standard device requests.

typedef union __attribute__ ((packed))
{
    /** Standard Device Requests ***********************************/
    struct __attribute__ ((packed))
    {
        UINT8  bmRequestType; //from table 9-2 of USB2.0 spec
        UINT8  bRequest;      //from table 9-2 of USB2.0 spec
        UINT16 wValue;        //from table 9-2 of USB2.0 spec
        UINT16 wIndex;        //from table 9-2 of USB2.0 spec
        UINT16 wLength;       //from table 9-2 of USB2.0 spec
    };
    struct __attribute__ ((packed))
    {
        unsigned :8;
        unsigned :8;
        WORD_VAL W_Value;     //from table 9-2 of USB2.0 spec, allows byte/bitwise access
        WORD_VAL W_Index;     //from table 9-2 of USB2.0 spec, allows byte/bitwise access
        WORD_VAL W_Length;    //from table 9-2 of USB2.0 spec, allows byte/bitwise access
    };
    struct __attribute__ ((packed))
    {
        unsigned Recipient:5;  //Device,Interface,Endpoint,Other
        unsigned RequestType:2;//Standard,Class,Vendor,Reserved
        unsigned DataDir:1;    //Host-to-device,Device-to-host
        unsigned :8;
        UINT8 bFeature;        //DEVICE_REMOTE_WAKEUP,ENDPOINT_HALT
        unsigned :8;
        unsigned :8;
        unsigned :8;
        unsigned :8;
        unsigned :8;
    };
    struct __attribute__ ((packed))
    {
        union                               // offset   description
        {                                   // ------   ------------------------
            UINT8 bmRequestType;            //   0      Bit-map of request type
            struct
            {
                unsigned    recipient:  5;  //          Recipient of the request
                unsigned    type:       2;  //          Type of request
                unsigned    direction:  1;  //          Direction of data X-fer
            };
        }requestInfo;
    };
    struct __attribute__ ((packed))
    {
        unsigned :8;
        unsigned :8;
        UINT8 bDscIndex;                //For Configuration and String DSC Only
        UINT8 bDescriptorType;          //Device,Configuration,String
        UINT16 wLangID;                 //Language ID
        unsigned :8;
        unsigned :8;
    };
    struct __attribute__ ((packed))
    {
        unsigned :8;
        unsigned :8;
        BYTE_VAL bDevADR;       //Device Address 0-127
        UINT8 bDevADRH;          //Must equal zero
        unsigned :8;
        unsigned :8;
        unsigned :8;
        unsigned :8;
    };
    struct __attribute__ ((packed))
    {
        unsigned :8;
        unsigned :8;
        UINT8 bConfigurationValue;         //Configuration Value 0-255
        UINT8 bCfgRSD;           //Must equal zero (Reserved)
        unsigned :8;
        unsigned :8;
        unsigned :8;
        unsigned :8;
    };
    struct __attribute__ ((packed))
    {
        unsigned :8;
        unsigned :8;
        UINT8 bAltID;            //Alternate Setting Value 0-255
        UINT8 bAltID_H;          //Must equal zero
        UINT8 bIntfID;           //Interface Number Value 0-255
        UINT8 bIntfID_H;         //Must equal zero
        unsigned :8;
        unsigned :8;
    };
    struct __attribute__ ((packed))
    {
        unsigned :8;
        unsigned :8;
        unsigned :8;
        unsigned :8;
        UINT8 bEPID;             //Endpoint ID (Number & Direction)
        UINT8 bEPID_H;           //Must equal zero
        unsigned :8;
        unsigned :8;
    };
    struct __attribute__ ((packed))
    {
        unsigned :8;
        unsigned :8;
        unsigned :8;
        unsigned :8;
        unsigned EPNum:4;       //Endpoint Number 0-15
        unsigned :3;
        unsigned EPDir:1;       //Endpoint Direction: 0-OUT, 1-IN
        unsigned :8;
        unsigned :8;
        unsigned :8;
    };
    struct __attribute__ ((packed))
    {
         unsigned :8;
         unsigned :8;
         BYTE bReportType;    // wValue highByte
         BYTE bReportID;        // wValue lowByte
         unsigned :8;
         unsigned :8;
         unsigned :8;
         unsigned :8;
     };
    /** End: Standard Device Requests ******************************/

} CTRL_TRF_SETUP, SETUP_PKT, *PSETUP_PKT;

// *****************************************************************************
/* Data Transfer Flags

The following flags are used in the flags parameter of the "USBDEVTransferData"
and "USBHALTransferData" routines.  They can be accessed by the bitfield
definitions or the macros can be OR'd together to identify the endpoint number
and properties of the data transfer.

<code>
 7 6 5 4 3 2 1 0 - Field name
 | | | | \_____/
 | | | |    +----- ep_num    - Endpoint number
 | | | +---------- zero_pkt  - End transfer with short or zero-sized packet
 | | +------------ dts       - 0=DATA0 packet, 1=DATA1 packet
 | +-------------- force_dts - Force data toggle sync to match dts field
 +---------------- direction - Transfer direction: 0=Receive, 1=Transmit
</code>
*/


typedef union
{
    UINT8    bitmap;
    struct
    {
        unsigned ep_num:    4;
        unsigned zero_pkt:  1;
        unsigned dts:       1;
        unsigned force_dts: 1;
        unsigned direction: 1;
    }field;
} TRANSFER_FLAGS;

// Buffer Descriptor Status Register layout.
typedef union __attribute__ ((packed)) _BD_STAT
{
    struct __attribute__ ((packed)){
        unsigned            :2;
        unsigned    BSTALL  :1;     //Buffer Stall Enable
        unsigned    DTSEN   :1;     //Data Toggle Synch Enable
        unsigned            :2;     //Reserved - write as 00
        unsigned    DTS     :1;     //Data Toggle Synch Value
        unsigned    UOWN    :1;     //USB Ownership
    };
    struct __attribute__ ((packed)){
        unsigned            :2;
        unsigned    PID0    :1;
        unsigned    PID1    :1;
        unsigned    PID2    :1;
        unsigned    PID3    :1;

    };
    struct __attribute__ ((packed)){
        unsigned            :2;
        unsigned    PID     :4;         //Packet Identifier
    };
    UINT16          Val;
} BD_STAT;

// BDT Entry Layout
typedef union __attribute__ ((packed))__BDT
{
    struct __attribute__ ((packed))
    {
        BD_STAT     STAT;
        UINT16      CNT:10;
        UINT32      ADR;                      //Buffer Address
    };
    struct __attribute__ ((packed))
    {
        UINT32      res  :16;
        UINT32      count:10;
    };
    UINT32          w[2];
    UINT16          v[4];
    UINT64          Val;
} BDT_ENTRY;

// USTAT Register Layout
typedef union __USTAT
{
    struct
    {
        unsigned filler1           :2;
        unsigned ping_pong         :1;
        unsigned direction         :1;
        unsigned endpoint_number   :4;
    };
    UINT8 Val;
} USTAT_FIELDS;

/*******************************************************************************
 Macros
*******************************************************************************/

#define USBHandleBusy(handle) (handle==0?0:((volatile BDT_ENTRY*)handle)->STAT.UOWN)

// advance the passed pointer to the next buffer state
//#define USBAdvancePingPongBuffer(buffer) ((BYTE_VAL*)buffer)->Val ^= USB_NEXT_PING_PONG;
//#define USBHALPingPongSetToOdd(buffer)   {((BYTE_VAL*)buffer)->Val |= USB_NEXT_PING_PONG;}
//#define USBHALPingPongSetToEven(buffer)  {((BYTE_VAL*)buffer)->Val &= ~USB_NEXT_PING_PONG;}

// transform decimal to bcd number
#define BCD(x)   ((( x / 10 ) << 4) | ( x % 10 ))

#define SetConfigurationOptions()   {\
    U1CNFG1 = 0;\
    U1EIE = 0x9F;\
    U1IE = 0x99 | USB_SOF_INTERRUPT | USB_ERROR_INTERRUPT;\
    U1OTGCON &= 0x000F;\
    U1OTGCON |= USB_PULLUP_OPTION;\
}

#define USBSE0Event             0// U1IRbits.URSTIF//  U1CONbits.SE0

/*******************************************************************************
 Function Prototypes
*******************************************************************************/

//External Functions
extern void USBEventHandler(void); // defined in main.c

//Internal Functions
void USBDeviceInit(void);
void USBCheckCable(void);
void USBDeviceTasks(void);
void USBCheckHIDRequest(void);
void USBEnableEndpoint(UINT8, UINT8);
USB_HANDLE USBTransferOnePacket(UINT8, UINT8*);
//USB_HANDLE USBTransferOnePacket(UINT8, UINT8, UINT8*, UINT8);
//BOOL USBHandleBusy(USB_HANDLE);

static void USBStallHandler(void);
static void USBCtrlEPService(void);
static void USBCtrlTrfSetupHandler(void);
static void USBCtrlTrfOutHandler(void);
static void USBCtrlTrfInHandler(void);
static void USBCheckStdRequest(void);
static void USBStdFeatureReqHandler(void);
static void USBCtrlEPAllowStatusStage(void);
static void USBCtrlEPAllowDataStage(void);
static void USBStdGetDscHandler(void);
static void USBStdGetStatusHandler(void);
static void USBCtrlEPServiceComplete(void);
static void USBCtrlTrfTxService(void);
static void USBCtrlTrfRxService(void);
static void USBStdSetCfgHandler(void);
static void USBConfigureEndpoint(UINT8, UINT8);
static void USBStallEndpoint(UINT8, UINT8);

#endif // _USB_H_
