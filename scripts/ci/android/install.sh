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

# Get project generator
cd ~/
rm -rf projectGenerator
mkdir -p ~/projectGenerator
cd ~/projectGenerator
echo "Downloading projectGenerator from ci server"
wget http://ci.openframeworks.cc/projectGenerator/projectGenerator_linux
chmod +x projectGenerator_linux