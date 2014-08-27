#! /bin/bash
#
# Free Image
# cross platform image io
# http://freeimage.sourceforge.net
#
# Makefile build system, 
# some Makefiles are out of date so patching/modification may be required

FORMULA_TYPES=( "osx" "vs" "win_cb" "ios" "android" )

# define the version
VER=3160 # 3.16.0

# tools for git use
GIT_URL=
GIT_TAG=

# download the source code and unpack it into LIB_NAME
function download() {
	if [ "$TYPE" == "vs" -o "$TYPE" == "win_cb" ] ; then
		# For win32, we simply download the pre-compiled binaries.
		curl -LO http://downloads.sourceforge.net/freeimage/FreeImage"$VER"Win32.zip
		unzip -qo FreeImage"$VER"Win32.zip
		rm FreeImage"$VER"Win32.zip
	else
		curl -LO http://downloads.sourceforge.net/freeimage/FreeImage"$VER".zip
		unzip -qo FreeImage"$VER".zip
		rm FreeImage"$VER".zip
	fi
}

# prepare the build environment, executed inside the lib src dir
function prepare() {
	
	if [ "$TYPE" == "osx" ] ; then

		# patch outdated Makefile.osx provided with FreeImage, check if patch was applied first
		if patch -p0 -u -N --dry-run --silent < $FORMULA_DIR/Makefile.osx.patch 2>/dev/null ; then
			patch -p0 -u < $FORMULA_DIR/Makefile.osx.patch
		fi

		# set SDK using apothecary settings
		sed -i tmp "s|MACOSX_SDK =.*|MACOSX_SDK = $OSX_SDK_VER|" Makefile.osx
		sed -i tmp "s|MACOSX_MIN_SDK =.*|MACOSX_MIN_SDK = $OSX_MIN_SDK_VER|" Makefile.osx

	elif [ "$TYPE" == "ios" ] ; then

		mkdir -p Dist/$TYPE

		# copy across new Makefile for iOS.
		cp -v $FORMULA_DIR/Makefile.ios Makefile.ios
	fi
}

# executed inside the lib src dir
function build() {
	
	if [ "$TYPE" == "osx" ] ; then
		make -f Makefile.osx

		strip -x Dist/libfreeimage.a

	elif [ "$TYPE" == "ios" ] ; then

		# Notes: 
        # --- for 3.1+ Must use "-DNO_LCMS -D__ANSI__ -DDISABLE_PERF_MEASUREMENT" to compile LibJXR
        # --- arm64 has lots of hotfixes using sed inline here.
		export TOOLCHAIN=$XCODE_DEV_ROOT/Toolchains/XcodeDefault.xctoolchain
        
        local IOS_ARCHS="i386 x86_64 armv7 armv7s arm64"
        #local IOS_ARCHS="arm64" # for future arm64
        local STDLIB="libc++"
        local CURRENTPATH=`pwd`

        # Validate environment
        case $XCODE_DEV_ROOT in
            *\ * )
                echo "Your Xcode path contains whitespaces, which is not supported."
                exit 1
                ;;
        esac
        case $CURRENTPATH in
            *\ * )
                echo "Your path contains whitespaces, which is not supported by 'make install'."
                exit 1
                ;;
        esac

        # loop through architectures! yay for loops!
        for IOS_ARCH in ${IOS_ARCHS}
        do
            export ARCH=$IOS_ARCH
            
            local EXTRA_PLATFORM_CFLAGS="" # will add -fvisibility=hidden $(INCLUDE) to makefile
			export EXTRA_PLATFORM_LDFLAGS=""
			if [[ "${IOS_ARCH}" == "i386" || "${IOS_ARCH}" == "x86_64" ]];
			then
				export IOS_PLATFORM="iPhoneSimulator"
				#EXTRA_PLATFORM_CFLAGS=""
				#EXTRA_PLATFORM_LDFLAGS=""
			else
				#EXTRA_PLATFORM_CFLAGS=""
				#EXTRA_PLATFORM_LDFLAGS=""
				export IOS_PLATFORM="iPhoneOS"
			fi

			export IOS_DEVROOT=$XCODE_DEV_ROOT/Platforms/$IOS_PLATFORM.platform/Developer
			export IOS_SDKROOT=$IOS_DEVROOT/SDKs/$IOS_PLATFORM$IOS_SDK_VER.sdk
			export IOS_CC=$TOOLCHAIN/usr/bin/clang

			#export TARGET_NAME="build/$TYPE/$IOS_ARCH/libfreeimage.a"
			export TARGET_NAME="libfreeimage-$IOS_ARCH.a"
			export HEADER="Source/FreeImage.h"

			echo "Building FreeType VERISON:$VER for PLATFORM:$IOS_PLATFORM SDK:$IOS_SDK_VER ARCH:$IOS_ARCH"

			export CPP=$TOOLCHAIN/usr/bin/clang++
			export CXX=$TOOLCHAIN/usr/bin/c++
			export CXXCPP=$TOOLCHAIN/usr/bin/cpp
	
			export LD=$TOOLCHAIN/usr/bin/ld
			export AR=$TOOLCHAIN/usr/bin/ar
			export AS=$TOOLCHAIN/usr/bin/as
			export NM=$TOOLCHAIN/usr/bin/nm
			export RANLIB=$TOOLCHAIN/usr/bin/ranlib
			export LIBTOOL=$TOOLCHAIN/usr/bin/libtool

			local MIN_IOS_VERSION=$IOS_MIN_SDK_VER
		    # min iOS version for arm64 is iOS 7

		    if [ "$IOS_ARCH" == "arm64" ] ; then
		    	MIN_IOS_VERSION="7.0"

		    	# Manually Fix major issues with arm64 for iOS from some source libraries.
		    	cp -v Source/ZLib/gzguts.h Source/ZLib/gzguts.h.orig
		    	#define LSEEK errors fixed by definig unistd for ZLib
		    	sed -i temp '20i\
					#include <unistd.h>' Source/ZLib/gzguts.h

				cp -v Source/LibJXR/image/decode/segdec.c Source/LibJXR/image/decode/segdec.c.orig
				
		    	sed -e 's/#if defined(_M_IA64) || defined(_ARM_)/#if defined(_M_IA64) || defined(_ARM_) || defined(__ARMEL__) || defined(_M_ARM) || defined(__arm__) || defined(__arm64__)/g' Source/LibJXR/image/decode/segdec.c > Source/LibJXR/image/decode/segdec.c

		    	cp -v Source/LibJXR/image/sys/xplatform_image.h Source/LibJXR/image/sys/xplatform_image.h.orig
		    	sed -e 's/#if defined(_ARM_) || defined(UNDER_CE)/#if defined(_ARM_) || defined(UNDER_CE) || defined(__ARMEL__) || defined(_M_ARM) || defined(__arm__) || defined(__arm64__)/g' Source/LibJXR/image/decode/segdec.c > Source/LibJXR/image/decode/segdec.c

		    	cp -v Source/LibJXR/jxrgluelib/JXRGlueJxr.c Source/LibJXR/jxrgluelib/JXRGlueJxr.c.orig

		    	sed -i temp '31i\
					#include <wchar.h>' Source/LibJXR/./jxrgluelib/JXRGlueJxr.c

		    fi

		    export EXTRA_PLATFORM_CFLAGS="$EXTRA_PLATFORM_CFLAGS -mdynamic-no-pic" # will add -fvisibility=hidden $(INCLUDE) to makefile
			export EXTRA_PLATFORM_LDFLAGS="$EXTRA_PLATFORM_LDFLAGS -isysroot $IOS_SDKROOT -Wl,-dead_strip"
			export CC="$IOS_CC"
			export CFLAGS="-arch $IOS_ARCH -O2 -Wall -Wmissing-prototypes -pipe $EXTRA_PLATFORM_CFLAGS -stdlib=$STDLIB -Wno-ctor-dtor-privacy -Wc++11-narrowing -ffast-math -fno-strict-aliasing -fmessage-length=0 -fexceptions -DNO_LCMS -D__ANSI__ -DDISABLE_PERF_MEASUREMENT -fvisibility=hidden -miphoneos-version-min=$MIN_IOS_VERSION -isysroot $IOS_SDKROOT"
			export CXXFLAGS="-fvisibility-inlines-hidden"
			export LDFLAGS="-arch $IOS_ARCH $EXTRA_PLATFORM_LDFLAGS -miphoneos-version-min=$MIN_IOS_VERSION"
			export LDFLAGS_PHONE=$LDFLAGS
			
			# run makefile
			make -f Makefile.ios
      
            unset ARCH IOS_DEVROOT IOS_SDKROOT IOS_CC TARGET_NAME HEADER
            unset CC CPP CXX CXXCPP CFLAGS CXXFLAGS LDFLAGS LD AR AS NM RANLIB LIBTOOL 
            unset EXTRA_PLATFORM_CFLAGS EXTRA_PLATFORM_LDFLAGS IOS_PLATFORM

            if [ "$IOS_ARCH" == "arm64" ] ; then
		    
		    	# reset back to originals
		    	cp -v Source/ZLib/gzguts.h.orig Source/ZLib/gzguts.h
				cp -v Source/LibJXR/image/decode/segdec.c.orig Source/LibJXR/image/decode/segdec.c
				cp -v Source/LibJXR/image/sys/xplatform_image.h.orig Source/LibJXR/image/sys/xplatform_image.h
				cp -v Source/LibJXR/jxrgluelib/JXRGlueJxr.c.orig Source/LibJXR/jxrgluelib/JXRGlueJxr.c

		    fi
     	

     		echo "Completed Build for $IOS_ARCH of FreeType"

     		# Strip debug symbols! for size and speed!
     		strip -x Dist/$TYPE/libfreeimage-$IOS_ARCH.a

		done

		echo "Completed Build for $TYPE"


		cd Dist/$TYPE/
		# stripping the lib prefix to bypass any issues with existing sdk libraries
		echo "Creating Fat Lib for crypto"
		lipo -create libfreeimage-armv7.a \
					libfreeimage-armv7s.a \
					libfreeimage-arm64.a \
					libfreeimage-i386.a \
					libfreeimage-x86_64.a \
					-output freeimage.a
		# validate all stripped debug:
		strip -x freeimage.a
		cd ../../

		# copy includes
		echo "Copying includes"

		# include copied in the makefile to libs/$TYPE/include

		unset TOOLCHAIN

	elif [ "$TYPE" == "android" ] ; then
		echoWarning "TODO: android build"
	fi
}

# executed inside the lib src dir, first arg $1 is the dest libs dir root
function copy() {
	
	# headers
	mkdir -p $1/include
	
	cp -v Dist/*.h $1/include
	

	# lib
	if [ "$TYPE" == "osx" ] ; then
		mkdir -p $1/lib/$TYPE
		cp -v Dist/libfreeimage.a $1/lib/$TYPE/freeimage.a
	elif [ "$TYPE" == "vs" -o "$TYPE" == "win_cb" ] ; then
		mkdir -p $1/lib/$TYPE
		cp -v Dist/FreeImage.lib $1/lib/$TYPE/FreeImage.lib
		cp -v Dist/FreeImage.dll $1/../../export/$TYPE/FreeImage.dll
	elif [ "$TYPE" == "ios" ] ; then
		mkdir -p $1/lib/$TYPE
		cp -v Dist/$TYPE/freeimage.a $1/lib/$TYPE/freeimage.a

	elif [ "$TYPE" == "android" ] ; then
		echoWarning "TODO: copy android lib"
	fi	
}

# executed inside the lib src dir
function clean() {
	
	if [ "$TYPE" == "android" ] ; then
		echoWarning "TODO: clean android"
	elif [ "$TYPE" == "ios" ] ; then
		# clean up compiled libraries
		
		make clean
		rm -rf Dist
		#clean
		
	else
		make clean
		# run dedicated clean script
		clean.sh
	fi

}
