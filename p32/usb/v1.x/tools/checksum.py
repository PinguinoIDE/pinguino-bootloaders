#!/usr/bin/env python
#  -*- coding: UTF-8 -*-

"""-----------------------------------------------------------------------------
	checksum
	author : 2015 - regis blanchot <rblanchot@gmail.com> 
	descr. : calculate checksum from one intel hex line
	usage  : ./checksum.py 020000041D03

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

if (len(sys.argv) > 1):

    line = sys.argv[1]
    byte_count = int(line[1:3], 16)

    # checksum calculation
    # ----------------------------------------------------------

    end = 9 + byte_count * 2 # position of checksum at end of line

    checksum = 0
    i = 1
    while i < end:
        checksum = checksum + (0x100 - int(line[i:i+2], 16) ) & 0xff # eq. to not(i)
        i = i + 2

    print "Checksum is", hex(checksum)

else:

    print "No file to proceed"
    print "usage ex: ./checksum.py :04000005BFC00000"
