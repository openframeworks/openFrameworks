#! /bin/bash
#
# Open Asset Import Library
# cross platform 3D model loader
# https://github.com/assimp/assimp
#
# uses CMake

# define the version
VER=3.1.1

# tools for git use
GIT_URL=
# GIT_URL=https://github.com/assimp/assimp.git
GIT_TAG=

FORMULA_TYPES=( "osx" "osx-clang-libc++" "ios" )

IOS_SDK_VERSION=7.1
IOS_SDK_TARGET=5.1.1
XCODE_ROOT_DIR=/Applications/Xcode.app/Contents
TOOLCHAIN=$XCODE_ROOT_DIR/Developer/Toolchains/XcodeDefault.xctoolchain


# download the source code and unpack it into LIB_NAME
function download() {

	# stable release from source forge
	curl -LO "https://github.com/assimp/assimp/archive/v$VER.zip"
	unzip -oq "v$VER.zip"
	mv "assimp-$VER" assimp
	rm "v$VER.zip"

    # fix an issue with static libs being disabled - see issue https://github.com/assimp/assimp/issues/271
    # this could be fixed fairly soon - so see if its needed for future releases.
    sed -i -e 's/SET ( ASSIMP_BUILD_STATIC_LIB OFF/SET ( ASSIMP_BUILD_STATIC_LIB ON/g' assimp/CMakeLists.txt
    sed -i -e 's/option ( BUILD_SHARED_LIBS "Build a shared version of the library" ON )/option ( BUILD_SHARED_LIBS "Build a shared version of the library" OFF )/g' assimp/CMakeLists.txt
}

# prepare the build environment, executed inside the lib src dir
function prepare() {

    rm -f CMakeCache.txt || true

    # we don't use the build script for iOS now as it is less reliable than doing it our self
	if [ "$TYPE" == "ios" ] ; then
		# ref: http://stackoverflow.com/questions/6691927/how-to-build-assimp-library-for-ios-device-and-simulator-with-boost-library

        IOS_SDK_DEVICE=iPhoneOS

		local buildOpts="-DASSIMP_BUILD_STATIC_LIB=1 -DASSIMP_BUILD_SHARED_LIB=0 -DASSIMP_ENABLE_BOOST_WORKAROUND=1"
        libsToLink=""

        archs=("armv7" "armv7s" "arm64" "i386" "x86_64")
        for curArch in "${archs[@]}"
        do
            echo "Building $curArch "

            IOS_SDK_DEVICE=iPhoneOS

            if [ "$curArch" == "i386" ] || [ "$curArch" == "x86_64" ]; then
                echo 'Target SDK set to SIMULATOR.'
                IOS_SDK_DEVICE=iPhoneSimulator
            fi

            OUR_DEV_ROOT="$XCODE_ROOT_DIR/Developer/Platforms/$IOS_SDK_DEVICE.platform/Developer"
            OUR_SDK_ROOT="$OUR_DEV_ROOT/SDKs/$IOS_SDK_DEVICE$IOS_SDK_VERSION.sdk"

            OUR_CFLAGS="-arch $curArch -O3 -DNDEBUG -funroll-loops -isysroot $OUR_SDK_ROOT -stdlib=libstdc++ -miphoneos-version-min=$IOS_SDK_TARGET -I$OUR_SDK_ROOT/usr/include/"

            export LDFLAGS="-L$OUR_SDK_ROOT/usr/lib/"
            export DEVROOT="$OUR_DEV_ROOT"
            export SDKROOT="$OUR_SDK_ROOT"
            export CFLAGS="$OUR_CFLAGS"
            export CPPFLAGS="$OUR_CFLAGS"
            export CXXFLAGS="$OUR_CFLAGS"

            #echo " out c_flags are $OUR_CFLAGS "

            cmake -G 'Unix Makefiles' $buildOpts -DCMAKE_C_FLAGS="$OUR_CFLAGS" -DCMAKE_CXX_FLAGS="$OUR_CFLAGS" -DCMAKE_CXX_FLAGS="$OUR_CFLAGS".

            $XCODE_ROOT_DIR/Developer/usr/bin/make clean
            $XCODE_ROOT_DIR/Developer/usr/bin/make assimp -j 8 -l

            fileToRenameTo="./lib/libassimp-$TYPE-$curArch.a"

            mv ./lib/libassimp.a $fileToRenameTo

            libsToLink="$libsToLink $fileToRenameTo"

            $XCODE_ROOT_DIR/Developer/usr/bin/make clean

        done

		# link into universal lib
		command="lipo -create $libsToLink -o lib/libassimp-ios.a"
        $command || true
	fi

	if [ "$TYPE" == "osx" ] ; then

		# warning, assimp on github uses the ASSIMP_ prefix for CMake options ...
		# these may need to be updated for a new release
		local buildOpts="--build build/$TYPE -DASSIMP_BUILD_STATIC_LIB=1 -DASSIMP_BUILD_SHARED_LIB=0 -DASSIMP_ENABLE_BOOST_WORKAROUND=1"

		# 32 bit
		cmake -G 'Unix Makefiles' $buildOpts -DCMAKE_C_FLAGS="-arch i386 -O3 -DNDEBUG -funroll-loops" -DCMAKE_CXX_FLAGS="-arch i386 -stdlib=libstdc++ -O3 -DNDEBUG -funroll-loops" .
		make assimp
		mv lib/libassimp.a lib/libassimp-osx-i386.a
		make clean

		# 64 bit
		cmake -G 'Unix Makefiles' $buildOpts -DCMAKE_C_FLAGS="-arch x86_64 -O3 -DNDEBUG -funroll-loops" -DCMAKE_CXX_FLAGS="-arch x86_64 -stdlib=libc++ -O3 -DNDEBUG -funroll-loops" .
		make assimp 
		mv lib/libassimp.a lib/libassimp-osx-x86_64.a
		make clean

		# link into universal lib
		lipo -c lib/libassimp-osx-i386.a lib/libassimp-osx-x86_64.a -o lib/libassimp-osx.a

	elif [ "$TYPE" == "osx-clang-libc++" ] ; then
		echoWarning "WARNING: this needs to be updated - do we even need it anymore?"

		# warning, assimp on github uses the ASSIMP_ prefix for CMake options ...
		# these may need to be updated for a new release
		local buildOpts="--build build/$TYPE"

		export CPP=`xcrun -find clang++`
		export CXX=`xcrun -find clang++`
		export CXXCPP=`xcrun -find clang++`
		export CC=`xcrun -find clang`
		
		# 32 bit
		cmake -G 'Unix Makefiles' $buildOpts -DCMAKE_C_FLAGS="-arch i386 $assimp_flags" -DCMAKE_CXX_FLAGS="-arch i386 -std=c++11 -stdlib=libc++ $assimp_flags" .
		make assimp -j 
		mv lib/libassimp.a lib/libassimp-i386.a
		make clean

		# rename lib
		libtool -c lib/libassimp-i386.a -o lib/libassimp-osx.a

	elif [ "$TYPE" == "linux" ] ; then
		echoWarning "TODO: linux build"

	elif [ "$TYPE" == "linux64" ] ; then
		echoWarning "TODO: linux64 build"

	elif [ "$TYPE" == "vs" ] ; then
		echoWarning "TODO: vs build"

	elif [ "$TYPE" == "win_cb" ] ; then
		echoWarning "TODO: win_cb build"

	elif [ "$TYPE" == "android" ] ; then
		echoWarning "TODO: android build"
	fi
}

# executed inside the lib src dir, first arg $1 is the dest libs dir root
function copy() {

	# headers
	mkdir -p $1/include
    rm -r $1/include/assimp || true
    rm -r $1/include/* || true
	cp -Rv include/* $1/include

	# libs
	mkdir -p $1/lib/$TYPE
	if [ "$TYPE" == "vs" ] ; then
		cp -Rv lib/libassimp.lib $1/lib/$TYPE/assimp.lib
	elif [ "$TYPE" == "osx" ] ; then
		cp -Rv lib/libassimp-osx.a $1/lib/$TYPE/assimp.a
	elif [ "$TYPE" == "ios" ] ; then
		cp -Rv lib/libassimp-ios.a $1/lib/$TYPE/assimp.a
	else
		cp -Rv lib/libassimp.a $1/lib/$TYPE/assimp.a
	fi
}

# executed inside the lib src dir
function clean() {

	if [ "$TYPE" == "vs" ] ; then
		echoWarning "TODO: clean vs"

	elif [ "$TYPE" == "vs" ] ; then
		echoWarning "TODO: clean android"

	else
		make clean
		rm -f CMakeCache.txt
	fi
}
