#!/bin/sh
# 
# build Tess2 for iOS and iOS Simulator
#
# largely adapted from the Assimp build scripts

set -e

### Variables

BUILD_DIR="./lib/ios"

IOS_BASE_SDK="6.1"
IOS_DEPLOY_TGT="5.1"

### Functions

setenv_all() {

	# Add internal libs
	export CFLAGS="$CFLAGS"
	export CPP="$DEVROOT/usr/bin/llvm-cpp-4.2"
	export CXX="$DEVROOT/usr/bin/llvm-g++-4.2"
	export CXXCPP="$DEVROOT/usr/bin/llvm-cpp-4.2"
	export CC="$DEVROOT/usr/bin/llvm-gcc-4.2"
	export LD=$DEVROOT/usr/bin/ld
	export AR=$DEVROOT/usr/bin/ar
	export AS=$DEVROOT/usr/bin/as
	export NM=$DEVROOT/usr/bin/nm
	export RANLIB=$DEVROOT/usr/bin/ranlib
	export LDFLAGS="-L$SDKROOT/usr/lib/"
	
	export CPPFLAGS=$CFLAGS
	export CXXFLAGS=$CFLAGS
}

setenv_armv7() {
	unset DEVROOT SDKROOT CFLAGS CC LD CPP CXX AR AS NM CXXCPP RANLIB LDFLAGS CPPFLAGS CXXFLAGS
	export DEVROOT=$(xcode-select --print-path)/Platforms/iPhoneOS.platform/Developer
	export SDKROOT=$DEVROOT/SDKs/iPhoneOS$IOS_BASE_SDK.sdk
	export CFLAGS="-arch armv7 -pipe -no-cpp-precomp -isysroot $SDKROOT -miphoneos-version-min=$IOS_DEPLOY_TGT -I$SDKROOT/usr/include/"
	setenv_all
	rm -f CMakeCache.txt
	cmake -G 'Unix Makefiles' -DCMAKE_TOOLCHAIN_FILE=./build/ios/IOS_ARMV7_TOOLCHAIN.cmake
}

setenv_armv7s() {
	unset DEVROOT SDKROOT CFLAGS CC LD CPP CXX AR AS NM CXXCPP RANLIB LDFLAGS CPPFLAGS CXXFLAGS
	export DEVROOT=$(xcode-select --print-path)/Platforms/iPhoneOS.platform/Developer
	export SDKROOT=$DEVROOT/SDKs/iPhoneOS$IOS_BASE_SDK.sdk
	export CFLAGS="-arch armv7s -pipe -no-cpp-precomp -isysroot $SDKROOT -miphoneos-version-min=$IOS_DEPLOY_TGT -I$SDKROOT/usr/include/"
	setenv_all
	rm -f CMakeCache.txt
	cmake -G 'Unix Makefiles' -DCMAKE_TOOLCHAIN_FILE=./build/ios/IOS_ARMV7S_TOOLCHAIN.cmake
}

setenv_i386() {
	unset DEVROOT SDKROOT CFLAGS CC LD CPP CXX AR AS NM CXXCPP RANLIB LDFLAGS CPPFLAGS CXXFLAGS
	export DEVROOT=$(xcode-select --print-path)/Platforms/iPhoneSimulator.platform/Developer
	export SDKROOT=$DEVROOT/SDKs/iPhoneSimulator$IOS_BASE_SDK.sdk
	export CFLAGS="-arch i386 -pipe -no-cpp-precomp -isysroot $SDKROOT -miphoneos-version-min=$IOS_DEPLOY_TGT"
	setenv_all
	rm -f CMakeCache.txt
	cmake -G 'Unix Makefiles' -DCMAKE_TOOLCHAIN_FILE=./build/ios/IOS_i386_TOOLCHAIN.cmake
}

### GO

cd ../../

# armv7
setenv_armv7
echo 'Building armv7 library'
make clean; make
mv libtess2.a libtess2-armv7.a

#armv7s
setenv_armv7s
echo 'Building armv7s library'
make clean; make
mv libtess2.a libtess2-armv7s.a

# i386
setenv_i386
echo 'Building i386 library'
make clean; make
mv libtess2.a libtess2-i386.a

# link into universal lib
echo 'Creating fat binary...'
lipo -c libtess2-armv7.a libtess2-armv7s.a libtess2-i386.a -o libtess2.a
