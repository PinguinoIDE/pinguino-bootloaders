/********************************************************************
 FileName:     main.c
 Dependencies: See INCLUDES section
 Processor:    PIC32MX USB Microcontrollers
 Hardware:     The code is natively intended to be used on Pinguino boards
               The firmware may be modified for use on other USB platforms
               by editing the hardware.h file.
 Compiler:     XC32 (for PIC32)

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

 ********************************************************************/

#if !defined(__PIC32MX__)
#error "This project was designed for PIC32MX family devices."
#endif

#include "p32xxxx.h"
#include "GenericTypeDefs.h"
#include "config.h"
#include "hardware.h"
#include "flash.h"
#include "USB/usb.h"
#include "USB/usb_function_hid.h"

//#include <plib.h> // nvm.h & reset.h
//#include <peripheral/nvm.h>

//BMXDRMSZ: Data RAM Size Register
//BMXPFMSZ: Program Flash Size Register
//BMXBOOTSZ: Boot Flash Size Register

#if defined(__32MX270F256B__)
    // seems BMXPFMSZ returns a bad value for this proc.
    #define UserAppMemEnd     		0x1D040000
#else
    #define UserAppMemEnd     		0x1D000000 + BMXPFMSZ
#endif

//#define ProgramMemStopNoConfigs       0x1D000000 + BMXPFMSZ
//#define ProgramMemStopWithConfigs	0x1D000000 + BMXPFMSZ
#define UserAppMemFree                  UserAppMemEnd - UserAppMemStart
#define MaxPageToErase                  UserAppMemFree / FLASH_PAGE_SIZE
//#define MaxPageToErase                BMXPFMSZ/FLASH_PAGE_SIZE		 //Last full page of flash on the PIC24FJ256GB110, which does not contain the flash configuration words.
//#define MaxPageToEraseNoConfigs       BMXPFMSZ/FLASH_PAGE_SIZE		 //Last full page of flash on the PIC24FJ256GB110, which does not contain the flash configuration words.
//#define MaxPageToEraseWithConfigs     BMXPFMSZ/FLASH_PAGE_SIZE		 //Page 170 contains the flash configurations words on the PIC24FJ256GB110.  Page 170 is also smaller than the rest of the (1536 byte) pages.

/**** definitions *************************************************************/

//Unlock Configs Command Definitions
#define UNLOCKCONFIG			0x00	//Sub-command for the ERASE_DEVICE command
#define LOCKCONFIG			0x01	//Sub-command for the ERASE_DEVICE command

//Switch State Variable Choices
#define	QUERY_DEVICE			0x02	//Command that the host uses to learn about the device (what regions can be programmed, and what type of memory is the region)
#define	UNLOCK_CONFIG			0x03	//Note, this command is used for both locking and unlocking the config bits (see the "//Unlock Configs Command Definitions" below)
#define ERASE_DEVICE			0x04	//Host sends this command to start an erase operation.  Firmware controls which pages should be erased.
#define PROGRAM_DEVICE			0x05	//If host is going to send a full DataBlockSize to be programmed, it uses this command.
#define	PROGRAM_COMPLETE		0x06	//If host send less than a DataBlockSize to be programmed, or if it wished to program whatever was left in the buffer, it uses this command.
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

#define WORDSIZE                        4       // unsigned int = 32 bits = 4 bytes
#define	TotalPacketSize                 64
#define DataBlockSize                   56      //Number of bytes in the "Data" field of a standard request to/from the PC.  Must be an even number from 2 to 56.
#define BufferSize 			DataBlockSize/WORDSIZE      //32 16-bit words of buffer

/** PRIVATE PROTOTYPES *********************************************/
//BOOL Switch2IsPressed(void);
//BOOL Switch3IsPressed(void);
//void Emulate_Mouse(void);

//static void InitializeSystem(void);
//void ProcessIO(void);
//void UserInit(void);
//void YourHighPriorityISRCode();
//void YourLowPriorityISRCode();
//void EraseFlash(void);

void BlinkUSBStatus(void);
void WriteFlash(void);
void HandlePacket(void);
void DelayUs (UINT32);

void __attribute__((nomips16)) SoftReset(void);
UINT32 __attribute__((nomips16)) ReadCoreTimer(void);
void __attribute__((nomips16)) ResetCoreTimer(void);
void __attribute__((nomips16)) FlashOperation(UINT32, UINT32, UINT32);

void memclear (void *, UINT32);
void memcopy (void *, void *, UINT32);

//UINT32 ReadProgramMemory(UINT32);

/** T Y P E  D E F I N I T I O N S ************************************/

typedef union __attribute__((packed)) _USB_HID_BOOTLOADER_COMMAND
{
    UINT8 Contents[TotalPacketSize];

    //For ??? command
    /*
    struct __attribute__((packed))
    {
        UINT8 Command;
        WORD AddressHigh;
        WORD AddressLow;
        UINT8 Size;
        UINT8 PadBytes[(TotalPacketSize - 6) - (DataBlockSize)];
        UINT16 Data[DataBlockSize / WORDSIZE];
    };
    */
    //For GET_DATA / PROGRAM_DEVICE command
    struct __attribute__((packed))
    {
        UINT8  Command;
        UINT32 Address;
        UINT8  Size;
        UINT8  PadBytes[TotalPacketSize - 6 - DataBlockSize];
        UINT32 Data[BufferSize];
    };

    //For QUERRY_DEVICE command
    struct __attribute__((packed))
    {
        UINT8  Command1;
        UINT8  PacketDataFieldSize;
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

    //For lock/unlock config command
    /*
    struct __attribute__((packed))
    {
        UINT8 Command;
        UINT8 LockValue;
    };
    */
}
USBPacket;

/** VARIABLES ******************************************************/
//#pragma udata

//#pragma udata USB_VARS
USBPacket PacketFromPC; //64 byte buffer for receiving packets on EP1 OUT from the PC
USBPacket PacketToPC; //64 byte buffer for sending packets on EP1 IN to the PC
USBPacket PacketFromPCBuffer;

//#pragma udata
USB_HANDLE USBOutHandle = 0;
USB_HANDLE USBInHandle = 0;
//BOOL blinkStatusValid = TRUE;

//UINT8 MaxPageToErase;
//UINT32 ProgramMemStopAddress;
//UINT8 ErasePageTracker;
UINT8  BootState;
UINT32 ProgrammingBuffer[BufferSize];
UINT8  BufferedDataIndex;
UINT32 ProgrammedPointer;
//UINT8 ConfigsProtected;

//#pragma code

/********************************************************************
 * Main program entry point.
 *******************************************************************/

int main(void)
{
    //void (*fptr)(void) = (void (*)(void))BootMemStart;
    void (*fptr)(void) = (void (*)(void))(PA_TO_KVA0(UserAppMemStart));

    //Initialize all of the LED pins
    mLED_Init();
    mLED_Both_Off();
    //Initialize all of the push buttons
    mSWITCH_Init();

    //Call the user application if the switch button is not pressed
    //the bootloader jumps to a hard-coded virtual memory address

    if (!mSWITCH_Pressed())
    {
        fptr();
        while(1);
    }

    //If the switch button is being pressed, the PIC attempts to establish
    //communication with the Pinguino uploader on you computer.
    //The uploader will send over new .hex program,
    //and the PIC will write the program into KSEG0 program flash
    //beginning at 0x9D001000.
    //The next time the PIC is reset (and the button is not being pressed),
    //it will begin executing the program you loaded.

    #if 0
    #if defined(USE_USB_BUS_SENSE_IO)
    tris_usb_bus_sense = INPUT_PIN; // See HardwareProfile.h
    #endif

    #if defined(USE_SELF_POWER_SENSE_IO)
    tris_self_power = INPUT_PIN; // See HardwareProfile.h
    #endif
    #endif

    // Initializes USB module SFRs and firmware
    USBDeviceInit(); //usb_device.c

    //Initialize the variable holding the handle for the last transmission
    USBOutHandle = 0;
    USBInHandle = 0;

    //blinkStatusValid = TRUE;

    //Initialize bootloader state variables
    //MaxPageToErase = MaxPageToEraseNoConfigs; //Assume we will not allow erase/programming of config words (unless host sends override command)
    //ProgramMemStopAddress = ProgramMemStopNoConfigs;
    //ProgramMemStopAddress = ProgramMemStop;
    //ConfigsProtected = LOCKCONFIG; //Assume we will not erase or program the vector table at first.  Must receive unlock config bits/vectors command first.
    BootState = IdleState;
    ProgrammedPointer = InvalidAddress;
    BufferedDataIndex = 0;

    #if defined(USB_INTERRUPT)
    USBDeviceAttach();
    #endif

    while (1)
    {
        #if defined(USB_POLLING)
        // Check bus status and service USB interrupts.
        USBDeviceTasks();
        #endif

        //Blink the LEDs according to the USB device status
        BlinkUSBStatus();

        // If the device is not configured or is suspended
        // we don't want to execute any application code
        // so we go back to the top of the while loop
        if ( (USBSuspendControl == 1) || (USBDeviceState < CONFIGURED_STATE) )
            continue;

        //Otherwise we are free to handle packets
        HandlePacket();
    }
}

/********************************************************************
 * BlinkUSBStatus turns on and off LEDs 
 * corresponding to the USB device state.
 *******************************************************************/

void BlinkUSBStatus(void)
{
    static UINT16 led_count = 0;

    if (led_count == 0)
        led_count = 10000;
    
    led_count--;

    #if 1
    // If the device is configured and not suspended
    if ( (!USBSuspendControl) && (USBDeviceState == CONFIGURED_STATE) )
    {
        if (led_count == 0)
            mLED_1_Toggle();
    }
    #else
    if (USBSuspendControl == 1)
    {
        if (led_count == 0)
        {
            mLED_1_Off();
            mLED_2_Toggle();
            //mLED_Toggle();
        }
    }
    else
    {
        if (USBDeviceState == DETACHED_STATE)
        {
            mLED_Both_Off();
        }
        else if (USBDeviceState == ATTACHED_STATE)
        {
            mLED_Both_On();
        }
        else if (USBDeviceState == POWERED_STATE)
        {
            mLED_Only_1_On();
        }
        else if (USBDeviceState == DEFAULT_STATE)
        {
            mLED_Only_2_On();
        }
        else if (USBDeviceState == ADDRESS_STATE)
        {
            if (led_count == 0)
            {
                mLED_1_Toggle();
                mLED_2_Off();
                //mLED_Toggle();
            }
        }
        else if (USBDeviceState == CONFIGURED_STATE)
        {
            if (led_count == 0)
            {
                //mLED_1_Toggle();
                //mLED_2_Toggle();       // Alternate blink
                mLED_Toggle();
            }
        }
    }
    #endif
}

/******************************************************************************
 * Bootloader commands routines.
 *****************************************************************************/

void HandlePacket(void)
{
    UINT8 i;
    UINT16 j;

    if (BootState == IdleState)
    {
        // Are we done sending the last response.  We need to be before we
        // receive the next command because we clear the PacketToPC buffer
        // once we receive a command
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
    else //(BootState Not in Idle State)
    {
        switch (PacketFromPC.Command)
        {

            case QUERY_DEVICE:
            {
                //Prepare a response packet, which lets the PC software know
                //about the memory ranges of this device.

                PacketToPC.Command = (UINT8) QUERY_DEVICE;
                PacketToPC.PacketDataFieldSize = (UINT8) DataBlockSize;
                PacketToPC.DeviceFamily = (UINT8) DEVICE_FAMILY;
                PacketToPC.Type1 = (UINT8) TypeProgramMemory;
                PacketToPC.Address1 = (UINT32) UserAppMemStart;
                PacketToPC.Length1 = (UINT32) UserAppMemFree;
                PacketToPC.Type2 = (UINT8) TypeEndOfTypeList;

                /*
                if(ConfigsProtected == UNLOCKCONFIG)
                {
                    PacketToPC.Type2 = (UINT8)TypeProgramMemory;				//Overwrite the 0xFF end of list indicator if we wish to program the Vectors.
                    PacketToPC.Address2 = (UINT32)VectorsStart;
                    PacketToPC.Length2 = (UINT32)(VectorsEnd - VectorsStart);	//Size of program memory area
                    PacketToPC.Type3 = (UINT8)TypeConfigWords;
                    PacketToPC.Address3 = (UINT32)ConfigWordsStartAddress;
                    PacketToPC.Length3 = (UINT32)(ConfigWordsStopAddress - ConfigWordsStartAddress);
                    PacketToPC.Type4 = (UINT8)TypeEndOfTypeList;
                }
                */

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
                    //MaxPageToErase = MaxPageToEraseWithConfigs; //Assume we will not allow erase/programming of config words (unless host sends override command)
                    //ProgramMemStopAddress = ProgramMemStopWithConfigs;
                    //ProgramMemStopAddress = ProgramMemStop;
                    ConfigsProtected = UNLOCKCONFIG;
                }
                else //LockValue must be == LOCKCONFIG
                {
                    //MaxPageToErase = MaxPageToEraseNoConfigs;
                    //ProgramMemStopAddress = ProgramMemStopNoConfigs;
                    //ProgramMemStopAddress = ProgramMemStop;
                    ConfigsProtected = LOCKCONFIG;
                }
                BootState = IdleState;
            }
            break;
            */
            case ERASE_DEVICE:
            {
                //void* pFlash = (void*) UserAppMemStart;

                for (j = 0; j < MaxPageToErase; j++)
                {
                    //NVMErasePage(pFlash + (temp * FLASH_PAGE_SIZE));
                    //FlashErasePage(pFlash + (temp * FLASH_PAGE_SIZE));
                    //FlashErasePage((void*) UserAppMemStart + (p * FLASH_PAGE_SIZE));
                    FlashOperation(FLASH_PAGE_ERASE, UserAppMemStart + j * FLASH_PAGE_SIZE, 0);
                    //Call USBDriverService() periodically to prevent falling off
                    //the bus if any SETUP packets should happen to arrive.
                    USBDeviceTasks();
                }

                //Good practice to clear WREN bit anytime we are not expecting
                //to do erase/write operations, further reducing probability of accidental activation.
                //NVMCONbits.WREN = 0;
                //NVMCONCLR = _NVMCON_WREN_MASK;
                
                BootState = IdleState;
            }
            break;

            case PROGRAM_DEVICE:
            {
while (1)
{
    mLED_1_Toggle();
    DelayUs(500);
}
                if (ProgrammedPointer == (UINT32) InvalidAddress)
                    ProgrammedPointer = PacketFromPC.Address;

                if (ProgrammedPointer == (UINT32) PacketFromPC.Address)
                {
                    for (i = 0; i < (PacketFromPC.Size / WORDSIZE); i++)
                    {
                        UINT8 index;

                        //Data field is right justified.  Need to put it in the buffer left justified.
                        index = BufferSize - (PacketFromPC.Size / WORDSIZE) + i;
                        ProgrammingBuffer[BufferedDataIndex++] = PacketFromPC.Data[index];
                        ProgrammedPointer += WORDSIZE;
                        //Need to make sure we call WriteFlash() when buffer is full
                        if (BufferedDataIndex == BufferSize)
                        {
                            WriteFlash();
                        }
                    }
                }
                //else host sent us a non-contiguous packet address...  to make
                //this firmware simpler, host should not do this without sending
                //a PROGRAM_COMPLETE command in between program sections.
                BootState = IdleState;
            }
            break;

            case PROGRAM_COMPLETE:
            {
                WriteFlash();
                //Reinitialize pointer to an invalid range, so we know the next
                //PROGRAM_DEVICE will be the start address of a contiguous section.
                ProgrammedPointer = InvalidAddress;
                BootState = IdleState;
            }
            break;

            case GET_DATA:
            {
                if (!USBHandleBusy(USBInHandle))
                {
                    //Init pad bytes to 0x00...  Already done after we received
                    //the QUERY_DEVICE command (just after calling HIDRxReport()).
                    PacketToPC.Command = GET_DATA;
                    PacketToPC.Address = PacketFromPC.Address;
                    PacketToPC.Size = PacketFromPC.Size;

                    // memcopy from PacketFromPC.Address to PacketToPC.Data
                    for (i = 0; i < (PacketFromPC.Size / WORDSIZE); i++)
                    {
                        UINT32* p;
                        //UINT32 data;
                        UINT8 index;

                        index = BufferSize - (PacketFromPC.Size / WORDSIZE) + i;
                        //index = DataBlockSize / WORDSIZE + i - PacketFromPC.Size / WORDSIZE;

                        p = ((UINT32*) ((PacketFromPC.Address + (i * WORDSIZE)) | 0x80000000));
                        //data = *p;

                        //PacketToPC.Data[DataBlockSize / WORDSIZE + i - PacketFromPC.Size / WORDSIZE] = *((UINT32*) ((PacketFromPC.Address + (i * WORDSIZE)) | 0x80000000));
                        PacketToPC.Data[index] = *p;
                    }

                    USBInHandle = USBTxOnePacket(HID_EP, (BYTE*) & PacketToPC.Contents[0], 64);
                    BootState = IdleState;
                }

            }
            break;

            case RESET_DEVICE:
            {
                U1CON = 0x0000; //Disable USB module
                //Wait awhile for the USB cable capacitance to discharge down
                //to disconnected (SE0) state. Otherwise host might not realize
                //we disconnected/reconnected when we do the reset.
                DelayUs(1000);

                //Reset();
                SoftReset();
            }
            break;

        }//End switch

    }//End if/else

}//End HandlePacket()

/*******************************************************************
 * Write blocks of 32-bit word
 *******************************************************************/

void WriteFlash(void)
{
    UINT32 i = 0;
    //UINT32* address;

    while (BufferedDataIndex--)
    {
        //address = (ProgrammedPointer - (BufferedDataIndex * WORDSIZE));
        //NVMWriteWord((UINT32*) Address.Val, (UINT16) ProgrammingBuffer[i++]);
        //FlashWriteWord((UINT32*) Address.Val, (UINT16) ProgrammingBuffer[i++]);
        //FlashOperation(FLASH_WORD_WRITE, address, ProgrammingBuffer[i++]);

        FlashOperation(FLASH_WORD_WRITE, ProgrammedPointer - BufferedDataIndex * WORDSIZE, ProgrammingBuffer[i++]);
    }

    Nop();
}

/*******************************************************************
 * Triggers the software reset
 *******************************************************************/

void __attribute__((nomips16)) SoftReset(void)
{
    UINT16 status;

    // Suspend or Disable all Interrupts
    asm volatile ("di");

    if (! (DMACON & _DMACON_SUSPEND_MASK) )
    {
        DMACONSET = _DMACON_SUSPEND_MASK;
        while (DMACON & _DMACON_DMABUSY_MASK);
    }

    // Step 1 - Execute "unlock" sequence to access the RSWRST register.
    SYSKEY = 0;
    SYSKEY = 0xAA996655;
    SYSKEY = 0x556699AA;

    // Step 2 - Write a '1' to RSWRST.SWRST bit to arm the software reset.
    RSWRSTSET = _RSWRST_SWRST_MASK;

    // Step 3 - A Read of the RSWRST register must follow the write.
    // This action triggers the software reset, which should occur on the next clock cycle.
    (void) RSWRST;

    // Note: The read instruction must be following with either 4 nop
    // instructions (fills the instruction pipe) or a while(1)loop to
    // ensure no instructions can access the bus before reset occurs.
    while (1);
}

/*******************************************************************
 * Microsecond delay routine for MIPS processor.
 * mips_read_c0_register (C0_COUNT, 0) = _CP0_GET_COUNT()
 * #define _CP0_COUNT             9
 * #define _CP0_COUNT_SELECT      0
 * #define _CP0_GET_COUNT()       _mfc0 (_CP0_COUNT, _CP0_COUNT_SELECT)
 * #define _mfc0(rn, sel)         __builtin_mfc0(rn,sel)
 * asm volatile("mfc0   %0, $9" : "=r"(timer))
 *******************************************************************/

void __attribute__((nomips16)) ResetCoreTimer(void)
{
    asm volatile("mtc0 %0, $9" : : "r"(0));
}

UINT32 __attribute__((nomips16)) ReadCoreTimer(void)
{
    UINT32 timer;
    asm volatile("mfc0 %0, $9" : "=r"(timer) :);
    return timer;
}

void DelayUs (UINT32 usec)
{
    UINT32 stop;
    ResetCoreTimer();
    // CP0Count counts at half the CPU rate
    stop = usec * 1000 * FCPU_MHZ / 2;
    while (ReadCoreTimer() < stop);
}

/********************************************************************
* This function must generate MIPS32
********************************************************************/

//UINT16 FlashOperation(UINT16 operation, void* address, UINT16 data)
void __attribute__((nomips16)) FlashOperation(UINT32 op, UINT32 addr, UINT32 data)
{
    UINT32 status;

    #if 1
    if (op == FLASH_WORD_WRITE) mLED_1_Toggle();
    if (op == FLASH_PAGE_ERASE) mLED_2_Toggle();
    #endif

    // Convert Virtual Address to Physical Address
    // as NVMADDR only accept Physical Address
    //NVMADDR = KVA_TO_PA((UINT16)addr);
    //NVMADDR = (UINT32)address & 0x1FFFFFFF;
    //NVMADDR = KVA_TO_PA(addr);
    NVMADDR = addr & 0x1FFFFFFF;

    // Load data into NVMDATA register
    NVMDATA = data;

    // Suspend or Disable all Interrupts
    //#if 0
    asm volatile ("di %0" : "=r" (status));
    //#endif

    // Disable Flash Write/Erase operations
    // to allow operation bits to be writable
    NVMCONCLR = _NVMCON_WREN_MASK;

    // Enable Flash Write/Erase Operations
    // Enables LVD circuit
    // Select Flash operation to perform
    // Note: operation bits are writable when WREN = 0 (bit 14).
    NVMCON = _NVMCON_WREN_MASK | op;

    // Data sheet prescribes 6us delay for LVD to become stable.
    // To be on the safer side, we shall set 7us delay.
    DelayUs(7);

    // Write Keys
    NVMKEY = 0xAA996655;
    NVMKEY = 0x556699AA;

    // Start the operation (WR=1)
    NVMCONSET = _NVMCON_WR_MASK;

    // Wait for operation to complete (WR=0)
    while (NVMCON & _NVMCON_WR_MASK);

    // Disable Flash Write/Erase operations
    NVMCONCLR = _NVMCON_WREN_MASK;

    // Restore Interrupts if necessary
    if (status & 0x00000001)
    {
        asm volatile ("ei");
    }
    #if 0
    else
    {
        asm volatile ("di");
    }
    #endif

    //#if 0
    // Return NVMERR and LVDERR Error Status Bits
    if (NVMCON & (_NVMCON_WRERR_MASK | _NVMCON_LVDERR_MASK))
    {
        while (1)
        {
            mLED_2_Toggle();
            DelayUs(500);
        }
    }
    //#endif
}

/*******************************************************************
 * Clear an array.
 *******************************************************************/

void memclear(void *address, UINT32 nbytes)
{
    UINT32* wordp = (UINT32*) address;
    UINT32 nwords = nbytes / WORDSIZE;

    while (nwords-- > 0)
        *wordp++ = 0;
}

/*******************************************************************
 * Copy an array.
 *******************************************************************/

void memcopy (void *from, void *to, UINT32 nbytes)
{
    UINT32 *src = (UINT32*) from;
    UINT32 *dst = (UINT32*) to;
    UINT32 nwords = nbytes / WORDSIZE;

    while (nwords-- > 0)
        *dst++ = *src++;
}

/*******************************************************************
 * This function is called when the device becomes initialized,
 * which occurs after the host sends a SET_CONFIGURATION (wValue not = 0)
 * request.  This callback function should initialize the endpoints
 * for the device's usage according to the current configuration.
 *******************************************************************/

void USBCBInitEP(void)
{
    //enable the HID endpoint
    USBEnableEndpoint(HID_EP, USB_IN_ENABLED | USB_OUT_ENABLED | USB_HANDSHAKE_ENABLED | USB_DISALLOW_SETUP);
    //Arm the OUT endpoint for the first packet
    USBOutHandle = HIDRxPacket(HID_EP, (BYTE*) & PacketFromPCBuffer, 64);
}

/*******************************************************************
 * This function is called from the USB stack to notify a
 * user application that a USB event occured.  This callback
 * is in interrupt context when the USB_INTERRUPT option is selected.
 *******************************************************************/

BOOL USER_USB_CALLBACK_EVENT_HANDLER(USB_EVENT event, void *pdata, WORD size)
{
    switch (event)
    {
        case EVENT_CONFIGURED:
            USBCBInitEP();
            break;
        case EVENT_SET_DESCRIPTOR:
            //USBCBStdSetDscHandler();
            break;
        case EVENT_EP0_REQUEST:
            USBCheckHIDRequest();
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
    return TRUE;
}
