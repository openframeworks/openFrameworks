#!/bin/bash
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
of_root=$(readlink -f "$(dirname "$(readlink -f "$0")")/../..")

if [ $# -ge 3 ]; then
    branch=$3
else
    branch=stable
fi

if [ $# -ge 4 ]; then
    libs_abi=$4
else
    libs_abi=""
fi

REPO=https://github.com/openframeworks/openFrameworks.git
REPO_ALIAS=originlocal
BRANCH=$branch

PG_REPO=https://github.com/openframeworks/projectGenerator.git
PG_REPO_ALIAS=originhttps
PG_BRANCH=master

hostArch=`uname`


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

if [ "$platform" != "msys2" ] && [ "$platform" != "linux" ] && [ "$platform" != "linux64" ] && [ "$platform" != "linuxarmv6l" ] && [ "$platform" != "linuxarmv7l" ] && [ "$platform" != "vs2015" ] && [ "$platform" != "vs2017" ] && [ "$platform" != "osx" ] && [ "$platform" != "android" ] && [ "$platform" != "ios" ]; then
    echo usage:
    echo ./create_package.sh platform version
    echo platform:
    echo msys2, linux, linux64, linuxarmv6l, linuxarmv7l, vs2015, vs2017, osx, android, ios, all
    exit 1
fi

if [ "$version" == "" ]; then
    echo usage:
    echo ./create_package.sh platform version [branch]
    echo platform:
    echo msys2, linux, linux64, vs2015, vs2017, osx, android, ios, all
    echo
    echo branch:
    echo master, stable
    exit 1
fi

if [ "$platform" == "msys2" ] && ! ([ "$libs_abi" == "mingw32" ] || [ "$libs_abi" == "mingw64" ]); then
    echo ./create_package.sh : libs_abi must be \'mingw32\' or \'mingw64\' for \'msys2\' platform
    exit 1
fi

echo
echo
echo
echo --------------------------------------------------------------------------
echo "Creating package $version for $platform $libs_abi"
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

cd apps
echo "Cloning project generator from $PG_REPO $PG_BRANCH"
git clone $PG_REPO --depth=1 --branch=$PG_BRANCH 2> /dev/null
gitfinishedok=$?
if [ $gitfinishedok -ne 0 ]; then
    echo "Error connecting to github"
    exit 1
fi

cd $packageroot


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
    if [ "$pkg_platform" != "linuxarmv6l" ] && [ "$pkg_platform" != "linuxarmv7l" ]; then
        mkdir -p ${main_ofroot}/libs/openFrameworksCompiled/lib/linux64/
        cd ${main_ofroot}/libs/openFrameworksCompiled/lib/linux64/
        rm -f ${main_ofroot}/libs/openFrameworksCompiled/lib/linux64/libopenFrameworksDebug.a
        wget http://ci.openframeworks.cc/openFrameworks_libs/linux64/libopenFrameworksDebug.a

        cd ${main_ofroot}/apps/projectGenerator
        git pull origin $PG_BRANCH
        cd commandLine
        echo "Recompiling command line PG"
        if [ -d ~/logs ]; then
            PROJECT_OPTIMIZATION_CFLAGS_DEBUG="-O0 -g0" CXXFLAGS=-ftrack-macro-expansion=0 make DebugNoOF > ~/logs/compilePG.log 2>&1 &
            makePGPID=$!
            echoDots $makePGPID
            wait $makePGPID
        else
            PROJECT_OPTIMIZATION_CFLAGS_DEBUG="-O0 -g0" CXXFLAGS=-ftrack-macro-expansion=0 make DebugNoOF
        fi

        cd ${pkg_ofroot}
        echo "Creating project files for $pkg_platform"
        if [ "$pkg_platform" == "vs2015" ] || [ "$pkg_platform" == "vs2017" ]; then
            pg_platform="vs"
        else
            pg_platform=$pkg_platform
        fi
        ${main_ofroot}/apps/projectGenerator/commandLine/bin/projectGenerator_debug --recursive -p${pg_platform} -o$pkg_ofroot $pkg_ofroot/examples > /dev/null
    elif [ "$pkg_platform" == "linuxarmv6l" ] || [ "$pkg_platform" == "linuxarmv7l" ]; then
        for example_group in $pkg_ofroot/examples/*; do
            for example in $example_group/*; do
                if [ -d $example ]; then
                    cp $pkg_ofroot/scripts/templates/linux/Makefile $example/
                    cp $pkg_ofroot/scripts/templates/linux/config.make $example/
                fi
            done
        done
    fi
    cd ${pkg_ofroot}
}

function createPackage {
    pkg_platform=$1
    pkg_version=$2
    pkg_ofroot=$3
    main_ofroot=$4

    #remove previously created package
    cd $pkg_ofroot/..
	rm -Rf of_v${pkg_version}_${pkg_platform}${libs_abi}.*
	rm -Rf of_v${pkg_version}_${pkg_platform}${libs_abi}_*
    echo "Creating package $pkg_platform $version in $pkg_ofroot"

    #remove devApps folder
    rm -r $pkg_ofroot/apps/devApps

    #remove projectGenerator folder
    if [ "$pkg_platform" = "msys2" ]; then
    	rm -rf $pkg_ofroot/apps/projectGenerator
    fi

	cd $pkg_ofroot/examples

	#delete ios examples in other platforms
	if [ "$pkg_platform" != "ios" ]; then
		rm -Rf ios
		rm -Rf tvOS
	fi

	#delete android examples in other platforms
	if [ "$pkg_platform" != "android" ]; then
		rm -Rf android
	fi

	#delete desktop examples in mobile packages
	if [ "$pkg_platform" == "android" ] || [ "$pkg_platform" == "ios" ]; then
		rm -Rf 3d
		rm -Rf communication
		rm -Rf computer_vision
		rm -Rf events
		rm -Rf gl
		rm -Rf gles
		rm -Rf graphics
		rm -Rf gui
		rm -Rf input_output
		rm -Rf math
		rm -Rf shader
		rm -Rf sound
		rm -Rf strings
		rm -Rf templates
		rm -Rf threads
		rm -Rf video
        rm -Rf windowing
	fi

	#delete osx examples in linux
	if [ "$pkg_platform" == "linux" ] || [ "$pkg_platform" == "linux64" ] || [ "$pkg_platform" == "linuxarmv6l" ] || [ "$pkg_platform" == "linuxarmv7l" ]; then
	    rm -Rf video/osxHighPerformanceVideoPlayerExample
	    rm -Rf video/osxVideoRecorderExample
	fi

	if [ "$pkg_platform" == "linux" ] || [ "$pkg_platform" == "linux64" ]; then
	    rm -Rf gles
	fi

	if [ "$pkg_platform" == "linuxarmv6l" ] || [ "$pkg_platform" == "linuxarmv7l" ]; then
	    rm -Rf addons/3DModelLoaderExample
        rm -Rf addons/allAddonsExample
        rm -Rf addons/assimpExample
        rm -Rf addons/kinectExample
        rm -Rf addons/vectorGraphicsExample

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

        rm -Rf utils/systemSpeakExample
        rm -Rf utils/fileBufferLoadingCSVExample

        rm -Rf 3d/modelNoiseExample

        rm -Rf windowing
    fi

    if [ "$pkg_platform" == "linuxarmv6l" ]; then
        rm -Rf utils/dragDropExample
        rm -Rf utils/fileOpenSaveDialogExample
	fi

	if [ "$pkg_platform" == "msys2" ] || [ "$pkg_platform" == "vs2015" ] || [ "$pkg_platform" == "vs2017" ]; then
	    rm -Rf video/osxHighPerformanceVideoPlayerExample
	    rm -Rf video/osxVideoRecorderExample
	    rm -Rf gles
	fi

	if [ "$pkg_platform" == "osx" ]; then
	    rm -Rf gles
	    rm -Rf gl/computeShaderParticlesExample
	    rm -Rf gl/computeShaderTextureExample
        rm -Rf gl/transformFeedbackExample
	fi



	#delete tutorials by now
	rm -Rf $pkg_ofroot/tutorials

    #download external dependencies
    cd $pkg_ofroot/
    if [ "$pkg_platform" = "osx" ]; then
        scripts/osx/download_libs.sh
        scripts/emscripten/download_libs.sh -n
    elif [ "$pkg_platform" = "linux64" ]; then
        scripts/linux/download_libs.sh -a 64$libs_abi
        scripts/emscripten/download_libs.sh -n
    elif [ "$pkg_platform" = "linuxarmv6l" ]; then
        scripts/linux/download_libs.sh -a armv6l
    elif [ "$pkg_platform" = "linuxarmv7l" ]; then
        scripts/linux/download_libs.sh -a armv7l
    elif [ "$pkg_platform" = "msys2" ]; then
        if [ "$libs_abi" = "mingw64" ]; then
            scripts/msys2/download_libs.sh -a 64
        else
            scripts/msys2/download_libs.sh
        fi
    elif [ "$pkg_platform" = "vs2015" ]; then
        scripts/dev/download_libs.sh -p vs2015
    elif [ "$pkg_platform" = "vs2017" ]; then
        scripts/dev/download_libs.sh -p vs2017
    elif [ "$pkg_platform" = "android" ]; then
        scripts/android/download_libs.sh
    elif [ "$pkg_platform" = "ios" ]; then
        scripts/ios/download_libs.sh
    fi

    #create project files for platform
    createProjectFiles $pkg_platform $pkg_ofroot


    #delete other platform libraries
    if [ "$pkg_platform" = "linux" ]; then
        otherplatforms="linux64 linuxarmv6l linuxarmv7l osx msys2 vs ios tvos android"
    fi

    if [ "$pkg_platform" = "linux64" ]; then
        otherplatforms="linux linuxarmv6l linuxarmv7l osx msys2 vs ios tvos android"
    fi

    if [ "$pkg_platform" = "linuxarmv6l" ]; then
        otherplatforms="linux64 linux linuxarmv7l osx msys2 vs ios tvos android"
    fi

    if [ "$pkg_platform" = "linuxarmv7l" ]; then
        otherplatforms="linux64 linux linuxarmv6l osx msys2 vs ios tvos android"
    fi

    if [ "$pkg_platform" = "osx" ]; then
        otherplatforms="linux linux64 linuxarmv6l linuxarmv7l msys2 vs ios tvos android"
    fi

    if [ "$pkg_platform" = "msys2" ]; then
        otherplatforms="linux linux64 linuxarmv6l linuxarmv7l osx vs ios tvos android"
    fi

    if [ "$pkg_platform" = "vs2015" ] || [ "$pkg_platform" = "vs2017" ]; then
        otherplatforms="linux linux64 linuxarmv6l linuxarmv7l osx msys2 ios tvos android"
    fi

    if [ "$pkg_platform" = "ios" ]; then
        otherplatforms="linux linux64 linuxarmv6l linuxarmv7l msys2 vs android osx"
    fi

    if [ "$pkg_platform" = "android" ]; then
        otherplatforms="linux linux64 linuxarmv6l linuxarmv7l osx msys2 vs ios tvos"
    fi


	#download and uncompress PG
	echo "Creating projectGenerator"
	mkdir -p $HOME/.tmp
	export TMPDIR=$HOME/.tmp
    if [ "$pkg_platform" = "vs2015" ] || [ "$pkg_platform" = "vs2017" ]; then
		cd ${pkg_ofroot}/apps/projectGenerator/frontend
		npm install > /dev/null
		npm run build:vs > /dev/null
		mv dist/projectGenerator-win32-ia32 ${pkg_ofroot}/projectGenerator-vs
		cd ${pkg_ofroot}
		rm -rf apps/projectGenerator
		cd ${pkg_ofroot}/projectGenerator-vs/resources/app/app/
		wget http://ci.openframeworks.cc/projectGenerator/projectGenerator-vs.zip 2> /dev/null
		unzip projectGenerator-vs.zip 2> /dev/null
		rm projectGenerator-vs.zip
		cd ${pkg_ofroot}
		sed -i "s/osx/vs/g" projectGenerator-vs/resources/app/settings.json
	fi

    if [ "$pkg_platform" = "osx" ]; then
		wget http://ci.openframeworks.cc/projectGenerator/projectGenerator-osx.zip 2> /dev/null
        unzip projectGenerator-osx.zip
        mv projectGenerator-osx projectGenerator
        rm projectGenerator-osx.zip
        sed -i "s/osx/$pkg_platform/g" projectGenerator/projectGenerator.app/Contents/Resources/app/settings.json
		rm -rf apps/projectGenerator
	fi

    if [ "$pkg_platform" = "ios" ]; then
		wget http://ci.openframeworks.cc/projectGenerator/projectGenerator-ios.zip 2> /dev/null
        unzip projectGenerator-ios.zip
        mv projectGenerator-ios projectGenerator
        rm projectGenerator-ios.zip
		rm -rf apps/projectGenerator
	fi

	if [ "$pkg_platform" = "linux" ]; then
		cd ${pkg_ofroot}/apps/projectGenerator/frontend
		npm install > /dev/null
		npm run build:linux32 > /dev/null
		mv dist/projectGenerator-linux-ia32 ${pkg_ofroot}/projectGenerator-linux
		cd ${pkg_ofroot}
		sed -i "s/osx/linux/g" projectGenerator-linux/resources/app/settings.json
	fi

	if [ "$pkg_platform" = "linux64" ]; then
		cd ${pkg_ofroot}/apps/projectGenerator/frontend
		npm install > /dev/null
		npm run build:linux64 > /dev/null
		mv dist/projectGenerator-linux-x64 ${pkg_ofroot}/projectGenerator-linux64
		cd ${pkg_ofroot}
		sed -i "s/osx/linux64/g" projectGenerator-linux64/resources/app/settings.json
	fi

    if [ "$pkg_platform" = "android" ]; then
		cd ${pkg_ofroot}/apps/projectGenerator/frontend
		npm install > /dev/null
		npm run build:vs > /dev/null
		mv dist/projectGenerator-win32-ia32 ${pkg_ofroot}/projectGenerator-windows
		cd ${pkg_ofroot}/projectGenerator-windows/resources/app/app/
		wget http://ci.openframeworks.cc/projectGenerator/projectGenerator-vs.zip 2> /dev/null
		unzip projectGenerator-vs.zip 2> /dev/null
		rm projectGenerator-vs.zip
		cd ${pkg_ofroot}
		sed -i "s/osx/android/g" projectGenerator-windows/resources/app/settings.json

		wget http://ci.openframeworks.cc/projectGenerator/projectGenerator-android.zip 2> /dev/null
        unzip projectGenerator-android.zip
		mv projectGenerator-android projectGenerator-osx
        rm projectGenerator-android.zip

		cd ${pkg_ofroot}/apps/projectGenerator/frontend
		npm install > /dev/null
		npm run build:linux64 > /dev/null
		mv dist/projectGenerator-linux-x64 ${pkg_ofroot}/projectGenerator-linux64
		cd ${pkg_ofroot}
		sed -i "s/osx/android/g" projectGenerator-linux64/resources/app/settings.json
	fi

	# linux remove other platform projects from PG source and copy ofxGui
	if [ "$pkg_platform" = "linux" ] || [ "$pkg_platform" = "linux64" ] || [ "$pkg_platform" = "linuxarmv6l" ] || [ "$pkg_platform" = "linuxarmv7l" ] || [ "$pkg_platform" = "android" ]; then
	    cd ${pkg_ofroot}
		mv apps/projectGenerator/commandLine .
		mv apps/projectGenerator/ofxProjectGenerator .
		rm -rf apps/projectGenerator
		mkdir apps/projectGenerator
		mv commandLine apps/projectGenerator/
		mv ofxProjectGenerator apps/projectGenerator/
		cd apps/projectGenerator/commandLine
		deleteCodeblocks
		deleteVS
		deleteXcode
	fi

	#delete ofxAndroid in non android
	if [ "$pkg_platform" != "android" ]; then
		rm -Rf ofxAndroid
		rm -Rf ofxUnitTests
	fi
	#delete ofxiPhone in non ios
	if [ "$pkg_platform" != "ios" ]; then
		rm -Rf ofxiPhone
		rm -Rf ofxiOS
		rm -Rf ofxUnitTests
	fi

	#delete ofxMultiTouch & ofxAccelerometer in non mobile
	if [ "$pkg_platform" != "android" ] && [ "$pkg_platform" != "ios" ]; then
		rm -Rf ofxMultiTouch
		rm -Rf ofxAccelerometer
		rm -Rf ofxUnitTests
	fi

	if [ "$pkg_platform" == "ios" ] || [ "$pkg_platform" == "android" ]; then
	    rm -Rf ofxVectorGraphics
   	    rm -Rf ofxKinect
		rm -Rf ofxUnitTests
	fi

	#delete unit tests by now
	rm -Rf ${pkg_ofroot}/tests

	#delete eclipse projects
	if [ "$pkg_platform" != "android" ] && [ "$pkg_platform" != "linux" ] && [ "$pkg_platform" != "linux64" ] && [ "$pkg_platform" != "linuxarmv6l" ] && [ "$pkg_platform" != "linuxarmv7l" ]; then
		cd ${pkg_ofroot}
		deleteEclipse
		if [ -f libs/openFrameworks/.settings ]; then
    		rm -R libs/openFrameworks/.settings
    	fi
	fi

	#android, move paths.default.make to paths.make
	if [ "$pkg_platform" == "android" ]; then
	    cd ${pkg_ofroot}
	    #mv libs/openFrameworksCompiled/project/android/paths.default.make libs/openFrameworksCompiled/project/android/paths.make
	fi

    #delete other platforms OF project files
    cd ${pkg_ofroot}/libs/openFrameworksCompiled/lib
    rm -Rf $otherplatforms
    cd ${pkg_ofroot}/libs/openFrameworksCompiled/project
    rm -Rf $otherplatforms

    #delete scripts
    cd $pkg_ofroot/scripts
	if [ "$pkg_platform" != "linux64" ] && [ "$pkg_platform" != "linuxarmv6l" ] && [ "$pkg_platform" != "linuxarmv7l" ]; then
    	rm -Rf $otherplatforms
        rm -Rf ci dev apothecary
	else
    	rm -Rf msys2 vs osx ios android ci dev apothecary
	fi

    if [ "$pkg_platform" = "android" ] || [ "$pkg_platform" = "ios" ] || [ "$pkg_platform" = "vs" ]; then
        rm -Rf qtcreator emscripten
    fi

    if [ "$pkg_platform" = "msys2" ]; then
        rm -Rf emscripten
    fi

    #delete omap4 scripts for non armv7l
	if [ "$pkg_platform" = "linux64" ] || [ "$pkg_platform" = "linux" ] || [ "$pkg_platform" = "linuxarmv6l" ]; then
	    rm -Rf linux/ubuntu-omap4
	fi

	if [ "$pkg_platform" == "ios" ]; then
		rm -Rf osx
	fi

    #delete .svn dirs
    cd $pkg_ofroot
    rm -Rf $(find . -type d -name .svn)

    #delete .gitignore
    cd $pkg_ofroot
    rm -Rf $(find . -name .gitignore -not -path "scripts/templates/*")

    #delete dev folders
    cd ${pkg_ofroot}/scripts
    rm -Rf dev
    rm */download_libs.sh

	#delete xcode templates in other platforms
	cd $pkg_ofroot
	if [ "$pkg_platform" != "osx" ] && [ "$pkg_platform" != "ios" ]; then
		rm -Rf "xcode templates"
	fi
    echo ----------------------------------------------------------------------
    echo
    echo

    #choose readme
    cd $pkg_ofroot
    if [ "$platform" = "linux" ] || [ "$platform" = "linux64" ] || [ "$platform" = "linuxarmv6l" ] || [ "$platform" = "linuxarmv7l" ]; then
        cp docs/linux.md INSTALL.md
    fi

    if [ "$platform" = "vs2015" ] || [ "$platform" = "vs2017" ]; then
        cp docs/visualstudio.md INSTALL.md
    fi

    if [ "$platform" = "msys2" ]; then
        cp docs/msys2.md INSTALL.md
    fi

    if [ "$platform" = "osx" ] || [ "$platform" = "ios" ]; then
        cp docs/osx.md INSTALL.md
    fi

    if [ "$platform" = "android" ]; then
        # cp docs/android_eclipse.md INSTALL_ECLIPSE.md
        cp docs/android_studio.md INSTALL_ANDROID_STUDIO.md
    fi

    rm CONTRIBUTING.md

    #copy empty example
    cd $pkg_ofroot
    mkdir -p apps/myApps
    if [ "$pkg_platform" = "android" ]; then
        cp -r examples/android/androidEmptyExample apps/myApps/
    elif [ "$pkg_platform" = "ios" ]; then
        cp -r examples/ios/emptyExample apps/myApps/
    else
        cp -r examples/templates/emptyExample apps/myApps/
    fi

    #create compressed package
    if [ "$pkg_platform" = "linux" ] || [ "$pkg_platform" = "linux64" ] || [ "$pkg_platform" = "android" ] || [ "$pkg_platform" = "linuxarmv6l" ] || [ "$pkg_platform" = "linuxarmv7l" ]; then
        echo "compressing package to of_v${pkg_version}_${pkg_platform}${libs_abi}_release.tar.gz"
        cd $pkg_ofroot/..
        mkdir of_v${pkg_version}_${pkg_platform}${libs_abi}_release
        mv ${pkgfolder}/* of_v${pkg_version}_${pkg_platform}${libs_abi}_release
        COPYFILE_DISABLE=true tar czf of_v${pkg_version}_${pkg_platform}${libs_abi}_release.tar.gz of_v${pkg_version}_${pkg_platform}${libs_abi}_release
        rm -Rf of_v${pkg_version}_${pkg_platform}${libs_abi}_release
    else
        if [ "$libs_abi" = "" ]; then
            pkg_name=of_v${pkg_version}_${pkg_platform}_release
        else
            pkg_name=of_v${pkg_version}_${pkg_platform}_${libs_abi}_release
        fi 
        echo "compressing package to ${pkg_name}.zip"
        cd $pkg_ofroot/..
        mkdir ${pkg_name}
        mv ${pkgfolder}/* ${pkg_name}
        zip --symlinks -r ${pkg_name}.zip ${pkg_name} > /dev/null
        rm -Rf ${pkg_name}
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
