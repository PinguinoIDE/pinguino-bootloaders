########################################################################
#                                                                      #
#	Pinguino Bootloader v4.x                                           #
#	8-bit USB Bootloader                                               #
#	Author:	Régis Blanchot <rblanchot@gmail.com>                       #
#           André Gentric                                              #
#                                                                      #
#	Usage ex. : make --makefile=Makefile PROC=18f26j50 OSC=20          #
#          to compile the bootloader                                   #
#                                                                      #
#	This file is part of Pinguino Project (http://www.pinguino.cc)     #
#	Released under the LGPL license (www.gnu.org/licenses/lgpl.html)   #
#                                                                      #
########################################################################
#	Supported CPU's                                                    #
########################################################################
#                                                                      #
#	18f2455		18f4455                                                #
#	18f2550		18f4550                                                #
#	18f25k50	18f45k50                                               #
#	18f26j50	18f46j50                                               #
#	18F26J53	18F46J53                                               #
#	18F27J53	18F47J53                                               #
#                                                                      #
########################################################################
#	Work in progress                                                   #
########################################################################
#                                                                      #
#	16f1455                                                            #
#	18f13k50	18f14k50                                               #
#                                                                      #
########################################################################

########################################################################
#	SDCC'S TOOLCHAIN LOCATION                                          #
########################################################################

SDCCDIR		= /usr/bin

########################################################################
#	INTERNAL OPTIONAL CONFIGURATION OPTIONS                            #
########################################################################

LOW_SPEED	= 1
HIGH_SPEED	= 0
ONE			= 1

########################################################################
#	OPTIONAL CONFIGURATION OPTIONS                                     #
########################################################################

CPU			= $(PROC)
CRYSTAL		= $(OSC)

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
STRING		= 0
else
STRING		= $(STRINGDESC)
endif

########################################################################
#	DO NOT CHANGE FOLLOWINGS WITHOUT CARE                              #
########################################################################

# bootloader version (cf. changelog.txt)
MAJ_VER		= 4
MIN_VER		= 15

# Microchip Vendor ID / Pinguino Product ID (Microchip sublicense)
VID			= 0x04D8
PID			= 0xFEAA

# ENTRY address is the end of bootloader / start of user's application
# ENTRY must be a multiple of 1024 because :
# 1/ flash must be erased before any write,
# 2/ some PICs can only erase 1024-byte long blocks 
ENTRY		= 0x0C00

# PIC Family (18F or 16F ?)
FAM			= $(findstring 18f, $(CPU))

ifeq ($(FAM), 18f)
ARCH		= pic16
OPTIMIZ		= --optimize-df --optimize-cmp --obanksel=9 --denable-peeps
else
ARCH		= pic14
endif

ifeq ($(CRYSTAL), INTOSC)
PRJ		= Bootloader_v$(MAJ_VER).$(MIN_VER)_$(CPU)_$(CRYSTAL)
else
PRJ		= Bootloader_v$(MAJ_VER).$(MIN_VER)_$(CPU)_X$(CRYSTAL)MHz
endif

# files
SRCS		= $(wildcard src/*.c)
OBJS		= $(SRCS:src/%.c=obj/%.o)

# compiler
CC			= $(SDCCDIR)/sdcc

# Compiler flags
CFLAGS		= -V \
			  -m$(ARCH) \
			  -p$(CPU) \
			  $(OPTIMIZ) \
			  --use-non-free \
			  --fomit-frame-pointer \
			  -D VENDORID=$(VID) \
			  -D PRODUCTID=$(PID) \
			  -D ENTRY=$(ENTRY) \
			  -D STRING=$(STRING) \
			  -D SPEED=$(USBSPEED) \
			  -D LOW_SPEED=1 \
			  -D HIGH_SPEED=0 \
			  -D LVP=$(LVP) \
			  -D CRYSTAL=$(CRYSTAL) \
			  -D MAJOR_VERSION=$(MAJ_VER) \
			  -D MINOR_VERSION=$(MIN_VER)
#			  --pstack-model=large \

# Assembler flags
# -w[0|1|2] : set message level
AFLAGS		= -Wa,"-w0"

# Linker flags
# -m : output a map file
# -w : disable "processor mismatch" warning
# -s : we use our own linker script and startup code (work)
# -u : add macro value for script.
# --no-crt : do not link the default run-time modules

LDFLAGS		= -Wl-uENTRY=$(ENTRY),-slkr/boot4.$(CPU).lkr \
			  --no-crt

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

#all: start clean $(CRTP).o $(PRJ).hex size
#all: start clean $(CRTB).o $(PRJ).hex size
all: start clean $(PRJ).hex size

# Start message
start:
	@echo $(PRJ) ...

# Deletes all the debug files generated
clean:
	@rm -f obj/*.o  obj/*.asm obj/*.lst
	@rm -f hex/*.cod hex/*.map hex/*.lst

# Compiles obj/crt0Boot4.o
#$(CRTB).o:
#	@$(CC) $(CFLAGS) $(AFLAGS) -c obj/$(CRTB).c -o obj/$(CRTB).o

# Compiles and assembles all the src/.c in obj/.o
obj/%.o: src/%.c
	@$(CC) $(CFLAGS) $(AFLAGS) -c $< -o $@

# Links the modules all together
$(PRJ).hex: $(OBJS)
	@$(CC) $(CFLAGS) $(LDFLAGS) -o hex/$@ $^

# Calculates the whole code size
size:
	@tools/codesize.py hex/$(PRJ)
	
# Programs the Chip
burn:
	tools/picpgm -port USB -pic PIC$(CPU) -p_code ../hex/$(PRJ).hex

# Desassemble
desa:
	$(SDCCDIR)/gpdasm -p$(CPU) hex/$(PRJ).hex > ~/Bureau/$(PRJ).txt
