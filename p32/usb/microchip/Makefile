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

_MIPS16_ENABLE_ = false

# Where to find the gcc-mips-elf toolchain
#BINDIR		= /dvpt/gcc/mips/linux64/p32/bin
BINDIR		= /usr/bin

# ----------------------------------------------------------------------
# Version (cf. changelog.txt)
# ----------------------------------------------------------------------

MAJ_VER		= 0
MIN_VER		= 1

# ----------------------------------------------------------------------
# Project definition
# ----------------------------------------------------------------------

ifndef PROC
	PROC	= 32MX270F256B
	DEFAULT = YES
endif 

CRYSTAL		= 8
CPU			= $(PROC)
PRJ			= Bootloader_v$(MAJ_VER).$(MIN_VER)_$(CPU)_X$(CRYSTAL)MHz

# ----------------------------------------------------------------------
# Unsupported record in hex file / Jean-pierre Mandon 2012
# ----------------------------------------------------------------------
#		04		 : 4 bytes
#		0000	 : address
#		05		 : record type = Start_Linear_Address_Record
#		9D004000 : address where the user program should start
#				 : _RESET_ADDR (see lkr/procdefs.ld)
#		1A		 : checksum
# ----------------------------------------------------------------------

#ifneq (,$(filter $(CPU),32MX220F032B 32MX250F128B))
#else
#endif

FAMILY1		= $(findstring 32MX1, $(PROC))
FAMILY2		= $(findstring 32MX2, $(PROC))
FAMILY		= $(FAMILY1)$(FAMILY2)

ifneq (,$(filter $(FAMILY),32MX1 32MX2))
	BADRECORD   = ':04000005BFC0000078'
else
	BADRECORD   = '^:040000059D006000FA'
endif

# ----------------------------------------------------------------------
# Colors
# ----------------------------------------------------------------------

RED=\033[31;1m
GREEN=\033[32;1m
YELLOW=\033[33;1m
BLUE=\033[34;1m
TERM=\033[m

WARNING=$(YELLOW)
ERROR=$(RED)
NORMAL=$(GREEN)
END=$(TERM)

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

#-lm -lgcc -lc
LIBS		= -lm -lgcc -lc

LIBDIRS		=

# ----------------------------------------------------------------------
# Files
# ----------------------------------------------------------------------

SRCS		= $(wildcard *.c)
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

ifeq "$(_MIPS16_ENABLE_)" "true"
	# Generate MIPS16 code.
	MIPS16_OPT=-mips16
	# makes non-MIPS16 and MIPS16 code link-compatible.
	MIPS16_INTER=-minterlink-mips16
endif

CFLAGS		= $(INCLUDEDIRS)

LDFLAGS		= $(LIBDIRS) $(LIBS)
LDFLAGS	   += -G0
#--verbose

# -Os option leads to errors when non-zero initialized static variable are defined
# -msoft-float

ELF_FLAGS	= -EL -march=24kc \
			  -Os $(MIPS16_INTER) \
			  -ffunction-sections \
			  -fdata-sections \
			  -Wl,--gc-sections \
			  -Wl,-L$(OBJDIR) \
			  -Wl,-Map=output.map \
			  -D __PIC32MX__ \
			  -D __$(CPU)__ \
			  -T$(LKRDIR)/$(CPU).ld \
			  -T$(LKRDIR)/elf32pic32mx.x

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

#all: start clean copy $(PRJ).elf convert correct size
all: start clean copy $(PRJ).elf convert size
#all: start clean copy $(PRJ).elf convert correct
#all: start clean copy $(PRJ).elf convert
#all: start clean copy $(PRJ).elf
#all: start

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
	@$(RM) $(HEXDIR)/*.hex			> /dev/null

copy:
	@echo "Adding $(CPU) SFR definition"
	@$(CP) $(OBJDIR)/$(CPU).o $(OBJDIR)/processor.o

obj/%.o: %.c
	@echo "Compiling $<"
	@$(CC) $(ELF_FLAGS) $(LDFLAGS) $(CFLAGS) $(MIPS16_OPT) -c $< -o $@
ifdef DEBUG
	@echo "Disassembling all files"
	@$(OBJDUMP) -d -M intel -S $@ > obj/$*.S
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

#@grep --binary --invert-match $(BADRECORD) $(HEXDIR)/$(PRJ).hex > $(HEXDIR)/temp.hex
#@sed 's/$(BADRECORD)/$(NEWRECORD)/g' $(HEXDIR)/$(PRJ).hex > $(HEXDIR)/temp.hex
#@$(MV) $(HEXDIR)/temp.hex $(HEXDIR)/$(PRJ).hex  
#sed '/$(BADRECORD)/d' $(HEXDIR)/$(PRJ).hex
correct:
	@echo "Fixing bad record"
	@grep --binary --invert-match $(BADRECORD) $(HEXDIR)/$(PRJ).hex > $(HEXDIR)/temp.hex
	@rm -f $(HEXDIR)/$(PRJ).hex
	@mv $(HEXDIR)/temp.hex $(HEXDIR)/$(PRJ).hex

size:
ifdef DEBUG
	@tools/codesize.py $(HEXDIR)/$(PRJ) DEBUG=yes
	@$(SIZE) $(HEXDIR)/$(PRJ).hex
else
	@tools/codesize.py $(HEXDIR)/$(PRJ)
endif

burn:
	@tools/pic32prog -S -p $(HEXDIR)/$(PRJ).hex
