/********************************************************************
 FileName:     main.c
 Dependencies: See INCLUDES section
 Processor:		PIC32MX USB Microcontrollers
 Hardware:		The code is natively intended to be used on the following
                                hardware platforms:
                                Explorer 16 + PIC32MX460F512L PIM.  The firmware may be
                                modified for use on other USB platforms by editing the
                                HardwareProfile.h file.
 Complier:  	Microchip C32 (for PIC32)
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

 ********************************************************************
 File Description:

 Change History:

  Rev   Description
  ----- ---------------------------------------------
  v2.2  Adapted from PIC18F87J50 HID Bootloader Firmware
        as basis for BootApplication().  The rest of the
        code was taken from the Simple HID Demo in 
        MCHPFSUSB v2.2.

  vx.x  Fixed race condition where an OUT packet could potentially
        clear the prior IN packet depending on the bus communication
        order.

 ********************************************************************/

#ifndef USBMOUSE_C
#define USBMOUSE_C

//#define TEST

/** CONFIGURATION **************************************************/
#if !defined(__PIC32MX__)
#error "This project was designed for PIC32MX family devices.  Please select the appropriate project for your target device family."
#endif

/** INCLUDES *******************************************************/
#include <p32xxxx.h>
#include "config.h"
#include "hardware.h"
#include "flash.h"
#include "usb/usb.h"
#include "usb/usb_function_hid.h"

/** C O N S T A N T S **********************************************************/

/**** configurations settings *************************************************/
// BMXPFMSZ<31:0>: Program Flash Memory (PFM) Size bits
// 0x00004000 = device has 16 KB Flash
// 0x00008000 = device has 32 KB Flash
// 0x00010000 = device has 64 KB Flash
// 0x00020000 = device has 128 KB Flash
#define MaxPageToEraseNoConfigs         BMXPFMSZ/FLASH_PAGE_SIZE
#define MaxPageToEraseWithConfigs       BMXPFMSZ/FLASH_PAGE_SIZE

// Physical address
#define ProgramMemStopNoConfigs		    0x1D000000 + BMXPFMSZ
#define ProgramMemStopWithConfigs	    0x1D000000 + BMXPFMSZ

/**** definitions *************************************************************/
//Switch State Variable Choices
#define	QUERY_DEVICE				    0x02	//Command that the host uses to learn about the device (what regions can be programmed, and what type of memory is the region)
#define	UNLOCK_CONFIG				    0x03	//Note, this command is used for both locking and unlocking the config bits (see the "//Unlock Configs Command Definitions" below)
#define ERASE_DEVICE				    0x04	//Host sends this command to start an erase operation.  Firmware controls which pages should be erased.
#define PROGRAM_DEVICE				    0x05	//If host is going to send a full RequestDataBlockSize to be programmed, it uses this command.
#define	PROGRAM_COMPLETE			    0x06	//If host send less than a RequestDataBlockSize to be programmed, or if it wished to program whatever was left in the buffer, it uses this command.
#define GET_DATA				        0x07	//The host sends this command in order to read out memory from the device.  Used during verify (and read/export hex operations)
#define	RESET_DEVICE				    0x08	//Resets the microcontroller, so it can update the config bits (if they were programmed, and so as to leave the bootloader (and potentially go back into the main application)

//Unlock Configs Command Definitions
#define UNLOCKCONFIG				    0x00	//Sub-command for the ERASE_DEVICE command
#define LOCKCONFIG				        0x01	//Sub-command for the ERASE_DEVICE command

//Query Device Response "Types" 
#define	TypeProgramMemory			    0x01    //When the host sends a QUERY_DEVICE command, need to respond by populating a list of valid memory regions that exist in the device (and should be programmed)
#define TypeEEPROM				        0x02
#define TypeConfigWords				    0x03
#define	TypeEndOfTypeList			    0xFF    //Sort of serves as a "null terminator" like number, which denotes the end of the memory region list has been reached.

//BootState Variable States
#define	IdleState				        0x00
#define NotIdleState				    0x01

//OtherConstants
#define InvalidAddress				    0xFFFFFFFF

//Application and Microcontroller constants
#define DEVICE_FAMILY                   0x03    //0x01 for PIC18, 0x02 for PIC24, 0x03 for PIC32

#define	TotalPacketSize                 0x40
#define WORDSIZE                        0x04    //PIC18 uses 2 byte instruction words, PIC24 uses 3 byte "instruction words" (which take 2 addresses, since each address is for a 16 bit word; the upper word contains a "phantom" byte which is unimplemented.).
#define RequestDataBlockSize            56      //Number of bytes in the "Data" field of a standard request to/from the PC.  Must be an even number from 2 to 56.
#define BufferSize 				        0x20    //32 16-bit words of buffer

/** PRIVATE PROTOTYPES *********************************************/

void BlinkUSBStatus(void);
void BootApplication(void);
void EraseFlash(void);
void WriteFlashSubBlock(void);
void __attribute__((nomips16)) PicReset(void);
//DWORD ReadProgramMemory(DWORD);

/** T Y P E  D E F I N I T I O N S ************************************/

typedef union __attribute__((packed)) _USB_HID_BOOTLOADER_COMMAND
{
    unsigned char Contents[64];

    struct __attribute__((packed))
    {
        unsigned char Command;
        WORD AddressHigh;
        WORD AddressLow;
        unsigned char Size;
        unsigned char PadBytes[(TotalPacketSize - 6) - (RequestDataBlockSize)];
        unsigned int Data[RequestDataBlockSize / WORDSIZE];
    };

    struct __attribute__((packed))
    {
        unsigned :8; //char Command;
        DWORD Address;
        //unsigned :8; //char Size;
        //unsigned char PadBytes[(TotalPacketSize - 6) - (RequestDataBlockSize)];
        //unsigned int Data[RequestDataBlockSize / WORDSIZE];
    };

    struct __attribute__((packed))
    {
        unsigned :8; //char Command;
        unsigned char PacketDataFieldSize;
        unsigned char DeviceFamily;
        unsigned char Type1;
        unsigned long Address1;
        unsigned long Length1;
        unsigned char Type2;
        unsigned long Address2;
        unsigned long Length2;
        unsigned char Type3; //End of sections list indicator goes here, when not programming the vectors, in that case fill with 0xFF.
        unsigned long Address3;
        unsigned long Length3;
        unsigned char Type4; //End of sections list indicator goes here, fill with 0xFF.
        unsigned char ExtraPadBytes[33];
    };

    struct __attribute__((packed))
    { //For lock/unlock config command
        unsigned :8; // char Command;
        unsigned char LockValue;
    };
}
PacketToFromPC;

/** VARIABLES ******************************************************/
#pragma udata

#pragma udata USB_VARS
PacketToFromPC PacketFromPC; //64 byte buffer for receiving packets on EP1 OUT from the PC
PacketToFromPC PacketToPC;   //64 byte buffer for sending packets on EP1 IN to the PC
PacketToFromPC PacketFromPCBuffer;

#pragma udata
USB_HANDLE USBOutHandle = 0;
USB_HANDLE USBInHandle = 0;

unsigned char MaxPageToErase;
unsigned long ProgramMemStopAddress;
unsigned char BootState;
unsigned char ErasePageTracker;
unsigned int ProgrammingBuffer[BufferSize];
unsigned char BufferedDataIndex;
unsigned long ProgrammedPointer;
unsigned char ConfigsProtected;


/** DECLARATIONS ***************************************************/
#pragma code

/********************************************************************
 * Function:        void main(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        Main program entry point.
 *
 * Note:            None
 *******************************************************************/

int main(void)
{
    #if defined(TEST)
    unsigned long led_count = 0;
    mLED_Init();
    while (1)
    {
        if (led_count == 0)
        {
            led_count = 0xFFFF;
            mLED_Toggle();
        }
        led_count--;
    }
    #else

    mLED_Init();
    mSWITCH_Init();

    if (SWITCH == 1)
    {
        void (*fptr)(void);
        fptr = (void (*)(void))BootMemStart;
        fptr();
    }

    //initialize the variable holding the handle for the last transmission
    USBOutHandle = 0;
    USBInHandle = 0;

    //Initialize bootloader state variables
    MaxPageToErase = MaxPageToEraseNoConfigs; //Assume we will not allow erase/programming of config words (unless host sends override command)
    ProgramMemStopAddress = ProgramMemStopNoConfigs;
    ConfigsProtected = LOCKCONFIG; //Assume we will not erase or program the vector table at first.  Must receive unlock config bits/vectors command first.
    BootState = IdleState;
    ProgrammedPointer = InvalidAddress;
    BufferedDataIndex = 0;

    // Initializes USB module SFRs and firmware
    USBDeviceInit(); //usb_device.c

    #if defined(USB_INTERRUPT)
    USBDeviceAttach();
    #endif

    while (1)
    {
        #if defined(USB_POLLING)
        // Check bus status and service USB interrupts.
        USBDeviceTasks();
        #endif

        //Blink USERLED according to the USB device status
        BlinkUSBStatus();

        // User Application USB tasks
        if ((USBDeviceState < CONFIGURED_STATE) || (USBSuspendControl == 1))
            return;

        BootApplication();
    }
    #endif
}//end main

/********************************************************************
 * Function:        void BlinkUSBStatus(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        BlinkUSBStatus turns on and off LEDs 
 *                  corresponding to the USB device state.
 *
 * Note:            mLED macros can be found in hardware.h
 *                  USBDeviceState is declared and updated in
 *                  usb_device.c.
 *******************************************************************/

void BlinkUSBStatus(void)
{
    static WORD led_count = 0;

    if (led_count == 0)
        led_count = 30000U;

    led_count--;

    if (USBSuspendControl == 1)
    {
        if (led_count == 0)
        {
            mLED_Toggle();
        }
    }

    else
    {
        if (USBDeviceState == DETACHED_STATE)
        {
            mLED_Off();
        }
        else if (USBDeviceState == ATTACHED_STATE)
        {
            mLED_On();
        }
        else if (USBDeviceState == POWERED_STATE)
        {
            mLED_On();
        }
        else if (USBDeviceState == DEFAULT_STATE)
        {
            mLED_On();
        }
        else if (USBDeviceState == ADDRESS_STATE)
        {
            if (led_count == 0)
            {
                mLED_Toggle();
            }
        }
        else if (USBDeviceState == CONFIGURED_STATE)
        {
            if (led_count == 0)
            {
                mLED_Toggle();
            }
        }
    }
}//end BlinkUSBStatus

void __attribute__((nomips16)) PicReset(void)
{
    #if 0
    int intStat;
    int dmaSusp;
    #endif
    volatile int* p = &RSWRST;

    #if 0
    #ifdef _DMAC
    intStat=INTDisableInterrupts();
    dmaSusp=DmaSuspend();
    #else
    intStat=INTDisableInterrupts();
    #endif
    #endif
    // Disable all interrupts
    asm("di");
    // Unlocking sequence
    SYSKEY = 0;
    SYSKEY = 0xAA996655;
    SYSKEY = 0x556699AA;
    // Soft Reset
    RSWRSTSET=_RSWRST_SWRST_MASK;
    *p;
    while(1);
}

/******************************************************************************
 * Function:        void ProcessIO(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This function is a place holder for other user routines.
 *                  It is a mixture of both USB and non-USB tasks.
 *
 * Note:            None
 *****************************************************************************/
void BootApplication(void)
{
    unsigned char i;
    unsigned int j;
    //DWORD_VAL FlashMemoryValue;

    if (BootState == IdleState)
    {
        //Are we done sending the last response.  We need to be before we 
        //  receive the next command because we clear the PacketToPC buffer
        //  once we receive a command
        if (!USBHandleBusy(USBInHandle))
        {
            if (!USBHandleBusy(USBOutHandle)) //Did we receive a command?
            {
                for (i = 0; i < TotalPacketSize; i++)
                {
                    PacketFromPC.Contents[i] = PacketFromPCBuffer.Contents[i];
                }

                USBOutHandle = USBRxOnePacket(HID_EP, (BYTE*) & PacketFromPCBuffer, 64);
                BootState = NotIdleState;

                //Prepare the next packet we will send to the host, by initializing the entire packet to 0x00.	
                for (i = 0; i < TotalPacketSize; i++)
                {
                    //This saves code space, since we don't have to do it independently in the QUERY_DEVICE and GET_DATA cases.
                    PacketToPC.Contents[i] = 0;
                }
            }
        }
    }
    else //(BootState must be in NotIdleState)
    {
        switch (PacketFromPC.Command)
        {

        case QUERY_DEVICE:
        {
            //Prepare a response packet, which lets the PC software know about the memory ranges of this device.

            PacketToPC.Command = (unsigned char) QUERY_DEVICE;
            PacketToPC.PacketDataFieldSize = (unsigned char) RequestDataBlockSize;
            PacketToPC.DeviceFamily = (unsigned char) DEVICE_FAMILY;

            PacketToPC.Type1 = (unsigned char) TypeProgramMemory;
            PacketToPC.Address1 = (unsigned long) ProgramMemStart;
            PacketToPC.Length1 = (unsigned long) (ProgramMemStopAddress - ProgramMemStart); //Size of program memory area
            PacketToPC.Type2 = (unsigned char) TypeEndOfTypeList;

            //				if(ConfigsProtected == UNLOCKCONFIG)
            //				{
            //    				PacketToPC.Type2 = (unsigned char)TypeProgramMemory;				//Overwrite the 0xFF end of list indicator if we wish to program the Vectors.
            //                    PacketToPC.Address2 = (unsigned long)VectorsStart;
            //                    PacketToPC.Length2 = (unsigned long)(VectorsEnd - VectorsStart);	//Size of program memory area
            //
            //                    PacketToPC.Type3 = (unsigned char)TypeConfigWords;
            //                    PacketToPC.Address3 = (unsigned long)ConfigWordsStartAddress;
            //                    PacketToPC.Length3 = (unsigned long)(ConfigWordsStopAddress - ConfigWordsStartAddress);
            //                    PacketToPC.Type4 = (unsigned char)TypeEndOfTypeList;
            //				}

            //Init pad bytes to 0x00...  Already done after we received the QUERY_DEVICE command (just after calling HIDRxPacket()).

            if (!USBHandleBusy(USBInHandle))
            {
                USBInHandle = USBTxOnePacket(HID_EP, (BYTE*) & PacketToPC, 64);
                BootState = IdleState;
            }
        }
        break;
        /*
        case UNLOCK_CONFIG:
        {
            if (PacketFromPC.LockValue == UNLOCKCONFIG)
            {
                MaxPageToErase = MaxPageToEraseWithConfigs; //Assume we will not allow erase/programming of config words (unless host sends override command)
                ProgramMemStopAddress = ProgramMemStopWithConfigs;
                ConfigsProtected = UNLOCKCONFIG;
            }
            else //LockValue must be == LOCKCONFIG
            {
                MaxPageToErase = MaxPageToEraseNoConfigs;
                ProgramMemStopAddress = ProgramMemStopNoConfigs;
                ConfigsProtected = LOCKCONFIG;
            }
            BootState = IdleState;
        }
        break;
        */
        case ERASE_DEVICE:
        {
            void* pFlash = (void*) ProgramMemStart;
            int temp;

            for (temp = 0; temp < (MaxPageToErase); temp++)
            {
                FlashErasePage(pFlash + (temp * FLASH_PAGE_SIZE));
                USBDeviceTasks(); //Call USBDriverService() periodically to prevent falling off the bus if any SETUP packets should happen to arrive.
            }

            NVMCONbits.WREN = 0; //Good practice to clear WREN bit anytime we are not expecting to do erase/write operations, further reducing probability of accidental activation.
            BootState = IdleState;
        }
            break;

        case PROGRAM_DEVICE:
        {
            if (ProgrammedPointer == (unsigned long) InvalidAddress)
                ProgrammedPointer = PacketFromPC.Address;

            if (ProgrammedPointer == (unsigned long) PacketFromPC.Address)
            {
                for (i = 0; i < (PacketFromPC.Size / WORDSIZE); i++)
                {
                    unsigned int index;

                    index = (RequestDataBlockSize - PacketFromPC.Size) / WORDSIZE + i;
                    ProgrammingBuffer[BufferedDataIndex] = PacketFromPC.Data[(RequestDataBlockSize - PacketFromPC.Size) / WORDSIZE + i]; //Data field is right justified.  Need to put it in the buffer left justified.
                    BufferedDataIndex++;
                    ProgrammedPointer += WORDSIZE;
                    if (BufferedDataIndex == (RequestDataBlockSize / WORDSIZE)) //Need to make sure it doesn't call WriteFlashSubBlock() unless BufferedDataIndex/2 is an integer
                    {
                        WriteFlashSubBlock();
                    }
                }
            }
            //else host sent us a non-contiguous packet address...  to make this firmware simpler, host should not do this without sending a PROGRAM_COMPLETE command in between program sections.
            BootState = IdleState;
        }
        break;

        case PROGRAM_COMPLETE:
        {
            WriteFlashSubBlock();
            ProgrammedPointer = InvalidAddress; //Reinitialize pointer to an invalid range, so we know the next PROGRAM_DEVICE will be the start address of a contiguous section.
            BootState = IdleState;
        }
        break;

        case GET_DATA:
        {
            if (!USBHandleBusy(USBInHandle))
            {
                //Init pad bytes to 0x00...  Already done after we received the QUERY_DEVICE command (just after calling HIDRxReport()).
                PacketToPC.Command = GET_DATA;
                PacketToPC.Address = PacketFromPC.Address;
                PacketToPC.Size = PacketFromPC.Size;

                for (i = 0; i < (PacketFromPC.Size / WORDSIZE); i++)
                {
                    DWORD* p;
                    DWORD data;

                    p = ((DWORD*) ((PacketFromPC.Address + (i * WORDSIZE)) | 0x80000000));
                    data = *p;

                    PacketToPC.Data[RequestDataBlockSize / WORDSIZE + i - PacketFromPC.Size / WORDSIZE] = *((DWORD*) ((PacketFromPC.Address + (i * WORDSIZE)) | 0x80000000));
                }

                USBInHandle = USBTxOnePacket(HID_EP, (BYTE*) & PacketToPC.Contents[0], 64);
                BootState = IdleState;
            }
        }
        break;

        case RESET_DEVICE:
        {
            U1CON = 0x0000; //Disable USB module
            //And wait awhile for the USB cable capacitance to discharge down to disconnected (SE0) state.
            //Otherwise host might not realize we disconnected/reconnected when we do the reset.
            //A basic for() loop decrementing a 16 bit number would be simpler, but seems to take more code space for
            //a given delay.  So do this instead:
            for (j = 0; j < 0xFFFF; j++)
            {
                Nop();
            }
            
            PicReset();
        }
        break;

        }//End switch
    }//End if/else

}//End ProcessIO()

void WriteFlashSubBlock(void) //Use word writes to write code chunks less than a full 64 byte block size.
{
    unsigned int i = 0;

    DWORD_VAL Address;
    while (BufferedDataIndex > 0) //While data is still in the buffer.
    {
        Address.Val = (ProgrammedPointer - (BufferedDataIndex * WORDSIZE));
        FlashWriteWord((DWORD*) Address.Val, (unsigned int) ProgrammingBuffer[i++]);
        BufferedDataIndex = BufferedDataIndex - 1; //Used up 2 (16-bit) words from the buffer.
    }

    Nop();
}

/*******************************************************************
 * Function:        void USBCBInitEP(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This function is called when the device becomes
 *                  initialized, which occurs after the host sends a
 * 					SET_CONFIGURATION (wValue not = 0) request.  This 
 *					callback function should initialize the endpoints 
 *					for the device's usage according to the current 
 *					configuration.
 *
 * Note:            None
 *******************************************************************/
void USBCBInitEP(void)
{
    //enable the HID endpoint
    USBEnableEndpoint(HID_EP, USB_IN_ENABLED | USB_OUT_ENABLED | USB_HANDSHAKE_ENABLED | USB_DISALLOW_SETUP);
    //Arm the OUT endpoint for the first packet
    USBOutHandle = HIDRxPacket(HID_EP, (BYTE*) & PacketFromPCBuffer, 64);
}

/********************************************************************
 * Function:        void USBCBSendResume(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        The USB specifications allow some types of USB
 * 					peripheral devices to wake up a host PC (such
 *					as if it is in a low power suspend to RAM state).
 *					This can be a very useful feature in some
 *					USB applications, such as an Infrared remote
 *					control	receiver.  If a user presses the "power"
 *					button on a remote control, it is nice that the
 *					IR receiver can detect this signalling, and then
 *					send a USB "command" to the PC to wake up.
 *					
 *					The USBCBSendResume() "callback" function is used
 *					to send this special USB signalling which wakes 
 *					up the PC.  This function may be called by
 *					application firmware to wake up the PC.  This
 *					function should only be called when:
 *					
 *					1.  The USB driver used on the host PC supports
 *						the remote wakeup capability.
 *					2.  The USB configuration descriptor indicates
 *						the device is remote wakeup capable in the
 *						bmAttributes field.
 *					3.  The USB host PC is currently sleeping,
 *						and has previously sent your device a SET 
 *						FEATURE setup packet which "armed" the
 *						remote wakeup capability.   
 *
 *					This callback should send a RESUME signal that
 *                  has the period of 1-15ms.
 *
 * Note:            Interrupt vs. Polling
 *                  -Primary clock
 *                  -Secondary clock ***** MAKE NOTES ABOUT THIS *******
 *                   > Can switch to primary first by calling USBCBWakeFromSuspend()
 
 *                  The modifiable section in this routine should be changed
 *                  to meet the application needs. Current implementation
 *                  temporary blocks other functions from executing for a
 *                  period of 1-13 ms depending on the core frequency.
 *
 *                  According to USB 2.0 specification section 7.1.7.7,
 *                  "The remote wakeup device must hold the resume signaling
 *                  for at lest 1 ms but for no more than 15 ms."
 *                  The idea here is to use a delay counter loop, using a
 *                  common value that would work over a wide range of core
 *                  frequencies.
 *                  That value selected is 1800. See table below:
 *                  ==========================================================
 *                  Core Freq(MHz)      MIP         RESUME Signal Period (ms)
 *                  ==========================================================
 *                      48              12          1.05
 *                       4              1           12.6
 *                  ==========================================================
 *                  * These timing could be incorrect when using code
 *                    optimization or extended instruction mode,
 *                    or when having other interrupts enabled.
 *                    Make sure to verify using the MPLAB SIM's Stopwatch
 *                    and verify the actual signal on an oscilloscope.
 *******************************************************************/
/*
void USBCBSendResume(void)
{
    static WORD delay_count;

    USBResumeControl = 1; // Start RESUME signaling

    delay_count = 1800U; // Set RESUME line for 1-13 ms
    do
    {
        delay_count--;
    }
    while (delay_count);
    USBResumeControl = 0;
}
*/
/*******************************************************************
 * Function:        BOOL USER_USB_CALLBACK_EVENT_HANDLER(
 *                        USB_EVENT event, void *pdata, WORD size)
 *
 * PreCondition:    None
 *
 * Input:           USB_EVENT event - the type of event
 *                  void *pdata - pointer to the event data
 *                  WORD size - size of the event data
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This function is called from the USB stack to
 *                  notify a user application that a USB event
 *                  occured.  This callback is in interrupt context
 *                  when the USB_INTERRUPT option is selected.
 *
 * Note:            None
 *******************************************************************/

BOOL USER_USB_CALLBACK_EVENT_HANDLER(USB_EVENT event, void *pdata, WORD size)
{
    switch (event)
    {
        case EVENT_CONFIGURED:
            USBCBInitEP();
            break;
        /*
        case EVENT_SET_DESCRIPTOR:
            USBCBStdSetDscHandler();
            break;
        */
        case EVENT_EP0_REQUEST:
            USBCheckHIDRequest();
            break;
        /*
        case EVENT_SOF:
            USBCB_SOF_Handler();
            break;
        case EVENT_SUSPEND:
            USBCBSuspend();
            break;
        case EVENT_RESUME:
            USBCBWakeFromSuspend();
            break;
        case EVENT_BUS_ERROR:
            USBCBErrorHandler();
            break;
        */
        case EVENT_TRANSFER:
            Nop();
            break;
        default:
            break;
    }
    return TRUE;
}

/** EOF main.c *************************************************/
#endif
