#!/usr/bin/env bash
set -e
VER=master
PLATFORM=""
ARCH=""
OVERWRITE=0

while [[ $# -gt 1 ]]; do
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
        -o|--overwrite)
        OVERWRITE=1
        ;;
        *)
                # unknown option
        ;;
    esac
    shift # past argument or value
done



#VER=$1
#PLATFORM=$2
#ARCH=$3
#if [ $# -eq 4 ]; then
#    OVERWRITE=$4
#else
#    OVERWRITE=1
#fi
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
fi

echo "Uncompressing libraries $PLATFORM $ARCH"
if [ "$PLATFORM" == "msys2" ] || [ "$PLATFORM" == "vs" ]; then
    unzip -qo ../scripts/dev/$PKG
else
    tar xjf ../scripts/dev/$PKG
fi

rm ../scripts/dev/$PKG
