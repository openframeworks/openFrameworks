#! /bin/bash
#
# Free Type
# cross platform ttf/optf font loder
# http://freetype.org/
#
# an autotools project

FORMULA_TYPES=( "osx" "vs" "win_cb" "ios" "android" )

# define the version
VER=2.5.3

# tools for git use
GIT_URL=http://git.savannah.gnu.org/r/freetype/freetype2.git
GIT_TAG=VER-2-5-3

# download the source code and unpack it into LIB_NAME
function download() {
	curl -LO http://download.savannah.gnu.org/releases/freetype/freetype-$VER.tar.gz
	tar -xf freetype-$VER.tar.gz
	mv freetype-$VER freetype
	rm freetype*.tar.gz
}

# prepare the build environment, executed inside the lib src dir
function prepare() {
	: # noop
	mkdir -p lib/$TYPE
}

# executed inside the lib src dir
function build() {
	
	if [ "$TYPE" == "osx" ] ; then
		local BUILD_TO_DIR=$BUILD_DIR/freetype/build/$TYPE/

		local STDLIB="libstdc++"
		local OSX_ARCH="i386"
		local TOOLCHAIN=$XCODE_DEV_ROOT/Toolchains/XcodeDefault.xctoolchain 
		export CC=$TOOLCHAIN/usr/bin/cc
		export CPP=$TOOLCHAIN/usr/bin/cpp
		export CXX=$TOOLCHAIN/usr/bin/c++

		./configure --prefix=$BUILD_TO_DIR --without-bzip2 --enable-static=yes --enable-shared=no \
			CFLAGS="-arch $OSX_ARCH -pipe -stdlib=$STDLIB -Wno-trigraphs -fpascal-strings -O2 -Wreturn-type -Wunused-variable -fmessage-length=0 -fvisibility=hidden" \
			CPP=$CPP \
			CXX=$CXX
		make clean 
		make
		make install
		cp $BUILD_TO_DIR/lib/libfreetype.a lib/$TYPE/libfreetype-$OSX_ARCH.a

		unset OSX_ARCH STDLIB

		# x86_64
		local STDLIB="libc++"
		local OSX_ARCH="x86_64"
		./configure --prefix=$BUILD_TO_DIR --without-bzip2 --enable-static=yes --enable-shared=no \
			CFLAGS="-arch $OSX_ARCH -pipe -stdlib=$STDLIB -Wno-trigraphs -fpascal-strings -O2 -Wreturn-type -Wunused-variable -fmessage-length=0 -fvisibility=hidden" \
			CPP=$CPP \
			CXX=$CXX
		make clean
		make
		make install
		cp $BUILD_TO_DIR/lib/libfreetype.a lib/$TYPE/libfreetype-$OSX_ARCH.a

		cd lib/$TYPE/
		lipo -create libfreetype-i386.a \
					libfreetype-x86_64.a \
					-output libfreetype.a
		cd ../../

		unset OSX_ARCH STDLIB TOOLCHAIN CC CPP CXX

		echo "-----------"
		echo "$BUILD_DIR"
	
	elif [ "$TYPE" == "vs" ] ; then
   		make clean
        # Force config: auto detection is wrong
        cp -v builds/win32/w32-vcc.mk config.mk
        make
	
	elif [ "$TYPE" == "win_cb" ] ; then
		# configure with arch
		if [ $ARCH ==  32 ] ; then
			./configure CFLAGS="-arch i386"
		elif [ $ARCH == 64 ] ; then
			./configure CFLAGS="-arch x86_64"
		fi
		
		make clean; 
		make

	elif [ "$TYPE" == "ios" ] ; then

		unset IOS_DEVROOT IOS_SDKROOT
		local TOOLCHAIN=$XCODE_DEV_ROOT/Toolchains/XcodeDefault.xctoolchain 
		local IOS_DEVROOT=$XCODE_DEV_ROOT/Platforms/iPhoneOS.platform/Developer
		local IOS_SDKROOT=$IOS_DEVROOT/SDKs/iPhoneOS$IOS_SDK_VER.sdk
		local IOS_CC=$TOOLCHAIN/usr/bin/cc
		local IOS_HOST="arm-apple-darwin"
		local IOS_PREFIX="/usr/local/iphone"

		export CPP=$TOOLCHAIN/usr/bin/cpp
		export CXX=$TOOLCHAIN/usr/bin/c++
		export CXXCPP=$TOOLCHAIN/usr/bin/cpp
		export CC=$TOOLCHAIN/usr/bin/cc
		export LD=$TOOLCHAIN/usr/bin/ld
		export AR=$TOOLCHAIN/usr/bin/ar
		export AS=$TOOLCHAIN/usr/bin/as
		export NM=$TOOLCHAIN/usr/bin/nm
		export RANLIB=$TOOLCHAIN/usr/bin/ranlib
		export LDFLAGS="-L$IOS_SDKROOT/usr/lib/"

		local STDLIB="libc++" 
		#local STDLIB="libstdc++"

		# armv7
		local IOS_ARCH="armv7"
		./configure --without-bzip2 --prefix=$IOS_PREFIX --host=$IOS_HOST --enable-static=yes --enable-shared=no \
			CC=$IOS_CC \
			CFLAGS="-arch $IOS_ARCH -pipe -stdlib=$STDLIB -Wno-trigraphs -fpascal-strings -O2 -Wreturn-type -Wunused-variable -fmessage-length=0 -fvisibility=hidden -miphoneos-version-min=$IOS_MIN_SDK_VER -I$IOS_SDKROOT/usr/include/libxml2 -isysroot $IOS_SDKROOT" \
			AR=$AR \
			LDFLAGS="-arch $IOS_ARCH -isysroot $IOS_SDKROOT -miphoneos-version-min=$IOS_MIN_SDK_VER"
		make clean; make
		cp objs/.libs/libfreetype.a lib/$TYPE/libfreetype-$IOS_ARCH.a
		unset IOS_ARCH

		# armv7s
		local IOS_ARCH="armv7s"
		./configure --without-bzip2 --prefix=$IOS_PREFIX --host=$IOS_HOST --enable-static=yes --enable-shared=no \
			CC=$IOS_CC \
			CFLAGS="-arch $IOS_ARCH -pipe -stdlib=$STDLIB -Wno-trigraphs -fpascal-strings -O2 -Wreturn-type -Wunused-variable -fmessage-length=0 -fvisibility=hidden -miphoneos-version-min=$IOS_MIN_SDK_VER -I$IOS_SDKROOT/usr/include/libxml2 -isysroot $IOS_SDKROOT" \
			AR=$AR \
			LDFLAGS="-arch $IOS_ARCH -isysroot $IOS_SDKROOT -miphoneos-version-min=$IOS_MIN_SDK_VER"
		make clean; make
		cp objs/.libs/libfreetype.a lib/$TYPE/libfreetype-$IOS_ARCH.a
		unset IOS_ARCH

		# arm64

		local IOS_MIN_ARM64_SDK_VER="7.0"
		local IOS_ARCH="arm64"
		#export STDLIB="libc++"
		./configure --without-bzip2 --prefix=$IOS_PREFIX --host=$IOS_HOST --enable-static=yes --enable-shared=no \
			CC=$IOS_CC \
			CFLAGS="-arch $IOS_ARCH -pipe -stdlib=$STDLIB -Wno-trigraphs -fpascal-strings -O2 -Wreturn-type -Wunused-variable -fmessage-length=0 -fvisibility=hidden -miphoneos-version-min=$IOS_MIN_SDK_VER -I$IOS_SDKROOT/usr/include/libxml2 -isysroot $IOS_SDKROOT" \
 			AS=$AS \
 			AR=$AR \
	        NM=$IOS_DEVROOT/usr/bin/nm \
			LDFLAGS="-arch $IOS_ARCH -isysroot $IOS_SDKROOT -miphoneos-version-min=$IOS_MIN_SDK_VER"
		make clean; make
		cp objs/.libs/libfreetype.a lib/$TYPE/libfreetype-$IOS_ARCH.a
		unset IOS_ARCH
		unset IOS_MIN_ARM64_SDK_VER

		# simulator

		# i386
		unset IOS_DEVROOT IOS_SDKROOT
		#export STDLIB="libstdc++"
		local IOS_DEVROOT=$XCODE_DEV_ROOT/Platforms/iPhoneSimulator.platform/Developer
		local IOS_SDKROOT=$IOS_DEVROOT/SDKs/iPhoneSimulator$IOS_SDK_VER.sdk

		unset LDFLAGS
		export LDFLAGS="-L$IOS_SDKROOT/usr/lib/"
		local IOS_ARCH="i386"
		./configure --without-bzip2 --enable-static=yes --enable-shared=no \
			CC=$IOS_CC \
			CFLAGS="-arch $IOS_ARCH -pipe -stdlib=$STDLIB -Wno-trigraphs -fpascal-strings -O2 -Wreturn-type -Wunused-variable -fmessage-length=0 -fvisibility=hidden -miphoneos-version-min=$IOS_MIN_SDK_VER -I$IOS_SDKROOT/usr/include/libxml2 -isysroot $IOS_SDKROOT" \
			CPP=$CPP \
			CXX=$CXX \
			LDFLAGS="-arch $IOS_ARCH -isysroot $IOS_SDKROOT -miphoneos-version-min=$IOS_MIN_SDK_VER"
		make clean; make
		cp objs/.libs/libfreetype.a lib/$TYPE/libfreetype-$IOS_ARCH.a
		unset IOS_ARCH

		# x86_64
		local IOS_ARCH="x86_64"
		#export STDLIB="libc++"
		./configure --without-bzip2 --enable-static=yes --enable-shared=no \
			CC=$IOS_CC \
			CFLAGS="-arch $IOS_ARCH -pipe -stdlib=$STDLIB -Wno-trigraphs -fpascal-strings -O2 -Wreturn-type -Wunused-variable -fmessage-length=0 -fvisibility=hidden -miphoneos-version-min=$IOS_MIN_SDK_VER -I$IOS_SDKROOT/usr/include/libxml2 -isysroot $IOS_SDKROOT" \
			CPP=$CPP \
			CXX=$CXX \
			LDFLAGS="-arch $IOS_ARCH -isysroot $IOS_SDKROOT -miphoneos-version-min=$IOS_MIN_SDK_VER"
		make clean; make
		cp objs/.libs/libfreetype.a lib/$TYPE/libfreetype-$IOS_ARCH.a
		unset IOS_ARCH

		# link into universal lib
		cd lib/$TYPE/
		lipo -create libfreetype-armv7.a \
					libfreetype-armv7s.a  \
					libfreetype-arm64.a \
					libfreetype-i386.a \
					libfreetype-x86_64.a \
					-output libfreetype.a
		cd ../../
 
		unset IOS_DEVROOT IOS_SDKROOT IOS_AR IOS_HOST IOS_PREFIX  CPP CXX CXXCPP CXXCPP CC LD AS AR NM RANLIB LDFLAGS STDLIB

	elif [ "$TYPE" == "android" ] ; then
		echoWarning "TODO: build android"
	fi
}

# executed inside the lib src dir, first arg $1 is the dest libs dir root
function copy() {

	# copy headers 
	mkdir -p $1/include/freetype2/config

	# copy files from the build root

	cp -Rv include/* $1/include/freetype2/

	# older versions before 2.5.x
	#mkdir -p $1/include/freetype2/freetype/config
	#cp -Rv include/* $1/include/freetype2/freetype
	#cp -Rv include/ft2build.h $1/include/

	# copy lib
	mkdir -p $1/lib/$TYPE

	if [ "$TYPE" == "osx" ] ; then
		cp -v lib/$TYPE/libfreetype.a $1/lib/$TYPE/freetype.a
	elif [ "$TYPE" == "ios" ] ; then
		cp -v lib/$TYPE/libfreetype.a $1/lib/$TYPE/freetype.a
	elif [ "$TYPE" == "vs" ] ; then
		# cp -v lib/$TYPE/libfreetype.lib $1/lib/$TYPE/libfreetype.lib
		echoWarning "TODO: copy vs lib"
	elif [ "$TYPE" == "win_cb" ] ; then
		# cp -v lib/$TYPE/libfreetype.a $1/lib/$TYPE/libfreetype.a
		echoWarning "TODO: copy win_cb lib"
	elif [ "$TYPE" == "android" ] ; then
		echoWarning "TODO: copy android lib"
	fi
}

# executed inside the lib src dir
function clean() {

	if [ "$TYPE" == "vs" ] ; then
		echoWarning "TODO: clean vs"
	
	elif [ "$TYPE" == "android" ] ; then
		echoWarning "TODO: clean android"
	
	else
		make clean
		rm -f *.a *.lib
	fi
}