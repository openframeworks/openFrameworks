#!/usr/bin/env bash
if [ $# -gt 1 ]; then
    version=$1
else
    version=master
fi
../dev/download_libs.sh $version msys2 32
