#!/bin/bash
set -ev
# capture failing exits in commands obscured behind a pipe
set -o pipefail
ROOT=${TRAVIS_BUILD_DIR:-"$( cd "$(dirname "$0")/../../.." ; pwd -P )"}
cd ~
# check if cached directory exists
if [ "$(ls -A android-ndk-r10e)" ]; then
    echo "Using cached NDK"
    ls -A android-ndk-r10e
else
    echo "Downloading NDK"
    # curl -Lk http://dl.google.com/android/ndk/android-ndk-r10e-linux-x86_64.bin -o ndk.bin
    # get slimmed and recompressed NDK from our server instead
    curl -Lk http://ci.openframeworks.cc/android-ndk-r10e.tar.bz2 -o ndk.bin
    chmod a+x ndk.bin
    # "Normal" way:
    # ./ndk.bin -y | grep -v Extracting
    # extract with 7zip, -mmt=1 makes it use only 1 thread to reduce memory consumption:
    # 7z x -mmt=1 ndk.bin $(sed 's/^/ -xr!/g' $ROOT/scripts/ci/android/NDK_excludes.txt) | grep -v -e "Extracting" -e "Skipping"
    # extract customized NDK:
    tar -xjf ndk.bin
    rm ndk.bin
fi
NDK_ROOT=$(echo ${PWD} | sed "s/\//\\\\\//g")
cat $ROOT/libs/openFrameworksCompiled/project/android/paths.default.make | sed s/path_to/${NDK_ROOT}/ > $ROOT/libs/openFrameworksCompiled/project/android/paths.make
