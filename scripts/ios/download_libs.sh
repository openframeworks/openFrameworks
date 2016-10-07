#!/usr/bin/env bash
if [ $# -gt 0 ]; then
    version=$1
else
    version=master
fi
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cd $SCRIPT_DIR
../dev/download_libs.sh $version tvos 1
../dev/download_libs.sh $version tvos 2 0
../dev/download_libs.sh $version ios 1 0
../dev/download_libs.sh $version ios 2 0
