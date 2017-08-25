/***********************************************************************
    Title:  USB Pinguino Bootloader
    File:   descriptors.c
    Descr.: USB descriptors for PIC32 processors
    Author: Regis Blanchot <rblanchot@gmail.com>

    This file is part of Pinguino (http://www.pinguino.cc)
    Released under the LGPL license (http://www.gnu.org/licenses/lgpl.html)
***********************************************************************/

#ifndef __DESCRIPTORS_C
#define __DESCRIPTORS_C

#include "p32xxxx.h"
#include "typedefs.h"
#include "boot.h"               // USB Vendor and Product IDs
#include "usb.h"                // USB Device abstraction layer interface

/* Device Descriptor */

const USB_DEVICE_DESCRIPTOR usb_device_descriptor =
{
    sizeof(USB_DEVICE_DESCRIPTOR),           // 0x12 Size of this descriptor in bytes
    USB_DESCRIPTOR_DEVICE,                      // DEVICE descriptor type
    0x0200,                                     // USB Spec Release Number in BCD format
    0x00,                                       // Class Code
    0x00,                                       // Subclass code
    0x00,                                       // Protocol code
    USB_EP0_BUFF_SIZE,                          // Max packet size for EP0, see boot.h
    USB_VENDOR_ID,                              // Vendor ID, see boot.h
    USB_PRODUCT_ID,                             // Product ID, , see boot.h
    (BCD(USB_MAJOR_VER)<<8)|BCD(USB_MINOR_VER), // Version number, see boot.h
    0x01,                                       // Manufacturer string index
    0x02,                                       // Product string index
    0x03,                                       // Device serial number string index
    0x01                                        // Number of possible configurations
};

/* Configuration Descriptor */

//const UINT8 usb_config_descriptor =
const USB_CONFIG_DESCRIPTOR usb_config_descriptor =
{
    /* Configuration Descriptor */
    {
        sizeof(USB_CONFIGURATION_DESCRIPTOR),// 0x09 Size of this descriptor in bytes
        USB_DESCRIPTOR_CONFIGURATION,           // CONFIGURATION descriptor type
        CONFIGURATION_TOTAL_LENGTH,             // Total length of data for this cfg
        USB_MAX_NUM_INT,                        // Number of interfaces in this cfg
        1,                                      // Index value of this configuration
        0,                                      // Configuration string index
        _DEFAULT|_SELF,                         // Attributes, see usb_ch9.h
        50                                      // 100mA max power consumption (2X mA)
    },

    /* Interface Descriptor */
    {
        sizeof(USB_INTERFACE_DESCRIPTOR),    // 0x09 Size of this descriptor in bytes
        USB_DESCRIPTOR_INTERFACE,               // INTERFACE descriptor type
        0,                                      // Interface Number
        0,                                      // Alternate Setting Number
        2,                                      // Number of endpoints in this intf
        HID_INTF,                               // Class code
        0,                                      // Subclass code
        0,                                      // Protocol code
        0                                       // Interface string index
    },

    /* HID Class-Specific Descriptor */
    {
        sizeof(USB_HID_DESCRIPTOR),          // 0x09
        DSC_HID,                                // HID descriptor type
        0x0111,                                 // HID Spec Release Number in BCD format (1.11)
        0x00,                                   // Country Code (0x00 for Not supported)
        HID_NUM_OF_DSC,                         // Number of class descriptors
        DSC_RPT,                                // Report descriptor type
        HID_RPT01_SIZE                          // Size of the report descriptor = sizeof(hid_rpt01)
    },

    /* Endpoint Descriptor */
    {
        sizeof(USB_ENDPOINT_DESCRIPTOR),     // 0x07
        USB_DESCRIPTOR_ENDPOINT,                // Endpoint Descriptor
        HID_EP | _EP_IN,                        // EndpointAddress
        _INTERRUPT,                             // Endpoint Transfer Type
        HID_INT_IN_EP_SIZE,                     // size
        0x01                                    // Interval
    },

    /* Endpoint Descriptor */
    {
        sizeof(USB_ENDPOINT_DESCRIPTOR),     // 0x07
        USB_DESCRIPTOR_ENDPOINT,                // Endpoint Descriptor
        HID_EP | _EP_OUT,                       // EndpointAddress
        _INTERRUPT,                             // Endpoint Transfer Type
        HID_INT_OUT_EP_SIZE,                    // size
        0x01                                    // Interval
    }
};

/* String Descriptor */

// Language code string descriptor
const USB_STRING_INIT(1) lang =
{
    sizeof(lang),
    USB_DESCRIPTOR_STRING,
    {0x0409}
};

// Manufacturer string descriptor
const USB_STRING_INIT(9) manu =
{
    sizeof(manu),
    USB_DESCRIPTOR_STRING,
    {
        'S','e','a','I','c','e','L','a','b',
        /*
        ' ',
        '(','R','.','B','l','a','n','c','h','o','t',')'
        */
    }
};

// Product string descriptor
const USB_STRING_INIT(8) prod =
{
    sizeof(prod),
    USB_DESCRIPTOR_STRING,
    {
        'P','i','n','g','u','i','n','o',
        /*
        ' ',
        '(','w','w','w','.','p','i','n','g','u','i','n','o','.','c','c',')'
        */
    }
};

// Serial Number string descriptor
const USB_STRING_INIT(7) seri =
{
    sizeof(seri),
    USB_DESCRIPTOR_STRING,
    SERIAL
};

//Array of string descriptors
const UINT8 *const usb_string_descriptor[] =
{
    (const UINT8 *const)&lang,
    (const UINT8 *const)&manu,
    (const UINT8 *const)&prod,
    (const UINT8 *const)&seri
};

/* HID Descriptor */
const struct {UINT8 report[HID_RPT01_SIZE];} hid_rpt01=
{
    {
        0x06, 0x00, 0xFF,           // Usage Page = 0xFF00 (Vendor Defined Page 1)
        0x09, 0x01,                 // Usage (Vendor Usage 1)
        0xA1, 0x01,                 // Collection (Application)
        0x19, 0x01,                 //      Usage Minimum
        0x29, 0x40,                 //      Usage Maximum 	//64 input usages total (0x01 to 0x40)
        0x15, 0x00,                 //      Logical Minimum (data bytes in the report may have minimum value = 0x00)
        0x26, 0xFF, 0x00,           //      Logical Maximum (data bytes in the report may have maximum value = 0x00FF = unsigned 255)
        0x75, 0x08,                 //      Report Size: 8-bit field size
        0x95, 0x40,                 //      Report Count: Make sixty-four 8-bit fields (the next time the parser hits an "Input", "Output", or "Feature" item)
        0x81, 0x00,                 //      Input (Data, Array, Abs): Instantiates input packet fields based on the above report size, count, logical min/max, and usage.
        0x19, 0x01,                 //      Usage Minimum
        0x29, 0x40,                 //      Usage Maximum 	//64 output usages total (0x01 to 0x40)
        0x91, 0x00,                 //      Output (Data, Array, Abs): Instantiates output packet fields.  Uses same report size and count as "Input" fields, since nothing new/different was specified to the parser since the "Input" item.
        0xC0                        // End Collection
    }
};

#endif // __DESCRIPTORS_C
