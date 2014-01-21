#!/bin/sh
###############################################################################
# PICPgm install script
# Version 1.0.1
# 22.08.2010
# Christian Stadler
###############################################################################

version="1.0.1"
exe_destination="/usr/bin"
cfg_destination="/usr/local/picpgm"

echo ""
echo "PICPgm install script v$version"
echo "============================"

# copy executable
echo "copy executable to $exe_destination ..."
cp -f ./picpgm $exe_destination

# create directory for config
echo "create directory $cfg_destination ..."
mkdir $cfg_destination

# copy config file
echo "copy configuration file to $cfg_destination ..."
cp ./pgmifcfg.xml $cfg_destination

echo "Installation of PICPgm finished!"

