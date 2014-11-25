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
GIT_URL=https://github.com/danoli3/FreeImage
GIT_TAG=3.16.0

# download the source code and unpack it into LIB_NAME
function download() {
	if [ "$TYPE" == "vs" -o "$TYPE" == "win_cb" ] ; then
		# For win32, we simply download the pre-compiled binaries.
		curl -LO http://downloads.sourceforge.net/freeimage/FreeImage"$VER"Win32.zip
		unzip -qo FreeImage"$VER"Win32.zip
		rm FreeImage"$VER"Win32.zip
	elif [[ "${TYPE}" == "osx" || "${TYPE}" == "ios" ]]; then
        # Fixed issues for OSX / iOS for FreeImage compiling in git repo.
        echo "Downloading from $GIT_URL for OSX/iOS"
		echo $GIT_URL
		curl -Lk $GIT_URL/archive/$GIT_TAG.tar.gz -o FreeImage-$GIT_TAG.tar.gz
		tar -xf FreeImage-$GIT_TAG.tar.gz
		mv FreeImage-$GIT_TAG FreeImage
		rm FreeImage-$GIT_TAG.tar.gz
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
		mkdir -p builddir/$TYPE

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
		export TOOLCHAIN=$XCODE_DEV_ROOT/Toolchains/XcodeDefault.xctoolchain
		export TARGET_IOS
        
        local IOS_ARCHS="i386 x86_64 armv7 armv7s arm64"
        local STDLIB="libc++"
        local CURRENTPATH=`pwd`

		SDKVERSION=`xcrun -sdk iphoneos --show-sdk-version`	
        DEVELOPER=$XCODE_DEV_ROOT
		TOOLCHAIN=${DEVELOPER}/Toolchains/XcodeDefault.xctoolchain
		VERSION=$VER

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

        mkdir -p "builddir/$TYPE"

        # loop through architectures! yay for loops!
        for IOS_ARCH in ${IOS_ARCHS}
        do

        	unset ARCH IOS_DEVROOT IOS_SDKROOT IOS_CC TARGET_NAME HEADER
            unset CC CPP CXX CXXCPP CFLAGS CXXFLAGS LDFLAGS LD AR AS NM RANLIB LIBTOOL 
            unset EXTRA_PLATFORM_CFLAGS EXTRA_PLATFORM_LDFLAGS IOS_PLATFORM NO_LCMS

            export ARCH=$IOS_ARCH
            
            local EXTRA_PLATFORM_CFLAGS=""
			export EXTRA_PLATFORM_LDFLAGS=""
			if [[ "${IOS_ARCH}" == "i386" || "${IOS_ARCH}" == "x86_64" ]];
			then
				PLATFORM="iPhoneSimulator"
			
			else
				PLATFORM="iPhoneOS"
			fi

			export CROSS_TOP="${DEVELOPER}/Platforms/${PLATFORM}.platform/Developer"
			export CROSS_SDK="${PLATFORM}${SDKVERSION}.sdk"
			export BUILD_TOOLS="${DEVELOPER}"

			MIN_IOS_VERSION=$IOS_MIN_SDK_VER
		    # min iOS version for arm64 is iOS 7
		
		    if [[ "${IOS_ARCH}" == "arm64" || "${IOS_ARCH}" == "x86_64" ]]; then
		    	MIN_IOS_VERSION=7.0 # 7.0 as this is the minimum for these architectures
		    elif [ "${IOS_ARCH}" == "i386" ]; then
		    	MIN_IOS_VERSION=5.1 # 6.0 to prevent start linking errors
		    fi

		    MIN_TYPE=-miphoneos-version-min=
		    if [[ "${IOS_ARCH}" == "i386" || "${IOS_ARCH}" == "x86_64" ]]; then
		    	MIN_TYPE=-mios-simulator-version-min=
		    fi

			export TARGET_NAME="$CURRENTPATH/libfreeimage-$IOS_ARCH.a"
			export HEADER="Source/FreeImage.h"

			export CC=$TOOLCHAIN/usr/bin/clang
			export CPP=$TOOLCHAIN/usr/bin/clang++
			export CXX=$TOOLCHAIN/usr/bin/clang++
			export CXXCPP=$TOOLCHAIN/usr/bin/clang++
	
			export LD=$TOOLCHAIN/usr/bin/ld
			export AR=$TOOLCHAIN/usr/bin/ar
			export AS=$TOOLCHAIN/usr/bin/as
			export NM=$TOOLCHAIN/usr/bin/nm
			export RANLIB=$TOOLCHAIN/usr/bin/ranlib
			export LIBTOOL=$TOOLCHAIN/usr/bin/libtool


		  	export EXTRA_PLATFORM_CFLAGS="$EXTRA_PLATFORM_CFLAGS"
			export EXTRA_PLATFORM_LDFLAGS="$EXTRA_PLATFORM_LDFLAGS -isysroot ${CROSS_TOP}/SDKs/${CROSS_SDK} -Wl,-dead_strip -I${CROSS_TOP}/SDKs/${CROSS_SDK}/usr/include/ $MIN_TYPE$MIN_IOS_VERSION "

		   	EXTRA_LINK_FLAGS="-arch $IOS_ARCH -fmessage-length=0 -fdiagnostics-show-note-include-stack -fmacro-backtrace-limit=0 -Wno-trigraphs -fpascal-strings -Os -Wno-missing-field-initializers -Wno-missing-prototypes -Wno-return-type -Wno-non-virtual-dtor -Wno-overloaded-virtual -Wno-exit-time-destructors -Wno-missing-braces -Wparentheses -Wswitch -Wno-unused-function -Wno-unused-label -Wno-unused-parameter -Wno-unused-variable -Wunused-value -Wno-empty-body -Wno-uninitialized -Wno-unknown-pragmas -Wno-shadow -Wno-four-char-constants -Wno-conversion -Wno-constant-conversion -Wno-int-conversion -Wno-bool-conversion -Wno-enum-conversion -Wno-shorten-64-to-32 -Wno-newline-eof -Wno-c++11-extensions -DHAVE_UNISTD_H=1 -DOPJ_STATIC -DNO_LCMS -D__ANSI__ -DDISABLE_PERF_MEASUREMENT -DLIBRAW_NODLL -DLIBRAW_LIBRARY_BUILD -DFREEIMAGE_LIB -fexceptions -fasm-blocks -fstrict-aliasing -Wdeprecated-declarations -Winvalid-offsetof -Wno-sign-conversion -Wmost -Wno-four-char-constants -Wno-unknown-pragmas -DNDEBUG -fPIC -fexceptions -fvisibility=hidden"
			EXTRA_FLAGS="$EXTRA_LINK_FLAGS -ffast-math -DDISABLE_PERF_MEASUREMENT $MIN_TYPE$MIN_IOS_VERSION -isysroot ${CROSS_TOP}/SDKs/${CROSS_SDK} -I${CROSS_TOP}/SDKs/${CROSS_SDK}/usr/include/"

		    export CC="$CC $EXTRA_FLAGS"
			export CFLAGS="-arch $IOS_ARCH $EXTRA_FLAGS"
			export CXXFLAGS="$EXTRA_FLAGS -std=c++11 -stdlib=libc++"
			export LDFLAGS="-arch $IOS_ARCH $EXTRA_PLATFORM_LDFLAGS $EXTRA_LINK_FLAGS $MIN_TYPE$MIN_IOS_VERSION -std=c++11 -stdlib=libc++"
			export LDFLAGS_PHONE=$LDFLAGS

			mkdir -p "$CURRENTPATH/builddir/$TYPE/$IOS_ARCH"
			LOG="$CURRENTPATH/builddir/$TYPE/$IOS_ARCH/build-freeimage-${VER}-$IOS_ARCH.log"
			echo "-----------------"
			echo "Building FreeImage-${VER} for ${PLATFORM} ${SDKVERSION} ${IOS_ARCH} : iOS Minimum=$MIN_IOS_VERSION"
			set +e

			echo "Running make for ${IOS_ARCH}"
			echo "Please stand by..."

			
			# run makefile
			make -f Makefile.ios >> "${LOG}" 2>&1
			if [ $? != 0 ];
		    then 
		    	echo "Problem while make - Please check ${LOG}"
		    	exit 1
		    else
		    	echo "Make Successful for ${IOS_ARCH}"
		    fi
		    
     		echo "Completed Build for $IOS_ARCH of FreeImage"

     		mv -v libfreeimage-$IOS_ARCH.a Dist/$TYPE/libfreeimage-$IOS_ARCH.a

     		cp Source/FreeImage.h Dist

            unset ARCH IOS_DEVROOT IOS_SDKROOT IOS_CC TARGET_NAME HEADER
            unset CC CPP CXX CXXCPP CFLAGS CXXFLAGS LDFLAGS LD AR AS NM RANLIB LIBTOOL 
            unset EXTRA_PLATFORM_CFLAGS EXTRA_PLATFORM_LDFLAGS IOS_PLATFORM NO_LCMS

		done

		echo "Completed Build for $TYPE"

        echo "-----------------"
		echo `pwd`
		echo "Finished for all architectures."
		mkdir -p "$CURRENTPATH/builddir/$TYPE/$IOS_ARCH"
		LOG="$CURRENTPATH/builddir/$TYPE/build-freeimage-${VER}-lipo.log"


		cd Dist/$TYPE/
		# link into universal lib
		echo "Running lipo to create fat lib"
		echo "Please stand by..."
		lipo -create libfreeimage-armv7.a \
					libfreeimage-armv7s.a \
					libfreeimage-arm64.a \
					libfreeimage-i386.a \
					libfreeimage-x86_64.a \
					-output freeimage.a >> "${LOG}" 2>&1


		if [ $? != 0 ];
		then 
		    echo "Problem while creating fat lib with lipo - Please check ${LOG}"
		    exit 1
		else
		   	echo "Lipo Successful."
		fi

		lipo -info freeimage.a
		echo "--------------------"
		echo "Stripping any lingering symbols"
		echo "Please stand by..."
		# validate all stripped debug:
		strip -x freeimage.a  >> "${LOG}" 2>&1
		if [ $? != 0 ];
		then 
		    echo "Problem while stripping lib - Please check ${LOG}"
		    exit 1
		else
		    echo "Strip Successful for ${LOG}"
		fi
		cd ../../

		echo "--------------------"
		echo "Build Successful for FreeImage $TYPE $VER"

		# include copied in the makefile to libs/$TYPE/include
		unset TARGET_IOS
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
		rm -f *.a *.lib
		rm -f builddir/$TYPE
		rm -f builddir
		rm -f lib		
	else
		make clean
		# run dedicated clean script
		clean.sh
	fi

}
