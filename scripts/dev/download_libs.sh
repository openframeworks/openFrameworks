#!/usr/bin/env bash
set -e
VER=master
PLATFORM=""
ARCH=""
OVERWRITE=1
SILENT_ARGS=""

printHelp(){
cat << EOF
    Usage: download_libs.sh [OPTIONS]

    Options:

    -v, --version VERSION       OF version to download the libraries for. Defaults to master
    -p, --platform PLATFORM     Platorm among: android, emscritpen, ios, linux, linux64, linuxarmv6l, linuxarmv7l, msys2, osx, tvos, vs2015, vs2017
                                If not specified tries to autodetect the platform.
    -a, --arch ARCH             Architecture:
                                    vs2015: 32 or 64
                                    vs2017: 32 or 64
                                    msys2: 32
                                    android: armv7 or x86 (if not specified will download both)
                                    linux: 64gcc4, 64gcc5, 64gcc6 / 64, armv6l or armv7l
    -n, --no-overwrite          Merge new libraries with exisiting ones, use only to download same version for different platforms
                                If not set deletes any existing libraries
    -s, --silent                Silent download progress
    -h, --help                  Shows this message
EOF
}

download(){
    echo "Downloading $1"
    wget ci.openframeworks.cc/libs/$1 $SILENT_ARGS
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
    else
        # otherwise we are on windows and will download 32bit msys2
        PLATFORM="msys2"
        ARCH=32
    fi
fi

if [ "$ARCH" == "" ]; then
    if [ "$PLATFORM" == "linux" ]; then
        ARCH=$(uname -m)
        if [ "$ARCH" == "x86_64" ]; then
            GCC_VERSION=$(gcc -dumpversion | cut -f1 -d.)
            if [ $GCC_VERSION -eq 4 ]; then
                ARCH=64gcc4
            elif [ $GCC_VERSION -eq 5 ]; then
                ARCH=64gcc5
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
        ARCH=32
    fi
fi

if [ "$PLATFORM" == "linux" ] && [ "$ARCH" == "64" ]; then
    ARCH=64gcc6
fi

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cd $SCRIPT_DIR

if [ "$ARCH" == "" ] && [ "$PLATFORM" == "vs2015" ]; then
    PKGS="openFrameworksLibs_${VER}_${PLATFORM}_32_1.zip \
          openFrameworksLibs_${VER}_${PLATFORM}_32_2.zip \
          openFrameworksLibs_${VER}_${PLATFORM}_32_3.zip \
          openFrameworksLibs_${VER}_${PLATFORM}_32_4.zip \
          openFrameworksLibs_${VER}_${PLATFORM}_64_1.zip \
          openFrameworksLibs_${VER}_${PLATFORM}_64_2.zip \
          openFrameworksLibs_${VER}_${PLATFORM}_64_3.zip \
          openFrameworksLibs_${VER}_${PLATFORM}_64_4.zip"
elif [ "$ARCH" == "" ] && [ "$PLATFORM" == "vs2017" ]; then
    PKGS="openFrameworksLibs_${VER}_${PLATFORM}_32_1.zip \
          openFrameworksLibs_${VER}_${PLATFORM}_32_2.zip \
          openFrameworksLibs_${VER}_${PLATFORM}_32_3.zip \
          openFrameworksLibs_${VER}_${PLATFORM}_32_4.zip \
          openFrameworksLibs_${VER}_${PLATFORM}_64_1.zip \
          openFrameworksLibs_${VER}_${PLATFORM}_64_2.zip \
          openFrameworksLibs_${VER}_${PLATFORM}_64_3.zip \
          openFrameworksLibs_${VER}_${PLATFORM}_64_4.zip"
elif [ "$PLATFORM" == "msys2" ]; then
    PKGS="openFrameworksLibs_${VER}_${PLATFORM}_${ARCH}_.zip"
elif [ "$PLATFORM" == "vs2015" ] || [ "$PLATFORM" == "vs2017" ]; then
    PKGS="openFrameworksLibs_${VER}_${PLATFORM}_${ARCH}_1.zip \
          openFrameworksLibs_${VER}_${PLATFORM}_${ARCH}_2.zip \
          openFrameworksLibs_${VER}_${PLATFORM}_${ARCH}_3.zip \
          openFrameworksLibs_${VER}_${PLATFORM}_${ARCH}_4.zip"
elif [ "$ARCH" == "" ] && [[ "$PLATFORM" == "osx" || "$PLATFORM" == "ios" || "$PLATFORM" == "tvos" ]]; then
    PKGS="openFrameworksLibs_${VER}_${PLATFORM}1.tar.bz2 \
          openFrameworksLibs_${VER}_${PLATFORM}2.tar.bz2 \
          openFrameworksLibs_${VER}_${PLATFORM}3.tar.bz2 \
          openFrameworksLibs_${VER}_${PLATFORM}4.tar.bz2"
elif [ "$ARCH" == "" ] && [ "$PLATFORM" == "android" ]; then
    PKGS="openFrameworksLibs_${VER}_${PLATFORM}armv7.tar.bz2 \
          openFrameworksLibs_${VER}_${PLATFORM}x86.tar.bz2"
else # Linux
    PKGS="openFrameworksLibs_${VER}_${PLATFORM}${ARCH}.tar.bz2"
fi

for PKG in $PKGS; do
    download $PKG
done

cd ../../
mkdir -p libs
cd libs

if [ $OVERWRITE -eq 1 ]; then
    echo "Removing old libraries"
    libs=("boost" "cairo" "curl" "FreeImage" "freetype" "glew" "glfw" "json" "libpng" "openssl" "pixman" "poco" "rtAudio" "tess2" "uriparser" "utf8" "videoInput" "zlib" "opencv" "ippicv" "assimp" "libxml2" "svgtiny" "README.md")
    for lib in $libs; do
        if [ -e $lib ]; then
            rm -rf $lib
        fi
    done
fi

for PKG in $PKGS; do
    echo "Uncompressing libraries ${PLATFORM}${ARCH} from $PKG"
    if [ "$PLATFORM" == "msys2" ] || [ "$PLATFORM" == "vs2015" ] || [ "$PLATFORM" == "vs2017" ]; then
        unzip -qo ../scripts/dev/$PKG
        rm ../scripts/dev/$PKG
    else
        tar xjf ../scripts/dev/$PKG
        rm ../scripts/dev/$PKG
    fi
done

if [ "$PLATFORM" == "osx" ]; then
    addonslibs=("opencv" "ippicv" "libusb" "assimp" "libxml2" "svgtiny" "poco" "openssl")
    addons=("ofxOpenCv" "ofxOpenCv" "ofxKinect" "ofxAssimpModelLoader" "ofxSvg" "ofxSvg" "ofxPoco" "ofxPoco")
elif [ "$PLATFORM" == "vs2015" ] || [ "$PLATFORM" == "vs2017" ]; then
    addonslibs=("opencv" "ippicv" "libusb" "assimp" "libxml2" "svgtiny" "poco")
    addons=("ofxOpenCv" "ofxOpenCv" "ofxKinect" "ofxAssimpModelLoader" "ofxSvg" "ofxSvg" "ofxPoco")
elif [ "$PLATFORM" == "ios" ] || [ "$PLATFORM" == "tvos" ]; then
    addonslibs=("opencv" "ippicv" "assimp" "libxml2" "svgtiny" "poco" "openssl")
    addons=("ofxOpenCv" "ofxOpenCv" "ofxAssimpModelLoader" "ofxSvg" "ofxSvg" "ofxPoco" "ofxPoco")
else
    addonslibs=("opencv" "ippicv" "assimp" "libxml2" "svgtiny" "poco")
    addons=("ofxOpenCv" "ofxOpenCv" "ofxAssimpModelLoader" "ofxSvg" "ofxSvg" "ofxPoco")
fi

for ((i=0;i<${#addonslibs[@]};++i)); do
    if [ -e ${addonslibs[i]} ]; then
        echo "Copying ${addonslibs[i]} to ${addons[i]}"
        if [ $OVERWRITE -eq 1 ] && [ -e ../addons/${addons[i]}/libs/${addonslibs[i]} ]; then
            echo "Removing old opencv libraries"
            rm -rf ../addons/${addons[i]}/libs/${addonslibs[i]}
        fi
        mkdir -p ../addons/${addons[i]}/libs/${addonslibs[i]}
        rsync -a ${addonslibs[i]}/ ../addons/${addons[i]}/libs/${addonslibs[i]}
        rm -rf ${addonslibs[i]}
    fi
done
