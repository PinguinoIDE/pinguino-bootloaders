########################################################################
#                                                                      #
#	Pinguino Bootloader v4.x                                           #
#	Microchip 8-bit PIC USB Bootloader                                 #
#	Author:	Régis Blanchot <rblanchot@gmail.com>                       #
#           André Gentric                                              #
#                                                                      #
#	Usage : make --makefile=Makefile PROC=18f45k50                     #
#           make --makefile=Makefile PROC=18f2550 OSC=20               #
#           make --makefile=Makefile PROC=16F1459 OSC=INTOSC           #
#           make --makefile=Makefile COMP=XC8  PROC=18F47J53 OSC=8     #
#           make --makefile=Makefile COMP=SDCC PROC=18F26J50 OSC=8     #
#                                                                      #
#	This file is part of Pinguino Project (http://www.pinguino.cc)     #
#	Released under the LGPL license (www.gnu.org/licenses/lgpl.html)   #
#                                                                      #
########################################################################
#	Supported PICs                                                     #
########################################################################
#                                                                      #
#	16F1459                                                            #
#	18F13k50	18F14k50                                               #
#	18F2455		18F4455                                                #
#	18F2550		18F4550                                                #
#	18F25k50	18F45k50                                               #
#	18F26j50	18F46j50                                               #
#	18F26J53	18F46J53                                               #
#	18F27J53	18F47J53                                               #
#                                                                      #
########################################################################
#	TODO                                                   #
########################################################################
#                                                                      #
########################################################################

########################################################################
#	PROGRAMMER                                       #
########################################################################

PROGRAMMER	= PK3
#PROGRAMMER	= PK2

########################################################################
#	COMPILERS TOOLCHAIN LOCATION                                       #
########################################################################

SDCCDIR		= /opt/pinguino/p8/bin
XC8DIR		= /opt/microchip/xc8/v1.38/bin
MPLABDIR	= /opt/microchip/mplabx/v3.30
JAVADIR		= $(MPLABDIR)/sys/java/jre1.8.0_65/bin
JAVA		= $(JAVADIR)/java
IPE			= $(MPLABDIR)/mplab_ipe/ipecmd.jar

########################################################################
#	INTERNAL OPTIONAL CONFIGURATION OPTIONS                            #
########################################################################

BOOT_USE_DEBUG=0
BOOT_USE_LOWPOWER=0
BOOT_USE_HID=0
BOOT_USE_UART=0
BOOT_USE_CDC=0
BOOT_USE_BULK=1

########################################################################
#	CONFIGURATION OPTIONS                                              #
########################################################################

LOW_SPEED	= 1
HIGH_SPEED	= 0
ONE			= 1

# Compiler (XC8 by default)
ifeq ("x${COMP}", "x")
	COMPILER	= XC8
else
	COMPILER	= $(shell echo $(COMP) | tr '[:lower:]' '[:upper:]')
endif

# Microchip PIC (PIC18F47J53 by default)
ifeq ("x${PROC}", "x")
	CPU			= 18f47j53
else
	CPU			= $(shell echo $(PROC) | tr '[:upper:]' '[:lower:]')
endif

# external crystal frequency (internal osc. used by default)
ifeq ("x${OSC}", "x")
	CRYSTAL		= INTOSC
else
	CRYSTAL		= $(OSC)
endif

# usb speed (HIGH_SPEED = 0 / LOW_SPEED = 1)
ifeq ("x${SPEED}", "x")
	USBSPEED	= 0
else
	USBSPEED	= $(SPEED)
endif

# low voltage programing (YES = 1 / NO = 0)
ifeq ("x${VOLTAGE}", "x")
	LVP			= 0
else
	LVP			= $(VOLTAGE)
endif

# string descriptor flag  (YES = 1 / NO = 0)
ifeq ("x${STRINGDESC}", "x")
	STRING		= 1
else
	STRING		= $(STRINGDESC)
endif

########################################################################
#	DO NOT CHANGE FOLLOWINGS WITHOUT CARE                              #
########################################################################

# bootloader version (cf. CHANGELOG file)
MAJ_VER		= 4
MIN_VER		= 18
SUB_VER		= 5

# Microchip Vendor ID / Pinguino Product ID (Microchip sublicense)
VID			= 0x04D8
PID			= 0xFEAA

# PIC Family (18F or 16F ?)
# APPSTART address is the end of bootloader / start of user's application
# APPSTART must be a multiple of 1024 (18F only) because :
# 1/ flash must be erased before any write,
# 2/ some PICs can only erase 1024-byte long blocks 

FAM			= $(findstring 16f, $(CPU))

ifeq ($(FAM), 16f)
	ARCH	= pic14
	USBRAM	= 2080h-21FFh
	APPSTART= 0x500
	COMPILER= XC8
	OPTIMIZ	= --opt=default,+asm,+asmfile,-speed,+space,-debug
else
	ARCH	= pic16
	USBRAM	= 500h-57Fh
	APPSTART= 0x0C00
	ifeq ($(COMPILER), SDCC)
		#APPSTART	= 0x0C00
		OPTIMIZ		= --optimize-df --optimize-cmp --obanksel=9 --denable-peeps
	else
		#APPSTART	= 0x0800
		#OPTIMIZ		= --opt=default,+asm,+asmfile,-speed,+space,-debug
		OPTIMIZ		= --opt=default
	endif
endif

APPSTARTDEC	= $(shell printf "%d" $(APPSTART))
BOOTENDDEC	= $(shell echo $(APPSTARTDEC)-$(ONE)|bc)
BOOTEND		= $(shell printf "%x" $(BOOTENDDEC))

# Project name
ifeq ($(CRYSTAL), INTOSC)
	PRJ		= Pinguino_Bootloader_v$(MAJ_VER).$(MIN_VER).$(SUB_VER)_$(COMPILER)_$(CPU)_$(CRYSTAL)
else
	PRJ		= Pinguino_Bootloader_v$(MAJ_VER).$(MIN_VER).$(SUB_VER)_$(COMPILER)_$(CPU)_X$(CRYSTAL)MHz
endif

# C files
SRCS		= $(wildcard src/*.c)

# SDCC compiler
ifeq ($(COMPILER), SDCC)

	CC		= $(SDCCDIR)/sdcc
	OBJS	= $(SRCS:src/%.c=obj/%.o)

	PFLAGS	= -V -m$(ARCH) -p$(CPU) \
			  --use-non-free \
			  --fomit-frame-pointer \
			  --std-c99

# XC8 compiler
else

	CC		= $(XC8DIR)/xc8
	OBJS	= $(SRCS:src/%.c=obj/%.p1)

	PFLAGS	= --CHIP=$(CPU) \
			  -P -N64 -V -M -Q -G \
			  --warn=0 \
			  --asmlist \
			  --addrqual=reject \
			  --mode=pro \
			  --ERRATA=none \
			  --output=-mcof,+elf:multilocs \
			  --stack=compiled:auto:auto

endif

# Compiler flags
CFLAGS		= $(PFLAGS) $(OPTIMIZ) \
			  -DVENDORID=$(VID) \
			  -DPRODUCTID=$(PID) \
			  -DCHIP=$(CPU) \
			  -DAPPSTART=$(APPSTART) \
			  -DSTRING=$(STRING) \
			  -DSPEED=$(USBSPEED) \
			  -DLOW_SPEED=1 \
			  -DHIGH_SPEED=0 \
			  -DVOLTAGE=$(LVP) \
			  -DCRYSTAL=$(CRYSTAL) \
			  -DMAJOR_VERSION=$(MAJ_VER) \
			  -DMINOR_VERSION=$(MIN_VER) \
			  -DBOOT_USE_DEBUG=$(BOOT_USE_DEBUG) \
			  -DBOOT_USE_LOWPOWER=$(BOOT_USE_LOWPOWER) \
			  -DBOOT_USE_HID=$(BOOT_USE_HID) \
			  -DBOOT_USE_UART=$(BOOT_USE_UART) \
			  -DBOOT_USE_CDC=$(BOOT_USE_CDC) \
			  -DBOOT_USE_BULK=$(BOOT_USE_BULK)

# Assembler flags
# -w[0|1|2] : set message level
ifeq ($(COMPILER), SDCC)

AFLAGS		= -Wa,"-w0"

endif

# Linker flags
ifeq ($(COMPILER), SDCC)

# -m : output a map file
# -w : disable "processor mismatch" warning
# -s : we use our own linker script and startup code (work)
# -u : add macro value for script.
# --no-crt : do not link the default run-time modules
LDFLAGS		= -Wl-uAPPSTART=$(APPSTART),-slkr/boot4.$(CPU).lkr \
			  --no-crt

else

# --rom=0-7FF : limit the bootloader to a specified ROM range
#LDFLAGS		= --runtime=+init,+clib,+clear,-config,-download,-flp,-no_startup,-osccal,-keep,-plib,-resetbits,-stackcall \

	ifeq ($(FAM), 16f)
		LDFLAGS		= --runtime=-init,+clib,-clear,-config,-download,-flp,-no_startup,-osccal,-keep,-plib,-resetbits,-stackcall \
					  --double=24 --float=24 \
					  --output=intel \
					  --summary=default,+psect,+class,+mem,-hex,-file \
					  --rom=0-$(BOOTEND) \
					  -L-AUSB=$(USBRAM) \
					  -L-pusbram=USB
	else
		LDFLAGS		= --runtime=-init,+clib,-clear,-config,-download,-flp,-no_startup,-osccal,-keep,-plib,-resetbits,-stackcall \
					  --double=24 --float=24 \
					  --output=intel \
					  --summary=default,+psect,+class,+mem,-hex,-file \
					  --rom=0-$(BOOTEND) \
					  -L-AUSB=$(USBRAM) \
					  -L-ASMALLCONST=0h-$(BOOTEND)h \
					  -L-AMEDIUMCONST=0h-$(BOOTEND)h \
					  -L-pusbram=USB \
					  -L-psmallconst=SMALLCONST \
					  -L-pmediumconst=MEDIUMCONST
	endif
endif

########################################################################
#	RULES                                                              #
########################################################################
#                                                                      #
#	$@ 	Le nom de la cible                                             #
#	$< 	Le nom de la première dépendance                               #
#	$^ 	La liste des dépendances                                       #
#	$? 	La liste des dépendances plus récentes que la cible            #
#	$* 	Le nom du fichier sans suffixe                                 #
#                                                                      #
########################################################################

all: clean $(PRJ).hex

clean:
	@echo "***********************************************"
	@echo "\033[1m$(PRJ)\033[0m"
	@echo "***********************************************"
	@rm -f obj/*.o  obj/*.asm obj/*.lst
	@rm -f obj/*.d  obj/*.p1 obj/*.pre obj/*.as obj/*.sdb
	@rm -f hex/*.cod hex/*.map hex/*.lst hex/*.rlf hex/*.sdb

ifeq ($(COMPILER),XC8)

# Compile and assemble all the src/.c in obj/.p1
obj/%.p1: src/%.c
	@$(CC) --pass1 $(CFLAGS) $(LDFLAGS) $(SRCS) -Oobj/

# Links the modules all together
$(PRJ).hex: $(OBJS)
	@$(CC) $(CFLAGS) $(LDFLAGS) $(OBJS) -Ohex/$(PRJ).hex

else

# Compile and assemble all the src/.c in obj/.o
obj/%.o: src/%.c
	@$(CC) $(CFLAGS) $(AFLAGS) -c $< -o$@
	#@$(CC) $(CFLAGS) $(AFLAGS) -c $(SRCS) -oobj/ 

# Links the modules all together
$(PRJ).hex: $(OBJS)
	@$(CC) $(CFLAGS) $(LDFLAGS) -ohex/$@ $^
	#@$(CC) $(CFLAGS) $(LDFLAGS) -ohex/$(PRJ).hex $(OBJS)

endif

# Code size
size:
	@echo -e "\033[1mCode size :\033[0m"
	@tools/codesize.py hex/$(PRJ)

# Programs the Chip
# cf. IPECMD.txt
# COMMAND		MEANING					DEFAULT
# -TP<tool>		Tool Selection			None
# -P<proc>		Processor Selection		None 
# -F<file>		Hex File Selection		None 
# -M			Program Device			P,E,I,C - Nothing = all
# -W			Externally power target	Power from PM3 
#	tools/picpgm -port USB -pic PIC$(CPU) -p_code ../hex/$(PRJ).hex

upload:
	@$(JAVA) -jar $(IPE) \
			 -M \
			 -TP$(PROGRAMMER) \
			 -P$(CPU) \
			 -Fhex/$(PRJ).hex
	@rm -f -v MPLABXLog.* > /dev/null
	@rm -f -v log.* > /dev/null

#			 -GP$(APPSTART)-1FFF \
#			 -GP0008-1FFF \
#			  > hex/memory.hex

read:
	@$(JAVA) -jar $(IPE) \
			 -GFhex/memory.hex \
			 -TP$(PROGRAMMER) \
			 -P$(CPU)
	@rm -f -v MPLABXLog.* > /dev/null
	@rm -f -v log.* > /dev/null

# Desassemble
desaboot:
	@$(SDCCDIR)/gpdasm -p$(CPU) hex/$(PRJ).hex

desamem:
	@$(SDCCDIR)/gpdasm -p$(CPU) hex/memory.hex
