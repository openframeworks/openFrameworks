#!/usr/bin/env bash
# $1 -> platform: msys2, linux, linux64, vs, osx, ios, all
# $2 -> version number: 006
#
# This script removes folders clones the openFrameworks repo
# and deletes parts of it to create the final package.
# Do not try to modify it to run over your local install of
# openFrameworks or it'll remove it along with any projects it
# might contain.

platform=$1
version=$2

all_platforms="linux linux64 linuxarmv6l linuxarmv7l linuxaarch64 osx msys2 vs ios macos tvos android emscripten"

of_root=$(readlink -f "$(dirname "$(readlink -f "$0")")/../..")

if [ $# -ge 3 ]; then
    branch=$3
else
    branch=stable
fi

if [ $# -ge 4 ]; then
    LIBS_ABI=$4
else
    LIBS_ABI=""
fi

REPO=https://github.com/openframeworks/openFrameworks.git
REPO_ALIAS=originlocal
BRANCH=$branch

PG_REPO=https://github.com/openframeworks/projectGenerator.git
PG_REPO_ALIAS=originhttps
PG_BRANCH=master

hostArch=`uname`

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

if [[ ! -d "$SCRIPT_DIR" ]]; then SCRIPT_DIR="$PWD"; fi
. "$SCRIPT_DIR/downloader.sh"

isRunning(){
    if [ “$hostArch” == “Linux” ]; then
        if [ -d /proc/$1 ]; then
            return 0
        else
            return 1
        fi
    else
        number=$(ps aux | sed -E "s/[^ ]* +([^ ]*).*/\1/g" | grep ^$1$ | wc -l)

        if [ $number -gt 0 ]; then
            return 0;
        else
            return 1;
        fi
    fi
}

echoDots(){
    sleep 0.1 # Waiting for a brief period first, allowing jobs returning immediatly to finish
    while isRunning $1; do
        for i in $(seq 1 10); do
            echo -ne .
            if ! isRunning $1; then
                printf "\r"
                return;
            fi
            sleep 1
        done
        printf "\r                    "
        printf "\r"
    done
}


if [ "$platform" != "msys2" ] && [ "$platform" != "linux" ] && [ "$platform" != "linux64" ] && [ "$platform" != "linuxarmv6l" ] && [ "$platform" != "linuxaarch64" ] && [ "$platform" != "linuxarmv7l" ] && [ "$platform" != "vs" ] && [ "$platform" != "vs2019" ] && [ "$platform" != "osx" ] && [ "$platform" != "android" ] && [ "$platform" != "ios" ] && [ "$platform" != "macos" ]; then
    echo usage:
    echo ./create_package.sh platform version
    echo platform:
    echo msys2, linux, linux64, linuxarmv6l, linuxaarch64, linuxarmv7l, vs, vs2019, osx, android, ios, macos, all
    exit 1
fi

if [ "$version" == "" ]; then
    echo usage:
    echo ./create_package.sh platform version [branch]
    echo platform:
    echo msys2, linux, linux64, vs, osx, android, ios, macos, all
    echo
    echo branch:
    echo master, stable
    exit 1
fi


echo
echo
echo
echo --------------------------------------------------------------------------
echo "Creating package $version for $platform $LIBS_ABI"
echo --------------------------------------------------------------------------
echo

# This script removes folders clones the openFrameworks repo
# and deletes parts of it to create the final package.
# Do not try to modify it to run over your local install of
# openFrameworks or it'll remove it along with any projects it
# might contain.
# Instead we download it in a folder with a different name so it's
# safe to delete it completely at the end
pkgfolder=openFrameworks_pkg_creation

rm -rf ${pkgfolder}
echo "Cloning OF from $REPO $BRANCH"
git clone $REPO --depth=1 --branch=$BRANCH ${pkgfolder} 2> /dev/null
gitfinishedok=$?
if [ $gitfinishedok -ne 0 ]; then
    echo "Error connecting to github"
    exit 1
fi

cd ${pkgfolder}
packageroot=$PWD
cd $packageroot

echo "Updating project generator to latest"
./scripts/dev/init_submodules.sh

#cd apps
#echo "Cloning project generator from $PG_REPO $PG_BRANCH"
#git clone $PG_REPO --depth=1 --branch=$PG_BRANCH 2> /dev/null
#gitfinishedok=$?
#if [ $gitfinishedok -ne 0 ]; then
#    echo "Error connecting to github"
#    exit 1
#fi

function deleteCodeblocks {
    #delete codeblock files
    rm *.cbp
    rm *.workspace
}

function deleteMakefiles {
    #delete makefiles
    rm Makefile
    rm *.make
}

function deleteVS {
    #delete VS files
    rm *.vcxproj
    rm *.vcxproj.user
    rm *.vcxproj.filters
    rm *.sln
}

function deleteXcode {
    #delete osx files
    rm -Rf *.xcodeproj
    rm openFrameworks-Info.plist
    rm Project.xcconfig
}

function deleteEclipse {
    #delete eclipse project
    rm $(find . -name .*project)
}


function createProjectFiles {
    if [ "$PKG_PLATFORM" != "linuxarmv6l" ] && [ "$PKG_PLATFORM" != "linuxarmv7l" ] && [ "$PKG_PLATFORM" != "linuxaarch64" ] && [ "$PKG_PLATFORM" != "android" ] ; then
        mkdir -p ${MAIN_OFROOT}/libs/openFrameworksCompiled/lib/linux64/
        cd ${MAIN_OFROOT}/apps/projectGenerator/commandLine
        echo "Recompiling command line PG"
        if [ -d ~/logs ]; then
            PROJECT_OPTIMIZATION_CFLAGS_RELEASE="-O0 -g0" CXXFLAGS=-ftrack-macro-expansion=0 make Release > ~/logs/compilePG.log 2>&1 &
            makePGPID=$!
            echoDots $makePGPID
            wait $makePGPID
        else
            PROJECT_OPTIMIZATION_CFLAGS_RELEASE="-O0 -g0" CXXFLAGS=-ftrack-macro-expansion=0 make Release
        fi

        cd ${PKG_OFROOT}
        echo "Creating project files for $PKG_PLATFORM"
        if [ "$PKG_PLATFORM" == "vs" ] || [ "$PKG_PLATFORM" == "vs2019" ] || [ "$PKG_PLATFORM" == "android" ] || [ "$PKG_PLATFORM" == "ios" ]; then
            PG_PLATFORM=$PKG_PLATFORM
        else
            PG_PLATFORM="$PKG_PLATFORM,vscode"
        fi


        if [ -f "${MAIN_OFROOT}/apps/projectGenerator/commandLine/bin/projectGenerator" ]; then
            echo "projectGenerator exists..."

            echo "##[group]PG building example projects"
            # ${MAIN_OFROOT}/apps/projectGenerator/commandLine/bin/projectGenerator --recursive -p${PG_PLATFORM} -o$PKG_OFROOT $PKG_OFROOT/examples > /dev/null
            ${MAIN_OFROOT}/apps/projectGenerator/commandLine/bin/projectGenerator --recursive -p${PG_PLATFORM} -o$PKG_OFROOT $PKG_OFROOT/examples
            echo "##[endgroup]"

        else
            echo "projectGenerator does not exist. Continue."
            ls ${MAIN_OFROOT}/apps/projectGenerator/commandLine/bin/
            exit 1
        fi


        #fix config.make because the project generator is putting in the full path to the OF_ROOT as it is designed to do.
        #in this case we actually don't want to set it as the default of ../../../ is fine.
        find $PKG_OFROOT/examples -name "config.make" -type f -exec sed -i 's/^OF_ROOT =.*/# OF_ROOT = ..\/..\/..\//' {} \;
    elif [ "$PKG_PLATFORM" == "linuxarmv6l" ] || [ "$PKG_PLATFORM" == "linuxarmv7l" ] || [ "$PKG_PLATFORM" == "linuxaarch64" ]; then
        for example_group in $PKG_OFROOT/examples/*; do
            for example in $example_group/*; do
                if [ -d $example ]; then
                    cp $PKG_OFROOT/scripts/templates/linux/Makefile $example/
                    cp $PKG_OFROOT/scripts/templates/linux/config.make $example/
                fi
            done
        done
    fi
    cd ${PKG_OFROOT}
}


remove_current_platform() {
    local platforms="$1"
    local platform_to_remove="$2"
    echo "$platforms" | sed "s/\b$platform_to_remove\b//g"
}

function createPackage {
    PKG_PLATFORM=$1
    PKG_VERSION=$2
    PKG_OFROOT=$3
    MAIN_OFROOT=$4

    #remove previously created package
    cd $PKG_OFROOT/..
    rm -Rf of_v${PKG_VERSION}_${PKG_PLATFORM}${LIBS_ABI}.*
    rm -Rf of_v${PKG_VERSION}_${PKG_PLATFORM}${LIBS_ABI}_*
    echo "Creating package $PKG_PLATFORM $version in $PKG_OFROOT"

    #remove devApps folder
    rm -r $PKG_OFROOT/apps/devApps

    cd $PKG_OFROOT/examples

    #delete ios examples in other platforms
    if [ "$PKG_PLATFORM" != "ios" ]; then
        rm -Rf ios
        rm -Rf tvOS
    fi

    #delete android examples in other platforms
    if [ "$PKG_PLATFORM" != "android" ]; then
        rm -Rf android
    fi


    if [ "$PKG_PLATFORM" != "linuxarmv6l" ] && [ "$PKG_PLATFORM" != "linuxarmv7l" ] && [ "$PKG_PLATFORM" != "linuxaarch64" ]; then
        rm -Rf events/rpiTouchExample
    fi

    #delete desktop examples in mobile packages
    if [ "$PKG_PLATFORM" == "android" ] || [ "$PKG_PLATFORM" == "ios" ]; then
        rm -Rf computer_vision/kinectExample
        rm -Rf gl/glInfoExample
        rm -Rf gl/alphaMaskingShaderExample
        rm -Rf gl/billboardExample
        rm -Rf gl/billboardRotationExample
        rm -Rf gl/multiLightExample
        rm -Rf gl/multiTextureShaderExample
        rm -Rf gl/pointsAsTextures
        rm -Rf gl/gpuParticleSystemExample
        rm -Rf gl/vboMeshDrawInstancedExample
        rm -Rf gl/shaderExample
        rm -Rf gl/computeShaderParticlesExample
        rm -Rf gl/computeShaderTextureExample
        rm -Rf gl/pixelBufferExample
        rm -Rf gl/textureBufferInstancedExample
        rm -Rf gl/threadedPixelBufferExample
        rm -Rf gl/transformFeedbackExample
        rm -Rf utils/dragDropExample
        rm -Rf utils/fileOpenSaveDialogExample
        rm -Rf utils/systemSpeakExample
        rm -Rf utils/fileBufferLoadingCSVExample
        rm -Rf 3d/modelNoiseExample
        rm -Rf windowing
        rm -Rf input_output
        rm -Rf shader
        rm -Rf sound
        rm -Rf threads
        rm -Rf windowing
    fi

    if [ "$PKG_PLATFORM" == "linux" ] || [ "$PKG_PLATFORM" == "linux64" ]; then
        rm -Rf gles
    fi

    if [ "$PKG_PLATFORM" == "linuxarmv6l" ] || [ "$PKG_PLATFORM" == "linuxarmv7l" ] || [ "$PKG_PLATFORM" == "linuxaarch64" ]; then

    rm -Rf gl/glInfoExample
        rm -Rf gl/alphaMaskingShaderExample
        rm -Rf gl/billboardExample
        rm -Rf gl/billboardRotationExample
        rm -Rf gl/multiLightExample
        rm -Rf gl/multiTextureShaderExample
        rm -Rf gl/pointsAsTextures
        rm -Rf gl/gpuParticleSystemExample
        rm -Rf gl/vboMeshDrawInstancedExample
        rm -Rf gl/shaderExample
        rm -Rf gl/computeShaderParticlesExample
        rm -Rf gl/computeShaderTextureExample
        rm -Rf gl/pixelBufferExample
        rm -Rf gl/textureBufferInstancedExample
        rm -Rf gl/threadedPixelBufferExample
        rm -Rf gl/transformFeedbackExample
        rm -Rf utils/dragDropExample
        rm -Rf utils/fileOpenSaveDialogExample
        rm -Rf utils/systemSpeakExample
        rm -Rf utils/fileBufferLoadingCSVExample
        rm -Rf 3d/modelNoiseExample
        rm -Rf windowing
    fi

    if [ "$PKG_PLATFORM" == "msys2" ] || [ "$PKG_PLATFORM" == "vs2019" ] || [ "$PKG_PLATFORM" == "vs" ]; then
        rm -Rf video/osxHighPerformanceVideoPlayerExample
        rm -Rf video/osxVideoRecorderExample
        rm -Rf gles
    fi

    if [ "$PKG_PLATFORM" == "osx" ] || [ "$PKG_PLATFORM" == "macos" ]; then

        rm -Rf gl/computeShaderParticlesExample
        rm -Rf gl/computeShaderTextureExample
    fi

    #delete tutorials by now
    rm -Rf $PKG_OFROOT/tutorials

    RELEASE="${RELEASE:-latest}"

    cd $PKG_OFROOT/
    echo " Location: {$PKG_OFROOT}"
    if [ "$PKG_PLATFORM" = "osx" ]; then
        scripts/osx/download_libs.sh -t $RELEASE
        scripts/emscripten/download_libs.sh -n -t $RELEASE
    elif [ "$PKG_PLATFORM" = "linux64" ]; then
        scripts/linux/download_libs.sh -a 64$LIBS_ABI
        scripts/emscripten/download_libs.sh -n -t $RELEASE
    elif [ "$PKG_PLATFORM" = "linuxarmv6l" ]; then
        scripts/linux/download_libs.sh -a armv6l -t $RELEASE
    elif [ "$PKG_PLATFORM" = "linuxarmv7l" ]; then
        scripts/linux/download_libs.sh -a armv7l -t $RELEASE
    elif [ "$PKG_PLATFORM" = "linuxaarch64" ]; then
        scripts/linux/download_libs.sh -a aarch64 -t $RELEASE
    elif [ "$PKG_PLATFORM" = "msys2" ]; then
        scripts/msys2/download_libs.sh -a $LIBS_ABI -t $RELEASE
        scripts/emscripten/download_libs.sh -n -t $RELEASE
    elif [ "$PKG_PLATFORM" = "vs2019" ]; then
        scripts/vs/download_libs.sh -a $LIBS_ABI -t $RELEASE
        scripts/emscripten/download_libs.sh -n -t $RELEASE
    elif [ "$PKG_PLATFORM" = "vs" ]; then
        if [ "$LIBS_ABI" = "" ]; then
            scripts/vs/download_libs_2019_x64.sh -t $RELEASE
        else
            scripts/vs/download_libs.sh -a $LIBS_ABI -t $RELEASE
        fi
        scripts/emscripten/download_libs.sh -n -t $RELEASE
    elif [ "$PKG_PLATFORM" = "android" ]; then
        scripts/android/download_libs.sh -t $RELEASE
    elif [ "$PKG_PLATFORM" = "ios" ]; then
        scripts/macos/download_libs.sh -t $RELEASE
     elif [ "$PKG_PLATFORM" = "emscripten" ]; then
       scripts/emscripten/download_libs.sh -n -t $RELEASE
    elif [ "$PKG_PLATFORM" = "macos" ]; then
        scripts/osx/download_libs.sh -t $RELEASE
        scripts/macos/download_libs.sh -t $RELEASE
        scripts/emscripten/download_libs.sh -n -t $RELEASE
    fi

    createProjectFiles $PKG_PLATFORM $PKG_OFROOT

    if [ "$PKG_PLATFORM" = "linux" ]; then
        otherplatforms=$(remove_current_platform "$all_platforms" "linux")
    elif [ "$PKG_PLATFORM" = "linux64" ]; then
        otherplatforms=$(remove_current_platform "$all_platforms" "emscripten")
        otherplatforms=$(remove_current_platform "$otherplatforms" "linux64")
    elif [ "$PKG_PLATFORM" = "linuxarmv6l" ]; then
        otherplatforms=$(remove_current_platform "$all_platforms" "linuxarmv6l")
    elif [ "$PKG_PLATFORM" = "linuxarmv7l" ]; then
        otherplatforms=$(remove_current_platform "$all_platforms" "linuxarmv7l")
    elif [ "$PKG_PLATFORM" = "linuxaarch64" ]; then
        otherplatforms=$(remove_current_platform "$all_platforms" "linuxaarch64")
    elif [ "$PKG_PLATFORM" = "osx" ]; then
        otherplatforms=$(remove_current_platform "$all_platforms" "osx")
        otherplatforms=$(remove_current_platform "$otherplatforms" "emscripten")
    elif [ "$PKG_PLATFORM" = "msys2" ]; then
        otherplatforms=$(remove_current_platform "$all_platforms" "msys2")
    elif [ "$PKG_PLATFORM" = "vs" ] || [ "$PKG_PLATFORM" = "vs2019" ]; then
        otherplatforms=$(remove_current_platform "$all_platforms" "vs")
        otherplatforms=$(remove_current_platform "$otherplatforms" "emscripten")
    elif [ "$PKG_PLATFORM" = "ios" ]; then
        otherplatforms=$(remove_current_platform "$all_platforms" "ios")
        otherplatforms=$(remove_current_platform "$otherplatforms" "osx")
        otherplatforms=$(remove_current_platform "$otherplatforms" "tvos")
        otherplatforms=$(remove_current_platform "$otherplatforms" "macos")
    elif [ "$PKG_PLATFORM" = "macos" ]; then
        otherplatforms=$(remove_current_platform "$all_platforms" "macos")
        otherplatforms=$(remove_current_platform "$otherplatforms" "ios")
        otherplatforms=$(remove_current_platform "$otherplatforms" "osx")
        otherplatforms=$(remove_current_platform "$otherplatforms" "tvos")
        otherplatforms=$(remove_current_platform "$otherplatforms" "emscripten")
    elif [ "$PKG_PLATFORM" = "emscripten" ]; then
        otherplatforms=$(remove_current_platform "$all_platforms" "emscripten")
    elif [ "$PKG_PLATFORM" = "android" ]; then
        otherplatforms=$(remove_current_platform "$all_platforms" "android")
    else
        echo "Unknown platform: $PKG_PLATFORM"
        exit 1
    fi

    #download and uncompress PG
    echo "Creating projectGenerator"
    mkdir -p $HOME/.tmp
    export TMPDIR=$HOME/.tmp

    # FIXME: Temporary fix for latest projectGenerator
    # there is no "latest" release so we use nightly. feel free to remove this when PG/Apothecary releases are in sync
    if [ "$RELEASE" = "latest" ]; then
        RELEASE="nightly"
    fi


    if [ "$PKG_PLATFORM" = "vs" ] || [ "$PKG_PLATFORM" = "vs2019" ] || [ "$PKG_PLATFORM" = "msys2" ]; then
        downloader https://github.com/openframeworks/projectGenerator/releases/download/$RELEASE/projectGenerator-vs-gui.zip 2> /dev/null
        mkdir -p projectGenerator
        unzip -q projectGenerator-vs-gui.zip -d "projectGenerator" 2> /dev/null
        rm projectGenerator-vs-gui.zip
        rm -rf apps/projectGenerator
    fi

    if [ "$PKG_PLATFORM" = "osx" ] || [ "$PKG_PLATFORM" = "ios" ] || [ "$PKG_PLATFORM" = "macos" ]; then
        downloader https://github.com/openframeworks/projectGenerator/releases/download/$RELEASE/projectGenerator-osx.zip 2> /dev/null
        unzip -q projectGenerator-osx.zip
        mv projectGenerator-osx/ projectGenerator
        rm projectGenerator-osx.zip
        rm -rf apps/projectGenerator
    fi

    if [ "$PKG_PLATFORM" = "linux64" ]; then
        downloader https://github.com/openframeworks/projectGenerator/releases/download/nightly/projectGenerator-linux-gui.gz 2> /dev/null
        tar -xzvf projectGenerator-linux-gui.gz
        mv $(find . -maxdepth 1 -type d -name "projectGenerator-*") projectGenerator
        rm -rf apps/projectGenerator
        rm projectGenerator-linux-gui.gz
    fi

    if [ "$PKG_PLATFORM" = "android" ]; then

        if [ "${LIBS_ABI}" == "windows" ]; then
            downloader https://github.com/openframeworks/projectGenerator/releases/download/$RELEASE/projectGenerator-vs-gui.zip 2> /dev/null
            unzip -q -d "projectGenerator" projectGenerator-vs-gui.zip 2> /dev/null
            rm projectGenerator-vs-gui.zip
            cd ${PKG_OFROOT}
        elif [ "${LIBS_ABI}" == "macos" ]; then
            downloader https://github.com/openframeworks/projectGenerator/releases/download/$RELEASE/projectGenerator-osx.zip 2> /dev/null
            unzip -q projectGenerator-osx.zip
            mv projectGenerator-osx/ projectGenerator
            rm projectGenerator-osx.zip
            rm -rf apps/projectGenerator
        elif [ "${LIBS_ABI}" == "linux64" ]; then
            downloader https://github.com/openframeworks/projectGenerator/releases/download/nightly/projectGenerator-linux-gui.gz 2> /dev/null
            tar -xzvf projectGenerator-linux-gui.gz
            mv $(find . -maxdepth 1 -type d -name "projectGenerator-*") projectGenerator
            #mv projectGenerator-linux-gui projectGenerator
            rm -rf apps/projectGenerator
            rm projectGenerator-linux-gui.gz
        fi

    fi

    # linux remove other platform projects from PG source and copy ofxGui
    if [ "$PKG_PLATFORM" = "linux" ] || [ "$PKG_PLATFORM" = "linux64" ] || [ "$PKG_PLATFORM" = "linuxarmv6l" ] || [ "$PKG_PLATFORM" = "linuxaarch64" ] || [ "$PKG_PLATFORM" = "linuxarmv7l" ] || [ "$PKG_PLATFORM" = "android" ]; then
        cd ${PKG_OFROOT}
        if [ -e apps/projectGenerator/commandLine ]; then
            mv apps/projectGenerator/commandLine .
            rm -rf apps/projectGenerator
            mkdir apps/projectGenerator
            mv commandLine apps/projectGenerator/
            cd apps/projectGenerator/commandLine
            deleteCodeblocks
            deleteVS
            deleteXcode
        fi
    fi

    #delete ofxAndroid in non android
    if [ "$PKG_PLATFORM" != "android" ]; then
        rm -Rf ofxAndroid
        rm -Rf ofxUnitTests
    fi
    #delete ofxiPhone in non ios
    if [ "$PKG_PLATFORM" != "ios" ] && [ "$PKG_PLATFORM" != "macos" ] && [ "$PKG_PLATFORM" != "osx" ]; then
        rm -Rf ofxiPhone
        rm -Rf ofxiOS
        rm -Rf ofxUnitTests
    fi

    #delete ofxMultiTouch in non mobile
    if [ "$PKG_PLATFORM" != "android" ] && [ "$PKG_PLATFORM" != "ios" ] && [ "$PKG_PLATFORM" != "macos" ]; then
        rm -Rf ofxMultiTouch
        rm -Rf ofxAccelerometer
        rm -Rf ofxUnitTests
    fi

    if [ "$PKG_PLATFORM" == "ios" ] || [ "$PKG_PLATFORM" == "android" ]; then
        rm -Rf ofxVectorGraphics
        rm -Rf ofxKinect
        rm -Rf ofxUnitTests
    fi

    #delete unit tests by now
    rm -Rf ${PKG_OFROOT}/tests

    #android, move paths.default.make to paths.make
    if [ "$PKG_PLATFORM" == "android" ]; then
        cd ${PKG_OFROOT}
    fi

    #delete other platforms OF project files
    cd ${PKG_OFROOT}/libs/openFrameworksCompiled/lib
    rm -Rf $otherplatforms
    cd ${PKG_OFROOT}/libs/openFrameworksCompiled/project
    rm -Rf $otherplatforms

    cd ${PKG_OFROOT}/libs/
    rm -Rf download

    #delete scripts
    cd $PKG_OFROOT/scripts

    mkdir -p developer
    # Copy the specified scripts to the new folder
    cp dev/download_libs.sh developer/
    cp dev/download_pg.sh developer/
    cp dev/downloader.sh developer/
    if [ -f dev/init_submodules.sh ]; then
        cp dev/init_submodules.sh developer/
    else
        echo "Warning: dev/init_submodules.sh does not exist. Skipping copy."
    fi

    if [ "$PKG_PLATFORM" != "linux" ] && [ "$PKG_PLATFORM" != "linux64" ] && [ "$PKG_PLATFORM" != "linuxarmv6l" ] && [ "$PKG_PLATFORM" != "linuxarmv7l" ] && [ "$PKG_PLATFORM" != "linuxaarch64" ]; then
        rm -Rf $otherplatforms
        rm -Rf ci dev apothecary
    else
        rm -Rf msys2 vs vs2019 osx ios android ci dev apothecary
    fi

    if [ "$PKG_PLATFORM" = "android" ] || [ "$PKG_PLATFORM" = "ios" ]; then
        rm -Rf qtcreator emscripten
    fi

#    if [ "$PKG_PLATFORM" = "msys2" ]; then
#        rm -Rf emscripten
#    fi

    #delete omap4 scripts for non armv7l
    if [ "$PKG_PLATFORM" = "linux64" ] || [ "$PKG_PLATFORM" = "linux" ] || [ "$PKG_PLATFORM" = "linuxarmv6l" ] || [ "$PKG_PLATFORM" = "linuxaarch64" ]; then
        rm -Rf linux/ubuntu-omap4
    fi


    #delete .svn dirs
    cd $PKG_OFROOT
    rm -Rf $(find . -type d -name .svn)

    #delete .gitignore
    cd $PKG_OFROOT
    # rm -Rf $(find . -name .gitignore -not -path "scripts/templates/*")
    find $PKG_OFROOT -type f -name .gitignore -not -path "scripts/templates/*" -exec rm {} +

    #delete dev folders
    cd ${PKG_OFROOT}/scripts
    rm -Rf dev
    # rm */download_libs.sh

    # put the useful dev scripts back
    mv developer dev

    # make sure any remaining dev scripts are executable
    find . -type f -name "*.sh" -exec chmod +x {} +

    #delete xcode templates in other platforms
    cd $PKG_OFROOT
    if [ "$PKG_PLATFORM" != "osx" ] && [ "$PKG_PLATFORM" != "ios" ] && [ "$platform" != "macos" ]; then
        rm -Rf "xcode templates"
    fi
    echo ----------------------------------------------------------------------
    echo
    echo

    #choose readme
    cd $PKG_OFROOT

    rm -Rf .github/
    rm -Rf .git/
    rm -Rf .appveyor.yml
    rm -Rf .travis.yml
    rm -Rf .gitmodules
    rm -Rf .gitattributes


    if [ "$platform" = "linux" ] || [ "$platform" = "linux64" ] || [ "$platform" = "linuxarmv6l" ] || [ "$platform" = "linuxarmv7l" ] || [ "$platform" = "linuxaarch64" ]; then
        cp docs/linux.md INSTALL.md
    fi

    if [ "$platform" = "vs" ] ||  [ "$platform" = "vs2019" ]; then
        cp docs/visualstudio.md INSTALL.md
    fi

    if [ "$platform" = "msys2" ]; then
        cp docs/msys2.md INSTALL.md
    fi

    if [ "$platform" = "osx" ] || [ "$platform" = "ios" ] || [ "$platform" = "macos" ]; then
        cp docs/osx.md INSTALL.md
    fi

    if [ "$platform" = "android" ]; then
        cp docs/android_studio.md INSTALL_ANDROID_STUDIO.md
    fi

    rm CONTRIBUTING.md

    #copy empty example
    cd $PKG_OFROOT

    cd examples
    find . -type f -name '*.dll' -delete

    cd $PKG_OFROOT
    mkdir -p apps/myApps
    if [ "$PKG_PLATFORM" = "android" ]; then
        cp -r examples/android/androidEmptyExample apps/myApps/
    elif [ "$PKG_PLATFORM" = "ios" ]; then
        cp -r examples/ios/emptyExample apps/myApps/
    else
        cp -r examples/templates/emptyExample apps/myApps/
    fi

    #create compressed package
   if [[ "$PKG_PLATFORM" =~ ^(linux|linux64|android|linuxarmv6l|linuxarmv7l|linuxaarch64|macos|ios|osx)$ ]] && [ "${LIBS_ABI}" != "windows" ]; then
        if [ "$LIBS_ABI" = "" ]; then
            PKG_NAME=of_v${PKG_VERSION}_${PKG_PLATFORM}_release
        else
            PKG_NAME=of_v${PKG_VERSION}_${PKG_PLATFORM}_${LIBS_ABI}_release
        fi
        echo "compressing package to of_v${PKG_VERSION}_${PKG_PLATFORM}_${LIBS_ABI}_release.tar.gz"
        cd $PKG_OFROOT/..
        mkdir ${PKG_NAME}
        mv ${pkgfolder}/* ${PKG_NAME}
        mv ${pkgfolder}/.* ${PKG_NAME} 2>/dev/null || true # add hidden files
        COPYFILE_DISABLE=true tar czf ${PKG_NAME}.tar.gz ${PKG_NAME}
        rm -Rf ${PKG_NAME}
    else
        if [ "$LIBS_ABI" = "" ]; then
            PKG_NAME=of_v${PKG_VERSION}_${PKG_PLATFORM}_release
        else
            PKG_NAME=of_v${PKG_VERSION}_${PKG_PLATFORM}_${LIBS_ABI}_release
        fi 
        echo "compressing package to ${PKG_NAME}.zip"
        cd $PKG_OFROOT/..
        mkdir ${PKG_NAME}
        mv ${pkgfolder}/* ${PKG_NAME}
        mv ${pkgfolder}/.* ${PKG_NAME} 2>/dev/null || true # add hidden files
        zip --symlinks -r ${PKG_NAME}.zip ${PKG_NAME} > /dev/null
        rm -Rf ${PKG_NAME}
    fi
}

set -o pipefail  # trace ERR through pipes
set -o errtrace  # trace ERR through 'time command' and other functions
set -o nounset   # set -u : exit the script if you try to use an uninitialized variable
set -o errexit   # set -e : exit the script if any statement returns a non-true return value

cleanup() {
    cd $packageroot/..
    rm -rf ${pkgfolder}
    rm -rf $HOME/.tmp/npm*
}
trap cleanup 0

error() {
  local parent_lineno="$1"
  if [[ "$#" = "3" ]] ; then
    local message="$2"
    local code="${3:-1}"
    echo "Error on or near line ${parent_lineno}: ${message}; exiting with status ${code}"
  else
    local code="${2:-1}"
    echo "Error on or near line ${parent_lineno}; exiting with status ${code}"
  fi
  rm -rf $HOME/.tmp/npm*
}
trap 'error ${LINENO}' ERR

createPackage $platform $version $packageroot $of_root
