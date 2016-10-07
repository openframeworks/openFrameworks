#!/usr/bin/env bash
if [ $# -gt 0 ]; then
    ARCH=$1
else
    ARCH=$(uname -m)
fi
if [ $# -gt 1 ]; then
    version=$2
else
    version=master
fi

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cd $SCRIPT_DIR

export LC_ALL=C
if [ "$ARCH" == "x86_64" ]; then
    GCC_MAJOR_GT_4=$(expr `gcc -dumpversion | cut -f1 -d.` \> 4)
    if [ $GCC_MAJOR_GT_4 -eq 1 ]; then
        ../dev/download_libs.sh $version linux 64gcc5
    else
        ../dev/download_libs.sh $version linux 64
    fi
else
    ../dev/download_libs.sh $version linux $ARCH
fi
