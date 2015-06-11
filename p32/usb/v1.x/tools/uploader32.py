#!/usr/bin/env python
#-*- coding: iso-8859-15 -*-

"""---------------------------------------------------------------------
     _____  _____ __   _  ______ _     _ _____ __   _  _____ 
    |_____]   |   | \  | |  ____ |     |   |   | \  | |     |
    |       __|__ |  \_| |_____| |_____| __|__ |  \_| |_____|

    Pinguino Stand-alone Uploader for 32-bit Pinguino

    Author:          Regis Blanchot <rblanchot@gmail.com> 
    Last release:    2014-09-09
    
    This library is free software you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the
    Free Software Foundation, Inc.
    51 Franklin Street, Fifth Floor
    Boston, MA  02110-1301  USA
---------------------------------------------------------------------"""

# Based on UBW32 / Microchip mphidflash software licensed (GNU GPL v3) by Phillip Burgess <pburgess@dslextreme.com>
# UBW32 sources at : http://vak-opensource.googlecode.com/svn/!svn/bc/117/trunk/utilities/ubw32/
# MPHIDFLASH sources at : http://mphidflash.googlecode.com/svn-history/r2/trunk/
# PyUSB Doc : http://wiki.erazor-zone.de/wiki:projects:python:pyusb:pydoc
# Device Descriptors : lsusb -v -d 04d8:003C

import sys
import os
import usb
import time

# Globales
#-----------------------------------------------------------------------

#memstart                        =    0    # bootloader offset
#memend                          =    0    # get its value later

# Hex format record types
# ----------------------------------------------------------------------

Data_Record                     =     00
End_Of_File_Record              =     01
Extended_Segment_Address_Record =     02
Start_Segment_Address_Record    =     03
Extended_Linear_Address_Record  =     04
Start_Linear_Address_Record     =     05

# 32-bit Pinguino's ID
#-----------------------------------------------------------------------

VENDOR_ID                       =    0x04D8 # Microchip License
PRODUCT_ID                      =    0x003C # Pinguino Sub-License

# Data block description
# ----------------------------------------------------------------------
BYTESPERADDRESS                 =    1
MAXPACKETSIZE                   =    64
DATABLOCKSIZE                   =    56        # MAXPACKETSIZE - Block Command Size

# Response packet structure
# ----------------------------------------------------------------------
"""
struct __attribute__ ((packed))
{
2    unsigned char Command;
56    unsigned char PacketDataFieldSize;
3    unsigned char DeviceFamily;
1    unsigned char Type1;
0 48 0 189    unsigned long Address1; BD003000
0 208 1 96   unsigned long Length1;   6001D000
255    unsigned char Type2;
    unsigned long Address2;
    unsigned long Length2;
    unsigned char Type3;        //End of sections list indicator goes here, when not programming the vectors, in that case fill with 0xFF.
    unsigned long Address3;
    unsigned long Length3;            
    unsigned char Type4;        //End of sections list indicator goes here, fill with 0xFF.
    unsigned char ExtraPadBytes[33];
};
"""

BOOT_CMD                        =    0      # char = 1 byte
BOOT_PACKET_SIZE                =    1      # char = 1 byte
BOOT_DEVICE_FAMILY              =    2      # char = 1 byte
BOOT_TYPE1                      =    3      # char = 1 byte
BOOT_ADDR1                      =    4      # long = 4 bytes
BOOT_LEN1                       =    8      # long = 4 bytes
BOOT_TYPE2                      =    12     # char = 1 byte
BOOT_ADDR2                      =    13     # long = 4 bytes
BOOT_LEN2                       =    17     # long = 4 bytes
BOOT_TYPE3                      =    21     # char = 1 byte
BOOT_ADDR3                      =    22     # long = 4 bytes
BOOT_LEN3                       =    26     # long = 4 bytes

BOOT_TYPE_LEN                   =    9      # char + long + long = 9 bytes

BOOT_DEVID1                     =    6
BOOT_DEVID2                     =    7
BOOT_DEVID3                     =    8
BOOT_DEVID4                     =    9

# Sent packet structure
# ----------------------------------------------------------------------

"""
struct __attribute__ ((packed))
{
    unsigned char Command;
    unsigned long Address;
    unsigned char Size;
    unsigned char PadBytes[(TotalPacketSize - 6) - (RequestDataBlockSize)];    
    unsigned int Data[RequestDataBlockSize/WORDSIZE];
};
"""

BOOT_ADDR                       =    1
BOOT_ADDR_LSBLO                 =    1
BOOT_ADDR_LSBHI                 =    2
BOOT_ADDR_MSBLO                 =    3
BOOT_ADDR_MSBHI                 =    4
BOOT_CMD_SIZE                   =    5
BOOT_CMD_PAD                    =    6
BOOT_CMD_DATA                   =    7
    
# Command Definitions
# ----------------------------------------------------------------------

UNLOCKCONFIG_CMD                =    0x00    # sub-command for the ERASE_DEVICE_CMD
LOCKCONFIG_CMD                  =    0x01    # sub-command for the ERASE_DEVICE_CMD
QUERY_DEVICE_CMD                =    0x02    # what regions can be programmed, and what type of memory is the region
UNLOCK_CONFIG_CMD               =    0x03    # for both locking and unlocking the config bits
ERASE_DEVICE_CMD                =    0x04    # to start an erase operation, firmware controls which pages should be erased
PROGRAM_DEVICE_CMD              =    0x05    # to send a full RequestDataBlockSize to be programmed
PROGRAM_COMPLETE_CMD            =    0x06    # if host send less than a RequestDataBlockSize to be programmed, or if it wished to program whatever was left in the buffer, it uses this command
GET_DATA_CMD                    =    0x07    # the host sends this command in order to read out memory from the device. Used during verify (and read/export hex operations)
RESET_DEVICE_CMD                =    0x08    # resets the microcontroller, so it can update the config bits (if they were programmed, and so as to leave the bootloader (and potentially go back into the main application)

# Query Device Response
# ----------------------------------------------------------------------

TypeProgramMemory               =    0x01    # when the host sends a QUERY_DEVICE command, need to respond by populating a list of valid memory regions that exist in the device (and should be programmed)
TypeEEPROM                      =    0x02
TypeConfigWords                 =    0x03
TypeEndOfTypeList               =    0xFF    # sort of serves as a "null terminator" like number, which denotes the end of the memory region list has been reached.

# Device family
# ----------------------------------------------------------------------

DEVICE_FAMILY_PIC18             =    0x01
DEVICE_FAMILY_PIC24             =    0x02
DEVICE_FAMILY_PIC32             =    0x03

# HID endpoints
# ----------------------------------------------------------------------

IN_EP                           =    0x81    # endpoint for Hid reads
OUT_EP                          =    0x01    # endpoint for Hid writes

# Configuration
# ----------------------------------------------------------------------

INTERFACE_ID                    =    0x00
ACTIVE_CONFIG                   =    1
TIMEOUT                         =    1000

# Error codes returned by various functions
# ----------------------------------------------------------------------

ERR_NONE                        =    0
ERR_CMD_ARG                     =    1
ERR_CMD_UNKNOWN                 =    2
ERR_DEVICE_NOT_FOUND            =    3
ERR_USB_INIT1                   =    4
ERR_USB_INIT2                   =    5
ERR_USB_OPEN                    =    6
ERR_USB_WRITE                   =    7
ERR_USB_READ                    =    8
ERR_HEX_OPEN                    =    9
ERR_HEX_STAT                    =    10
ERR_HEX_MMAP                    =    11
ERR_HEX_SYNTAX                  =    12
ERR_HEX_CHECKSUM                =    13
ERR_HEX_RECORD                  =    14
ERR_VERIFY                      =    15
ERR_EOL                         =    16
ERR_USB_ERASE                   =    17

# Table with supported USB devices
# device_id:[PIC name, flash size(in bytes), eeprom size (in bytes)] 
#-----------------------------------------------------------------------

devices_table = \
{  
    0x04A00053: ['32MX220F032B'    , 0x9D008000, 0x9D003180 ], #32K
    0x04A04053: ['32MX220F032D'    , 0x9D008000, 0x9D003000 ], #32K
    0x04D00053: ['32MX250F128B'    , 0x9D020000, 0x9D003180 ], #128K
    0x06600053: ['32MX270F256B'    , 0x9D040000, 0x9D003180 ]  #256K
}

# ----------------------------------------------------------------------
def getDevice(vendor, product):
# ----------------------------------------------------------------------
    """ Search USB device and returns a DeviceHandle object """
    busses = usb.busses()
    for bus in busses:
        for device in bus.devices:
            if device.idVendor == vendor and device.idProduct == product:
                return device
    return ERR_DEVICE_NOT_FOUND

# ----------------------------------------------------------------------
def initDevice(device):
# ----------------------------------------------------------------------
    """ Init pinguino device """

    handle = device.open()

    if handle:
        try:
            handle.detachKernelDriver(INTERFACE_ID)

        except usb.USBError as msg:
            #print msg.message
            pass

        handle.setConfiguration(ACTIVE_CONFIG)
        handle.claimInterface(INTERFACE_ID)

        return handle
    return ERR_USB_INIT1

# ----------------------------------------------------------------------
def closeDevice(handle):
# ----------------------------------------------------------------------
    """ Close currently-open USB device """
    handle.releaseInterface()

# ----------------------------------------------------------------------
def resetDevice(handle):
# ----------------------------------------------------------------------
    """ reset device """
    usbBuf = [RESET_DEVICE_CMD] * MAXPACKETSIZE
    #usbBuf[BOOT_CMD] = RESET_DEVICE_CMD
    return sendCMD(handle, usbBuf)

# ----------------------------------------------------------------------
def getDeviceFamily(handle):
# ----------------------------------------------------------------------
    """ get device info """
    usbBuf = [QUERY_DEVICE_CMD] * MAXPACKETSIZE
    #usbBuf[BOOT_CMD] = QUERY_DEVICE_CMD
    usbBuf = getResponse(handle, usbBuf)
    return usbBuf[BOOT_DEVICE_FAMILY]
        
# ----------------------------------------------------------------------
def getDeviceID(handle):
# ----------------------------------------------------------------------
    """ get 4-byte device ID from location 0xBF80F220 / 0x9F80F220 """

    usbBuf = readFlash(handle, 0xBF80F220, 4)

    devid = (usbBuf[60]      ) | \
            (usbBuf[61] <<  8) | \
            (usbBuf[62] << 16) | \
            (usbBuf[63] << 24)

    #print hex(devid)
    # mask device id to get revision number
    device_rev = ( ( devid >> 24 ) & 0xF0 ) >> 4
    # mask revision number to get device id
    device_id  = devid & 0x0FFFFFFF

    return device_id, device_rev

# ----------------------------------------------------------------------
def getDeviceFlash(handle):
# ----------------------------------------------------------------------
    """ get size of program memory area """
    usbBuf = [QUERY_DEVICE_CMD] * MAXPACKETSIZE
    #usbBuf[BOOT_CMD] = QUERY_DEVICE_CMD
    usbBuf = getResponse(handle, usbBuf)

    """
    j=3
    while usbBuf[j] != TypeEndOfTypeList:
        if usbBuf[j] == TypeProgramMemory:
            bf = (usbBuf[j + 5]      ) | \
                 (usbBuf[j + 6] <<  8) | \
                 (usbBuf[j + 7] << 16)
        j = j + BOOT_TYPE_LEN
    """
    # user program address
    ps = (usbBuf[BOOT_ADDR1 + 0]      ) | \
         (usbBuf[BOOT_ADDR1 + 1] <<  8) | \
         (usbBuf[BOOT_ADDR1 + 2] << 16) | \
         (usbBuf[BOOT_ADDR1 + 3] << 24)

    # free memory
    bf = (usbBuf[BOOT_LEN1 + 0]      ) | \
         (usbBuf[BOOT_LEN1 + 1] <<  8) | \
         (usbBuf[BOOT_LEN1 + 2] << 16)
         
    #      | \
    #     (usbBuf[BOOT_LEN1 + 3] << 24)

    return ps, bf
    
# ----------------------------------------------------------------------
def getDeviceName(device_id):
# ----------------------------------------------------------------------
    for n in devices_table:
        if n == device_id:
            return devices_table[n][0]
    return ERR_DEVICE_NOT_FOUND

# ----------------------------------------------------------------------
def sendCMD(handle, usbBuf):
# ----------------------------------------------------------------------
    """ Send a command to the bootloader """
    sent_bytes = handle.interruptWrite(OUT_EP, usbBuf, TIMEOUT)
    if sent_bytes == len(usbBuf):
        return ERR_NONE
    else:
        return ERR_USB_WRITE

# ----------------------------------------------------------------------
def getResponse(handle, usbBuf):
# ----------------------------------------------------------------------
    """ Send a command and get a response from the bootloader """
    sent_bytes = handle.interruptWrite(OUT_EP, usbBuf, TIMEOUT)
    if sent_bytes == len(usbBuf):
        try:
            usbBuf = handle.interruptRead(IN_EP, MAXPACKETSIZE, TIMEOUT)
        except:
            print "Error: no response from the bootloader"
            closeDevice(handle)
            sys.exit(0)
        #print usbBuf
        return usbBuf
    return ERR_USB_WRITE

# ----------------------------------------------------------------------
def eraseFlash(handle):
# ----------------------------------------------------------------------
    """ erase the whole flash memory """
    usbBuf = [ERASE_DEVICE_CMD] * MAXPACKETSIZE
    #usbBuf[BOOT_CMD] = ERASE_DEVICE_CMD
    return sendCMD(handle, usbBuf)
    
# ----------------------------------------------------------------------
def writeFlash(handle, address, block):
# ----------------------------------------------------------------------
    """ write a block of code """
    length = len(block)

    if length == 0:
        # Short data packets need flushing
        usbBuf = [PROGRAM_COMPLETE_CMD] * MAXPACKETSIZE
        #usbBuf[BOOT_CMD] = PROGRAM_COMPLETE_CMD
        return sendCMD(handle, usbBuf)
        
    # command code
    usbBuf = [PROGRAM_DEVICE_CMD] * MAXPACKETSIZE
    #usbBuf[BOOT_CMD] = PROGRAM_DEVICE_CMD
    # block's address (0x12345678 => "12345678")
    address = "%08X" % (address / BYTESPERADDRESS)
    usbBuf[BOOT_ADDR + 0] = int(address[6:8], 16)
    usbBuf[BOOT_ADDR + 1] = int(address[4:6], 16)
    usbBuf[BOOT_ADDR + 2] = int(address[2:4], 16)
    usbBuf[BOOT_ADDR + 3] = int(address[0:2], 16)
    # data's length
    usbBuf[BOOT_CMD_SIZE] = length
    # add data 'right justified' within packet
    for i in range(length):
        usbBuf[MAXPACKETSIZE - length + i] = block[i]
    # write data packet on usb device
    status = sendCMD(handle, usbBuf)

    if status == ERR_NONE and length < DATABLOCKSIZE:
        # Short data packets need flushing
        #usbBuf = [PROGRAM_COMPLETE_CMD] * MAXPACKETSIZE
        usbBuf[BOOT_CMD] = PROGRAM_COMPLETE_CMD
        status = sendCMD(handle, usbBuf)

    return status

# ----------------------------------------------------------------------
def readFlash(handle, address, length):
# ----------------------------------------------------------------------
    """ read a block of flash """
    # command code
    usbBuf = [GET_DATA_CMD] * MAXPACKETSIZE
    #usbBuf[BOOT_CMD] = GET_DATA_CMD
    # address
    usbBuf[BOOT_ADDR + 0] = (address      ) & 0xFF
    usbBuf[BOOT_ADDR + 1] = (address >> 8 ) & 0xFF
    usbBuf[BOOT_ADDR + 2] = (address >> 16) & 0xFF
    usbBuf[BOOT_ADDR + 3] = (address >> 24) & 0xFF
    # size of block to read
    usbBuf[BOOT_CMD_SIZE] = length
    # send request to the bootloader
    return getResponse(handle, usbBuf)

# ----------------------------------------------------------------------
def hexWrite(handle, filename, memstart, memend):
# ----------------------------------------------------------------------
    """ Parse the Hex File Format and send data to usb device """

    """
    [0]        Start code, one character, an ASCII colon ':'.
    [1:3]    Byte count, two hex digits, a number of bytes (hex digit pairs) in the data field. 16 (0x10) or 32 (0x20) bytes of data are the usual compromise values between line length and address overhead.
    [3:7]    Address, four hex digits, a 16-bit address of the beginning of the memory position for the data. Limited to 64 kilobytes, the limit is worked around by specifying higher bits via additional record types. This address is big endian.
    [7:9]    Record type, two hex digits, 00 to 05, defining the type of the data field.
    [9:*]    Data, a sequence of n bytes of the data themselves, represented by 2n hex digits.
    [*:*]    Checksum, two hex digits - the least significant byte of the two's complement of the sum of the values of all fields except fields 1 and 6 (Start code ":" byte and two hex digits of the Checksum). It is calculated by adding together the hex-encoded bytes (hex digit pairs), then leaving only the least significant byte of the result, and making a 2's complement (either by subtracting the byte from 0x100, or inverting it by XOR-ing with 0xFF and adding 0x01). If you are not working with 8-bit variables, you must suppress the overflow by AND-ing the result with 0xFF. The overflow may occur since both 0x100-0 and (0x00 XOR 0xFF)+1 equal 0x100. If the checksum is correctly calculated, adding all the bytes (the Byte count, both bytes in Address, the Record type, each Data byte and the Checksum) together will always result in a value wherein the least significant byte is zero (0x00).
            For example, on :0300300002337A1E
            03 + 00 + 30 + 00 + 02 + 33 + 7A = E2, 2's complement is 1E
    """

    data        = []
    old_address = 0
    max_address = 0
    address_Hi  = 0
    codesize    = 0

    bufLen      = 0
    addrSave    = 0

    # image of the whole PIC memory (above memstart)
    # --------------------------------------------------------------

    #print "board.memstart = %d" % board.memstart)
    #print "board.memend = %d" % board.memend)
    for i in range(memend - memstart):
        data.append(0xFF)

    # load hex file
    # ----------------------------------------------------------------------

    hexfile = open(filename, 'r')
    lines = hexfile.readlines()
    hexfile.close()

    # read each line in file
    # --------------------------------------------------------------

    for line in lines:

        byte_count = int(line[1:3], 16)
        # lower 16 bits (bits 0-15) of the data address
        address_Lo = int(line[3:7], 16)
        record_type= int(line[7:9], 16)

        # checksum calculation
        # ----------------------------------------------------------
        end = 9 + byte_count * 2 # position of checksum at end of line
        checksum = int(line[end:end+2], 16)

        cs = 0
        i = 1
        while i < end:
            cs = cs + (0x100 - int(line[i:i+2], 16) ) & 0xff # eq. to not(i)
            i = i + 2

        if checksum != cs:
            return ERR_HEX_CHECKSUM

        # extended linear address record
        # ----------------------------------------------------------
        if record_type == Extended_Linear_Address_Record:

            # upper 16 bits (bits 16-31) of the data address
            address_Hi = int(line[9:13], 16) << 16

        # data record
        # ----------------------------------------------------------
        elif record_type == Data_Record:

            address = address_Hi + address_Lo
            print "0x%08X to be written" % address

            # max address
            if (address > old_address) and (address < memend):
                max_address = address + byte_count
                old_address = address
                #print "max_address = %d" % max_address

            # code size
            if (address >= memstart) and (address < memend):
                codesize = codesize + byte_count
                #print "codesize = %d" % codesize

            # data append
            for i in range(byte_count):
                #print "index=%d" % (address - memstart + i)
                #Caution : addresses are not always contiguous
                #data.append(int(line[9 + (2 * i) : 11 + (2 * i)], 16))
                data[address - memstart + i] = int(line[9 + (2 * i) : 11 + (2 * i)], 16)

        # end of file record
        # ----------------------------------------------------------
        elif record_type == End_Of_File_Record:

            break

        # unsupported record type
        # ----------------------------------------------------------
        else:

            print "ERR_HEX_RECORD"

    # max_address must be divisible by DATABLOCKSIZE
    # --------------------------------------------------------------

    #max_address = max_address + MAXPACKETSIZE - (max_address % MAXPACKETSIZE)
    max_address = max_address + 64 - (max_address % 64)
    #print "max_address = 0x%08X" % max_address

    # write blocks of DATABLOCKSIZE bytes
    # --------------------------------------------------------------

    for addr in range(memstart, max_address, DATABLOCKSIZE):
        index = addr - memstart
        #print "index = %d" % index)
        #print "data = %s" % data[index:index+DATABLOCKSIZE])
        writeFlash(handle, addr, data[index:index+DATABLOCKSIZE])

    usbBuf = [PROGRAM_COMPLETE_CMD] * MAXPACKETSIZE
    #usbBuf[BOOT_CMD] = PROGRAM_COMPLETE_CMD
    status = sendCMD(handle, usbBuf)

    print "%d bytes written" % codesize

    #return status
    return ERR_NONE

# ----------------------------------------------------------------------
def main(filename):
# ----------------------------------------------------------------------

    # check file to upload
    # ------------------------------------------------------------------

    if filename == '':
        print "No program to write"
        sys.exit(0)

    hexfile = open(filename, 'r')
    if hexfile == "":
        print "Unable to open %s" % filename
        sys.exit(0)
    hexfile.close()

    # search for a Pinguino board
    # --------------------------------------------------------------

    device = getDevice(VENDOR_ID, PRODUCT_ID)
    if device == ERR_DEVICE_NOT_FOUND:
        print "Pinguino not found"
        print "Is your device connected and/or in bootloader mode ?"
        sys.exit(0)
    else:
        print "Pinguino found ..."

    handle = initDevice(device)

    if handle == ERR_USB_INIT1:
        print "... but upload is not possible."
        print "Press the Reset button and try again."
        sys.exit(0)

    elif handle == None:
        sys.exit(0)

    #print "%s - %s" % (handle.getString(device.iProduct, 30), handle.getString(device.iManufacturer, 30))
    #print "%s" % handle.getString(device.iProduct, 30)

    # find out the processor
    # --------------------------------------------------------------

    if getDeviceFamily(handle) != DEVICE_FAMILY_PIC32:
        print "Error: not a PIC32 family device"
        closeDevice(handle)
        sys.exit(0)

    device_id, device_rev = getDeviceID(handle)
    proc = getDeviceName(device_id)
    print " - with PIC%s (id=0x%08X, rev.%01X)" % (proc, device_id, device_rev)

    """
    if proc != self.board.proc:
        self.add_report("Error: Program compiled for %s but device has %s" % (self.board.proc, proc))
        self.closeDevice()
        return
    """

    # find out flash memory size
    # ------------------------------------------------------------------
    
    memstart, memfree  = getDeviceFlash(handle)
    memend   = memstart + memfree
    memstart = memstart + 0x80000000
    memend   = memend   + 0x80000000
    print " - with %d bytes free (%d KB)" % (memfree, memfree/1024)
    print " - from 0x%08X to 0x%08X" % (memstart, memend)

    # start erasing
    # --------------------------------------------------------------

    #print "Erasing flash memory ..."
    status = eraseFlash(handle)
    if status != ERR_NONE:
        print "Erase Error!"
        closeDevice(handle)
        sys.exit(0)

    # start writing
    # --------------------------------------------------------------

    print "Uploading user program ..."
    status = hexWrite(handle, filename, memstart, memend)
    if status != ERR_NONE:
        print "Write Error!"
        closeDevice(handle)
        sys.exit(0)

    print "%s successfully uploaded" % os.path.basename(filename)

    # reset and start start user's app.
    # --------------------------------------------------------------

    #print "Resetting ..."
    print "Starting user program ..."
    status = resetDevice(handle)
    if status != ERR_NONE:
        print "Reset Error!"
        closeDevice(handle)
        sys.exit(0)

    print "Ready."
    sys.exit(0)

# ----------------------------------------------------------------------

if __name__ == "__main__":
    i = -1
    for arg in sys.argv:
        i = i + 1
    if i == 1:
        main(sys.argv[1])
    else:
        print "Usage: uploader32.py path/filename.hex"

# ----------------------------------------------------------------------
