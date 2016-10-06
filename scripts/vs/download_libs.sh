#!/usr/bin/env bash
if [ $# -gt 1 ]; then
    version=$1
else
    version=master
fi

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cd $SCRIPT_DIR
echo "BUILDER $BUILDER PLATFORM $PLATFORM"
if [ ! -z ${PLATFORM+x} ]; then
    if [ "$PLATFORM" == "x86" ]; then
        BITS=32
    else
        BITS=64
    fi
    ../dev/download_libs.sh $version vs BITS
else
    ../dev/download_libs.sh $version vs 32
    ../dev/download_libs.sh $version vs 64
fi
