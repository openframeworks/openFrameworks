#!/bin/bash
set -ev
# capture failing exits in commands obscured behind a pipe
set -o pipefail

if [ -z ${OF_ROOT} ]; then
    OF_ROOT=${TRAVIS_BUILD_DIR:-"$( cd "$(dirname "$0")/../../.." ; pwd -P )"}
fi

if [[ "$GITHUB_ACTIONS" = true ]]; then
    OF_ROOT=$GITHUB_WORKSPACE
fi

# Install linux dependencies (for project generator to work)
sudo $OF_ROOT/scripts/linux/ubuntu/install_dependencies.sh -y;

sudo add-apt-repository -y ppa:dns/gnu
sudo apt-get update -q
sudo apt-get install -y --allow-unauthenticated gcc-6 g++-6
sudo apt-get install -y gperf coreutils libxrandr-dev libxinerama-dev libx11-dev libxcursor-dev libxi-dev
sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-6 100
sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-6 100

SCRIPT_DIR="${BASH_SOURCE%/*}"
if [[ ! -d "$SCRIPT_DIR" ]]; then SCRIPT_DIR="$PWD"; fi
. "$SCRIPT_DIR/../../dev/downloader.sh"

# Download NDK
cd ~

# check if cached directory exists
if [ "$(ls -A ${NDK_DIR})" ]; then
    echo "Using cached NDK"
    ls -A ${NDK_DIR}
else
    echo "Downloading NDK"
    downloader -q "https://dl.google.com/android/repository/$NDK_DIR-linux-x86_64.zip"
    echo "Uncompressing NDK"
    unzip "$NDK_DIR-linux-x86_64.zip" > /dev/null 2>&1
fi

# Build project generator
if [ -f ~/projectGenerator/projectGenerator_linux ]; then
    echo "project generator is locally cached not building"
#    below is if we need to debug any crashes with pg
#    sudo apt-get install -y gdb
#    gdb -ex run ~/projectGenerator/projectGenerator_linux
else
    echo "building project generator"
    git clone --depth=1 https://github.com/openframeworks/projectGenerator
    rm -rf $OF_ROOT/apps/projectGenerator 2> /dev/null
    mv projectGenerator $OF_ROOT/apps/projectGenerator
    cd $OF_ROOT/
    scripts/linux/download_libs.sh
    cd $OF_ROOT/apps/projectGenerator/commandLine
    make -j Debug -C .
    ret=$?
    if [ $ret -ne 0 ]; then
          echo "Failed building Project Generator"
          exit 1
    fi

    mkdir -p ~/projectGenerator
    cp bin/projectGenerator_debug ~/projectGenerator/projectGenerator_linux
fi

chmod +x ~/projectGenerator/projectGenerator_linux
cd $OF_ROOT/

# old approach
#cd ~/
#rm -rf projectGenerator
#mkdir -p ~/projectGenerator
#cd ~/projectGenerator
#echo "Downloading projectGenerator from ci server"
#downloader http://ci.openframeworks.cc/projectGenerator/projectGenerator_linux
#chmod +x projectGenerator_linux
