#!/bin/bash
NDK_DIR=android-ndk-r12b
set -ev
# capture failing exits in commands obscured behind a pipe
set -o pipefail
if [ -z ${OF_ROOT} ]; then
    OF_ROOT=${TRAVIS_BUILD_DIR:-"$( cd "$(dirname "$0")/../../.." ; pwd -P )"}
fi
cd ~
# check if cached directory exists
if [ "$(ls -A ${NDK_DIR})" ]; then
    echo "Using cached NDK"
    ls -A ${NDK_DIR}
else
    echo "Downloading NDK"
    wget https://dl.google.com/android/repository/android-ndk-r12b-linux-x86_64.zip
    echo "Uncompressing NDK"
    unzip android-ndk-r12b-linux-x86_64.zip > /dev/null 2>&1
fi
NDK_ROOT=$(echo ${PWD} | sed "s/\//\\\\\//g")
cat $OF_ROOT/libs/openFrameworksCompiled/project/android/paths.default.make | sed s/path_to/${NDK_ROOT}/ > $OF_ROOT/libs/openFrameworksCompiled/project/android/paths.make
