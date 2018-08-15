#!/usr/bin/env bash
set -e
VER=master
PLATFORM=""
ARCH=""
OVERWRITE=1
SILENT_ARGS=""

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cd $SCRIPT_DIR

printHelp(){
cat << EOF
    Usage: download_generator.sh [OPTIONS]

    Options:
    -p, --platform PLATFORM     Platorm among: android, emscritpen, ios, linux, linux64, linuxarmv6l, linuxarmv7l, msys2, osx, tvos, vs2015, vs2017
                                If not specified tries to autodetect the platform.
   
EOF
}

SCRIPT_DIR="${BASH_SOURCE%/*}"
if [[ ! -d "$SCRIPT_DIR" ]]; then SCRIPT_DIR="$PWD"; fi
. "$SCRIPT_DIR/downloader.sh"

PLATFORM=$1;

if [ "$PLATFORM" == "" ]; then
    echo "Error: invalid argument: $key"
    printHelp
    exit 1
fi

if [ "$PLATFORM" == "osx" ] || [ "$PLATFORM" == "linux" ]; then
	SEPERATOR="_"
else
	SEPERATOR="-"
fi


# Get project generator
cd ./../../
echo "Downloading oF projectGenerator from ci server for $PLATFORM"
downloader http://ci.openframeworks.cc/projectGenerator/projectGenerator$SEPERATOR$PLATFORM.zip
unzip -oq projectGenerator$SEPERATOR$PLATFORM
rm projectGenerator$SEPERATOR$PLATFORM 2> /dev/null
echo "Project Generator for $PLATFORM"
