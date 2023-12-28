#!/usr/bin/env bash
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cd $SCRIPT_DIR
cd $SCRIPT_DIR
if [ ! -z ${BITS+x} ]; then
    ../dev/download_libs.sh -p osx -b -a $BITS $@
else
    ../dev/download_libs.sh -p osx -b -a x86_64 $@
    ../dev/download_libs.sh -p osx -b -a arm64 -n $@
fi
