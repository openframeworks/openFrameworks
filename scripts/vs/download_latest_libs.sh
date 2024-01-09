#!/usr/bin/env bash
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cd $SCRIPT_DIR
cd $SCRIPT_DIR
if [ ! -z ${BITS+x} ]; then
    ../dev/download_libs.sh -p vs -b -a $BITS $@
else
    ../dev/download_libs.sh -p vs -b -a 64 $@
    ../dev/download_libs.sh -p vs -b -a arm64 -n $@
    ../dev/download_libs.sh -p vs -b -a arm64ec -n $@
fi
