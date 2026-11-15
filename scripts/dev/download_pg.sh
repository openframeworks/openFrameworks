#!/usr/bin/env bash
set -e
VER=master
PLATFORM=""
ARCH=""
OVERWRITE=1
SILENT_ARGS=""
BLEEDING_EDGE=0
DL_VERSION=2.8.0
TAG=""

printHelp(){
cat << EOF
    Usage: download_pg.sh [OPTIONS]

    Options:

    -v, --version VERSION       OF version to download the libraries for. Defaults to master
    -p, --platform PLATFORM     Platform among: android, emscripten, ios, linux, linux64, linuxarmv6l, linuxarmv7l, msys2, osx, tvos, vs
                                If not specified tries to autodetect the platform
    -s, --silent                Silent download progress
    -h, --help                  Shows this message
    -t, --tag                   tag release for libraries
EOF
}

SCRIPT_DIR="${BASH_SOURCE%/*}"
if [[ ! -d "$SCRIPT_DIR" ]]; then SCRIPT_DIR="$PWD"; fi
. "$SCRIPT_DIR/downloader.sh"

download(){
    echo "Downloading $1"

    REPO="nightly"
    if [[ $BLEEDING_EDGE = 1 ]] ; then
        REPO="latest"
    fi
    if [[ $TAG != "" ]] ; then
        REPO="$TAG"
    fi

    echo downloader https://github.com/openframeworks/projectGenerator/releases/download/$REPO/$1
    downloader https://github.com/openframeworks/projectGenerator/releases/download/$REPO/$1

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
        -t|--tag)
        TAG="$2"
        shift # past argument
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
    elif [[ "$OS" =~ "MINGW" ]] || [[ "$OS" =~ "MSYS" ]]; then
        PLATFORM="msys2"
    else
        PLATFORM="vs"
    fi
fi

if [ "$ARCH" == "" ]; then
    if [ "$PLATFORM" == "linux" ]; then
        ARCH=$(uname -m)
        if [ "$ARCH" == "x86_64" ]; then
            ARCH=x86_64
        elif [ "$ARCH" == "armv7l" ]; then
            ARCH=armv7l
        elif [[ "$ARCH" == "aarch64" ]] || [[ "$ARCH" == "arm64" ]]; then
            ARCH=arm64
        else
            ARCH=unknown
        fi
    elif [ "$PLATFORM" == "msys2" ]; then
        ARCH=universal
    elif [ "$PLATFORM" == "vs" ]; then
        ARCH=$(uname -m)
        if [ "$ARCH" == "x86_64" ]; then
            ARCH=64
        elif [[ "$ARCH" == "aarch64" ]] || [[ "$ARCH" == "arm64" ]]; then
            ARCH=arm64
        else
            ARCH=universal
        fi
    elif [[ "$PLATFORM" == "osx" ]] || [[ "$PLATFORM" == "macos" ]]; then
        ARCH=$(uname -m)
        if [ "$ARCH" == "x86_64" ]; then
            ARCH=64
        elif [[ "$ARCH" == "aarch64" ]] || [[ "$ARCH" == "arm64" ]]; then
            ARCH=arm64
        else
            ARCH=universal
        fi
    fi
fi

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cd "$SCRIPT_DIR"

OUTDIR=../../

ZIP="zip"
if [[ $BLEEDING_EDGE = 1 ]] ; then
    VER=latest
fi

if [[ $TAG != "" ]] ; then
    VER="$TAG"
fi

if [ "$PLATFORM" == "linux" ]; then
    ZIP="tar.bz2"
fi
if [ "$PLATFORM" == "vs" ]; then
    EXT=".exe"
else
    EXT=".app"
fi

OUTPUT=projectGenerator
if [ "$PLATFORM" == "msys2" ] || [ "$PLATFORM" == "vs" ]; then
    GUI="-gui"
else
    GUI=""
fi
PKG="projectGenerator-${PLATFORM}${GUI}.${ZIP}"
if [ "$PLATFORM" == "linux" ]; then
    case "$ARCH" in
        x86_64|amd64|64) PKG="projectGenerator-linux-gui.gz" ;;
        aarch64|arm64) PKG="projectGenerator-linux-arm64-gui.gz" ;;
        *) echo "No Linux projectGenerator GUI release for architecture: $ARCH" >&2; exit 1 ;;
    esac
fi
echo " openFrameworks download_pg.sh"

cd ../../
mkdir -p libs
cd libs

mkdir -p download
cd download

download $PKG

if [ "$PLATFORM" == "linux" ]; then
    PG_STAGE=$(mktemp -d "${OUTDIR}/${OUTPUT}.XXXXXX")
    trap 'rm -rf "$PG_STAGE"' EXIT
    tar -xzf "$PKG" --strip-components=1 -C "$PG_STAGE"
    CLI=""
    for candidate in \
        "$PG_STAGE/resources/app.asar.unpacked/app/projectGenerator" \
        "$PG_STAGE/resources/app/app/projectGenerator"; do
        if [ -f "$candidate" ]; then CLI="$candidate"; break; fi
    done
    if [[ ! -f "$PG_STAGE/projectGenerator" || -z "$CLI" ]]; then
        echo "Linux projectGenerator package is missing the GUI or bundled CLI" >&2
        exit 1
    fi
    cp "$CLI" "$PG_STAGE/projectGeneratorCmd"
    chmod +x "$PG_STAGE/projectGenerator" "$PG_STAGE/projectGeneratorCmd" "$CLI"
    rm -rf "${OUTDIR}/${OUTPUT}"
    mv "$PG_STAGE" "${OUTDIR}/${OUTPUT}"
    echo " openFrameworks download projectGenerator and install complete!"
    exit 0
fi

if [ -d "${OUTDIR}/${OUTPUT}" ] || [ -f "${OUTDIR}/${OUTPUT}" ]; then
    rm -rf "${OUTDIR}/${OUTPUT}"
fi

echo " Uncompressing Project Generator for [$PLATFORM] from [$PKG] to [${OUTDIR}/${OUTPUT}]"
if [ "$PLATFORM" == "msys2" ] || [ "$PLATFORM" == "vs" ]; then
    unzip -q "$PKG" -d "${OUTDIR}/${OUTPUT}"
    #rm $PKG
else
    mkdir -p "${OUTDIR}${OUTPUT}"
    tar xjf "$PKG" -C "${OUTDIR}${OUTPUT}"
    if [ "$PLATFORM" == "osx" ]; then
        mv "${OUTDIR}/${OUTPUT}/projectGenerator-osx/projectGenerator$EXT" "${OUTDIR}/${OUTPUT}/projectGenerator$EXT"
        rm -rf "${OUTDIR}/${OUTPUT}/projectGenerator-osx/"
    fi
fi

if [ "$PLATFORM" == "msys2" ] || [ "$PLATFORM" == "vs" ]; then
    if ! command -v rsync &> /dev/null; then      
        cp -ar "${OUTDIR}/${OUTPUT}/resources/app/app/projectGenerator.exe" "${OUTDIR}/${OUTPUT}/projectGeneratorCmd.exe"
    else
        rsync -avzp "${OUTDIR}/${OUTPUT}/resources/app/app/projectGenerator.exe" "${OUTDIR}/${OUTPUT}/projectGeneratorCmd.exe"
    fi
    # if command -v chmod &> /dev/null; then
        chmod +x "${OUTDIR}/${OUTPUT}/projectGeneratorCmd.exe"
        chmod +x "${OUTDIR}/${OUTPUT}/resources/app/app/projectGenerator.exe"
    # else
    #     echo "Warning: chmod command not found, skipping permission adjustment."
    # fi
else
    if ! command -v rsync &> /dev/null; then      
        cp -arX "${OUTDIR}/${OUTPUT}/projectGenerator$EXT/Contents/Resources/app/app/projectGenerator" "${OUTDIR}/${OUTPUT}/projectGenerator"
    else
        rsync -avzp "${OUTDIR}/${OUTPUT}/projectGenerator$EXT/Contents/Resources/app/app/projectGenerator" "${OUTDIR}/${OUTPUT}/projectGenerator"
    fi
    # if command -v chmod &> /dev/null; then
        chmod +x "${OUTDIR}/${OUTPUT}/projectGenerator"
        chmod +x "${OUTDIR}/${OUTPUT}/projectGenerator$EXT/Contents/MacOS/projectGenerator"
    # else
    #     echo "Warning: chmod command not found, skipping permission adjustment."
    # fi
fi

echo " ------ "
echo " openFrameworks download projectGenerator and install complete!"
