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

#include <pic18fregs.h>
#include "hardware.h"
#include "picUSB.h"
#include "types.h"

extern void usb_ep_data_out_callback(char end_point);

// Device and configuration descriptors.  These are used as the
// host enumerates the device and discovers what class of device
// it is and what interfaces it supports.
// TODO: remove below lines and replace with the apropriate device_desc.blength etc.

__code USB_Device_Descriptor device_descriptor = 
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
    0,                                          // Device serial number string index
    1                                           // Number of possible configurations
};

// added on 11/10/13
/*
__code USB_Device_Qualifier_Descriptor device_qualifier_descriptor = 
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

__code USB_Configuration_Descriptor configuration_descriptor =
{
    // Configuration Descriptor Header
    {sizeof(USB_Configuration_Descriptor_Header),// Size of this descriptor in bytes
    CONFIGURATION_DESCRIPTOR,                   // CONFIGURATION descriptor type
    sizeof(USB_Configuration_Descriptor),       // Total length of data for this configuration
    1,                                          // Number of interfaces in this configuration
    1,                                          // Index value of this configuration
    0,                                          // Configuration string index
    192,//DEFAULT | POWERED,                    // Attributes
    125},                                       // Maximum Power Consumption in 2mA units
    // Data Interface Descriptor with in and out EPs
    {sizeof(USB_Interface_Descriptor),          // Size of this descriptor in bytes
    INTERFACE_DESCRIPTOR,                       // Interface descriptor type
    0,                                          // Interface Number
    0,                                          // Alternate Setting Number
    2,                                          // Number of endpoints in this interface
    0xff,                                       // Class code
    0xff,                                       // TODO: Subclass code
    0xff,                                       // TODO: Protocol code
    0},                                         // Index of String Descriptor Describing this interface-->2
    // Endpoint 1 Out
    {sizeof(USB_Endpoint_Descriptor),           // Size of Descriptor
    ENDPOINT_DESCRIPTOR,                        // Descriptor Type
    0x01,                                       // Endpoint Address
    0x02,                                       // Attribute = Bulk Transfer
    EP1_BUFFER_SIZE,                            // Packet Size
    0x00},                                      // Poll Intervall
    // Endpoint 1 IN
    {sizeof(USB_Endpoint_Descriptor),           // Size of Descriptor
    ENDPOINT_DESCRIPTOR,                        // Descriptor Type
    0x81,                                       // Endpoint Address
    0x02,                                       // Attribute = Bulk Transfer
    EP1_BUFFER_SIZE,                            // Packet Size
    0x00}
};

    const char lang[] = {sizeof(lang),  STRING_DESCRIPTOR,
        0x09,0x04
        }; // english = 0x0409

#if (STRING == 1)

    const char manu[] = {sizeof(manu),  STRING_DESCRIPTOR,
        'R',0x00,'.',0x00,'B',0x00,'l',0x00,'a',0x00,'n',0x00,'c',0x00,'h',0x00,'o',0x00,'t',0x00,
//        '/',0x00,
//        'A',0x00,'.',0x00,'G',0x00,'e',0x00,'n',0x00,'t',0x00,'r',0x00,'i',0x00,'c',0x00
        };

    const char prod[] = {sizeof(prod),  STRING_DESCRIPTOR,
        'P',0x00,'i',0x00,'n',0x00,'g',0x00,'u',0x00,'i',0x00,'n',0x00,'o',0x00
        };

#else

    const char manu[] = {sizeof(manu),  STRING_DESCRIPTOR,
        'R',0x00,'B',0x00,'+',0x00,'A',0x00,'G',0x00,
        };

    const char prod[] = {sizeof(prod),  STRING_DESCRIPTOR,
        'P',0x00,'8',0x00
        };

#endif

    const char * const string_descriptor[] = { lang, manu, prod};

// Global variables
byte deviceState;
byte selfPowered;
//byte remoteWakeup;
byte currentConfiguration;
byte deviceAddress;
byte ctrlTransferStage;                 // Holds the current stage in a control transfer
byte requestHandled;                    // Set to 1 if request was understood and processed.
byte *outPtr;                           // Data to send to the host
byte *inPtr;                            // Data from the host
word wCount;                            // Number of bytes of data

/**
    Buffer descriptors Table (see datasheet page 171)
    A RAM Bank (2, 4 or 13 depending on MCU) is used specifically for
    endpoint buffer control in a structure known as
    Buffer Descriptor Table (BDTZ).
    TODO: find something smarter to allocate the buffer,
    like in usbmmap.c of the microchip firmware.
    If not all endpoints are used the space in RAM is wasted.
**/

///
/// 2012-07-04 ep_bdt[4] -> ep_bdt[32] updated by André
///

#if   defined(__18f14k50) || defined(__18f14k50)
    // Bank 2
    volatile BufferDescriptorTable __at (0x200) ep_bdt[4];
#elif defined(__18f26j53) || defined(__18f46j53) || \
      defined(__18f27j53) || defined(__18f47j53)
    // Bank 13
    volatile BufferDescriptorTable __at (0xD00) ep_bdt[4];
#else
    // Bank 4
    volatile BufferDescriptorTable __at (0x400) ep_bdt[4];
#endif

#pragma udata usbram5 SetupPacket controlTransferBuffer
volatile setupPacketStruct SetupPacket;
volatile byte controlTransferBuffer[EP0_BUFFER_SIZE];

volatile allcmd bootCmd;

  /**
   * Data stage for a Control Transfer that sends data to the host
   **/

void InDataStage()
{
    byte i;
    word bufferSize;

    // Determine how many bytes are going to the host
    if(wCount < EP0_BUFFER_SIZE)
        bufferSize = wCount;
    else
        bufferSize = EP0_BUFFER_SIZE;

    // Load the high two bits of the byte count into BC8:BC9
    // Clear BC8 and BC9
    EP_IN_BD(0).Stat.uc &= ~(BDS_BC8 | BDS_BC9);
    EP_IN_BD(0).Stat.uc |= (byte)((bufferSize & 0x0300) >> 8);
    EP_IN_BD(0).Cnt = (byte)(bufferSize & 0xFF);
    //EP_IN_BD(0).ADDR = PTR16(&controlTransferBuffer);
    EP_IN_BD(0).ADDR = (unsigned long)&controlTransferBuffer;

    // Update the number of bytes that still need to be sent.  Getting
    // all the data back to the host can take multiple transactions, so
    // we need to track how far along we are.
    wCount = wCount - bufferSize;

    // Move data to the USB output buffer from wherever it sits now.
    inPtr = (byte *)&controlTransferBuffer;

    for (i=0;i<bufferSize;i++)
        *inPtr++ = *outPtr++;
}

/**
 * Configures the buffer descriptor for endpoint 0
 * so that it is waiting for the status stage of a control transfer.
 **/
  
void WaitForSetupStage()
{
    ctrlTransferStage = SETUP_STAGE;
    EP_OUT_BD(0).Cnt = EP0_BUFFER_SIZE;
    //EP_OUT_BD(0).ADDR = PTR16(&SetupPacket);
    EP_OUT_BD(0).ADDR = (unsigned long)&SetupPacket;
    // Give to SIE, enable data toggle checks
    EP_OUT_BD(0).Stat.uc = BDS_UOWN | BDS_DTSEN;
    EP_IN_BD(0).Stat.uc = 0x00;           // Give control to CPU
}

void EnableUSBModule()
{
    // TBD: Check for voltage coming from the USB cable and use that
    // as an indication we are attached.
    if(UCONbits.USBEN == 0)
    {
        UCON = 0;               // USB Control Register
        UIE = 0;                // Disable USB Interrupt Register
        UCONbits.USBEN = 1;     // Enable USB module
        deviceState = ATTACHED;
    }
    // If we are attached and no single-ended zero is detected, then
    // we can move to the Powered state.
    if ((deviceState == ATTACHED) && !UCONbits.SE0)
    {
        UIR = 0;
        UIE = 0;                // Disable USB Interrupt Register
        UIEbits.URSTIE = 1;     // Enable USB Reset Interrupt
        UIEbits.IDLEIE = 1;     // Enable IDle Detect USB Interrupt
        deviceState = POWERED;
    }
}

/**
 * Main entry point for USB tasks.
 * Checks interrupts, then checks for transactions.
 **/
 
void ProcessUSBTransactions()
{
    // See if the device is connected yet.
    if(deviceState == DETACHED)
        return;

    // UnSuspend
    // If the USB became active then wake up from suspend
    if(UIRbits.ACTVIF && UIEbits.ACTVIE)
    {
        UCONbits.SUSPND = 0;
        UIEbits.ACTVIE = 0;
        UIRbits.ACTVIF = 0;
    }

    // If we are supposed to be suspended, then don't try performing any processing.
    if(UCONbits.SUSPND == 1)
        return;

    // Process a bus reset
    if (UIRbits.URSTIF && UIEbits.URSTIE)
    {
        UEIR  = 0x00;
        UIR   = 0x00;
        // UEIE : BTSEE — — BTOEE DFN8EE CRC16EE CRC5EE PIDEE
        UEIE  = 0x9f; // 0b10011111
        // UIE : — SOFIE STALLIE IDLEIE TRNIE ACTVIE UERRIE URSTIE
        UIE   = 0x7F; //0x7b; // 0b01111011
        UADDR = 0x00;

        // Set endpoint 0 as a control pipe
        UEP0 = EP_CTRL | HSHK_EN;

        // Flush any pending transactions
        while (UIRbits.TRNIF == 1)
            UIRbits.TRNIF = 0;

        // Enable packet processing
        UCONbits.PKTDIS = 0;

        // Prepare for the Setup stage of a control transfer
        WaitForSetupStage();

        //	remoteWakeup = 0;                 // Remote wakeup is off by default
        selfPowered = 0;                      // Self powered is off by default
        currentConfiguration = 0;             // Clear active configuration
        deviceState = DEFAULT;
    }

    // Suspend all processing until we detect activity on the USB bus
    if (UIRbits.IDLEIF && UIEbits.IDLEIE)
    {
        UIEbits.ACTVIE = 1;
        UIRbits.IDLEIF = 0;
        UCONbits.SUSPND = 1;

        #if defined(__18f25k50) || defined(__18f45k50)

            PIR3bits.USBIF = 0;
            INTCONbits.IOCIF = 0;
            PIE3bits.USBIE = 1;
            INTCONbits.IOCIE = 1;

        #else

            PIR2bits.USBIF = 0;
            INTCONbits.RBIF = 0;
            PIE2bits.USBIE = 1;
            INTCONbits.RBIE = 1;

        #endif
    }

    // Full speed devices get a Start Of Frame (SOF) packet every 1 millisecond.
    // Nothing is currently done with this interrupt (it is simply masked out).
    // TBD: Add a callback routine to do something
    if (UIRbits.SOFIF && UIEbits.SOFIE)
    {
        UIRbits.SOFIF = 0;
    }

    // In response to the code stalling an endpoint.
    if (UIRbits.STALLIF && UIEbits.STALLIE)
    {
        if(UEP0bits.EPSTALL == 1)
        {
            // Prepare for the Setup stage of a control transfer
            WaitForSetupStage();
            UEP0bits.EPSTALL = 0;
        }
        UIRbits.STALLIF = 0;
    }

    // TBD: See where the error came from.
    if (UIRbits.UERRIF && UIEbits.UERRIE)
        UIRbits.UERRIF = 0;// Clear errors

    // Unless we have been reset by the host, no need to keep processing
    if (deviceState < DEFAULT)  // DETACHED, ATTACHED or POWERED
        return;

    /*
     * A transaction has finished.  Try default processing on endpoint 0.
     * This is the starting point for processing a Control Transfer.
     * The code directly follows the sequence of transactions described in
     * the USB spec chapter 5. The only Control Pipe in this firmware is
     * the Default Control Pipe (endpoint 0).
     * Control messages that have a different destination will be discarded.
     */

    if(UIRbits.TRNIF && UIEbits.TRNIE)
    {

        // get encoded number of the last active Endpoint
        byte PID;
        byte end_point = USTAT >> 3;
        word i, bufferSize;

        if (end_point == 0) // Endpoint 0
        {

            if (USTATbits.DIR == OUT)
            {
                // Endpoint 0:out
                // Pull PID from middle of BD0STAT
                PID = (EP_OUT_BD(0).Stat.uc & 0x3C) >> 2;
                
                /*
                * SETUP PID - a transaction is starting
                * Process the Setup stage of a control transfer.
                * This code initializes the flags that let the firmware know
                * what to do during subsequent stages of the transfer.
                * Only Ep0 is handled here.
                */

                if (PID == 0x0D)
                {
                    // Note: Microchip says to turn off the UOWN bit on the IN direction as
                    // soon as possible after detecting that a SETUP has been received.
                    EP_IN_BD(0).Stat.uc &= ~BDS_UOWN;
                    EP_OUT_BD(0).Stat.uc &= ~BDS_UOWN;

                    // Initialize the transfer process
                    ctrlTransferStage = SETUP_STAGE;
                    requestHandled = 0;                   // Default is that request hasn't been handled
                    wCount = 0;                           // No bytes transferred

                    /*
                     * See if this is a standard (as definded in USB chapter 9) request
                     */

                    //byte request = SetupPacket.bRequest;

                    // Not a standard request - don't process here.
                    // Class or Vendor requests have to be handled seperately.
                    if((SetupPacket.bmRequestType & 0x60) != 0x00)
                        return;

                    if (SetupPacket.bRequest == SET_ADDRESS)
                    {
                        // Set the address of the device.  All future requests
                        // will come to that address.  Can't actually set UADDR
                        // to the new address yet because the rest of the SET_ADDRESS
                        // transaction uses address 0.
                        requestHandled = 1;
                        deviceState = ADDRESS;
                        deviceAddress = SetupPacket.wValue0;
                    }

                    else if (SetupPacket.bRequest == GET_DESCRIPTOR)
                    {
                        if(SetupPacket.bmRequestType == 0x80)
                        {
                            //byte descriptorType  = SetupPacket.wValue1;
                            //byte descriptorIndex = SetupPacket.wValue0;

                            if (SetupPacket.wValue1 == DEVICE_DESCRIPTOR)
                            {
                                requestHandled = 1;
                                outPtr = (byte *)&device_descriptor;
                                wCount = sizeof(USB_Device_Descriptor);
                            }

                            else if (SetupPacket.wValue1 == CONFIGURATION_DESCRIPTOR)
                            {
                                requestHandled = 1;
                                outPtr = (byte *)&configuration_descriptor;
                                wCount = configuration_descriptor.Header.wTotalLength;
                            }

                            else if (SetupPacket.wValue1 == STRING_DESCRIPTOR)
                            {
                                requestHandled = 1;
                                //outPtr = (byte *)&string_descriptor[SetupPacket.wValue0];
                                outPtr = string_descriptor[SetupPacket.wValue0];
                                wCount = *outPtr;
                            }
// added on 11/10/13
/*
                            else if (SetupPacket.wValue1 == DEVICE_QUALIFIER_DESCRIPTOR)
                            {
                                requestHandled = 1;
                                // TODO: check if this is needed if not requestHandled is not set to 1 the device will
                                // stall later when the linux kernel requests this descriptor
                                outPtr = (byte *)&device_qualifier_descriptor;
                                wCount = sizeof(USB_Device_Qualifier_Descriptor);
                            }
*/
                        }
                    }

                    else if (SetupPacket.bRequest == SET_CONFIGURATION)
                    {
                        requestHandled = 1;

                        // configure endpoints

                        /*  ------------------------------------------------------------
                            UEP1bits.EPHSHK   = 1;		// EP handshaking on
                            UEP1bits.EPCONDIS = 1;		// control transfers off
                            UEP1bits.EPOUTEN  = 1;		// EP OUT enabled
                            UEP1bits.EPINEN   = 1;		// EP IN enabled
                            ----------------------------------------------------------*/

                        UEP1 = 0b00011110;
                        
                        // for IN
                        // set DTS bit, turn on data togle sync TOGGLE
                        EP_IN_BD(1).Stat.uc  = 0b01000000;

                        // for OUT
                        EP_OUT_BD(1).Cnt  = EP1_BUFFER_SIZE;
                        //EP_OUT_BD(1).ADDR = PTR16(&bootCmd);
                        EP_OUT_BD(1).ADDR = (unsigned long)&bootCmd;
                        // set UOWN bit, SIE owns the buffer
                        EP_OUT_BD(1).Stat.uc = 0b10000000;

                        currentConfiguration = SetupPacket.wValue0;
                        // TBD: ensure the new configuration value is one that
                        // exists in the descriptor.
                        if (currentConfiguration == 0)
                            // If configuration value is zero, device is put in
                            // address state (USB 2.0 - 9.4.7)
                            deviceState = ADDRESS;
                        else
                            // Set the configuration.
                            deviceState = CONFIGURED;

                        // Initialize the endpoints for all interfaces
                        // TBD: Add initialization code here for any additional
                        // interfaces beyond the one used for the HID
                    }

                    else if (SetupPacket.bRequest == GET_CONFIGURATION)
                    {
                        requestHandled = 1;
                        outPtr = (byte*)&currentConfiguration;
                        wCount = 1;
                    }

                    /*
                    else if (SetupPacket.bRequest == GET_STATUS)
                    {
                    //    GetStatus();
                    }

                    else if ((SetupPacket.bRequest == CLEAR_FEATURE) || (request == SET_FEATURE))
                    {
                    //    SetFeature();
                    }
                    */

                    else if (SetupPacket.bRequest == GET_INTERFACE)
                    {
                        // No support for alternate interfaces.  Send
                        // zero back to the host.
                        requestHandled = 1;
                        controlTransferBuffer[0] = 0;
                        outPtr = (byte *)&controlTransferBuffer;
                        wCount = 1;
                    }

                    else if (SetupPacket.bRequest == SET_INTERFACE)
                    {
                        // No support for alternate interfaces - just ignore.
                        requestHandled = 1;
                    }

                    /*
                    else if (SetupPacket.bRequest == SET_DESCRIPTOR) {
                    }
                    else if (SetupPacket.bRequest == SYNCH_FRAME) {
                    }
                    else {
                    }
                    */

                    // TBD: Add handlers for any other classes/interfaces in the device
                    if (!requestHandled)
                    {
                        // If this service wasn't handled then stall endpoint 0
                        EP_OUT_BD(0).Cnt = EP0_BUFFER_SIZE;
                        //EP_OUT_BD(0).ADDR = PTR16(&SetupPacket);
                        EP_OUT_BD(0).ADDR = (unsigned long)&SetupPacket;
                        EP_OUT_BD(0).Stat.uc = BDS_UOWN | BDS_BSTALL;
                        EP_IN_BD(0).Stat.uc = BDS_UOWN | BDS_BSTALL;
                    }

                    else if (SetupPacket.bmRequestType & 0x80)
                    {
                        // Device-to-host
                        if(SetupPacket.wLength < wCount)
                            wCount = SetupPacket.wLength;

                        InDataStage();
                        ctrlTransferStage = DATA_IN_STAGE;
                        // Reset the out buffer descriptor for endpoint 0
                        EP_OUT_BD(0).Cnt = EP0_BUFFER_SIZE;
                        //EP_OUT_BD(0).ADDR = PTR16(&SetupPacket);
                        EP_OUT_BD(0).ADDR = (unsigned long)&SetupPacket;
                        EP_OUT_BD(0).Stat.uc = BDS_UOWN;

                        // Set the in buffer descriptor on endpoint 0 to send data
                        //EP_IN_BD(0).ADDR = PTR16(&controlTransferBuffer);
                        EP_IN_BD(0).ADDR = (unsigned long)&controlTransferBuffer;
                        // Give to SIE, DATA1 packet, enable data toggle checks
                        EP_IN_BD(0).Stat.uc = BDS_UOWN | BDS_DTS | BDS_DTSEN;
                    }

                    else
                    {
                        // Host-to-device
                        ctrlTransferStage = DATA_OUT_STAGE;

                        // Clear the input buffer descriptor
                        EP_IN_BD(0).Cnt = 0;
                        EP_IN_BD(0).Stat.uc = BDS_UOWN | BDS_DTS | BDS_DTSEN;

                        // Set the out buffer descriptor on endpoint 0 to receive data
                        EP_OUT_BD(0).Cnt = EP0_BUFFER_SIZE;
                        //EP_OUT_BD(0).ADDR = PTR16(&controlTransferBuffer);
                        EP_OUT_BD(0).ADDR = (unsigned long)&controlTransferBuffer;
                        // Give to SIE, DATA1 packet, enable data toggle checks
                        EP_OUT_BD(0).Stat.uc = BDS_UOWN | BDS_DTS | BDS_DTSEN;
                    }

                    // Enable SIE token and packet processing
                    UCONbits.PKTDIS = 0;
                }
                
                else if (ctrlTransferStage == DATA_OUT_STAGE)
                {
                    /**
                        Complete the data stage so that all information has
                        passed from host to device before servicing it.
                        Data stage for a Control Transfer that reads data
                        from the host.
                    **/

                    bufferSize = ((0x03 & EP_OUT_BD(0).Stat.uc) << 8) | EP_OUT_BD(0).Cnt;

                    // Accumulate total number of bytes read
                    wCount = wCount + bufferSize;

                    outPtr = (byte*)&controlTransferBuffer;

                    for (i=0;i<bufferSize;i++)
                        *inPtr++ = *outPtr++;

                    // Turn control over to the SIE and toggle the data bit
                    if(EP_OUT_BD(0).Stat.DTS)
                        EP_OUT_BD(0).Stat.uc = BDS_UOWN | BDS_DTSEN;
                    else
                        EP_OUT_BD(0).Stat.uc = BDS_UOWN | BDS_DTS | BDS_DTSEN;
                }

                else
                {
                    // Prepare for the Setup stage of a control transfer
                    WaitForSetupStage();
                }
            }

            else // if(USTATbits.DIR == IN)
            {
                // Endpoint 0:in
                if ((UADDR == 0) && (deviceState == ADDRESS))
                {
                    // TBD: ensure that the new address matches the value of
                    // "deviceAddress" (which came in through a SET_ADDRESS).
                    UADDR = SetupPacket.wValue0;
                    if(UADDR == 0)
                        // If we get a reset after a SET_ADDRESS, then we need
                        // to drop back to the Default state.
                        deviceState = DEFAULT;
                }

                if (ctrlTransferStage == DATA_IN_STAGE)
                {
                    // Start (or continue) transmitting data
                    InDataStage();

                    // Turn control over to the SIE and toggle the data bit
                    if(EP_IN_BD(0).Stat.DTS)
                        EP_IN_BD(0).Stat.uc = BDS_UOWN | BDS_DTSEN;
                    else
                        EP_IN_BD(0).Stat.uc = BDS_UOWN | BDS_DTS | BDS_DTSEN;
                }

                else
                {
                    // Prepare for the Setup stage of a control transfer
                    WaitForSetupStage();
                }

            }
        }

        else //if (end_point == 1) // EndPoint 1
        {

            if (!USTATbits.DIR) // If OUT
                usb_ep_data_out_callback(end_point);
        }

        // Turn off interrupt
        UIRbits.TRNIF = 0;
    }
}
