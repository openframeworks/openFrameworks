#!/usr/bin/env bash
if [ $# -gt 1 ]; then
    version=$1
else
    version=master
fi
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cd $SCRIPT_DIR
../dev/download_libs.sh $version android armv7
../dev/download_libs.sh $version android x86 0
