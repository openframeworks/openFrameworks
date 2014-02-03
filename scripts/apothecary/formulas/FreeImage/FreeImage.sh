#! /bin/bash
#
# Free Image
# cross platform image io
# http://freeimage.sourceforge.net
#
# Makefile build system, 
# some Makefiles are out of date so patching/modification may be required

FORMULA_TYPES=( "osx" "osx-clang-libc++" "vs2010" "win_cb" "ios" "android" )

VER=3154 # 3.15.4

# download the source code and unpack it into LIB_NAME
function download() {
	curl -LO http://downloads.sourceforge.net/freeimage/FreeImage"$VER".zip
	unzip -qo FreeImage"$VER".zip
	rm FreeImage"$VER".zip
}

# executed inside the lib src dir
function build() {
	
	if [ "$TYPE" == "osx" ] ; then
		
		# patch outdated Makefile.osx,
		# use "# patched" string to determine if patch was applied
		if grep -Fq "# patched" Makefile.osx ; then
			: # noop, skip if patch was already applied
		else
			patch -p1 -u < $FORMULA_DIR/Makefile.osx.patch
		fi

		# @tgfrerer patch FreeImage source - clang is much less forgiving about
		# type overruns and missing standard header files. 
		# this patch replicates gcc's behaviour
		patch -p1 -u  < $FORMULA_DIR/freeimage.clang.source.patch

		# set SDK
		sed -i tmp "s|MACOSX_SDK =.*|MACOSX_SDK = $OSX_SDK_VER|" Makefile.osx
		sed -i tmp "s|MACOSX_MIN_SDK =.*|MACOSX_MIN_SDK = $OSX_MIN_SDK_VER|" Makefile.osx

		make -f Makefile.osx

	elif [ "$TYPE" == "osx-clang-libc++" ] ; then

		# patch Makefile.osx,
		# use "# patched" string to determine if patch was applied
		if grep -Fq "# patched" Makefile.osx ; then
			: # noop, skip if patch was already applied
		else
			patch -p1 -u < $FORMULA_DIR/Makefile.osx-clang-libc++.patch
		fi

		
		# @tgfrerer patch FreeImage source - clang is much less forgiving about
		# type overruns and missing standard header files. 
		# this patch replicates gcc's behaviour
		patch -p1 -u  < $FORMULA_DIR/freeimage.clang.source.patch

		# set SDK
		sed -i tmp "s|MACOSX_SDK =.*|MACOSX_SDK = $OSX_SDK_VER|" Makefile.osx
		sed -i tmp "s|MACOSX_MIN_SDK =.*|MACOSX_MIN_SDK = $OSX_MIN_SDK_VER|" Makefile.osx

		make -j -f Makefile.osx
	
	elif [ "$TYPE" == "vs2010" ] ; then
		#MSBuild.exe FreeImage.2008.sln
		echoWarning "TODO: vs2010 build"

	elif [ "$TYPE" == "win_cb" ] ; then
		#make -f Makefile.minigw
		echoWarning "TODO: win_cb build"

	elif [ "$TYPE" == "ios" ] ; then

		# patch outdated Makefile.iphone to build universal sim/armv7/armv7s lib,
		# use "# patched" string to determine if patch was applied
		if grep -Fq "# patched" Makefile.iphone ; then
			: # noop, skip if patch was already applied
		else
			patch -p1 -u < $FORMULA_DIR/Makefile.iphone.patch
		fi

		# @tgfrerer patch FreeImage source - clang is much less forgiving about
		# type overruns and missing standard header files. 
		# this patch replicates gcc's behaviour
		patch -p1 -u  < $FORMULA_DIR/freeimage.clang.source.patch

		# set SDKs
		sed -i tmp "s|MACOSX_DEPLOYMENT_TARGET =.*|MACOSX_DEPLOYMENT_TARGET = $OSX_SDK_VER|" Makefile.iphone
		sed -i tmp "s|MACOSX_MIN_SDK =.*|MACOSX_MIN_SDK = $OSX_MIN_SDK_VER|" Makefile.iphone
		sed -i tmp "s|IPHONEOS_SDK =.*|IPHONEOS_SDK = $IOS_SDK_VER|" Makefile.iphone
		sed -i tmp "s|IPHONEOS_MIN_SDK =.*|IPHONEOS_MIN_SDK = $IOS_MIN_SDK_VER|" Makefile.iphone

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
	if [ "$TYPE" == "osx" -o "osx-clang-libc++" ] ; then
		mkdir -p $1/lib/$TYPE
		cp -v Dist/libfreeimage.a $1/lib/$TYPE/freeimage.a

	elif [ "$TYPE" == "vs2010" -o  "$TYPE" == "win_cb" ] ; then
		mkdir -p $1/lib/$TYPE
		cp -v Dist/libfreeimage.lib $1/lib/$TYPE/freeimage.lib

	elif [ "$TYPE" == "ios" ] ; then
		mkdir -p $1/lib/$TYPE
		cp -v Dist/libfreeimage-ios.a $1/lib/$TYPE/freeimage.a

	elif [ "$TYPE" == "android" ] ; then
		echoWarning "TODO: copy android lib"
	fi	
}

# executed inside the lib src dir
function clean() {
	
	if [ "$TYPE" == "vs2010" ] ; then
		echoWarning "TODO: clean vs2010"

	elif [ "$TYPE" == "android" ] ; then
		echoWarning "TODO: clean android"
		
	else
		make clean
	fi
}
