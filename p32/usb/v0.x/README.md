Note :
===========
This code compiles but it (v0.x) doesn't work at that time :-(
It is published only with the hope that it will be useful for the
Pinguino community.

License :
===========

This directory contains various C files to build the 32-bit Pinguino USB
HID Bootloader. These files are based on Microchip USB HID Bootloader
published under the following Software License Agreement : 

Copyright © 2007-2008 Microchip Technology Inc.  All rights reserved.           
                                                                                
Microchip licenses to you the right to use, modify, copy and distribute Software
only when embedded on a Microchip microcontroller or digital signal controller  
that is integrated into your product or third party product (pursuant to the    
sublicense terms in the accompanying license agreement).                        
                                                                                
You should refer to the license agreement accompanying this Software for        
additional information regarding your rights and obligations.                   
                                                                                
SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,   
EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF        
MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.  
IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER       
CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR    
OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES         
INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR     
CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF        
SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES          
(INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.     
                                                                                
Usage :
===========

It is possible to automatically build the bootloader for a variety of
Microchip 32-bit processors. To build it for a particular processor run
for e.g. :

	make --makefile=Makefile PROC=32MX250F128B

Use DEBUG to disassemble the code. Results are in obj/*.S
for e.g. :

	make --makefile=Makefile PROC=32MX250F128B DEBUG=yes

If your processor is not yet supported, just add a linker file in the
lkr directory.

Contact :
===========

Please contact rblanchot@pinguino.cc if you add support to any processor,
make any improvment or fix any bug.
