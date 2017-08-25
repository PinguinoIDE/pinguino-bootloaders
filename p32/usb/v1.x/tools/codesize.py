#!/usr/bin/env python
#  -*- coding: UTF-8 -*-

"""-----------------------------------------------------------------------------
	codesize
	calculate code size from .hex file
	usage: ./codesize.py filename (without .hex extension)
	2011 - regis blanchot <rblanchot@gmail.com> 

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU Lesser General Public
	License as published by the Free Software Foundation; either
	version 2.1 of the License, or (at your option) any later version.

	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public
	License along with this library; if not, write to the Free Software
	Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
	--------------------------------------------------------------------------"""

import sys

# Hex format record types
Data_Record = 00
End_Of_File_Record = 01
Extended_Segment_Address_Record = 02
Start_Segment_Address_Record = 03
Extended_Linear_Address_Record = 04
Start_Linear_Address_Record = 05
    
codesize = 0
address_Hi = 0
max_address = 0
old_address = 0

BOOTSTART = 0x1D000000
BOOTEND   = 0x1D003000
BOOTLENGTH   = 0

if (len(sys.argv) > 1):

    filename = sys.argv[1]
    
    if (len(sys.argv) > 2):
        option = sys.argv[2]
    else:
        option = ""
        
    fichier = open(filename, 'r')
    lines = fichier.readlines()

    for line in lines:

        byte_count = int(line[1:3], 16)
        address_Lo = int(line[3:7], 16)
        record_type= int(line[7:9], 16)

        # extended linear address record
        if record_type == Extended_Linear_Address_Record:
            address_Hi = int(line[9:13], 16)

        # data
        elif record_type == Data_Record:

            # 32-bit address
            address = (address_Hi << 16) + address_Lo
            #print "address : 0x%X" % address

            # code starts at 0x1D000000 and can't be upon 0x1D003000
            if (address > BOOTSTART) and (address < BOOTEND):
                
                # code size
                codesize = codesize + byte_count

                # address calculation
                if (address > old_address):
                    max_address = address + byte_count
                    old_address = address

            # display
            #print "%s -> 0x%08X" % (line,address),
            #for i in range(byte_count):
            #    print "%02X" % int(line[9 + (2 * i) : 11 + (2 * i)], 16),
            #print

        # bootloader jump address
        elif record_type == Start_Linear_Address_Record:
            #print "Reset vector at 0x%X" % address
            break
            
        # end of file record
        elif record_type == End_Of_File_Record:
            break
            
        # unsupported record type
        #Extended_Segment_Address_Record = 02
        #Start_Segment_Address_Record = 03
        else:

            print "Caution : unsupported record type in hex file"
            print "Line %s" % line

    fichier.close()

    print "Code size is : %d bytes (0x%X)" % (codesize, codesize)

    # 4K align
    if (option <> ""):
        BOOTEND = 4096 * (int(max_address/4096) + 1)
        BOOTSTART = BOOTSTART | 0x80000000
        BOOTEND   = BOOTEND   | 0x80000000
        BOOTLENGTH = BOOTEND - BOOTSTART
        IVTLENGTH = 0xA00
        IVTSTART = BOOTEND
        STARTUPSTART = BOOTEND + IVTLENGTH
        STARTUPLENGTH = 0x200
        RESETLENGTH = 0x10
        RESETSTART = BOOTEND + 0x1000
        APPSTART = RESETSTART + RESETLENGTH
        
        print "BEST CONFIG POSSIBLE :"
        print
        print "In the bootloader linker script :"
        print "    _ebase_address = 0x%X" % BOOTEND
        print "    kseg0_program_mem    (rx)  : ORIGIN = 0x%X, LENGTH = 0x%X" % (BOOTSTART, BOOTLENGTH)
        print "    exception_mem              : ORIGIN = 0x%X, LENGTH = 0x%X" % (IVTSTART, IVTLENGTH)
        print "In the application linker script :"
        print "    _ebase_address = 0x%X" % IVTSTART
        print "    _RESET_ADDR = 0x%X" % RESETSTART
        print "    exception_mem              : ORIGIN = 0x%X, LENGTH = 0x%X" % (BOOTEND, IVTLENGTH)
        print "    kseg0_boot_mem             : ORIGIN = 0x%X  LENGTH = 0x%X" % (STARTUPSTART, STARTUPLENGTH)
        print "    kseg1_boot_mem             : ORIGIN = 0x%X  LENGTH = 0x%X" % (RESETSTART, RESETLENGTH)
        print "    kseg0_program_mem    (rx)  : ORIGIN = 0x%X, LENGTH = ??? " % (APPSTART)

else:
    print "No file to proceed"
    print "usage: ./codesize.py filename (without .hex extension)"
