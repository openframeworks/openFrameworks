#!/bin/bash

export LC_ALL=C
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"
OF_ROOT=$SCRIPT_DIR/../..

# parameters
THREADS=""
BUILD_TYPE="Release"
for arg in "$@"
do
    if [[ $arg == -j* ]]; then
        THREADS=$arg
    elif [[ $arg == "Debug" ]] || [[ $arg == "Release" ]]; then
        BUILD_TYPE=$arg
    fi
done

# ignore all example categories we do not want
# match all subdirectories that end on "Example"
# enter them, clean, compile and run    
find $OF_ROOT/examples \
    -type d \( -name android -o -name ios -o -name gles \) -prune -false \
    -o -type d -name '*Example' \
    -exec bash -c "cd {} && make clean && make $BUILD_TYPE $THREADS && make Run$BUILD_TYPE" \;
