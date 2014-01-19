#!/usr/bin/env python
#-*- coding: iso-8859-15 -*-

"""---------------------------------------------------------------------
     _____  _____ __   _  ______ _     _ _____ __   _  _____ 
    |_____]   |   | \  | |  ____ |     |   |   | \  | |     |
    |       __|__ |  \_| |_____| |_____| __|__ |  \_| |_____|

    Pinguino Stand-alone Programmer

    Author:          Regis Blanchot <rblanchot@gmail.com> 
    Last release:    2013-11-21
    
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

#-----------------------------------------------------------------------
#    Usage: programmer8.py path/filename.hex target
#    ex : programmer8.py blink.hex 18f47j53
#-----------------------------------------------------------------------

import sys
import os
import usb

# Globales
#-----------------------------------------------------------------------

memstart                        =    0x0C00    # bootloader offset
memend                          =    0x0000    # get its value from
                                               # getDeviceFlash(device_id)
# 8-bit Pinguino's ID
#-----------------------------------------------------------------------

VENDOR_ID                       =    0x04D8    # Microchip License
PRODUCT_ID                      =    0xFEAA    # Pinguino Sub-License

# Hex format record types
#-----------------------------------------------------------------------

Data_Record                     =     00
End_Of_File_Record              =     01
Extended_Segment_Address_Record =     02
Start_Segment_Address_Record    =     03
Extended_Linear_Address_Record  =     04
Start_Linear_Address_Record     =     05

# usbBuf Data Packet Structure
#-----------------------------------------------------------------------
#    __________________
#    |    COMMAND     |   0       [CMD]
#    |      LEN       |   1       [LEN]
#    |     ADDRL      |   2       [ADDRL]
#    |     ADDRH      |   3       [ADDRH]
#    |     ADDRU      |   4       [ADDRU]
#    |                |   5       [DATA]
#    .                .
#    .      DATA      .
#    .                .
#    |                |   62
#    |________________|   63
#
#-----------------------------------------------------------------------

BOOT_CMD                        =    0
BOOT_CMD_LEN                    =    1
BOOT_ADDR_LO                    =    2
BOOT_ADDR_HI                    =    3
BOOT_ADDR_UP                    =    4
BOOT_DATA_START                 =    5

BOOT_DEV1                       =    5
BOOT_DEV2                       =    6

BOOT_VER_MINOR                  =    2
BOOT_VER_MAJOR                  =    3

BOOT_SIZE                       =    1

# Bootloader commands
#-----------------------------------------------------------------------

READ_VERSION_CMD                =    0x00
READ_FLASH_CMD                  =    0x01
WRITE_FLASH_CMD                 =    0x02
ERASE_FLASH_CMD                 =    0x03
#READ_EEDATA_CMD                =    0x04
#WRITE_EEDATA_CMD               =    0x05
#READ_CONFIG_CMD                =    0x06
#WRITE_CONFIG_CMD               =    0x07
RESET_CMD                       =    0xFF

# Block's size to write
#-----------------------------------------------------------------------

DATABLOCKSIZE                   =    32

# USB Max. Packet size
#-----------------------------------------------------------------------

MAXPACKETSIZE                   =    64

# Bulk endpoints
#-----------------------------------------------------------------------

IN_EP                           =    0x81    # or 0x82
OUT_EP                          =    0x01    # endpoint for Bulk writes

# Configuration
#-----------------------------------------------------------------------

ACTIVE_CONFIG                   =    0x01   # or 0x03
INTERFACE_ID                    =    0x00
TIMEOUT                         =    10000

# Error codes returned by various functions
#-----------------------------------------------------------------------

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
        0x4740: ['18f13k50'     , 0x02000, 0x80 ],
        0x4700: ['18lf13k50'    , 0x02000, 0x80 ],

        0x4760: ['18f14k50'     , 0x04000, 0xFF ],
        0x4720: ['18f14k50'     , 0x04000, 0xFF ],

        0x2420: ['18f2450'      , 0x04000, 0x00 ],
        0x1260: ['18f2455'      , 0x06000, 0xFF ],
        0x2A60: ['18f2458'      , 0x06000, 0xFF ],

        0x4C00: ['18f24J50'     , 0x04000, 0x00 ],
        0x4CC0: ['18lf24J50'    , 0x04000, 0x00 ],
        
        0x1240: ['18f2550'      , 0x08000, 0xFF ],
        0x2A40: ['18f2553'      , 0x08000, 0xFF ],
        0x4C20: ['18f25J50'     , 0x08000, 0x00 ],
        0x4CE0: ['18lf25J50'    , 0x08000, 0x00 ],
        0x5C20: ['18f25k50'     , 0x08000, 0xFF ],
        0x5CA0: ['18lf25k50'    , 0x08000, 0xFF ],

        0x4C40: ['18f26J50'     , 0x10000, 0x00 ],
        0x4D00: ['18lf26J50'    , 0x10000, 0x00 ],
        
        0x5860: ['18f27j53'     , 0x20000, 0x00 ],

        0x1200: ['18f4450'      , 0x04000, 0x00 ],
        0x1220: ['18f4455'      , 0x06000, 0x00 ],
        0x2A20: ['18f4458'      , 0x06000, 0xFF ],
        0x4C60: ['18f44J50'     , 0x04000, 0x00 ],
        0x4D20: ['18lf44J50'    , 0x04000, 0x00 ],
        
        0x1200: ['18f4550'      , 0x08000, 0xFF ],
        0x2A00: ['18f4553'      , 0x08000, 0xFF ],
        
        0x4C80: ['18f45J50'     , 0x08000, 0x00 ],
        0x4D40: ['18lf45J50'    , 0x08000, 0x00 ],
        
        0x4CA0: ['18f46J50'     , 0x10000, 0x00 ],
        0x4D60: ['18f46J50'     , 0x10000, 0x00 ],

        0x58E0: ['18f47j53'     , 0x20000, 0x00 ],
        
        0x4100: ['18f65J50'     , 0x08000, 0x00 ],
        0x1560: ['18f66J50'     , 0x10000, 0x00 ],
        0x4160: ['18f66J55'     , 0x18000, 0x00 ],
        0x4180: ['18f67J50'     , 0x20000, 0x00 ],

        0x41A0: ['18f85J50'     , 0x08000, 0x00 ],
        0x41E0: ['18f86J50'     , 0x10000, 0x00 ],
        0x1F40: ['18f86J55'     , 0x18000, 0x00 ],
        0x4220: ['18f87J50'     , 0x20000, 0x00 ]
    }

# ----------------------------------------------------------------------
def getDevice(vendor, product):
# ----------------------------------------------------------------------
    """ search USB device and returns a DeviceHandle object """
    busses = usb.busses()
    for bus in busses:
        for device in bus.devices:
            if device.idVendor == vendor and device.idProduct == product:
                return device
    return ERR_DEVICE_NOT_FOUND

# ----------------------------------------------------------------------
def initDevice(device, config):
# ----------------------------------------------------------------------
    """ init pinguino device """
    handle = device.open()
    if handle:
        handle.setConfiguration(config)
        handle.claimInterface(INTERFACE_ID)
        return handle
    return ERR_USB_INIT1

# ----------------------------------------------------------------------
def closeDevice(handle):
# ----------------------------------------------------------------------
    """ Close currently-open USB device """
    handle.releaseInterface()

# ----------------------------------------------------------------------
def sendCMD(handle, usbBuf):  
# ----------------------------------------------------------------------
    """ send command to the bootloader """
    sent_bytes = handle.bulkWrite(OUT_EP, usbBuf, TIMEOUT)
    if sent_bytes == len(usbBuf):
        return handle.bulkRead(IN_EP, MAXPACKETSIZE, TIMEOUT)
        #return ERR_NONE
    else:        
        return ERR_USB_WRITE


# receiving a message from Pinguino
def received():
    while True :
      message = ""
      received = None
      while received == None :
        try :
          received = read(dh, 64)
        except usb.USBError as err:
          pass
      for i in received:
        message += chr(i)
      print message

    """
    tempo=1
    while True: 
      cadena = ""
      lu= None
      time.sleep(tempo)
    # receiving data one character at a time from Pinguino
      while lu==None :
        try :
          lu=dh.bulkRead (ENDPOINT_IN, 1, TIMEOUT)
        except usb.USBError as err:
          pass
      for i in lu:
        cadena += chr(i)
      print cadena

def read(dh, length, timeout = 0):
  return dh.bulkRead(ENDPOINT_IN, length, timeout)

def write(dh, buffer, timeout = 0):
  return 
    """

# ----------------------------------------------------------------------
def main(filename, target):
# ----------------------------------------------------------------------

    # check file to upload
    # ------------------------------------------------------------------

    if filename == '':
        print "No program to write"
        sys.exit(0)

    file = open(filename, 'r')

    if file == "":
        print "Unable to open %s" % filename
        sys.exit(0)
    file.close()

    # check processor
    # ------------------------------------------------------------------

    r = False
    for device_id  in devices_table:
        p = devices_table[device_id][0]
        if (p == target):
            r = True
            break
    if (r == False):
        print "target chip not supported"
        sys.exit(0)

    # waiting for a Pinguino board to be connected
    # ------------------------------------------------------------------

    while True:
        device = getDevice(VENDOR_ID, PRODUCT_ID)
        if device == ERR_DEVICE_NOT_FOUND:
            print "Pinguino not connected"
        else:
            print "Pinguino found !"
            break
        
    # check bootloader version
    # ------------------------------------------------------------------

    try:
        # v2.x
        handle = initDevice(device, 0x03)
        ACTIVE_CONFIG = 0x03
        IN_EP = 0x82
    except usb.USBError:
        # v4.x
        handle = initDevice(device, 0x01)
        ACTIVE_CONFIG = 0x01
        IN_EP = 0x81

    if handle == ERR_USB_INIT1:
        print "Unknown USB error."
        sys.exit(0)

    # send target type to the programmer
    # ------------------------------------------------------------------

    usbBuf = [0] * MAXPACKETSIZE
    
    if "j" in target:
        usbBuf[BOOT_CMD] = 'j'
    else:
        usbBuf[BOOT_CMD] = 'n'

    response = ""
    while ( response != (79, 75) ):
        response = sendCMD(handle, usbBuf)
        
    print "Target identified"

# ----------------------------------------------------------------------

if __name__ == "__main__":
    i = -1
    for arg in sys.argv:
        i = i + 1
    if i == 2:
        main(sys.argv[1], sys.argv[2])
    else:
        print "Usage: programmer8.py path/filename.hex target"
        print "ex : programmer8.py blink.hex 18f47j53"
