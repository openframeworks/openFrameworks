#!/usr/bin/env bash
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cd $SCRIPT_DIR
cd $SCRIPT_DIR
if [ ! -z ${BITS+x} ]; then
    ../dev/download_libs.sh -p ios -b -a $BITS -n $@
else
    ../dev/download_libs.sh -p ios -b -n $@
fi
