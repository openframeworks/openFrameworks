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
    -p, --platform PLATFORM     Platform among: linux, msys2, osx, macos, vs
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

linuxPGVariant() (
    local ID="" VERSION_ID="" VERSION_CODENAME="" UBUNTU_CODENAME=""
    local root="${OF_PG_PROBE_ROOT:-}"
    [[ -r "$root/etc/os-release" ]] || return 0
    . "$root/etc/os-release"
    if [[ "$UBUNTU_CODENAME" == "jammy" || "$VERSION_CODENAME" == "jammy" || ( "$ID" == "ubuntu" && "$VERSION_ID" == 22.04* ) ]]; then
        printf '%s' '-ubuntu22'
    fi
)

trap 'trapError "$LINENO" "$?"' ERR
trap 'trapError "$LINENO" 130' SIGINT
trap 'trapError "$LINENO" 143' SIGTERM

trapError() {
    local parent_lineno="${1:-unknown}"
    local code="${2:-1}"
    trap - ERR SIGINT SIGTERM
    echo "Error on or near line ${parent_lineno}; exiting with status ${code}" >&2
    exit "$code"
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

OUTPUT=projectGenerator
case "$PLATFORM" in
    linux)
        case "$ARCH" in
            x86_64|amd64|64) PKG="projectGenerator-linux$(linuxPGVariant)-gui.gz" ;;
            aarch64|arm64) PKG="projectGenerator-linux-arm64-gui.gz" ;;
            *) echo "No Linux projectGenerator GUI release for architecture: $ARCH" >&2; exit 1 ;;
        esac
        ;;
    vs|msys2) PKG="projectGenerator-vs-gui.zip" ;;
    osx|macos) PKG="projectGenerator-osx.zip" ;;
    *) echo "Unsupported projectGenerator platform: $PLATFORM" >&2; exit 1 ;;
esac

echo " openFrameworks download_pg.sh"
cd ../../
mkdir -p libs/download
cd libs/download

download "$PKG"

PG_STAGE=$(mktemp -d "${OUTDIR}/${OUTPUT}.XXXXXX")
trap 'rm -rf "$PG_STAGE"' EXIT
echo " Uncompressing Project Generator for [$PLATFORM] from [$PKG]"
case "$PLATFORM" in
    linux)
        tar -xzf "$PKG" --strip-components=1 -C "$PG_STAGE"
        RESOURCES="$PG_STAGE/resources"
        GUI_BIN="$PG_STAGE/projectGenerator"
        CLI_NAME=projectGenerator
        CLI_DEST="$PG_STAGE/projectGeneratorCmd"
        ;;
    vs|msys2)
        unzip -q "$PKG" -d "$PG_STAGE"
        RESOURCES="$PG_STAGE/resources"
        GUI_BIN="$PG_STAGE/projectGenerator.exe"
        CLI_NAME=projectGenerator.exe
        CLI_DEST="$PG_STAGE/projectGeneratorCmd.exe"
        ;;
    osx|macos)
        unzip -q "$PKG" -d "$PG_STAGE"
        if [ -d "$PG_STAGE/projectGenerator-osx/projectGenerator.app" ]; then
            mv "$PG_STAGE/projectGenerator-osx/projectGenerator.app" "$PG_STAGE/projectGenerator.app"
            rm -rf "$PG_STAGE/projectGenerator-osx"
        fi
        RESOURCES="$PG_STAGE/projectGenerator.app/Contents/Resources"
        GUI_BIN="$PG_STAGE/projectGenerator.app/Contents/MacOS/projectGenerator"
        CLI_NAME=projectGenerator
        CLI_DEST="$PG_STAGE/projectGenerator"
        ;;
esac

CLI=""
for candidate in \
    "$RESOURCES/app.asar.unpacked/app/$CLI_NAME" \
    "$RESOURCES/app/app/$CLI_NAME"; do
    if [ -f "$candidate" ]; then CLI="$candidate"; break; fi
done
if [[ ! -f "$GUI_BIN" ]]; then
    echo "ProjectGenerator package for $PLATFORM is missing the GUI" >&2
    exit 1
fi
if [[ -z "$CLI" && "$PLATFORM" == "linux" ]]; then
    CLI_PKG="${PKG%-gui.gz}.tar.bz2"
    echo "Bundled CLI is missing; downloading $CLI_PKG from the same release"
    download "$CLI_PKG"
    CLI_STAGE=$(mktemp -d "$PG_STAGE/cli.XXXXXX")
    tar -xjf "$CLI_PKG" -C "$CLI_STAGE"
    if [[ ! -f "$CLI_STAGE/$CLI_NAME" ]]; then
        echo "ProjectGenerator CLI package is missing $CLI_NAME" >&2
        exit 1
    fi
    if [[ -f "$RESOURCES/app.asar" ]]; then
        CLI="$RESOURCES/app.asar.unpacked/app/$CLI_NAME"
    else
        CLI="$RESOURCES/app/app/$CLI_NAME"
    fi
    mkdir -p "$(dirname "$CLI")"
    cp "$CLI_STAGE/$CLI_NAME" "$CLI"
    rm -rf "$CLI_STAGE"
fi
if [[ -z "$CLI" ]]; then
    echo "ProjectGenerator package for $PLATFORM is missing the GUI or bundled CLI" >&2
    exit 1
fi
cp "$CLI" "$CLI_DEST"
chmod +x "$GUI_BIN" "$CLI_DEST" "$CLI"
rm -rf "${OUTDIR}/${OUTPUT}"
mv "$PG_STAGE" "${OUTDIR}/${OUTPUT}"
echo " openFrameworks download projectGenerator and install complete!"
