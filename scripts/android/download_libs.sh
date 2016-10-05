#!/usr/bin/env bash
if [ $# -gt 1 ]; then
    version=$1
else
    version=master
fi
../dev/download_libs.sh $version android armv7
../dev/download_libs.sh $version android x86
