########################################################################
#                                                                      #
#	Pinguino Bootloader v0.x                                           #
#	32-bit USB HID Bootloader                                          #
#	Author : Régis Blanchot <rblanchot@gmail.com>                      #
#                                                                      #
#	Usage ex. : make --makefile=Makefile PROC=32MX250F128B [DEBUG=yes] #
#                                                                      #
#	Supported CPU : 32MX250F128B                                       #
#	                32MX270F256B                                       #
#                                                                      #
#	This file is part of Pinguino Project (http://www.pinguino.cc)     #
#	Released under the LGPL license (www.gnu.org/licenses/lgpl.html)   #
#                                                                      #
########################################################################

_MIPS16_ENABLE_	= true
_OVCLCK_ENABLE_	= false
_TEST_ENABLE_	= false

# Where to find the gcc-mips-elf toolchain
BINDIR		= /dvpt/pinguino/git-copy/pinguino-compilers/linux64/p32/bin
#BINDIR		= /usr/bin

# ----------------------------------------------------------------------
# Version (will override boot.h #define)
# ----------------------------------------------------------------------

MAJ_VER		= 1
MIN_VER		= 1
#DOT_VER		= 3

# ----------------------------------------------------------------------
# Processor family
# ----------------------------------------------------------------------

ifndef PROC
	PROC	= 32MX270F256B
	DEFAULT = YES
endif 

FAMILYMZ	= $(findstring 32MZ,  $(PROC))
FAMILYMX1	= $(findstring 32MX1, $(PROC))
FAMILYMX2	= $(findstring 32MX2, $(PROC))
FAMILY		= $(FAMILYMX1)$(FAMILYMX2)$(FAMILYMZ)

CRYSTAL		= 8
TOOL		= PK3

# ----------------------------------------------------------------------
# Overclock (possible value : 42, 48, 60, 64)
# ----------------------------------------------------------------------

ifeq "$(_OVCLCK_ENABLE_)" "true"
	FCPU=64
else
	FCPU=40
endif

# ----------------------------------------------------------------------
# Project's name
# ----------------------------------------------------------------------

ifeq "$(_TEST_ENABLE_)" "true"
	PRJ		= Test
else
#	PRJ		= Bootloader_v$(MAJ_VER).$(MIN_VER)_$(PROC)_$(FCPU)MHz
	PRJ		= Bootloader_v$(MAJ_VER).$(MIN_VER)_$(PROC)_X$(CRYSTAL)MHz
endif

# ----------------------------------------------------------------------
# Unsupported record in hex file / Jean-pierre Mandon 2012
# ----------------------------------------------------------------------
#	04		 : 4 bytes
#	0000	 : address
#	05		 : record type = Start_Linear_Address_Record
#	9D004000 : address where the user program should start
#			 : _RESET_ADDR (see lkr/procdefs.ld)
#	1A		 : checksum
# ----------------------------------------------------------------------

ifneq (,$(filter $(FAMILY),32MX1 32MX2))
	BADRECORD   = '^:04000005BFC0000078'
else
	BADRECORD   = '^:040000059D006000FA'
endif

# ----------------------------------------------------------------------
# MIPS Core
# ----------------------------------------------------------------------
# PIC32MX has a MIPS M4K core
# PIC32MZ has a MIPS microAptiv core

ifeq "$(FAMILY)" "32MZ"
	CORE		= m14k
	#CORE		= m14kc
else
	#CORE		= 24kc
	CORE		= m4k
endif

# ----------------------------------------------------------------------
# Colors
# ----------------------------------------------------------------------

RED=\033[31;1m
GREEN=\033[32;1m
YELLOW=\033[33;1m
BLUE=\033[34;1m
BOLD=\033[0;1;31m
NORM=\033[0m

WARNING=$(YELLOW)
ERROR=$(RED)
NORMAL=$(GREEN)
END=$(NORM)

# ----------------------------------------------------------------------
# Directories
# ----------------------------------------------------------------------

ORIGIN		= $(shell pwd)
SRCDIR		= $(ORIGIN)
HEXDIR		= $(ORIGIN)/hex
INCDIR		= $(ORIGIN)
LKRDIR		= $(ORIGIN)/lkr
OBJDIR		= $(ORIGIN)/obj/non-free

INCLUDEDIRS	= -I$(LKRDIR) \
			  -I$(OBJDIR) \
			  -I$(INCDIR) \
			  -I$(INCDIR)/non-free

# -lm -lgcc 
LIBS		= -lc
LIBDIRS		=

# ----------------------------------------------------------------------
# Files
# ----------------------------------------------------------------------

ifeq "$(_TEST_ENABLE_)" "true"
	SRCS	= test.c delay.c core.c serial.c
else
	ALLSRCS	= $(wildcard *.c)
	SRCS	= $(filter-out test.c, $(ALLSRCS))
endif

OBJS		= $(SRCS:%.c=obj/%.o)

# ----------------------------------------------------------------------
# Commands
# ----------------------------------------------------------------------

CC			= $(BINDIR)/p32-gcc
OBJCOPY		= $(BINDIR)/p32-objcopy
SIZE		= $(BINDIR)/p32-size
OBJDUMP		= $(BINDIR)/p32-objdump
RM			= rm -f -v
CP			= cp
MV			= mv

# ----------------------------------------------------------------------
# Flags
# ----------------------------------------------------------------------
# https://sourceware.org/binutils/docs/ld/Options.html#Options

ifeq "$(_MIPS16_ENABLE_)" "true"
	# Generate MIPS16 code.
	MIPS16_OPT=-mips16
	# makes non-MIPS16 and MIPS16 code link-compatible.
	MIPS16_INTER=-minterlink-mips16
endif

CFLAGS		= -fdollars-in-identifiers $(INCLUDEDIRS)

LDFLAGS		= $(LIBDIRS) $(LIBS)
#LDFLAGS	   += -G0
#--verbose

#-nostdlib		Only search library directories explicitly specified on the command line
#-static		Do not link against shared libraries
#--strip-all	Omit all symbol information from the output file
#--gc-sections	Enable garbage collection of unused input sections
#-Map=mapfile	Print a link map to the file mapfile
    
ELF_FLAGS	= -march=$(CORE) \
			  -EL -nostdlib -static \
			  -Os $(MIPS16_INTER) \
			  -ffunction-sections \
			  -fdata-sections \
			  -Wl,--strip-all \
			  -Wl,--gc-sections \
			  -Wl,-L$(OBJDIR) \
			  -Wl,-Map=output.map \
			  -D __P32GCC__ \
			  -D __PIC32MX__ \
			  -D __$(PROC)__ \
			  -D TEST=$(_TEST_ENABLE_) \
			  -D FCPUMHZ=$(FCPU) \
			  -D USB_MAJOR_VER=$(MAJ_VER) \
			  -D USB_MINOR_VER=$(MIN_VER) \
			  -T$(LKRDIR)/$(PROC).ld \
			  -T$(LKRDIR)/elf32pic32mx.x

#			  -D USB_DOT_VER=$(DOT_VER) \
#			  -Wl,--script=$(LKRDIR)/$(PROC).ld
#			  -T$(LKRDIR)/elf32pic32mx.x

#-----------------------------------------------------------------------
# Rules
#-----------------------------------------------------------------------
#
#	$@ 	Le nom de la cible
#	$< 	Le nom de la première dépendance
#	$^ 	La liste des dépendances
#	$? 	La liste des dépendances plus récentes que la cible
#	$* 	Le nom du fichier sans suffixe
#
#-----------------------------------------------------------------------

#all: start clean copy $(PRJ).elf convert fix size burn
all: start clean copy $(PRJ).elf convert size burn
#all: start clean copy $(PRJ).elf convert fix size
#all: start clean copy $(PRJ).elf convert size
#all: start clean copy $(PRJ).elf convert
#all: start clean copy $(PRJ).elf
#all: start
#all: burn

start:
ifeq ($(DEFAULT), YES)
	@echo "$(WARNING)No Processor has been defined.$(END)"
	@echo "$(WARNING)Usage ex. : make --makefile=Makefile PROC=32MX270F256B [DEBUG=yes]$(END)"
	@echo "$(WARNING)Using 32MX270F256B as default processor$(END)"
endif
	@echo "$(NORMAL)Compiling $(PRJ)$(END)"

clean:
	@echo "Cleaning all build files"
	@$(RM) $(OBJDIR)/processor.o	> /dev/null
	@$(RM) $(ORIGIN)/obj/*.o		> /dev/null
	@$(RM) $(ORIGIN)/obj/*.S		> /dev/null
	@$(RM) $(HEXDIR)/*.elf			> /dev/null
#	@$(RM) $(HEXDIR)/*.hex			> /dev/null

copy:
	@echo "Adding $(PROC) SFR definition"
	@$(CP) $(OBJDIR)/$(PROC).o $(OBJDIR)/processor.o

obj/%.o: %.c
	@echo "Compiling $<"
	@$(CC) $(ELF_FLAGS) $(LDFLAGS) $(CFLAGS) $(MIPS16_OPT) -c $< -o $@
ifdef DEBUG
	@echo "Disassembling all files"
	@$(OBJDUMP) -D -M intel -S $@ > obj/$*.S
	cat obj/$*.S
endif

$(PRJ).elf: $(OBJS)
	@echo "Linking all the modules together"
	@$(CC) $(ELF_FLAGS) $(LDFLAGS) $(CFLAGS) -o hex/$@ $^ \
		$(LKRDIR)/crt0.S \
		$(OBJDIR)/processor.o \
		$(INCDIR)/p32xxxx.h \
		$(LIBS)

convert:
	@echo "Converting elf to hex"
	@$(OBJCOPY) -O ihex $(HEXDIR)/$(PRJ).elf $(HEXDIR)/$(PRJ).hex

fix:
	@echo "Fixing bad records"
	@sed -i -e 's/:020000049D005D/:020000041D00DD/g' $(HEXDIR)/$(PRJ).hex
	@sed -i -e 's/:020000049FC09B/:020000041FC01B/g' $(HEXDIR)/$(PRJ).hex
	@sed -i -e 's/:02000004BFC07B/:020000041FC01B/g' $(HEXDIR)/$(PRJ).hex
#	@sed -i -e '/$(BADRECORD)/d' $(HEXDIR)/$(PRJ).hex

size:
ifdef DEBUG
	@tools/codesize.py $(HEXDIR)/$(PRJ).hex DEBUG=yes
	@$(SIZE) $(HEXDIR)/$(PRJ).hex
else
	@tools/codesize.py $(HEXDIR)/$(PRJ).hex
endif

# cf. IPECMD.txt
# COMMAND		MEANING					DEFAULT
# -TP<tool>		Tool Selection			None
# -P<proc>		Processor Selection		None 
# -F<file>		Hex File Selection		None 
# -M			Program Device			Do Not Program 
# -W			Externally power target	Power from PM3 

burn:
#	@tools/pic32prog -S -p $(HEXDIR)/$(PRJ).hex
#	C:\Program Files\Microchip\MPLABX\mplab_ide\bin
	@echo "Burning $(PRJ).hex file"
	@java -jar /opt/microchip/mplabx/mplab_ipe/ipecmd.jar \
			-W \
			-M \
			-TP$(TOOL) \
			-P$(PROC) \
			-F$(HEXDIR)/$(PRJ).hex
	@$(RM) MPLABXLog.* > /dev/null
	@$(RM) log.* > /dev/null
