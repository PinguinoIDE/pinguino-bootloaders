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

codesize = 0
address_Hi = 0
max_address = 0
old_address = 0

if (len(sys.argv) > 1):

    filename = sys.argv[1]
    fichier = open(filename + ".hex", 'r')
    lines = fichier.readlines()

    for line in lines:

        byte_count = int(line[1:3], 16)
        address_Lo = int(line[3:7], 16)
        record_type= int(line[7:9], 16)

        # extended linear address record
        if record_type == 4:
            address_Hi = int(line[9:13], 16) << 16

        # data
        if record_type == 0:

            # 32-bit address
            address = address_Hi + address_Lo

            # code size
            codesize = codesize + byte_count

            # address calculation
            if (address > old_address) : # and (address < 0x0C00):
                max_address = address + byte_count
                old_address = address

    fichier.close()

    page1024 = int(max_address/1024) + 1

    print "code size is : %d bytes (0x%X)" % (codesize, codesize)
    print "max. address : 0x%X" % max_address
    print "user app. address : 0x%X" % (page1024 * 1024)
    #print "still %d bytes to gain !" % ( max_address - ( ( page1024 - 1 ) * 1024 ) )
    #print
else:
    print "No file to proceed"
    print "usage: ./codesize.py filename (without .hex extension)"
