#!/usr/bin/env bash
set -e
VER=master
PLATFORM=""
ARCH=""
OVERWRITE=1
SILENT_ARGS=""
BLEEDING_EDGE=0

printHelp(){
cat << EOF
    Usage: download_pg.sh [OPTIONS]

    Options:

    -v, --version VERSION       OF version to download the libraries for. Defaults to master
    -p, --platform PLATFORM     Platorm among: android, emscritpen, ios, linux, linux64, linuxarmv6l, linuxarmv7l, msys2, osx, tvos, vs
                                If not specified tries to autodetect the platform
    -s, --silent                Silent download progress
    -h, --help                  Shows this message
EOF
}

SCRIPT_DIR="${BASH_SOURCE%/*}"
if [[ ! -d "$SCRIPT_DIR" ]]; then SCRIPT_DIR="$PWD"; fi
. "$SCRIPT_DIR/downloader.sh"

download(){
    echo "Downloading $1"
    
    if [[ $BLEEDING_EDGE = 1 ]] ; then
        echo downloader https://github.com/openframeworks/projectGenerator/releases/download/bleeding/$1 $SILENT_ARGS
        downloader https://github.com/openframeworks/projectGenerator/releases/download/bleeding/$1 $SILENT_ARGS
    else
        echo downloader https://github.com/openframeworks/projectGenerator/releases/download/nightly/$1
        downloader https://github.com/openframeworks/projectGenerator/releases/download/nightly/$1 
    fi
}

# trap any script errors and exit
trap 'trapError ${LINENO}' ERR
trap "trapError" SIGINT SIGTERM

trapError() {
    local parent_lineno="$1"
    if [[ "$#" = "3" ]] ; then
        local message="$2"
        local code="${3:-1}"
        echo "Error on or near line ${parent_lineno}: ${message}; exiting with status ${code}"
    else
        local code="${2:-1}"
        echo "Error on or near line ${parent_lineno}; exiting with status ${code}"
    fi

    if [ -e openFrameworksLibs* ]; then
        echo "removing packages"
    	rm openFrameworksLibs*
    fi
    exit "${code}"
}


while [[ $# -gt 0 ]]; do
    key="$1"
    case $key in
        -v|--version)
        VER="$2"
        shift # past argument
        ;;
        -p|--platform)
        PLATFORM="$2"
        shift # past argument
        ;;
        -a|--arch)
        ARCH="$2"
        shift # past argument
        ;;
        -n|--no-overwrite)
        OVERWRITE=0
        ;;
        -b|--bleeding-edge)
        BLEEDING_EDGE=1
        ;;
        -s|--silent)
        SILENT_ARGS=-nv
        ;;
        -h|--help)
        printHelp
        exit 0
        ;;
        *)
        echo "Error: invalid argument: $key"
        printHelp
        exit 1
        ;;
    esac
    shift # past argument or value
done

if [ "$PLATFORM" == "" ]; then
    OS=$(uname)
    if [ "$OS" == "Linux" ]; then
        PLATFORM="linux"
    elif [ "$OS" == "Darwin" ]; then
        PLATFORM="osx"
    elif [ "${OS:0:5}" == "MINGW" ]; then
        PLATFORM="msys2"
    else
        # otherwise we are on windows and will download vs
        PLATFORM="vs"
    fi
fi

if [ "$ARCH" == "" ]; then
    if [ "$PLATFORM" == "linux" ]; then
        ARCH=$(uname -m)
        if [ "$ARCH" == "x86_64" ]; then
            GCC_VERSION=$(gcc -dumpversion | cut -f1 -d.)
            if [ $GCC_VERSION -eq 4 ]; then
                ARCH=64gcc6
            elif [ $GCC_VERSION -eq 5 ]; then
                ARCH=64gcc6
            else
                ARCH=64gcc6
            fi
        elif [ "$ARCH" == "armv7l" ]; then
            # Check for Raspberry Pi
            if [ -f /opt/vc/include/bcm_host.h ]; then
                ARCH=armv6l
            fi
        elif [ "$ARCH" == "i686" ] || [ "$ARCH" == "i386" ]; then
            cat << EOF
32bit linux is not officially supported anymore but compiling
the libraries using the build script in apothecary/scripts
should compile all the dependencies without problem
EOF
            exit 1
        fi
    elif [ "$PLATFORM" == "msys2" ]; then
        if [ "$MSYSTEM" == "MINGW64" ]; then
            ARCH=mingw64
        elif [ "$MSYSTEM" == "MINGW32" ]; then
            ARCH=mingw32
        elif [ "$MSYSTEM" == "UCRT64" ]; then
            ARCH=ucrt64
        elif [ "$MSYSTEM" == "CLANG64" ]; then
            ARCH=clang64
        fi
    fi

    if [ "$PLATFORM" == "osx" ]; then
        ARCH=x86_64
    fi
fi


SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cd "$SCRIPT_DIR"

OUTDIR=../../


if [[ $BLEEDING_EDGE = 1 ]] ; then
    VER=bleeding
fi

if [ "$PLATFORM" == "vs" ]; then
    EXT=".exe"
else
    EXT=".app"
fi
OUTPUT=projectGenerator-$PLATFORM
if [ "$PLATFORM" == "msys2" ] || [ "$PLATFORM" == "vs" ]; then
    GUI="-gui"
else
    GUI=""
fi
PKG="projectGenerator-${PLATFORM}${GUI}.zip"
download $PKG

echo "Uncompressing Project Generator for $PLATFORM from $PKG"
if [ "$PLATFORM" == "msys2" ] || [ "$PLATFORM" == "vs" ]; then
    unzip -q "$PKG" -d "$OUTPUT"
    rm $PKG
else
    tar xjf "$PKG"
    rm $PKG
fi

if [ -d "${OUTDIR}/${OUTPUT}" ] || [ -f "${OUTDIR}/${OUTPUT}" ]; then
        rm -rf "${OUTDIR}/${OUTPUT}"
fi

if [ "$PLATFORM" == "msys2" ] || [ "$PLATFORM" == "vs" ]; then

    if ! command -v rsync &> /dev/null
    then      
        cp -ar ${OUTPUT}/ ${OUTDIR}/${OUTPUT}
    else
        rsync -a ${OUTPUT}/ ${OUTDIR}/${OUTPUT}
    fi
    rm -rf $OUTPUT
else
    if ! command -v rsync &> /dev/null
    then      
        cp -ar $OUTPUT/projectGenerator$EXT $OUTDIR/ 
    else
        rsync -a $OUTPUT/projectGenerator$EXT $OUTDIR  
    fi
    rm -rf $OUTPUT/projectGenerator$EXT

fi

rm -rf $OUTPUT
rm -rf $PKG

echo "Completed projectGenerator in place"



