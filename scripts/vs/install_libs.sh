#!/usr/bin/env bash
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cd $SCRIPT_DIR
if [ ! -z ${BITS+x} ]; then
    ../dev/local_libs.sh -p vs -b $@ -a $BITS
else
    ../dev/local_libs.sh -p vs -b $@
fi
