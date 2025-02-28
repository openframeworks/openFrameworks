#!/usr/bin/env bash
set -e
VER=master
PLATFORM=""
ARCH=""
OVERWRITE=1
LEGACY=0
SILENT_ARGS=""
NO_SSL=""
BLEEDING_EDGE=0
DL_VERSION=2.6.4
TAG=""

printHelp(){
cat << EOF
    Usage: download_libs.sh [OPTIONS]

    Options:

    -v, --version VERSION       OF version to download the libraries for. Defaults to master
    -p, --platform PLATFORM     Platorm among: android, emscritpen, ios, linux, linux64, linuxarmv6l, linuxarmv7l, msys2, osx, tvos, vs
                                If not specified tries to autodetect the platform.
    -a, --arch ARCH             Architecture:
                                    vs: 64
                                    msys2: 64
                                    android: armv7, arm64, and x86 (if not specified will download all)
                                    linux: 64gcc6, armv6l or armv7l
    -n, --no-overwrite          Merge new libraries with existing ones, use only to download same version for different platforms
                                If not set deletes any existing libraries
    -s, --silent                Silent download progress
    -h, --help                  Shows this message
    -k, --no-ssl                Allow no SSL validation
    -t, --tag                   tag release for libraries
EOF
}

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
if [[ ! -d "$SCRIPT_DIR" ]]; then SCRIPT_DIR="$PWD"; fi
. "$SCRIPT_DIR/downloader.sh"

download(){
    echo ' -----'
    #echo " Downloading $1"
    # downloader ci.openframeworks.cc/libs/$1 $SILENT_ARGS

    COMMAND=" "

    if [[ $BLEEDING_EDGE = 1 ]] ; then
        REPO="latest"
    else
        REPO="nightly"
    fi

    #FIXME: remove later, now forcing "latest"
    REPO="latest"


    if [[ $TAG != "" ]] ; then
        REPO="$TAG"
    fi

    for PKG in $1; do
        COMMAND+="https://github.com/openframeworks/apothecary/releases/download/$REPO/$PKG "
    done
    # echo $COMMAND;
    downloader $COMMAND $SILENT_ARGS $NO_SSL
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
        SILENT_ARGS=1
        ;;
        -k|--no-ssl)
        NO_SSL=1
        ;;
        -m|--msystem)
        MSYSTEM="$2"
        shift # past argument
        ;;
        -t|--tag)
        TAG="$2"
        shift # past argument
        ;;
        -l|--legacy)
        LEGACY=1
        ;;
        -h|--help)
        printHelp
        exit 0
        ;;
        *)
        echo "Error: invalid argument: $key"
        printHelp
        # exit 1
        ;;
    esac
    shift # past argument or value
done

if [[ "$TARGET" != "" ]] && [[ "$PLATFORM" == "" ]]; then
    PLATFORM=$TARGET
fi

if [ "$PLATFORM" == "" ]; then
    OS=$(uname)
    if [ "$OS" == "Linux" ]; then
        PLATFORM="linux"
    elif [ "$OS" == "Darwin" ]; then
        PLATFORM="osx"
    elif [ "${OS:0:5}" == "MINGW" ]; then
        PLATFORM="msys2"
    else
        # otherwise we are on windows and will download 32bit msys2
        PLATFORM="msys2"
    fi
fi

if [ "$ARCH" == "" ]; then
    if [ "$PLATFORM" == "linux" ]; then
        ARCH=$(uname -m)
        if [ "$ARCH" == "x86_64" ]; then
            if command -v gcc &> /dev/null
            then
                GCC_VERSION=$(gcc -dumpversion | cut -f1 -d.)
            else
                GCC_VERSION=6
            fi
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
        if [ "$MSYSTEM" == "MINGW64" ] || [ "$MSYSTEM" == "mingw64" ]; then
            ARCH=mingw64
        elif [ "$MSYSTEM" == "CLANGARM64" ] || [ "$MSYSTEM" == "clangarm64" ]; then
            ARCH=clangarm64
        elif [ "$MSYSTEM" == "UCRT64" ]; then
            ARCH=ucrt64
        elif [ "$MSYSTEM" == "CLANG64" ]; then
            ARCH=clang64
        else
            ARCH=clang64
        fi
    fi

    if [ "$PLATFORM" == "osx" ]; then
        ARCH=x86_64
    fi
fi


SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cd "$SCRIPT_DIR"

if [[ $BLEEDING_EDGE = 1 ]] ; then
    VER=latest
else
    VER=master
fi

if [[ $TAG != "" ]] && [[ $TAG != "nightly" ]] ; then
    VER="$TAG"
fi

if [ "$PLATFORM" == "linux" ] && [ "$ARCH" == "64" ]; then
    if [[ $BLEEDING_EDGE = 1 ]] ; then
        ARCH=64_gcc6
    else
        ARCH=64gcc6
    fi
fi

echo " openFrameworks download_libs.sh v$DL_VERSION args=$@"

if [ "$PLATFORM" == "emscripten" ]; then
    if [[ $BLEEDING_EDGE = 1 ]] ; then
        if [[ $ARCH = "" ]] ; then
            ARCH="32"
        fi
    fi
fi

if [ "$PLATFORM" == "msys2" ]; then
    if [[ $BLEEDING_EDGE = 1 ]] ; then
        PKGS="openFrameworksLibs_${VER}_${PLATFORM}_${ARCH}.zip"
    else    
        PKGS="openFrameworksLibs_${VER}_${PLATFORM}_${ARCH}.zip"
    fi
elif [ "$ARCH" == "" ] && [ "$PLATFORM" == "vs" ]; then
    if [[ $BLEEDING_EDGE = 1 ]]; then
        if [[ $LEGACY == 1 ]]; then
            PKGS="openFrameworksLibs_${VER}_${PLATFORM}_2019_64_1.zip \
                  openFrameworksLibs_${VER}_${PLATFORM}_2019_64_2.zip"
        else
            PKGS="openFrameworksLibs_${VER}_${PLATFORM}_64_1.zip \
                  openFrameworksLibs_${VER}_${PLATFORM}_64_2.zip \
                  openFrameworksLibs_${VER}_${PLATFORM}_arm64_1.zip \
                  openFrameworksLibs_${VER}_${PLATFORM}_arm64_2.zip \
                  openFrameworksLibs_${VER}_${PLATFORM}_arm64ec_1.zip \
                  openFrameworksLibs_${VER}_${PLATFORM}_arm64ec_2.zip"
        fi
    else
        PKGS="openFrameworksLibs_${VER}_${PLATFORM}_64_1.zip \
          openFrameworksLibs_${VER}_${PLATFORM}_64_2.zip \
          openFrameworksLibs_${VER}_${PLATFORM}_64_3.zip \
          openFrameworksLibs_${VER}_${PLATFORM}_64_4.zip"
      fi
elif [ "$PLATFORM" == "vs" ]; then
    if [[ $BLEEDING_EDGE = 1 ]] ; then
        if [[ $LEGACY == 1 ]]; then
            PKGS="openFrameworksLibs_${VER}_${PLATFORM}_2019_64_1.zip \
                  openFrameworksLibs_${VER}_${PLATFORM}_2019_64_2.zip"
        else
            PKGS="openFrameworksLibs_${VER}_${PLATFORM}_${ARCH}_1.zip \
                  openFrameworksLibs_${VER}_${PLATFORM}_${ARCH}_2.zip"
        fi
    else       
        PKGS="openFrameworksLibs_${VER}_${PLATFORM}_${ARCH}_1.zip \
              openFrameworksLibs_${VER}_${PLATFORM}_${ARCH}_2.zip \
              openFrameworksLibs_${VER}_${PLATFORM}_${ARCH}_3.zip \
              openFrameworksLibs_${VER}_${PLATFORM}_${ARCH}_4.zip"
    fi
elif [[ "$PLATFORM" =~ ^(osx|ios|tvos|xros|catos|watchos|macos)$ ]]; then
    if [[ $BLEEDING_EDGE = 1 ]] ; then
        PKGS="openFrameworksLibs_${VER}_${PLATFORM}_1.tar.bz2 \
              openFrameworksLibs_${VER}_${PLATFORM}_2.tar.bz2 \
              openFrameworksLibs_${VER}_${PLATFORM}_3.tar.bz2"
    else    
        PKGS="openFrameworksLibs_${VER}_${PLATFORM}1.tar.bz2 \
              openFrameworksLibs_${VER}_${PLATFORM}2.tar.bz2 \
              openFrameworksLibs_${VER}_${PLATFORM}3.tar.bz2 \
              openFrameworksLibs_${VER}_${PLATFORM}4.tar.bz2" 
    fi 
elif [ "$ARCH" == "" ] && [ "$PLATFORM" == "android" ]; then
    if [[ $BLEEDING_EDGE = 1 ]] ; then
        PKGS="openFrameworksLibs_${VER}_${PLATFORM}_armv7.tar.bz2 \
              openFrameworksLibs_${VER}_${PLATFORM}_arm64.tar.bz2 \
              openFrameworksLibs_${VER}_${PLATFORM}_x86_64.tar.bz2
              openFrameworksLibs_${VER}_${PLATFORM}_x86.tar.bz2"
    else
        PKGS="openFrameworksLibs_${VER}_${PLATFORM}armv7.tar.bz2 \
          openFrameworksLibs_${VER}_${PLATFORM}arm64.tar.bz2 \
          openFrameworksLibs_${VER}_${PLATFORM}x86.tar.bz2"
    fi
elif [ "$PLATFORM" == "emscripten" ]; then
    if [[ $BLEEDING_EDGE = 1 ]] ; then
        PKGS="openFrameworksLibs_${VER}_${PLATFORM}_${ARCH}.tar.bz2"
    else
        PKGS="openFrameworksLibs_${VER}_${PLATFORM}${ARCH}.tar.bz2"
    fi
else # Linux
    if [[ $BLEEDING_EDGE = 1 ]] ; then
        PKGS="openFrameworksLibs_${VER}_${PLATFORM}${ARCH}.tar.bz2"
    else
        PKGS="openFrameworksLibs_${VER}_${PLATFORM}${ARCH}.tar.bz2"
    fi
fi

cd ../../
mkdir -p libs
cd libs

mkdir -p download
cd download

# IFS=' ' read -r -a PKGS_DATA <<< "$PKGS"
# if [ $OVERWRITE -eq 1 ]; then
#     for ((i = 0; i < ${#PKGS_DATA[@]}; i++)); do
#         FILE_CHECK="${PKGS_DATA[$i]}"
#         # Check if the file exists
#         if [ -e "${FILE_CHECK}" ]; then
#             echo " Removing Prior Download:[${FILE_CHECK}]"
#             # Remove the file or directory
#             rm -rf "${FILE_CHECK}"
#         fi
#     done
# fi

download "${PKGS[@]}"

cd ../ # back to libs
libs=("boost" "cairo" "curl" "FreeImage" "brotli" "fmod" "freetype" "glew" "glfw" "json" "libpng" "openssl" "pixman" "poco" "rtAudio" "tess2" "uriparser" "utf8" "videoInput" "zlib" "opencv" "ippicv" "assimp" "libxml2" "svgtiny" "fmt")
if [ $OVERWRITE -eq 1 ]; then
    echo " "
    echo " Overwrite - Removing prior libraries for [$PLATFORM]"
    for ((i=0;i<${#libs[@]};++i)); do
        if [ -e "${libs[i]}/lib/$PLATFORM" ]; then
            echo "  Removing: [${libs[i]}/lib/$PLATFORM]"
            rm -rf "${libs[i]}/lib/$PLATFORM"
        fi
        if [ "$PLATFORM" == "msys2" ] || [ "$PLATFORM" == "vs" ]; then
            if [ -e "${libs[i]}/bin" ]; then
                echo "  Removing: [${libs[i]}/bin]"
                rm -rf "${libs[i]}/bin"
            fi
        fi
        if [ -e "${libs[i]}/include" ]; then
            echo "  Removing: [${libs[i]}/include]"
            rm -rf "${libs[i]}/include"
        fi

    done
fi

if [ "$PLATFORM" == "osx" ]; then
    echo " "
    echo " Overwrite - Removing prior libraries for [$PLATFORM]"
    for ((i=0;i<${#libs[@]};++i)); do
        xcframework_path="${libs[i]}/lib/macos/${libs[i]}.xcframework/macos-arm64_x86_64"
        if [ $OVERWRITE -eq 1 ]; then
            if [ -e "$xcframework_path" ]; then
                echo "  Removing: [$xcframework_path]"
                rm -rf "$xcframework_path"
            fi
        fi
        info_plist_path="${libs[i]}/lib/macos/${libs[i]}.xcframework/Info.plist"
        if [ -e "$info_plist_path" ]; then
            #echo "  Backing up: [${info_plist_path}] to [${info_plist_path}.bak]"
            cp "$info_plist_path" "${info_plist_path}.bak"
        fi
    done
fi

echo " ------ "
for PKG in $PKGS; do
    echo " Uncompressing libraries [${PLATFORM}] from [$PKG]"
    if [ "$PLATFORM" == "msys2" ] || [ "$PLATFORM" == "vs" ]; then
        unzip -qo download/$PKG
        # rm -r download/$PKG
    else

        # FIXME: this if can be removed after this is fixed properly on apothecary, see:
        # https://github.com/openframeworks/openFrameworks/issues/8206

        if [ "$PLATFORM" == "linux" ] && { [ "$ARCH" == "aarch64" ] || [ "$ARCH" == "armv7l" ] || [ "$ARCH" == "armv6l" ]; }; then
            echo "tar xjfv download/$PKG  --strip-components=1"
            tar xjf download/$PKG --strip-components=1
        else
            tar xjfv download/$PKG
        fi
        # rm -r download/$PKG
    fi
    echo " Deployed libraries from [download/$PKG] to [/libs]"
done

if [ "$PLATFORM" == "osx" ]; then
    echo " "
    for ((i=0;i<${#libs[@]};++i)); do
        info_plist_path="${libs[i]}/lib/macos/${libs[i]}.xcframework/Info.plist"
        if [ -e "${info_plist_path}.bak" ]; then
            #echo "  Restoring: [${info_plist_path}.bak] to [${info_plist_path}]"
            mv "${info_plist_path}.bak" "$info_plist_path"
        fi
    done
fi

if [[ $BLEEDING_EDGE = 1 ]] ; then
    if [ "$PLATFORM" == "osx" ]; then
        addonslibs=("opencv" "ippicv" "libusb" "assimp" "libxml2" "svgtiny" "poco")
        addons=("ofxOpenCv" "ofxOpenCv" "ofxKinect" "ofxAssimpModelLoader" "ofxSvg" "ofxSvg" "ofxPoco" )
    elif [ "$PLATFORM" == "vs" ]; then
        addonslibs=("opencv" "ippicv" "libusb" "assimp" "libxml2" "svgtiny" "poco")
        addons=("ofxOpenCv" "ofxOpenCv" "ofxKinect" "ofxAssimpModelLoader" "ofxSvg" "ofxSvg" "ofxPoco")
    elif [ "$PLATFORM" == "ios" ] || [ "$PLATFORM" == "tvos" ]; then
        addonslibs=("opencv" "ippicv" "assimp" "libxml2" "svgtiny" "poco" )
        addons=("ofxOpenCv" "ofxOpenCv" "ofxAssimpModelLoader" "ofxSvg" "ofxSvg" "ofxPoco")
    else
        addonslibs=("opencv" "ippicv" "assimp" "libxml2" "svgtiny" "poco")
        addons=("ofxOpenCv" "ofxOpenCv" "ofxAssimpModelLoader" "ofxSvg" "ofxSvg" "ofxPoco")
    fi
else
    if [ "$PLATFORM" == "osx" ]; then
        addonslibs=("opencv" "ippicv" "libusb" "assimp" "libxml2" "svgtiny" "poco" "openssl")
        addons=("ofxOpenCv" "ofxOpenCv" "ofxKinect" "ofxAssimpModelLoader" "ofxSvg" "ofxSvg" "ofxPoco" "ofxPoco")
    elif [ "$PLATFORM" == "vs" ]; then
        addonslibs=("opencv" "ippicv" "libusb" "assimp" "libxml2" "svgtiny" "poco")
        addons=("ofxOpenCv" "ofxOpenCv" "ofxKinect" "ofxAssimpModelLoader" "ofxSvg" "ofxSvg" "ofxPoco")
    elif [ "$PLATFORM" == "ios" ] || [ "$PLATFORM" == "tvos" ]; then
        addonslibs=("opencv" "ippicv" "assimp" "libxml2" "svgtiny" "poco" )
        addons=("ofxOpenCv" "ofxOpenCv" "ofxAssimpModelLoader" "ofxSvg" "ofxSvg" "ofxPoco")
    else
        addonslibs=("opencv" "ippicv" "assimp" "libxml2" "svgtiny" "poco")
        addons=("ofxOpenCv" "ofxOpenCv" "ofxAssimpModelLoader" "ofxSvg" "ofxSvg" "ofxPoco")
    fi
fi

echo "   ------ "
if [ "$PLATFORM" == "osx" ]; then
    if [ $OVERWRITE -eq 1 ]; then
        echo " Overwrite - addon xCFramework: [${addons[i]} - ${addonslibs[i]}]"
        xcframework_path="../addons/${addons[i]}/libs/${addonslibs[i]}/lib/macos/${addonslibs[i]}.xcframework/macos-arm64_x86_64"
        if [ -e "$xcframework_path" ]; then
            echo "  Removing: [$xcframework_path]"
            rm -rf "$xcframework_path"
        fi
    fi
    info_plist_path="../addons/${addons[i]}/libs/${addonslibs[i]}/lib/macos/${addonslibs[i]}.xcframework/Info.plist"
    if [ -e "$info_plist_path" ]; then
        cp "$info_plist_path" "${info_plist_path}.bak"
    fi
fi


if [ $OVERWRITE -eq 1 ]; then 
    for ((i=0;i<${#addonslibs[@]};++i)); do
        if [ -e ${addonslibs[i]} ] ; then

            echo " Overwrite - addon: [${addons[i]} - ${addonslibs[i]}]"
            if [ -e ../addons/${addons[i]}/libs/${addonslibs[i]}/lib/$PLATFORM ]; then
                echo "   Remove binaries: [${addons[i]}/libs/${addonslibs[i]}/lib/$PLATFORM]"
                rm -rf ../addons/${addons[i]}/libs/${addonslibs[i]}/lib/$PLATFORM
            fi
            if [ -e ../addons/${addons[i]}/libs/${addonslibs[i]}/bin ]; then
                echo "   Remove binaries: [${addons[i]}/libs/${addonslibs[i]}/bin]"
                rm -rf ../addons/${addons[i]}/libs/${addonslibs[i]}/bin
            fi
            if [ -e ../addons/${addons[i]}/libs/${addonslibs[i]}/include ]; then
                echo "   Remove include: [${addons[i]}/libs/include]"
                rm -rf ../addons/${addons[i]}/libs/${addonslibs[i]}/include
            fi
        fi
    done
    echo "   ------ "
fi

for ((i=0;i<${#addonslibs[@]};++i)); do
    if [ -e "${addonslibs[i]}" ]; then
        echo "   Deploying [${addonslibs[i]}] to [../addons/${addons[i]}/libs]"
        mkdir -p ../addons/${addons[i]}/libs/${addonslibs[i]}
        if ! command -v rsync &> /dev/null
        then      
            cp -a ${addonslibs[i]}/* ../addons/${addons[i]}/libs/${addonslibs[i]}    
        else
            rsync -azp ${addonslibs[i]}/ ../addons/${addons[i]}/libs/${addonslibs[i]}/
        fi
        rm -rf ${addonslibs[i]}
    fi
done

if [ "$PLATFORM" == "osx" ]; then
    echo " "
    for ((i=0;i<${#addonslibs[@]};++i)); do
        if [ -e ${addonslibs[i]} ] ; then
            info_plist_path="../addons/${addons[i]}/libs/${addonslibs[i]}/lib/macos/${addonslibs[i]}.xcframework/Info.plist"
            if [ -e "${info_plist_path}.bak" ]; then
                mv "${info_plist_path}.bak" "$info_plist_path"
            fi
        fi
    done
fi

echo " ------ "
echo " openFrameworks download_libs and install complete!"
