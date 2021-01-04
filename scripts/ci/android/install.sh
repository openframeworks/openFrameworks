#!/bin/bash
set -ev
# capture failing exits in commands obscured behind a pipe
set -o pipefail


if [ -z ${OF_ROOT} ]; then
    OF_ROOT=${TRAVIS_BUILD_DIR:-"$( cd "$(dirname "$0")/../../.." ; pwd -P )"}
fi

# Install linux dependencies (for project generator to work)
sudo $OF_ROOT/scripts/linux/ubuntu/install_dependencies.sh -y;

# Download NDK
cd ~
# check if cached directory exists
if [ "$(ls -A ${NDK_DIR})" ]; then
    echo "Using cached NDK"
    ls -A ${NDK_DIR}
else
    echo "Downloading NDK"
    wget -q "https://dl.google.com/android/repository/$NDK_DIR-linux-x86_64.zip"
    echo "Uncompressing NDK"
    unzip "$NDK_DIR-linux-x86_64.zip" > /dev/null 2>&1
fi

# Build project generator
if [ -f ~/projectGenerator/projectGenerator_linux ]; then
    echo "project generator is locally cached not building"
    chmod +x ~/projectGenerator/projectGenerator_linux
else
    echo "building project generator"
    git clone --depth=1 https://github.com/openframeworks/projectGenerator
    rm -rf $OF_ROOT/apps/projectGenerator 2> /dev/null
    mv projectGenerator $OF_ROOT/apps/projectGenerator
    cd $OF_ROOT/
    scripts/linux/download_libs.sh
    cd $OF_ROOT/apps/projectGenerator/commandLine
    make Release -C .
    ret=$?
    if [ $ret -ne 0 ]; then
          echo "Failed building Project Generator"
          exit 1
    fi

    mkdir -p ~/projectGenerator
    cp bin/projectGenerator ~/projectGenerator/projectGenerator_linux
    chmod +x ~/projectGenerator/projectGenerator_linux
    cd $OF_ROOT/
fi

# old approach
#cd ~/
#rm -rf projectGenerator
#mkdir -p ~/projectGenerator
#cd ~/projectGenerator
#echo "Downloading projectGenerator from ci server"
#wget http://ci.openframeworks.cc/projectGenerator/projectGenerator_linux
#chmod +x projectGenerator_linux
