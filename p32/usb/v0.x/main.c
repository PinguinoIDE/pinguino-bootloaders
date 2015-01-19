/*******************************************************************************
    Title:	USB Pinguino Bootloader
    File:	main.c
    Descr.:     USB HID bootloader for PIC32 processors
    Author:	Régis Blanchot <rblanchot@gmail.com>

    This file is part of Pinguino (http://www.pinguino.cc)
    Released under the LGPL license (http://www.gnu.org/licenses/lgpl.html)

 Software License Agreement:

 The software supplied herewith by Microchip Technology Incorporated
 (the ?Company?) for its PIC? Microcontroller is intended and
 supplied to you, the Company?s customer, for use solely and
 exclusively on Microchip PIC Microcontroller products. The
 software is owned by the Company and/or its supplier, and is
 protected under applicable copyright laws. All rights are reserved.
 Any use in violation of the foregoing restrictions may subject the
 user to criminal sanctions under applicable laws, as well as to
 civil liability for the breach of the terms and conditions of this
 license.

 THIS SOFTWARE IS PROVIDED IN AN ?AS IS? CONDITION. NO WARRANTIES,
 WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
 TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT,
 IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR
 CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 ******************************************************************************/

#include "p32xxxx.h"                            // Registers definitions
#include "typedefs.h"                           // UINT8, UINT32, ...
#include "config.h"                             // Config. bits
#include "mem.h"                                // Pinguino memory regions description
#include "hardware.h"                           // Pinguino boards hardware description
#include "flash.h"                              // Flash write and flash erase functions
#include "core.h"                               // MemCopy, MemClear, core timer functions
#include "delay.h"                              // Delayus
#include "usb.h"                                // USB device framework definitions
#include "debug.h"                              // Debug functions

#if defined(DEBUG)                              // defined in hardware.h
#include "serial.h"                             // UART functions
#endif

/*******************************************************************************
 * CONSTANTS
 ******************************************************************************/

//Switch State Variable Choices
#define	QUERY_DEVICE			0x02	//Command that the host uses to learn about the device (what regions can be programmed, and what type of memory is the region)
#define	UNLOCK_CONFIG			0x03	//Note, this command is used for both locking and unlocking the config bits (see the "//Unlock Configs Command Definitions" below)
#define ERASE_DEVICE			0x04	//Host sends this command to start an erase operation.  Firmware controls which pages should be erased.
//Sub-command for the ERASE_DEVICE command
//#define UNLOCKCONFIG			0x00    //Unlock Configs Command Definitions
//#define LOCKCONFIG			0x01	//lock Configs Command Definitions
#define PROGRAM_DEVICE			0x05	//If host is going to send a full DataBlockSize8 to be programmed, it uses this command.
#define	PROGRAM_COMPLETE		0x06	//If host send less than a DataBlockSize8 to be programmed, or if it wished to program whatever was left in the buffer, it uses this command.
#define GET_DATA			0x07	//The host sends this command in order to read out memory from the device.  Used during verify (and read/export hex operations)
#define	RESET_DEVICE			0x08	//Resets the microcontroller, so it can update the config bits (if they were programmed, and so as to leave the bootloader (and potentially go back into the main application)

//Query Device Response "Types" 
#define	TypeProgramMemory		0x01    //When the host sends a QUERY_DEVICE command, need to respond by populating a list of valid memory regions that exist in the device (and should be programmed)
#define TypeEEPROM			0x02
#define TypeConfigWords			0x03
#define	TypeEndOfTypeList		0xFF    //Sort of serves as a "null terminator" like number, which denotes the end of the memory region list has been reached.

//BootState Variable States
#define	IdleState			0x00
#define NotIdleState			0x01

//OtherConstants
#define InvalidAddress			0xFFFFFFFF

//Application and Microcontroller constants
#define DEVICE_FAMILY                   0x03    //0x01 for PIC18, 0x02 for PIC24, 0x03 for PIC32

#define	TotalPacketSize8                HID_INT_EP_SIZE
#define DataBlockSize8                  56      //Number of bytes in the "Data" field of a standard request to/from the PC.  Must be an even number from 2 to 56.
#define BufferSize32 			(DataBlockSize8/WORDSIZE)

/*******************************************************************************
 * TYPE DEFINITIONS
 ******************************************************************************/

typedef union __attribute__((packed)) _USB_HID_BOOTLOADER_COMMAND
{
    UINT8 Contents[TotalPacketSize8];

    //For GET_DATA / PROGRAM_DEVICE command
    struct __attribute__((packed))
    {
        UINT8  Command;
        UINT32 Address;
        UINT8  Size;
        UINT8  PadBytes[TotalPacketSize8 - 6 - DataBlockSize8];
        UINT32 Data32[BufferSize32];
    };

    //For QUERRY_DEVICE command
    struct __attribute__((packed))
    {
        UINT8  Command1;
        UINT8  DataSize;
        UINT8  DeviceFamily;
        UINT8  Type1;
        UINT32 Address1;
        UINT32 Length1;
        UINT8  Type2;
        UINT32 Address2;
        UINT32 Length2;
        UINT8  Type3; //End of sections list indicator goes here, when not programming the vectors, in that case fill with 0xFF.
        UINT32 Address3;
        UINT32 Length3;
        UINT8  Type4; //End of sections list indicator goes here, fill with 0xFF.
        UINT8  ExtraPadBytes[33];
    };
} USBPacket;

/*******************************************************************************
 * VARIABLES
 ******************************************************************************/

//64 bytes buffer for receiving packets on EP1 OUT from the PC
static USBPacket PacketFromPC;
//64 bytes buffer for sending packets on EP1 IN to the PC
static USBPacket PacketToPC;
static USBPacket PacketFromPCBuffer;

// USB_HANDLE is a pointer (void *) to an entry in the BDT.
static USB_HANDLE USBOutHandle = 0;
static USB_HANDLE USBInHandle = 0;
static UINT8  BootState;
static UINT32 ProgrammingBuffer32[BufferSize32];
static UINT8  BufferedDataIndex32;
static UINT32 ProgrammedPointer32;

USB_DEVICE_STATE USBDeviceState;

/*******************************************************************************
 * FUNCTION PROTOTYPES
 ******************************************************************************/

//void USBEventHandler(USB_EVENT);
       void USBEventHandler(void);
static void USBPacketHandler(void);
static void WriteFlashBlock(void);

/*******************************************************************************
 * MAIN PROGRAM ENTRY POINT
 ******************************************************************************/

int main(void)
{
    static UINT32 led_count = 0;
    //User App. hard-coded virtual memory address
    UINT32 *UserAppPtr;
    void (*UserApp)(void);

    UserAppPtr = (UINT32*)APP_RESET_ADDR;
    UserApp    = (void (*)(void))APP_RESET_ADDR;

    //Initialize Interrupts
    //EnableMultiVectoredInt();

    //Initialize LEDs and push button
    mLED_Init();
    mLED_Both_Off();
    mSWITCH_Init();

    #if defined(DEBUG)
        SerialInit(9600);
        SerialPrintChar(12); // CLS
        SerialPrint("*** PINGUINO ***\r\n");
        SerialPrint("BOOTLOADER v");
        SerialPrintNumber(USB_MAJOR_VER, 10);
        SerialPrint(".");
        SerialPrintNumber(USB_MINOR_VER, 10);
        SerialPrint(".");
        SerialPrintNumber(USB_DOT_VER, 10);
        SerialPrint("\r\n");
        SerialPrint("FCPU ");
        SerialPrintNumber(FCPU/1000000, 10);
        SerialPrint("MHz\r\n");
        SerialPrint("FPB  ");
        SerialPrintNumber(FPB/1000000, 10);
        SerialPrint("MHz\r\n");
        SerialPrintNumber(DATA_MEM_LENGTH/1024, 10);
        SerialPrint("K RAM SYSTEM\r\n");
        SerialPrintNumber(APP_PROGRAM_LENGTH/1024, 10);
        SerialPrint("K FLASH FREE\r\n");
        /*
        SerialPrint("0x");
        SerialPrintNumber(APP_EBASE_ADDR, 16);
        SerialPrint("\r\n");
        */
        SerialPrint("READY\r\n");
    #endif

    #if 0
    while(1)
    {
        mLED_1_Toggle();
        for (led_count = 0; led_count < 100; led_count++)
            Delayus(1000);
    }
    #endif

    //Call the user application if the switch button is not pressed.

    if (!mSWITCH_Pressed() && *UserAppPtr != InvalidAddress)
    {
        UserApp();
        //while(1);
    }

    // Initializes USB module SFRs and firmware
    USBDeviceInit();

    //Initialize the variable holding the handle for the last transmission
    USBOutHandle = 0;
    USBInHandle = 0;

    BootState = IdleState;
    ProgrammedPointer32 = InvalidAddress;
    BufferedDataIndex32 = 0;

    while (1)
    {
        // Check bus status and service USB interrupts.
        USBDeviceTasks();
        
        if (led_count == 0)
            led_count = 10000;

        led_count--;

        //Handle packets and blink the led
        //only if device is configured and not suspended
        //if ( !USBSuspendControl && USBDeviceState == CONFIGURED_STATE )
        if ( !U1PWRCbits.USUSPEND && USBDeviceState == CONFIGURED_STATE )
        {
            if (led_count == 0)
                mLED_1_Toggle();
            USBPacketHandler();
        }
    }
}

/*******************************************************************************
 * Bootloader commands routine.
 ******************************************************************************/

static void USBPacketHandler(void)
{
    UINT32 i;
    UINT8 nwords32;
    UINT8 index32;

    // *** RB20141210 : OK ***
    if (BootState == IdleState)
    {
        // Are we done sending the last response ?
        // Check USBOutHandle->STAT.UOWN
        if (!USBHandleBusy(USBInHandle))
        {
            // Did we receive a command ?
            // Check USBOutHandle->STAT.UOWN
            if (!USBHandleBusy(USBOutHandle))
            {
                // Make a copy of received data.
                // void Memcopy (void *from, void *to, UINT32 nbytes)
                MemCopy(&PacketFromPCBuffer, &PacketFromPC, TotalPacketSize8);

                // Restart receiver, to be ready for a next packet.
                USBOutHandle = USBTransferOnePacket(OUT_FROM_HOST, (UINT8*)&PacketFromPCBuffer);
                BootState = NotIdleState;

                //Initialize the next packet sent to the host
                MemClear(&PacketToPC, TotalPacketSize8);
            }
        }
    }

    else //(BootState Not in Idle State)
    {
        switch (PacketFromPC.Command)
        {

//******************************************************************************
            case QUERY_DEVICE:
//******************************************************************************

                // Prepare a response packet
                PacketToPC.Command1     = (UINT8)  QUERY_DEVICE;
                PacketToPC.DataSize     = (UINT8)  DataBlockSize8;
                PacketToPC.DeviceFamily = (UINT8)  DEVICE_FAMILY;
                PacketToPC.Type1        = (UINT8)  TypeProgramMemory;
                PacketToPC.Address1     = (UINT32) APP_PROGRAM_ADDR_START;
                PacketToPC.Length1      = (UINT32) APP_PROGRAM_LENGTH;
                PacketToPC.Type2        = (UINT8)  TypeProgramMemory;
                PacketToPC.Address2     = (UINT32) FCPU;
                PacketToPC.Length2      = (UINT32) FPB;
                PacketToPC.Type3        = (UINT8)  TypeProgramMemory;
                PacketToPC.Address3     = (UINT32) USB_MAJOR_VER;
                PacketToPC.Length3      = (UINT32) USB_MINOR_VER;
                PacketToPC.Type4        = (UINT8)  TypeEndOfTypeList;

                // Send the packet to the host
                if (!USBHandleBusy(USBInHandle))
                {
                    //#define USBTxOnePacket(ep,data,len)     USBTransferOnePacket(ep,IN_TO_HOST,data,len)
                    //USBInHandle = USBTxOnePacket(HID_EP, (UINT8*)&PacketToPC, TotalPacketSize8);
                    //USBInHandle = USBTransferOnePacket(HID_EP, IN_TO_HOST, (UINT8*)&PacketToPC, TotalPacketSize8);
                    USBInHandle = USBTransferOnePacket(IN_TO_HOST, (UINT8*)&PacketToPC);
                    BootState = IdleState;
                }
                break;

//******************************************************************************
            case GET_DATA:
//******************************************************************************

                // Prepare a response packet
                PacketToPC.Command = GET_DATA;
                PacketToPC.Address = PacketFromPC.Address;
                PacketToPC.Size = PacketFromPC.Size;

                nwords32 = PacketFromPC.Size / WORDSIZE;

                // void memcopy (void *from, void *to, UINT32 nbytes)
                // memcopy from PacketFromPC.Address to PacketToPC.Data32
                MemCopy( (void*) (ConvertFlashToVirtualAddress(PacketFromPC.Address)),
                         (void*) (PacketToPC.Data32 + BufferSize32 - nwords32),
                         nwords32 );

                if (!USBHandleBusy(USBInHandle))
                {
                    //USBInHandle = USBTxOnePacket(HID_EP, (UINT8*)&PacketToPC, TotalPacketSize8);
                    //USBInHandle = USBTransferOnePacket(HID_EP, IN_TO_HOST, (UINT8*)&PacketToPC, TotalPacketSize8);
                    USBInHandle = USBTransferOnePacket(IN_TO_HOST, (UINT8*)&PacketToPC);
                    BootState = IdleState;
                }
                break;

//******************************************************************************
            case ERASE_DEVICE:
//******************************************************************************

                //void* pFlash = (void*) UserAppMemStart;

                //for (i = 0; i < MaxPageToErase; i++)
                for (i = APP_PROGRAM_ADDR_START; i < APP_PROGRAM_ADDR_END; i = i + FLASH_PAGE_SIZE)
                {
                    //NVMErasePage(pFlash + (temp * FLASH_PAGE_SIZE));
                    //FlashErasePage(pFlash + (temp * FLASH_PAGE_SIZE));
                    //FlashErasePage((void*) UserAppMemStart + (p * FLASH_PAGE_SIZE));
                    //FlashOperation(FLASH_PAGE_ERASE, UserAppMemStart + i * FLASH_PAGE_SIZE, 0);
                    FlashOperation(FLASH_PAGE_ERASE,  (void*)i, 0);
                    //Call USBDriverService() periodically to prevent falling off
                    //the bus if any SETUP packets should happen to arrive.
                    USBDeviceTasks();
                }

                //Good practice to clear WREN bit anytime we are not expecting
                //to do erase/write operations, further reducing probability of accidental activation.
                //NVMCONbits.WREN = 0;
                //NVMCONCLR = _NVMCON_WREN_MASK;
                
                BootState = IdleState;
                break;

//******************************************************************************
            case PROGRAM_DEVICE:
//******************************************************************************

                // number of 32-bit words to write
                nwords32 = PacketFromPC.Size / WORDSIZE;

                if (ProgrammedPointer32 == InvalidAddress)
                    ProgrammedPointer32 = PacketFromPC.Address;

                if (ProgrammedPointer32 == PacketFromPC.Address)
                {
                    for (i = 0; i < nwords32; i++)
                    {
                        //Data field is right justified.
                        //Need to put it in the buffer left justified.

                        // BufferSize32=14, nwords32=14
                        // index32 from 0 to 13
                        index32 = BufferSize32 - nwords32 + i;

                        // BufferedDataIndex32 from 0 to 13
                        ProgrammingBuffer32[BufferedDataIndex32] = PacketFromPC.Data32[index32];
                        BufferedDataIndex32 += 1;// if 13 then 14
                        ProgrammedPointer32 += WORDSIZE;
                        //Call WriteFlashBlock() when buffer is full
                        if (BufferedDataIndex32 == BufferSize32)
                        {
                            WriteFlashBlock();
                        }
                    }
                }
                //else host sent us a non-contiguous packet address...  to make
                //this firmware simpler, host should not do this without sending
                //a PROGRAM_COMPLETE command in between program sections.
                BootState = IdleState;
                break;

//******************************************************************************
            case PROGRAM_COMPLETE:
//******************************************************************************

                WriteFlashBlock();
                //Reinitialize pointer to an invalid range, so we know the next
                //PROGRAM_DEVICE will be the start address of a contiguous section.
                ProgrammedPointer32 = InvalidAddress;
                BootState = IdleState;
                break;

//******************************************************************************
            case RESET_DEVICE:
//******************************************************************************

                // Disable the USB module and wait for the USB cable
                // capacitance to discharge down to disconnected (SE0) state.
                // Otherwise host might not realize we disconnected/reconnected
                // when we do the reset.
                U1CONCLR = 0xFF;
                Delayus(1000);
                SoftReset();
                break;

        }//End switch

    }//End if/else

}//End USBPacketHandler()

/*******************************************************************************
 * Notify uploader32.py that a USB event occured.
 ******************************************************************************/

void USBEventHandler(void)
{
    //enable the HID endpoint
    USBEnableEndpoint(HID_EP, USB_IN_ENABLED | USB_OUT_ENABLED | USB_HANDSHAKE_ENABLED | USB_DISALLOW_SETUP);
    //Arm the OUT endpoint for the first packet
    //USBOutHandle = USBTransferOnePacket(HID_EP, OUT_FROM_HOST, (UINT8*) &PacketFromPCBuffer, TotalPacketSize8);
    USBOutHandle = USBTransferOnePacket(OUT_FROM_HOST, (UINT8*) &PacketFromPCBuffer);
}

#if 0
void USBEventHandler(USB_EVENT event) //, void *pdata, UINT16 size)
{
    switch (event)
    {
        case EVENT_CONFIGURED:
            //USBCBInitEP();
            //enable the HID endpoint
            USBEnableEndpoint(HID_EP, USB_IN_ENABLED | USB_OUT_ENABLED | USB_HANDSHAKE_ENABLED | USB_DISALLOW_SETUP);
            //Arm the OUT endpoint for the first packet
            //USBOutHandle = HIDRxPacket(HID_EP, (UINT8*) &PacketFromPCBuffer, TotalPacketSize8);
            //USBOutHandle = USBRxOnePacket(HID_EP, (UINT8*) &PacketFromPCBuffer, TotalPacketSize8);
            USBOutHandle = USBTransferOnePacket(HID_EP, OUT_FROM_HOST, (UINT8*) &PacketFromPCBuffer, TotalPacketSize8);
            break;
        case EVENT_EP0_REQUEST:
            USBCheckHIDRequest();
            break;
        case EVENT_SET_DESCRIPTOR:
            //USBCBStdSetDscHandler();
            break;
        case EVENT_SOF:
            //USBCB_SOF_Handler();
            break;
        case EVENT_SUSPEND:
            //USBCBSuspend();
            break;
        case EVENT_RESUME:
            //USBCBWakeFromSuspend();
            break;
        case EVENT_BUS_ERROR:
            //USBCBErrorHandler();
            break;
        case EVENT_TRANSFER:
            Nop();
            break;
        default:
            break;
    }
    //return TRUE;
}
#endif

/*******************************************************************************
 * Write blocks of 32-bit words
 * from (ProgrammedPointer32 - 56) to ProgrammedPointer32
 ******************************************************************************/

static void WriteFlashBlock(void)
{
    UINT32 i = 0;

    while (BufferedDataIndex32)
    {
        FlashOperation(FLASH_WORD_WRITE,
                       (void*) ProgrammedPointer32 - BufferedDataIndex32 * WORDSIZE,
                       ProgrammingBuffer32[i++] );
        BufferedDataIndex32 -= 1;
    }

    Nop(); // Why ?
}
