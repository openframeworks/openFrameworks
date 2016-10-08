#!/usr/bin/env bash
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cd $SCRIPT_DIR
../dev/download_libs.sh -p tvos -a 1 -o
../dev/download_libs.sh -p tvos -a 2
../dev/download_libs.sh -p ios -a 1
../dev/download_libs.sh -p ios -a 2
