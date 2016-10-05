#!/usr/bin/env bash
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cd $SCRIPT_DIR
../dev/download_libs.sh $1 tvos 1
../dev/download_libs.sh $1 tvos 2
../dev/download_libs.sh $1 ios 1
../dev/download_libs.sh $1 ios 2
