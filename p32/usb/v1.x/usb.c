/*******************************************************************************
    Title:	USB Pinguino Bootloader
    File:	usb.c
    Descr.: USB routines for PIC32 processors
    Author:	R?gis Blanchot <rblanchot@gmail.com>

    This file is part of Pinguino (http://www.pinguino.cc)
    Released under the LGPL license (http://www.gnu.org/licenses/lgpl.html)
********************************************************************************
 IMPORTANT
    Once an interrupt bit has been set by the USB OTG module (in U1IR, U1EIR or U1OTGIR),
    it must be cleared by software by writing a ?1? to the appropriate bit position
    to clear the interrupt.
    The USB Interrupt bit, USBIF (IFS1<25>), must be cleared before the end of the ISR.
*******************************************************************************/

#ifndef __USB_C
#define __USB_C

#include "p32xxxx.h"
#include "typedefs.h"
#include "boot.h"               // USB Vendor and Product IDs
#include "flash.h"              // ConvertToPhysicalAddress and KVA_TO_PA
#include "usb.h"                // USB Device abstraction layer interface
#include "debug.h"              // Debug functions

#if defined(DEBUG)              // defined in hardware.h
#include "serial.h"             // UART functions
#endif

/* Device Descriptor */

const USB_DEVICE_DESCRIPTOR device_dsc=
{
    sizeof(USB_DEVICE_DESCRIPTOR),              // 0x12 Size of this descriptor in bytes
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

/* Configuration 1 Descriptor */

// Total length in chars of data returned
#define CONFIGURATION_TOTAL_LENGTH (sizeof(USB_CONFIGURATION_DESCRIPTOR) + \
                                    sizeof(USB_INTERFACE_DESCRIPTOR) + \
                                    sizeof(USB_HID_DESCRIPTOR) + \
                                    sizeof(USB_ENDPOINT_DESCRIPTOR) + \
                                    sizeof(USB_ENDPOINT_DESCRIPTOR) )


const UINT8 configDescriptor1[]={

    /* Configuration Descriptor */
    sizeof(USB_CONFIGURATION_DESCRIPTOR),   // 0x09 Size of this descriptor in bytes
    USB_DESCRIPTOR_CONFIGURATION,           // CONFIGURATION descriptor type
    CONFIGURATION_TOTAL_LENGTH,0x00,        // Total length of data for this cfg
    USB_MAX_NUM_INT,                        // Number of interfaces in this cfg
    1,                                      // Index value of this configuration
    0,                                      // Configuration string index
    _DEFAULT|_SELF,                         // Attributes, see usb_ch9.h
    50,                                     // 100mA max power consumption (2X mA)

        /* Interface Descriptor */
        sizeof(USB_INTERFACE_DESCRIPTOR),   // 0x09 Size of this descriptor in bytes
        USB_DESCRIPTOR_INTERFACE,           // INTERFACE descriptor type
        0,                                  // Interface Number
        0,                                  // Alternate Setting Number
        2,                                  // Number of endpoints in this intf
        HID_INTF,                           // Class code
        0,                                  // Subclass code
        0,                                  // Protocol code
        0,                                  // Interface string index

            /* HID Class-Specific Descriptor */
            sizeof(USB_HID_DESCRIPTOR),     // 0x09
            DSC_HID,                        // HID descriptor type
            0x11,0x01,                      // HID Spec Release Number in BCD format (1.11)
            0x00,                           // Country Code (0x00 for Not supported)
            HID_NUM_OF_DSC,                 // Number of class descriptors
            DSC_RPT,                        // Report descriptor type
            HID_RPT01_SIZE,0x00,            // sizeof(hid_rpt01)= Size of the report descriptor

            /* Endpoint Descriptor */
            sizeof(USB_ENDPOINT_DESCRIPTOR),// 0x07
            USB_DESCRIPTOR_ENDPOINT,        // Endpoint Descriptor
            HID_EP | _EP_IN,                // EndpointAddress
            _INTERRUPT,                     // Endpoint Transfer Type
            HID_INT_IN_EP_SIZE,0x00,        // size
            0x01,                           // Interval

            /* Endpoint Descriptor */
            sizeof(USB_ENDPOINT_DESCRIPTOR),// 0x07
            USB_DESCRIPTOR_ENDPOINT,        // Endpoint Descriptor
            HID_EP | _EP_OUT,               // EndpointAddress
            _INTERRUPT,                     // Endpoint Transfer Type
            HID_INT_OUT_EP_SIZE,0x00,       // size
            0x01                            // Interval
};

// Language code string descriptor
const USB_STRING_INIT(1) sd000 = {
    sizeof(sd000),USB_DESCRIPTOR_STRING,
    {0x0409}
};

// Manufacturer string descriptor
const USB_STRING_INIT(9) sd001 = {
    sizeof(sd001),
    USB_DESCRIPTOR_STRING,
    {'S','e','a','I','c','e','L','a','b'}
};

// Product string descriptor
const USB_STRING_INIT(26) sd002 = {
    sizeof(sd002),
    USB_DESCRIPTOR_STRING,
    { 'P','i','n','g','u','i','n','o',' ',
      '(','w','w','w','.','p','i','n','g','u','i','n','o','.','c','c',')' }
};

// Serial Number string descriptor
const USB_STRING_INIT(10) sd003 = {
    sizeof(sd003),
    USB_DESCRIPTOR_STRING,
    {'R','.','B','l','a','n','c','h','o','t'}
};

//Class specific descriptor - HID
const struct{UINT8 report[HID_RPT01_SIZE];}hid_rpt01={{
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
}};

//Array of configuration descriptors
const UINT8 *const USB_CD_Ptr[]=
{
    (const UINT8 *const)&configDescriptor1
};

//Array of string descriptors
const UINT8 *const USB_SD_Ptr[]=
{
    (const UINT8 *const)&sd000,
    (const UINT8 *const)&sd001,
    (const UINT8 *const)&sd002,
    (const UINT8 *const)&sd003
};

UINT8 idle_rate;
UINT8 active_protocol;   // [0] Boot Protocol [1] Report Protocol

// defined in main.c
extern USB_VOLATILE USB_DEVICE_STATE USBDeviceState;

USB_VOLATILE UINT8 USBActiveConfiguration;
USB_VOLATILE UINT8 USBAlternateInterface[USB_MAX_NUM_INT];
USB_VOLATILE UINT8 shortPacketStatus;
USB_VOLATILE UINT8 controlTransferState;
USB_VOLATILE UINT8 endpoint_number;
USB_VOLATILE UINT8 USBStatusStageTimeoutCounter;

USB_VOLATILE UINT8 *pDst;

USB_VOLATILE IN_PIPE inPipe;
USB_VOLATILE OUT_PIPE outPipe;

USB_VOLATILE BOOL USBCableConnected = FALSE;      // Current VBUS level
USB_VOLATILE BOOL RemoteWakeup;
//USB_VOLATILE BOOL USBBusIsSuspended;
USB_VOLATILE BOOL BothEP0OutUOWNsSet;

USB_VOLATILE USTAT_FIELDS USTATcopy;

USB_VOLATILE EP_STATUS ep_data_in[USB_MAX_EP_NUMBER+1];
USB_VOLATILE EP_STATUS ep_data_out[USB_MAX_EP_NUMBER+1];

volatile BDT_ENTRY *pBDTEntryEP0OutCurrent;
volatile BDT_ENTRY *pBDTEntryEP0OutNext;
volatile BDT_ENTRY *pBDTEntryOut[USB_MAX_EP_NUMBER+1];
volatile BDT_ENTRY *pBDTEntryIn[USB_MAX_EP_NUMBER+1];

volatile BOOL USBDeferStatusStagePacket;
volatile BOOL USBStatusStageEnabledFlag1;
volatile BOOL USBStatusStageEnabledFlag2;
volatile BOOL USBDeferINDataStagePackets;
volatile BOOL USBDeferOUTDataStagePackets;

/********************************************************************
 * Section B: EP0 Buffer Space
 *******************************************************************/

volatile CTRL_TRF_SETUP SetupPkt;           // 8-byte only
volatile UINT8 CtrlTrfData[USB_EP0_BUFF_SIZE];

/********************************************************************
 * Section C: non-EP0 Buffer Space
 *******************************************************************/
// Can provide compile time option to do software pingpong
#if 1
volatile unsigned char hid_report_out[HID_INT_OUT_EP_SIZE];
volatile unsigned char hid_report_in[HID_INT_IN_EP_SIZE];
#endif

/********************************************************************
 * Section A: Buffer Descriptor Table
 * - 256 bytes max.  Actual size depends on number of endpoints enabled and
 *   the ping pong buffering mode.
 * - USB_MAX_EP_NUMBER is defined in usb_device.h
 *******************************************************************/

volatile BDT_ENTRY BDT[(USB_MAX_EP_NUMBER + 1) * 4] __attribute__ ((aligned (512)));

/****************************************************************************
 * Initializes the device stack it in the default state
 ***************************************************************************/

void USBDeviceInit(void)
{
    UINT8 i;
    UINT32 BDTBaseAddress = ConvertToPhysicalAddress(&BDT);

    // Clear all of the BDT entries
    for(i=0; i<(sizeof(BDT)/sizeof(BDT_ENTRY)); i++)
        BDT[i].Val = 0x00;

    // Initialize USB stack software state variables
    inPipe.info.Val = 0;
    outPipe.info.Val = 0;
    outPipe.wCount.Val = 0;

    // Set flags to TRUE, so the USBCtrlEPAllowStatusStage() function knows not to
    // try and arm a status stage, even before the first control transfer starts.
    USBStatusStageEnabledFlag1 = TRUE;
    USBStatusStageEnabledFlag2 = TRUE;
    // Initialize other flags
    USBDeferINDataStagePackets = FALSE;
    USBDeferOUTDataStagePackets = FALSE;
    //USBBusIsSuspended = FALSE;

    // Initialize all pBDTEntryIn[] and pBDTEntryOut[] pointers to NULL,
    // so they don't get used inadvertently.
    for(i = 0; i < (UINT8)(USB_MAX_EP_NUMBER+1); i++)
    {
        pBDTEntryIn[i]     = 0;
        pBDTEntryOut[i]    = 0;
        ep_data_in[i].Val  = 0;
        ep_data_out[i].Val = 0;
    }

    //Get ready for the first packet
    pBDTEntryIn[0] = (volatile BDT_ENTRY*)&BDT[EP0_IN_EVEN];

    // Clear active configuration
    USBActiveConfiguration = 0;

    // -------------------------------------------------------------------------
    // ---REGISTERS-------------------------------------------------------------
    // -------------------------------------------------------------------------

    // Init. OTG module Off
    U1PWRC = 0;
    U1OTGCON = 0;
    U1CON = 0;

    // Power up the USB module
    U1PWRCSET = _U1PWRC_USBPWR_MASK;

    // Set the address of the BDT
    U1BDTP1 = (BDTBaseAddress & 0x0000FF00) >> 8;
    U1BDTP2 = (BDTBaseAddress & 0x00FF0000) >> 16;
    U1BDTP3 = (BDTBaseAddress & 0xFF000000) >> 24;

    // Initialize all endpoints
    //U1EP0 = 0x00;
    //U1EP1 = 0x00;

    // Reset to default address
    U1ADDR = 0x00;

    // Configure the module
    U1CNFG1 = 0;

    // Reset ping-pong buffer pointers to EVEN
    // Allow RESUME signaling
    //U1CONCLR = _U1CON_PPBRST_MASK; // | _U1CON_RESUME_MASK;

    // Enable packet processing
    //USBPacketDisable = 0;
    //U1CONCLR = _U1CON_PKTDIS_MASK;

    // Enable general USB interrupts
    //IEC1SET = _IEC1_USBIE_MASK;
    //IFS1CLR = _IFS1_USBIF_MASK;

    // Enable USB interrupts
    U1IE    = 0x00; // _U1IE_UERRIE_MASK;
    U1EIE   = 0x00; // all error interrupts
    U1OTGIE = _U1OTGIE_SESVDIE_MASK;

    // Clear USB interrupts flags
    U1IR    = 0xFF; // _U1IR_UERRIF_MASK;
    U1EIR   = 0xFF; // all error flags
    U1OTGIR = _U1OTGIR_SESVDIF_MASK;

    // Indicate that we are now in the detached state
    USBDeviceState = DETACHED_STATE;
}

/***********************************************************************
 * Check if USB cable is plugged or unplugged
 **********************************************************************/

void USBCheckCable(void)
{
    BOOL VBUSLocal = (U1OTGSTATbits.SESVD != 0);

    #ifdef DEBUG
    //SerialPrint("USB check cable\r\n");
    #endif

    if (VBUSLocal != USBCableConnected)
    {
        USBCableConnected = VBUSLocal;

        if (USBCableConnected)
        {
            #ifdef DEBUG
            SerialPrint("> USB cable plugged\r\n");
            #endif

            #if 0
            // Enable power to module
            while (U1PWRCbits.USBBUSY);
            U1PWRCbits.USBPWR = 1;

            // Wait until USB module and supporting circuitry is enabled
            // All USB-related modules should be initialised before.
            //while (!(U1CON & _U1CON_USBEN_MASK))
            while (!U1CONbits.USBEN)
                U1CONSET = _U1CON_USBEN_MASK;
            #else
            // Enable USB module
            while (!U1CONbits.USBEN)
                U1CONSET = _U1CON_USBEN_MASK;

            // Power up the USB module
            U1PWRCSET = _U1PWRC_USBPWR_MASK;
            #endif

            // Enable power to the D+/D- pull-up resistors
            U1OTGCONSET = _U1OTGCON_DMPULUP_MASK | _U1OTGCON_DPPULUP_MASK;
                     //_U1OTGCON_VBUSDIS_MASK | _U1OTGCON_VBUSON_MASK;

            // Enable USB interrupts
            //U1OTGIE = _U1OTGIE_ACTVIE_MASK;
            //U1OTGIR = _U1OTGIR_ACTVIF_MASK;
            U1IE = _U1IE_URSTIE_MASK | _U1IE_IDLEIE_MASK;
            U1IR = _U1IR_URSTIF_MASK | _U1IR_IDLEIF_MASK;

            // moved to the powered state
            USBDeviceState = POWERED_STATE;
        }
        else
        {
            #ifdef DEBUG
            SerialPrint("> USB cable unplugged\r\n");
            #endif

            // Disable USB interrupts
            U1IE  = 0;
            //U1EIE = 0;

            // Remove power from the D+/D- pull-up resistors
            U1OTGCONCLR = _U1OTGCON_DMPULUP_MASK | _U1OTGCON_DPPULUP_MASK;

            // Disable USB module
            U1CON = 0;

            // Suspend the USB module
            //U1PWRCCLR = _U1PWRC_USBPWR_MASK;
            U1PWRC = _U1PWRC_USUSPEND_MASK;

            // Back to the detached state
            USBDeviceState = DETACHED_STATE;
        }
    }
}

/****************************************************************************
    This function is the main state machine of the USB device side stack.
    This function should be called periodically to receive and transmit
    packets through the stack.  This function should be called  preferably
    once every 100us during the enumeration process. After the enumeration
    process this function still needs to be called periodically to respond to
    various situations on the bus but is more relaxed in its time requirements.
    This function should also be called at least as fast as the OUT data
    expected from the PC.
  ***************************************************************************/

void USBDeviceTasks(void)
{
    UINT8 i;

    #ifdef DEBUG
    //SerialPrint("U1IE=0b");
    //SerialPrintNumber(U1IE, 2);
    //SerialPrint("\r\n");
    #endif

   /*
    * Check if the USB cable is plugged
    */

    if (U1OTGIE & _U1OTGIE_SESVDIE_MASK)
    {
        if (U1OTGIR & _U1OTGIR_SESVDIF_MASK)
        {
            #ifdef DEBUG
            //SerialPrint(UART,"SESSION CHANGE DETECTED\r\n");
            #endif

            // Power up the USB module
            // and enable USB reset and idle interrupts
            // If the device is connected
            USBCheckCable();

            // Clear SESVD flag
            U1OTGIR |= _U1OTGIR_SESVDIF_MASK;
        }
    }

    // Pointless to continue servicing if the device is detached.
    if (USBDeviceState == DETACHED_STATE)
    {
        // At this point the PIC can also go into sleep, idle
        // or switch to a slower clock.
        return;
    }

    /*
     * Task A: Service USB Activity Interrupt
     */

    #if 1
    // If the USB became active then wake up from suspend
    //if ( (U1OTGIRbits.ACTVIF) && (U1OTGIEbits.ACTVIE) )
    if (U1OTGIE & _U1OTGIE_ACTVIE_MASK)
    {
        if (U1OTGIR & _U1OTGIR_ACTVIF_MASK)
        {
            #ifdef DEBUG
            //SerialPrint("USB DEV. WOKE UP\r\n");
            #endif

            // Enable USB reset and idle interrupts
            // because once the USUSPEND is clear
            // the URSTIF bit will be asserted.
            U1IE = _U1IE_URSTIE_MASK | _U1IE_IDLEIE_MASK;
            U1IR = _U1IR_URSTIF_MASK | _U1IR_IDLEIF_MASK;

            // Exit from supsend mode
            //U1PWRCbits.USUSPEND = 0;
            U1PWRCCLR = _U1PWRC_USUSPEND_MASK;

            U1OTGIR |= _U1OTGIR_ACTVIF_MASK;
        }
    }
    #endif

    #if 0
    //if (U1IRbits.RESUMEIF && U1IEbits.RESUMEIE)
    if (U1IE & _U1IE_RESUMEIE_MASK)
    {
        if (U1IR & _U1IR_RESUMEIF_MASK)
        {
            #ifdef DEBUG
            SerialPrint("USB DEV. RESUMED\r\n");
            #endif

            // Exit from supsend mode
            U1PWRCbits.USUSPEND = 0;

            // Clear RESUME interrupt
            U1IR |= _U1IR_RESUMEIF_MASK;
        }
    }
    #endif

    #if 1
    // Pointless to continue servicing if the device is in suspend mode.
    if (U1PWRC & _U1PWRC_USUSPEND_MASK)
    {
        #ifdef DEBUG
        SerialPrint("> USB module suspended\r\n");
        #endif

        //IFS1CLR = _IFS1_USBIF_MASK;
        return;
    }
    #endif

    /*
     * Task B: Service USB Bus Reset Interrupt.
     * When the host wants to start communicating with a device it will start
     * by applying a 'Reset' condition which sets the device to its default
     * unconfigured state.
     * The Reset condition involves the host pulling down both data lines to low
     * levels (SE0) for at least 10 ms. The device may recognise the reset
     * condition after 2.5 us.
     * This 'Reset' should not be confused with a micro-controller power-on
     * type reset. It is a USB protocol reset to ensure that the device USB
     * signaling starts from a known state.
     * When bus reset is received during suspend, ACTVIF will be set first,
     * once the UCONbits.SUSPND is clear, then the URSTIF bit will be asserted.
     * This is why URSTIF is checked after ACTVIF.
     */

    //if (U1IRbits.URSTIF && U1IEbits.URSTIE)
    if (U1IE & _U1IE_URSTIE_MASK)
    {
        if (U1IR & _U1IR_URSTIF_MASK)
        {
            #ifdef DEBUG
            //SerialPrint("USB DEV. RESETED\r\n");
            #endif

            // Sets the device to its default unconfigured state.
            USBDeviceInit();

            // Initialize EP0 as a Ctrl EP
            U1EP0 = EP_CTRL | USB_HANDSHAKE_ENABLED;

            //Prepare for the first SETUP on EP0 OUT
            BDT[EP0_OUT_EVEN].ADR = ConvertToPhysicalAddress(&SetupPkt);
            BDT[EP0_OUT_EVEN].CNT = USB_EP0_BUFF_SIZE;
            BDT[EP0_OUT_EVEN].STAT.Val = _USIE|_DAT0|_BSTALL;

            // Flush any pending transactions
            while (U1IR & _U1IR_TRNIF_MASK)
                U1IR |= _U1IR_TRNIF_MASK;

            // Enable packet processing (PKTDIS=0)
            // Stop trying to reset ping pong buffer pointers
            // *** MUST BE CLEARED ***
            U1CONCLR = _U1CON_PKTDIS_MASK | _U1CON_PPBRST_MASK; // | _U1CON_RESUME_MASK;

            // Interrupts we want to check
            U1IE = _U1IE_IDLEIE_MASK    | _U1IE_TRNIE_MASK  |
                   _U1IE_STALLIE_MASK   | _U1IE_UERRIE_MASK;
                   //_U1IE_URSTIE_MASK;
                   //_U1IE_SOFIE_MASK;
                   //_U1IE_RESUMEIE_MASK;
            U1IR = _U1IR_IDLEIF_MASK    | _U1IR_TRNIF_MASK  |
                   _U1IR_STALLIF_MASK   | _U1IR_UERRIF_MASK;
                   //_U1IR_URSTIF_MASK;
                   //_U1IR_SOFIF_MASK;
                   //_U1IR_RESUMEIF_MASK;

            U1EIE   = 0xFF;
            U1EIR   = 0xFF;

            // Move to default state
            USBDeviceState = DEFAULT_STATE;

            // Clear RESET interrupt
            U1IR |= _U1IR_URSTIF_MASK;
        }
    }

    /*
     * Task C: Service other USB interrupts
     */

    #if 1
    //if (U1IRbits.IDLEIF && U1IEbits.IDLEIE)
    if (U1IE & _U1IE_IDLEIE_MASK)
    {
        if (U1IR & _U1IR_IDLEIF_MASK)
        {
            #ifdef DEBUG
            //SerialPrint("USB DEV. SUSPENDED\r\n");
            #endif

            // At this point the PIC can also go into sleep, idle
            // or switch to a slower clock.
            U1PWRC |= _U1PWRC_USUSPEND_MASK;

            // Interrupts we want to check
            U1IE = _U1IE_URSTIE_MASK; // | _U1IE_IDLEIE_MASK;
            U1IR = 0xFF;

            U1OTGIE = _U1OTGIE_ACTVIE_MASK | _U1OTGIE_SESVDIE_MASK;
            U1OTGIR = _U1OTGIR_ACTVIF_MASK | _U1OTGIR_SESVDIF_MASK;

            // Pointless to continue servicing if the device is in suspend mode.
            //IFS1CLR = _IFS1_USBIF_MASK;
            return;
        }
    }
    #endif

    #if 0
    if (U1IR & _U1IR_SOFIF_MASK)
    {
        if (U1IE & _U1IE_SOFIE_MASK)
        {
            #ifdef DEBUG
            //SerialPrint("USB START OF FRAME\r\n");
            #endif
            U1IR |= _U1IR_SOFIF_MASK;
        }
    }
    #endif

    #if 1
    //if (U1IRbits.STALLIF && U1IEbits.STALLIE)
    if (U1IE & _U1IE_STALLIE_MASK)
    {
        if (U1IR & _U1IR_STALLIF_MASK)
        {
            #ifdef DEBUG
            //SerialPrint("USB DEV. STALLED\r\n");
            #endif

            if(U1EP0 & _U1EP0_EPSTALL_MASK)
            {
                // UOWN - if 0, owned by CPU, if 1, owned by SIE
                if ( (pBDTEntryEP0OutCurrent->STAT.Val == _USIE)  &&
                     (pBDTEntryIn[0]->STAT.Val == (_USIE|_BSTALL)) )
                {
                    // Set ep0Bo to stall also
                    pBDTEntryEP0OutCurrent->STAT.Val = _USIE|_DAT0|_DTSEN|_BSTALL;
                }
                // Clear stall status
                U1EP0CLR = _U1EP0_EPSTALL_MASK;
            }
            U1IR |= _U1IR_STALLIF_MASK;
        }
    }
    #endif

    #if 1
    //if(USBErrorIF && USBErrorIE)
    //if (U1IRbits.UERRIF && U1IEbits.UERRIE)
    if (U1IE & _U1IE_UERRIE_MASK)
    {
        if ( U1IR & _U1IR_UERRIF_MASK)
        {
            #ifdef DEBUG
            SerialPrint("ERROR CODE 0x");
            SerialPrintNumber(U1EIR, 2);
            SerialPrint("\r\n");
            #endif

            // Clear Error interrupt
            U1EIR = 0xFF;               // This clears UERRIF
            U1IR |= _U1IR_UERRIF_MASK;
        }
    }
    #endif

    /*
     * Pointless to continue servicing if the host has not sent a bus reset.
     */

    if(USBDeviceState < DEFAULT_STATE)
    {
        //IFS1CLR = _IFS1_USBIF_MASK;
        return;
    }

    /*
     * Task D: Servicing USB Transaction Complete Interrupt
     */

    if (U1IE & _U1IE_TRNIE_MASK)
    {
        //Drain or deplete the USAT FIFO entries.
        //If the USB FIFO ever gets full, USB bandwidth utilization can be compromised,
        //and the device won't be able to receive SETUP packets.
        //for(i = 0; i < 4; i++)
        //{
            if (U1IR & _U1IR_TRNIF_MASK)
            {
                #ifdef DEBUG
                //SerialPrint("USB TRANSACTION COMPLETED\r\n");
                #endif

                //Save and extract USTAT register info.  Will use this info later.
                USTATcopy.Val = U1STAT;
                endpoint_number = USTATcopy.endpoint_number;

                //Keep track of the hardware ping pong state for endpoints other
                //than EP0, if ping pong buffering is enabled.
                if (USTATcopy.direction == OUT_FROM_HOST)
                    ep_data_out[endpoint_number].bits.ping_pong_state ^= 1;
                else
                    ep_data_in[endpoint_number].bits.ping_pong_state ^= 1;

                //USBCtrlEPService only services transactions over EP0.
                //It ignores all other EP transactions.
                if(endpoint_number == 0)
                    USBCtrlEPService();

                // Clear the TRNIF interrupt
                U1IR |= _U1IR_TRNIF_MASK;
            }
        //}
    }

    //IFS1CLR = _IFS1_USBIF_MASK;

}//end of USBDeviceTasks()

/********************************************************************
 * Function:        void USBCtrlEPService(void)
 *
 * PreCondition:    USTAT is loaded with a valid endpoint address.
 *
 * Overview:        USBCtrlEPService checks for three transaction
 *                  types that it knows how to service and services
 *                  them:
 *                  1. EP0 SETUP
 *                  2. EP0 OUT
 *                  3. EP0 IN
 *                  It ignores all other types (i.e. EP1, EP2, etc.)
 *******************************************************************/

static void USBCtrlEPService(void)
{
    //If we get to here, that means a successful transaction has just occurred
    //on EP0.  This means "progress" has occurred in the currently pending
    //control transfer, so we should re-initialize our timeout counter.

    //Check if the last transaction was on EP0 OUT endpoint
    if((USTATcopy.Val & USTAT_EP0_PP_MASK) == USTAT_EP0_OUT_EVEN)
    {
        //Point to the EP0 OUT buffer of the buffer that arrived
        pBDTEntryEP0OutCurrent = (volatile BDT_ENTRY*)&BDT[(USTATcopy.Val & USTAT_EP_MASK)>>2];
	//Set the next out to the current out packet
        pBDTEntryEP0OutNext = pBDTEntryEP0OutCurrent;
	//Toggle it to the next ping pong buffer (if applicable)
        ((BYTE_VAL*)&pBDTEntryEP0OutNext)->Val ^= USB_NEXT_EP0_OUT_PING_PONG;

        //If the current EP0 OUT buffer has a SETUP packet
        if(pBDTEntryEP0OutCurrent->STAT.PID == PID_SETUP)
        {
            UINT8 setup_cnt;

            //The SETUP transaction data may have gone into the the CtrlTrfData
            //buffer, or elsewhere, depending upon how the BDT was prepared
            //before the transaction.  Therefore, we should copy the data to the
            //SetupPkt buffer so it can be processed correctly by USBCtrlTrfSetupHandler().
            for(setup_cnt = 0; setup_cnt < 8; setup_cnt++) //SETUP data packets always contain exactly 8 bytes.
            {
                *(UINT8*)((UINT8*)&SetupPkt + setup_cnt) = *(UINT8*)ConvertRAMToVirtualAddress(pBDTEntryEP0OutCurrent->ADR);
                pBDTEntryEP0OutCurrent->ADR++;
            }
            pBDTEntryEP0OutCurrent->ADR = ConvertToPhysicalAddress(&SetupPkt);

            //Handle the control transfer (parse the 8-byte SETUP command and figure out what to do)
            USBCtrlTrfSetupHandler();
        }
        else
        {
            //Handle the DATA transfer
            USBCtrlTrfOutHandler();
        }
    }
    else if((USTATcopy.Val & USTAT_EP0_PP_MASK) == USTAT_EP0_IN)
    {
        //Otherwise the transmission was and EP0 IN so take care of the IN transfer
        USBCtrlTrfInHandler();
    }

}//end USBCtrlEPService

/********************************************************************
 * Function:        void USBCtrlTrfSetupHandler(void)
 *
 * PreCondition:    SetupPkt buffer is loaded with valid USB Setup Data
 *
 * Overview:        This routine is a task dispatcher and has 3 stages.
 *                  1. It initializes the control transfer state machine.
 *                  2. It calls on each of the module that may know how to
 *                     service the Setup Request from the host.
 *                     Module Example: USBD, HID, CDC, MSD, ...
 *                     A callback function, USBCBCheckOtherReq(),
 *                     is required to call other module handlers.
 *                  3. Once each of the modules has had a chance to check if
 *                     it is responsible for servicing the request, stage 3
 *                     then checks direction of the transfer to determine how
 *                     to prepare EP0 for the control transfer.
 *                     Refer to USBCtrlEPServiceComplete() for more details.
 *
 * Note:            Microchip USB Firmware has three different states for
 *                  the control transfer state machine:
 *                  1. WAIT_SETUP
 *                  2. CTRL_TRF_TX (device sends data to host through IN transactions)
 *                  3. CTRL_TRF_RX (device receives data from host through OUT transactions)
 *                  Refer to firmware manual to find out how one state
 *                  is transitioned to another.
 *
 *                  A Control Transfer is composed of many USB transactions.
 *                  When transferring data over multiple transactions,
 *                  it is important to keep track of data source, data
 *                  destination, and data count. These three parameters are
 *                  stored in pSrc,pDst, and wCount. A flag is used to
 *                  note if the data source is from ROM or RAM.
 *
 *******************************************************************/

static void USBCtrlTrfSetupHandler(void)
{
    #if defined(DEBUG)
    //SerialPrint("> USBCtrlTrfSetupHandler\r\n");
    #endif

    //--------------------------------------------------------------------------
    //1. Re-initialize state tracking variables related to control transfers.
    //--------------------------------------------------------------------------
    shortPacketStatus = SHORT_PKT_NOT_USED;
    USBDeferStatusStagePacket = FALSE;
    USBDeferINDataStagePackets = FALSE;
    USBDeferOUTDataStagePackets = FALSE;
    BothEP0OutUOWNsSet = FALSE;
    controlTransferState = WAIT_SETUP;

    //Abandon any previous control transfers that might have been using EP0.
    //Ordinarily, nothing actually needs abandoning, since the previous control
    //transfer would have completed successfully prior to the host sending the next
    //SETUP packet.  However, in a timeout error case, or after an EP0 STALL event,
    //one or more UOWN bits might still be set.  If so, we should clear the UOWN bits,
    //so the EP0 IN/OUT endpoints are in a known inactive state, ready for re-arming
    //by the class request handler that will be called next.

    pBDTEntryIn[0]->STAT.Val &= ~(_USIE);
    //USBAdvancePingPongBuffer(&pBDTEntryIn[0]);
    ((BYTE_VAL*)&pBDTEntryIn[0])->Val ^= USB_NEXT_EP0_IN_PING_PONG;

    pBDTEntryIn[0]->STAT.Val &= ~(_USIE);
    //USBAdvancePingPongBuffer(&pBDTEntryIn[0]);
    ((BYTE_VAL*)&pBDTEntryIn[0])->Val ^= USB_NEXT_EP0_IN_PING_PONG;

    pBDTEntryEP0OutNext->STAT.Val &= ~(_USIE);

    inPipe.info.Val = 0;
    inPipe.wCount.Val = 0;
    outPipe.info.Val = 0;
    outPipe.wCount.Val = 0;

    //--------------------------------------------------------------------------
    //2. Now find out what was in the SETUP packet, and begin handling the request.
    //--------------------------------------------------------------------------

    //Check for standard USB "Chapter 9" requests.
    USBCheckStdRequest();
    //Check for USB device class specific requests
    USBCheckHIDRequest();
    
    //--------------------------------------------------------------------------
    //3. Re-arm EP0 IN and EP0 OUT endpoints, based on the control transfer in
    //   progress.  If one of the above handlers (in step 2) knew how to process
    //   the request, it will have set one of the inPipe.info.bits.busy or
    //   outPipe.info.bits.busy flags = 1.  This lets the
    //   USBCtrlEPServiceComplete() function know how and which endpoints to
    //   arm.  If both info.bits.busy flags are = 0, then no one knew how to
    //   process the request.  In this case, the default behavior will be to
    //   perform protocol STALL on EP0.
    //--------------------------------------------------------------------------
    USBCtrlEPServiceComplete();

}//end USBCtrlTrfSetupHandler

/******************************************************************************
 * Function:        void USBCtrlTrfOutHandler(void)
 *
 * Overview:        This routine handles an OUT transaction according to
 *                  which control transfer state is currently active.
 *
 * Note:            Note that if the the control transfer was from
 *                  host to device, the session owner should be notified
 *                  at the end of each OUT transaction to service the
 *                  received data.
 *****************************************************************************/

static void USBCtrlTrfOutHandler(void)
{
    #if defined(DEBUG)
    //SerialPrint("> USBCtrlTrfOutHandler\r\n");
    #endif

    if(controlTransferState == CTRL_TRF_RX)
    {
        USBCtrlTrfRxService();	//Copies the newly received data into the appropriate buffer and configures EP0 OUT for next transaction.
    }
    else //In this case the last OUT transaction must have been a status stage of a CTRL_TRF_TX (<setup><in><in>...<OUT>  <-- this last OUT just occurred as the status stage)
    {
        //If the status stage is complete, this means we are done with the
        //control transfer.  Go back to the idle "WAIT_SETUP" state.
        controlTransferState = WAIT_SETUP;

        //Prepare EP0 OUT for the next SETUP transaction, however, it may have
        //already been prepared if ping-pong buffering was enabled on EP0 OUT,
        //and the last control transfer was of direction: device to host, see
        //USBCtrlEPServiceComplete().  If it was already prepared, do not want
        //to do anything to the BDT.
        if(BothEP0OutUOWNsSet == FALSE)
        {
            pBDTEntryEP0OutNext->CNT = USB_EP0_BUFF_SIZE;
            pBDTEntryEP0OutNext->ADR = ConvertToPhysicalAddress(&SetupPkt);
            pBDTEntryEP0OutNext->STAT.Val = _USIE|_DAT0|_DTSEN|_BSTALL;
        }
        else
        {
            BothEP0OutUOWNsSet = FALSE;
        }
    }
}

/******************************************************************************
 * Function:        void USBCtrlTrfInHandler(void)
 *
 * Overview:        This routine handles an IN transaction according to
 *                  which control transfer state is currently active.
 *
 * Note:            A Set Address Request must not change the acutal address
 *                  of the device until the completion of the control
 *                  transfer. The end of the control transfer for Set Address
 *                  Request is an IN transaction. Therefore it is necessary
 *                  to service this unique situation when the condition is
 *                  right. Macro mUSBCheckAdrPendingState is defined in
 *                  usb9.h and its function is to specifically service this
 *                  event.
 *****************************************************************************/

static void USBCtrlTrfInHandler(void)
{
    UINT8 lastDTS;

    #if defined(DEBUG)
    //SerialPrint("> USBCtrlTrfInHandler\r\n");
    #endif

    lastDTS = pBDTEntryIn[0]->STAT.DTS;

    //switch to the next ping pong buffer
    //USBAdvancePingPongBuffer(&pBDTEntryIn[0]);
    ((BYTE_VAL*)&pBDTEntryIn[0])->Val ^= USB_NEXT_EP0_IN_PING_PONG;

    //Must check if in ADR_PENDING_STATE.  If so, we need to update the address
    //now, since the IN status stage of the control transfer has evidently
    //completed successfully.
    if (USBDeviceState == ADR_PENDING_STATE)
    {
        U1ADDR = SetupPkt.bDevADR.Val;
        if(U1ADDR != 0)
            USBDeviceState = ADDRESS_STATE;
        else
            USBDeviceState = DEFAULT_STATE;
    }

    if(controlTransferState == CTRL_TRF_TX)
    {
        // RB 07-10-2014 : CtrlTrfData to &CtrlTrfData
        pBDTEntryIn[0]->ADR = ConvertToPhysicalAddress(CtrlTrfData);
        USBCtrlTrfTxService();

        //Check if we have already sent a short packet.  If so, configure
        //the endpoint to STALL in response to any further IN tokens (in the
        //case that the host erroneously tries to receive more data than it
        //should).
        if(shortPacketStatus == SHORT_PKT_SENT)
        {
            // If a short packet has been sent, don't want to send any more,
            // stall next time if host is still trying to read.
            pBDTEntryIn[0]->STAT.Val = _USIE|_BSTALL;
        }
        else
        {
            if(lastDTS == 0)
                pBDTEntryIn[0]->STAT.Val = _USIE|_DAT1|_DTSEN;
            else
                pBDTEntryIn[0]->STAT.Val = _USIE|_DAT0|_DTSEN;
        }
    }
    else // must have been a CTRL_TRF_RX status stage IN packet (<setup><out><out>...<IN>  <-- this last IN just occurred as the status stage)
    {
        //if someone is still expecting data from the control transfer
        //  then make sure to terminate that request and let them know that
        //  they are done
        if(outPipe.info.bits.busy == 1)
        {
            if(outPipe.pFunc != 0)
                outPipe.pFunc();

            outPipe.info.bits.busy = 0;
        }

        controlTransferState = WAIT_SETUP;
        //Don't need to arm EP0 OUT here.  It was already armed by the last <out> that
        //got processed by the USBCtrlTrfRxService() handler.
    }
}

/********************************************************************
 * Function:        void USBCheckStdRequest(void)
 *
 * Overview:        This routine checks the setup data packet to see
 *                  if it knows how to handle it
 *******************************************************************/

static void USBCheckStdRequest(void)
{
    if(SetupPkt.RequestType != USB_SETUP_TYPE_STANDARD_BITFIELD)
        return;

    switch(SetupPkt.bRequest)
    {
        #if 0
        case USB_REQUEST_GET_STATUS:
            #if defined(DEBUG)
            SerialPrint("GET_STATUS\r\n");
            #endif
            USBStdGetStatusHandler();
            break;

        case USB_REQUEST_CLEAR_FEATURE:
        case USB_REQUEST_SET_FEATURE:
            #if defined(DEBUG)
            SerialPrint("SET_FEATURE\r\n");
            #endif
            USBStdFeatureReqHandler();
            break;
        #endif

        case USB_REQUEST_SET_ADDRESS:
            #if defined(DEBUG)
            SerialPrint("SET_ADDRESS\r\n");
            #endif
            // Generate a zero length packet
            inPipe.info.bits.busy = 1;
            // Update state only
            USBDeviceState = ADR_PENDING_STATE;
            break;

        case USB_REQUEST_GET_DESCRIPTOR:
            #if defined(DEBUG)
            SerialPrint("GET_DESCRIPTOR\r\n");
            #endif
            USBStdGetDscHandler();
            break;

        case USB_REQUEST_GET_CONFIGURATION:
            #if defined(DEBUG)
            SerialPrint("GET_CONFIGURATION\r\n");
            #endif
            // Set Source
            inPipe.pSrc.bRam = (UINT8*)&USBActiveConfiguration;
            // Set memory type
            inPipe.info.bits.ctrl_trf_mem = USB_EP0_RAM;
            // Set data count
            inPipe.wCount.v[0] = 1;
            inPipe.info.bits.busy = 1;
            break;

        case USB_REQUEST_SET_CONFIGURATION:
            #if defined(DEBUG)
            SerialPrint("SET_CONFIGURATION\r\n");
            #endif
            USBStdSetCfgHandler();
            break;

        #if 0
        case USB_REQUEST_SET_DESCRIPTOR:
            #if defined(DEBUG)
            SerialPrint("SET_DESCRIPTOR\r\n");
            #endif
            //USB_SET_DESCRIPTOR_HANDLER(EVENT_SET_DESCRIPTOR,0,0);
            break;

        case USB_REQUEST_GET_INTERFACE:
            #if defined(DEBUG)
            SerialPrint("interface.\r\n");
            #endif
            // Set source
            inPipe.pSrc.bRam = (UINT8*)&USBAlternateInterface[SetupPkt.bIntfID];
            // Set memory type
            inPipe.info.bits.ctrl_trf_mem = USB_EP0_RAM;
            // Set data count
            inPipe.wCount.v[0] = 1;
            inPipe.info.bits.busy = 1;
            break;

        case USB_REQUEST_SET_INTERFACE:
            #if defined(DEBUG)
            SerialPrint("SET_INTERFACE\r\n");
            #endif
            inPipe.info.bits.busy = 1;
            USBAlternateInterface[SetupPkt.bIntfID] = SetupPkt.bAltID;
            break;

        case USB_REQUEST_SYNCH_FRAME:
            #if defined(DEBUG)
            SerialPrint("SYNCH_FRAME\r\n");
            #endif
            break;
            
        default:
            #if defined(DEBUG)
            SerialPrint("UNKNOWN\r\n");
            #endif
            break;
        #endif
    }
}//end USBCheckStdRequest

/********************************************************************
 * Handles HID Report request that happen on EP0.
 *******************************************************************/

void USBHIDCBGetReportHandler(void)
{ 
    if (SetupPkt.bReportType == HID_INPUT_REPORT)
    { 
        if (SetupPkt.bReportID == 0x00)
        {             
            inPipe.pSrc.bRam = (UINT8*)&hid_report_in;
            inPipe.wCount.Val = sizeof(hid_report_in);
            inPipe.info.Val = USB_EP0_INCLUDE_ZERO | USB_EP0_BUSY | USB_EP0_RAM;
        } 
    }
}  //end USBHIDCBGetReportHandler

/********************************************************************
 * Handles HID specific request that happen on EP0.
 *******************************************************************/

void USBCheckHIDRequest(void)
{
    if (SetupPkt.Recipient != USB_SETUP_RECIPIENT_INTERFACE_BITFIELD)
        return;

    if (SetupPkt.bIntfID != HID_INTF_ID)
        return;

    if (SetupPkt.bRequest == USB_REQUEST_GET_DESCRIPTOR)
    {
        switch (SetupPkt.bDescriptorType)
        {
            case DSC_HID: //HID Descriptor
                #if defined(DEBUG)
                SerialPrint("HID descriptor.\r\n");
                #endif
                if (USBActiveConfiguration == 1)
                {
                    //USBEP0SendROMPtr((const UINT8*)&configDescriptor1 + 18, sizeof(USB_HID_DSC)+3, USB_EP0_INCLUDE_ZERO);
                    //18 is the offset from start of the configuration descriptor
                    //to the start of the HID descriptor.
                    inPipe.pSrc.bRom = (const UINT8*)&configDescriptor1 + 18;
                    inPipe.wCount.Val = sizeof(USB_HID_DESCRIPTOR);
                    inPipe.info.Val = USB_EP0_INCLUDE_ZERO | USB_EP0_BUSY | USB_EP0_ROM;
                }
                break;

            case DSC_RPT:  //Report Descriptor
                #if defined(DEBUG)
                SerialPrint("Report descriptor.\r\n");
                #endif
                if (USBActiveConfiguration == 1)
                {
                    //USBEP0SendROMPtr((const UINT8*)&hid_rpt01, sizeof(hid_rpt01), USB_EP0_INCLUDE_ZERO);
                    inPipe.pSrc.bRom = (const UINT8*)&hid_rpt01;
                    inPipe.wCount.Val = sizeof(hid_rpt01);
                    inPipe.info.Val = USB_EP0_INCLUDE_ZERO | USB_EP0_BUSY | USB_EP0_ROM;
                }
                break;
        }
    }

    if (SetupPkt.RequestType != USB_SETUP_TYPE_CLASS_BITFIELD)
        return;

    switch (SetupPkt.bRequest)
    {
        case GET_REPORT:
            #if defined(DEBUG)
            SerialPrint("GET_REPORT\r\n");
            #endif
            USBHIDCBGetReportHandler();
            break;

        #if 0
        case SET_REPORT:
            #if defined(DEBUG)
            SerialPrint("SET_REPORT\r\n");
            #endif
            #if defined USER_SET_REPORT_HANDLER
                USER_SET_REPORT_HANDLER();
            #endif
            break;

        case GET_IDLE:
            #if defined(DEBUG)
            SerialPrint("GET_IDLE\r\n");
            #endif
            //USBEP0SendRAMPtr( (UINT8*)&idle_rate, 1, USB_EP0_INCLUDE_ZERO);
            inPipe.pSrc.bRam = (UINT8*)&idle_rate;
            inPipe.wCount.Val = 1;
            inPipe.info.Val = USB_EP0_INCLUDE_ZERO | USB_EP0_BUSY | USB_EP0_RAM;
            break;
        #endif

        case SET_IDLE:
            #if defined(DEBUG)
            SerialPrint("SET_IDLE\r\n");
            #endif
            //USBEP0Transmit(USB_EP0_NO_DATA);
            inPipe.info.Val = USB_EP0_NO_DATA | USB_EP0_BUSY;
            idle_rate = SetupPkt.W_Value.byte.HB;
            break;

        #if 0
        case GET_PROTOCOL:
            #if defined(DEBUG)
            SerialPrint("GET_PROTOCOL\r\n");
            #endif
            //USBEP0SendRAMPtr( (UINT8*)&active_protocol, 1, USB_EP0_NO_OPTIONS);
            inPipe.pSrc.bRam = (UINT8*)&active_protocol;
            inPipe.wCount.Val = 1;
            inPipe.info.Val = USB_EP0_NO_OPTIONS | USB_EP0_BUSY | USB_EP0_RAM;
            break;

        case SET_PROTOCOL:
            #if defined(DEBUG)
            SerialPrint("SET_PROTOCOL\r\n");
            #endif
            //USBEP0Transmit(USB_EP0_NO_DATA);
            inPipe.info.Val = USB_EP0_NO_DATA | USB_EP0_BUSY;
            active_protocol = SetupPkt.W_Value.byte.LB;
            break;
        #endif
    }
}

/********************************************************************
 * Handles the standard SET & CLEAR FEATURES requests
 *******************************************************************/
#if 0
static void USBStdFeatureReqHandler(void)
{
    BDT_ENTRY *p;
    EP_STATUS current_ep_data;

    //Check if the host sent a valid SET or CLEAR feature (remote wakeup) request.
    if((SetupPkt.bFeature == USB_FEATURE_DEVICE_REMOTE_WAKEUP)&&
       (SetupPkt.Recipient == USB_SETUP_RECIPIENT_DEVICE_BITFIELD))
    {
        inPipe.info.bits.busy = 1;
        if(SetupPkt.bRequest == USB_REQUEST_SET_FEATURE)
            RemoteWakeup = TRUE;
        else
            RemoteWakeup = FALSE;
    }//end if

    //Check if the host sent a valid SET or CLEAR endpoint halt request.
    if((SetupPkt.bFeature == USB_FEATURE_ENDPOINT_HALT)&&
       (SetupPkt.Recipient == USB_SETUP_RECIPIENT_ENDPOINT_BITFIELD)&&
       (SetupPkt.EPNum != 0) && (SetupPkt.EPNum <= USB_MAX_EP_NUMBER)&&
       (USBDeviceState == CONFIGURED_STATE))
    {
        //The request was valid.  Take control of the control transfer and
        //perform the host requested action.
        inPipe.info.bits.busy = 1;

        //Fetch a pointer to the BDT that the host wants to SET/CLEAR halt on.
        if(SetupPkt.EPDir == OUT_FROM_HOST)
        {
            p = (BDT_ENTRY*)pBDTEntryOut[SetupPkt.EPNum];
            current_ep_data.Val = ep_data_out[SetupPkt.EPNum].Val;
        }
        else
        {
            p = (BDT_ENTRY*)pBDTEntryIn[SetupPkt.EPNum];
            current_ep_data.Val = ep_data_in[SetupPkt.EPNum].Val;
        }

        //If ping pong buffering is enabled on the requested endpoint, need
        //to point to the one that is the active BDT entry which the SIE will
        //use for the next attempted transaction on that EP number.
        if(current_ep_data.bits.ping_pong_state == 0) //Check if even
        {
            //USBHALPingPongSetToEven(&p);
            ((BYTE_VAL*)&p)->Val &= ~USB_NEXT_PING_PONG;
        }
        else //else must have been odd
        {
            //USBHALPingPongSetToOdd(&p);
            ((BYTE_VAL*)&p)->Val |= USB_NEXT_PING_PONG;
        }

        //Update the BDT pointers with the new, next entry based on the feature
        //  request
        if(SetupPkt.EPDir == OUT_FROM_HOST)
        {
            pBDTEntryOut[SetupPkt.EPNum] = (volatile BDT_ENTRY *)p;
        }
        else
        {
            pBDTEntryIn[SetupPkt.EPNum] = (volatile BDT_ENTRY *)p;
        }

        //Check if it was a SET_FEATURE endpoint halt request
        if(SetupPkt.bRequest == USB_REQUEST_SET_FEATURE)
        {
            if(p->STAT.UOWN == 1)
            {
                //Mark that we are terminating this transfer and that the user
                //  needs to be notified later
                if(SetupPkt.EPDir == 0)
                {
                    ep_data_out[SetupPkt.EPNum].bits.transfer_terminated = 1;
                }
                else
                {
                    ep_data_in[SetupPkt.EPNum].bits.transfer_terminated = 1;
                }
            }

            //Then STALL the endpoint
            p->STAT.Val |= _USIE|_BSTALL;
        }
        else
        {
            //Else the request must have been a CLEAR_FEATURE endpoint halt.
            //toggle over the to the non-active BDT
            //USBAdvancePingPongBuffer(&p);
            ((BYTE_VAL*)&p)->Val ^= USB_NEXT_PING_PONG;

            if(p->STAT.UOWN == 1)
            {
                //Clear UOWN and set DTS state so it will be correct the next time
                //the application firmware uses USBTransferOnePacket() on the EP.
                p->STAT.Val &= (~_USIE);    //Clear UOWN bit
                p->STAT.Val |= _DAT1;       //Set DTS to DATA1
                //USB_TRANSFER_TERMINATED_HANDLER(EVENT_TRANSFER_TERMINATED,p,sizeof(p));
            }
            else
            {
                //UOWN already clear, but still need to set DTS to DATA1
                p->STAT.Val |= _DAT1;
            }

            //toggle back to the active BDT (the one the SIE is currently looking at
            //and will use for the next successful transaction to take place on the EP
            //USBAdvancePingPongBuffer(&p);
            ((BYTE_VAL*)&p)->Val ^= USB_NEXT_PING_PONG;

            //Check if we are currently terminating, or have previously terminated
            //a transaction on the given endpoint.  If so, need to clear UOWN,
            //set DTS to the proper state, and call the application callback
            //function.
            if((current_ep_data.bits.transfer_terminated != 0) || ((p->STAT.UOWN == 1) && (p->STAT.BSTALL == 0)))
            {
                if(SetupPkt.EPDir == 0)
                {
                    ep_data_out[SetupPkt.EPNum].bits.transfer_terminated = 0;
                }
                else
                {
                    ep_data_in[SetupPkt.EPNum].bits.transfer_terminated = 0;
                }
                //clear UOWN, clear DTS to DATA0, and finally remove the STALL condition
                p->STAT.Val &= ~(_USIE | _DAT1 | _BSTALL);
                //USB_TRANSFER_TERMINATED_HANDLER(EVENT_TRANSFER_TERMINATED,p,sizeof(p));
            }
            else
            {
                //clear UOWN, clear DTS to DATA0, and finally remove the STALL condition
                p->STAT.Val &= ~(_USIE | _DAT1 | _BSTALL);
            }
        }//end if
    }//end if
}//end USBStdFeatureReqHandler
#endif

/********************************************************************
 * Function:        void USBCtrlEPAllowStatusStage(void)
 *
 * PreCondition:    Before USBCtrlEPAllowStatusStage() is called, the USBDeviceInit()
 *                  function must have been called at least once, since the last
 *                  microcontroller reset or power up cycle.
 *
 * Overview:        This function can be called to prepare the appropriate
 *                  EP0 endpoint (IN or OUT) to be ready to allow the status
 *                  stage of the currently pending control transfer to complete.
 *                  This function will be called automatically by the
 *                  USB stack, while processing a control transfer, unless an
 *                  application specific/device class specific handler sets
 *                  the USBDeferStatusStagePacket boolean to TRUE, while processing
 *                  the SETUP packet that initiated the control transfer.  If
 *                  application specific code sets USBDeferStatusStagePacket to TRUE,
 *                  it is then responsible for calling USBCtrlEPAllowStatusStage(),
 *                  once the control transfer data has been satisfactorily processed.
 *
 * Note:            This function is re-entrant, and is safe to call from multiple
 *                  contexts in the application (either higher, same, or lower
 *                  priority than the context that the USBDeviceTasks() function
 *                  executes at.
 *******************************************************************/

static void USBCtrlEPAllowStatusStage(void)
{
    //Check and set two flags, prior to actually modifying any BDT entries.
    //This double checking is necessary to make certain that
    //USBCtrlEPAllowStatusStage() can be called twice simultaneously (ex: once
    //in main loop context, while simultaneously getting an interrupt which
    //tries to call USBCtrlEPAllowStatusStage() again, at the same time).
    if(USBStatusStageEnabledFlag1 == FALSE)
    {
        USBStatusStageEnabledFlag1 = TRUE;

        if(USBStatusStageEnabledFlag2 == FALSE)
        {
            USBStatusStageEnabledFlag2 = TRUE;

            //Determine which endpoints (EP0 IN or OUT needs arming for the status
            //stage), based on the type of control transfer currently pending.
            if(controlTransferState == CTRL_TRF_RX)
            {
                pBDTEntryIn[0]->CNT = 0;
                pBDTEntryIn[0]->STAT.Val = _USIE|_DAT1|_DTSEN;
            }
            else if(controlTransferState == CTRL_TRF_TX)
            {
                BothEP0OutUOWNsSet = FALSE;	//Indicator flag used in USBCtrlTrfOutHandler()

                //This buffer (when ping pong buffering is enabled on EP0 OUT) receives the
                //next SETUP packet.
                pBDTEntryEP0OutCurrent->CNT = USB_EP0_BUFF_SIZE;
                pBDTEntryEP0OutCurrent->ADR = ConvertToPhysicalAddress(&SetupPkt);
                pBDTEntryEP0OutCurrent->STAT.Val = _USIE|_BSTALL; //Prepare endpoint to accept a SETUP transaction
                BothEP0OutUOWNsSet = TRUE;	//Indicator flag used in USBCtrlTrfOutHandler()

                //This EP0 OUT buffer receives the 0-byte OUT status stage packet.
                pBDTEntryEP0OutNext->CNT = USB_EP0_BUFF_SIZE;
                pBDTEntryEP0OutNext->ADR = ConvertToPhysicalAddress(&SetupPkt);
                pBDTEntryEP0OutNext->STAT.Val = _USIE;           // Note: DTSEN is 0
            }
        }
    }
}

/*******************************************************************************
  Function: void USBCtrlEPAllowDataStage(void);

  Summary: This function allows the data stage of either a host-to-device or
            device-to-host control transfer (with data stage) to complete.
            This function is meant to be used in conjunction with either the
            USBDeferOUTDataStage() or USBDeferINDataStage().  If the firmware
            does not call either USBDeferOUTDataStage() or USBDeferINDataStage(),
            then the firmware does not need to manually call
            USBCtrlEPAllowDataStage(), as the USB stack will call this function
            instead.

  Description:

  Conditions: A control transfer (with data stage) should already be pending,
                if the firmware calls this function.  Additionally, the firmware
                should have called either USBDeferOUTDataStage() or
                USBDeferINDataStage() at the start of the control transfer, if
                the firmware will be calling this function manually.
  *****************************************************************************/

static void USBCtrlEPAllowDataStage(void)
{
    USBDeferINDataStagePackets = FALSE;
    USBDeferOUTDataStagePackets = FALSE;

    if(controlTransferState == CTRL_TRF_RX) //(<setup><out><out>...<out><in>)
    {
        //Prepare EP0 OUT to receive the first OUT data packet in the data stage sequence.
        pBDTEntryEP0OutNext->CNT = USB_EP0_BUFF_SIZE;
        pBDTEntryEP0OutNext->ADR = ConvertToPhysicalAddress(&CtrlTrfData);
        pBDTEntryEP0OutNext->STAT.Val = _USIE|_DAT1|_DTSEN;
    }
    else    //else must be controlTransferState == CTRL_TRF_TX (<setup><in><in>...<in><out>)
    {
        //Error check the data stage byte count.  Make sure the user specified
        //value was no greater than the number of bytes the host requested.
        if(SetupPkt.wLength < inPipe.wCount.Val)
        {
            inPipe.wCount.Val = SetupPkt.wLength;
        }

        //Copies one IN data packet worth of data from application buffer
        //to CtrlTrfData buffer.  Also keeps track of how many bytes remaining.
        USBCtrlTrfTxService();

        //Cnt should have been initialized by responsible request owner (ex: by
        //using the USBEP0SendRAMPtr() or USBEP0SendROMPtr() API function).
        pBDTEntryIn[0]->ADR = ConvertToPhysicalAddress(&CtrlTrfData);
        pBDTEntryIn[0]->STAT.Val = _USIE|_DAT1|_DTSEN;
    }
}

/********************************************************************
 * Handles the standard GET_DESCRIPTOR request.
 *******************************************************************/

static void USBStdGetDscHandler(void)
{
    if(SetupPkt.bmRequestType == 0x80)
    {
        #if defined(DEBUG)
        //SerialPrint("Device is sending ");
        #endif

        inPipe.info.Val = USB_EP0_ROM | USB_EP0_BUSY | USB_EP0_INCLUDE_ZERO;

        switch(SetupPkt.bDescriptorType)
        {
            case USB_DESCRIPTOR_DEVICE:
                #if defined(DEBUG)
                //SerialPrint("device descriptor. ");
                #endif
                inPipe.pSrc.bRom = (const UINT8*)&device_dsc;
                inPipe.wCount.Val = sizeof(device_dsc);
                break;

            case USB_DESCRIPTOR_CONFIGURATION:
                #if defined(DEBUG)
                //SerialPrint("configuration descriptor. ");
                #endif
                inPipe.pSrc.bRom = *(USB_CD_Ptr+SetupPkt.bDscIndex);

                // This must be loaded using byte addressing.  The source pointer
                // may not be word aligned for the 16 or 32 bit machines resulting
                // in an address error on the dereference.
                inPipe.wCount.byte.LB = *(inPipe.pSrc.bRom + 2);
                inPipe.wCount.byte.HB = *(inPipe.pSrc.bRom + 3);
                break;

            case USB_DESCRIPTOR_STRING:
                #if defined(DEBUG)
                //SerialPrint("string descriptor. ");
                #endif
                //USB_NUM_STRING_DESCRIPTORS is the number of string descriptors.
                if(SetupPkt.bDscIndex < USB_NUM_STRING_DESCRIPTORS)
                {
                    //Get a pointer to the String descriptor requested
                    inPipe.pSrc.bRom = *(USB_SD_Ptr + SetupPkt.bDscIndex);
                    // Set data count
                    inPipe.wCount.Val = *inPipe.pSrc.bRom;
                }
                else
                {
                    inPipe.info.Val = 0;
                }
                break;

            default:
                #if defined(DEBUG)
                //SerialPrint("null descriptor. ");
                #endif
                inPipe.info.Val = 0;
                break;

        }//end switch
    }//end if
}//end USBStdGetDscHandler

/********************************************************************
 * Handles the standard GET_STATUS request
 *******************************************************************/
#if 0
static void USBStdGetStatusHandler(void)
{
    CtrlTrfData[0] = 0;                 // Initialize content
    CtrlTrfData[1] = 0;

    #if defined(DEBUG)
    //SerialPrint("Device is sending ");
    #endif

    switch(SetupPkt.Recipient)
    {
        case USB_SETUP_RECIPIENT_DEVICE_BITFIELD:
            #if defined(DEBUG)
            SerialPrint("USB_SETUP_RECIPIENT_DEVICE_BITFIELD\r\n");
            #endif
            inPipe.info.bits.busy = 1;
            /*
             * [0]: bit0: Self-Powered Status [0] Bus-Powered [1] Self-Powered
             *      bit1: RemoteWakeup        [0] Disabled    [1] Enabled
             */
            #if 0 // self_power is defined in hardware.h
            if(self_power == 1)
            {
                CtrlTrfData[0]|=0x01;
            }
            #endif
            if(RemoteWakeup == TRUE)
            {
                CtrlTrfData[0]|=0x02;
            }
            break;

        case USB_SETUP_RECIPIENT_INTERFACE_BITFIELD:
            #if defined(DEBUG)
            SerialPrint("USB_SETUP_RECIPIENT_INTERFACE_BITFIELD\r\n");
            #endif
            inPipe.info.bits.busy = 1;     // No data to update
            break;

        case USB_SETUP_RECIPIENT_ENDPOINT_BITFIELD:
            #if defined(DEBUG)
            SerialPrint("USB_SETUP_RECIPIENT_ENDPOINT_BITFIELD\r\n");
            #endif
            inPipe.info.bits.busy = 1;
            /*
             * [0]: bit0: Halt Status [0] Not Halted [1] Halted
             */
            BDT_ENTRY *p;

            if(SetupPkt.EPDir == 0)
                p = (BDT_ENTRY*)pBDTEntryOut[SetupPkt.EPNum];
            else
                p = (BDT_ENTRY*)pBDTEntryIn[SetupPkt.EPNum];

            // Use _BSTALL as a bit mask
            if(p->STAT.Val & _BSTALL)
                CtrlTrfData[0]=0x01;    // Set bit0

            break;

        }//end switch

    if(inPipe.info.bits.busy == 1)
    {
        // Set Source
        inPipe.pSrc.bRam = (UINT8*)&CtrlTrfData;
        // Set memory type
        inPipe.info.bits.ctrl_trf_mem = USB_EP0_RAM;
        // Set data count
        inPipe.wCount.v[0] = 2;
    }
}//end USBStdGetStatusHandler
#endif

/******************************************************************************
 * Function:        void USBCtrlEPServiceComplete(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This routine wrap up the remaining tasks in servicing
 *                  a Setup Request. Its main task is to set the endpoint
 *                  controls appropriately for a given situation. See code
 *                  below.
 *                  There are three main scenarios:
 *                  a) There was no handler for the Request, in this case
 *                     a STALL should be sent out.
 *                  b) The host has requested a read control transfer,
 *                     endpoints are required to be setup in a specific way.
 *                  c) The host has requested a write control transfer, or
 *                     a control data stage is not required, endpoints are
 *                     required to be setup in a specific way.
 *
 *                  Packet processing is resumed by clearing PKTDIS bit.
 *
 * Note:            None
 *****************************************************************************/

static void USBCtrlEPServiceComplete(void)
{
    #if defined(DEBUG)
    //SerialPrint("Done\r\n");
    #endif

    // PKTDIS: Packet Transfer Disable bit
    // Clear to resume packet processing.
    U1CONCLR = _U1CON_PKTDIS_MASK;

    //Check the busy bits and the SetupPtk.DataDir variables to determine what type of
    //control transfer is currently in progress.  We need to know the type of control
    //transfer that is currently pending, in order to know how to properly arm the
    //EP0 IN and EP0 OUT endpoints.

    if(inPipe.info.bits.busy == 0)
    {
        if(outPipe.info.bits.busy == 1)
        {
            controlTransferState = CTRL_TRF_RX;
            /*
             * Control Write:
             * <SETUP[0]><OUT[1]><OUT[0]>...<IN[1]> | <SETUP[0]>
             */

            //1. Prepare OUT EP to receive data, unless a USB class request handler
            //   function decided to defer the data stage (ex: because the intended
            //   RAM buffer wasn't available yet) by calling USBDeferDataStage().
            //   If it did so, it is then responsible for calling USBCtrlEPAllowDataStage(),
            //   once it is ready to begin receiving the data.
            if(USBDeferOUTDataStagePackets == FALSE)
            {
                USBCtrlEPAllowDataStage();
            }

            //2. IN endpoint 0 status stage will be armed by USBCtrlEPAllowStatusStage()
            //   after all of the OUT data has been received and consumed, or if a timeout occurs.
            USBStatusStageEnabledFlag2 = FALSE;
            USBStatusStageEnabledFlag1 = FALSE;
        }
        else
        {
            /*
             * If no one knows how to service this request then stall.
             * Must also prepare EP0 to receive the next SETUP transaction.
             */
            pBDTEntryEP0OutNext->CNT = USB_EP0_BUFF_SIZE;
            pBDTEntryEP0OutNext->ADR = ConvertToPhysicalAddress(&SetupPkt);
            pBDTEntryEP0OutNext->STAT.Val = _USIE|_DAT0|_DTSEN|_BSTALL;
            pBDTEntryIn[0]->STAT.Val = _USIE|_BSTALL;
        }
    }
    else    // A module has claimed ownership of the control transfer session.
    {
        if (SetupPkt.DataDir == USB_SETUP_DEVICE_TO_HOST_BITFIELD)
        {
            controlTransferState = CTRL_TRF_TX;
            /*
             * Control Read:
             * <SETUP[0]><IN[1]><IN[0]>...<OUT[1]> | <SETUP[0]>
             *
             * 1. Prepare IN EP to transfer data to the host.  If however the data
             *    wasn't ready yet (ex: because the firmware needs to go and read it from
             *    some slow/currently unavailable resource, such as an external I2C EEPROM),
             *    Then the class request handler reponsible should call the USBDeferDataStage()
             *    macro.  In this case, the firmware may wait up to 500ms, before it is required
             *    to transmit the first IN data packet.  Once the data is ready, and the firmware
             *    is ready to begin sending the data, it should then call the
             *    USBCtrlEPAllowDataStage() function to start the data stage.
             */
            if(USBDeferINDataStagePackets == FALSE)
            {
                USBCtrlEPAllowDataStage();
            }

            // 2. (Optionally) allow the status stage now, to prepare for early termination.
            //    Note: If a class request handler decided to set USBDeferStatusStagePacket == TRUE,
            //    then it is responsible for eventually calling USBCtrlEPAllowStatusStage() once it
            //    is ready.  If the class request handler does this, it needs to be careful to
            //    be written so that it can handle the early termination scenario.
            //    Ex: It should call USBCtrlEPAllowStatusStage() when any of the following occurs:
            //    1.  The desired total number of bytes were sent to the host.
            //    2.  The number of bytes that the host originally requested (in the SETUP packet that
            //        started the control transfer) has been reached.
            //    3.  Or, if a timeout occurs (ex: <50ms since the last successful EP0 IN transaction), regardless
            //        of how many bytes have actually been sent.  This is necessary to prevent a deadlock situation
            //        (where the control transfer can't complete, due to continuous NAK on status stage) if the
            //        host performs early termination.  If enabled, the USB_ENABLE_STATUS_STAGE_TIMEOUTS usb_config.h
            //        option can take care of this for you.
            //    Note: For this type of control transfer, there is normally no harm in simply arming the
            //    status stage packet right now, even if the IN data is not ready yet.  This allows for
            //    immediate early termination, without adding unecessary delay.  Therefore, it is generally not
            //    recommended for the USB class handler firmware to call USBDeferStatusStage(), for this
            //    type of control transfer.  If the USB class handler firmware needs more time to fetch the IN
            //    data that needs to be sent to the host, it should instead use the USBDeferDataStage() function.
            USBStatusStageEnabledFlag2 = FALSE;
            USBStatusStageEnabledFlag1 = FALSE;
            if(USBDeferStatusStagePacket == FALSE)
            {
                USBCtrlEPAllowStatusStage();
            }
        }
        else   // (SetupPkt.DataDir == USB_SETUP_DIRECTION_HOST_TO_DEVICE)
        {
            //This situation occurs for special types of control transfers,
            //such as that which occurs when the host sends a SET_ADDRESS
            //control transfer.  Ex:
            //
            //<SETUP[0]><IN[1]> | <SETUP[0]>

            //Although the data direction is HOST_TO_DEVICE, there is no data stage
            //(hence: outPipe.info.bits.busy == 0).  There is however still
            //an IN status stage.

            controlTransferState = CTRL_TRF_RX;     //Since this is a HOST_TO_DEVICE control transfer

            //1. Prepare OUT EP to receive the next SETUP packet.
            pBDTEntryEP0OutNext->CNT = USB_EP0_BUFF_SIZE;
            pBDTEntryEP0OutNext->ADR = ConvertToPhysicalAddress(&SetupPkt);
            pBDTEntryEP0OutNext->STAT.Val = _USIE|_BSTALL;

            //2. Prepare for IN status stage of the control transfer
            USBStatusStageEnabledFlag2 = FALSE;
            USBStatusStageEnabledFlag1 = FALSE;
            if(USBDeferStatusStagePacket == FALSE)
            {
                USBCtrlEPAllowStatusStage();
            }
        }
    }//end if(ctrl_trf_session_owner == MUID_NULL)
}

/******************************************************************************
 * Function:        void USBCtrlTrfTxService(void)
 *
 * PreCondition:    pSrc, wCount, and usb_stat.ctrl_trf_mem are setup properly.
 *
 * Overview:        This routine is used for device to host control transfers
 *					(IN transactions).  This function takes care of managing a
 *                  transfer over multiple USB transactions.
 *					This routine should be called from only two places.
 *                  One from USBCtrlEPServiceComplete() and one from
 *                  USBCtrlTrfInHandler().
 *****************************************************************************/

static void USBCtrlTrfTxService(void)
{
    UINT8 byteToSend;

    #if defined(DEBUG)
    //SerialPrint("> USBCtrlTrfSetupHandler\r\n");
    #endif

    //Figure out how many bytes of data to send in the next IN transaction.
    //Assume a full size packet, unless otherwise determined below.
    byteToSend = USB_EP0_BUFF_SIZE;

    if(inPipe.wCount.Val < (UINT8)USB_EP0_BUFF_SIZE)
    {
        byteToSend = inPipe.wCount.Val;

        //Keep track of whether or not we have sent a "short packet" yet.
        //This is useful so that later on, we can configure EP0 IN to STALL,
        //after we have sent all of the intended data.  This makes sure the
        //hardware STALLs if the host erroneously tries to send more IN token
        //packets, requesting more data than intended in the control transfer.
        if (shortPacketStatus == SHORT_PKT_NOT_USED)
            shortPacketStatus = SHORT_PKT_PENDING;
        else if(shortPacketStatus == SHORT_PKT_PENDING)
            shortPacketStatus = SHORT_PKT_SENT;
    }

    //Keep track of how many bytes remain to be sent in the transfer, by
    //subtracting the number of bytes about to be sent from the total.
    inPipe.wCount.Val = inPipe.wCount.Val - byteToSend;

    //Next, load the number of bytes to send to BC7..0 in buffer descriptor.
    //Note: Control endpoints may never have a max packet size of > 64 bytes.
    //Therefore, the BC8 and BC9 bits should always be maintained clear.
    pBDTEntryIn[0]->CNT = byteToSend;

    //Now copy the data from the source location, to the CtrlTrfData[] buffer,
    //which we will send to the host.
    pDst = (USB_VOLATILE UINT8*)CtrlTrfData;                // Set destination pointer

    if(inPipe.info.bits.ctrl_trf_mem == USB_EP0_ROM)   // Determine type of memory source
    {
        while(byteToSend)
        {
            *pDst++ = *inPipe.pSrc.bRom++;
            byteToSend--;
        }
    }
    else  // RAM
    {
        while(byteToSend)
        {
            *pDst++ = *inPipe.pSrc.bRam++;
            byteToSend--;
        }
    }
}

/******************************************************************************
 * Function:        void USBCtrlTrfRxService(void)
 *
 * PreCondition:    pDst and wCount are setup properly.
 *                  pSrc is always &CtrlTrfData
 *                  usb_stat.ctrl_trf_mem is always USB_EP0_RAM.
 *                  wCount should be set to 0 at the start of each control
 *                  transfer.
 *
 * Overview:        This routine is used for host to device control transfers
 *		    (uses OUT transactions).  This function receives the data that arrives
 *		    on EP0 OUT, and copies it into the appropriate outPipe.pDst.bRam
 *		    buffer.  Once the host has sent all the data it was intending
 *		    to send, this function will call the appropriate outPipe.pFunc()
 *		    handler (unless it is NULL), so that it can be used by the
 *		    intended target firmware.
 *****************************************************************************/

static void USBCtrlTrfRxService(void)
{
    UINT8 byteToRead;
    UINT8 i;

    #if defined(DEBUG)
    //SerialPrint("> USBCtrlTrfRxService\r\n");
    #endif

    //Load byteToRead with the number of bytes the host just sent us in the
    //last OUT transaction.
    byteToRead = pBDTEntryEP0OutCurrent->CNT;

    //Update the "outPipe.wCount.Val", which keeps track of the total number
    //of remaining bytes expected to be received from the host, in the control
    //transfer.  First check to see if the host sent us more bytes than the
    //application firmware was expecting to receive.
    if(byteToRead > outPipe.wCount.Val)
    {
        byteToRead = outPipe.wCount.Val;
    }

    //Reduce the number of remaining bytes by the number we just received.
    outPipe.wCount.Val = outPipe.wCount.Val - byteToRead;

    //Copy the OUT DATAx packet bytes that we just received from the host,
    //into the user application buffer space.
    for(i=0;i<byteToRead;i++)//while(byteToRead.Val)
    {
        *outPipe.pDst.bRam++ = CtrlTrfData[i];
    }

    //If there is more data to receive, prepare EP0 OUT so that it can receive
    //the next packet in the sequence.
    if(outPipe.wCount.Val > 0)
    {
        pBDTEntryEP0OutNext->CNT = USB_EP0_BUFF_SIZE;
        pBDTEntryEP0OutNext->ADR = ConvertToPhysicalAddress(&CtrlTrfData);
        if(pBDTEntryEP0OutCurrent->STAT.DTS == 0)
        {
            pBDTEntryEP0OutNext->STAT.Val = _USIE|_DAT1|_DTSEN;
        }
        else
        {
            pBDTEntryEP0OutNext->STAT.Val = _USIE|_DAT0|_DTSEN;
        }
    }
    else
    {
        //We have received all OUT packets that we were expecting to
        //receive for the control transfer.  Prepare EP0 OUT to receive
        //the next SETUP transaction that may arrive.
        pBDTEntryEP0OutNext->CNT = USB_EP0_BUFF_SIZE;
        pBDTEntryEP0OutNext->ADR = ConvertToPhysicalAddress(&SetupPkt);
        //Configure EP0 OUT to receive the next SETUP transaction for any future
        //control transfers.  However, set BSTALL in case the host tries to send
        //more data than it claims it was going to send.
        pBDTEntryEP0OutNext->STAT.Val = _USIE|_BSTALL;

        //All data bytes for the host to device control write (OUT) have now been
        //received successfully.
        //Go ahead and call the user specified callback function, to use/consume
        //the control transfer data (ex: if the "void (*function)" parameter
        //was non-NULL when USBEP0Receive() was called).
        if(outPipe.pFunc != 0)
        {
            outPipe.pFunc();
        }
        outPipe.info.bits.busy = 0;

        //Ready to arm status stage IN transaction now, if the application
        //firmware has completed processing the request.  If it is still busy
        //and needs more time to finish handling the request, then the user
        //callback (the one called by the outPipe.pFunc();) should set the
        //USBDeferStatusStagePacket to TRUE (by calling USBDeferStatusStage()).  In
        //this case, it is the application's firmware responsibility to call
        //the USBCtrlEPAllowStatusStage() function, once it is fully done handling the request.
        //Note: The application firmware must process the request and call
        //USBCtrlEPAllowStatusStage() in a semi-timely fashion.  "Semi-timely"
        //means either 50ms, 500ms, or 5 seconds, depending on the type of
        //control transfer.  See the USB 2.0 specification section 9.2.6 for
        //more details.
        if(USBDeferStatusStagePacket == FALSE)
        {
            USBCtrlEPAllowStatusStage();
        }
    }
}

/********************************************************************
 * Function:        void USBStdSetCfgHandler(void)
 * Overview:        This routine first disables all endpoints by
 *                  clearing UEP registers. It then configures
 *                  (initializes) endpoints by calling the callback
 *                  function USBCBInitEP().
 *******************************************************************/

static void USBStdSetCfgHandler(void)
{
    UINT8 i;

    // This will generate a zero length packet
    inPipe.info.bits.busy = 1;

    //Clear all of the endpoint control registers
    //DisableNonZeroEndpoints(USB_MAX_EP_NUMBER);
    U1EP1 = 0x00;

    //Clear all of the BDT entries
    for(i=0;i<(sizeof(BDT)/sizeof(BDT_ENTRY));i++)
    {
        BDT[i].Val = 0;
    }

    // Assert reset request to all of the Ping Pong buffer pointers
    //USBPingPongBufferReset = 1;
    U1CONSET = _U1CON_PPBRST_MASK;

    //clear the alternate interface settings
    //memset((void*)&USBAlternateInterface,0x00,USB_MAX_NUM_INT);
    //MemClear ((void*)&USBAlternateInterface, USB_MAX_NUM_INT);
    #if 0
    for(i=0;i<USB_MAX_NUM_INT;i++)
    {
        USBAlternateInterface[i] = 0x00;
    }
    #else
    USBAlternateInterface[0] = 0;
    #endif

    //Stop trying to reset ping pong buffer pointers
    //USBPingPongBufferReset = 0;
    U1CONCLR = _U1CON_PPBRST_MASK;

    pBDTEntryIn[0] = (volatile BDT_ENTRY*)&BDT[EP0_IN_EVEN];

    //Set the next out to the current out packet
    pBDTEntryEP0OutCurrent = (volatile BDT_ENTRY*)&BDT[EP0_OUT_EVEN];
    pBDTEntryEP0OutNext = pBDTEntryEP0OutCurrent;

    //set the current configuration
    USBActiveConfiguration = SetupPkt.bConfigurationValue;

    //if the configuration value == 0
    if(USBActiveConfiguration == 0)
    {
        //Go back to the addressed state
        USBDeviceState = ADDRESS_STATE;
    }
    else
    {
        //initialize the required endpoints
        //USB_SET_CONFIGURATION_HANDLER(EVENT_CONFIGURED,(void*)&USBActiveConfiguration,1);
        //USBEventHandler(EVENT_CONFIGURED);//,(void*)&USBActiveConfiguration,1);
        USBEventHandler();

        //Otherwise go to the configured state.  Update the state variable last,
        //after performing all of the set configuration related initialization
        //tasks.
        USBDeviceState = CONFIGURED_STATE;
    }//end if(SetupPkt.bConfigurationValue == 0)
}

/********************************************************************
 * Configure the specified endpoint with the specified direction
 *******************************************************************/

static void USBConfigureEndpoint(UINT8 ep, UINT8 dir)
{
    volatile BDT_ENTRY* handle;

    //Compute a pointer to the even BDT entry corresponding to the
    //EPNum and direction values passed to this function.
    handle = (volatile BDT_ENTRY*)&BDT[EP0_OUT_EVEN]; //Get address of start of BDT
    handle += EP(ep, dir, 0);     //Add in offset to the BDT of interest

    handle->STAT.UOWN = 0;  //mostly redundant, since USBStdSetCfgHandler()
    //already cleared the entire BDT table

    //Make sure our pBDTEntryIn/Out[] pointer is initialized.  Needed later
    //for USBTransferOnePacket() API calls.
    if (dir == OUT_FROM_HOST)
    {
        pBDTEntryOut[ep] = handle;
    }
    else
    {
        pBDTEntryIn[ep] = handle;
    }

    handle->STAT.DTS = 0;
    (handle+1)->STAT.DTS = 1;
}

/*****************************************************************************************************************
    This function will enable the specified endpoint with the specified options.

  Input:
    UINT8 ep -       the endpoint to be configured
    UINT8 options -  optional settings for the endpoint. The options should
                    be ORed together to form a single options string. The
                    available optional settings for the endpoint. The
                    options should be ORed together to form a single options
                    string. The available options are the following\:
                    * USB_HANDSHAKE_ENABLED enables USB handshaking (ACK,
                      NAK)
                    * USB_HANDSHAKE_DISABLED disables USB handshaking (ACK,
                      NAK)
                    * USB_OUT_ENABLED enables the out direction
                    * USB_OUT_DISABLED disables the out direction
                    * USB_IN_ENABLED enables the in direction
                    * USB_IN_DISABLED disables the in direction
                    * USB_ALLOW_SETUP enables control transfers
                    * USB_DISALLOW_SETUP disables control transfers
  *****************************************************************************************************************/

void USBEnableEndpoint(UINT8 ep, UINT8 options)
{
    //Update the relevant UEPx register to actually enable the endpoint with
    //the specified options
    UINT8* p;

    //Use USBConfigureEndpoint() to set up the pBDTEntryIn/Out[ep] pointer and
    //starting DTS state in the BDT entry.
    if (options & USB_OUT_ENABLED)
    {
        USBConfigureEndpoint(ep, OUT_FROM_HOST);
    }

    if (options & USB_IN_ENABLED)
    {
        USBConfigureEndpoint(ep, IN_TO_HOST);
    }

    p = (UINT8*)(&U1EP0+(4*ep));

    *p = options;
}

/********************************************************************
 * Function:        void USBStallEndpoint(UINT8 ep, UINT8 dir)
 *
 * Input:
 *   UINT8 ep - the endpoint the data will be transmitted on
 *   UINT8 dir - the direction of the transfer
 *
 * Side Effects:    Endpoint is STALLed
 *
 * Overview:        STALLs the specified endpoint
 *******************************************************************/

static void USBStallEndpoint(UINT8 ep, UINT8 dir)
{
    BDT_ENTRY *p;

    if (ep == 0)
    {
        /*
         * If no one knows how to service this request then stall.
         * Must also prepare EP0 to receive the next SETUP transaction.
         */
        pBDTEntryEP0OutNext->CNT = USB_EP0_BUFF_SIZE;
        pBDTEntryEP0OutNext->ADR = ConvertToPhysicalAddress(&SetupPkt);

        /* v2b fix */
        pBDTEntryEP0OutNext->STAT.Val = _USIE|_DAT0|_DTSEN|_BSTALL;
        pBDTEntryIn[0]->STAT.Val = _USIE|_BSTALL;
    }
    else
    {
        p = (BDT_ENTRY*)(&BDT[EP(ep,dir,0)]);
        p->STAT.Val |= _BSTALL | _USIE;

        //If the device is in FULL ping pong modes
        //then stall that entry as well
        p = (BDT_ENTRY*)(&BDT[EP(ep,dir,1)]);
        p->STAT.Val |= _BSTALL | _USIE;
    }
}

/********************************************************************
 * Function:        USB_HANDLE USBTransferOnePacket(
 *                      UINT8 ep,
 *                      UINT8 dir,
 *                      UINT8* data,
 *                      UINT8 len)
 *
 * PreCondition:    The pBDTEntryIn[] or pBDTEntryOut[] pointer to
 *		the endpoint that will be used must have been
 *		initialized, prior to calling USBTransferOnePacket().
 *		Therefore, the application firmware should not call
 *		USBTransferOnePacket() until after the USB stack has been
 		initialized (by USBDeviceInit()), and the host has sent a
 *		set configuration request.  This can be checked by
 *		verifying that the USBGetDeviceState() == CONFIGURED_STATE,
 *		prior to calling USBTransferOnePacket().
 *
 *		Note: If calling the USBTransferOnePacket()
 *		function from within the USBCBInitEP() callback function,
 *		the set configuration is still being processed and the
 *		USBDeviceState may not be == CONFIGURED_STATE yet.  In this
 *		special case, the USBTransferOnePacket() may still be
 *		called, but make sure that the endpoint has been enabled
 *		and initialized by the USBEnableEndpoint() function first.
 *
 * Input:
 *   UINT8 ep - the endpoint number that the data will be transmitted or
 *		received on
 *   UINT8 dir - the direction of the transfer
 *              This value is either OUT_FROM_HOST or IN_TO_HOST
 *   UINT8* data - For IN transactions: pointer to the RAM buffer containing
 *		the data to be sent to the host.
 *		For OUT transactions: pointer to the RAM buffer that the
 *		received data should get written to.
 *   UINT8 len - length of the data needing to be sent (for IN transactions).
 *		For OUT transactions, the len parameter should normally be set
 *		to the endpoint size specified in the endpoint descriptor.
 *
 * Output:
 *   USB_HANDLE - handle to the transfer.  The handle is a pointer to
 *		the BDT entry associated with this transaction.  The
 *		status of the transaction (ex: if it is complete or still
 *		pending) can be checked using the USBHandleBusy() macro
 *		and supplying the USB_HANDLE provided by
 *		USBTransferOnePacket().
 *
 * Overview:    The USBTransferOnePacket() function prepares a USB endpoint
 *		so that it may send data to the host (an IN transaction),
 *		or receive data from the host (an OUT transaction).  The
 *		USBTransferOnePacket() function can be used both to receive
 *		and send data to the host.  The USBTransferOnePacket()
 *		function is the primary API function provided by the USB
 *		stack firmware for sending or receiving application data
 *		over the USB port.  The USBTransferOnePacket() is intended
 *		for use with all application endpoints.  It is not used for
 *		sending or receiving applicaiton data through endpoint 0
 *		by using control transfers.  Separate API functions,
 *		such as USBEP0Receive(), USBEP0SendRAMPtr(), and
 *		USBEP0SendROMPtr() are provided for this purpose.
 *
 *		The	USBTransferOnePacket() writes to the Buffer Descriptor
 *		Table (BDT) entry associated with an endpoint buffer, and
 *		sets the UOWN bit, which prepares the USB hardware to
 *		allow the transaction to complete.  The application firmware
 *		can use the USBHandleBusy() macro to check the status of the
 *		transaction, to see if the data has been successfully
 *		transmitted yet.
 *******************************************************************/

//USB_HANDLE USBTransferOnePacket(UINT8 ep, UINT8 dir, UINT8* data, UINT8 len)
USB_HANDLE USBTransferOnePacket(UINT8 dir, UINT8* data)
{
    volatile BDT_ENTRY *handle;

    //If the direction is IN point to the IN BDT of the specified endpoint
    if (dir == IN_TO_HOST)
    {
        handle = pBDTEntryIn[HID_EP];
    }
    else // OUT_FROM_HOST
    {
        handle = pBDTEntryOut[HID_EP];
    }

    //Error checking code.
    //Make sure the handle (pBDTEntryIn[ep] or pBDTEntryOut[ep])
    //is initialized before using it.
    if (handle == 0)
    {
        return 0;
    }

    //Set the data pointer, data length, and enable the endpoint
    handle->ADR = ConvertToPhysicalAddress(data);
    handle->CNT = HID_INT_EP_SIZE;
    handle->STAT.Val &= _DTSMASK;
    handle->STAT.Val |= _USIE | _DTSEN;

    //Point to the next buffer for ping pong purposes.
    if (dir == IN_TO_HOST)
    {
        //USBAdvancePingPongBuffer(&pBDTEntryIn[HID_EP]);
        ((BYTE_VAL*)&pBDTEntryIn[HID_EP])->Val ^= USB_NEXT_PING_PONG;
    }
    else
    {
        //USBAdvancePingPongBuffer(&pBDTEntryOut[HID_EP]);
        ((BYTE_VAL*)&pBDTEntryOut[HID_EP])->Val ^= USB_NEXT_PING_PONG;
    }
    
    return (USB_HANDLE)handle;
}

#endif // __USB_C
