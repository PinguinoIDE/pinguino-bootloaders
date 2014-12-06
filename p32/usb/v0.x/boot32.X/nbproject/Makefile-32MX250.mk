#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-32MX250.mk)" "nbproject/Makefile-local-32MX250.mk"
include nbproject/Makefile-local-32MX250.mk
endif
endif

# Environment
MKDIR=mkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=32MX250
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/boot32.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/boot32.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=../lkr/crt0.S ../main.c ../serial.c ../flash.c ../delay.c ../core.c ../usb.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/_ext/1360930308/crt0.o ${OBJECTDIR}/_ext/1472/main.o ${OBJECTDIR}/_ext/1472/serial.o ${OBJECTDIR}/_ext/1472/flash.o ${OBJECTDIR}/_ext/1472/delay.o ${OBJECTDIR}/_ext/1472/core.o ${OBJECTDIR}/_ext/1472/usb.o
POSSIBLE_DEPFILES=${OBJECTDIR}/_ext/1360930308/crt0.o.d ${OBJECTDIR}/_ext/1472/main.o.d ${OBJECTDIR}/_ext/1472/serial.o.d ${OBJECTDIR}/_ext/1472/flash.o.d ${OBJECTDIR}/_ext/1472/delay.o.d ${OBJECTDIR}/_ext/1472/core.o.d ${OBJECTDIR}/_ext/1472/usb.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/_ext/1360930308/crt0.o ${OBJECTDIR}/_ext/1472/main.o ${OBJECTDIR}/_ext/1472/serial.o ${OBJECTDIR}/_ext/1472/flash.o ${OBJECTDIR}/_ext/1472/delay.o ${OBJECTDIR}/_ext/1472/core.o ${OBJECTDIR}/_ext/1472/usb.o

# Source Files
SOURCEFILES=../lkr/crt0.S ../main.c ../serial.c ../flash.c ../delay.c ../core.c ../usb.c


CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-32MX250.mk dist/${CND_CONF}/${IMAGE_TYPE}/boot32.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=32MX250F128B
MP_LINKER_FILE_OPTION=,--script="../lkr/32MX250F128B/procdefs.ld"
# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assembleWithPreprocess
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/1360930308/crt0.o: ../lkr/crt0.S  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1360930308 
	@${RM} ${OBJECTDIR}/_ext/1360930308/crt0.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360930308/crt0.o 
	@${RM} ${OBJECTDIR}/_ext/1360930308/crt0.o.ok ${OBJECTDIR}/_ext/1360930308/crt0.o.err 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360930308/crt0.o.d" "${OBJECTDIR}/_ext/1360930308/crt0.o.asm.d" -t $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC} $(MP_EXTRA_AS_PRE)  -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -c -mprocessor=$(MP_PROCESSOR_OPTION) -mno-float -MMD -MF "${OBJECTDIR}/_ext/1360930308/crt0.o.d"  -o ${OBJECTDIR}/_ext/1360930308/crt0.o ../lkr/crt0.S  -Wa,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_AS_POST),-MD="${OBJECTDIR}/_ext/1360930308/crt0.o.asm.d",--defsym=__ICD2RAM=1,--defsym=__MPLAB_DEBUG=1,--gdwarf-2,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_PK3=1,--no-warn
	
else
${OBJECTDIR}/_ext/1360930308/crt0.o: ../lkr/crt0.S  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1360930308 
	@${RM} ${OBJECTDIR}/_ext/1360930308/crt0.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360930308/crt0.o 
	@${RM} ${OBJECTDIR}/_ext/1360930308/crt0.o.ok ${OBJECTDIR}/_ext/1360930308/crt0.o.err 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360930308/crt0.o.d" "${OBJECTDIR}/_ext/1360930308/crt0.o.asm.d" -t $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC} $(MP_EXTRA_AS_PRE)  -c -mprocessor=$(MP_PROCESSOR_OPTION) -mno-float -MMD -MF "${OBJECTDIR}/_ext/1360930308/crt0.o.d"  -o ${OBJECTDIR}/_ext/1360930308/crt0.o ../lkr/crt0.S  -Wa,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_AS_POST),-MD="${OBJECTDIR}/_ext/1360930308/crt0.o.asm.d",--no-warn
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/1472/main.o: ../main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/main.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/main.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/main.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -mno-float -O1 -MMD -MF "${OBJECTDIR}/_ext/1472/main.o.d" -o ${OBJECTDIR}/_ext/1472/main.o ../main.c   
	
${OBJECTDIR}/_ext/1472/serial.o: ../serial.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/serial.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/serial.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/serial.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -mno-float -O1 -MMD -MF "${OBJECTDIR}/_ext/1472/serial.o.d" -o ${OBJECTDIR}/_ext/1472/serial.o ../serial.c   
	
${OBJECTDIR}/_ext/1472/flash.o: ../flash.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/flash.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/flash.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/flash.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -mno-float -O1 -MMD -MF "${OBJECTDIR}/_ext/1472/flash.o.d" -o ${OBJECTDIR}/_ext/1472/flash.o ../flash.c   
	
${OBJECTDIR}/_ext/1472/delay.o: ../delay.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/delay.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/delay.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/delay.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -mno-float -O1 -MMD -MF "${OBJECTDIR}/_ext/1472/delay.o.d" -o ${OBJECTDIR}/_ext/1472/delay.o ../delay.c   
	
${OBJECTDIR}/_ext/1472/core.o: ../core.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/core.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/core.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/core.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -mno-float -O1 -MMD -MF "${OBJECTDIR}/_ext/1472/core.o.d" -o ${OBJECTDIR}/_ext/1472/core.o ../core.c   
	
${OBJECTDIR}/_ext/1472/usb.o: ../usb.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/usb.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/usb.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/usb.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -mno-float -O1 -MMD -MF "${OBJECTDIR}/_ext/1472/usb.o.d" -o ${OBJECTDIR}/_ext/1472/usb.o ../usb.c   
	
else
${OBJECTDIR}/_ext/1472/main.o: ../main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/main.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/main.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/main.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -mno-float -O1 -MMD -MF "${OBJECTDIR}/_ext/1472/main.o.d" -o ${OBJECTDIR}/_ext/1472/main.o ../main.c   
	
${OBJECTDIR}/_ext/1472/serial.o: ../serial.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/serial.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/serial.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/serial.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -mno-float -O1 -MMD -MF "${OBJECTDIR}/_ext/1472/serial.o.d" -o ${OBJECTDIR}/_ext/1472/serial.o ../serial.c   
	
${OBJECTDIR}/_ext/1472/flash.o: ../flash.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/flash.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/flash.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/flash.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -mno-float -O1 -MMD -MF "${OBJECTDIR}/_ext/1472/flash.o.d" -o ${OBJECTDIR}/_ext/1472/flash.o ../flash.c   
	
${OBJECTDIR}/_ext/1472/delay.o: ../delay.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/delay.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/delay.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/delay.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -mno-float -O1 -MMD -MF "${OBJECTDIR}/_ext/1472/delay.o.d" -o ${OBJECTDIR}/_ext/1472/delay.o ../delay.c   
	
${OBJECTDIR}/_ext/1472/core.o: ../core.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/core.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/core.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/core.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -mno-float -O1 -MMD -MF "${OBJECTDIR}/_ext/1472/core.o.d" -o ${OBJECTDIR}/_ext/1472/core.o ../core.c   
	
${OBJECTDIR}/_ext/1472/usb.o: ../usb.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/usb.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/usb.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/usb.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -mno-float -O1 -MMD -MF "${OBJECTDIR}/_ext/1472/usb.o.d" -o ${OBJECTDIR}/_ext/1472/usb.o ../usb.c   
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compileCPP
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/boot32.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    ../lkr/32MX250F128B/procdefs.ld
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mdebugger -D__MPLAB_DEBUGGER_PK3=1 -mprocessor=$(MP_PROCESSOR_OPTION) -nostartfiles -mno-float -o dist/${CND_CONF}/${IMAGE_TYPE}/boot32.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}           -mreserve=data@0x0:0x1FC -mreserve=boot@0x1FC00490:0x1FC00BEF  -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_PK3=1,--report-mem,--verbose
	
else
dist/${CND_CONF}/${IMAGE_TYPE}/boot32.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   ../lkr/32MX250F128B/procdefs.ld
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mprocessor=$(MP_PROCESSOR_OPTION) -nostartfiles -mno-float -o dist/${CND_CONF}/${IMAGE_TYPE}/boot32.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}          -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--report-mem,--verbose
	${MP_CC_DIR}/xc32-bin2hex dist/${CND_CONF}/${IMAGE_TYPE}/boot32.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} 
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/32MX250
	${RM} -r dist/32MX250

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell "${PATH_TO_IDE_BIN}"mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
