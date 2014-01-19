/*-------------------------------------------------------------------------
  config.c - PIC configuration words

             (c) 2006 Pierre Gaufillet <pierre.gaufillet@magic.fr> 

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
-------------------------------------------------------------------------*/

/* $Id: config.c,v 1.1 2006/04/17 20:36:15 gaufille Exp $ */

#include <pic18fregs.h>
#include "flash.h"

/* Set the PIC config words */
code char __at 0x300000 conf1 = 0x24;
code char __at 0x300001 conf2 = 0x0e;
code char __at 0x300002 conf3 = 0x3f;
code char __at 0x300003 conf4 = 0x1e;
code char __at 0x300005 conf5 = 0x81;
code char __at 0x300006 conf6 = 0x81;
code char __at 0x300008 conf7 = 0x0f;
code char __at 0x300009 conf8 = 0xc0;
code char __at 0x30000A conf9 = 0x0f;
code char __at 0x30000B conf10 = 0xa0;
code char __at 0x30000C conf11 = 0x0f;
code char __at 0x30000D conf12 = 0x40;

/* Memory sections for flash operations */
const uchar section_descriptor [22] = {3,
                                     FLASH_SECTION_READ,
                                     0x00, 0x00, 0x00,
                                     0xff, 0x1f, 0x00,
                                     FLASH_SECTION_READ | FLASH_SECTION_WRITE,
                                     0x00, 0x20, 0x00,
                                     0xff, 0x7f, 0x00,
                                     FLASH_SECTION_READ,
                                     0x00, 0x00, 0x30,
                                     0x0d, 0x00, 0x30};

