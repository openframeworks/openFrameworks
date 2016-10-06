#!/usr/bin/env bash
set -e
VER=$1
PLATFORM=$2
ARCH=$3
if [ $# -gt 4 ]; then
    OVERWRITE=$4
else
    OVERWRITE=1
fi

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

if [ "$PLATFORM" == "msys2" ] || [ "$PLATFORM" == "vs" ]; then
    PKG=openFrameworksLibs_${VER}_${PLATFORM}${ARCH}.zip
else
    PKG=openFrameworksLibs_${VER}_${PLATFORM}${ARCH}.tar.bz2
fi
cd $SCRIPT_DIR
echo "Downloading $PKG from $PWD"
wget ci.openframeworks.cc/libs/$PKG

cd ../../
mkdir -p libs
cd libs

if [ $OVERWRITE -eq 1 ]; then
    echo "Removing old libraries"
    libs=("boost" "cairo" "curl" "FreeImage" "freetype" "glew" "glfw" "json" "libpng" "openssl" "pixman" "poco" "rtAudio" "tess2" "uriparser" "utf8" "videoInput" "zlib" "README.md")
    for lib in $libs; do
        if [ -e $lib ]; then
            rm -rf $lib
        fi
    done
    echo "Uncompressing libraries $PLATFORM $ARCH"
    if [ "$PLATFORM" == "msys2" ] || [ "$PLATFORM" == "vs" ]; then
        unzip -qo ../scripts/dev/$PKG
    else
        tar xjf ../scripts/dev/$PKG
    fi
else
    echo "Uncompressing libraries $PLATFORM $ARCH"
    if [ "$PLATFORM" == "msys2" ] || [ "$PLATFORM" == "vs" ]; then
        unzip -qn ../scripts/dev/$PKG
    else
        tar xjkf ../scripts/dev/$PKG
    fi
fi


rm ../scripts/dev/$PKG
