// Firmware framework for USB I/O on PIC 18F2455 (and siblings)
// Copyright (C) 2005 Alexander Enzmann
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

#include "compiler.h"
#include "types.h"
//#include "boot.h"
#include "hardware.h"
#include "usb.h"
#if (BOOT_USE_DEBUG)
#include "serial.h"
#endif

extern void UsbBootCmd(void);
extern void UsbBootExit(void);

#if (BOOT_USE_LOWPOWER)
extern u8 userApp;
#endif

// To prevent a bug in XC8 v1.36, v1.37
#if defined(__XC8__) && defined(__18f47j53)
u8 _mediumconst=0;
u8 _smallconst=0;
#endif

// Device and configuration descriptors.  These are used as the
// host enumerates the device and discovers what class of device
// it is and what interfaces it supports.

const USB_Device_Descriptor device_descriptor = 
{
    sizeof(USB_Device_Descriptor),              // Size of this descriptor in bytes
    DEVICE_DESCRIPTOR,                          // Device descriptor type
    0x0200,                                     // USB Spec Release Number in BCD format (0x0100 for USB 1.0, 0x0110 for USB1.1, 0x0200 for USB2.0)
    0xFF,                                       // Class Code-->00
    0xFF,                                       // Subclass code
    0xFF,                                       // Protocol code
    EP0_BUFFER_SIZE,                            // Max packet size for EP0
    VENDORID,                                   // Vendor ID, microchip=0x04D8, generic=0x05f9, test=0x067b
    PRODUCTID,                                  // Product ID 0x00A für CDC, generic=0xffff, test=0x2303
    (BCD(MAJOR_VERSION)<<8)|BCD(MINOR_VERSION), // Device release number in BCD format-->0
    1,                                          // Manufacturer string index (0=no string descriptor)
    2,                                          // Product string index (0=no string descriptor)
    3,                                          // Device serial number string index
    1                                           // Number of possible configurations
};

// added on 11/10/13
/*
const USB_Device_Qualifier_Descriptor device_qualifier_descriptor = 
{
    sizeof(USB_Device_Qualifier_Descriptor),    // Size of this descriptor in bytes
    DEVICE_QUALIFIER_DESCRIPTOR,                // Device descriptor type
    0x0200,                                     // USB Spec Release Number in BCD format (0x0100 for USB 1.0, 0x0110 for USB1.1, 0x0200 for USB2.0)
    0xFF,                                       // Class Code-->00
    0xFF,                                       // Subclass code
    0xFF,                                       // Protocol code
    EP0_BUFFER_SIZE,                            // Max packet size for EP0
    1,                                          // Number of possible configurations
    0                                           // Reserved
};
*/

const USB_Configuration_Descriptor configuration_descriptor = 
{
    // Configuration Descriptor Header
    {
        sizeof(USB_Configuration_Descriptor_Header),// Size of this descriptor in bytes
        CONFIGURATION_DESCRIPTOR,                   // CONFIGURATION descriptor type
        sizeof(USB_Configuration_Descriptor),       // Total length of data for this configuration
        1,                                          // Number of interfaces in this configuration
        1,                                          // Index value of this configuration
        0,                                          // Configuration string index
        DEFAULTATTR | BUSPOWERED | REMOTEWAKEUP,    // 192,//DEFAULT | POWERED,                    // Attributes
        125                                         // Maximum Power Consumption in 2mA units
    },                                              

    // Data Interface Descriptor with in and out EPs
    {
        sizeof(USB_Interface_Descriptor),           // Size of this descriptor in bytes
        INTERFACE_DESCRIPTOR,                       // Interface descriptor type
        0,                                          // Interface Number
        0,                                          // Alternate Setting Number
        NB_ENDPOINTS,                               // Number of endpoints in this interface
        0xff,                                       // Class code
        0xff,                                       // TODO: Subclass code
        0xff,                                       // TODO: Protocol code
        0                                           // Index of String Descriptor Describing this interface-->2
    },

    // Endpoint 1 Out
    {
        sizeof(USB_Endpoint_Descriptor),            // Size of Descriptor
        ENDPOINT_DESCRIPTOR,                        // Descriptor Type
        0x01,                                       // Endpoint Address
        BULK_TRANSFER,                              // Attribute = Bulk Transfer
        EP1_BUFFER_SIZE,                            // Packet Size
        0x00                                        // Poll Intervall
    },

    // Endpoint 1 IN
    {
        sizeof(USB_Endpoint_Descriptor),            // Size of Descriptor
        ENDPOINT_DESCRIPTOR,                        // Descriptor Type
        0x81,                                       // Endpoint Address
        BULK_TRANSFER,                              // Attribute = Bulk Transfer
        EP1_BUFFER_SIZE,                            // Packet Size
        0x00                                        // Poll Intervall
    }
};

// Language code string descriptor (english)
const USB_String_Descriptor iLang = 
{
    sizeof(iLang),
    STRING_DESCRIPTOR,
    {0x0409}
};

// Manufacturer string descriptor
const USB_String_Descriptor iManu = 
{
    sizeof(iManu),
    STRING_DESCRIPTOR,
    {'S','e','a','I','c','e','L','a','b'}
};

// Product string descriptor
const USB_String_Descriptor iProd = 
{
    sizeof(iProd),
    STRING_DESCRIPTOR,
    {'P','i','n','g','u','i','n','o'}
};

// Serial Number string descriptor
const USB_String_Descriptor iSeri = 
{
    sizeof(iSeri),
    STRING_DESCRIPTOR,
    SERIAL
};

// Array of string descriptors
const u8 * const string_descriptor[] = 
{
    (const u8 * const)&iLang,
    (const u8 * const)&iManu,
    (const u8 * const)&iProd,
    (const u8 * const)&iSeri
};

// Global variables
u8 deviceState = DETACHED;
//u8 deviceAddress = 0;
u8 currentConfiguration = 0;
u8 ctrlTransferStage;               // Holds the current stage in a control transfer
u8 requestHandled;                  // Set to 1 if request was understood and processed.
u8 *pBufferToHost;                  // Data to send to the host
u8 *pBufferFromHost;                // Data from the host
u16 wCount;                         // Number of bytes of data

/***********************************************************************
 * Buffer Descriptors Table (see datasheet p171) must be placed at
 * specific RAM Bank depending on MCU.
 * Endpoints buffers must be in the USB RAM (between 400h and 7FFh).
 **********************************************************************/

#ifdef __XC8__
    #define BD_ADDR_TAG @##BD_ADDR
    // Each endpoint has IN and OUT direction
    BufferDescriptorTable ep_bdt[2*NB_ENDPOINTS] BD_ADDR_TAG;
    #if defined(__16f1459)
        u8 __section("usbram") dummy; // to prevent a compilation error
        setupPacketStruct SetupPacket @ 0x2010; //0x2080;
        u8 controlTransferBuffer[EP0_BUFFER_SIZE] @ 0x2050; //0x20C0;
    #else
        setupPacketStruct __section("usbram") SetupPacket;             //0x500
        u8 __section("usbram") controlTransferBuffer[EP0_BUFFER_SIZE]; //0x540
    #endif
#else // SDCC
    BufferDescriptorTable __at BD_ADDR ep_bdt[2*NB_ENDPOINTS];
    #if defined(__16f1459)
        setupPacketStruct __at 0x2010 SetupPacket;
        u8 __at 0x2050 controlTransferBuffer[EP0_BUFFER_SIZE];
    #else
        #pragma udata usbram5 SetupPacket controlTransferBuffer
        setupPacketStruct SetupPacket;
        u8 controlTransferBuffer[EP0_BUFFER_SIZE];
    #endif
#endif

allcmd bootCmd;

/***********************************************************************
 * Configures the buffer descriptor for endpoint 0
 * so that it is waiting for the status stage of a control transfer.
 **********************************************************************/
  
void UsbSetupStage(void)
{
    #if 0//(BOOT_USE_DEBUG)
    SerialPrintLN("Setup");
    #endif

    ctrlTransferStage = SETUP_STAGE;

    // SIE owns this buffer
    EP_OUT_BD(0).CNT = EP0_BUFFER_SIZE;
    EP_OUT_BD(0).ADDR = (u16)&SetupPacket;
    EP_OUT_BD(0).STAT.val = BDS_UOWN | BDS_DTSEN;

    // CPU owns this buffer
    //EP_IN_BD(0).STAT.val = 0x00;
    EP_IN_BD(0).STAT.val = BDS_COWN;
}

/***********************************************************************
 * Data stage for a Control Transfer that sends data to the host
 **********************************************************************/

void UsbDataInStage(void)
{
    // bufferSize <= EP0_BUFFER_SIZE <= 64
    #if (BOOT_USE_LARGE_EP)
    u16 bufferSize;
    #else
    u8 bufferSize;
    #endif
    
    #if 0//(BOOT_USE_DEBUG)
    SerialPrintLN("Data IN");
    #endif

    // Determine how many bytes are going to the host
    if (wCount < EP0_BUFFER_SIZE)
        bufferSize = wCount;
    else
        bufferSize = EP0_BUFFER_SIZE;
        
    // Update the number of bytes that still need to be sent.
    wCount = wCount - bufferSize;

    // Clear BC8 and BC9
    EP_IN_BD(0).STAT.val &= ~(BDS_BC8 | BDS_BC9);

    #if (BOOT_USE_LARGE_EP)
    // Load the high two bits of the byte count into BC8:BC9
    EP_IN_BD(0).STAT.val |= (u8)((bufferSize & 0x0300) >> 8);
    EP_IN_BD(0).CNT = (u8)(bufferSize & 0xFF);
    #else
    EP_IN_BD(0).CNT = bufferSize;
    #endif
    
    EP_IN_BD(0).ADDR = (u16)&controlTransferBuffer;

    // Move data to the USB output buffer
    pBufferFromHost = (u8*)&controlTransferBuffer;

    while (bufferSize--)
        *pBufferFromHost++ = *pBufferToHost++;
}

/***********************************************************************
 * Display the USB error
 **********************************************************************/

#if 0 //(BOOT_USE_DEBUG)
void UsbErrorEvent(void)
{
    SerialPrint("ERROR : UEIR=0b");
    SerialPrintNumber(UEIR, 2);
    SerialPrint("\r\n");
    UEIR = 0x00;                    // reset error flags
}
#endif

/***********************************************************************
 * Reset the USB bus
 * When the host wants to start communicating with a device it will
 * start by applying a 'Reset' condition which sets the device to its
 * uncongigured default state.
 **********************************************************************/
 
void UsbResetEvent(void)
{
    #if (BOOT_USE_DEBUG)
    SerialPrintLN("Reseted");
    #endif

    UIR  = 0;                       // Reset all interrupts
    UEIR = 0;                       // Reset all errors interrupts
    UEP0 = 0;                       // Reset EP0
    UCONbits.PPBRST = 1;            // Reset ping pong buffer pointers
    UADDR = 0;                      // Reset the address back to 0
    //deviceAddress = 0;              // Reset the address back to 0
    deviceState = DEFAULT;          // Device is now in the default state
    currentConfiguration = 0;       // clear active configuration

    UCONbits.PKTDIS = 0;            // Enable packet processing
    UCONbits.PPBRST = 0;            // Stop trying to reset ping pong buffer pointers
    UEP0 = EP_CTRL | HSHK_EN;       // Set EP0 as a control endpoint with handshaking enabled
    
    while (UIRbits.TRNIF)
        UIRbits.TRNIF = 0;          // Empty the USTAT FIFO
    
    UsbSetupStage();
}

/***********************************************************************
 * Suspend all processing until we detect activity on the USB bus
 * The state of the data lines when the pulled up line is high,
 * and the other line is low, is called the idle state.
 * This is the state of the lines before and after a packet is sent.
 * NB : The ACTVIF bit cannot be cleared immediately after the USB module
 * wakes up from Suspend or while the USB module is suspended.
 **********************************************************************/

#if (BOOT_USE_LOWPOWER)
void UsbSuspendEvent(void)
{
    #if 0//(BOOT_USE_DEBUG)
    SerialPrintLN("Suspended");
    #endif

    UCONbits.SUSPND = 1;            // Switch to suspended mode
    LedOff();                       // Led Off

    #if (BOOT_USE_DEBUG)
    SerialDisable();
    #endif

    // Enable USB interrupt
    #if defined(__18f25k50) || defined(__18f45k50)
    PIR3bits.USBIF = 0;
    PIE3bits.USBIE = 1;
    #else
    PIR2bits.USBIF = 0;
    PIE2bits.USBIE = 1;
    #endif

    __asm__("SLEEP");

    // Disable USB interrupt
    #if defined(__18f25k50) || defined(__18f45k50)
    PIE3bits.USBIE = 0;
    #else
    PIE2bits.USBIE = 0;
    #endif

    #if 0//(BOOT_USE_DEBUG)
    SerialInit(9600);
    SerialPrintLN("Woken up");
    #endif
}
#endif

/***********************************************************************
 * A transaction has finished.  Try default processing on endpoint 0.
 * This is the starting point for processing a Control Transfer.
 * The code directly follows the sequence of transactions described in
 * the USB spec chapter 5. The only Control Pipe in this firmware is
 * the Default Control Pipe (endpoint 0).
 * Control messages that have a different destination will be discarded.
 **********************************************************************/

void UsbTransEvent(void)
{
    //u8 bufferSize;
    u16 bufferSize;
    u8 pid, ep = USTAT >> 3;            // Get encoded number (bit 6-3)
                                        // of the last active Endpoint

    if (ep == 1)                        // EndPoint 1
    {
        //if (USTATbits.DIR == OUT)
        if (!USTATbits.DIR)
        {
            #if 0//(BOOT_USE_DEBUG)
            SerialPrintLN("EP1 OUT");
            #endif

            UsbBootCmd();
        }
    }

    else //if (ep == 0)                   // Endpoint 0
    {
        if (USTATbits.DIR == OUT)
        {
            #if 0//(BOOT_USE_DEBUG)
            SerialPrintLN("EP0 OUT");
            #endif

            // Pull PID from middle of BD0STAT
            pid = (EP_OUT_BD(0).STAT.val & 0x3C) >> 2;

            /*
            * SETUP PID - a transaction is starting
            * Process the Setup stage of a control transfer.
            * This code initializes the flags that let the firmware know
            * what to do during subsequent stages of the transfer.
            * Only Ep0 is handled here.
            */

            if (pid == PID_SETUP)
            {
                #if 0//(BOOT_USE_DEBUG)
                SerialPrintLN("PID SETUP");
                #endif
                // Note: Microchip says to turn off the UOWN bit on
                // the IN direction as soon as possible after detecting
                // that a SETUP has been received.
                EP_IN_BD(0).STAT.val  &= ~BDS_UOWN;
                EP_OUT_BD(0).STAT.val &= ~BDS_UOWN;

                // Initialize the transfer process
                ctrlTransferStage = SETUP_STAGE;
                requestHandled = 0;     // request hasn't been handled yet
                wCount = 0;             // No bytes transferred

                // Not a standard request - don't process here.
                // Class or Vendor requests have to be handled seperately.
                if (SetupPacket.bmRequestType & 0x60)
                {
                    #if 0//(BOOT_USE_DEBUG)
                    SerialPrint("NOSTDREQ:");
                    //SerialPrintNumber((SetupPacket.bmRequestType & 0x60)>>5, 10);
                    SerialPrint("\r\n");
                    #endif
                    return;
                }

                // Set the address of the device.  All future requests
                // will come to that address.  Can't actually set UADDR
                // to the new address yet because the rest of the SET_ADDRESS
                // transaction uses address 0.
                if (SetupPacket.bRequest == SET_ADDRESS)
                {
                    #if 0//(BOOT_USE_DEBUG)
                    SerialPrintLN("SET_ADDR");
                    #endif
                    requestHandled = 1;
                    deviceState = ADDRESS;
                    //deviceAddress = SetupPacket.wValue0;
                }

                else if (SetupPacket.bRequest == GET_DESCRIPTOR)
                {
                    if(SetupPacket.bmRequestType == 0x80)
                    {
                        //u8 descriptorType  = SetupPacket.wValue1;
                        //u8 descriptorIndex = SetupPacket.wValue0;

                        if (SetupPacket.wValue1 == DEVICE_DESCRIPTOR)
                        {
                            #if 0//(BOOT_USE_DEBUG)
                            SerialPrintLN("DEV_DESC");
                            #endif
                            requestHandled = 1;
                            pBufferToHost = (u8*)&device_descriptor;
                            wCount = sizeof(USB_Device_Descriptor);
                        }

                        else if (SetupPacket.wValue1 == CONFIGURATION_DESCRIPTOR)
                        {
                            #if 0//(BOOT_USE_DEBUG)
                            SerialPrintLN("CONF_DESC");
                            #endif
                            requestHandled = 1;
                            pBufferToHost = (u8*)&configuration_descriptor;
                            wCount = configuration_descriptor.Header.wTotalLength;
                        }

                        else if (SetupPacket.wValue1 == STRING_DESCRIPTOR)
                        {
                            #if 0//(BOOT_USE_DEBUG)
                            SerialPrintLN("STR_DESC");
                            #endif
                            requestHandled = 1;
                            pBufferToHost = (u8*)string_descriptor[SetupPacket.wValue0];
                            wCount = *pBufferToHost;
                        }
                        
                        /*
                        else if (SetupPacket.wValue1 == DEVICE_QUALIFIER_DESCRIPTOR)
                        {
                            requestHandled = 1;
                            pBufferToHost = (u8 *)&device_qualifier_descriptor;
                            wCount = sizeof(USB_Device_Qualifier_Descriptor);
                        }
                        */
                    }
                }

                else if (SetupPacket.bRequest == SET_CONFIGURATION)
                {
                    #if 0//(BOOT_USE_DEBUG)
                    SerialPrintLN("SET_CONF");
                    #endif
                    requestHandled = 1;

                    // If configuration value is zero, put device in address state
                    currentConfiguration = SetupPacket.wValue0;
                    if (currentConfiguration == 0)
                    {
                        deviceState = ADDRESS;
                    }
                    
                    // Otherwise, initialize the endpoints
                    else
                    {
                        #ifdef BOOT_USE_HID
                            //UsbHIDInitEndpoint();
                        #endif
                        
                        #ifdef BOOT_USE_UART
                            //UsbUARTInitEndpoint();
                        #endif
                        
                        #ifdef BOOT_USE_CDC
                            //UsbCDCInitEndpoint();
                        #endif      
                        
                        #if (BOOT_USE_BULK)
                            //UsbBulkInitEndpoint();
                            //UEP1 = 0b00011110;
                            UEP1 = EP_CTRL | EP_OUT | EP_IN | HSHK_EN; 
                            //UEP1 = EP_OUT | EP_IN | HSHK_EN; 

                            // for IN
                            EP_IN_BD(1).STAT.val  = BDS_DTS;

                            // for OUT
                            EP_OUT_BD(1).CNT  = EP1_BUFFER_SIZE;
                            EP_OUT_BD(1).ADDR = (u16)&bootCmd;
                            EP_OUT_BD(1).STAT.val = BDS_UOWN;
                            //EP_OUT_BD(1).STAT.val = BDS_UOWN | BDS_DTSEN;

                        #endif

                        deviceState = CONFIGURED;
                    }   
                }

                else if (SetupPacket.bRequest == GET_CONFIGURATION)
                {
                    #if 0//(BOOT_USE_DEBUG)
                    SerialPrintLN("GET_CONF");
                    #endif
                    requestHandled = 1;
                    pBufferToHost = (u8*)&currentConfiguration;
                    wCount = 1;
                }

                else if (SetupPacket.bRequest == GET_INTERFACE)
                {
                    #if 0//(BOOT_USE_DEBUG)
                    SerialPrintLN("GET_INTF");
                    #endif
                    // No support for alternate interfaces.
                    // Send zero back to the host.
                    requestHandled = 1;
                    controlTransferBuffer[0] = 0;
                    pBufferToHost = (u8*)&controlTransferBuffer;
                    wCount = 1;
                }

                /*
                else if (SetupPacket.bRequest == GET_STATUS)
                {
                    UsbGetStatus();
                }

                else if ((SetupPacket.bRequest == CLEAR_FEATURE) || (request == SET_FEATURE))
                {
                    UsbSetFeature();
                }
                
                else if (SetupPacket.bRequest == SET_INTERFACE)
                {
                    #if (BOOT_USE_DEBUG)
                    SerialPrintLN("SET_INTF");
                    #endif
                    // No support for alternate interfaces - just ignore.
                    requestHandled = 1;
                }

                else if (SetupPacket.bRequest == SET_DESCRIPTOR)
                {
                    UsbSetDescriptor();
                }
                
                else if (SetupPacket.bRequest == SYNCH_FRAME)
                {
                    UsbSyncFrame();
                }
                
                else
                    //
                */

                // If this service wasn't handled then stall endpoint 0
                // TBD: Add handlers for any other classes/interfaces in the device

                if (!requestHandled)
                {
                    EP_OUT_BD(0).CNT = EP0_BUFFER_SIZE;
                    EP_OUT_BD(0).ADDR = (u16)&SetupPacket;
                    EP_OUT_BD(0).STAT.val = BDS_UOWN | BDS_BSTALL;

                    EP_IN_BD(0).STAT.val  = BDS_UOWN | BDS_BSTALL;
                }

                else if (SetupPacket.bmRequestType & 0x80)
                {
                    if(SetupPacket.wLength < wCount)
                        wCount = SetupPacket.wLength;

                    UsbDataInStage();
                    
                    ctrlTransferStage = DATA_IN_STAGE;
                    
                    // Reset the out buffer descriptor for endpoint 0
                    EP_OUT_BD(0).CNT = EP0_BUFFER_SIZE;
                    EP_OUT_BD(0).ADDR = (u16)&SetupPacket;
                    EP_OUT_BD(0).STAT.val = BDS_UOWN;

                    // Set the in buffer descriptor on endpoint 0 to send data
                    EP_IN_BD(0).ADDR = (u16)&controlTransferBuffer;
                    EP_IN_BD(0).STAT.val = BDS_UOWN | BDS_DTS | BDS_DTSEN;
                }

                else
                {
                    ctrlTransferStage = DATA_OUT_STAGE;

                    // Set the out buffer descriptor on endpoint 0 to receive data
                    EP_OUT_BD(0).CNT = EP0_BUFFER_SIZE;
                    EP_OUT_BD(0).ADDR = (u16)&controlTransferBuffer;
                    EP_OUT_BD(0).STAT.val = BDS_UOWN | BDS_DTS | BDS_DTSEN;

                    // Clear the input buffer descriptor
                    EP_IN_BD(0).CNT = 0;
                    EP_IN_BD(0).STAT.val = BDS_UOWN | BDS_DTS | BDS_DTSEN;
                }

                // Enable SIE token and packet processing
                UCONbits.PKTDIS = 0;
            }
            
            // Complete the data stage so that all information has passed
            // from host to device before servicing it.
            
            else if (ctrlTransferStage == DATA_OUT_STAGE)
            {
                #if (BOOT_USE_LARGE_EP)
                bufferSize = ((0x03 & EP_OUT_BD(0).STAT.val) << 8) | EP_OUT_BD(0).CNT;
                #else
                bufferSize = EP_OUT_BD(0).CNT;
                #endif
                
                // Accumulate total number of bytes read
                wCount = wCount + bufferSize;

                pBufferToHost = (u8*)&controlTransferBuffer;

                while (bufferSize--)
                    *pBufferFromHost++ = *pBufferToHost++;
                
                // Turn control over to the SIE and toggle the data bit
                if(EP_OUT_BD(0).STAT.DTS)
                    EP_OUT_BD(0).STAT.val = BDS_UOWN | BDS_DTSEN;
                else
                    EP_OUT_BD(0).STAT.val = BDS_UOWN | BDS_DTSEN | BDS_DTS;
            }

            else
            {
                UsbSetupStage();
            }
        }

        else // if(USTATbits.DIR == IN)
        {
            #if 0//(BOOT_USE_DEBUG)
            SerialPrintLN("EP0 IN");
            #endif

            if ((UADDR == 0) && (deviceState == ADDRESS))
            {
                // The new address come in through a SET_ADDRESS
                UADDR = SetupPacket.wValue0;
                //UADDR = deviceAddress;
                // If we get a reset after a SET_ADDRESS, then we need
                // to drop back to the Default state.
                if (UADDR == 0)
                    deviceState = DEFAULT;
            }

            if (ctrlTransferStage == DATA_IN_STAGE)
            {
                // Start (or continue) transmitting data
                UsbDataInStage();

                // Turn control over to the SIE and toggle the data bit
                if(EP_IN_BD(0).STAT.DTS)
                    EP_IN_BD(0).STAT.val = BDS_UOWN | BDS_DTSEN;
                else
                    EP_IN_BD(0).STAT.val = BDS_UOWN | BDS_DTSEN | BDS_DTS;
            }

            else
            {
                UsbSetupStage();
            }
        }
    }
}

/***********************************************************************
 * Check for transitions between DETACHED, ATTACHED and POWERED states
 * Check if USB cable is plugged or unplugged.
 * When the device is plugged in to the host, the host will see either
 * D+ or D- go to a '1' level, and will know that a device has been
 * plugged in. The '1' level will be on D- for a low speed device and
 * D+ for a full (or high) speed device.
 **********************************************************************/

void UsbUpdate(void)
{
    //UCONbits.USBEN = 0;                 // USB disabled to enable readings

    // D+ and D- pins are input only
    // TRIS bits will always read as ‘1’
    //VBUS_TRIS |= VBUS_MASK;             // VBUS Pin as Input

    #if 0//(BOOT_USE_DEBUG)
    SerialPrint("VBUS = 0b");
    SerialPrintNumber(VBUS_PORT, 2);
    SerialPrint("\r\n");
    #endif

    // Check for transitions between DETACHED and ATTACHED states
    if (UsbOn())                        // 1 = Not attached, 0 = Attached
    {
        if (UCONbits.USBEN == 0)        // Enable USB if it is disabled
        {
            UCON = 0;
            //UIE = 0;
            UCONbits.USBEN = 1;
            deviceState = ATTACHED;
            #if 0//(BOOT_USE_DEBUG)
            SerialPrintLN("Attached");
            #endif
        }
    }
    
    #if (BOOT_USE_LOWPOWER)
    else // if (UsbOff())
    {
        if (UCONbits.USBEN == 1)        // Disable USB if it is enabled
        {
            UCON = 0;
            //UIE = 0;
            deviceState = DETACHED;
            #if 0//(BOOT_USE_DEBUG)
            SerialPrintLN("Detached");
            #endif

            // No USB, No User App. so let's go to sleep mode
            if (userApp == FALSE)
            {
                #if 0//(BOOT_USE_DEBUG)
                SerialPrintLN("Sleep mode");
                #endif
                
                UsbSuspendEvent();
                
                #if 0//(BOOT_USE_DEBUG)
                SerialPrintLN("Woken up");
                #endif
            }
            
            else
            {
                #if 0//(BOOT_USE_DEBUG)
                SerialPrintLN("User app");
                #endif
                UsbBootExit();                  // Jump to user app.
            }
        }
    }
    #endif
    
    // Check for transitions between ATTACHED and POWERED states
    if ((deviceState == ATTACHED) && !UCONbits.SE0)
    {
        UIR = 0;
        //UIE = 0;
        deviceState = POWERED;
        #if 0//(BOOT_USE_DEBUG)
        SerialPrintLN("Powered");
        #endif
    }
}

/***********************************************************************
 * Service USB interrupts
 **********************************************************************/

void UsbProcessEvents(void)
{
    // Stop here if USB cable is not attached
    // and power is not steady
    if (deviceState < POWERED)
        return;

    // UnSuspend
    // The USB became active then we wake up from suspend.
    // NB1 : Once the SUSPND is clear the URSTIF bit will be asserted.
    // NB2 : The USB module may not be immediately operational after clearing
    // the SUSPND bit if using the 48 MHz PLL source because the PLL will
    // require time to lock.

    if (UIRbits.ACTVIF)
    {
        #if (BOOT_USE_DEBUG)
        SerialPrintLN("Activated");
        #endif

        UCONbits.SUSPND = 0;            // exit from suspended mode
        //while (UIRbits.ACTVIF)
            UIRbits.ACTVIF = 0;
        //return;
    }

    #if (BOOT_USE_LOWPOWER)
    if (UIRbits.RESUMEIF)
    {
        #if (BOOT_USE_DEBUG)
        SerialPrintLN("Resumed");
        #endif

        UCONbits.SUSPND = 0;            // exit from suspended mode
        UIRbits.RESUMEIF = 0;
        //return;
    }
    #endif
    
    // If we are supposed to be suspended, then don't try performing any processing.
    //#if (BOOT_USE_LOWPOWER)
    if (UCONbits.SUSPND)
        return;
    //#endif
    
    // Process a bus reset
    if (UIRbits.URSTIF)
    {
        UsbResetEvent();
        UIRbits.URSTIF = 0;             // Clear the reset interrupt
        //return;
    }
    
    // Suspend all processing before and after a packet is sent
    // eventually enter in sleep mode 
    if (UIRbits.IDLEIF)
    {
        #if (BOOT_USE_LOWPOWER)
        UsbSuspendEvent();
        #endif
        UIRbits.IDLEIF = 0;             // Clear the idle interrupt
        //return;
    }

    // Full speed devices get a Start Of Frame (SOF) packet every 1 ms.
    // Nothing is done but we could add a callback routine to do something
    // like blinking the USERLED (to be checked)
    if (UIRbits.SOFIF)
    {
        UIRbits.SOFIF = 0;
        //return;
    }
    
    // In response to the code stalling an endpoint.
    if (UIRbits.STALLIF && UEP0bits.EPSTALL)
    {
        #if (BOOT_USE_DEBUG)
        SerialPrintLN("EP error");
        #endif

        UsbSetupStage();
        UEP0bits.EPSTALL = 0;
        UIRbits.STALLIF = 0;            // Clear the stall interrupt
        //return;
    }

    // Display error
    #if 0 //(BOOT_USE_DEBUG)
    if (UIRbits.UERRIF)
    {
        UsbErrorEvent();
        UIRbits.UERRIF = 0;
        return;
    }
    #endif
    
    // Unless we have been reset by the host, no need to keep processing
    if (deviceState < DEFAULT)
        return;

    // Check for pending USB transactions
    if (UIRbits.TRNIF)
    {
        UsbTransEvent();
        UIRbits.TRNIF = 0;              // Clear the transfer interrupt
        //return;
    }
}
