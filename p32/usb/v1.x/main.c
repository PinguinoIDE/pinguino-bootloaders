/***********************************************************************
    Title:  USB Pinguino Bootloader
    File:   main.c
    Descr.: USB HID bootloader for PIC32 processors
    Author: Régis Blanchot <rblanchot@gmail.com>

    This file is part of Pinguino (http://www.pinguino.cc)
    Released under the LGPL license (http://www.gnu.org/licenses/lgpl.html)
 **********************************************************************/

#include "p32xxxx.h"                    // Registers definitions
//#include "cp0defs.h"                    // CP0 registers definitions
//#include "regdef.h"                     // MIPS registers names
//#include "debug.h"                      // Debug functions
#if !defined(__P32GCC__)                // MPLABX/XC32
#include "mplabx.h"                     // Config. bits
#endif
#include "typedefs.h"                   // UINT8, UINT32, ...
#include "mem.h"                        // Pinguino memory regions description
#include "hardware.h"                   // Pinguino boards hardware description
#include "flash.h"                      // Flash write and flash erase functions
#include "core.h"                       // MemCopy, MemClear, core timer functions
#include "delay.h"                      // Delayus
#include "usb.h"                        // USB device framework definitions

#if (_DEBUG_ENABLE_)                    // defined in makefile
#include "serial.h"                     // UART functions
#endif

/***********************************************************************
 * CONSTANTS
 **********************************************************************/

//Switch State Variable Choices
#define	QUERY_DEVICE            0x02    //Command that the host uses to learn about the device (what regions can be programmed, and what type of memory is the region)
#define	UNLOCK_CONFIG           0x03    //Note, this command is used for both locking and unlocking the config bits (see the "//Unlock Configs Command Definitions" below)
#define ERASE_DEVICE            0x04    //Host sends this command to start an erase operation.  Firmware controls which pages should be erased.
//Sub-command for the ERASE_DEVICE command
//#define UNLOCKCONFIG          0x00    //Unlock Configs Command Definitions
//#define LOCKCONFIG            0x01    //lock Configs Command Definitions
#define PROGRAM_DEVICE          0x05    //If host is going to send a full DataBlockSize8 to be programmed, it uses this command.
#define	PROGRAM_COMPLETE        0x06    //If host send less than a DataBlockSize8 to be programmed, or if it wished to program whatever was left in the buffer, it uses this command.
#define GET_DATA                0x07    //The host sends this command in order to read out memory from the device.  Used during verify (and read/export hex operations)
#define	RESET_DEVICE            0x08    //Resets the microcontroller, so it can update the config bits (if they were programmed, and so as to leave the bootloader (and potentially go back into the main application)

//Query Device Response "Types" 
#define	TYPEPROGRAMMEMORY       0x01    //When the host sends a QUERY_DEVICE command, need to respond by populating a list of valid memory regions that exist in the device (and should be programmed)
#define TYPEEEPROM              0x02
#define TYPECONFIGWORDS         0x03
#define	TYPEENDOFTYPELIST       0xFF    //Sort of serves as a "null terminator" like number, which denotes the end of the memory region list has been reached.

//BootState Variable States
#define	IDLESTATE               0x00
#define NOTIDLESTATE            0x01

//OtherConstants
#define INVALIDADDRESS          0xFFFFFFFF

//Application and Microcontroller constants
#define DEVICE_FAMILY           0x03    //0x01 for PIC18, 0x02 for PIC24, 0x03 for PIC32

#define	TOTALPACKETSIZE8        HID_INT_EP_SIZE
#define DATABLOCKSIZE8          56      //Number of bytes in the "Data" field of a standard request to/from the PC.  Must be an even number from 2 to 56.
#define BUFFERSIZE32            (DATABLOCKSIZE8/WORDSIZE)

/***********************************************************************
 * TYPE DEFINITIONS
 **********************************************************************/

typedef union __attribute__((packed)) _USB_HID_BOOTLOADER_COMMAND
{
    UINT8 Contents[TOTALPACKETSIZE8];

    //For GET_DATA / PROGRAM_DEVICE command
    struct __attribute__((packed))
    {
        UINT8  Command;
        UINT32 Address;
        UINT8  Size;
        UINT8  PadBytes[TOTALPACKETSIZE8 - 6 - DATABLOCKSIZE8];
        UINT32 Data32[BUFFERSIZE32];
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
        UINT32 major;
        UINT32 minor;
        UINT32 devpt;
        UINT8  Type4; //End of sections list indicator goes here, fill with 0xFF.
        UINT8  ExtraPadBytes[33];
    };
} USBPacket;

/***********************************************************************
 * VARIABLES
 **********************************************************************/

//64 bytes buffer for receiving packets on EP1 OUT from the PC
static USBPacket PacketFromPC;
//64 bytes buffer for sending packets on EP1 IN to the PC
static USBPacket PacketToPC;
static USBPacket PacketFromPCBuffer;

// USB_HANDLE is a pointer (void *) to an entry in the BDT.
static USB_HANDLE USBOutHandle = 0;
static USB_HANDLE USBInHandle = 0;
static UINT8  BootState;
static UINT32 DataBuffer32[BUFFERSIZE32];
static UINT8  DataIndex32;
static UINT32 Address32;

USB_DEVICE_STATE USBDeviceState;

/*******************************************************************************
 * FUNCTION PROTOTYPES
 ******************************************************************************/

//void USBEventHandler(USB_EVENT);
       void USBEventHandler(void);
static void USBPacketHandler(void);
static void WriteFlashBlock(void);

/***********************************************************************
 * Entry point of the entire application
 **********************************************************************/
/*
asm("           .section .reset,\"ax\",@progbits");
asm("           .set noreorder");
asm("           .ent _reset");
asm("_reset:    la      k0, _startup");
asm("           jr      k0");
asm("           nop");
asm("           .end _reset");
asm("           .globl _reset");
*/
/***********************************************************************
 * STARTUP SEQUENCE
 **********************************************************************/
/*
asm("           .section .startup,\"ax\",@progbits");
asm("           .set noreorder");
asm("           .set nomips16");
asm("           .ent _startup");
//asm("           .type _start, function");
asm("_startup:  la      $sp, _stack");
asm("           la      $ra, main");
asm("           la      $gp, _gp");
asm("           jr      $ra");
//asm ("          .text");
asm("           .end _startup");
asm("           .globl _startup");
*/
/***********************************************************************
 * MAIN PROGRAM ENTRY POINT
 **********************************************************************/

int main(void)
{
    static UINT32 led_count = 0;

    //User App. hard-coded virtual memory address
    //UINT32 *UserAppPtr = (UINT32*)APP_RESET_ADDR;
    void  (*UserAppFnc)(void) = (void (*)(void))APP_RESET_ADDR;

    //UserAppPtr = (UINT32*)APP_RESET_ADDR;

    // Setup wait states
    #if !defined(__PIC32MX2__)
    CHECON = 2; // 010 = Two Wait states
    BMXCONCLR = (1<<6); // Bit 6 : 0 = Data RAM accesses from CPU have zero wait states for address setup
    CHECONSET = (1<<5)|(1<<4); // 11 = Enable predictive prefetch for both cacheable and non-cacheable regions
    #endif
    
    /*
    // Initialize STATUS register: master interrupt disable
    _mtc0(_CP0_STATUS, _CP0_STATUS_SELECT, _CP0_STATUS_CU0_MASK | _CP0_STATUS_BEV_MASK);

    // Clear CAUSE register: use special interrupt vector 0x200
    _mtc0(_CP0_CAUSE, _CP0_CAUSE_SELECT, _CP0_CAUSE_IV_MASK);

    // Config register: enable kseg0 caching
    _mtc0(_CP0_CONFIG, _CP0_CONFIG_SELECT, _CP0_GET_CONFIG() | 3);

    // Initialize all .bss variables by zeros
    extern unsigned char __bss_start, __bss_end;
    MemClear(&__bss_start, &__bss_end - &__bss_start);
    */
    
    //Initialize LEDs and push button
    mLED_Init();
    mLED_1_On();
    mLED_2_Off();
    mSWITCH_Init();

    #if (_DEBUG_ENABLE_)
        SerialInit(9600);
        SerialPrint("\r\n\f");// CLS
        SerialPrint("www.PINGUINO.cc \r\n");
        SerialPrint("BOOTLOADER v");
        SerialPrintNumber(USB_MAJOR_VER, 10);
        SerialPrint(".");
        SerialPrintNumber(USB_MINOR_VER, 10);
        SerialPrint(".");
        SerialPrintNumber(USB_DEVPT_VER, 10);
        SerialPrint("b\r\n");
        SerialPrint("rblanchot@gmail.com\r\n");
        #if 0
        SerialPrint("FCPU ");
        SerialPrintNumber(FCPU/1000000, 10);
        SerialPrint("MHz\r\n");
        SerialPrint("FPB  ");
        SerialPrintNumber(FPB/1000000, 10);
        SerialPrint("MHz\r\n");
        SerialPrintNumber((BMXBOOTSZ>>10), 10);
        SerialPrint("KB BOOT SYSTEM\r\n");
        SerialPrintNumber(DATA_MEM_LENGTH/1024, 10);
        SerialPrint("/");
        SerialPrintNumber((BMXDRMSZ>>10), 10);
        SerialPrint("KB RAM SYSTEM\r\n");
        SerialPrintNumber(APP_PROGRAM_LENGTH/1024, 10);
        SerialPrint("/");
        SerialPrintNumber((BMXPFMSZ>>10), 10);
        SerialPrint("KB FLASH FREE\r\n");
        #elif 0
        SerialPrint("APP_EBASE_ADDR = 0x");
        SerialPrintNumber(APP_EBASE_ADDR, 16);
        SerialPrint("\r\n");
        SerialPrint("APP_RESET_ADDR = 0x");
        SerialPrintNumber(APP_RESET_ADDR, 16);
        SerialPrint("\r\n");
        SerialPrint("APP_PROGRAM_ADDR_START = 0x");
        SerialPrintNumber(APP_PROGRAM_ADDR_START, 16);
        SerialPrint("\r\n");
        SerialPrint("APP_PROGRAM_ADDR_END = 0x");
        SerialPrintNumber(APP_PROGRAM_ADDR_END, 16);
        SerialPrint("\r\n");
        #endif
        SerialPrint("READY\r\n");
    #endif

    // Call the user application if the switch button is not pressed.
    // If there's no application, we just go on.

    //if (!mSWITCH_Pressed() && *UserAppPtr != INVALIDADDRESS)
    if (mSWITCH_NotPressed())
    {
            #if (_DEBUG_ENABLE_)
                SerialPrint("Switch not pressed\r\n");
                SerialPrint("Address at 0x");
                SerialPrintNumber(APP_RESET_ADDR, 16);
                SerialPrint(" is 0x");
                SerialPrintNumber(*(UINT32*)APP_RESET_ADDR, 16);
                SerialPrint("\r\n");
            #endif
        //if (*UserAppPtr != INVALIDADDRESS)
        if (*(UINT32*)APP_RESET_ADDR != INVALIDADDRESS)
        {
            #if (_DEBUG_ENABLE_)
                SerialPrint("Try to start the User's App ...\r\n");
            #endif
            UserAppFnc();
        }
    }

    #if (_DEBUG_ENABLE_)
        SerialPrint("No User App. detected\r\n");
        SerialPrint("Starting the bootloader ...\r\n");
    #endif

    // Initializes the variable holding the handle for the last transmission
    USBOutHandle = 0;
    USBInHandle = 0;

    BootState = IDLESTATE;
    Address32 = INVALIDADDRESS;
    DataIndex32 = 0;

    // Initializes USB module SFRs and firmware
    USBDeviceInit();
    //USBCheckCable(); // Now called from USBDeviceTasks

    while(1)
    {
        // Blink the led
        if (led_count == 0)
        {
            led_count = 10000;
            mLED_1_Toggle();
            mLED_2_Toggle();
        }
        led_count--;
    
        // Check bus status and service USB interrupts.
        USBDeviceTasks();

        //Handle packets only if device is configured and not suspended
        if (!U1PWRCbits.USUSPEND && USBDeviceState == CONFIGURED_STATE)
            USBPacketHandler();
    }
}

/***********************************************************************
 * Bootloader service commands routine.
 **********************************************************************/

static void USBPacketHandler(void)
{
    UINT32 i;
    UINT8 nwords32;
    UINT8 index32;

    #if (_DEBUG_ENABLE_)
    //SerialPrint("> USBPacketHandler\r\n");
    #endif

    // *** RB20141210 : OK ***
    if (BootState == IDLESTATE)
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
                MemCopy(&PacketFromPCBuffer, &PacketFromPC, TOTALPACKETSIZE8);

                // Restart receiver, to be ready for a next packet.
                USBOutHandle = USBTransferOnePacket(OUT_FROM_HOST, (UINT8*)&PacketFromPCBuffer);
                BootState = NOTIDLESTATE;

                //Initialize the next packet sent to the host
                MemClear(&PacketToPC, TOTALPACKETSIZE8);
            }
        }
    }

    else //(BootState Not in Idle State)
    {
        #if 0//(_DEBUG_ENABLE_)
        SerialPrint("> Received command ");
        SerialPrintNumber(PacketFromPC.Command, 10);
        SerialPrint("\r\n");
        #endif

        switch (PacketFromPC.Command)
        {

//**********************************************************************
            case QUERY_DEVICE:
//**********************************************************************

                #if 0//(_DEBUG_ENABLE_)
                SerialPrint("> Sending Device data\r\n");
                #endif

                // Prepare a response packet
                PacketToPC.Command1     = (UINT8)  QUERY_DEVICE;
                PacketToPC.DataSize     = (UINT8)  DATABLOCKSIZE8;
                PacketToPC.DeviceFamily = (UINT8)  DEVICE_FAMILY;
                PacketToPC.Type1        = (UINT8)  TYPEPROGRAMMEMORY;
                PacketToPC.Address1     = (UINT32) APP_PROGRAM_ADDR_START;
                PacketToPC.Length1      = (UINT32) APP_PROGRAM_LENGTH;
                PacketToPC.Type2        = (UINT8)  TYPEPROGRAMMEMORY;
                PacketToPC.Address2     = (UINT32) FCPU;
                PacketToPC.Length2      = (UINT32) FPB;
                PacketToPC.Type3        = (UINT8)  TYPEPROGRAMMEMORY;
                PacketToPC.major        = (UINT32) USB_MAJOR_VER;
                PacketToPC.minor        = (UINT32) USB_MINOR_VER;
                PacketToPC.devpt        = (UINT32) USB_DEVPT_VER;
                PacketToPC.Type4        = (UINT8)  TYPEENDOFTYPELIST;

                // Send the packet to the host
                if (!USBHandleBusy(USBInHandle))
                {
                    //#define USBTxOnePacket(ep,data,len)     USBTransferOnePacket(ep,IN_TO_HOST,data,len)
                    //USBInHandle = USBTxOnePacket(HID_EP, (UINT8*)&PacketToPC, TotalPacketSize8);
                    //USBInHandle = USBTransferOnePacket(HID_EP, IN_TO_HOST, (UINT8*)&PacketToPC, TotalPacketSize8);
                    USBInHandle = USBTransferOnePacket(IN_TO_HOST, (UINT8*)&PacketToPC);
                    BootState = IDLESTATE;
                }
                break;

//**********************************************************************
            case GET_DATA:
//**********************************************************************

                // Prepare a response packet
                PacketToPC.Command = GET_DATA;
                PacketToPC.Address = PacketFromPC.Address;
                PacketToPC.Size = PacketFromPC.Size;

                //nwords32 = PacketFromPC.Size / WORDSIZE;

                #if 0//(_DEBUG_ENABLE_)
                SerialPrint("> Reading 0x");
                SerialPrintNumber(ConvertFlashToVirtualAddress(PacketFromPC.Address), 16);
                SerialPrint("\r\n");
                #endif
                
                // void memcopy (void *from, void *to, UINT32 nbytes)
                // Copy memory from PacketFromPC.Address to PacketToPC.Data32
                MemCopy( (void*) ConvertFlashToVirtualAddress(PacketFromPC.Address),
                         (void*) PacketToPC.Data32,
                         PacketFromPC.Size );
                
                #if 0//(_DEBUG_ENABLE_)
                SerialPrint("Sending Device's ID 0x");
                SerialPrintNumber(PacketToPC.Data32[0], 16);
                SerialPrint("\r\n");
                #endif

                if (!USBHandleBusy(USBInHandle))
                {
                    USBInHandle = USBTransferOnePacket(IN_TO_HOST, (UINT8*)&PacketToPC);
                    BootState = IDLESTATE;
                }
                break;

//**********************************************************************
            case ERASE_DEVICE:
//**********************************************************************
                
                FlashClearError();
                // erase memory from ebase address to be able to write the
                // user application Interrupt Vector Table
                for (i = APP_EBASE_ADDR;            //APP_PROGRAM_ADDR_START;
                     i < APP_PROGRAM_ADDR_END;
                     i = i + FLASH_PAGE_SIZE)
                {
                    #if 0//(_DEBUG_ENABLE_)
                    SerialPrint("Erasing block from 0x");
                    SerialPrintNumber(i, 16);
                    SerialPrint(" to 0x");
                    SerialPrintNumber(i + FLASH_PAGE_SIZE, 16);
                    SerialPrint("\r\n");
                    #endif

                    FlashErasePage((void*)i);
                    //Call USBDeviceTasks() periodically to prevent falling off
                    //the bus if any SETUP packets should happen to arrive.
                    //USBDeviceTasks();
                    //IFS1CLR = _IFS1_USBIF_MASK;
                }

                BootState = IDLESTATE;
                break;

//**********************************************************************
            case PROGRAM_DEVICE:
//**********************************************************************

                // number of 32-bit words to write
                nwords32 = PacketFromPC.Size / WORDSIZE;

                if (Address32 == INVALIDADDRESS)
                    Address32 = PacketFromPC.Address;

                if (Address32 == PacketFromPC.Address)
                {
                    for (i = 0; i < nwords32; i++)
                    {
                        //Data field is right justified.
                        //Need to put it in the buffer left justified.

                        // BufferSize32=14, nwords32=14
                        // index32 from 0 to 13
                        index32 = BUFFERSIZE32 - nwords32 + i;

                        // DataIndex32 from 0 to 13
                        DataBuffer32[DataIndex32] = PacketFromPC.Data32[index32];
                        DataIndex32 += 1;// if 13 then 14
                        Address32 += WORDSIZE;
                        //Call WriteFlashBlock() when buffer is full
                        if (DataIndex32 == BUFFERSIZE32)
                            WriteFlashBlock();
                    }
                }
                //else host sent us a non-contiguous packet address...  to make
                //this firmware simpler, host should not do this without sending
                //a PROGRAM_COMPLETE command in between program sections.
                BootState = IDLESTATE;
                break;

//**********************************************************************
            case PROGRAM_COMPLETE:
//**********************************************************************

                WriteFlashBlock();
                //Reinitialize pointer to an invalid range, so we know the next
                //PROGRAM_DEVICE will be the start address of a contiguous section.
                Address32 = INVALIDADDRESS;
                BootState = IDLESTATE;
                break;

//**********************************************************************
            case RESET_DEVICE:
//**********************************************************************

                // Disable the USB module and wait for the USB cable
                // capacitance to discharge down to disconnected (SE0) state.
                // Otherwise host might not realize we disconnected/reconnected
                // when we do the reset.
                U1CON = 0x00;
                Delayus(1000);
                SoftReset();
                break;

        }//End switch

    }//End if/else

}//End USBPacketHandler()

/***********************************************************************
 * Notify uploader32.py that a USB event occured.
 **********************************************************************/

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

/***********************************************************************
 * Write blocks of 32-bit words
 * DataIndex32 : number of words to write
 * from (Address32 - 56) to Address32
 **********************************************************************/

static void WriteFlashBlock()
{
    UINT32 i = 0;

    #if 0//(_DEBUG_ENABLE_)
    //SerialPrint("0x");
    SerialPrintNumber(Address32 - DataIndex32 * WORDSIZE, 16);
    #endif

    while (DataIndex32)
    {
        FlashWriteWord((void*) Address32 - DataIndex32 * WORDSIZE, DataBuffer32[i]);
                       
        #if 0//(_DEBUG_ENABLE_)
        SerialPrint("[");
        SerialPrintNumber((UINT32)DataBuffer32[i], 16);
        SerialPrint("]");
        SerialPrint("=[");
        SerialPrintNumber(*(UINT32*)((void*) Address32 - DataIndex32 * WORDSIZE), 16);
        SerialPrint("] ");
        #endif
        
        DataIndex32 -= 1;
        i += 1;
    }

    #if 0//(_DEBUG_ENABLE_)
    SerialPrint("\r\n");
    #endif
    //Nop(); // Why ? Not necessary for PIC32MX2 family
}
