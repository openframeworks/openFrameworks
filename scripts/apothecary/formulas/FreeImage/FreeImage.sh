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

		# patch outdated Makefile.iphone to build universal sim/armv7/armv7s lib
		if patch -p1 -u -N --dry-run --silent < $FORMULA_DIR/Makefile.iphone.patch 2>/dev/null ; then
			patch -p1 -u < $FORMULA_DIR/Makefile.iphone.patch
		fi

		# @tgfrerer patch FreeImage source - clang is much less forgiving about
		# type overruns and missing standard header files. 
		# this patch replicates gcc's behaviour
		if patch -p1 -u -N --dry-run --silent < $FORMULA_DIR/freeimage.clang.source.patch 2>/dev/null ; then
			patch -p1 -u -N  < $FORMULA_DIR/freeimage.clang.source.patch
		fi

		# set SDKs
		sed -i tmp "s|MACOSX_DEPLOYMENT_TARGET =.*|MACOSX_DEPLOYMENT_TARGET = $OSX_SDK_VER|" Makefile.iphone
		sed -i tmp "s|MACOSX_MIN_SDK =.*|MACOSX_MIN_SDK = $OSX_MIN_SDK_VER|" Makefile.iphone
		sed -i tmp "s|IPHONEOS_SDK =.*|IPHONEOS_SDK = $IOS_SDK_VER|" Makefile.iphone
		sed -i tmp "s|IPHONEOS_MIN_SDK =.*|IPHONEOS_MIN_SDK = $IOS_MIN_SDK_VER|" Makefile.iphone
	fi
}

# executed inside the lib src dir
function build() {
	
	if [ "$TYPE" == "osx" ] ; then
		make -f Makefile.osx

	elif [ "$TYPE" == "ios" ] ; then

		# armv7 (+ simulator)
		sed -i tmp "s|ARCH_PHONE =.*|ARCH_PHONE = armv7|" Makefile.iphone
		make -f Makefile.iphone

		# armv7s
		sed -i tmp "s|ARCH_PHONE =.*|ARCH_PHONE = armv7s|" Makefile.iphone
		make -f Makefile.iphone

		# link into universal lib
		lipo -c Dist/libfreeimage-simulator.a Dist/libfreeimage-armv7.a Dist/libfreeimage-armv7s.a -o Dist/libfreeimage-ios.a

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
		cp -v Dist/libfreeimage-ios.a $1/lib/$TYPE/freeimage.a

	elif [ "$TYPE" == "android" ] ; then
		echoWarning "TODO: copy android lib"
	fi	
}

# executed inside the lib src dir
function clean() {
	
	if [ "$TYPE" == "android" ] ; then
		echoWarning "TODO: clean android"
		
	else
		make clean
		# run dedicated clean script
		clean.sh
	fi

}
