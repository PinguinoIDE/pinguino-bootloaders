#!/bin/bash

# ----------------------------------------------------------------------
# description:		Build GPUTILS and SDCC compiler for ©Microchip PIC
# author:			(c)2011 - regis blanchot <rblanchot@gmail.com>
# first release:	20-04-2011
# last release:		16-08-2012
# ----------------------------------------------------------------------

########################################################################
# Default settings section
########################################################################

# Set to 'sudo' if you need superuser privileges while installing
SUDO=
# Override automatic detection of cpus to compile on
CPUS=

########################################################################
# CPU's detection
########################################################################

if [ "x${CPUS}" == "x" ]; then
	if which getconf > /dev/null; then
		CPUS=$(getconf _NPROCESSORS_ONLN)
	else
		CPUS=1
	fi
	PARALLEL=-j$((CPUS + 1))
else
	PARALLEL=-j${CPUS}
fi

echo "${CPUS} cpu's detected running make with '${PARALLEL}' flag"

########################################################################
# Settings
########################################################################

# Install the compiler where the script is
PREFIX=$(pwd)
SOURCES=${PREFIX}/sources
LOG=${PREFIX}/log
export PATH=${PATH}:${PREFIX}/bin

# SVN Versions
SDCC=sdcc
GPUTILS=gputils

# Flags
MAKEFLAGS=${PARALLEL}

########################################################################
# Functions
########################################################################

# Log a message out to the console
function log {
    echo "----------------------------------------"
    echo " $*"
    echo "----------------------------------------"
}

# Fetch a versioned file from a URL
function fetch {
    log "Downloading $1 sources..."
	svn co $2
}

# Install a build
function install {
    log $1
    ${SUDO} make ${MAKEFLAGS} $2 $3 $4 $5 $6 $7 $8
}

########################################################################
# Start
########################################################################

start=`date +"%H:%M:%S"`

########################################################################
# OS detection
########################################################################

case "$(uname)" in
	Linux)
	echo "Found Linux OS."
	;;
	Darwin)
	echo "Found Darwin OS."
	echo "OS not yet supported. Aborting!"
	exit 1
	;;
	CYGWIN*)
	echo "Found CygWin that means Windows most likely."
	echo "OS not yet supported. Aborting!"
	exit 1
	;;
	*)
	echo "OS not supported. Aborting!"
	exit 1
	;;
esac

########################################################################
# Build section
########################################################################

mkdir -p ${LOG}/${SDCC} ${LOG}/${GPUTILS} ${SOURCES}

cd ${SOURCES}

# Fetch the latest sources
fetch ${SDCC} https://sdcc.svn.sourceforge.net/svnroot/sdcc/trunk/sdcc
fetch ${GPUTILS} https://gputils.svn.sourceforge.net/svnroot/gputils/trunk/gputils

cd ${PREFIX}

if [ ! -e build ]; then
    mkdir build
fi

# Building gputils
cd build
log "Configuring ${GPUTILS}"
../sources/${GPUTILS}/configure --prefix=${PREFIX} \
								2>&1 | tee ${LOG}/${GPUTILS}/configure.log
log "Building ${GPUTILS}"
make ${MAKEFLAGS} 2>&1 | tee ${LOG}/${GPUTILS}/make.log
install ${GPUTILS} install
cd ..
log "Cleaning up ${GPUTILS}"
rm -rf build/* ${GPUTILS}

# Building sdcc
cd build
log "Configuring ${SDCC}"
../sources/${SDCC}/configure	--prefix=${PREFIX} \
								--disable-mcs51-port \
								--disable-z80-port \
								--disable-z180-port \
								--disable-r2k-port \
								--disable-r3ka-port \
								--disable-gbz80-port \
								--disable-ds390-port \
								--disable-ds400-port \
                            --enable-pic14-port \
							--enable-pic16-port \
								--disable-hc08-port \
								--disable-s08-port \
								--disable-avr-port \
								--disable-xa51-port \
								--disable-ucsim \
							--enable-device-lib \
								--disable-packihx \
							--enable-sdcpp \
								--disable-sdcdb \
							--enable-sdbinutils \
								--disable-doc \
                            --disable-libgc \
							--enable-floats \
							--enable-new-pics \
								2>&1 | tee ${LOG}/${SDCC}/configure.log

log "Building ${SDCC}"
make ${MAKEFLAGS} 2>&1 | tee ${LOG}/${SDCC}/make.log
install ${SDCC} install
cd ..
log "Cleaning up ${SDCC}"
rm -rf build/* ${SDCC}

########################################################################
# End
########################################################################

end=`date +"%H:%M:%S"`

log "sdcc toolchain successfully build."

if [ -e version.txt ]; then
    rm version.txt
fi

echo "author  : rblanchot@gmail.com" >> version.txt
echo "script  : build-sdcc-v2.sh" >> version.txt
echo "host    : `uname` (`uname -r`)" >> version.txt
echo "build   : `date +"%d-%m-%y"`" >> version.txt
echo "version : `bin/sdcc -v`" >> version.txt
echo "          `bin/gpasm -v`" >> version.txt
#echo "process : end-start"
