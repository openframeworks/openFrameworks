#!/usr/bin/env bash
if [ $# -gt 0 ]; then
    ARCH=$1
else
    ARCH=$(uname -m)
fi
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cd $SCRIPT_DIR

export LC_ALL=C
if [ "$ARCH" == "x86_64" ]; then
    GCC_MAJOR_GT_4=$(expr `gcc -dumpversion | cut -f1 -d.` \> 4)
    if [ $GCC_MAJOR_GT_4 -eq 1 ]; then
        ARCH=64gcc5
    else
        ARCH=64
    fi
fi
../dev/download_libs.sh -p linux -a $ARCH -o
